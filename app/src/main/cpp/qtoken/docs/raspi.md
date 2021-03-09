# Qtoken CLI Documentation and Setup

## Build and Install
This documentation assumes you have received a ZIP file with the Qtoken Raspberry Pi 4 deployment package. You can get setup with the following simple steps:

- Make sure you have `unzip` installed: `sudo apt install unzip`
- Unzip the package: `unzip VIN_raspi4_deploy.zip`
- Run the install script: `sudo ./raspi-install.sh`
- You should now be able to run both the `VIN` and `VIN_test` binaries.

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

## Debug Logs
Qtoken nodes store log outputs in `/var/log/VIN/main_log` and ``/var/log/VIN/main_log.0`. You can send these logs along with your support requests so we have more visibility.

## Command Reference

1. Put
   - `put <key> <value>`
   - Puts a value with the given key.
2. Get
   - `get <key>`
   - Takes a given key and returns the Key-Value pair from any node. The value returned is saved in `/opt/VIN/outputs`. No other output is displayed.
3. Spread
   - `spread <filepath>`
   - Splits a file of any type located in a given path into Qtokens and then spreads them across the network. An encrypted receipt is then generated and stored in `/opt/VIN/receipts/sent`.
4. Share
   -  `share <filepath> <ip address> <port of another node>`
   -  Spreads a file, then the peer will automatically establish a secure channel with IP Address and receipt port of specified peer in the network and transfer the encrypted receipt over. The receiver peer will automatically call `gather` on the receipt once decrypted.
5. Gather
   - `gather <receipt filepath>`
   - Gathers a spread file using the given receipt. It will be reassembled into the output directory `/opt/VIN/outputs`.
