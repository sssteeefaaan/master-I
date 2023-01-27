#!/bin/bash
set -m

mongod --syslog --bind_ip_all --replSet uns-replicaset &

/scripts/rs-init.sh

fg $(jobs -l | grep $! | cut -c2)