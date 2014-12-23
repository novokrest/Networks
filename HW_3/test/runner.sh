#!/bin/bash

SOURCE_DIR=~/Github/Networks/HW_3
CURRENT_DIR=${SOURCE_DIR}/test

SERVER_DIR=${SOURCE_DIR}/Server
SERVER_START=${SERVER_DIR}/start.sh
SERVER_EXE=${SERVER_DIR}/server

CLIENT_DIR=${SOURCE_DIR}/Client
CLIENT_START=${CLIENT_DIR}/start.sh
CLIENT_EXE=${CLIENT_DIR}/client


CLIENTS_COUNT=$1
REQUEST_SIZE=$2
IP=$3
PORT=$4

#Run server
#./server ${PORT} &

#Run clients
for i in `seq 1 ${CLIENTS_COUNT}`
do
    CLIENT_NAME="client_$i"
    echo "${CLIENT_NAME} starts"
    ./client -n ${CLIENT_NAME} -sip ${IP} -sport ${PORT} -rs ${REQUEST_SIZE} &
done

wait

RESULT_DIR_NAME="test_cs${CLIENTS_COUNT}_rs${REQUEST_SIZE}"
echo ${RESULT_DIR_NAME}
mkdir ${RESULT_DIR_NAME}
mv *.log ${RESULT_DIR_NAME}



