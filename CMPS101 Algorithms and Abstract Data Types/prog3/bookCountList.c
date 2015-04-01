//Evan Hughes [eshughes@ucsc.edu]
//Cristian Gonzalez <cfgonzal@ucsc.edu>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "bookCountList.h"

//struct for the node, and the typedef so we can refer to struct Node with a simple NodeType
typedef struct Node {
    long bookID;
    int count;
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

//this struct isn't an array but an array of this struct is used in the sortList function, in order to keep track of both the bookID and count when sorting with insertionSort
struct sortingArray{
	long bookID;
	int count;
};

//list constructor
countList newList (void){
    countList new = malloc(sizeof(struct List));
    new->length = 0;
    new->header = malloc (sizeof(ListType));
    new->header->first = NULL;
    new->header->last = NULL;
    new->header->curr = NULL;
    return new;
}

//constructor to allocate memory for sorter struct
sorter newArray(void){
	sorter new = malloc(sizeof(struct sortingArray));
	return new;
}

//frees memory for list
void freeList(countList L){
    while(L->header->first != NULL){
        L->header->curr = L->header->first;
        free(L->header->curr);
	L->header->first = L->header->first->next;
    }
    free(L->header);
	free(L);
}

//returns true if the list is empty
int isEmpty(countList L){
    if(L->length == 0)
        return 1;
    else
        return 0;
}

// returns true is current == NULL
int offEnd(countList L){
    if(L->header->curr == NULL)
        return 1;
    else
        return 0;
}

// returns true if current == first and !offEnd()
int atFirst(countList L){
    assert(!offEnd(L));
    if(L->header->curr == L->header->first)
        return 1;
    else
        return 0;
}

// returns true if current == last and !offEnd()
int atLast(countList L) { 
    assert(!offEnd(L));
    if(L->header->curr == L->header->last)
        return 1;
    else
        return 0;
}

//returns bookID of first element in list
long getFirst(countList L){ 
    assert(!isEmpty(L));
    return L->header->first->bookID;
}

//returns count of first element in list
int getFirstCount(countList L){
	assert(!isEmpty(L));
	return L->header->first->count;
}

//returns bookID of last element in list
long getLast(countList L){ 
    assert(!isEmpty(L));
    return L->header->last->bookID;
}

//returns bookID of current element in list
long getCurrent(countList L){ 
    assert(!offEnd(L));
    return L->header->curr->bookID;
}

//returns the length of the list
int getLength(countList L){ 
    return L->length;
}

//returns TRUE if given bookID is in the given list
int has(countList L, long bookID){
	assert(!isEmpty(L));
	moveFirst(L);
	while(!offEnd(L)){
		if(L->header->curr->bookID == bookID)
			return 1;
		moveNext(L);
	}
	return 0;
}

//empties list
void makeEmpty(countList L){ 
    L->header->first = NULL;
    L->header->last = NULL;
    L->header->curr = NULL;
    L->length = 0;
}

void moveFirst(countList L){ 
    if(!isEmpty(L))
        L->header->curr = L->header->first;
}

void moveLast(countList L){
    if(!isEmpty(L))
        L->header->curr = L->header->last;
}


void movePrev(countList L){
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

void moveNext(countList L){ 
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

//adds a book to a list, if it is already in the list, increments the count for that bookID
void add(countList L, long bookID){
	int inList = 0;
	if(!isEmpty(L)){
		moveFirst(L);
		while(L->header->curr != NULL){
			if(L->header->curr->bookID == bookID){
				inList = 1;
				L->header->curr->count++;
			}
			moveNext(L);
		}
		if(!inList){
			insertAfterLast(L, bookID);
		}
	} else {
		insertAfterLast(L, bookID);
	}
}

//inserts a bookID AND a count, sorting by the value of count. only to be used with sorting, regular insertion should not insert a count as to keep track of copurchases
void sortedCountInsert(countList L, long bookID, int count){
	NodeType* newNode = malloc(sizeof(struct Node));
	newNode->bookID = bookID;
	newNode->count = count;
	assert(newNode != NULL);
	if(isEmpty(L)){
        L->header->first = L->header->curr = L->header->last = newNode;
        L->header->first->prev = NULL;
        L->header->last->next = NULL;
	} else {
		moveFirst(L);
		while(L->header->curr != NULL){
			if(count < L->header->curr->count)
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

void insertBeforeFirst(countList L, long bookID){
    NodeType* newNode = malloc(sizeof(struct Node));
    newNode->bookID = bookID;
    newNode->count = 1;
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
void insertAfterLast(countList L, long bookID){ 
    NodeType* newNode = malloc(sizeof(struct Node));
    newNode->bookID = bookID;
    newNode->count = 1;
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
void insertBeforeCurrent(countList L, long bookID){
    NodeType* newNode = malloc(sizeof(struct Node));
    newNode->bookID = bookID;
    newNode->count = 1;
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
void insertAfterCurrent(countList L, long bookID){
    NodeType* newNode = malloc(sizeof(struct Node));
    newNode->bookID = bookID;
    newNode->count = 1;
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
void deleteFirst(countList L){
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
void deleteLast(countList L){ 
    assert(!isEmpty(L));
    NodeType* tmp = L->header->last;
    L->header->last->prev->next = NULL;
    L->header->last = L->header->last->prev;
    free(tmp);
    L->length--;
}

// delete the current element.
// Pre: !isEmpty(), !offEnd(); Post: offEnd()
void deleteCurrent(countList L){
	int empty;
	if(L->length == 1){
		empty = 1;
	} else {
		empty = 0;
	}
    if(offEnd(L)){
        printf("Cannot delete current. Current is offEnd.\n");
    } else {
        assert(!isEmpty(L));
        assert(!offEnd(L));
        NodeType* tmp = L->header->curr;
        if(atFirst(L)){
            deleteFirst(L);
        } else if(atLast(L)){
		deleteLast(L);
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
	if(empty){
		assert(isEmpty(L));
	}
}

//returns the bookID with the highest count, references customer's purchase history as to not return a bookID of something they have already purchased
long getHighestCount(countList L, countList bought){
	assert(!isEmpty(L));
	//printList(L);
	sortList(L);
	//printList(L);
	long bookID = getFirst(L);
	moveFirst(L);
	while(has(bought, bookID)){
		moveNext(L);
		if(offEnd(L)){
			return bookID = -1;
		}
		bookID = getCurrent(L);
		//printf("has will be: %d with the bookID %ld\n", has(bought, bookID), bookID);
	}
	return bookID;
}

//sorts the provided list by placing all element data into an array, then performing insertion sort back into the list
void sortList(countList L){
	int length = L->length;
	sorter *array = malloc(length*sizeof(struct sortingArray));
	for(int i = 0; i < length; i++){
		array[i] = newArray();
		array[i]->bookID = getFirst(L);
		array[i]->count = getFirstCount(L);
		if(!isEmpty(L)){
			deleteFirst(L);
		}
	}
	for(int i = 0; i < length; i++){
		sortedCountInsert(L, array[i]->bookID, array[i]->count);
		free(array[i]);
	}
	free(array);
}

void printList(countList L){
	if(!isEmpty(L)){
		NodeType* holder = L->header->curr;
		moveFirst(L);
		while(L->header->curr != NULL){
			long showData = L->header->curr->bookID;
			int countData = L->header->curr->count;
			//printf("Node %d bookID: %ld \n", nodeNum, showData);
			printf("bookID: %ld , count: %d \n", showData, countData);
			moveNext(L);
		}
	L->header->curr = holder;
	}
	printf("\n");
}
