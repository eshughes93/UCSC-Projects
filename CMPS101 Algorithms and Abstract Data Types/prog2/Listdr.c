//Evan Hughes [eshughes@ucsc.edu]
// Driver file for Linked List ADT
#include "List.h"

int main (int argc, char **argv){
    ListRef list = newList();
    if(isEmpty(list)){
        printf("Successfully created an empty list\n");
    }
    //4 8 15 16 23 42
    long firstTry = 4;
    long second = 8;
    long third = 15;
    long fourth = 16;
    long fifth = 23;
    long sixth = 42;

    insertBeforeFirst(list, firstTry);
    insertAfterLast(list,second);
    insertAfterLast(list,fourth);
    insertBeforeCurrent(list,third);
    moveLast(list);
    insertAfterCurrent(list,fifth);
    sortedInsert(list,sixth);

    //When they aren't commented out, the print statements in the List.c file 
    // for these functions proves their functionality
    movePrev(list);
    getFirst(list);
    getLast(list);
    getCurrent(list);
    getLength(list);
    
    printList(list);
    printf("Copying List A to List B\n");
    ListRef ListB = copyList(list);
    printf("List A: ");
    printList(list);
    printf("\nList B: ");
    printList(ListB);
    equals(list,ListB);
    printf("\nDeleting current\n");

    deleteCurrent(list);
    printList(list);
    moveFirst(list);
    moveNext(list);
    printf("\nMoving current, attempting to deleteCurrent again.\n");
    getCurrent(list);
    printf("Deleting current\n");
    deleteCurrent(list);
    printList(list);


    printf("\nDeleting first\n");

    deleteFirst(list);
    printList(list);
    printf("\nDeleting last\n");

    deleteLast(list);
    printList(list);

    printf("Comparing lists again.\n");
    equals(list,ListB);
    printf("List A: ");
    printList(list);
    printf("\nList B: ");
    printList(ListB);
    
    printf("\nFreeing Lists\n");
    freeList(list);
    freeList(ListB);

}

