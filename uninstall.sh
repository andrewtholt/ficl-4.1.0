#!/bin/bash

set -x

MAJOR=4
MINOR=1.0

sudo rm -f /usr/local/bin/ficl
sudo rm -f /usr/local/include/ficl.h
sudo rm -f /usr/local/include/ficllocal.h
sudo rm -f /usr/local/include/ficltokens.h
sudo rm -f /usr/local/include/ficlplatform/unix.h
sudo rm -f /usr/local/lib/libficl.so.$MAJOR.$MINOR
sudo rm -f /usr/local/lib/libficl.a
sudo rm -f /usr/local/lib/libficl.so.$MAJOR
sudo rm -f /usr/local/lib/libficl.so

