docker run --gpus all -id \
--security-opt=no-new-privileges \
--name sangjune118 \
-v /data/rvi/dataset:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /home/sangjune/dev:/root/dev \
--ipc host \
--restart unless-stopped \
-p 48206:6006 \
-p 48222:22 \
-p 48288:8888 \
-p 48289:8889 \
sangjune:11.8.0
