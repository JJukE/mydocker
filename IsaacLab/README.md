# General Process for Docker Container

General-purpose processing for (1) docker image pulling, (2) docker building, (3) docker container running.

### 1. Pull a docker image

```bash
sh docker_pull.sh
```

In the `Dockerfile`, specify the image name to the proper IsaacLab version you want at the [document](https://isaac-sim.github.io/IsaacLab/main/source/deployment/docker.html). `CUDA 12.8` will be installed via Dockerfile. You can modify the version you want.
- IMG_NVDA: Name of the docker image by NVIDIA
  - In my case, `export IMG_NVDA="nvcr.io/nvidia/isaac-lab:2.3.2"`

### 2. Build the image

```bash
sh docker_build.sh
```

Specify your image name and build the image using the nvidia image via [Dockerfile](./Dockerfile).
- IMG_NVDA: Name of the docker image by NVIDIA which you pulled above
- IMG_NAME: Name of your image to be built

### 3. Docker run

```bash
bash docker_run.sh
```

Specify your preference with the environment variables:
- CONT_NAME: Name of the container to be created
- VOL_DATA: Linking any path to dataset for training (maybe different to each server)
- VOL_RVI, VOL_RVI2: Linking the lab dataset folder of the storage server to the specificed container path
- VOL_MINE, VOL_MINE2: Linking your dataset folder of the storage server to the specificed container path
- VOL_DEV: Linking your dev folder of the node to the specificed container path
- PORT_TB: Port number of the Tensorboard (6006)
- PORT_SSH: Port number of the SSH (22)
- PORT_JUP: The first port number of the jupyter (8888)
- PORT_JUP_: The second port number of the jupyter (8889)
- IMG_NAME: Name of the image you built above

#### for non-GUI setting

```bash
bash docker_run_nonGUI.sh
```

If `docker: Error response from daemon: failed to create task for container: failed to create shim task: OCI runtime create failed: runc create failed: unable to start container process: error during container init: error mounting "[home_path]/.Xauthority" to rootfs at "/root/.Xauthority": mount [home_path]/.Xauthority:/root/.Xauthority (via /proc/self/fd/6), flags: 0x5000: not a directory: unknown: Are you trying to mount a directory onto a file (or vice-versa)? Check if the specified host path exists and is the expected type.` occurs because the docker image is configured in GUI-including workstation:
```bash
# ls -al [home_path]
# for example, if the home_path is `/home/sangjune/`,
rm -rf [home_path]/.Xauthority # ex) rm -rf /home/sangjune/.Xauthority
touch [home_path]/.Xauthority # ex) touch /home/sangjune/.Xauthority

# and rerun the docker
docker stop $CONT_NAME && docker rm $CONT_NAME
bash docker_run_nonGUI.sh
```


