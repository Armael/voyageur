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
*/ 

Towns* parse_VerticesCoordinates(FILE* f) {
  if(f != NULL) {
    rewind(f);
    int nodes_nb;
    int found;
    found = fscanf(f, "%d!\n", &nodes_nb);
    if(found != 1) return NULL;
    
    Towns* t = towns_new();

    int id;
    float x, y;
    char name[20];
    while((found = fscanf(f, "%d: %f; %f!\n", &id, &x, &y)) && (found != EOF)) {
      if(found != 3) {
	printf("-> found : %d\n", found);
	free(t);
	return NULL;
      }

      snprintf(name, 20, "%d", id);
      vector_set(t->names, id-1, (Generic)(void*)strdup(name));
      vector_set(t->x, id-1, (Generic)x);
      vector_set(t->y, id-1, (Generic)y);

      t->nb++;
    }

    return t;
  }
  return NULL;
}

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

