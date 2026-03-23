# Temp WAR for inability to move carb.imgui:
# carb namespace is not a namespace, but a module. Which doesn't allow us to move carb.imgui out of kernel completely.
# For backward compatibility keep that file here.
import omni.kit.app
omni.kit.app.log_deprecation("carb.imgui is deprecated. Use omni.kit.imgui instead.")
from omni.kit.imgui import *
