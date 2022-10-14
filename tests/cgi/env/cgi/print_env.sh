#!/bin/bash

str=`echo -n "PATH_INFO: ${PATH_INFO}\n"`
str+=`echo -n "QUERY_STRING: ${QUERY_STRING}\n"`

# headers
echo -e -n "Status: 200\r\n"
echo -e -n "Content-type: text/plain\r\n"
echo -e -n "Connection: close\r\n"
echo -e -n "Content-Length: "
echo -e -n `echo -n "${str}" | wc -m`
echo -e -n "\r\n"
echo -e -n "\r\n"

# body
echo -n -e "${str}"
