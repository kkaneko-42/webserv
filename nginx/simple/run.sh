#!/bin/bash

rm -rf log
mkdir log

docker-compose up -d

# GET
curl localhost:8080 > "log/get_default.log"

# POST
curl -X POST localhost:8080 > "log/post_default.log"

# DELETE
curl -X DELETE localhost:8080 > "log/delete_default.log"

docker-compose down
