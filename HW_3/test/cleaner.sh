#!/bin/bash

SOURCE_DIR=..

SERVER_DIR=${SOURCE_DIR}/Server
SERVER_CLEANER=${SERVER_DIR}/cleaner.sh

CLIENT_DIR=${SOURCE_DIR}/Client
CLIENT_CLEANER=${CLIENT_DIR}/cleaner.sh

${SERVER_CLEANER}
${CLIENT_CLEANER}

rm -f *.log
