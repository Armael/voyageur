#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "vector.h"

typedef struct edge {
  int dest;
  int weight;
  
  struct edge* next;
} Edge;

typedef struct graph {
  Vector* nodes;
  int nodesNb;
} Graph;

/* Renvoie un nouveau graphe vide (sans nœuds ni arêtes) */
Graph* graph_new();

/* Crée un nouveau nœud dans le graphe, et renvoie son numéro.  La
 case de g->nodes est initialisée à NULL car le nœud est isolé (NULL
 représente ici la liste simplement chainée vide) */
int graph_addNode(Graph* g);

/* Renvoie un nouveau graphe contenant nodesNb nœuds isolés, de
   numéros 0 à (nodesNb-1) */
Graph* graph_newWithNodes(int nodesNb);

/* Ajoute une arête entre les nœuds d'indices n1 et n2, et de poids
   weight. Si n1 ou n2 n'est pas dans le graphe, la fonction de fait
   rien. */
void graph_addEdge(int n1, int n2, int weight);

/* Libère la mémoire occupée par le graphe (nœuds et arêtes) */
Edge* graph_free(Graph* g);

#endif
