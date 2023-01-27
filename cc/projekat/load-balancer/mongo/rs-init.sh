#!/bin/bash

mongosh --eval "rs.initiate({
 _id: \"uns-replicaset\",
 members: [
   {_id: 0, host: \"uns-database-01:27017\"},
   {_id: 1, host: \"uns-database-02:27017\"}
 ]
})"