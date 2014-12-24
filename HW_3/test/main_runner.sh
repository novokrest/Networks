#!/bin/bash

SOURCE_DIR=~/Github/Networks/HW_3
CURRENT_DIR=${SOURCE_DIR}/test

SERVER_DIR=${SOURCE_DIR}/Server
SERVER_START=${SERVER_DIR}/start.sh
SERVER_EXE=${SERVER_DIR}/server

CLIENT_DIR=${SOURCE_DIR}/Client
CLIENT_START=${CLIENT_DIR}/start.sh
CLIENT_EXE=${CLIENT_DIR}/client

#Compile server and client and copy to current dir
echo "Server compile..."
cd ${SERVER_DIR}
#${SERVER_START}
cd -
echo "Server copy"
cp -f ${SERVER_EXE} ${CURRENT_DIR}

echo "Client compile"
cd ${CLIENT_DIR}
#${CLIENT_START}
cd -
echo "Client copy"
cp -f ${CLIENT_EXE} ${CURRENT_DIR}

IP=192.168.1.2
PORT=7000

MAX_CLIENTS_COUNT=30
REQUEST_SIZES=(
1
10
100
1000
10000
100000
1000000
)

for cc in `seq 1 ${MAX_CLIENTS_COUNT}`
do
    echo "LOOP"
    let REQUEST_SIZE=10
    for rs in ${REQUEST_SIZES[*]}
    do
        ./runner.sh ${cc} ${rs} ${IP} ${PORT}
        wait
    done
done