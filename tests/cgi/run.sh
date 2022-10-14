#!/bin/bash
OS=`uname`

RED='\033[0;31m'
GREEN='\033[32m'
NC='\033[0m'

# build
make -C ../../ 2>&1 >/dev/null || exit
cp ../../webserv ./;

# execute
if [ "${OS}" = "Linux" ]; then
    valgrind --log-file="valgrind.log" --leak-check=yes ./webserv > /dev/null 2>&1 &
    echo "waiting for starting test..."
    sleep 2
else
    ./webserv > /dev/null 2>&1 &
    sleep 2
fi

# run test
for src in `ls -1 ./src/ | grep .py`
do
    python3 ./src/$src
done

# terminate
if [ "${OS}" = "Linux" ]; then
    pid=`ps | grep 'memcheck' | awk '{print $1}'`;
    kill ${pid};
    
    # wait valgrind exit
    while [ `ps | grep 'memcheck' | awk '{print $1}'` ]; do
        echo "waiting for valgrind exit...";
        sleep 0.1;
    done

    cat "valgrind.log" | grep ERROR | grep -v " 0 errors " > /dev/null
    result=$?
    if [ "$result" = "0" ]; then
        echo -e "valgrind: ${RED}KO!!!!!!!!!!:${NC}"
    else
        echo -e "valgrind: ${GREEN}OK${NC}"
    fi
else
    echo TODO

    pid=`ps | grep 'memcheck' | grep -v grep | awk '{print $1}'`;
    kill ${pid};
fi
