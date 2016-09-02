#!/usr/bin/env bash

mkdir -p build
mkdir -p tmp

if [[ "$(cmake --version | grep -E '[0-9]+.[0-9]+.[0-9]+' | sed 's/[a-z ]//gi')" == "3.6.1" ]]
then
  echo "Valid cmake"
else
  cd tmp
  wget https://cmake.org/files/v3.6/cmake-3.6.1.tar.gz
  tar -xvf cmake-3.6.1.tar.gz &> /dev/null
  cd cmake-3.6.1
  echo "Configure cmake..."
  ./configure &> /dev/null
  echo "Building cmake..."
  make -j 4 &> /dev/null
  echo "Installing cmake..."
  sudo make install &> /dev/null
  echo "  done"
fi

export PATH="/usr/local/bin:$PATH"

if [[ "$(cmake --version | grep -E '[0-9]+.[0-9]+.[0-9]+' | sed 's/[a-z ]//gi')" == "3.6.1" ]]
then
  echo "Failed to install cmake"
  exit 1
fi
