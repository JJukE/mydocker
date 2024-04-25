docker run --gpus all -id \
--name sangjune113 \
-v /data/rvi/dataset:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /home/sangjune/dev:/root/dev \
-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
-e DISPLAY=unix$DISPLAY \
--ipc host \
--restart unless-stopped \
-p 28206:6006 \
-p 28222:22 \
-p 28288:8888 \
-p 28289:8889 \
f1.unist.info:443/jjuke_113:0.0.3
