# Utilise une image Ubuntu légère
FROM ubuntu:22.04

# Empêche les prompts interactifs
ARG DEBIAN_FRONTEND=noninteractive

# Mise à jour et installation des outils
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    iputils-ping \
    net-tools \
    curl \
    wget \
    nano \
    git \
    nmap \
    iproute2 \
    socat \
    tcpdump \
    libcrypto++-dev \
    libcrypto++-doc \
    libcrypto++-utils \
    make \
    cmake \
    sudo \
    && apt-get clean

COPY infection /home/infection
COPY stockholm /app/stockholm
COPY iron_dome /app/iron_dome

# Ajoute un utilisateur non root pour éviter de tout faire en root
RUN useradd -ms /bin/bash cyberuser
RUN echo "cyberuser ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
RUN chown -R cyberuser:cyberuser /app /home/infection /var/log
USER cyberuser
WORKDIR /home/cyberuser


# Copie les fichiers du projet dans le conteneur (si besoin)
# COPY . /home/cyberuser

CMD ["/bin/bash"]