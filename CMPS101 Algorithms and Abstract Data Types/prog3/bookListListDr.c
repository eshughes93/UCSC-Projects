//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>
// Driver File for bookListList.c/h , a list of lists, each node in the list contains a bookID and a list of copurchases made with that book
// the list of copurchases is the countList, in bookCountList.h
#include "bookCountList.h"
#include "bookListList.h"
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char **argv){
	chainList list = newChainList();
	printf("creating a new chainList\n");
	long first = 4;
	long second = 8;
	long third = 15;
	long fourth = 16;
	long fifth = 23;
	long sixth = 42;
	printf("adding 4, 8, 15, 16, 23, 42 to the list\n");
	//say these 6 books hash to same spot in table, all go in this linked list
	chainAdd(list, first);
	chainAdd(list, second);
	chainAdd(list, third);
	chainAdd(list, fourth);
	chainAdd(list, fifth);
	chainAdd(list, sixth);
	
	//customer 1 copurchases books one and two (first and second)
	printf("copurchasing books 4 and 8\n");
	copurchase(list, first, second);
	printf("printing copurchase list for book 4: \n");
	printBookList(list, first);
}
