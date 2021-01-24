#!/bin/bash

apt update
apt -y install git
apt -y install unzip
apt -y install wget
apt -y install screen

git clone https://github.com/8Michelle/pytorch-CycleGAN-and-pix2pix.git
cd pytorch-CycleGAN-and-pix2pix
pip install -r requirements.txt
mkdir saved_models
mv ../spider2castle.tar.gz datasets/
cd datasets
tar -xvzf spider2castle.tar.gz
