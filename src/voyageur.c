#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "vector.h"
#include "graph.h"

float dist(float x1, float y1, float x2, float y2) {
  return sqrtf(fabsf(x2 - x1) + fabsf(y2 - y1));
}

Graph* parse_basic(FILE* f) {
  if(f != NULL) {
    rewind(f);
    int nodes_nb;
    fscanf(f, "%d!\n", &nodes_nb);
    
    Graph* g = graph_newWithNodes(nodes_nb+1);

    int n1, n2;
    float w;
    while(fscanf(f, "%d %d: %f!\n", &n1, &n2, &w) > 0) {
      graph_addEdge(g, n1, n2, w);
    }

    return g;
  }
  return NULL;
}

Graph* parse(FILE* f) {
  if(f != NULL) {
    rewind(f);
    int nodes_nb;
    fscanf(f, "%d!\n", &nodes_nb);
    
    Graph* g = graph_newWithNodes(nodes_nb+1);
    Vector* v = vector_newWithSize(nodes_nb+1);
    vector_fill(v, nodes_nb+1, NULL);

    int i;
    size_t n;
    char* pos;
    int ret;
    while((ret = fscanf(f, "%d: ", &i)) && (ret != EOF)) {
      getline(&pos, &n, f);
      vector_set(v, i, pos);
    }

    int j;
    for(i=0; i < nodes_nb+1; i++) {
      for(j=0; j < nodes_nb+1; j++) {
	if(i != j) { /* On ajoute pas d'arête de i vers lui-même */
	  float x1, y1, x2, y2;
	  sscanf(vector_get(v, i), "%f; %f!\n", &x1, &y1);
	  sscanf(vector_get(v, j), "%f; %f!\n", &x2, &y2);
	  graph_addEdge(g, i, j, dist(x1, x2, y1, y2));
	}
      }
    }

    for(i=0; i < nodes_nb+1; i++)
      free(vector_get(v, i));

    vector_free(v);

    return g;	
  }
  return NULL;
}

int main() {
  return 0;
}
