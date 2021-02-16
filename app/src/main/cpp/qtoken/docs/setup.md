# Qtoken CLI Documentation and Setup

## Build and Install
1. *Ubuntu 20.04*
   - Navigate to `qtoken-cpp/scripts` and run `sudo ./ubuntu-install.sh`. This will download and install all VIN dependencies.
   - After this is done, from the project root run the following commands:
       - `mkdir build && cd build`
       - `cmake ..`
       - `make` or `cmake --build .` [Note: you can use `make -jX` where X is the number of threads to speed up install]

2. *Raspberry Pi OS [2021-01-11]*
   - Navigate to `qtoken-cpp/scripts` and run `sudo ./raspi-install.sh`
   - Follow step ii. from the Ubuntu install

3. *Any other UNIX OS*

As of now we only support Ubuntu and Raspbian. For other UNIX OSs, consult your package manager for the `apt` dependencies. You can build `aff3ct` and `kademlia` from source and move the libs/includes to the `/usr/lib` equivalent.

4. *Windows*

We currently do not support Windows. We've had it work in WSL1/2 but YMMV. You can use Docker, if not a VM.

## Running tests
After following the above steps, you should have a test executable generated in `qtoken-cpp/build/test` called `VIN_test`. You can run it to run the test suite.

## Running the network

To run the VIN, you can spin up a few nodes on your own machine localhost or spin up nodes in different machines.

1. Localhost:

To spin up a basic network on your machine, you can use the following steps:

- Open 3 terminal sessions at the directory where your `VIN` executable lives.
- On one of them, run `./VIN -b`. This will serve as the bootstrap node and will occupy port 8000 for incoming connections.
- On another, run `./VIN`. This will start a VIN peer on the default port for data (8080) and the default receipt server (9090)
- On the final terminal, run `./VIN -p <DATA_PORT> -r <RECEIPT_PORT>` where `<DATA_PORT>` and `<RECEIPT_PORT>` must be different from the other peer.


2. IP Network:

To spin up a basic VIN on an IP based network (such as AWS, LAN with routers/switches and DHCP, VMware), you can use the following steps:

- Follow the install steps for all devices in the network (n.b: mass deployment is an upcoming feature).
- On any node, from the `VIN` executable directory run `./VIN -b` to start a bootstrap node. Note down the IP address of this device.
- In all subsequent nodes, use a text editor such as `vim` or `nano` to edit the Qtoken config file `/etc/opt/VIN/defaults.cfg`. Replace the `default_bootstrap_address` variable to look like this:
   `default_bootstrap_address = "<BOOTSTRAP_NODE_IP_ADDRESS>:8000";`
    Make sure to leave the quotes (`"`) intact.
- Following this, run `./VIN` on all non-bootstrap nodes.

Please ensure that there are no other processes that are using all ports mentioned above! At this point you have successfully launched a Virgil Integrity Network. Each peer should display a CLI with all available commands.

## Disk
All Qtoken Disk IO happens in `/opt/VIN`. Any receipts/reconstructed data will be stored there.

## Command Reference
N.B. Question marks denote optional arguments

1. Put
   - `put <key> <value>`
   - Puts a value with the given key.
2. Get
   - `get <key>`
   - Takes a given key and returns the Key-Value pair from any node. The value returned is saved in `/opt/VIN/outputs`. No other output is displayed.
3. Spread
   - `spread <filepath> <ip address>? <port of another node>?`
   - Splits a file of any type located in a given path into Qtokens and then spreads them across the network. An encrypted receipt is then generated and stored in `/opt/VIN/receipts/sent`.
   - If the IP Address and receipt port of another peer in the network is included, the peer will automatically establish a secure channel with that peer and transfer the encrypted receipt over. The receiver peer will automatically call `gather` on the receipt once decrypted
4. Gather
   - `gather <receipt filepath>`
   - Gathers a spread file using the given receipt. It will be reassembled into the output directory `/opt/VIN/outputs`.
