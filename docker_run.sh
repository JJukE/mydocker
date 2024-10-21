docker run --gpus all -id \
--security-opt=no-new-privileges \
--name $CONT_NAME \
-v $VOL_RVI \
-v $VOL_MINE \
-v $VOL_DEV \
--ipc host \
--restart unless-stopped \
-p $PORT_TB:6006 \
-p $PORT_SSH:22 \
-p $PORT_JUP:8888 \
-p $PORT_JUP_:8889 \
$IMAGE_NAME