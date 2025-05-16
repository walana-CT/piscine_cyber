FROM debian:stable

# Installer services
RUN apt-get update && \
    apt-get install -y nginx tor openssh-server && \
    apt-get clean

# Créer un utilisateur
RUN useradd -m -s /bin/bash student && \
    echo "student:student" | chpasswd

# Config SSH (sur port 2222 par exemple)
COPY sshd_config /etc/ssh/sshd_config

# Config nginx et tor
COPY nginx.conf /etc/nginx/nginx.conf
COPY torrc /etc/tor/torrc
COPY index.html /var/www/html/index.html

# Script de démarrage
COPY start.sh /start.sh
RUN chmod +x /start.sh

# Exposer les ports nécessaires
EXPOSE 80 2222

# Lancer les services
CMD ["/start.sh"]