//Evan Hughes [eshughes@ucsc.edu]
//Program 4 CMPS101
//Google Directions and BFS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "graph.h"

//This function takes in a graph, as well as source and destination vertices. It performs breadth-first-search from the source, and prints output
//displaying the shortest path from the source to the destination.
void printPath(graphRef g, int source, int destination){
	doBFS(g, source); //run BFS
	int distance = getDistance(g, destination); //get the distance to the destination according to last BFS
	if(distance == -1){ //if the returned distance is -1, it means there is no path
		printf("There is no path from %d to %d in the graph. \n", source, destination); 
	} else {
		printf("The distance from %d to %d is %d edges. Follow this path: ", source, destination, distance);
		ListRef path = getPathTo(g, destination); //list containing the vertices which create the path from destination to source
		moveFirst(path);
		while(!offEnd(path)){
			int p = getCurrent(path);
			printf("%d, ", p);
			moveNext(path);
		}
		printf("%d.\n", destination);
		freeList(path);
	}
}

//function to open input files and initialize graph accordingly. calls the output function as well
void openFile(char *filename){
	int numVerts;
	int numQueries;
	FILE *input = fopen(filename, "r");  //open the file
	if(input == NULL){
		printf("error\n");
	} else {
		fscanf(input, "%d", &numVerts); //fill in number of vertices
		fscanf(input, "%d", &numQueries); //fill in number of queries
		//printf("numVerts: %d, numQueries: %d\n", numVerts, numQueries);
	}
	graphRef graph = newGraph(numVerts); //make a new graph
	char delimiters[] = " \n"; //delimiters for strtok, we know there are spaces between numbers in the input lines, as well as a newline when there is a new vertex
	char *token; //will hold the char we want to look at
	char verts[2*numVerts]; //fgets puts a whole line of input into this string
	fgets(verts, numVerts, input); //call fgets after calling fscanf before we start our real input, fscanf leaves behind a newline which tells fgets to stop reading before its actually done anything
	for(int i = 0; i < numVerts; i++){ //take input for each line that contains a vertex and its adjacency list
		fgets(verts, numVerts*2, input);
		//printf("fgets verts: %s", verts);
		token = strtok(verts, delimiters);
		char *end;
		int base = 10;
		int edge = strtol(token, &end, base);  //convert the token char to an integer 
		//printf("looking at vertex %d \n", edge);
		int currVert = edge;  //tells us which vertex we are looking at the edges of
		while(token != NULL){
			token = strtok(NULL, delimiters);
			if(token == NULL) break;  //calling strtol on a NULL token segfaults
			edge = strtol(token, &end, base);
			//printf("adding edge: %d \n", edge);
			addEdge(graph, currVert, edge); //add the edge to the graph
		}
	}
	for(int i = 0; i < numQueries; i++){  //input telling us the path queries
		fgets(verts, numVerts*2, input);
		token = strtok(verts, delimiters);
		//printf("fgets verts: %s\n", verts);
		if(token != NULL){
			char *end;
			int base = 10;
			int destination = strtol(token, &end, base);  //destination vertex
			int source = destination; //source vertex
			token = strtok(NULL, delimiters);
			if(token == NULL) break;
			destination = strtol(token, &end, base);
			printPath(graph, source, destination);  //calls output function
		}
	}
	fclose(input);  //close the file we opened
	freeGraph(graph);
}
//main function. calls openFile, which in turn calls printPath
int main(int argc, char **argv){
	if(argc != 1) {
		for(int i = 1; i < argc; i++){
			openFile(argv[i]);
		}
	}
}
