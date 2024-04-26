docker run --gpus all -id \
--name sangjune_GET3D \
-v /data/rvi/dataset/:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /home/sangjune/dev:/root/dev \
-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
-e DISPLAY=unix$DISPLAY \
--ipc host \
--restart unless-stopped \
-p 8206:6006 \
-p 8222:22 \
-p 8288:8888 \
-p 8289:8889 \
get3d