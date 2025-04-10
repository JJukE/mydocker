# for x11 forwarding, run this in the workstation!
# 1. check `$DISPLAY` of host
# 2. run docker_run_x11.sh in the workstation, not ssh
# 3. check if `$DISPLAY` of host and container are same
# 4. If different, modify docker_run_x11.sh `-e DISPLAY=...` part so that the values are same
docker run --gpus all -id \
--security-opt=no-new-privileges \
--name $CONT_NAME \
-v $VOL_RVI \
-v $VOL_MINE \
-v $VOL_RVI2 \
-v $VOL_MINE2 \
-v $VOL_DEV \
-v $VOL_DATA \
-v $VOL_HDD \
-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
-e DISPLAY=$DISPLAY \
--ipc host \
--restart unless-stopped \
-p $PORT_TB:6006 \
-p $PORT_SSH:22 \
-p $PORT_JUP:8888 \
-p $PORT_JUP_:8889 \
$IMG_NAME
