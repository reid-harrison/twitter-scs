#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tweet.h"

list *makeList() {
    list *newList = malloc(sizeof(list));
    
    if (newList == NULL) {
        printf("OUT OF MEMORY");
        exit(1);
    }
    
    newList->head = NULL;
    newList->size = 0;
    
    return newList;
}

node *makeNode(char *tweet) {
	node *newNode = malloc(sizeof(node));
	char *tempTweet = (char *) malloc(strlen(tweet));
	strcpy(tempTweet, tweet);
	
	if (newNode == NULL) {
		printf("OUT OF MEMORY");
		exit(1);
	}

	newNode->tweet = tempTweet;
	newNode->next = NULL;
	newNode->prev = NULL;
	
	return newNode;
}

void addTweet(list *list, node *node) {
    struct node *temp;

	if (list == NULL || node == NULL) {
		return;
	}
	
	if (list->head == NULL) {
		list->head = node;
		list->head->prev = node;
		list->head->next = node;
		list->size = 1;
		return;
	}
	
	if (list->size >= 10) {
	    temp = list->head;
	    temp->prev->next = temp->next;
	    list->head = temp->next;
	    list->head->prev = temp->prev;
	    
	    free(temp);
	}
	
	node->next = list->head;
	node->prev = list->head->prev;
	node->prev->next = node;
	node->next->prev = node;
	
	if (list->size < 10) {
	    list->size++;
	}
	
	return;
}

void printTweets(list *list) {
   	int i = 1;
   	node *curr;
	
	if (list == NULL || list->head == NULL) {
		printf("EMPTY LIST\n");
		return;
	}
	
	curr = list->head;
	
	do {
	    printf("%d%s", i, ". ");
	    //printf("%s", ". ");
		printTweet(curr);
		curr = curr->next;
		i++;
	} while(curr != list-> head);
}


void printTweet(node *node) {
  	if (node == NULL) {
		printf("NULL tweet\n");
		return;
	}
	
	printf("%s\n", node->tweet);
}

char *listToStr(list *list) {
    int i = 1;
    char *tweet = NULL;
    char *allTweets = NULL;
   	node *curr;
   	
	if (list == NULL || list->head == NULL) {
		tweet = "<?>";
		return tweet;
	}

	curr = list->head;
	
	do {
	    tweet = (char *) malloc(strlen(curr->tweet));

	    if (allTweets == NULL) {
	        allTweets = (char *) malloc(strlen(tweet) + 3);
	    }
	    
	    strcpy(tweet, curr->tweet);
	    tweet = strcat(tweet, "<?>");
	    allTweets = strcat(allTweets, tweet);
		curr = curr->next;
	} while(curr != list->head);
	
	return allTweets;
}

list *copyList(list *list) {
    struct list *newList;
    node *curr;
    
    if (list == NULL || list->head == NULL) {
        return NULL;
    }
    
    newList = makeList();
    curr = list->head;
    
    do {
	    addTweet(newList, makeNode(curr->tweet));
	    
	    curr = curr->next;
	} while(curr != list->head);
	
	return newList;
}

void freeList(list *list) {
	node* temp1;
	node* temp2;
	
	if (list == NULL || list->head == NULL){
		return;
	}
	
	if (list->head->next != list->head) {
	
		temp1 = list->head->next;
		
		do {
			temp2 = temp1->next;
			free(temp1);
			temp1 = temp2;
		} while (temp1 != list->head);
	} else {
		temp1 = list->head;
	}
	
	free(temp1);
	list->head = NULL;
}

int listsEqual(list *list1, list *list2) {
    node *curr1;
    node *curr2;

    if (list1 == NULL || list2 == NULL) {
        return -1;
    }

    if (list1->size != list2->size) {
        return 0;
    }
    
    curr1 = list1->head;
    curr2 = list2->head;
    
    do {
	    if (strcmp(curr1->tweet, curr2->tweet) != 0) {
	        return 0;
	    }
	    
	    curr1 = curr1->next;
	    curr2 = curr2->next;
	} while(curr1 != list1->head);
	
	return 1;
}