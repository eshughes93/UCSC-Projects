//Evan Hughes [eshughes@ucsc.edu]
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "graph.h"


//Does the output, printing a path from the given source vertex to the given destination vertex. Performs BFS. 
void printPath(graphRef g, int source, int destination){
	doBFS(g, source);
	int distance = getDistance(g, destination);
	printf("The distance from %d to %d is: %d edges. Follow this path: ", source, destination, distance);
	ListRef path = getPathTo(g, destination);
	moveFirst(path);
	while(!offEnd(path)){
		int p = getCurrent(path);
		printf("%d, ", p);
		moveNext(path);
	}
	printf("%d.\n", destination);
	freeList(path);
}

//Simple hard-coded program, makes two graphs and prints each vertexes list of edges, and prints the path from 1 to n.
int main(int argc, char **argv){
	graphRef g = newGraph(5);
	addEdge(g, 0, 1);
	addEdge(g, 1, 3);
	addEdge(g, 3, 2);
	addEdge(g, 3, 0);
	addEdge(g, 2, 4);
	addEdge(g, 4, 3);
	addEdge(g, 3, 4);
	printGraph(g);
	printPath(g, 0, 4);
	graphRef k = newGraph(8);
	addEdge(k, 0, 1);
	addEdge(k, 1, 2);
	addEdge(k, 2, 3);
	addEdge(k, 3, 4);
	addEdge(k, 4, 5);
	addEdge(k, 5, 6);
	addEdge(k, 6, 7);
	printGraph(k);
	printPath(k, 0, 7);
	freeGraph(g);
	freeGraph(k);
}


