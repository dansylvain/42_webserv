#!/usr/bin/env python3

import sys

sys.stderr = open('/dev/null', 'w')

try:
	with open("skynet.html", 'r') as html:
		output = html.read()
		
	content_length = len(output.encode('utf-8'))
	print("HTTP/1.1 200 OK\r")

	sys.exit(1)
	
	print("Content-Type: text/html; charset=utf-8\r")	
	print(f"Content-Length: {content_length}\r")
	print("\r")
	print(output)

except Exception as e:
	sys.exit(1) 