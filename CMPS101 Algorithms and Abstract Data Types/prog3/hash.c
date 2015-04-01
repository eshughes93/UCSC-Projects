//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hash.h"
#include "bookCountList.h"
#include "bookListList.h"

//Hash table struct. has a length, a load which is the number of elements contained inside, and an array of lists which is the actual table
struct hashTable {
	long length;
	int load;
	chainList *array;
};

//constructor for the hash table
// Pre: none
// Post: returns a new, empty hash table
hashref newHashTable(long length){
	hashref new = malloc(sizeof(struct hashTable));
	assert(new != NULL);
	new->length = length;
	new->load = 0;
	new->array = malloc(new->length * sizeof(chainList));
	assert(new->array != NULL);
	for(long index = 0; index < new->length; index++){
		new->array[index] = NULL;
	}
	return new;
}

//multiplicative hashing, uses a constant (sqrt(5)-1)/2 multiplied with the bookID, takes the fractional part of that product and multiplies it with the table size to get the hashcode
long hashcode(hashref h, long bookID){
	double constant = bookID*((sqrt(5) - 1) / 2);
	double a = constant - ((long)constant);	//subtract the integer part, leaves the fractional. same as modulo 1, but we can't do that with double-precision floating point numbers
	long hashcode = h->length*a;
	//printf("%ld \n", hashcode);
	return hashcode;
}

//frees memory for hash table
void freeHashTable(hashref h){
	for(long i = 0; i < h->length; i++){
		if(h->array[i] != NULL){
			freeChainList(h->array[i]);
		}
	}
	free(h->array);
	free(h);
}

//inserts a bookID into the hash table. 
void insertHash(hashref h, long bookID){
	long index = hashcode(h, bookID);
	if(h->array[index] == NULL){
		h->array[index] = newChainList();
	}
	chainList list = h->array[index];
	chainAdd(list, bookID);
	h->load++;
}


//updates the copurchases in the given list, the most recently purchased book will always be the last in the provided list when calling this function
void updateCopurchases(hashref h, countList books){
	long newBook = getLast(books);
	long index = hashcode(h, newBook);
	chainList bookList = h->array[index];
	moveFirst(books);
	while(!atLast(books)){
		long oldBook = getCurrent(books);
		long oldIndex = hashcode(h, oldBook);
		chainList oldBookList = h->array[oldIndex];
		copurchase(oldBookList, oldBook, newBook);
		copurchase(bookList, newBook, oldBook);
		moveNext(books);
	}
}

//gets the highest count of the provided bookID, passes the customer's previously bought books as to not recommend something already purchased
long recommendation(hashref h, long bookID, countList bought){
	long index = hashcode(h, bookID);
	chainList bookList = h->array[index];
	return chainGetHighestCount(bookList, bookID, bought);
}

//prints book list of given bookID
void searchTable(hashref h, long bookID){
	long index = hashcode(h, bookID);
	if(h->array[index] != NULL){
		printBookList(h->array[index], bookID);
	} else {
		printf("No book found for %ld\n", bookID);
	}
}

