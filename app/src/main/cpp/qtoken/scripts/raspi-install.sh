#!/bin/sh

# get required libs
sudo apt-get -y update && apt-get -y upgrade
sudo apt-get -y install libboost-all-dev git cmake g++ libssl-dev valgrind unzip wget cppcheck curl libgflags-dev libconfig++-dev libsodium-dev pkg-config libpoco-dev

# build/install kademlia (FIXME: Use our mirror for includes)
git clone https://github.com/DavidKeller/kademlia.git
cd kademlia
curl -uanonymous: -O https://opai.jfrog.io/artifactory/VIN/libkademlia.raspi4.so
sudo mv ./libkademlia.raspi4.so /usr/lib/libkademlia.so
sudo mkdir /usr/include/kademlia
sudo cp -r include/kademlia/* /usr/include/kademlia

# build/install aff3ct
cd ..
wget https://gitlab.com/aff3ct/aff3ct/-/jobs/302344037/artifacts/raw/builds/aff3ct_master_linux_gcc_x64_avx2_1ceddfc.zip -O aff3ct.zip
unzip aff3ct.zip
sudo cp -r build_linux_gcc_x64_avx2/include/aff3ct-2.3.5* /usr/include/
curl -uanonymous: -O https://opai.jfrog.io/artifactory/VIN/libaff3ct-2.3.5.raspi4.so
chmod 777 libaff3ct-2.3.5.raspi4.so
sudo mv libaff3ct-2.3.5.raspi4.so /usr/lib/libaff3ct-2.3.5.so

# set up config vars/dirs
./package.sh

# finally, install VIN
sudo dpkg -i VIN_0.1.0.deb
sudo apt update
sudo apt --fix-broken install
