#!/bin/bash

make -C ../../ 2>&1 >/dev/null || exit
cp ../../webserv ./

./webserv > exe.log &

# ./webserv > exe.log &
echo -n "wait..."
while [ `grep "wait Events..." exe.log | wc -l` -eq 0 ]; do
  echo -n "."
  sleep 0.2
done
echo

# go test .
bash test.sh

pkill webserv
