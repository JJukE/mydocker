ARG IMG_NVDA=""
FROM ${IMG_NVDA}

ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# software installation
RUN rm /etc/apt/sources.list.d/cuda.list
# RUN rm /etc/apt/sources.list.d/nvidia-ml.list

RUN apt-get update

# basic
RUN apt install -y --no-install-recommends \
    nano tmux wget iputils-ping git g++ net-tools curl zip unzip ffmpeg

# miniconda (https://github.com/ContinuumIO/docker-images/blob/master/miniconda3/debian/Dockerfile)
RUN apt install -y --no-install-recommends \
    bzip2 \
    ca-certificates \
    libglib2.0-0 \
    libsm6 \
    libxext6 \
    libxrender1 \
    mercurial \
    openssh-client \
    procps \
    subversion
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

# @option: SSH server
RUN apt install -y --no-install-recommends openssh-server

# limit password quality
RUN apt install -y libpam-pwquality

# libraries
RUN apt install bash-completion

RUN echo 'PASS_MIN_LEN 8' >> /etc/login.defs
RUN echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config

RUN apt clean
RUN rm -rf /var/lib/apt/lists/*

RUN echo 'alias ca="conda activate"' >> /root/.bashrc
RUN echo 'conda activate' >> /root/.bashrc

WORKDIR /root/dev

# entry script
COPY entry.sh /entry.sh
RUN chmod +x /entry.sh
ENTRYPOINT [ "/entry.sh" ]
