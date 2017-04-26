#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include "index.h"

/*
 * creates or updates list of files which contain word
 */
void listInsert(node * temp){
        if(temp->list==NULL){
        	node *newlist = malloc(sizeof(node));
        	temp->list = newlist;
        	newlist->string = fp;
        	newlist->count = 1;
        }
        else{
        	node *newlist = malloc(sizeof(node));
        	newlist->string = fp;
        	node *last = temp;
        	if(newlist->count == 0){
        		(newlist->count)++;
        	}
        	
        	while(last->list!=NULL){
			if(strcmp(last->list->string, fp)==0){
				++(last->list->count);
				return;
			}
        		last = last->list;
        	}
        	last->list = newlist;
        }

return;
}

/*
 * inserts new word into linked list of all words found
 */
node* stringInsert(node * head, char *in)
{
/*** 
 *check empty list
 */
    if(head==NULL){
        node *newnode = malloc(sizeof(node));
        newnode->string=malloc(strlen(in)+1);
        strcpy(newnode->string, in);
        newnode->next=NULL;
        head=newnode;
        
        
        listInsert(newnode);
	
        return head;
    }
/***
 *check head node insertion
 */
    if(stringCompare(in, head->string)<0 || stringCompare(in, head->string)==0){
    	if(stringCompare(in, head->string)==0){
    		listInsert(head);
    		return head;
    	}
        node *newnode = malloc(sizeof(node));
        newnode->string=malloc(strlen(in)+1);
        strcpy(newnode->string, in);
        newnode->next=head;

	listInsert(newnode);

        return newnode;
    }
    
    node *current = head;
    node *cnext = head->next;

/***
 *iterate through list and insert
 */
    while(cnext != NULL){
        if(stringCompare(in, cnext->string)<0 || stringCompare(in, cnext->string)==0){
        	if(stringCompare(in, cnext->string)==0){
        		listInsert(cnext);
        		return head;
        	}
            node *newnode = malloc(sizeof(node));
            newnode->string=malloc(strlen(in)+1);
            strcpy(newnode->string, in);
            newnode->next=cnext;
            current->next=newnode;

		listInsert(newnode);

            return head;
        }else if(stringCompare(in, cnext->string)>0){
            current=cnext;
            cnext=current->next;
        }
    }

/***
 *check last node insertion
 */
    if(stringCompare(in, current->string)>0){
        node *newnode = malloc(sizeof(node));
        newnode->string=malloc(strlen(in)+1);
        strcpy(newnode->string, in);
        newnode->next=NULL;
        current->next=newnode;
	
	listInsert(newnode);
    }

    return head;
}

/***
 *compare string nodes for sorting
 */
int stringCompare(char *s1, char *s2){
    return strcmp(s1, s2);
}

/***
 *print the list
 */
void printList(node * listhead) {
    node * current = listhead;
    FILE * test = fopen(arg, "r");
    if(test != NULL){
    	printf("Output file '%s' already exists. Exiting.\n", arg);
    	exit(1);
    }
    int fd = open(arg, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    int result = 0;
    int namelen = 0;
    result = write(fd,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n",39);
    result = write(fd,"<fileIndex>\n",12);

    while (current != NULL) {
		node * ff = current->list;
		result = write(fd,"\t<word text=\"",13);
		namelen = strlen(current->string);
		result = write(fd,current->string,namelen);
		result = write(fd,"\">\n",3);
        
        while(ff!=NULL){
        	int r = 1;
        	int q = ff->count;
        	int * n = NULL;
        	n = &ff->count;
            while (*n > 9) {
                *n /= 10;
                r++;
            }
        	char * buf = (char*)calloc(r+2,sizeof(char));
        	sprintf(buf,"%d",q);
        	
        	result = write(fd,"\t\t<file name=\"",14);
        	namelen = strlen(ff->string);
        	result = write(fd,ff->string,namelen);
        	result = write(fd,"\">",2);
        	result = write(fd,buf,r);
        	result = write(fd,"</file>\n",8);
        	ff = ff->list;
	}

        result = write(fd,"\t</word>\n",9);
        current = current->next;
    }
    result = write(fd,"</fileIndex>\n",13);
    close(fd);
    return;
}

/***
 *iterates through list and frees allocated memory
 */
void cleanList(node * listhead){

	node * current;
	node * temp;
	
	while (listhead != NULL) {
		while(listhead->list != NULL){
			temp = listhead->list;
			listhead->list = listhead->list->list;
			free(temp);
		}
		current = listhead;
		listhead = listhead->next;
		free(current->string);
		free(current);
	}
}



void openFile(char * pill){
	char *input = pill;
	FILE *file = NULL;
	file = fopen(input, "r");
	fp = basename(pill);
	
	/*
	 * creates single string of entire contents of file to parse
	 * 
	 */
	int length = 0;
	while(1){
		fgetc(file);
		if( feof(file) )
		      { 
		         break ;
		      }
		length++;
	}
	char * string = (char *) calloc(length+1, sizeof(char));
		memset(string, '\0', length+1);
		fseek(file, SEEK_SET, 0);
		fread(string,length+1,1,file);

	int wordLength = 0;
	printf("String: %s\n\n",string);
	while(string[wordLength]!= '\0' && length>0){
/***
 *Finds valid word inside string and length of word.
 */		
		while(isalnum(string[wordLength])){
			wordLength++;
		}
/***
 *Turns word from master string into a valid lowercase string in order to sort. 
 */
		char *word;
		word =(char*)calloc(wordLength+1,  sizeof(char));
		memset(word, '\0', wordLength+1);
		strncpy(word, string, wordLength);
		int i;

		if(!isalpha(word[0])){
			wordLength = 0;
		}

		for(i = 0; word[i]; i++){
			if(isalpha(word[i])){
				word[i] = tolower(word[i]);
			}
		}


/***
 *Creates temporary holder of remaining master string in order to loop again to find more words.
 */
		char * stringHolder;
		stringHolder = string;
		length-=wordLength+1;
		strcpy(string,stringHolder+1+wordLength);
/***
 *Puts word into Linked list and sorts.
 */
		if(wordLength>0){
			sortedList=stringInsert(sortedList, word);
		}
		wordLength = 0;

		free(word);
	}
		free(string);	
		
		return;
}

/*
 * opens directory and opens all files and directories inside.
 */
void openDirectory(char * pill){
	DIR *pdir = NULL;
	struct dirent *pent = NULL;
	dirp = pill;
	pdir = opendir (pill);
	if (pdir == NULL) { 
		printf ("\nERROR! pdir could not be initialized correctly %s \n",strerror(errno));
		exit (1); 
	}
	
	pent = readdir(pdir);
	printf("Name: %s\n",pill);
	
	/*
	 * While there is a file or directory in the current open directory,
	 * open the file and parse or go to the next directory.
	 */
	while(pent !=NULL){
	if(pent!= NULL){	
		if(strcmp(pent->d_name, ".") == 0 || strcmp(pent->d_name,"..") == 0){
			pent = readdir(pdir);
			pent = readdir(pdir);
			if (pent == NULL) { 
				printf ("empty directory: no file found\n");
				return; 
			}
		}
	
		/*
		 * Appends path to directory or file to open properly.
		 */
		
		if(pent->d_type == DT_DIR){
			printf("\nopen directory: %s\n", pent->d_name);
			int len = strlen(pent->d_name) + strlen(pill);
			char *apple = (char*)calloc(len+2,sizeof(char));
			strcat(apple,pill);
			char * xxx = pill+(strlen(pill)-1);
			if(strcmp(xxx,"/")){
				strcat(apple,"/");
			}
			strcat(apple,pent->d_name);
			printf("\nopen directoryapp: %s\n", apple);
			openDirectory(apple);
	
		}
		else if(pent->d_type == DT_REG){
			printf("\nopen file: %s\n", pent->d_name);
			int len = strlen(pent->d_name) + strlen(pill);
			char *apple = (char*)calloc(len+2,sizeof(char));
			strcat(apple,pill);
			char * www = pill+(strlen(pill)-1);
			if(strcmp(www,"/")){
				strcat(apple,"/");
			}
			strcat(apple,pent->d_name);
			printf("\nopen fileapp: %s\n", apple);
			openFile(apple);
		}
		else{
			printf("Error, Neither Dir or File\n");
		}
		
		pent = readdir(pdir);
	}
	}
	closedir(pdir);
	return;
}



int main(int argc, char **argv){
	/*
	 * Decides if given a directory or file and opens it.
	 */
	arg = argv[1];
	char * begin = NULL;
	struct stat path_stat;
	stat(argv[2], &path_stat);
	int h = S_ISREG(path_stat.st_mode);
	begin = argv[2];
		if(h == 1){
			openFile(begin);
		}
		else if (h == 0){
			openDirectory(begin);
		}
		else{
			printf("Error, not a file or dir.\n");
		}

		/*
		 * Prints list to given file, and then frees all memory in the linked lists.
		 */
   		printList(sortedList);
		cleanList(sortedList);

	return 0;
}