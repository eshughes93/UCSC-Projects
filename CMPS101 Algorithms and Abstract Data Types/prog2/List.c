//Evan Hughes [eshughes@ucsc.edu]

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

//struct for the node, and the typedef so we can refer to struct Node with a simple NodeType
typedef struct Node {
    long data;
    struct Node* next;
    struct Node* prev;
} NodeType;

//struct for the ADT handle, uses typedef for ease of access with ListType
typedef struct ListHeader {
    NodeType* first;
    NodeType* last;
    NodeType* curr;
} ListType;

//the list struct which is accessible by the client with a handle to the adt and a reference for the length of the linked list
struct List{
    ListType* header;
    int length;
};


//List constructor, returns a new, empty list. 
ListRef newList (void){
    ListRef new = malloc(sizeof(struct List));
    new->length = 0;
    new->header = malloc (sizeof(ListType));
    new->header->first = NULL;
    new->header->last = NULL;
    new->header->curr = NULL;
    return new;
}


//deletes and frees the list of allocated memory
void freeList(ListRef L){
    while(L->header->first != NULL){
        L->header->curr = L->header->first;
        free(L->header->curr);
        L->header->first = L->header->first->next;
    }
    free(L->header);
	free(L);
}



/*** Access functions ***/

//returns true if the list is empty
int isEmpty(ListRef L){
    if(L->length == 0)
        return 1;
    else
        return 0;
}

// returns true is current == NULL
int offEnd(ListRef L){
    if(L->header->curr == NULL)
        return 1;
    else
        return 0;
}

// returns true if current == first and !offEnd()
int atFirst(ListRef L){
    assert(!offEnd(L));
    if(L->header->curr == L->header->first)
        return 1;
    else
        return 0;
}

// returns true if current == last and !offEnd()
int atLast(ListRef L) { 
    assert(!offEnd(L));
    if(L->header->curr == L->header->last)
        return 1;
    else
        return 0;
}

// return the first element; pre: !isEmpty()
long getFirst(ListRef L){ 
    assert(!isEmpty(L));
    //printf("First data entry is %ld\n", L->header->first->data);
    return L->header->first->data;
}

// return the last element; pre: !isEmpty()
long getLast(ListRef L){ 
    assert(!isEmpty(L));
    //printf("Last data entry is %ld\n", L->header->last->data);
    return L->header->last->data;
}

// return the current element pre: !offEnd()
long getCurrent(ListRef L){ 
    assert(!offEnd(L));
    //printf("Curr data entry is %ld\n", L->header->curr->data);
    return L->header->curr->data;
}

// return the length of the list
int getLength(ListRef L){ 
    //printf("Length of the list is %d\n", L->length);
    return L->length;
}

// return true iff the two lists have the same keys
// in the same order
int equals (ListRef A, ListRef B){
    if(A->length != B->length){
        printf("The lists are not identical\n");
        return 0;
    } else {
        moveFirst(A);
        moveFirst(B);
        while(A->header->curr != NULL || B->header->curr != NULL){
            if(A->header->curr-> data == B->header->curr->data){
                A->header->curr = A->header->curr->next;
                B->header->curr = B->header->curr->next;
            } else {
                break;
            }
        }
        if(offEnd(A) && offEnd(B)){
            printf("\nThe lists are identical.\n");
            return 1;
        } else {
            return 0;
        }
    }
    moveFirst(A);
    moveFirst(B);
}

/*** Manipulation procedures ***/

/*
//performs insertionSort algorithm upon a given array, user can see changes in passed array
void insertionSort(int keys[], int numKeys){
	ListRef list = newList();
	for(int i = 0; i < numKeys; i++){
		sortedInsert(list, (long)keys[i]);
	}
	//printList(list);
	for(int i = 0; i < numKeys; i++){
		keys[i] = (int)getFirst(list);
		if(!isEmpty(list))
			deleteFirst(list);
	}
	freeList(list);
}
*/

// make the list empty. Post: isEmpty()
void makeEmpty(ListRef L){ 
    L->header->first = NULL;
    L->header->last = NULL;
    L->header->curr = NULL;
    L->length = 0;
}

// set current marker to the first element in the list
// Pre: !isEmpty(); Post: !offEnd()
void moveFirst(ListRef L){ 
    if(!isEmpty(L))
        L->header->curr = L->header->first;
}

// set current marker to the last element in the list
// Pre: !isEmpty(); Post: !offEnd()
void moveLast(ListRef L){
    if(!isEmpty(L))
        L->header->curr = L->header->last;
}


// set current marker one step backward.
// Pre: !offEnd(); Post: offEnd() only if atFirst() was true
void movePrev(ListRef L){
    int off = 0;
    if(atFirst(L)){
        off = 1;
    }
    if(!offEnd(L)){
        L->header->curr = L->header->curr->prev;
    } else {
        printf("Cannot movePrev, curr is offEnd\n");
    }
    if(off){
        assert(offEnd(L));
    }
}

// set current marker one step forward.
// Pre: !offEnd(); Post: offEnd() only if atLast() was true
void moveNext(ListRef L){ 
    int off = 0;
    if(atLast(L)){
        off = 1;
    }
    if(!offEnd(L)){
        L->header->curr = L->header->curr->next;
    } else {
        printf("Cannot moveNext, curr is offEnd\n");
    }
    if(off){
        assert(offEnd(L));
    }
}

//Inserts into the list in a sorted order, from least to greatest
void sortedInsert(ListRef L, long data){
	NodeType* newNode = malloc(sizeof(struct Node));
	newNode->data = data;
	assert(newNode != NULL);
	if(isEmpty(L)){
        L->header->first = L->header->curr = L->header->last = newNode;
        L->header->first->prev = NULL;
        L->header->last->next = NULL;
	} else {
		moveFirst(L);
		while(L->header->curr != NULL){
			if(data > L->header->curr->data)
				L->header->curr = L->header->curr->next;
			else
				break;
		}
		if(offEnd(L)){
			moveLast(L);       
			L->header->last->next = newNode;
			newNode->prev = L->header->last;
			L->header->last = newNode;
			L->header->last->next = NULL;
		} else if(atFirst(L)){
			L->header->first->prev = newNode;
			newNode->next = L->header->first;
			L->header->first = newNode;
			L->header->first->prev = NULL;
		} else {
			newNode->next = L->header->curr;
			newNode->prev = L->header->curr->prev;
			L->header->curr->prev->next = newNode;
			L->header->curr->prev = newNode;	
		}
	}
	L->header->curr = newNode;
	L->length++;
	assert(!offEnd(L));
	assert(!isEmpty(L));
}


// Inserts new element before first
// Post: !isEmpty()
void insertBeforeFirst(ListRef L, long data){
    NodeType* newNode = malloc(sizeof(struct Node));
    newNode->data = data;
    assert(newNode != NULL);
    if(isEmpty(L)){
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
    assert(!isEmpty(L));
}

// Inserts new element after last one
// Post: !isEmpty()
void insertAfterLast(ListRef L, long data){ 
    NodeType* newNode = malloc(sizeof(struct Node));
    newNode->data = data;
    assert(newNode != NULL);
    if(isEmpty(L)){
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
    assert(!isEmpty(L));
}

// Inserts new element before current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()
void insertBeforeCurrent(ListRef L, long data){
    NodeType* newNode = malloc(sizeof(struct Node));
    newNode->data = data;
    NodeType* curr = L->header->curr;
    if(isEmpty(L)){
        printf("List is empty\n");
        L->header->first = L->header->curr = L->header->last = newNode;
        L->header->first->prev = NULL;
        L->header->last->next = NULL;
    } else if(atFirst(L)){
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
    assert(!isEmpty(L));
    assert(!offEnd(L));
}

// Inserts new element after current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()
void insertAfterCurrent(ListRef L, long data){
    NodeType* newNode = malloc(sizeof(struct Node));
    newNode->data = data;
    NodeType* curr = L->header->curr;
    if(isEmpty(L)){
        printf("List is empty\n");
        L->header->first = L->header->curr = L->header->last = newNode;
        L->header->first->prev = NULL;
        L->header->last->next = NULL;
    } else if(atLast(L)){
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
    assert(!isEmpty(L));
    assert(!offEnd(L));
}

// delete the first element. Pre: !isEmpty()
void deleteFirst(ListRef L){
    assert(!isEmpty(L));
    NodeType* tmp = L->header->first;
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

// delete the last element. Pre: !isEmpty()
void deleteLast(ListRef L){ 
    assert(!isEmpty(L));
    NodeType* tmp = L->header->last;
    L->header->last->prev->next = NULL;
    L->header->last = L->header->last->prev;
    free(tmp);
    L->length--;
}

// delete the current element.
// Pre: !isEmpty(), !offEnd(); Post: offEnd()
void deleteCurrent(ListRef L){
    if(offEnd(L)){
        printf("Cannot delete current. Current is offEnd.\n");
    } else {
        assert(!isEmpty(L));
        assert(!offEnd(L));
        NodeType* tmp = L->header->curr;
        if(atFirst(L)){
            deleteFirst(L);
        } else {
            L->header->curr->prev->next = L->header->curr->next;
            L->header->curr->next->prev = L->header->curr->prev;
            L->header->curr->next = NULL;
            L->header->curr->prev = NULL;
            L->header->curr = NULL;
            free(tmp);
            assert(offEnd(L));
            L->length--;
        }
    }
}

/*** Other operations ***/

void printList(ListRef L){
if(isEmpty(L)){
} else {
if(offEnd(L)){
moveLast(L);
}
assert(!offEnd(L));
    int nodeNum = 0;
    NodeType* holder = L->header->curr;
    if(isEmpty(L)){
        printf("The list is empty\n");
    } else {
        moveFirst(L);
        while(L->header->curr != NULL){
            long showData = L->header->curr->data;
            //printf("Node %d data: %ld \n", nodeNum, showData);
	    printf("%ld \n", showData);
            nodeNum++;
            moveNext(L);
        }
    }
    L->header->curr = holder;
}
}
ListRef copyList(ListRef L){
    ListRef new = newList();
    moveFirst(L);
    NodeType* holder = L->header->curr;
    while(L->header->curr != NULL){
        sortedInsert(new, L->header->curr->data);
        L->header->curr = L->header->curr->next;
    }
    L->header->curr = holder;
    return new;
}
