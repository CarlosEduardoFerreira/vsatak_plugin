#!/bin/sh

# get required libs
sudo apt-get -y update && sudo apt-get -y upgrade
sudo apt-get -y install libboost-all-dev git cmake g++ libssl-dev valgrind unzip wget cppcheck curl libgflags-dev libconfig++-dev libsodium-dev pkg-config libpoco-dev

# Ensure boost is found
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/:${LD_LIBRARY_PATH}
sudo ln -s /usr/lib/x86_64-linux-gnu/libboost* /usr/lib

# build/install kademlia (FIXME: Use our mirror)
git clone https://github.com/DavidKeller/kademlia.git
cd kademlia
curl -uanonymous: -O https://opai.jfrog.io/artifactory/VIN/libkademlia.ubuntu_20_04.so
sudo mv ./libkademlia.ubuntu_20_04.so /usr/lib/libkademlia.so
sudo mkdir /usr/include/kademlia
sudo cp -r include/kademlia/* /usr/local/include/kademlia

# build/install aff3ct
cd ..
wget https://gitlab.com/aff3ct/aff3ct/-/jobs/302344037/artifacts/raw/builds/aff3ct_master_linux_gcc_x64_avx2_1ceddfc.zip -O aff3ct.zip
unzip aff3ct.zip
sudo mkdir /usr/local/include/aff3ct-2.3.5
sudo cp -r build_linux_gcc_x64_avx2/include/aff3ct-2.3.5* /usr/local/include/aff3ct-2.3.5
curl -uanonymous: -O https://opai.jfrog.io/artifactory/VIN/libaff3ct-2.3.5.ubuntu_20_04.so
chmod 777 libaff3ct-2.3.5.ubuntu_20_04.so
sudo mv libaff3ct-2.3.5.ubuntu_20_04.so /usr/lib/libaff3ct-2.3.5.so

# set up config vars/dirs
./package.sh

# finally, install VIN
sudo dpkg -i VIN_0.1.0.deb
sudo apt update
sudo apt --fix-broken install
