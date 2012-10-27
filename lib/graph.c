#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "vector.h"

Edge* edge_cons(Edge* list, int dest, float weight) {
  Edge* new = malloc(sizeof(Edge));
  
  new->dest = dest;
  new->weight = weight;
  new->next = list;

  return new;
}

/* Renvoie un nouveau graphe vide (sans nœuds ni arêtes) */
Graph* graph_new() {
  Graph* g = malloc(sizeof(Graph));
  g->nodes = vector_new();
  g->nodesNb = 0;
  return g;
}

/* Crée un nouveau nœud dans le graphe, et renvoie son numéro.  La
 case de g->nodes est initialisée à NULL car le nœud est isolé (NULL
 représente ici la liste simplement chainée vide) */
int graph_addNode(Graph* g) {
  int newNodeId = g->nodesNb;
  vector_set(g->nodes, newNodeId, NULL);
  g->nodesNb++;

  return newNodeId;
}  

/* Renvoie un nouveau graphe contenant nodesNb nœuds isolés, de
   numéros 0 à (nodesNb-1) */
Graph* graph_newWithNodes(int nodesNb) {
  Graph* g = malloc(sizeof(Graph));
  g->nodes = vector_newWithSize(nodesNb);
  g->nodesNb = 0;

  int i;
  for(i=0; i < nodesNb; i++) {
    graph_addNode(g);
  }

  return g;
}

/* Ajoute une arête entre les nœuds d'indices n1 et n2, et de poids
   weight. Si n1 ou n2 n'est pas dans le graphe, la fonction ne fait
   rien. */
void graph_addEdge(Graph* g, int n1, int n2, float weight) {
  if(n1 < g->nodesNb && n2 < g->nodesNb) {
    /* Il faut ajouter deux arêtes : n1->n2 et n2->n1 car le graphe
       n'est pas orienté */
    Edge* e1 = edge_cons(vector_get(g->nodes, n1), n2, weight);
    Edge* e2 = edge_cons(vector_get(g->nodes, n2), n1, weight);

    vector_set(g->nodes, n1, e1);
    vector_set(g->nodes, n2, e2);
  }
}

/* Libère la mémoire occupée par le graphe (nœuds et arêtes) */
void graph_free(Graph* g) {
  int i;
  for(i=0; i < g->nodesNb; i++) {
    Edge* e = vector_get(g->nodes, i);
    while(e != NULL) {
      Edge* next = e->next;
      free(e);
      e = next;
    }
  }

  vector_free(g->nodes);
  free(g);
}

/* Écrit dans le fichier filename une représentation du graphe que
   l'on peut ensuite compiler avec neato (de graphviz) pour obtenir
   une représetation graphique du graphe */
void graph_toNeato(Graph* g, char* filename) {
  FILE* f;

  if((f = fopen(filename, "w")) == NULL)
    return;

  fprintf(f, "graph G {\n");

  int i;
  for(i=0; i < g->nodesNb; i++) {
    Edge* e = vector_get(g->nodes, i);
    while(e != NULL) {
      fprintf(f, "  %d -- %d [label=\"%f\"];\n", i, e->dest, e->weight);
      e = e->next;
    }
  }
  
  fprintf(f, "}\n");
  fclose(f);
}
