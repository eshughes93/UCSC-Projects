//Evan Hughes [eshughes@ucsc.edu]
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "graph.h"

static int white = 0;
static int grey = 1;
static int black = 2;

struct graph{
	ListRef *graph;
	int *color;
	int *parent;
	int *distance;
	int numEdges;
	int numVerts;
};
//creates new graph with n vertices numbered 0 to n-1 with no edges, returns handle to graph
graphRef newGraph(int n){
	graphRef new = malloc(sizeof(struct graph));
	assert(new != NULL);
	new->graph = malloc(sizeof(ListRef) * (n));
	new->color = calloc(n, sizeof(int));
	new->parent = calloc(n, sizeof(int));
	new->distance = calloc(n, sizeof(int));
	for(int i = 0; i < n; i++){
		new->graph[i] = newList();
		new->color[i] = white;
		new->parent[i] = -1;
		new->distance[i] = -1;
		//printf("i = %d, ", i);
		//if(new->color[i] == white)
		//	printf("color = white, ");
		//printf("parent[i] = %d, ", new->parent[i]);
		//printf("distance[i] = %d \n", new->distance[i]);
	}
	new->numEdges = 0;
	new->numVerts = n;
	return new;
}

//adds edge (from, to) the graph
void addEdge(graphRef g, int from, int to){
	sortedInsert(g->graph[from], to);
	g->numEdges++;
} 


//performs BFS from source vertex, updates graph's color, parent, and distance arrays, erases results of any previous BFS
void doBFS(graphRef g, int s){
	for(int i = 0; i < g->numVerts; i++){
		g->color[i] = white;
		g->parent[i] = -1;
		g->distance[i] = -1;
	}
	int u;
	ListRef q = newList();
	g->color[s] = grey;
	g->distance[s] = 0;
	enqueue(q,s);
	while(!isEmpty(q)){
		u = dequeue(q);
		moveFirst(g->graph[u]);
		while(!offEnd(g->graph[u])){
			int v = getCurrent(g->graph[u]);
			if(g->color[v] == white){
				g->color[v] = grey;
				g->distance[v] = g->distance[u] + 1;
				g->parent[v] = u;
				enqueue(q,v);
			}
			moveNext(g->graph[u]);
		}
		g->color[u] = black;
	}
	freeList(q);
} 

//returns number of edges in shortest path from last BFS to source destination
//returns -1 if there is no path
int getDistance(graphRef g, int destination){
	int distance = g->distance[destination];
	return distance;
} 

//returns a List containing the path from the last doBFS source to destination, 
//returns null pointer if no path exists
ListRef getPathTo(graphRef g, int destination){\
	ListRef path = newList();
	int stone = g->parent[destination];
	while(stone != -1){
		insertBeforeFirst(path, stone);
		stone = g->parent[stone];
	}
	return path;
}

void printGraph(graphRef g){
	for(int i = 0; i < g->numVerts; i++){
		printf("vertex: %d connected to: ", i);
		printList(g->graph[i]);
		printf("\n");
	}
}
//frees memory for graph
void freeGraph(graphRef g){
	free(g->color);
	free(g->parent);
	free(g->distance);
	for(int i = 0; i < g->numVerts; i++){
		freeList(g->graph[i]);
	}
	free(g->graph);
	free(g);
}
