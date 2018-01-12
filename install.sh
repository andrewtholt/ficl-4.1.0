#!/bin/bash

set -x

make
strip ./ficl

HERE=$(pwd)

if [ ! -d "/usr/local/bin" ]; then
    sudo mkdir -p /usr/local/bin
fi

sudo mv ./ficl /usr/local/bin

if [ -f "libficl.so.4.1.0" ]; then
    sudo rm -f /usr/local/lib/libficl.s0.*
    sudo mv libficl.so.4.1.0 /usr/lib/lib
    cd /usr/local/lib
    sudo ln -s libficl.so.4 libficl.so.4.1.0
    sudo ln -s libficl.so libficl.so.4.1.0
    sudo ldconfig

    cs $HERE
fi
