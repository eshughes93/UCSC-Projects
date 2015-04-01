//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>

#include<stdio.h>
#include<stdlib.h>
#include "bookCountList.h"
#include "bookListList.h"
#include "hash.h"


void openFile(char *filename){
	FILE *input = fopen(filename, "r");  //open the file
	long numberOfCustomers = 0; //the number of customers provided in the file
	long numPurchases = 0;  //number of transactions as stated by the file
	long tableSize = 0;	//provided table size
	if(input == NULL){
		printf("error\n");
	} else {
		fscanf(input, "%ld", &numberOfCustomers); //initializing number of customers and number of purchases to their respective variables
		fscanf(input, "%ld", &numPurchases);
		fscanf(input, "%ld", &tableSize);
	}

	hashref bookHashTable = newHashTable(tableSize); //new hash table based on provided table size
	countList customerArray[numberOfCustomers+1]; //an array that is the size of the customer numbers + 1 to account for counting from 0 instead of 1
	for(long i = 1; i <= numberOfCustomers; i ++){
		customerArray[i] = newList();
	}
        
	for(int i = 0; i < numPurchases; i++){
		long customerID;
		long bookID;
		long print;
		fscanf(input, "%ld %ld %ld", &customerID, &bookID, &print); //initialize the customerID, bookID, and whether or not to print
		add(customerArray[customerID], bookID); //adds book to customer's purchase history
		insertHash(bookHashTable, bookID);	//inserts book into hash table
		updateCopurchases(bookHashTable, customerArray[customerID]);	//updates copurchases
		if(print){ //if print == 1, we print
			long recBook = recommendation(bookHashTable, bookID, customerArray[customerID]); //gets the recommended bookID
			if(recBook == -1){
				printf("Customers buying %ld read too much and have already purchased all possible recommendations\n", bookID); //if the customer has already bought all recommended books
			} else {
				printf("Customers buying %ld also bought %ld\n", bookID, recBook); //prints recommended bookID
			}
		}
	}
	for(long i = 1; i <= numberOfCustomers; i++){
		freeList(customerArray[i]); //free the lists containing each customer's purchase history
	}
	freeHashTable(bookHashTable); //free the hash table
	fclose(input);  //close the file we opened
}

//main function. simply steps through command line arguments and directs filenames to the openFile function.
int main (int argc, char **argv){
    if(argc == 1){

	} else {
	    for(int i = 1; i < argc; i++){
			openFile(argv[i]);
		}
    }
}
