docker run --gpus all -id \
--security-opt=no-new-privileges \
--name sangjune116 \
-v /data/rvi/dataset:/root/dataset \
-v /data/sangjune:/root/dataset_sj \
-v /home/sangjune/dev:/root/dev \
--ipc host \
--restart unless-stopped \
-p 28606:6006 \
-p 28622:22 \
-p 28688:8888 \
-p 28689:8889 \
sangjune:11.6.2
