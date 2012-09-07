#ifndef _LIST_H_DEF_
#define _LIST_H_DEF_

/*************************
** Datatype definitions **
**************************/

typedef struct node
{
	char *tweet;
	struct node *next;
	struct node *prev;
} node;

typedef struct list
{
    int size;
    struct node *head;
} list;

/** Creates a new, empty list **/
list *makeList();

/** Creates a new node with the given tweet **/
node *makeNode(char *tweet);

/** Adds a tweet to the given list **/
void addTweet(list *list, node *node);

/** Prints all tweets from the list **/
void printTweets(list *list);

/** Prints one tweet**/
void printTweet(node *node);

/** Converts a list to the string with a <?> token separating items **/
char *listToStr(list *list);

/** Returns a copied version of the given list **/
list *copyList(list *list);

/** Frees the memory allocated to the given list **/
void freeList(list *list);

/** Compares two lists. Returns -1 if they are different, 0 if they are the same **/
int cmpLists(list *list1, list *list2);

#endif
