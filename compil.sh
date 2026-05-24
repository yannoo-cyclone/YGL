#!/bin/sh

# Vérification des dépendances (sans bloquer si déjà installées)
sudo apt-get install -y libglfw3-dev libglew-dev 2>/dev/null || true

# Nettoyage et compilation
cd build || exit 1
rm -rf *
cmake .. || exit 1
cmake --build . -j$(nproc) || exit 1
cd ..
