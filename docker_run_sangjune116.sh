docker run --gpus all -id \
--name sangjune116 \
-v /data/rvi/dataset:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /mnt/hdd1:/root/hdd1 \
-v /home/sangjune/dev:/root/dev \
-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
-e DISPLAY=unix$DISPLAY \
--ipc host \
--restart unless-stopped \
-p 28606:6006 \
-p 28622:22 \
-p 28688:8888 \
-p 28689:8889 \
sangjune:11.6.2