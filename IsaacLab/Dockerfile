# Target the pre-built Isaac Lab container
ARG IMG_NVDA="nvcr.io/nvidia/isaac-lab:2.3.2"
FROM ${IMG_NVDA}

ENV TZ=Asia/Seoul
# Switch to root to ensure permission for apt installations
USER root

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y --no-install-recommends \
    nano tmux wget iputils-ping git g++ build-essential net-tools curl zip unzip ffmpeg \
    openssh-server libpam-pwquality bash-completion \
    libhidapi-hidraw0 libhidapi-libusb0 \
    bzip2 ca-certificates libglib2.0-0 libsm6 libxext6 libxrender1 mercurial openssh-client procps subversion && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# CUDA 12.8 installation
RUN wget https://developer.download.nvidia.com/compute/cuda/12.8.0/local_installers/cuda_12.8.0_570.86.10_linux.run -O /tmp/cuda.run && \
    sh /tmp/cuda.run --silent --toolkit && \
    rm /tmp/cuda.run
ENV CUDA_HOME=/usr/local/cuda-12.8
ENV PATH=${CUDA_HOME}/bin:${PATH}
ENV LD_LIBRARY_PATH=${CUDA_HOME}/lib64:${LD_LIBRARY_PATH}

# miniconda (https://github.com/ContinuumIO/docker-images/blob/master/miniconda3/debian/Dockerfile)
ENV PATH /opt/conda/bin:$PATH
# set version(Filename) and proper sha256(SHA256) from https://repo.anaconda.com/miniconda/
ARG CONDA_VERSION=py311_26.1.1-1
RUN set -x && \
    UNAME_M="$(uname -m)" && \
    if [ "${UNAME_M}" = "x86_64" ]; then \
    MINICONDA_URL="https://repo.anaconda.com/miniconda/Miniconda3-${CONDA_VERSION}-Linux-x86_64.sh"; \
    SHA256SUM="52d1f19154b0716d7dc0872f0d858702640da08a4e53fd0035ba988608203d6b"; \
    elif [ "${UNAME_M}" = "aarch64" ]; then \
    MINICONDA_URL="https://repo.anaconda.com/miniconda/Miniconda3-${CONDA_VERSION}-Linux-aarch64.sh"; \
    SHA256SUM="1726ea55308160a32e82d90a6f4914caa46f58fe157442224f2faf92fdef1e0e"; \
    fi && \
    wget "${MINICONDA_URL}" -O miniconda.sh -q && \
    echo "${SHA256SUM} miniconda.sh" > shasum && \
    if [ "${CONDA_VERSION}" != "latest" ]; then sha256sum --check --status shasum; fi && \
    mkdir -p /opt && \
    sh miniconda.sh -b -p /opt/conda && \
    rm miniconda.sh shasum && \
    ln -s /opt/conda/etc/profile.d/conda.sh /etc/profile.d/conda.sh && \
    echo ". /opt/conda/etc/profile.d/conda.sh" >> ~/.bashrc && \
    echo "conda activate base" >> ~/.bashrc && \
    find /opt/conda/ -follow -type f -name '*.a' -delete && \
    find /opt/conda/ -follow -type f -name '*.js.map' -delete && \
    /opt/conda/bin/conda clean -afy
# Accept Anaconda's Terms of Service for py311_26.1.1-1 version
RUN conda tos accept --override-channels --channel https://repo.anaconda.com/pkgs/main && \
    conda tos accept --override-channels --channel https://repo.anaconda.com/pkgs/r


# Install software and SSH utilities (Consolidated to reduce layers)
RUN apt-get update && apt-get install -y --no-install-recommends \
    nano tmux wget iputils-ping git g++ net-tools curl zip unzip ffmpeg tree \
    openssh-server libpam-pwquality bash-completion && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install zenity for GUI dialogs for Isaac Sim
RUN apt update && apt install -y --no-install-recommends zenity

# SSH Configuration
RUN echo 'PASS_MIN_LEN 8' >> /etc/login.defs && \
    echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config

# Set up Python aliases pointing to the native Omniverse runtime
RUN echo "alias python='/isaac-sim/python.sh'" >> /root/.bashrc && \
    echo "alias python3='/isaac-sim/python.sh'" >> /root/.bashrc

# Align working directory
WORKDIR /root/dev

# Entry script setup
COPY entry.sh /entry.sh
RUN chmod +x /entry.sh
ENTRYPOINT [ "/entry.sh" ]