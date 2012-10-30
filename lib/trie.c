#include <stdlib.h>
#include <stdint.h>

#include "trie.h"

/* Renvoie une nouvelle trie vide */
Trie* trie_new() {
  Trie* t = malloc(sizeof(Trie));
  t->coord_x = 0;
  t->coord_y = 0;
  t->is_word = 0;
  t->letters = NULL;

  return t;
}

/* Ajoute une lettre pointant sur le trie next au lettres de t.

   Attention : ne vérifie pas si la lettre n'est pas déja dans
   t->letters. Pour ne pas perdre l'information précédemment insérée,
   vous devez le vérifier vous-mêmes, avec trie_getLetter par
   exemple */
void trie_addLetter(Trie* t, char letter, Trie* next) {
  Letter* new = malloc(sizeof(Letter));
  new->letter = letter;
  new->next = next;
  new->tail = t->letters;

  t->letters = new;
}

/* Retourne la structure Letter correspondant à letter parmi les
   lettres de t.  Renvoie NULL si t ne comporte pas de lettre
   letter */
Letter* trie_getLetter(Trie* t, char letter) {
  Letter* l;
  for(l = t->letters; l != NULL; l = l->tail) {
    if(l->letter == letter) {
      return l;
    }
  }
  return NULL;
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
    char letter = name[0];
    Letter* l = trie_getLetter(t, letter);
    if(l == NULL) {
      trie_addLetter(t, letter, trie_addTown(NULL, &name[1], x, y));
    } else {
      l->next = trie_addTown(l->next, &name[1], x, y);
    }
  }
  return t;
}

/* Libère la mémoire occupée par une trie */
void trie_free(Trie* t) {
  if(t != NULL) {
    Letter* l = t->letters;
    while(l != NULL) {
      Letter* tmp = l->tail;
      trie_free(l->next);
      free(l);
      l = tmp;
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
    Letter* l;
    for(l = t->letters; l != NULL; l = l->tail) {
      if(l->letter == name[0]) {
	return trie_getCoord(l->next, &name[1], x, y);
      }
    }
    return -1;
  }
}
