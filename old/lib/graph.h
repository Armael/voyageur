#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "vector.h"

typedef struct graph {
  Vector* weight;
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

void graph_setWeight(Graph* g, int n1, int n2, float weight);

float graph_getWeight(Graph* g, int n1, int n2);

/* Libère la mémoire occupée par le graphe (nœuds et arêtes) */
void graph_free(Graph* g);

/* Écrit dans le fichier filename une représentation du graphe que
   l'on peut ensuite compiler avec neato (de graphviz) pour obtenir
   une représetation graphique du graphe */
/*void graph_toNeato(Graph* g, char* filename);*/

#endif
