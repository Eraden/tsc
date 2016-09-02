#!/usr/bin/env bash

mkdir -p build
mkdir -p tmp

if [ "$(cmake --version)" -eq "3.6.1" ]
then
  echo "Valid cmake"
else
  cd tmp
  wget https://cmake.org/files/v3.6/cmake-3.6.1.tar.gz
  tar -xvf cmake-3.6.1.tar.gz
  cd cmake-3.6.1
  ./configure
  make -j 4
  sudo make install
fi

if [ "$(cmake --version)" -ne "3.6.1" ]
then
  echo "Failed to install cmake"
  exit 1
fi

sudo apt-get update -qq
sudo apt-get upgrade