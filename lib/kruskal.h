#include "vector.h"
#include "graph.h"
#include "ftree.h"

#ifndef KRUSKAL_H_INCLUDED
#define KRUSKAL_H_INCLUDED

/* Type définissant une forêt. Il s'agit simplement d'un Vector
   contenant des arbres, et de leur nombre */
typedef struct forest {
  Vector* trees;
  int treesNb;
} Forest;

/* Arête utilisée par l'algorithme de Kruskal : elle connecte des
   nœuds de différents Ftrees placés dans une forêt. Elle contient les
   indices des nœuds, mais aussi les addresses du nœud Ftree
   correspondant (et également le poids).  Ces arêtes sont stockées
   sous forme de liste (simplement) chaînée, d'où la présence du
   pointeur next vers l'élément suivant (la liste vide est représentée
   par NULL) */
typedef struct kedge {
  int n1;
  Ftree* addr1;

  int n2;
  Ftree* addr2;

  float weight;

  struct kedge *next;
} Kedge;

/* La fonction cons : ajoute un élément en tête de liste */
Kedge* kruskal_cons(Kedge* list, int n1, Ftree* addr1,
                    int n2, Ftree* addr2, float weight);

/* À partir d'un graphe donné, crée une forêt où chaque Ftree est
   réduit à un nœud, et représente un nœud du graphe. Il est placé au
   même indice que dans le graphe, et contient cet indice */
Forest* initForestFromGraph(Graph* g);

/* À partir d'un graphe donné et de la forêt retournée par
   initForestFromGraph(), retourne la liste des arêtes sous forme de
   Kedges. */
Kedge* KedgeListFromGraph(Graph* g, Forest* f);

/* Fait un tri fusion d'une liste d'arêtes Kedges. Complexité
   O(n log(n)) */
Kedge* mergeSort(Kedge* list);

/* Applique l'algorithme de Kruskal, calculant l'arbre couvrant
   minimal d'un graphe */
Ftree* kruskal(Graph* g);

/* Libère la mémoire occupée par une forêt. Il ne doit pas y avoir de
   doublons dans les f->treesNb arbres de la forêt - s'il y en a, on
   peut préalablement les remplacer par NULL pour indiquer un arbre
   absent */
void freeForest(Forest* f);

/* Libère la mémoire occupée par une liste de Kedges */
void freeKedgeList(Kedge* list);

void printKedgeList(Kedge* list);

#endif
