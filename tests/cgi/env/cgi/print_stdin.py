#!/usr/bin/python3
s = input()

# headers
print("Status: 200", end="\r\n")
print("Content-type: text/plain", end="\r\n")
print("Connection: close", end="\r\n")
print(f"Content-Length: {len(s)}", end="\r\n")
print("", end="\r\n")

print(s, end="")
