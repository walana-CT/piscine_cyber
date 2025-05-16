#!/bin/bash

# Lancer le service SSH
service ssh start

# Lancer le service nginx
service nginx start

# Lancer le service Tor
tor &

# Garder le container en vie (boucle infinie propre)
tail -f /dev/null