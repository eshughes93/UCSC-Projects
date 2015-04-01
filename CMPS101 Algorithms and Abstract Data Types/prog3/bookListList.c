//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "bookCountList.h"
#include "bookListList.h"


//struct for the node, and the typedef so we can refer to struct Node with a simple NodeType
typedef struct listNode {
    long data;
    countList list;
    struct listNode* next;
    struct listNode* prev;
} listNodeType;

//struct for the ADT handle, uses typedef for ease of access with ListType
typedef struct listListHeader {
    listNodeType* first;
    listNodeType* last;
    listNodeType* curr;
} listListType;

//the list struct which is accessible by the client with a handle to the adt and a reference for the length of the linked list
struct listList{
    listListType* header;
    int length;
};

//creates a new list to be used for chaining in a hash table. 
chainList newChainList (void){
    chainList new = malloc(sizeof(struct listList));
    new->length = 0;
    new->header = malloc (sizeof(listListType));
    new->header->first = NULL;
    new->header->last = NULL;
    new->header->curr = NULL;
    return new;
}

//frees the chain list, as well as calling freeList on the countLists contained in each node
void freeChainList(chainList L){
    while(L->header->first != NULL){
	chainMoveFirst(L);
	freeList(L->header->curr->list);
        free(L->header->curr);
        L->header->first = L->header->first->next;
    }
    free(L->header);
	free(L);
}


//returns true if the list is empty
int chainIsEmpty(chainList L){
    if(L->length == 0)
        return 1;
    else
        return 0;
}

// returns true is current == NULL
int chainOffEnd(chainList L){
    if(L->header->curr == NULL)
        return 1;
    else
        return 0;
}

// returns true if current == first and !chainOffEnd()
int chainAtFirst(chainList L){
    assert(!chainOffEnd(L));
    if(L->header->curr == L->header->first)
        return 1;
    else
        return 0;
}

// returns true if current == last and !chainOffEnd()
int chainAtLast(chainList L) { 
    assert(!chainOffEnd(L));
    if(L->header->curr == L->header->last)
        return 1;
    else
        return 0;
}

//returns length of the list
int chainGetLength(chainList L){ 
    return L->length;
}

//finds the countList specific to the provided bookID, and then returns the bookID with the highest count inside that countList
long chainGetHighestCount(chainList L, long bookID, countList bought){
	assert(!chainIsEmpty(L));
	chainMoveFirst(L);
	while(!chainOffEnd(L)){
		if(L->header->curr->data == bookID){
			return getHighestCount(L->header->curr->list, bought);
		}
		chainMoveNext(L);
	}
	return 0;
}

//sets current to first
void chainMoveFirst(chainList L){ 
    if(!chainIsEmpty(L))
        L->header->curr = L->header->first;
}

//sets current to last
void chainMoveLast(chainList L){
    if(!chainIsEmpty(L))
        L->header->curr = L->header->last;
}

//moves to the previous node from the current
void chainMovePrev(chainList L){
    int off = 0;
    if(chainAtFirst(L)){
        off = 1;
    }
    if(!chainOffEnd(L)){
        L->header->curr = L->header->curr->prev;
    } else {
        printf("Cannot movePrev, curr is chainOffEnd\n");
    }
    if(off){
        assert(chainOffEnd(L));
    }
}

//moves to the next node from the current
void chainMoveNext(chainList L){ 
    int off = 0;
    if(chainAtLast(L)){
        off = 1;
    }
    if(!chainOffEnd(L)){
        L->header->curr = L->header->curr->next;
    } else {
        printf("Cannot moveNext, curr is chainOffEnd\n");
    }
    if(off){
        assert(chainOffEnd(L));
    }
}

//adds a book ID to the list (if there is a collision in the hash table, the second book will get added to this list where the first one is contained)
void chainAdd(chainList L, long bookID){
	int inList = 0;
	if(!chainIsEmpty(L)){
		chainMoveFirst(L);
		while(L->header->curr != NULL){
			if(L->header->curr->data == bookID){
				inList = 1;
			}
			chainMoveNext(L);
		}
		if(!inList){
			chainInsertAfterLast(L, bookID);
		}
	} else {
		chainInsertAfterLast(L, bookID);
	}
}

//increments counts appropriately for the copurchase of the two provided bookIDs
void copurchase(chainList L, long bookOne, long bookTwo){
	assert(!chainIsEmpty(L));
	chainMoveFirst(L);
	while(L->header->curr != NULL){
		countList list = L->header->curr->list;
		if(L->header->curr->data == bookOne){
			add(list, bookTwo);
		}
		chainMoveNext(L);
	}
}


// Inserts new element before first
// Post: !chainIsEmpty()
void chainInsertBeforeFirst(chainList L, long data){
    listNodeType* newNode = malloc(sizeof(struct listNode));
    newNode->data = data;
    newNode->list = newList();
    assert(newNode != NULL);
    if(chainIsEmpty(L)){
        //printf("List is empty\n");
        L->header->first = L->header->curr = L->header->last = newNode;
        L->header->first->prev = NULL;
        L->header->last->next = NULL;
    } else {
        L->header->first->prev = newNode;
        newNode->next = L->header->first;
        L->header->first = newNode;
        L->header->first->prev = NULL;
    }
    L->header->curr = newNode;
    L->length++;
    assert(!chainIsEmpty(L));
}

// Inserts new element after last one
// Post: !chainIsEmpty()
void chainInsertAfterLast(chainList L, long data){ 
    listNodeType* newNode = malloc(sizeof(struct listNode));
    newNode->data = data;
    newNode->list = newList();
    assert(newNode != NULL);
    if(chainIsEmpty(L)){
        //printf("List is empty\n");
        L->header->first = L->header->curr = L->header->last = newNode;
        L->header->first->prev = NULL;
        L->header->last->next = NULL;
    } else {
        L->header->last->next = newNode;
        newNode->prev = L->header->last;
        L->header->last = newNode;
        L->header->last->next = NULL;
    }
    L->header->curr = newNode;
    L->length++;
    assert(!chainIsEmpty(L));
}

// Inserts new element before current one
// Pre: !chainOffEnd(); Post: !chainIsEmpty(), !chainOffEnd()
void chainInsertBeforeCurrent(chainList L, long data){
    listNodeType* newNode = malloc(sizeof(struct listNode));
    newNode->data = data;
    newNode->list = newList();
    listNodeType* curr = L->header->curr;
    if(chainIsEmpty(L)){
        printf("List is empty\n");
        L->header->first = L->header->curr = L->header->last = newNode;
        L->header->first->prev = NULL;
        L->header->last->next = NULL;
    } else if(chainAtFirst(L)){
	L->header->first->prev = newNode;
        newNode->next = L->header->first;
        L->header->first = newNode;
        L->header->first->prev = NULL;
	} else {
        newNode->next = curr;
        newNode->prev = curr->prev;
        curr->prev->next = newNode;
        curr->prev = newNode;
    }
    L->header->curr = newNode;
    L->length++;
    assert(!chainIsEmpty(L));
    assert(!chainOffEnd(L));
}

// Inserts new element after current one
// Pre: !chainOffEnd(); Post: !chainIsEmpty(), !chainOffEnd()
void chainInsertAfterCurrent(chainList L, long data){
    listNodeType* newNode = malloc(sizeof(struct listNode));
    newNode->data = data;
    newNode->list = newList();
    listNodeType* curr = L->header->curr;
    if(chainIsEmpty(L)){
        printf("List is empty\n");
        L->header->first = L->header->curr = L->header->last = newNode;
        L->header->first->prev = NULL;
        L->header->last->next = NULL;
    } else if(chainAtLast(L)){
        L->header->last->next = newNode;
        newNode->prev = L->header->last;
        L->header->last = newNode;
        L->header->last->next = NULL;
    } else {
        newNode->prev = curr;
        newNode->next = curr->next;
        curr->next->prev = newNode;
        curr->next = newNode;
    }
    L->header->curr = newNode;
    L->length++;
    assert(!chainIsEmpty(L));
    assert(!chainOffEnd(L));
}

// delete the first element. Pre: !chainIsEmpty()
void chainDeleteFirst(chainList L){
    assert(!chainIsEmpty(L));
    listNodeType* tmp = L->header->first;
    if(L->header->first == L->header->last){
        L->header->first = NULL;
        free(tmp);
	L->length--;
    } else {
        L->header->first->next->prev = NULL;
        L->header->first = L->header->first->next;
        free(tmp);
        L->length--;
    }
}

// delete the last element. Pre: !chainIsEmpty()
void chainDeleteLast(chainList L){ 
    assert(!chainIsEmpty(L));
    listNodeType* tmp = L->header->last;
    L->header->last->prev->next = NULL;
    L->header->last = L->header->last->prev;
    free(tmp);
    L->length--;
}

// delete the current element.
// Pre: !chainIsEmpty(), !chainOffEnd(); Post: chainOffEnd()
void chainDeleteCurrent(chainList L){
    if(chainOffEnd(L)){
        printf("Cannot delete current. Current is offEnd.\n");
    } else {
        assert(!chainIsEmpty(L));
        assert(!chainOffEnd(L));
        listNodeType* tmp = L->header->curr;
        if(chainAtFirst(L)){
            chainDeleteFirst(L);
        } else {
            L->header->curr->prev->next = L->header->curr->next;
            L->header->curr->next->prev = L->header->curr->prev;
            L->header->curr->next = NULL;
            L->header->curr->prev = NULL;
            L->header->curr = NULL;
            free(tmp);
            assert(chainOffEnd(L));
            L->length--;
        }
    }
}

//prints the copurchase list of the provided bookID
void printBookList(chainList L, long bookID){
	if(!chainIsEmpty(L)){
		chainMoveFirst(L);
		while(L->header->curr != NULL){
			countList list = L->header->curr->list;
			if(L->header->curr->data == bookID){
				printList(list);
			}
		chainMoveNext(L);
		}
	}
}
