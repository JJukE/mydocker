#!/bin/bash

# Allow local X11 connections for GUI forwarding
# 1. check `$DISPLAY` of host (if blank, set with `export DISPLAY=:0.0`)
# 2. run the docker_run.sh in the workstation
# 3. check if `$DISPLAY` of host and container are same
# 4. If different, modify docker_run.sh `-e DISPLAY=` part so that 
xhost +

docker run --gpus all -id \
--security-opt=no-new-privileges \
--name $CONT_NAME \
-e NVIDIA_VISIBLE_DEVICES=all \
-e NVIDIA_DRIVER_CAPABILITIES=all \
-e "ACCEPT_EULA=Y" \
-e "PRIVACY_CONSENT=Y" \
-e DISPLAY=$DISPLAY \
-v /usr/share/vulkan/icd.d:/usr/share/vulkan/icd.d:ro \
-v /etc/vulkan/icd.d:/etc/vulkan/icd.d:ro \
-v /usr/share/nvidia:/usr/share/nvidia:ro \
-v /tmp/.X11-unix:/tmp/.X11-unix:rw \
-v $HOME/.Xauthority:/root/.Xauthority:rw \
-v ~/docker/isaac-sim/cache/kit:/isaac-sim/kit/cache:rw \
-v ~/docker/isaac-sim/cache/ov:/root/.cache/ov:rw \
-v ~/docker/isaac-sim/cache/pip:/root/.cache/pip:rw \
-v ~/docker/isaac-sim/cache/glcache:/root/.cache/nvidia/GLCache:rw \
-v ~/docker/isaac-sim/cache/computecache:/root/.nv/ComputeCache:rw \
-v ~/docker/isaac-sim/logs:/root/.nvidia-omniverse/logs:rw \
-v ~/docker/isaac-sim/data:/root/.local/share/ov/data:rw \
-v ~/docker/isaac-sim/documents:/root/Documents:rw \
-v $VOL_DATA \
-v $VOL_DATA2 \
-v $VOL_RVI \
-v $VOL_MINE \
-v $VOL_RVI2 \
-v $VOL_MINE2 \
-v $VOL_MINE3 \
-v $VOL_DEV \
--ipc host \
--restart unless-stopped \
-p $PORT_TB:6006 \
-p $PORT_SSH:22 \
-p $PORT_JUP:8888 \
-p $PORT_JUP_:8889 \
$IMG_NAME
