#!/bin/sh

# Installation des dépendances (si manquantes)
sudo apt-get install -y libglfw3-dev libglew-dev 2>/dev/null || true

# Nettoyage et compilation avec redirection vers compil.err
cd build || exit 1
rm -rf *
(cmake .. && cmake --build . -j$(nproc)) > ../compil.err 2>&1 || exit 1
cd ..
