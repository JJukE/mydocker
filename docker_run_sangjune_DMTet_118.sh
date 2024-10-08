docker run --gpus all -id \
--security-opt=no-new-privileges \
--name sangjune_DMTet_118 \
-v /data/rvi/dataset/:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /home/sangjune/dev:/root/dev \
-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
-e DISPLAY=unix$DISPLAY \
--ipc host \
--restart unless-stopped \
-p 38206:6006 \
-p 38222:22 \
-p 38288:8888 \
-p 38289:8889 \
sangjune_dmtet:11.8.0
