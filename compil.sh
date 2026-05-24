#!/bin/sh

sudo apt-get install -y libglfw3-dev  # Installe GLFW si manquant

cd build
rm -rf *
cmake ..
cmake --build .
cd ..

