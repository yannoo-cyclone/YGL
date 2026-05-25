#!/bin/sh

# Installation des dépendances (si manquantes)
sudo apt-get install -y libglfw3-dev libglew-dev 2>/dev/null 

# Nettoyage et compilation 
cd build 
rm -rf *
cmake .. 
cmake --build . -j$(nproc)
cd ..
