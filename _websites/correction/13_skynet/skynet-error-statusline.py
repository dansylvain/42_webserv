#!/usr/bin/env python3

import sys

try:
    with open("skynet.html", 'r') as html:
        output = html.read()
        
    content_length = len(output.encode('utf-8'))
    print("HHTTP/1.1 200 OK\r")
    print("Content-Type: text/html; charset=utf-8\r")
    print(f"Content-Length: {content_length}\r")
    print("\r")
    print(output)

except Exception as e:
    sys.exit(1) 
