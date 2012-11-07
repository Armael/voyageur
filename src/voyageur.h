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

  /* Le graphe des villes sélectionnées, sur lesquelles on appliquera
     l'algorithme de résolution du problème du voyageur du commerce */
  Graph* graph;

  /* Un tableau, indicé par les nœuds du graphe, et qui à chacun associe
     le nom de la ville correspondante */
  Vector* townsNames;
  /* La même chose, mais associe la coordonnée X de la ville */
  Vector* townsX;
  /* La même chose, mais associe la coordonnée Y de la ville */ 
  Vector* townsY;

  /* Nombre de villes dans le graphe (égal au nombre de nœuds) */
  int townsNb;

  Command commands[];
};

void freeGraph(Env* env);
void resetGraph(Env* env);


#endif
