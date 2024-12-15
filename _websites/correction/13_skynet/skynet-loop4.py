#!/usr/bin/env python3

import sys

try:

	sys.stderr = open('/dev/null', 'w')
	with open("skynet.html", 'r') as html:
		output = html.read()
		
	content_length = len(output.encode('utf-8'))
	print("HTTP/1.1 200 OK\r")

	while True:
		...
		
	print("Content-Type: text/html; charset=utf-8\r")	
	print(f"Content-Length: {content_length}\r")
	print("\r")
	print(output)

except Exception as e:
	sys.exit(1) 
