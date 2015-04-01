//Evan Hughes [eshughes@ucsc.edu]
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#define ROOT 1

//struct defining heap and properties
struct heap {
	int last;
	int size;
	int *keys;
};

//creates and returns a new heap
heapRef newHeap(int maxSize){
	heapRef new = malloc(sizeof(struct heap)+sizeof(int)*maxSize);
	new->size = maxSize;
	new->last = 0;
	new->keys = calloc(maxSize+1,sizeof(int));
	return new;
}

//creates and returns a new heap which has sorted and stored the input keys
heapRef buildHeap(int maxSize, int data[], int numData){
	heapRef new = malloc(sizeof(struct heap) + sizeof(int)*maxSize);
	new->size = maxSize;
	new->last = 0;
	new->keys = calloc(maxSize+1, sizeof(int));
	for(int i = 0; i < numData; i++){
		insert(new, data[i]);
	}
	maxHeapify(new, ROOT);
	return new;
}

void freeHeap(heapRef h){
	free(h->keys);
	free(h);
}

//checks to see if the heap is full
int isFull(heapRef h){
	if(h->last == h->size)
		return 1;
	else
		return 0;
}

//checks to see if heap is empty
int emptyHeap(heapRef h){
	if(h->last == 0)
		return 1;
	else
		return 0;
}

//returns the index of the parent of the child at the given index
int parent(int child){
	return child/2;
}
//returns the left child of an index
int lChild(int parent){
	return 2*parent;
}

//returns the right child of an index
int rChild(int parent){
	return 2*parent + 1;
}

//swaps two values in a given heap
void swap(heapRef h, int child, int parent){
	int tmp = h->keys[child];
	h->keys[child] = h->keys[parent];
	h->keys[parent] = tmp;
}

//returns the max value in the heap - does not delete
int maxValue(heapRef h){
	return h->keys[ROOT];
}

//deletes the max value, heapifies to restore sorted order
void deleteMax(heapRef h){
	if(!emptyHeap(h)){
	swap(h, ROOT, h->last--);
	maxHeapify(h, ROOT);
	}
}

//inserts a new value at the bottom, bubbles up to sort appropriately
void insert(heapRef h, int priority){
	h->keys[++h->last] = priority;
	for(int child = h->last; child > ROOT;){
		if(h->keys[child] < h->keys[parent(child)]) {
			break;
		} else {
			swap(h, child, parent(child));
		}
		child = parent(child);
	}
}

//sorts the heap starting from a given index (usually the ROOT)
void maxHeapify(heapRef h, int index){
	int l = lChild(index);
	int r = rChild(index);
	int larger;
	if(l <= h->last && h->keys[l] > h->keys[index]){
		larger = l;
	} else {
		larger = index;
	}
	if(r <= h->last && h->keys[r] > h->keys[larger]){
		larger = r;
	}
	if(larger != index){
		swap(h, index, larger);
		maxHeapify(h, larger);
	}
}

//prints the heap (goes in order of heap index, does not print sorted
void printHeap(heapRef h){
	for(int i = 1; i < h->last+1; i++){
		printf("%d : %d \n", i, h->keys[i]);
	}
}

/*
//sorts a given array - users can see changes to passed array
void heapSort(int keys[], int numKeys){
	heapRef sort = buildHeap(numKeys, keys, numKeys);
	for(int i = numKeys-1; i > 0; i--){
		keys[i] = maxValue(sort);
		deleteMax(sort);
	}
	keys[0] = maxValue(sort);
	freeHeap(sort);
}
*/
