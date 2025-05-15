FROM debian:bullseye

RUN apt update && apt install -y nginx tor openssh-server

# Configuration SSH
RUN mkdir /var/run/sshd
COPY sshd_config /etc/ssh/sshd_config

# Configuration Nginx
COPY nginx.conf /etc/nginx/nginx.conf
COPY index.html /var/www/html/index.html

# Configuration Tor
COPY torrc /etc/tor/torrc

# SSH user (facultatif)
RUN useradd -m student && echo "student:password" | chpasswd

EXPOSE 80 4250

CMD service ssh start && service nginx start && tor