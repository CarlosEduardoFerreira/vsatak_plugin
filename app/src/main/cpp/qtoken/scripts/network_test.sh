#!/bin/bash
BOOT_COMMS=command_files/boot_commands.txt
PUT_COMMS=command_files/put_commands.txt
GET_COMMS=command_files/get_commands.txt
KEY="key1"
VALUE="value2"
OUTPUT_FILE=../output/$KEY

# Create all command files
echo "Creating necessary files..."
mkdir -p command_files
mkdir -p logs
touch $BOOT_COMMS
touch $PUT_COMMS
touch $GET_COMMS

touch logs/boot_out.txt
touch logs/put_out.txt
touch logs/get_out.txt
touch logs/valg_log.txt

echo "Running test commands"
./VIN -t "$BOOT_COMMS" -b > logs/boot_out.txt &
./VIN -t "$PUT_COMMS" -p 8008 > logs/put_out.txt &
./VIN -t "$GET_COMMS" -p 8009 > logs/get_out.txt &

sleep 5
echo "put $KEY $VALUE" >> $PUT_COMMS
echo "[command] put $KEY $VALUE"
sleep 1
echo "get $KEY" >> $GET_COMMS
echo "[command] get $KEY"
sleep 1

echo "exiting"
echo "exit" >> $PUT_COMMS
echo "exit" >> $GET_COMMS
echo "exit" >> $BOOT_COMMS

if grep -q "$VALUE" ../output/$KEY; then
    echo "Execution succeeded"
    exit 0
fi
echo "Execution failed!"
exit 1
