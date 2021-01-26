#!/bin/bash

sudo groupadd camera
sudo useradd -G camera remote
sudo usermod -aG sudo remote
sudo mkhomedir_helper remote

# Changer le mot de passe ...
