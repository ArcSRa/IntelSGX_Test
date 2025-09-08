# Utiliser l'image officielle Ubuntu 24.04 LTS
FROM ubuntu:24.04

# Installer les dépendances communes et outils nécessaires
RUN apt-get update && apt-get install -y \
    build-essential \
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
    git \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Ajouter clé et dépôt Intel SGX
RUN curl -fsSLo /usr/share/keyrings/intel-sgx-deb.asc https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/intel-sgx-deb.asc] https://download.01.org/intel-sgx/sgx_repo/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/intel-sgx.list

# Mettre à jour et installer les paquets SGX
RUN apt-get update && apt-get install -y libsgx-dcap-quote-verify-dev && rm -rf /var/lib/apt/lists/*

# Cloner le dépôt Gramine
RUN git clone https://github.com/gramineproject/gramine.git /opt/gramine
RUN git clone https://github.com/ArcSRa/IntelSGX_Test.git /opt/testSGX

# Configurer le build avec meson (SGX activé, direct désactivé)
WORKDIR /opt/gramine
RUN meson setup build/ --buildtype=release -Ddirect=enabled -Dsgx=enabled

# Compiler Gramine
RUN meson compile -C build/

# Installer Gramine
RUN meson install -C build/

# Ajouter Gramine au PATH
ENV PATH="/usr/local/bin:$PATH"
WORKDIR /opt/testSGX

#CMD /bin/bash -c "gramine-sgx-gen-private-key && make SGX=1 && gramine-sgx helloworld && sleep infinity"
