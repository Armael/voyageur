#include "ftree.h"
#include "vector.h"
#include <stdlib.h>

/* Retourne un Ftree composé d'un unique nœud, contenant la data
   passée en argument. Celui-ci n'a donc pas de voisins
   initialement */
Ftree* ftree_create(void *data) {
  Ftree *p = malloc(sizeof(Ftree));
  p->data = data;
  p->neighbors = vector_new();
  p->neighborsNb = 0;

  return p;
}

/* Fusionne deux nœuds de deux Ftrees différents (sinon, on ferait
   apparaître un cycle…). Le premier devient voisin du second et
   réciproquement. Dès lors, les deux nœuds appartiennent au même
   arbre - pour libérer l'arbre entier il n'est nécessaire d'appeler
   la fonction de libération (ci-dessous) que sur l'un ou l'autre */
void ftree_join(Ftree *tree1, Ftree *tree2) {
  vector_set(tree1->neighbors, tree1->neighborsNb, tree2);
  vector_set(tree2->neighbors, tree2->neighborsNb, tree1);
  tree1->neighborsNb++;
  tree2->neighborsNb++;
}

/* Libération de la mémoire occupée par un Ftree, connaissant un nœud
   de l'arbre (que l'on passe en argument).

   On utilise pour cela un parcours récursif de l'arbre, en mémorisant
   l'adresse du nœud duquel on vient, pour ne pas rappeler
   récursivement dessus (puisque "être voisin de" est réflexif) */
void ftree_free_(Ftree *tree, Ftree *previous) {
  int i = 0;
  for(i = 0; i < tree->neighborsNb; i++) {
    if(vector_get(tree->neighbors, i) != previous) {
      ftree_free_(vector_get(tree->neighbors, i), tree);
    }
  }
  vector_free(tree->neighbors);
  free(tree);
}

void ftree_free(Ftree *tree) {
  ftree_free_(tree, NULL);
}

