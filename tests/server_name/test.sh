#!/bin/bash

CURL_LOG="curl.log"

RED='\033[31m'
GREEN='\033[32m'
END='\033[0m'

function STATUS() {
  status="$1"
  grep "< HTTP/1.1 ${status}" ${CURL_LOG} > /dev/null
  result=$?

  if [ "${result}" = "0" ]; then
    echo -n -e "${GREEN}OK${END} "
  else
    echo -n -e "${RED}KO${END} "
  fi
}

function BODY() {
  word="$1"
  grep "<title>${word}</title>" ${CURL_LOG} > /dev/null
  result=$?

  if [ "${result}" = "0" ]; then
    echo -n -e "${GREEN}OK${END} "
  else
    echo -n -e "${RED}KO${END} "
  fi
}

curl -v "localhost:4242" >${CURL_LOG} 2>&1
STATUS 200
BODY hoge

curl -v -H "Host:hoge.com" "localhost:4242" >${CURL_LOG} 2>&1
STATUS 200
BODY hoge

curl -v -H "Host:fuga.com" "localhost:4242" >${CURL_LOG} 2>&1
STATUS 200
BODY fuga

curl -v -H "Host:piyo.com" "localhost:4242" >${CURL_LOG} 2>&1
STATUS 200
BODY piyo

curl -v -H "Host:foo.com" "localhost:4242" >${CURL_LOG} 2>&1
STATUS 200
BODY hoge

curl -v "localhost:4243" >${CURL_LOG} 2>&1
STATUS 200
BODY hoge4243

curl -v -H "Host:hoge.com" "localhost:4243" >${CURL_LOG} 2>&1
STATUS 200
BODY hoge4243

curl -v -H "Host:fuga.com" "localhost:4243" >${CURL_LOG} 2>&1
STATUS 200
BODY hoge4243

curl -v -H "Host:piyo.com" "localhost:4243" >${CURL_LOG} 2>&1
STATUS 200
BODY hoge4243

curl -v -H "Host:foo.com" "localhost:4243" >${CURL_LOG} 2>&1
STATUS 200
BODY foo

echo
