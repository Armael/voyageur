#ifndef KRUSKAL_H_INCLUDED
#define KRUSKAL_H_INCLUDED

typedef struct forest {
  Vector* trees;
  int treesNb;
} Forest;

typedef struct kedge {
  int n1;
  Ftree* addr1;

  int n2;
  Ftree* addr2;

  int weight;

  struct kedge *next;
} Kedge;


Forest* initForestFromGraph(Graph* g);
Kedge* KedgeListFromGraph(Graph* g, Forest* f);

Kedge* mergeSort(Kedge* list);

#endif
