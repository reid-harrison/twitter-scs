/*///////////////////////////////////////////////////////////
*
* FILE:		    ttweetServer.c
* AUTHOR:	    Reid Harrison
* PROJECT:	    CS 3251 Project 1
* DESCRIPTION:	TTweet Server
* REFERENCE:    TCP/IP Sockets in C (Donahoo and Calvert)
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>	  /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>	  /* supports all sorts of functionality */
#include <unistd.h>	  /* for close() */
#include <string.h>	  /* support any string ops */
#include "tweet.h"

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 128		/* Your name can be as many as 40 chars*/

struct list *eatList;
struct list *sleepList;
struct list *networkingList;

/* The main function */
int main(int argc, char *argv[]) {

    int serverSock;			/* Server Socket */
    int clientSock;			/* Client Socket */
    struct sockaddr_in servAddr;	/* Local address */
    struct sockaddr_in clntAddr;	/* Client address */
    int servPort;	/* Server port */
    unsigned int clntLen;		/* Length of address data struct */
    
    eatList = makeList();
    sleepList = makeList();
    networkingList = makeList();

    char tweetBuf[RCVBUFSIZE];		/* Buff to store tweet */
    char responseBuf[SNDBUFSIZE];
    
    char *command;
    char *subject;
    char *newTweet;
    
    puts("Server started.");
    
    servPort = atoi(argv[1]);
    
    /* Create new TCP Socket for incoming requests*/
    if((serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    	exit(1);
    
    /* Construct local address structure*/
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);
    
    /* Bind to local address structure */
    if(bind(serverSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
        puts("Unable to bind socket to address.");
    	exit(1);
    }

    /* Listen for incoming connections */
    if(listen(serverSock, 5) < 0) {
        puts("Received bad data from socket.");
        exit(1);
    }
    
    /* Loop server forever*/
    while(1) {   
        clntLen = sizeof(clntAddr);
        
	    /* Accept incoming connection */
	    if ((clientSock = accept(serverSock, (struct sockaddr*) &clntAddr, &clntLen)) < 0) {
		    exit(1);
		}

        char clntName[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL) {
	        printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
	    } else {
	        puts("Unable to get client address\n");
	    }
	        
	    int msgSize = 0;
	    /* Extract the tweet and put it in tweetBuf */
	    if((msgSize = recv(clientSock, tweetBuf, BUFSIZE, 0)) < 0) {
	        puts("Message not received properly\n");
		    exit(1);
	    }
	    
	    if (tweetBuf != NULL) {
	        command = strtok(tweetBuf, "<?>");
	        printf("%s\n", tweetBuf);
	        if (command != NULL) {
	            subject = strtok(NULL, "<?>");
	        }
	        
	        if (subject != NULL && (strcmp(command, "upload") == 0)) {
	            newTweet = strtok(NULL, "<?>");
	        }
	    } else {
	        puts("ERROR: TweetBuf is NULL");
	    }
	    memset(&responseBuf, 0, SNDBUFSIZE);
	    
	    if (command != NULL) {
	        /**************
	         * DOWNLOAD    
	         **************/
	        if (strcmp(command, "download") == 0) {
	            if (strcmp(subject, "eat") == 0) {
	                strcpy(responseBuf, listToStr(eatList));
	            } else {
	                if (strcmp(subject, "sleep") == 0) {
	                    strcpy(responseBuf, listToStr(sleepList));
	                } else {
	                    if (strcmp(subject, "networking") == 0) {
	                        strcpy(responseBuf, listToStr(networkingList));
	                    }
	                }
	            }
	            
	            if (send(clientSock, responseBuf, strlen(responseBuf), 0) != strlen(responseBuf)) {
	                puts("Download did not complete");
		        } else {
		            puts("Tweets sent to client.");
		        }
	        } else {
	            /**************
	            * UPLOAD    
	            **************/
	            if (strcmp(command, "upload") == 0) {
	                if (strcmp(subject, "eat") == 0) {
	                    puts("EAT ADD");
	                    addTweet(eatList, makeNode(newTweet));
	                } else {
	                    if (strcmp(subject, "sleep") == 0) {
	                        puts("SLEEP ADD");
	                        addTweet(sleepList, makeNode(newTweet));
	                    } else {
	                        if (strcmp(subject, "networking") == 0) {
	                            puts("NETWORKING ADD");
	                            addTweet(networkingList, makeNode(newTweet));
	                        }
	                    }
	                }
	            }
	        }
	    }
    }

	close(clientSock);
}


