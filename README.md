# General Process for Docker Container

General-purpose processing for (1) docker image pulling, (2) docker building, (3) docker container running.

1. Pull a docker image

```bash
sh 1_docker_pull.sh
```

Modify the image name of the file with proper CUDA and Ubuntu version at [dockerhub_nvidia](https://hub.docker.com/r/nvidia/cuda/tags).

2. Build the image

```bash
sh 2_docker_build.sh
```

Modify your image name to be built and the first line of the [Dockerfile](./Dockerfile) to the image you pulled above.

3. Docker run

```bash
sh 3_docker_run.sh
sh 3_docker_run_x11.sh # if you use x11 forwarding
```

Modify your preference with the environment variables:
- CONT_NAME: Name of the container to be created
- VOL_RVI: Linking the lab dataset folder of the storage server to the specificed container path
- VOL_MINE: Linking your dataset folder of the storage server to the specificed container path
- VOL_DEV: Linking your dev folder of the node to the specificed container path
- PORT_TB: Port number of the Tensorboard (6006)
- PORT_SSH: Port number of the SSH (22)
- PORT_JUP: The first port number of the jupyter (8888)
- PORT_JUP_: The second port number of the jupyter (8889)
- IMAGE_NAME: Name of the image

