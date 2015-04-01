//Evan Hughes [eshughes@ucsc.edu]

#include<stdio.h>
#include<stdlib.h>
#include "List.h"


//This function reads through the file passed to it, creates an array dependent upon the number of customers stated in the file, then proceeds to read the rest of the file and add data to the array and linked list as appropriate.
void openFile(char *filename){
	FILE *input = fopen(filename, "r");  //open the file
	long numberOfCustomers = 0; //the number of customers provided in the file
	long numPurchases = 0;  //number of purhcases as stated by the file
	if(input == NULL){
		printf("error\n");
	} else {
		fscanf(input, "%ld", &numberOfCustomers); //initializing number of customers and number of purchases to their respective variables
		fscanf(input, "%ld", &numPurchases);
	}
	//printf("Number of customers: %ld\n", numberOfCustomers);
	//printf("Number of purchases: %ld\n", numPurchases);
	
	ListRef customerArray[numberOfCustomers+1]; //an array that is the size of the customer numbers + 1 to account for counting from 0 instead of 1
	for(long i = 1; i <= numberOfCustomers; i ++){
		customerArray[i] = newList();
	//	getLength(customerArray[i]);
	}
        
        //this for loop goes through each purchase, finds out what the customer and book ID are, 
        //then inserts in a sorted order the book ID in the appropriate index of the array. 
        //the appropriate index in the array is simply the customer ID.
	for(int i = 0; i < numPurchases; i++){
		long customerID;
		long bookID;
		fscanf(input, "%ld %ld", &customerID, &bookID);
		sortedInsert(customerArray[customerID], bookID);
	}
	
        //This is the actual program output. 
	printf("Customer ID       IDs of books purchased\n");
	for(long i = 1; i <= numberOfCustomers; i ++){
		printf("%ld                ", i);
		printList(customerArray[i]);
		printf("\n");
		freeList(customerArray[i]);
	}
	
	fclose(input);  //close the file we opened
}

//main function. simply steps through command line arguments and directs filenames to the openFile function.
int main (int argc, char **argv){
    if(argc == 1){
	    printf("No customers? Oh dear.\n");
	} else {
	    for(int i = 1; i < argc; i++){
			openFile(argv[i]);
		}
    }
}
