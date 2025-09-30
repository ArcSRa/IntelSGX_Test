FROM ubuntu:24.04

ARG SGX_VERSION=2.26
ARG SDK_BIN=sgx_linux_x64_sdk_2.26.100.0.bin
ENV DEBIAN_FRONTEND=noninteractive

# Dépendances communes et outils nécessaires
RUN apt-get update && apt-get install -y \
    build-essential \
    apt-utils \
    autoconf \
    bison \
    gawk \
    meson \
    lsb-release \
    nasm \
    pkg-config \
    python3 \
    python3-click \
    python3-jinja2 \
    python3-pyelftools \
    python3-tomli \
    python3-tomli-w \
    python3-voluptuous \
    wget \
    libunwind8 \
    libssl3 \
    libssl-dev \
    musl-tools \
    python3-pytest \
    cmake \
    libprotobuf-c-dev \
    protobuf-c-compiler \
    protobuf-compiler \
    python3-cryptography \
    python3-pip \
    python3-protobuf \
    gdb \
    git \
    curl \
    nano \
    automake \
    libtool \
    perl \
    libprotobuf-dev \
    libsystemd0 \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Clé et dépôt SGX (runtime/PSW via apt, SDK via bin officiel)
RUN curl -fsSL https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | tee /usr/share/keyrings/intel-sgx-deb.asc >/dev/null \
  && echo "deb [arch=amd64 signed-by=/usr/share/keyrings/intel-sgx-deb.asc] https://download.01.org/intel-sgx/sgx_repo/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/intel-sgx.list

RUN apt-get update && apt-get install -y libsgx-dcap-quote-verify-dev libsgx-urts libsgx-launch && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/intel

# SDK complet pour développement et SEAL
RUN wget https://download.01.org/intel-sgx/sgx-linux/${SGX_VERSION}/distro/ubuntu24.04-server/${SDK_BIN} \
    && chmod +x ${SDK_BIN} \
    && echo yes | ./${SDK_BIN} \
    && rm -f ${SDK_BIN}

# Vérifier que le header seal est bien là
RUN test -f /opt/intel/sgxsdk/include/sgx_tseal.h

ENV SGX_SDK=/opt/intel/sgxsdk
ENV PATH="${SGX_SDK}/bin:/usr/local/bin:$PATH"
RUN unset LD_LIBRARY_PATH
# Cloner Gramine et exemple SGX
WORKDIR /opt
RUN git clone https://github.com/gramineproject/gramine.git /opt/gramine
RUN git clone https://github.com/ArcSRa/IntelSGX_Test.git /opt/testSGX

WORKDIR /opt/gramine
RUN meson setup build/ --buildtype=release -Ddirect=enabled -Dsgx=enabled
RUN meson compile -C build/
RUN meson install -C build/

WORKDIR /opt/testSGX

# Entrée par défaut: shell interactif
CMD ["/bin/bash"]
