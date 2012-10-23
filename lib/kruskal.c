#include <stdlib.h>

#include "vector.h"
#include "graph.h"
#include "ftree.h"
#include "kruskal.h"

/* À partir d'un graphe donné, crée une forêt où chaque Ftree est
   réduit à un nœud, et représente un nœud du graphe. Il est placé au
   même indice que dans le graphe, et contient cet indice */
Forest* initForestFromGraph(Graph* g) {
  Forest* f = malloc(sizeof(Forest));

  /* On connait le nombre de nœuds à l'avance, optimisons, donc */
  f->trees = vector_newWithSize(g->nodesNb);
  f->treesNb = g->nodesNb;

  int i;
  for(i=0; i < f->treesNb; i++) {
    SET_FTREE(f->trees, i, ftree_create(NULL + i));
  }

  return f;
}

/* À partir d'un graphe donné et de la forêt retournée par
   initForestFromGraph(), retourne la liste des arêtes sous forme de
   Kedges. */
Kedge* KedgeListFromGraph(Graph* g, Forest* f) {
  Kedge* klist = NULL;

  int i;
  for(i=0; i < g->nodesNb; i++) {
    Edge* e = GET_EDGE(g->nodes, i);
    /* Parcours de la liste chainée des arêtes partant du nœud. On
     obtient bien des arêtes en double (obtenues depuis a->b et b->a)
     mais en pratique, l'algorithme de Kruskal n'est pas perturbé par
     la présence de doublons (ceux-ci sont ignorés) */
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

/* Coupe une liste d'arêtes de taille n en deux listes de taille
   n/2. Les listes de Kedges *part1 et *part2 passées en argument par
   référence sont modifiése pour correspondre aux deux parties
   obtenues.  Cette fonction n'alloue pas de mémoire : elle est
   destructive sur la liste passée en argument. Seules les deux
   parties sont utilisables après l'appel, la liste complète n'est
   plus accessible */
void split(Kedge* list, Kedge** part1, Kedge** part2) {
  *part1 = NULL;
  *part2 = NULL;

  int i = 0;
  while(list != NULL) {
    Kedge *next = list->next;
    /* On alterne en écrivant une fois dans *part1, une autre fois
       dans *part2 */
    if(i%2 == 0) {
      list->next = *part1;
      *part1 = list;
    } else {
      list->next = *part2;
      *part2 = list;
    }
    list = next;
    i++;
  }
}

/* Fusion de deux listes triées en une liste triée. Ici encore, le
   processus est destructif : tenter d'accéder à *part1 ou *part2
   après l'appel à la fonction n'a pas de sens */
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
    part2->next = merge(part1, part2->next);
    return part2;
  }
}

/* Tri fusion, complexité O(n log(n)) */
Kedge* mergeSort(Kedge* list) {
  if(list == NULL) {
    return list;
  } else {
    Kedge *part1, *part2;
    split(list, &part1, &part2);
    part1 = mergeSort(part1);
    part2 = mergeSort(part2);

    return merge(part1, part2);
  }
}

/* Applique l'algorithme de Kruskal, calculant l'arbre couvrant
   minimal d'un graphe */
Ftree* kruskal(Graph* g) {
  Forest* f = initForestFromGraph(g);
  Kedge* ke = KedgeListFromGraph(g, f);
  ke = mergeSort(ke);
  int i;
  while(ke != NULL) {
    if(vector_get(f->trees, ke->n1) != vector_get(f->trees, ke->n2)) {
      ftree_join(ke->addr1, ke->addr2);
      Ftree* fn2 = vector_get(f->trees, ke->n2);
      for(i=0; i < f->treesNb; i++) {
        if(vector_get(f->trees, i) == fn2) {
          vector_set(f->trees, i, vector_get(f->trees, ke->n1));
        }
      }
    }
    ke = ke->next;
  }

  /* Il n'y a au final qu'un arbre, et on ne veut pas le libérer
     puisqu'on le retourne */
  f->treesNb = 0;
  freeForest(f);

  freeKedgeList(ke);
  
  return vector_get(f->trees, 0);
}

/* Libère la mémoire occupée par une forêt. Il ne doit pas y avoir de
   doublons dans les f->treesNb arbres de la forêt - s'il y en a, on
   peut préalablement les remplacer par NULL pour indiquer un arbre
   absent */
void freeForest(Forest* f) {
  int i;
  for(i=0; i < f->treesNb; i++) {
    if(vector_get(f->trees, i))
      ftree_free(vector_get(f->trees, i));
  }
  
  vector_free(f->trees);
  free(f);
}

/* Libère la mémoire occupée par une liste de Kedges */
void freeKedgeList(Kedge* list) {
  Kedge* e;
  while(list != NULL) {
    e = list->next;
    free(list);
    list = e;
  }
}
