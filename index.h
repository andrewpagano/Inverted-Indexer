#ifndef INDEX_H_
#define INDEX_H_
/***
 *CS214 Assignment 2: Recursive Indexing
 *due 3/29
 *
 *@author Andrew Pagano
 *@author Thurgood Kilper
 */

typedef struct node{
    char *string;
    struct node *next;
    struct node *list;
	int count;
}node;

node * sortedList;
static char * arg;
static char *dirp;
static char *fp;

/***
 *function declarations
 */
void listInsert(node * temp);
node* stringInsert(node * head, char *in);
int stringCompare(char *s1, char *s2);
void printList(node * listhead);
void cleanList(node * listhead);
void openFile(char * pill);
void openDirectory(char * pill);

#endif /* INDEX_H_ */