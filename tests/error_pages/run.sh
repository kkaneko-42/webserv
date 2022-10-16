#!/bin/bash
OS=`uname`

RED='\033[0;31m'
GREEN='\033[32m'
NC='\033[0m'

SERVER_LAUNCH_STR="wait Events..."

# build
make -C ../../ 2>&1 >/dev/null || exit
cp ../../webserv ./;

# execute
if [ "${OS}" = "Linux" ]; then
    valgrind --log-file="valgrind.log" --leak-check=yes ./webserv > exe.log 2>&1 &
else
    echo "Only Linux is supported..."
    exit 1
    ./webserv > /dev/null 2>&1 &
fi

# wait
echo -n "waiting for starting test"
while [ `grep "$SERVER_LAUNCH_STR" exe.log | wc -l` -eq 0 ]; do
    echo -n '.'
    sleep 0.2
done
echo

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
    echo -n "waiting for valgrind exit";
    while [ `ps | grep 'memcheck' | awk '{print $1}'` ]; do
        echo -n "."
        sleep 0.2;
    done
    echo

    cat "valgrind.log" | grep ERROR | grep -v " 0 errors " > /dev/null
    result=$?
    if [ "$result" = "0" ]; then
        echo -e "valgrind: ${RED}KO!!!!!!!!!!:${NC}"
    else
        echo -e "valgrind: ${GREEN}OK${NC}"
    fi
else
    echo "todo for MacOS"

    pid=`ps | grep 'memcheck' | grep -v grep | awk '{print $1}'`;
    kill ${pid};
fi
