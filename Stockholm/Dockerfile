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
    && apt-get clean

COPY infection /home/infection
COPY app /app
WORKDIR /app
RUN make

# Ajoute un utilisateur non root pour éviter de tout faire en root
RUN useradd -ms /bin/bash cyberuser
RUN chown -R cyberuser:cyberuser /app /home/infection
USER cyberuser
WORKDIR /home/cyberuser


# Copie les fichiers du projet dans le conteneur (si besoin)
# COPY . /home/cyberuser

CMD ["/bin/bash"]