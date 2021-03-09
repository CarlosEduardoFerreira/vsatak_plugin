#!/bin/bash

# TODO: Versioning support
VER='0.1.0'
PACKAGE_NAME=VIN_$VER

# Destroy any existing package info
rm -rf $PACKAGE_NAME

# Build package structure
mkdir $PACKAGE_NAME
mkdir $PACKAGE_NAME/DEBIAN
mkdir -p $PACKAGE_NAME/usr/bin/
cp control $PACKAGE_NAME/DEBIAN/control
# cp postinst $PACKAGE_NAME/DEBIAN/
cp ../build/VIN $PACKAGE_NAME/usr/bin/

# Install Kademlia
mkdir -p $PACKAGE_NAME/usr/lib/
mkdir -p $PACKAGE_NAME/usr/include/kademlia
cp /usr/lib/libkademlia.so $PACKAGE_NAME/usr/lib/
cp -r /usr/include/kademlia/* $PACKAGE_NAME/usr/include/kademlia

# Handle config files
mkdir -p $PACKAGE_NAME/opt/VIN/
mkdir -p $PACKAGE_NAME/opt/VIN/keys/
mkdir -p $PACKAGE_NAME/opt/VIN/outputs/
mkdir -p $PACKAGE_NAME/opt/VIN/receipts/received/
mkdir -p $PACKAGE_NAME/opt/VIN/receipts/sent/

mkdir -p $PACKAGE_NAME/etc/opt/VIN/

if [[ -e ../defaults.cfg ]]; then
    cp ../defaults.cfg $PACKAGE_NAME/etc/opt/VIN/
elif [[ -e ./defaults.cfg ]]; then
    cp defaults.cfg $PACKAGE_NAME/etc/opt/VIN/
else
    echo "Default config not found!"
fi

mkdir -p $PACKAGE_NAME/var/log/VIN/logs/

dpkg-deb --build $PACKAGE_NAME
