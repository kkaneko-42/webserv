#!/bin/bash

rm -rf log
mkdir log

docker-compose up -d

curl -X POST --dump-header - localhost:8080
echo ---------------------------
curl -X POST --dump-header - localhost:8080/
echo ---------------------------
curl -X POST --dump-header - localhost:8080/hoge
echo ---------------------------
curl -X POST --dump-header - localhost:8080/hoge/
echo ---------------------------
curl -X POST --dump-header - localhost:8080/hoge/index.html
echo ---------------------------
curl -X DELETE --dump-header - localhost:8080/hoge/index.html
# echo "{\"fuge\": 5678}" | curl -X POST -v  -H "Content-type: application/json" -d @- http://localhost:8080/hoge

# GET
curl localhost:8080 > "log/get_default.log"

# POST
curl -X POST localhost:8080 > "log/post_default.log"

# DELETE
curl -X DELETE localhost:8080 > "log/delete_default.log"

docker-compose down
