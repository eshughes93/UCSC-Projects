//Evan Hughes [eshughes@ucsc.edu]
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "graph.h"

int main(int argc, char **argv){
	int numVertices = 5;
	graphRef g = newGraph(5);
	addEdge(g, 1, 2);
	addEdge(g, 2, 4);
	addEdge(g, 4, 3);
	addEdge(g, 4, 1);
	addEdge(g, 3, 5);
	addEdge(g, 5, 4);
	printGraph(g);
}
