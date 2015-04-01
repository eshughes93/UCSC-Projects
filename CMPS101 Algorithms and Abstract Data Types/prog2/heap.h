//Evan Hughes [eshughes@ucsc.edu]

#ifndef __HEAP_H__
#define __HEAP_H_

typedef struct heap* heapRef;

heapRef newHeap(int maxSize);// Constructor. Creates new heap. Pre-conditions: none, Post-conditions: Returns a new, empty heap.

heapRef buildHeap(int maxSize, int data[], int numData);  //builds and returns a new heap containing the given data[], sorted accordingly

void freeHeap(heapRef h); //frees memory allocated by given heap

/* Access functions */

int isFull(heapRef h);  //returns whether or not the heap is full

int emptyHeap(heapRef h); //returns whether or not the heap is empty

int maxValue(heapRef h); //returns the max value in the heap. does NOT remove it.

/* Manipulation Procedures */

void deleteMax(heapRef h); //deletes max value, then heapifies. Pre: !emptyHeap

void insert(heapRef h, int priority); //inserts a new key into the heap

void maxHeapify(heapRef h, int index);  //preserves the max heap property, sorts a heap from a given index, usually the ROOT.

//void heapSort(int keys[], int numKeys); //performs heapSort on an array of keys, user can see changes made to the passed keys[] array.

void swap(heapRef h, int child, int parent); //swaps the values at the child and parent indices of the heap

int parent(int child);  //returns the index of the parent of the given child index 

int lChild(int parent); //returns the index of the left child of the provided parent index

int rChild(int parent); //returns the index of the right child of the provided parent index

/* other functions */

void printHeap(heapRef h); //prints the array in the heap. does NOT print sorted order; prints heap order.


#endif
