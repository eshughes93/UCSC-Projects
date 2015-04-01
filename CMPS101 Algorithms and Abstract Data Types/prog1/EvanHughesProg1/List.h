// Evan Hughes [eshughes@ucsc.edu]

#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>


typedef struct List* ListRef;


ListRef newList (void);
   //
   // Constructor:    return a new valid empty list.
   // Precondition:   none;
   // Postcondition:  returns a properly constituted empty list.
   //

void freeList(ListRef);


/*** Access functions ***/

int isEmpty(ListRef L); // returns true is list is empty else returns false.
int offEnd(ListRef L); // returns true is current == NULL
int atFirst(ListRef L); // returns true if current == first and !offEnd()
int atLast(ListRef L); // returns true if current == last and !offEnd()
long getFirst(ListRef L); // return the first element; pre: !isEmpty()
long getLast(ListRef L); // return the last element; pre: !isEmpty()
long getCurrent(ListRef L); // return the current element pre: !offEnd()
int getLength(ListRef L); // return the length of the list
int equals (ListRef A, ListRef B); // return true iff the two lists have the same keys
// in the same order


/*** Manipulation procedures ***/

void makeEmpty(ListRef L); // make the list empty. Post: isEmpty()
void moveFirst(ListRef L); // set current marker to the first element in the list
// Pre: !isEmpty(); Post: !offEnd()

void moveLast(ListRef L); // set current marker to the last element in the list
// Pre: !isEmpty(); Post: !offEnd()
/*** Other operations ***/
//void printList(FILE* out, ListRef L);
ListRef copyList(ListRef L);
void movePrev(ListRef L); // set current marker one step backward.
// Pre: !offEnd(); Post: offEnd() only if atFirst() was true

void moveNext(ListRef L); // set current marker one step forward.
// Pre: !offEnd(); Post: offEnd() only if atLast() was true

void sortedInsert(ListRef L, long data); //insert new element according to data
//Post: !isEmpty()

void insertBeforeFirst(ListRef L, long data); // Inserts new element before first
// Post: !isEmpty()

void insertAfterLast(ListRef L, long data); // Inserts new element after last one
// Post: !isEmpty()

void insertBeforeCurrent(ListRef L, long data); // Inserts new element before current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()

void insertAfterCurrent(ListRef L, long data); // Inserts new element after current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()

void deleteFirst(ListRef L); // delete the first element. Pre: !isEmpty()
void deleteLast(ListRef L); // delete the last element. Pre: !isEmpty()
void deleteCurrent(ListRef L); // delete the current element.
// Pre: !isEmpty(), !offEnd(); Post: offEnd()


/*** Other operations ***/

void printList(ListRef L);
ListRef copyList(ListRef L);

#endif
