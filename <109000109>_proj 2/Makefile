#!/bin/bash

# specify the case you want to test here!
case=case1
#version=basic
version=advance
# Build and run your final project!
NTHU_bike:
	g++ -g -std=c++11 -o ./bin/main ./src/*.cpp
	./bin/main $(case) $(version)
	
RunBasicAll:
	for number in 1 2 3 4 5 6 7 8; do \
		./bin/main case$$number $(version);\
	done