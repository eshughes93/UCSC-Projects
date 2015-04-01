// Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>
#ifndef __BOOKCOUNTLIST_H__
#define __BOOKCOUNTLIST_H__

#include <stdbool.h>

typedef struct List* countList;
typedef struct sortingArray* sorter;

countList newList (void);
   //
   // Constructor:    return a new valid empty list.
   // Precondition:   none;
   // Postcondition:  returns a properly constituted empty list.
   //
sorter newArray(void); //constructor for sortingArray struct. pre: none, post: returns an empty "sorter"

void freeList(countList);


/*** Access functions ***/

int isEmpty(countList L); // returns true is list is empty else returns false.
int offEnd(countList L); // returns true is current == NULL
int atFirst(countList L); // returns true if current == first and !offEnd()
int atLast(countList L); // returns true if current == last and !offEnd()
long getFirst(countList L); // return the first element; pre: !isEmpty()
int getFirstCount(countList L); //returns the count stored in the first element
long getLast(countList L); // return the bookID of the last element; pre: !isEmpty()
long getCurrent(countList L); // return the bookID of the current element pre: !offEnd()
int getLength(countList L); // return the length of the list
int has(countList L, long bookID); //checks to see if a bookID is contained within the given list. pre: !isEmpty()
/*** Manipulation procedures ***/
void makeEmpty(countList L); // make the list empty. Post: isEmpty()
void moveFirst(countList L); // set current marker to the first element in the list
// Pre: !isEmpty(); Post: !offEnd()
void moveLast(countList L); // set current marker to the last element in the list
// Pre: !isEmpty(); Post: !offEnd()
void movePrev(countList L); // set current marker one step backward.
// Pre: !offEnd(); Post: offEnd() only if atFirst() was true
void moveNext(countList L); // set current marker one step forward.
// Pre: !offEnd(); Post: offEnd() only if atLast() was true

void add(countList L, long bookID); //adds a book to the list, if it is already in the list, increments the count for that book in the list. post: !isEmpty()
void sortedCountInsert(countList L, long bookID, int count); //insertion sort by count value, only to be used when sorting a list. post: !isEmpty()
void insertBeforeFirst(countList L, long data); // Inserts new element before first
// Post: !isEmpty()
void insertAfterLast(countList L, long data); // Inserts new element after last one
// Post: !isEmpty()
void insertBeforeCurrent(countList L, long data); // Inserts new element before current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()
void insertAfterCurrent(countList L, long data); // Inserts new element after current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()
void deleteFirst(countList L); // delete the first element. Pre: !isEmpty()
void deleteLast(countList L); // delete the last element. Pre: !isEmpty()
void deleteCurrent(countList L); // delete the current element.
// Pre: !isEmpty(), !offEnd(); Post: offEnd()

long getHighestCount(countList L, countList bought); //returns book ID of the book with the highest book count in the list, sorts the list before returning highest count
void sortList(countList L); //sorts the list by putting all list elements into an array, then doing insertion sort back into the list by descending count value
void printList(countList L);//prints the list, bookIDs and counts

#endif
