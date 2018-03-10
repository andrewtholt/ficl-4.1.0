#!/bin/bash

# set -x

make
strip ./ficl

HERE=$(pwd)

if [ ! -d "/usr/local/bin" ]; then
    sudo mkdir -p /usr/local/bin
fi

tput bold
echo
echo "Move ficl"
tput sgr0

sudo mv ./ficl /usr/local/bin

if [ -f "libficl.so.4.1.0" ]; then
    echo
    tput bold
    echo "Setup shared library and links."
    tput sgr0

    sudo rm -f /usr/local/lib/libficl.s0.*
    sudo mv libficl.so.4.1.0 /usr/lib/lib
    cd /usr/local/lib
    sudo ln -s libficl.so.4 libficl.so.4.1.0
    sudo ln -s libficl.so libficl.so.4.1.0
    sudo ldconfig

    cd $HERE

    sudo cp ./ficl.h /usr/local/include
    sudo cp ./ficllocal.h /usr/local/include
    sudo cp ./ficltokens.h /usr/local/include
    sudo mkdir /usr/local/include/ficlplatform
    sudo cp ./ficlplatform/*.h  /usr/local/include/ficlplatform
fi

if [ -d "HelperLibs" ]; then
    echo
    tput bold
    echo "Install Helper libs"
    tput sgr0

    cd HelperLibs
    ./build.sh install
fi

cd $HERE

if [ -d "./Lib" ]; then
    echo
    tput bold
    echo "Copying forth common function."
    tput sgr0

    FICL_LIB="/usr/local/lib/ficl"
    cd Lib
    mkdir -p $FICL_LIB
    sudo cp *.fth $FICL_LIB

    cd $HERE
fi
