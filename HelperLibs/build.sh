#!/bin/bash

# set -x

LIST=$(ls)
HERE=$(pwd)
INSTALL="NO"

if [ $# -gt 0 ]; then
    if [  "$1" == "install" ]; then
        INSTALL="YES"
        echo "Install after build"
    fi
fi

for N in $LIST; do
    if [ -d $N ]; then
        echo "Entering directory $N"
        cd $N
        make
        if [ $INSTALL == "YES" ]; then
            make install
        fi
        cd $HERE
    fi
done


