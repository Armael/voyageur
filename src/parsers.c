#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "towns.h"
#include "trie.h"

#include "utils.h"

/*
Graph* parse_EdgesDistances(FILE* f) {
  if(f != NULL) {
    rewind(f);
    int nodes_nb;
    fscanf(f, "%d!\n", &nodes_nb);
    
    Graph* g = graph_newWithNodes(nodes_nb+1);

    int n1, n2;
    float w;
    int ret;
    while((ret = fscanf(f, "%d %d: %f!\n", &n1, &n2, &w)) && (ret != EOF)) {
      graph_setWeight(g, n1, n2, w);
    }

    return g;
  }
  return NULL;
}

Graph* parse_VerticesCoordinates(FILE* f) {
  if(f != NULL) {
    rewind(f);
    int nodes_nb;
    fscanf(f, "%d!\n", &nodes_nb);
    
    Graph* g = graph_newWithNodes(nodes_nb+1);
    Vector* v = vector_newWithSize(nodes_nb+1);
    vector_fill(v, nodes_nb+1, (Generic)NULL);

    int i;
    size_t n;
    char* pos;
    int ret;
    while((ret = fscanf(f, "%d: ", &i)) && (ret != EOF)) {
      getline(&pos, &n, f);
      vector_set(v, i, (Generic)(void*)pos);
    }

    int j;
    for(i=0; i < nodes_nb+1; i++) {
      for(j=i+1; j < nodes_nb+1; j++) {
	  float x1, y1, x2, y2;
	  sscanf(vector_get(v, i).p, "%f; %f!", &x1, &y1);
	  sscanf(vector_get(v, j).p, "%f; %f!", &x2, &y2);
	  graph_setWeight(g, i, j, dist(x1, x2, y1, y2));
      }
    }

    for(i=0; i < nodes_nb+1; i++)
      free(vector_get(v, i).p);

    vector_free(v);

    return g;	
  }
  return NULL;
}
*/
Trie* parse_Towns(FILE* f) {
  Trie* my_towns = trie_new();

  if(f != NULL) {
    rewind(f);

    char* line = NULL;
    size_t n;
    while(getline(&line, &n, f) != -1) {
      strtok(line, ":");
      char* split = strtok(NULL, ":");
      if(split == NULL) {
	trie_free(my_towns);
	return NULL;
      }
      split[-1] = '\0'; /* Élimination du ':' */
      
      float x, y;
      int found;
      found = sscanf(split, " %f; %f!", &x, &y);
      if(found != 2) {
	trie_free(my_towns);
	return NULL;
      }
      my_towns = trie_addTown(my_towns, line, x, y);
    }
    free(line);
  }

  return my_towns;
}
