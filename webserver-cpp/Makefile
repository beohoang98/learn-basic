main:=src/main.cpp src/controller.h
lib:=$(shell find src/http/*.h src/http/*.cpp)

all:
	g++ -DWORKING_DIR='"$(shell pwd)/src/"' -g $(lib) $(main) -o bin/main
start:
	make && bin/main
run: #only
	bin/main