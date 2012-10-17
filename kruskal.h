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

  int weight;

  struct kedge *next;
} Kedge;

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

#endif
