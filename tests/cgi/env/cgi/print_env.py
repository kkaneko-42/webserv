#!/usr/local/bin/python3
import os

body = f"PATH_INFO: {os.environ['PATH_INFO']}\n"
body += f"QUERY_STRING: {os.environ['QUERY_STRING']}\n"

# header
print("Status: 200", end="\r\n")
print("Content-type: text/plain", end="\r\n")
print("Connection: close", end="\r\n")
print(f"Content-Length: {len(body)}", end="\r\n")
print("", end="\r\n")

# body
print(body, end="")
