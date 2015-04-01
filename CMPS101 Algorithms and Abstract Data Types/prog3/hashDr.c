//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>
//Driver file for hash.c/hash.h
#include "bookCountList.h"
#include "bookListList.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv){
	hashref hashtable = newHashTable(100); //makes a new hash table
	countList listArray[20];		//a array of countLists, similar to customerArray with lists of purchase history
	for(int i = 0; i < 20; i++){
		listArray[i] = newList();	//initialize lists in the array
	}
	for(int j = 0; j < 3; j++){		//we want to do this three times so that the same customers make multiple purchases
		for(int i = 0; i < 20; i++){		
			long random = (long) rand() % 5;	//random number between 1 and 15
			add(listArray[i], random);		//add the random number to the list array at index i
			insertHash(hashtable, random);		//insert the same random number into the hash table
			updateCopurchases(hashtable, listArray[i]);	//update counts
		}
	}
	//step through the possible random numbers and search for them in the hash table
	for(int i = 1; i < 5; i++){
		printf("Searching the table for bookID %d, printing copurchase list: \n", i);
		searchTable(hashtable, i);
	}
	for(int i = 0; i < 20; i++){
		freeList(listArray[i]); //free the lists
	}
	freeHashTable(hashtable); //free the hash table
}
