# Makefile for server.cpp and client.cpp
server: server.c
	gcc -Wall -o server server.c -lwiringPi

client: client.c
	gcc -Wall -o client client.c -lwiringPi
