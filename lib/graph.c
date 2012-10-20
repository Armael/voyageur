#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "vector.h"

/* Renvoie un nouveau graphe vide (sans nœuds ni arêtes) */
Graph* graph_new() {
  Graph* g = malloc(sizeof(Graph));
  g->nodes = vector_new();
  g->nodesNb = 0;
}

/* Crée un nouveau nœud dans le graphe, et renvoie son numéro.  La
 case de g->nodes est initialisée à NULL car le nœud est isolé (NULL
 représente ici la liste simplement chainée vide) */
int graph_addNode(Graph* g) {
  int newNodeId = g->nodesNb;
  SET_EDGE(g->nodes, newNodeId, NULL);
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
void graph_addEdge(Graph* g, int n1, int n2, int weight) {
  if(n1 < g->nodesNb && n2 < g->nodesNb) {
    /* Il faut ajouter deux arêtes : n1->n2 et n2->n1 car le graphe
       n'est pas orienté */
    Edge* e1 = malloc(sizeof(Edge));
    Edge* e2 = malloc(sizeof(Edge));

    e1->dest = n2;
    e2->dest = n1;
    e1->weight = e2->weight = weight;

    e1->next = GET_EDGE(g->nodes, n1);
    e2->next = GET_EDGE(g->nodes, n2);

    SET_EDGE(g->nodes, n1, e1);
    SET_EDGE(g->nodes, n2, e2);
  }
}

/* Libère la mémoire occupée par le graphe (nœuds et arêtes) */
Edge* graph_free(Graph* g) {
  int i;
  for(i=0; i < g->nodesNb; i++) {
    Edge* e = GET_EDGE(g->nodes, i);
    while(e != NULL) {
      Edge* next = e->next;
      free(e);
      e = e->next;
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
    Edge* e = GET_EDGE(g->nodes, i);
    while(e != NULL) {
      fprintf(f, "  %d -- %d [label=\"%d\"];\n", i, e->dest, e->weight);
      e = e->next;
    }
  }
  
  fprintf(f, "}\n");
  fclose(f);
}
