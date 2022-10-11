#!/bin/bash
# install
python3 -m pip install --upgrade pip;
python3 -m pip install requests;

# build
make -C ../../../ && cp ../../../webserv ./;

# execute test
./webserv ./methods_test.conf > /dev/null 2>&1 & \
for src in `ls -1 ./src/ | grep .py`
do
    python3 ./src/$src
done
pkill -KILL -f webserv;
