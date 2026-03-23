# Step 1: Target the pre-built Isaac Lab container
ARG IMG_NVDA="nvcr.io/nvidia/isaac-lab:2.3.2"
FROM ${IMG_NVDA}

ENV TZ=Asia/Seoul
# Switch to root to ensure permission for apt installations
USER root

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Step 2: Install software and SSH utilities (Consolidated to reduce layers)
RUN apt-get update && apt-get install -y --no-install-recommends \
    nano tmux wget iputils-ping git g++ net-tools curl zip unzip ffmpeg \
    openssh-server libpam-pwquality bash-completion && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# SSH Configuration
RUN echo 'PASS_MIN_LEN 8' >> /etc/login.defs && \
    echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config

# Step 3: Set up Python aliases pointing to the native Omniverse runtime
RUN echo "alias python='/isaac-sim/python.sh'" >> /root/.bashrc && \
    echo "alias python3='/isaac-sim/python.sh'" >> /root/.bashrc

# Align working directory with the pre-built container's structure
WORKDIR /workspace/isaaclab/dev

# Entry script setup
COPY entry.sh /entry.sh
RUN chmod +x /entry.sh
ENTRYPOINT [ "/entry.sh" ]