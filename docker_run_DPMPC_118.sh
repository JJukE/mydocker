docker run --gpus all -id \
--name DPMPC_118 \
--env DISPLAY=$DISPLAY --env="QT_X11_NO_MITSHM=1" \
-v /data/rvi/dataset/:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /home/sangjune/dev:/root/dev \
-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
--ipc host \
--restart unless-stopped \
-p 6006:6006 \
-p 2222:22 \
-p 8888:8888 \
sangjune:11.8.0