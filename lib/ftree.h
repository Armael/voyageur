#ifndef FTREE_H_INCLUDED
#define FTREE_H_INCLUDED
#include "vector.h"
#include "generic.h"

/* Définition d'un Ftree : il s'agit d'un arbre d'arité variable non
   enraciné : on décrit en fait ici un nœud de l'arbre, qui pointe
   vers ses voisins, (neighbors), et contient une étiquette data. */
typedef struct ftree {
  Generic data;
  Vector *neighbors;
  int neighborsNb;
} Ftree;

/* Retourne un Ftree composé d'un unique nœud, contenant la data
   passée en argument. Celui-ci n'a donc pas de voisins
   initialement */
Ftree* ftree_create(Generic data);

/* Fusionne deux nœuds de deux Ftrees différents (sinon, on ferait
   apparaître un cycle…). Le premier devient voisin du second et
   réciproquement. Dès lors, les deux nœuds appartiennent au même
   arbre - pour libérer l'arbre entier il n'est nécessaire d'appeler
   la fonction de libération (ci-dessous) que sur l'un ou l'autre */
void ftree_join(Ftree *tree1, Ftree *tree2);

/* Libération de la mémoire occupée par un Ftree, connaissant un nœud
   de l'arbre (que l'on passe en argument) */
void ftree_free(Ftree *tree);

#endif
