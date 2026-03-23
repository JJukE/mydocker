Docker Container Setting for RTX 4090 Workstation
---

Workstation environment:
* Driver: 550.142

#### 1. Clone Isaac Lab container repository, pull and build it
```bash
git clone https://github.com/isaac-sim/IsaacLab.git
cd IsaacLab
sudo chown -R sangjune:sangjune .

python docker/container.py start
# X11 forwarding enable? --> y
```

Check image and container with `docker images` and `docker ps -a`, e.g.,

image:
* isaac-lab-base:latest
* 17.6GB

container:
* isaac-lab-base

#### 2. Enter the container and run an example
```bash
python docker/container.py enter
# or just 'attach to running container' in the vscode

cd /workspace/isaaclab

isaaclab -p scripts/tutorials/00_sim/log_time.py --headless # without X11 forwarindg
```

> when vulkan-related error occurs, GPU and driver compatability can be checked with this [document](https://docs.omniverse.nvidia.com/dev-guide/latest/linux-troubleshooting.html#q3-how-to-verify-a-correct-vulkan-setup-with-vulkaninfo-or-vulkaninfosdk-utility)

To check the compatibility, execute in the container:
```bash
cd /workspace
wget [link_for_latest_release_for_linux] # 1.4.341.1 in 2026.03.19.
tar -xvf vulkansdk-linux-x86_64-[version].tar.xz

cd [version]/x86_64
bin/vulkaninfo
```

However, in my case, vulkan 1.1 is not supported, so GPU cannot be found.
So re-install the vulkan:
```bash
cd /workspace/[version]
# remove 'sudo' because we're installing vulkan in the container
find vulkansdk -type f -exec sed -i 's/sudo //g' {} +

./vulkansdk
```

and successfully 