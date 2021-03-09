#!/bin/bash

BUILD=../build
VIN=../build/VIN
VIN_TEST=../build/test/VIN_test
RASPI_INSTALL=./raspi-install.sh
UBUNTU_INSTALL=./ubuntu-install.sh
SCRIPT=$UBUNTU_INSTALL

# check args
if [[ $# -ne 1 ]]; then
    echo "usage: ./gen_deploy [OS]"
    exit 2
fi

# check OS
if [[ $1 = "raspi" ]]; then
    SCRIPT=$RASPI_INSTALL
elif [[ $1 != "ubuntu" ]]; then
    echo "Only raspi/ubuntu supported"
    exit 2
fi

# create package dir
mkdir VIN_deploy
cp $SCRIPT ./control ./package.sh ../defaults.cfg VIN_deploy

# get bins
if [[ -e $VIN ]]; then
    cp $VIN VIN_deploy
else
    echo "VIN binary not found; run make commands first"
    exit 2
fi

if [[ -e $VIN ]]; then
    cp $VIN_TEST VIN_deploy
else
    echo "VIN test binary not found; skipping"
fi

# generate package
zip -r VIN_deploy.zip VIN_deploy

# cleanup
rm -rf VIN_deploy

echo "package generated at $(pwd)/VIN_deploy.zip"
