#bin/bash

# 1. Télécharger le code source
wget https://download.savannah.nongnu.org/releases/oath-toolkit/oath-toolkit-2.6.9.tar.gz
tar -xvzf oath-toolkit-2.6.9.tar.gz
cd oath-toolkit-2.6.9

# 2. Configuration (en local)
./configure --prefix=$HOME/.local

# 3. Compilation
make
make install

# 4. Ajouter à ton PATH
echo 'export PATH=$HOME/.local/bin:$PATH' >> ~/.bashrc
source ~/.bashrc