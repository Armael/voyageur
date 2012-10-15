#include "vector.h"
#include "graph.h"
#include "ftree.h"

#define SET_FTREE(v,i,t) (vector_set(v,i,(void*)t))
#define GET_FTREE(v,i) ((Ftree*)vector_get(v,i))

Forest* initForestFromGraph(Graph* g) {
  Forest* f = malloc(sizeof(Forest));

  f->trees = vector_newWithSize(g->nodesNb);
  f->treesNb = g->nodesNb;

  int i;
  for(i=0; i < f->treesNb; i++) {
    SET_FTREE(f->trees, i, free_create((void*)i));
  }

  return f;
}

Kedge* KedgeListFromGraph(Graph* g, Forest* f) {
  Kedge* klist = NULL;

  int i;
  for(i=0; i < g->nodesNb; i++) {
    Edge* e = GET_EDGE(g->nodes, i);
    while(e != NULL) {
      Kedge* new = malloc(sizeof(Kedge));
      new->n1 = i;
      new->addr1 = GET_FTREE(f->trees, i);
      new->n2 = e->dest;
      new->addr2 = GET_FTREE(f->trees, e->dest);
      new->weight = e->weight;
      new->next = klist;

      klist = new;
      e = e->next;
    }
  }

  return klist;
}

void split(Kedge* list, Kedge** part1, Kedge** part2) {
  *part1 = NULL;
  *part2 = NULL;

  int i = 0;
  while(list != NULL) {
    Kedge *next = list->next;
    if(i%2 == 0) {
      list->next = *part1;
      *part1 = list;
    } else {
      list->nest = *part2;
      *part2 = list;
    }
    list = next;
    i++;
  }
}

Kedge* merge(Kedge* part1, Kedge* part2) {
  if(part1 == NULL) {
    return part2;
  } else if(part2 == NULL) {
    return part1;
  }

  if(part1->weight < part2->weight) {
    part1->next = merge(part1->next, part2);
    return part1;
  } else {
    part2->next = merge(part1; part2->next);
    return part2;
  }
}

Kedge* mergeSort(Kedge* list) {
  if(list == NULL) {
    return list;
  } else {
    Kedge* part1, part2;
    split(list, &part1, &part2);
    part1 = mergeSort(part1);
    part2 = mergeSort(part2);

    return merge(part1, part2);
  }
}


Ftree* kruskal(Graph* g) {
  Forest* f = initForestFromGraph(g);
  Kedge* ke = KedgeListFromGraph(g, f);
  ke = mergeSort(ke);
  int i;
  while(ke != NULL) {
    if(GET_FTREE(f->trees, ke->n1) != GET_FTREE(f->trees, ke->n2)) {
      ftree_join(ke->addr1, ke->addr2);
      Ftree* fn2 = GET_FTREE(f->trees, ke->n2);
      for(i=0; i < f->treesNb; i++) {
        if(GET_FTREE(f->trees, i) == fn2) {
          SET_FTREE(f->trees, i, GET_FTREE(f->trees, ke->n1));
        }
      }
    }
    ke = ke->next;
  }
  return GET_FTREE(f->trees, 0);
}

