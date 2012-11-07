#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "vector.h"

/* Renvoie un nouveau graphe vide (sans nœuds ni arêtes) */
Graph* graph_new() {
  Graph* g = malloc(sizeof(Graph));
  g->weight = vector_new();
  g->nodesNb = 0;
  return g;
}

/* Crée un nouveau nœud dans le graphe, et renvoie son numéro.  La
 case de g->nodes est initialisée à NULL car le nœud est isolé (NULL
 représente ici la liste simplement chainée vide) */
int graph_addNode(Graph* g) {
  int newNodeId = g->nodesNb;
  vector_set(g->weight, newNodeId, (Generic)(void*)vector_new());
  g->nodesNb++;

  return newNodeId;
}  

/* Renvoie un nouveau graphe contenant nodesNb nœuds isolés, de
   numéros 0 à (nodesNb-1) */
Graph* graph_newWithNodes(int nodesNb) {
  Graph* g = malloc(sizeof(Graph));
  g->weight = vector_newWithSize(nodesNb);
  g->nodesNb = nodesNb;

  int i;
  for(i=0; i < nodesNb; i++) {
    vector_set(g->weight, i, (Generic)(void*)vector_newWithSize(nodesNb-i+1));
  }

  return g;
}

void graph_setWeight(Graph* g, int n1, int n2, float weight) {
  if(n1 > n2) {
      int t = n1;
      n1 = n2;
      n2 = t;
  }
  
  Vector* v = (Vector*)vector_get(g->weight, n1).p;
  vector_set(v, n2, (Generic)weight);
}

float graph_getWeight(Graph* g, int n1, int n2) {
  if(n1 > n2) {
    int t = n1;
    n1 = n2;
    n2 = t;
  }

  return vector_get(vector_get(g->weight, n1).p, n2).f;
}

/* Libère la mémoire occupée par le graphe (nœuds et arêtes) */
void graph_free(Graph* g) {
  if(g == NULL) return;
  
  int i;
  for(i=0; i < g->nodesNb; i++) {
    vector_free(vector_get(g->weight, i).p);
  }

  vector_free(g->weight);
  free(g);
}

/* Écrit dans le fichier filename une représentation du graphe que
   l'on peut ensuite compiler avec neato (de graphviz) pour obtenir
   une représetation graphique du graphe */
/*void graph_toNeato(Graph* g, char* filename) {
  FILE* f;

  if((f = fopen(filename, "w")) == NULL)
    return;

  fprintf(f, "graph G {\n");

  int i;
  for(i=0; i < g->nodesNb; i++) {
    Edge* e = vector_get(g->nodes, i).p;
    while(e != NULL) {
      fprintf(f, "  %d -- %d [label=\"%f\"];\n", i, e->dest, e->weight);
      e = e->next;
    }
  }
  
  fprintf(f, "}\n");
  fclose(f);
  }*/
