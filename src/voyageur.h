#ifndef VOYAGEUR_H_INCLUDED
#define VOYAGEUR_H_INCLUDED

/* Environnement :
 * contient le graphe des villes, la base de données, et les 
 * tableaux d'information sur les villes du graphe
 */

typedef struct Command Command;
typedef struct Env Env;

struct Command {
  char* name;
  char* shorthelp;
  char* help;

  void (*f)(char** args,
	    Env* env);
};

struct Env {
  /* Quand est à 1, indique que l'on doit sortir de la boucle
     d'évaluation de commandes */
  int done;
  /* La base de données contenant des noms de villes, associés à leurs
     coordonnées */
  Trie* towns;

  /* Données des villes actuellement ajoutées, et sur lesquelles
     portera l'algorithme de résolution TSP */
  Towns* cur_towns;

  Command commands[];
};

void env_freeTowns(Env* env);
void env_resetTowns(Env* env);


#endif
