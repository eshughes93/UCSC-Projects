//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>

#include "bookCountList.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv){
	countList list = newList();
    if(isEmpty(list)){
        printf("Successfully created an empty list\n");
    }
    //4 8 15 16 23 42
    long first = 4;
    long second = 8;
    long third = 15;
    long fourth = 16;
    long fifth = 23;
    long sixth = 42;

    add(list, first);
    add(list,second);
    add(list,fourth);
    add(list,third);
    add(list,fifth);
    add(list,sixth);    
    add(list,sixth);
    add(list,sixth);
    add(list,sixth);
    add(list,sixth);

	printf("printing the list: \n");
	printList(list);
	printf("sorting the list by count, printing post-sort:\n");
    sortList(list);
    printList(list);
}
