# General Process for Docker Container

General-purpose processing for (1) docker image pulling, (2) docker building, (3) docker container running.

### 1. Pull a docker image

```bash
sh docker_pull.sh
```

Specify the image name to the proper CUDA and Ubuntu version you want at [dockerhub_nvidia](https://hub.docker.com/r/nvidia/cuda/tags).
- IMG_NVDA: Name of the docker image by NVIDIA

### 2. Build the image

```bash
sh docker_build.sh
```

Specify your image name and build the image using the nvidia image via [Dockerfile](./Dockerfile).
- IMG_NVDA: Name of the docker image by NVIDIA which you pulled above
- IMG_NAME: Name of your image to be built

### 3. Docker run

```bash
sh docker_run.sh
sh docker_run_x11.sh # if you use x11 forwarding
```

Specify your preference with the environment variables:
- CONT_NAME: Name of the container to be created
- VOL_DATA: Linking any path to dataset for training (maybe different to each server)
- VOL_RVI, VOL_RVI2: Linking the lab dataset folder of the storage server to the specificed container path
- VOL_MINE, VOL_MINE2: Linking your dataset folder of the storage server to the specificed container path
- VOL_MINE3: Linking your folder on file server 3 to the specified container path
- VOL_DEV: Linking your dev folder of the node to the specificed container path
- VOL_ETC (optional): An extra volume for one container, e.g. another member's folder; skipped when unset
- VOL_RVI3, VOL_RVI4 (optional): Linking the lab dataset folder on file server 3 or 4; skipped when unset
- VOL_MINE4 (optional): Linking your folder on file server 4; skipped when unset
- PORT_TB: Port number of the Tensorboard (6006)
- PORT_SSH: Port number of the SSH (22)
- PORT_JUP: The first port number of the jupyter (8888)
- PORT_JUP_: The second port number of the jupyter (8889)
- IMG_NAME: Name of the image you built above

#### Volume convention for the lab nodes

File servers 3 and 4 follow the same pattern as file servers 1 and 2: the lab dataset folder maps
to `/root/datasetN` and your own folder maps to `/root/dataset_sjN`.

```bash
export VOL_DATA=/home/sangjune/data:/root/data   # node-local disk; differs per node
export VOL_RVI=/data/rvi/dataset:/root/dataset
export VOL_MINE=/data/sangjune:/root/dataset_sj
export VOL_RVI2=/file2/dataset:/root/dataset2
export VOL_MINE2=/file2/sangjune:/root/dataset_sj2
export VOL_MINE3=/file3/sangjune:/root/dataset_sj3
export VOL_DEV=/home/sangjune/dev:/root/dev

# Optional: export only where the host path is available on that node
export VOL_RVI3=/file3/dataset:/root/dataset3
export VOL_MINE4=/file4/sangjune:/root/dataset_sj4
export VOL_RVI4=/file4/dataset:/root/dataset4
export VOL_ETC=/file3/jaehyeok:/root/dataset_etc   # per container
```

When to set the optional variables:
- Set `VOL_RVI3` and `VOL_RVI4` only when `/file3/dataset` or `/file4/dataset` exists on the node
  (`ls -d <path>`). Otherwise leave them unset.
- Set `VOL_MINE4` on any node where `/file4` is mounted (`df -h /file4` shows the NFS export),
  even if `/file4/sangjune` does not exist yet. On the first run Docker creates the missing bind
  source as `root`; this is how other members' `/file4/<name>` folders appear. The folder is then
  writable as root from inside the container, but host-side writes as your own user may need the
  ownership fixed first.
- Docker creates any missing bind source the same way, so a wrong path or a server that is not
  mounted on that node leaves an empty root-owned folder rather than an error. Check the host path
  before exporting a variable.
- Leave the optional file server variables unset on a machine without those mounts, such as a
  workstation outside the lab network.

