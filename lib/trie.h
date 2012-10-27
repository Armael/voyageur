#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <stdlib.h>

#include "vector.h"

#define CHAR_NUMBER 256

typedef struct trie {
  float coord_x;
  float coord_y;
  int is_word;
  Vector* next;
} Trie;

/* Renvoie une nouvelle trie vide */
Trie* trie_new();

/* Ajoute une ville à la trie, et insère ses coordonnées */ 
Trie* trie_addTown(Trie* t, char* name, int x, int y);

/* Libère la mémoire occupée par une trie */
void trie_free(Trie* t);

/* Retourne les coordonnées d'une ville de la trie d'après son nom.
 Si la fonction retourne -1, alors la ville n'a pas été trouvée dans
 la trie. Si au contraire elle a été trouvée, la fonction retourne
 0 */
int trie_getCoord(Trie* t, char* name, int* x, int* y);

#endif
