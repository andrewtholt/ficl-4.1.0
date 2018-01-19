#!/bin/bash

# set -x

make
strip ./ficl

HERE=$(pwd)

if [ ! -d "/usr/local/bin" ]; then
    sudo mkdir -p /usr/local/bin
fi

echo "Move ficl"
sudo mv ./ficl /usr/local/bin

if [ -f "libficl.so.4.1.0" ]; then
    echo "Setup shared library and links."
    sudo rm -f /usr/local/lib/libficl.s0.*
    sudo mv libficl.so.4.1.0 /usr/lib/lib
    cd /usr/local/lib
    sudo ln -s libficl.so.4 libficl.so.4.1.0
    sudo ln -s libficl.so libficl.so.4.1.0
    sudo ldconfig

    cd $HERE
fi

if [ -d "HelperLibs" ]; then
    cd HelperLibs
    ./build.sh install
fi

if [ -d "./Lib" ]; then
    echo "Copying forth common function."
    FICL_LIB="/usr/local/lib/ficl"
    cd Lib
    mkdir -p $FICL_LIB
    sudo cp *.fth $FICL_LIB

    cd $HERE
fi
