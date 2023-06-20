#!/bin/bash

# Install required libraries
sudo apt update
sudo apt install -y libcjson-dev libcurl4-openssl-dev

# Compile C file
gcc -o realnotes source.c -lcurl -lcjson

# Add the compiled binary to the bin directory
mkdir -p ~/bin
sudo cp realnotes /usr/local/bin

echo "Installation completed. The 'realnotes' binary is now available in the 'bin' directory."
