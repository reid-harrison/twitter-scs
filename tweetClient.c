/*///////////////////////////////////////////////////////////
*
* FILE:		    ttweet.c
* AUTHOR:	    Reid Harrison
* PROJECT:	    CS 3251 (Networking 1) Project 2
* DESCRIPTION:  TTweet Client
* REFERENCE:    TCP/IP Sockets in C (Donahoo and Calvert)
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>        /* for printf() and fprintf() */
#include <sys/types.h>
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include "tweet.h"

/* Constants */
#define RCVBUFSIZE 512	        /* The receive buffer size */
#define SNDBUFSIZE 512	        /* The send buffer size */

/* The main function */
int main(int argc, char *argv[]) {

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* The server address */
    
    char *option; /*Holds the option argument */
    char *addr; /*Holds the address argument */
    int servPort; /*Holds the port argument */
    char *subject;
    char *tweet; /*Holds the tweet argument */

    char sndbuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvbuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int sub = 0;
    int downloaded = 0;
    int differentLists = 0;
    int i = 0;
    int noTweetMsg = 1;
    
    list *baseList = makeList();
    list *newList;
    
    /* Get the arguments from the command line */
    if (argc != 6) 
    {
	    printf("Incorrect input format.\n");
	    exit(1);
    }
    option = argv[1];
    
    //If the option given is not valid
    if (!((strcmp(option, "-u") == 0) || (strcmp(option, "-d") == 0))) {
        puts("Invalid argument\n");
        exit(1);
    }
    
    if (strcmp(option, "-d") == 0) {
        if ((strcmp(argv[2], "-nosub") != 0) && (strcmp(argv[2], "-sub") != 0)) {
            puts("Invalid subscription argument!\n");
            exit(1);
        }
        
        if (strcmp(argv[2], "-nosub") != 0) {
            sub = 1;
        }
        
        addr = argv[3];
        servPort = atoi(argv[4]);
        subject = argv[5];
    }
    
    //If the upload option is chosen but a tweet is not entered
    if (strcmp(option, "-u") == 0) {
        addr = argv[2];
        servPort = atoi(argv[3]);
        tweet = argv[4];
        subject = argv[5];
        
        if (tweet == NULL || subject == NULL) {
           puts("Invalid argument\n");
           exit(1);
        }
        
        //If a tweet contains invalid characters
        if (tweet != NULL && ((strstr(tweet, "<?>") != NULL) || (strstr(tweet, "<?d?>") != NULL))) {
            puts("Invalid tweet.\n");
            exit(1);
        }
        
	}
	
    if (subject == NULL || !(strcmp(subject, "eat") == 0 || strcmp(subject, "sleep") == 0 || strcmp(subject, "networking") == 0)) {
        puts("Invalid Subject!\n");
        puts("Valid subjects: eat, sleep, networking\n");
        exit(1);
    }

    memset(sndbuf, 0, SNDBUFSIZE);
    
    /* If the user has chosen the upload option */
    if (strcmp("-u", option) == 0) {
    
        /* Create a new TCP socket*/
        clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSock < 0) {
            printf("Error creating socket.\n");
            exit(1);
        }
        
        /* Construct the server address structure */
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        
        int rtnVal = inet_pton(AF_INET, addr, &serv_addr.sin_addr.s_addr);
        if (rtnVal == 0) {
            printf("Error binding socket.\n");
            exit(1);
        }
        
        // Assign the port
        serv_addr.sin_port = htons(servPort);
        
        /* Establish connection to the server */
        if (connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            printf("Error connecting to server.\n");
            exit(1);
        }
        
        puts("Uploading tweet...");
        
        strcpy(sndbuf, "upload<?>");
        i += strlen("upload<?>");
        strcpy(sndbuf + i, subject);
        i += strlen(subject);
        strcpy(sndbuf + i, "<?>");
        i += strlen("<?>");
        strcpy(sndbuf + i, tweet);
        i += strlen(tweet);
        strcpy(sndbuf + i, "<?>");
        
        /* Send the tweet to the server */
        ssize_t numBytes = send(clientSock, sndbuf, SNDBUFSIZE, 0);
        if (numBytes < 0) {
            puts("Tweet uploading unsuccessful.");
            exit(1);
        } else {
            puts("Tweet uploaded!");
        }
        close(clientSock);
    } else {
        puts("Download tweets...");
        
        while(downloaded == 0 || sub == 1) {
            i = 0;
            /* Create a new TCP socket*/
            clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (clientSock < 0) {
                printf("Error creating socket.\n");
                exit(1);
            }
            
            /* Construct the server address structure */
            memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            
            int rtnVal = inet_pton(AF_INET, addr, &serv_addr.sin_addr.s_addr);
            if (rtnVal == 0) {
                printf("Error binding socket.\n");
                exit(1);
            }
            
            // Assign the port
            serv_addr.sin_port = htons(servPort);
            
            /* Establish connection to the server */
            if (connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
                printf("Error connecting to server.\n");
                exit(1);
            }
            
            strcpy(sndbuf, "download<?>");
            i += strlen("download<?>");

            strcpy(sndbuf + i, subject);
            i += strlen(subject);
            strcpy(sndbuf + i, "<?>");
            
            ssize_t numBytes = send(clientSock, sndbuf, SNDBUFSIZE, 0);
            if (numBytes < 0) {
                puts("Download unsuccessful.");
                exit(1);
            }
            
            unsigned int totalBytes = 0;
        
            char *result;
        
            /* Receive and print response from the server */
            while (totalBytes == 0) {
                numBytes = recv(clientSock, rcvbuf, RCVBUFSIZE, 0);
                totalBytes += numBytes;
                rcvbuf[numBytes] = '\0';
                result = (char*)malloc(strlen(rcvbuf));
	            strcpy(result, rcvbuf);
            }
            
            if (strcmp(result, "<?>") == 0) {
                if (noTweetMsg == 1) {
                    puts("There are no tweets for this subject.");
                    noTweetMsg = 0;
                }
                usleep(1000000);
                continue;
            }
            
            noTweetMsg = 1;
            
            newList = makeList();
            
            tweet = strtok(result, "<?>");
            addTweet(newList, makeNode(tweet));
            
            while((tweet = strtok(NULL, "<?>")) != NULL) {
                addTweet(newList, makeNode(tweet));
            }
            
            downloaded = 1;
            
            if (cmpLists(baseList, newList) != 0) {
                baseList = copyList(newList);
                differentLists = 1;
            }
            
            if (differentLists == 1) {
                puts("=============================================================");
                printTweets(baseList);
                differentLists = 0;
            }
            
            close(clientSock);
            
            if (sub == 1) {
                usleep(1000000);
            }
        }
        
    }
    
    return 0;
}

