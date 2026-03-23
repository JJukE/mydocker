import carb
import asyncio
import argparse
import omni.usd
from omni.kit import viewport_legacy
from pxr import Sdf, Usd
import omni.graph.core as og

aovs = []
frameNo = 0
frameStart = 0
frameCount = 0
inflightFileIO = 0
saveFolder = ""
eventSubscription = None


def load_callback(result, err):
    if result:
        carb.settings.get_settings().set_bool("/rtx/directLighting/sampledLighting/enabled", False)
        asyncio.ensure_future(create_render_product())
    else:
        carb.log_error("Usd stage file load failed")
        omni.kit.app.get_app().post_quit(-1)


async def do_shutdown():
    global eventSubscription
    omni.usd.get_context().close_stage(None)
    omni.kit.app.get_app().post_quit(0)
    eventSubscription = None


def on_rendering_event(e: carb.events.IEvent):
    global frameNo
    global frameStart
    global frameCount
    frameNo = e.payload["frame_number"]
    if (frameNo > (frameStart + frameCount + 10)):
        asyncio.ensure_future(do_shutdown())


async def create_render_product():
    try:
        global frameNo
        global frameStart
        global frameCount
        global saveFolder
        global inflightFileIO
        global eventSubscription

        stage = omni.usd.get_context().get_stage()
        session_layer = stage.GetSessionLayer()

        viewportFactory = viewport_legacy.get_viewport_interface()
        defaultWindow = viewportFactory.get_viewport_window()
        await omni.usd.get_context().next_frame_async(defaultWindow.get_id())
        aov_list = defaultWindow.get_aov_list()
        for aovName in aov_list:
            carb.log_info(f"Available aov: {aovName}")

        with Usd.EditContext(stage, session_layer):
            omni.kit.commands.execute("CopyPrim", path_from="/Render/RenderProduct_Viewport", path_to="/Render/RenderProduct_Dupe")
            # eventSubscription = None
            # await omni.kit.app.get_app().next_update_async()

            # Based off kit\source\extensions\omni.graph.ui\python\scripts\menu.py
            orchestration_graphs = og.get_global_orchestration_graphs_in_pipeline_stage(og.GraphPipelineStage.GRAPH_PIPELINE_STAGE_POSTRENDER)
            orchestration_graph = orchestration_graphs[0]
            (result, wrapper_node) = og.cmds.CreateGraphAsNode(
                graph=orchestration_graph,
                node_name="Pipeline",
                graph_path="/Render/RenderProduct_Dupe/Pipeline",
                evaluator_name="push",
                is_global_graph=True,
                backed_by_usd=True,
                fc_backing_type=og.GraphBackingType.GRAPH_BACKING_TYPE_FABRIC_SHARED,
                pipeline_stage=og.GraphPipelineStage.GRAPH_PIPELINE_STAGE_POSTRENDER)
            # await omni.kit.app.get_app().next_update_async()

            wrapped_graph = wrapper_node.get_wrapped_graph()

            og.cmds.CreateNode(graph=wrapped_graph,
                               node_path="/Render/RenderProduct_Dupe/Pipeline/GpuInteropRenderProductEntry",
                               node_type="omni.graph.nodes.GpuInteropRenderProductEntry", create_usd=True)
            entry_prim_node = wrapped_graph.get_node("/Render/RenderProduct_Dupe/Pipeline/GpuInteropRenderProductEntry")
            entry_prim_rp_attr = entry_prim_node.get_attribute("outputs:rp")
            entry_prim_gpu_attr = entry_prim_node.get_attribute("outputs:gpu")

            for aov in aovs:
                gpu_to_cpu_node_path = "/Render/RenderProduct_Dupe/Pipeline/GpuInteropGpuToCpuCopy" + "_" + aov
                og.cmds.CreateNode(graph=wrapped_graph,
                                   node_path=gpu_to_cpu_node_path,
                                   node_type="omni.graph.examples.cpp.GpuInteropGpuToCpuCopy", create_usd=True)
                gpu_to_cpu_prim_node = wrapped_graph.get_node(gpu_to_cpu_node_path)
                gpu_to_cpu_prim_inputs_rp_attr = gpu_to_cpu_prim_node.get_attribute("inputs:rp")
                gpu_to_cpu_prim_inputs_gpu_attr = gpu_to_cpu_prim_node.get_attribute("inputs:gpu")
                og.cmds.ConnectAttrs(src_attr=entry_prim_rp_attr, dest_attr=gpu_to_cpu_prim_inputs_rp_attr, modify_usd=True)
                og.cmds.ConnectAttrs(src_attr=entry_prim_gpu_attr, dest_attr=gpu_to_cpu_prim_inputs_gpu_attr, modify_usd=True)
                gpu_to_cpu_prim_outputs_rp_attr = gpu_to_cpu_prim_node.get_attribute("outputs:rp")
                gpu_to_cpu_prim_outputs_gpu_attr = gpu_to_cpu_prim_node.get_attribute("outputs:gpu")
                gpu_to_cpu_prim_outputs_aovCpu_attr = gpu_to_cpu_prim_node.get_attribute("outputs:aovCpu")

                gpu_to_cpu_prim_inputs_aov_attr = gpu_to_cpu_prim_node.get_attribute("inputs:aovGpu")
                og.Controller.set(gpu_to_cpu_prim_inputs_aov_attr, str(aov))

                cpu_to_disk_node_path = "/Render/RenderProduct_Dupe/Pipeline/GpuInteropCpuToDisk" + "_" + aov
                og.cmds.CreateNode(graph=wrapped_graph,
                                   node_path=cpu_to_disk_node_path,
                                   node_type="omni.graph.examples.cpp.GpuInteropCpuToDisk", create_usd=True)
                cpu_to_disk_prim_node = wrapped_graph.get_node(cpu_to_disk_node_path)
                cpu_to_disk_prim_inputs_rp_attr = cpu_to_disk_prim_node.get_attribute("inputs:rp")
                cpu_to_disk_prim_inputs_gpu_attr = cpu_to_disk_prim_node.get_attribute("inputs:gpu")
                cpu_to_disk_prim_inputs_aovCpu_attr = cpu_to_disk_prim_node.get_attribute("inputs:aovCpu")
                og.cmds.ConnectAttrs(src_attr=gpu_to_cpu_prim_outputs_rp_attr, dest_attr=cpu_to_disk_prim_inputs_rp_attr, modify_usd=True)
                og.cmds.ConnectAttrs(src_attr=gpu_to_cpu_prim_outputs_gpu_attr, dest_attr=cpu_to_disk_prim_inputs_gpu_attr, modify_usd=True)
                og.cmds.ConnectAttrs(src_attr=gpu_to_cpu_prim_outputs_aovCpu_attr, dest_attr=cpu_to_disk_prim_inputs_aovCpu_attr, modify_usd=True)

                cpu_to_disk_prim_inputs_aov_attr = cpu_to_disk_prim_node.get_attribute("inputs:aovGpu")
                og.Controller.set(cpu_to_disk_prim_inputs_aov_attr, str(aov))

                cpu_to_disk_prim_inputs_startFrame_attr = cpu_to_disk_prim_node.get_attribute("inputs:startFrame")
                og.Controller.set(cpu_to_disk_prim_inputs_startFrame_attr, frameStart)

                cpu_to_disk_prim_inputs_frameCount_attr = cpu_to_disk_prim_node.get_attribute("inputs:frameCount")
                og.Controller.set(cpu_to_disk_prim_inputs_frameCount_attr, frameCount)

                cpu_to_disk_prim_inputs_saveLocation_attr = cpu_to_disk_prim_node.get_attribute("inputs:saveLocation")
                og.Controller.set(cpu_to_disk_prim_inputs_saveLocation_attr, saveFolder)

                cpu_to_disk_prim_inputs_fileType_attr = cpu_to_disk_prim_node.get_attribute("inputs:fileType")
                if aov == "HdrColor":
                    og.Controller.set(cpu_to_disk_prim_inputs_fileType_attr, "exr")
                else:
                    og.Controller.set(cpu_to_disk_prim_inputs_fileType_attr, "png")

                cpu_to_disk_prim_inputs_inflightFileIO_attr = cpu_to_disk_prim_node.get_attribute("inputs:maxInflightWrites")
                og.Controller.set(cpu_to_disk_prim_inputs_inflightFileIO_attr, inflightFileIO)

            prim = stage.GetPrimAtPath("/Render/RenderProduct_Dupe")
            prim.CreateAttribute("ogPostProcessPath", Sdf.ValueTypeNames.String).Set("/Render/RenderProduct_Dupe/Pipeline")

        defaultWindow.set_render_product_path("/Render/RenderProduct_Dupe")

        for aov in aovs:
            defaultWindow.add_aov(aov)

    except Exception as e:
        carb.log_error(str(e))
        omni.kit.app.get_app().post_quit(-1)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", help="Path to USD stage", required=True)
    parser.add_argument("--aovs", nargs='+', default=[], help="List of AOVs to capture (i.e LdrColor)", required=True)
    parser.add_argument("--frameCount", type=int, help="Number of frames to capture", required=True)
    parser.add_argument("--frameStart", type=int, help="Frame number when capture to disk begins", required=True)
    parser.add_argument("--saveFolder", help="Path to folder to save images", required=True)
    parser.add_argument("--inflightFileIO", type=int, help="Number of inflight file save operations per node", default=2)

    try:
        global aovs
        global frameStart
        global frameCount
        global saveFolder
        global inflightFileIO
        global eventSubscription

        options = parser.parse_args()
        context = omni.usd.get_context()
        aovs = options.aovs
        frameCount = options.frameCount
        frameStart = options.frameStart
        saveFolder = options.saveFolder
        inflightFileIO = options.inflightFileIO
        context.open_stage_with_callback(options.file, load_callback)
        eventSubscription = (
            context
            .get_rendering_event_stream()
            .create_subscription_to_push_by_type(
                int(omni.usd.StageRenderingEventType.NEW_FRAME),
                on_rendering_event,
                name="New frame subcription")
        )
    except Exception as e:
        carb.log_error(str(e))
        omni.kit.app.get_app().post_quit(-1)


if __name__ == "__main__":
    main()
