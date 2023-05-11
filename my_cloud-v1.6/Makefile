
all:server client
CC=gcc
CFLAGS=-g -Wall

server:net.c filetransfer.c  server.c 

client:net.c filetransfer.c  client.c
install:
	cp client client-mod/
	cp server server-mod/
	
clean:
	rm server client
