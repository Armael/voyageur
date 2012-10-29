#include <stdlib.h>
#include <stdint.h>

#include "trie.h"

/* Renvoie une nouvelle trie vide */
Trie* trie_new() {
  Trie* t = malloc(sizeof(Trie));
  t->coord_x = 0;
  t->coord_y = 0;
  t->is_word = 0;
  t->next = NULL;

  return t;
}

/* Ajoute une ville à la trie, et insère ses coordonnées */
Trie* trie_addTown(Trie* t, char* name, float x, float y) {
  if(t == NULL) {
    t = trie_new();
  }
  
  if(name[0] == '\0') {
    t->is_word = 1;
    t->coord_x = x;
    t->coord_y = y;
  } else {
    if(t->next == NULL) {
      t->next = calloc(CHAR_NUMBER, sizeof(Trie));
    }

    int letter = (int)(uint8_t)name[0];
    t->next[letter] = trie_addTown(t->next[letter], &name[1], x, y);
  }
  return t;
}

/* Libère la mémoire occupée par une trie */
void trie_free(Trie* t) {
  int i;
  if(t != NULL) {
    if(t->next != NULL) {
      for(i=0; i < CHAR_NUMBER; i++) {
	Trie* child;
	if((child = t->next[i]) != NULL) {
          trie_free(child);
	}
      }
      free(t->next);
    }
  free(t);
  }
}

/* Retourne les coordonnées d'une ville de la trie d'après son nom.
 Si la fonction retourne -1, alors la ville n'a pas été trouvée dans
 la trie. Si au contraire elle a été trouvée, la fonction retourne
 0 */
int trie_getCoord(Trie* t, char* name, float* x, float* y) {
   if(t == NULL) {
     return -1;
   }
  
  if(name[0] == '\0') {
    if(t->is_word) {
      *x = t->coord_x;
      *y = t->coord_y;
      return 0;
    } else {
      return -1;
    }
  } else {
    if(t->next == NULL) {
      return -1;
    } else {
      return trie_getCoord(t->next[(int)name[0]], &name[1], x, y);
    }
  }
}
