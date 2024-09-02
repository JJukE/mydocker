docker run --gpus all -id \
--security-opt=no-new-privileges \
--name sangjune113 \
-v /data/rvi/dataset:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /home/sangjune/dev:/root/dev \
--ipc host \
--restart unless-stopped \
-p 28206:6006 \
-p 28222:22 \
-p 28288:8888 \
-p 28289:8889 \
sangjune:11.3.0
