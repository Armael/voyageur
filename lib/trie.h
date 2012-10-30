#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

typedef struct trie Trie;
typedef struct letter Letter;

struct trie {
  float coord_x;
  float coord_y;
  int is_word;
  Letter* letters;
};

struct letter {
  char letter;
  Trie* next;
  struct letter *tail;
};

/* Renvoie une nouvelle trie vide */
Trie* trie_new();

/* Ajoute une lettre pointant sur le trie next au lettres de t.

   Attention : ne vérifie pas si la lettre n'est pas déja dans
   t->letters. Pour ne pas perdre l'information précédemment insérée,
   vous devez le vérifier vous-mêmes, avec trie_getLetter par
   exemple */
void trie_addLetter(Trie* t, char letter, Trie* next);

/* Retourne la structure Letter correspondant à letter parmi les
   lettres de t.  Renvoie NULL si t ne comporte pas de lettre
   letter */
Letter* trie_getLetter(Trie* t, char letter);

/* Ajoute une ville à la trie, et insère ses coordonnées */ 
Trie* trie_addTown(Trie* t, char* name, float x, float y);

/* Libère la mémoire occupée par une trie */
void trie_free(Trie* t);

/* Retourne les coordonnées d'une ville de la trie d'après son nom.
 Si la fonction retourne -1, alors la ville n'a pas été trouvée dans
 la trie. Si au contraire elle a été trouvée, la fonction retourne
 0 */
int trie_getCoord(Trie* t, char* name, float* x, float* y);

#endif
