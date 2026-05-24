#!/bin/sh

# Installation des dépendances (si manquantes)
sudo apt-get install -y libglfw3-dev libglew-dev

# Nettoyage et compilation
cd build
rm -rf *
cmake ..
cmake --build . -j$(nproc)  # Utilise tous les cœurs disponibles
cd ..

