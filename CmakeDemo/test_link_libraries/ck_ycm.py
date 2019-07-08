#!/usr/bin/python3
 
import json
cmakeJson = 'build/compile_commands.json'
with open(cmakeJson, 'r') as f:
    data = json.load(f)
print (type(data))
print (data[0]['command'])
command = data[0]['command'].split()

for it in command:
    if(it.find("-I") == 0):
        print(it)
