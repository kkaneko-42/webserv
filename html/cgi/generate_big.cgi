#!/usr/local/bin/python3
import os

str = "hello, world\n"
str += f"PATH_INFO: {os.environ['PATH_INFO']}\n"
str += f"QUERY_STRING: {os.environ['QUERY_STRING']}\n"
str += str * 2000

print("Status: 200", end="\r\n")
print("Content-type: text/plain", end="\r\n")
print(f"Content-Length: {len(str)}", end="\r\n")
print("", end="\r\n")

print(str)
