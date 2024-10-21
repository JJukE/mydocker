# General Process for Docker Container

General-purpose processing for (1) docker image pulling, (2) docker building, (3) docker container running.

1. Pull a docker image

```bash
sh docker_pull.sh
```

Specify the image name to the proper CUDA and Ubuntu version you want at [dockerhub_nvidia](https://hub.docker.com/r/nvidia/cuda/tags).
- IMG_NVDA: Name of the NVIDIA image

2. Build the image

```bash
sh docker_build.sh
```

Specify your image name and build the image using the nvidia image via [Dockerfile](./Dockerfile).
- IMG_NVDA: Name of the NVIDIA image which you pulled above
- IMG_NAME: Name of your image to be built

3. Docker run

```bash
sh docker_run.sh
sh docker_run_x11.sh # if you use x11 forwarding
```

Specify your preference with the environment variables:
- CONT_NAME: Name of the container to be created
- VOL_RVI: Linking the lab dataset folder of the storage server to the specificed container path
- VOL_MINE: Linking your dataset folder of the storage server to the specificed container path
- VOL_DEV: Linking your dev folder of the node to the specificed container path
- PORT_TB: Port number of the Tensorboard (6006)
- PORT_SSH: Port number of the SSH (22)
- PORT_JUP: The first port number of the jupyter (8888)
- PORT_JUP_: The second port number of the jupyter (8889)
- IMG_NAME: Name of the image

