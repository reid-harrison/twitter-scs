#################################################################
##
## FILE:	Makefile
## PROJECT:	CS 3251 Project 2
## DESCRIPTION: Compiles "twitter" service
##
#################################################################

CC=gcc

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
	LDFLAGS=-lsocket -lnsl
    endif

all: client server 

client: linkedlist.o tweetClient.c tweet.h
	$(CC) tweetClient.c linkedlist.o -o client

server: linkedlist.o tweetServer.c tweet.h
	$(CC) tweetServer.c linkedlist.o -o server

clean:
	rm -f linkedlist client server *.o
