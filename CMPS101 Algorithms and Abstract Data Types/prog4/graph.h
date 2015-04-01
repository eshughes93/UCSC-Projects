//Evan Hughes [eshughes@ucsc.edu]
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "List.h"

typedef struct graph* graphRef;
//creates new graph with n vertices numbered 0 to n-1 with no edges, returns handle to graph
graphRef newGraph(int n); 

//adds edge (from, to) the graph
void addEdge(graphRef g, int from, int to); 

//performs BFS from source vertex s, updates graph's parent and distance arrays, erases results of any previous BFS
void doBFS(graphRef g, int s); 

//returns number of edges in shortest path from last BFS to source destination
//returns -1 if there is no path
int getDistance(graphRef g, int destination); 

//returns a List containing the path from the last doBFS source to destination, 
//returns null pointer if no path exists
ListRef getPathTo(graphRef g, int destination);

//prints graph
void printGraph(graphRef g);
//frees memory for graph
void freeGraph(graphRef g);

#endif
