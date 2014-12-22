#!/bin/bash

SOURCE_DIR=..
CURRENT_DIR=.

SERVER_DIR=${SOURCE_DIR}/Server
SERVER_START=${SERVER_DIR}/start.sh
SERVER_EXE=${SERVER_DIR}/server

CLIENT_DIR=${SOURCE_DIR}/Client
CLIENT_START=${CLIENT_DIR}/start.sh
CLIENT_EXE=${CLIENT_DIR}/client

clients=(clientA clientB clientC clientD clientE clientF clientG)
ip=127.0.0.1
port=7000

clear
cd ${SERVER_DIR}
${SERVER_START}
cd -

cd ${CLIENT_DIR}
bash ${CLIENT_START}
cd -

${SERVER_EXE} ${port} &

for name in ${clients[*]}
do
    ${CLIENT_EXE} -n ${name} -sip ${ip} -sport ${port} &
done

wait

