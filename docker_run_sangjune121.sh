docker run --gpus all -id \
--security-opt=no-new-privileges \
--name sangjune121 \
-v /data/rvi/dataset:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /home/sangjune/dev:/root/dev \
--ipc host \
--restart unless-stopped \
-p 48201:6006 \
-p 48202:22 \
-p 48208:8888 \
-p 48209:8889 \
sangjune:12.1.0
