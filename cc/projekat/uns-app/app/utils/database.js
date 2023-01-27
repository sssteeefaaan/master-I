const mongoose = require("mongoose")
const env = process.env

const db_url = env["DATABASE_URL"] || "mongodb://uns-database-01:27017,uns-database-02:27017/uns"

const options = {
  // auth: {
  //   username: env["DATABASE_USER"] || "admin",
  //   password: env["DATABASE_PASSWORD"] || "admin"
  // },
  // authMechanism: "DEFAULT",
  // authSource: "admin",
  appName: "uns-app",
  retryWrites: true,
  replicaSet: env["MONGO_REPLICA_SET_NAME"] || "uns-replicaset"
}

// Database instance
let db_client = mongoose

let connected = false

function getClient() {
    if(!connected){
      db_client.connect(db_url, options)
      console.log("Connected successfully to the database server")
      connected = true;
    }
    return db_client;
}

module.exports = {getClient}