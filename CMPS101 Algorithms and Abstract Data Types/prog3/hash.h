//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>

#ifndef __HASH_H__
#define __HASH_H__
#include "bookCountList.h"

typedef struct hashTable *hashref;

//Hash table constructor, post: returns a new, empty hash table
hashref newHashTable (long length); 

//frees the hash table
void freeHashTable(hashref h);

//hashcode calculator, calculates hashcode dependent on table size and given bookID
long hashcode(hashref h, long bookID);

//inserts a bookID into the hash table, post: hash table is not empty
void insertHash(hashref h, long bookID);

//updates copurchases in hash table based on given list, most recent customer purchase will always be the last one in the given list when calling this function
void updateCopurchases(hashref h, countList books);

//returns bookID with the highest count, references customer's previous purchases as to avoid recommending already purchased items
//this function will return a value of -1 if the customer has already purchased all recommended books
long recommendation(hashref h, long bookID, countList bought);

//prints the copurchase list of given bookID
void searchTable(hashref h, long bookID);


#endif
