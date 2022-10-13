#!/bin/bash

END="\033[0m"
RED="\033[31m"
GREEN="\033[32m"

function NG() {
  conf="$1"
  filename="${conf#ng/}"

  echo -n "Testing ${conf#ng/}..."
  echo "[${conf#ng/}]" >> test.log
  ../../webserv "$conf" 2>>test.log >/dev/null
  result=$?

  if [ "$result" = "0" ]; then
    echo -e "${RED}KO!!!!!!${END}"
  else
    echo -e "${GREEN}OK${END}"
  fi
}

echo -n > test.log

echo "[NG]"
for f in `ls ng`; do
  NG ng/$f
done
