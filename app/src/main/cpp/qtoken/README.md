# QToken-cpp
The C++ implementation of QToken.

## Build+Run
Refer to `docs/setup.md`.

## Docker
- With the latest version of Docker (tested on 19.03.13-ce) installed, run `docker build .` from the project root
- After the build is complete, it will display a hash, for example: `Successfully built d41cc18a46a2`
- You can run the alpine container with a working VIN build using `docker run -dit [HASH_FROM_PREVIOUS_STEP]`
- The above command will generate a container ID that you can find using `docker ps`
- Finally, you can access the VIN build using `docker exec -it [CONTAINER_ID_FROM_PREVIOUS_STEP] /bin/sh`

You will now have a shell into the project root, where the VIN executable can be accessed through the `build` directory.

## Aff3ct
The precompiled library can be downloaded at:
https://aff3ct.github.io/download.html

Alternatively clone and install the Aff3ct library:
- `git clone --recursive https://github.com/aff3ct/aff3ct.git`
- `mkdir aff3ct/build`
- `cd aff3ct/build`
- `cmake .. -DCMAKE_INSTALL_PREFIX="/usr" -DCMAKE_BUILD_TYPE="Install" -DAFF3CT_COMPILE_EXE=OFF -DAFF3CT_COMPILE_STATIC_LIB=ON`
- `sudo make install`

The install defaults to /usr
- /usr/lib should contain the static library and a cmake folder
- /usr/include should contain header files for the project

More Aff3ct documentation can be found here:
https://aff3ct.readthedocs.io/en/latest/user/installation/installation/installation.html#

## Debian Install
VIN can compiled and packaged as a debian to be installed on another debian system. This process is similar to the normal build process and can be completed two different ways.
### Building
#### Docker
- From the qtoken-cpp folder run `docker build -t deb_build:build -f docker/Dockerfile.build .`
- Once the container is done do `docker run -it deb_build:build bash`, this will attach a terminal session to the container.
- navigate to the `qtoken-cpp` dir then:
  - `mkdir build && cd build`
  - `cmake ..`
  - `make`
- Once the build is done do `cd ../scripts` and `./package.sh`
- If this succeeds you should have VIN-\<version\>.deb that can be installed with the later steps
#### Local
- One must install kademlia using the following commands
  - `git clone https://github.com/DavidKeller/kademlia.git`
  - `cd kademlia`
  - `mkdir build && cd build`
  - `cmake ..`
  - `make`
  - `cp ./src/kademlia/libkademlia.so /usr/lib/libkademlia.so`
  - `mkdir /usr/include/kademlia`
  - `cp ./src/kademlia/libkademlia.so /usr/lib`
  - `cp -r ../include/kademlia/* /usr/include/kademlia`
- navigate to the `qtoken-cpp` dir then:
  - `mkdir build && cd build`
  - `cmake ..`
  - `make`
- Once the build is done do `cd ../scripts` and `./package.sh`
- If this succeeds you should have VIN-\<version\>.deb that can be installed with the later steps

### Installing
- Put the .deb from the previous steps onto your target machine.
- run:
  - `dpkg -i VIN-<version>.deb`
  - `apt-get update`
  - `apt --fix-broken install`
- install aff3ct as described above.
- now run `VIN -[options]` as normal.

Virgil Systems® 2021
