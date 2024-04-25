docker run --gpus all -id \
--name sangjune118 \
-v /data/rvi/dataset:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /mnt/hdd1:/root/hdd1 \
-v /mnt/hdd2:/root/hdd2 \
-v /home/sangjune/dev:/root/dev \
-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
-e DISPLAY=unix$DISPLAY \
--ipc host \
--restart unless-stopped \
-p 38206:6006 \
-p 38222:22 \
-p 38288:8888 \
-p 38289:8889 \
f1.unist.info:443/jjuke_118:0.0.3