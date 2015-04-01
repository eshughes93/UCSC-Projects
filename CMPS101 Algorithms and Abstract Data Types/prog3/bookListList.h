//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>

#ifndef __BOOKLISTLIST_H__
#define __BOOKLISTLIST_H__
#include "bookCountList.h"

typedef struct listList* chainList;


chainList newChainList (void);
   //
   // Constructor:    return a new valid empty list.
   // Precondition:   none;
   // Postcondition:  returns a properly constituted empty list.
   //

//frees the list
void freeChainList(chainList);


/*** Access functions ***/

int chainIsEmpty(chainList L); // returns true is list is empty else returns false.
int chainOffEnd(chainList L); // returns true is current == NULL
int chainAtFirst(chainList L); // returns true if current == first and !offEnd()
int chainAtLast(chainList L); // returns true if current == last and !offEnd()

int chainGetLength(chainList L); // return the length of the list
long chainGetHighestCount(chainList L, long bookID, countList bought); //returns book ID of the book with the highest book count
/*** Manipulation procedures ***/
void chainAdd(chainList L, long bookID); //adds a book to the list. post: !isEmpty()
void copurchase(chainList L, long bookOne, long bookTwo); //increments counts appropriately for the copurchase of two provided bookIDs
void chainMoveFirst(chainList L); // set current marker to the first element in the list
// Pre: !isEmpty(); Post: !offEnd()
void chainMoveLast(chainList L); // set current marker to the last element in the list
// Pre: !isEmpty(); Post: !offEnd()
void chainMovePrev(chainList L); // set current marker one step backward.
// Pre: !offEnd(); Post: offEnd() only if atFirst() was true
void chainMoveNext(chainList L); // set current marker one step forward.
// Pre: !offEnd(); Post: offEnd() only if atLast() was true
void chainInsertBeforeFirst(chainList L, long data); // Inserts new element before first
// Post: !isEmpty()
void chainInsertAfterLast(chainList L, long data); // Inserts new element after last one
// Post: !isEmpty()
void chainInsertBeforeCurrent(chainList L, long data); // Inserts new element before current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()
void chainInsertAfterCurrent(chainList L, long data); // Inserts new element after current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()
void chainDeleteFirst(chainList L); // delete the first element. Pre: !isEmpty()
void chainDeleteLast(chainList L); // delete the last element. Pre: !isEmpty()
void chainDeleteCurrent(chainList L); // delete the current element.
// Pre: !isEmpty(), !offEnd(); Post: offEnd()
void printBookList(chainList L, long bookID);


#endif
