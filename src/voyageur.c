#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <math.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "graph.h"
#include "vector.h"
#include "trie.h"
#include "tsp.h"

#include "utils.h"
#include "parsers.h"

/* Informations sur les commandes disponibles (utilisé par
 * l'autocomplétion et l'aide sur les commandes)
 */

/* Noms des différentes commandes utilisables */
char* commands[] = {
  "load_edgelist",
  "load_coordinates",
  "load_db",
  "add",
  "list",
  "solve",
  "reset",
  "quit",
  "help",
  NULL
};

/* Chaîne d'aide décrivant l'utilisation de la commande située au même
   indice dans le tableau commands[] */
char* commands_help[] = {
  "Loads a file describing the list of edges between \
towns (1st and simpler file format). Directly fills \
the graph, without using the database.",
  "Loads a file describing the list of towns coordinates \
(2nd file format). Directly fills the graph, without \
using the database.",
  "Loads a file describing the named towns with their \
coordinates (3rd file format), and fills the database. \
The graph remains empty, towns had to be added with the \
command \"add\".",
  "Given it's name, adds a town from the database to \
the graph, to be used by the algorithm.",
  "Lists the towns currently added to the graph",
  "Solves the business traveler's problem for the towns \
currently in the graph. A filename can be passed, in this \
case, the path will be writen in it.",
  "Removes all towns in the graph.",
  "Quits the program.",
  "Given a command name, displays some help about it.",
  NULL
};

/* Variables utilisées et modifiées par les différentes fonction du
 * fichier (notamment la fonction d'exécution des comandes)
 */

/* Quand est à 1, indique que l'on doit sortir de la boucle
   d'évaluation de commandes */
static int done = 0;
/* La base de données contenant des noms de villes, associés à leurs
   coordonnées */
static Trie* towns = NULL;

/* Le graphe des villes sélectionnées, sur lesquelles on appliquera
   l'algorithme de résolution du problème du voyageur du commerce */
static Graph* graph = NULL;

/* Un tableau, indicé par les nœuds du graphe, et qui à chacun associe
   le nom de la ville correspondante */
static Vector* townsNames = NULL;
/* La même chose, mais associe la coordonnée X de la ville */
static Vector* townsX = NULL;
/* La même chose, mais associe la coordonnée Y de la ville */ 
static Vector* townsY = NULL;

/* Nombre de villes dans le graphe (égal au nombre de nœuds) */
static int townsNb = 0;

/* Fonctions écrites après le main de ce même fichier
 */
/* Exécute une ligne entrée par l'utilisateur, comportant une commande
   et d'éventuels arguments */
void execute_line(char* line);
/* Initialise la bibliothèque readline pour l'autocomplétion */
void initialize_readline();

void freeGraph() {
  graph_free(graph);

  int u;
  for(u = 0; u < townsNb; u++) {
    free(vector_get(townsNames, u).p);
  }
  vector_free(townsNames);
  vector_free(townsX);
  vector_free(townsY);
}

/* Réinitialise le graphe, libère la mémoire et le prépare à un nouvel
   usage */
void resetGraph() {
  freeGraph();

  townsNb = 0;

  graph = graph_new();
  townsNames = vector_new();
  townsX = vector_new();
  townsY = vector_new();
}

int main() {
  char *s, *line;

  resetGraph();

  initialize_readline();

  while(done == 0) {
    char prompt[20];
    if(townsNb) {
      snprintf(prompt, 20, "(%d towns) > ", townsNb);
    } else {
      snprintf(prompt, 20, "> ");
    }

    line = readline(prompt);
    
    if(line) {
      s = stripwhite(line);

      if(s[0] != '\0') {
	add_history(s);
	execute_line(s);
      }
      
      free(line);
    }
  }

  return 0;
}

/* Prend une ligne, entrée par l'utilisateur, se composant d'une
   commande et de ses éventuels arguments, et l'exécute */
void execute_line(char* line) {
  /* Élimination des potentiels caractères blancs en début de ligne.
   i : début de la commande */
  int i = 0;
  while(line[i] != '\0' && is_white(line[i])) i++;
  
  /* On cherche la fin de la commande (qui n'est composée que de
     caractères non blancs.
  j : premier blanc après la commande */
  int j = i;
  while(line[j] != '\0' && !is_white(line[j])) j++;
  
  /* k : Début du premier argument */
  int k = j;
  while(line[k] != '\0' && is_white(line[k])) k++;
  
  /* l : Premier blanc après le premier argument */
  int l = k;
  while(line[l] != '\0' && !is_white(line[l])) l++;

  /* Comparaisons pour trouver de quelle commande il s'agit */

  /* load_db : Commande de chargement de la base de données.
     Prend un argument : le nom du fichier contenant les villes */
  if(!strncmp(&line[i], "load_db", j-i-1)) {
    /* Premier argument : fichier contenant la base */
    if(l-k == 0) {
      printf("Usage : \"load_db <filename>\" where <filename> is the file \
containing the database of towns with their coordinates\n");
    } else {
      line[l] = '\0';
      char* filename = &line[k];
      FILE* towns_db_f;
      if((towns_db_f = fopen(filename, "r")) == NULL) {
	printf("Error: Unable to open %s\n", filename);
      } else {
	if(towns != NULL) {
	  trie_free(towns);
	}
	towns = parse_Towns(towns_db_f);
	fclose(towns_db_f);
      }
    }

    /* Ajoute une ville de la BDD au graphe, le nom étant passé en
       argument */
  } else if(!strncmp(&line[i], "add", j-i-1)) {
    /* Premier argument : ville à ajouter au graphe */
    if(l-k == 0) {
      printf("Usage : \"add <town name>\" where <town name> is a town in \
the database to add\n");
    } else {
      float x, y;
      line[l] = '\0';
      char* town_name = &line[k];
      if(trie_getCoord(towns, town_name, &x, &y) != 0) {
	printf("Error: this town is not in the database\n");
      } else {
	vector_set(townsX, townsNb, (Generic)x);
	vector_set(townsY, townsNb, (Generic)y);
	graph_addNode(graph); /* retourne townsNb */
	vector_set(townsNames, townsNb, (Generic)(void*)strdup(town_name));

	/* Connection à tous les autres nœuds préexistants */
	int u;
	for(u = 0; u < townsNb; u++) {
	  float d = dist(x, y, vector_get(townsX, u).f, vector_get(townsY, u).f);
	  graph_addEdge(graph, u, townsNb, d);
	}

        townsNb++;
      }
    }

    /* Liste les noms et coordonnées des villes actuellement ajoutées
       au graphe */
  } else if(!strncmp(&line[i], "list", j-i-1)) {
    /* Ne prend aucun argument */
    int u;
    for(u = 0; u < townsNb; u++) {
      printf("%s (%f, %f)\n", (char*)vector_get(townsNames, u).p, 
	     vector_get(townsX, u).f, vector_get(townsY, u).f);
    }

    /* Applique l'algorithme tsp() sur les villes actuellement dans le
       graphei. Si un nom de fichier est passé en argument, écrit la
       tournée dans ce fichier, sinon l'affiche simplement */
  } else if(!strncmp(&line[i], "solve", j-i-1)) {
    /* Prend comme argument le fichier dans lequel écrire la
       tournée. Si aucun fichier n'est passé en argument, écrit sur la
       sortie standard */
    FILE* fout;
    if(l-k == 0) {
      fout = stdout;
    } else {
      line[l] = '\0';
      if((fout = fopen(&line[k], "w")) == NULL) {
	printf("Error: unable to open %s for writing\n", &line[k]);
	return;
      }
    }

    Town* path = tsp(graph);
    
    /* Itération sur la tournée et affichage des villes */
    Town* a_town;
    for(a_town = path; a_town != NULL; a_town = a_town->next) {
      fprintf(fout, "%s\n", (char*)vector_get(townsNames, a_town->id).p);
    }
    
    freeTowns(path);

    if(l-k > 0)
      fclose(fout);

    /* Réinitialise le graphe */
  } else if(!strncmp(&line[i], "reset", j-i-1)) {
    /* Ne prend aucun argument */
    resetGraph();

    /* Quitte le programme */
  } else if(!strncmp(&line[i], "quit", j-i-1)) {
    /* Ne prend aucun argument */
    freeGraph();
    trie_free(towns);

    /* Indique que l'on sort de la boucle REPL */
    done = 1;

    /* Affiche de l'aide sur une commande */
  } else if(!strncmp(&line[i], "help", j-i-1)) {
    /* Prend une commande en argument */
    if(l-k == 0) {
      printf("Usage: \"help <command>\" where <command> is the \
command to display informations\n");
    } else {
      line[l] = '\0';

      int u;
      for(u = 0; commands[u] != NULL; u++) {
	if(!strcmp(commands[u], &line[k])) {
	  printf("%s\n", commands_help[u]);
	  break;
	}
      }
    }

    /*** Code mort ***/
    /* Anciennent utilisé à des fins de debug, set et get permettent
       très basiquement de lire/écrire dans la BDD */
  } else if(!strncmp(&line[i], "get", j-i-1)) {
    float x, y;
    char* s = stripwhite(&line[j]);
    int err = trie_getCoord(towns, s /* stripwhite(&line[j])*/, &x, &y);
    if(!err) {
      printf("%f, %f\n", x, y);
    } else {
      printf("Erreur : ce mot n'est pas dans la base de données\n");
    }

  } else if(!strncmp(&line[i], "set", j-i-1)) {
    float x, y;
    char w[200];
    sscanf(stripwhite(&line[j]), "%s %f %f", w, &x, &y);
    towns = trie_addTown(towns, w, x, y);
  } else if(!strncmp(&line[i], "quit", j-i-1)) {
    trie_free(towns);
    done = 1;
  } else {
    int i;
    printf("Available commands :\n\n");
    for(i = 0; commands[i] != NULL; i++) {
      printf("%s : %s\n", commands[i], commands_help[i]);
    }
  }
}

/*
 * Readline completion
 */

/* Structure décrivant un mot comme liste chainée de lettres. Dans la
suite, sera en fait utilisé pour représenter le miroir du mot
réellement voulu, les lettres étant empilées */
typedef struct word Word;
struct word {
  char letter;
  Word* tail;
};

/* Ajoute une lettre en tête de mot */
Word* cons(char letter, Word* w) {
  Word* l = malloc(sizeof(Word));
  l->letter = letter;
  l->tail = w;
  return l;
}

/* Structure décrivant une liste de mots (un mot étant un
   char*). Utilisée par la suite par la fonction retournant l'ensemble
   des mots d'un trie */
typedef struct words Words;
struct words {
  char* word;
  Words* tail;
};

/* Ajoute un mot en tête de liste */
Words* cons_word(char* word, Words* list) {
  Words* w = malloc(sizeof(Words));
  w->word = word;
  w->tail = list;
  return w;
}

/* Convertit un mot sous forme de la liste des lettres *en partant de
   la fin* (le mot représenté est en fait le miroir) en une chaine de
   caractères (qui devra être libérée - en fait elle le sera par la
   bibliothèque readline pour l'utilisation qui en est faite) */
char* to_string(Word* w) {
  int i;
  Word* w_;
  /* Premier parcours de la liste pour trouver la longueur du mot */
  for(i = 0, w_ = w; w_ != NULL; w_ = w_->tail, i++);

  char* s = malloc((i+1)*sizeof(char));
  s[i] = '\0';
  int j;
  for(j = i-1; w != NULL; w = w->tail, j--) {
    s[j] = w->letter;
  }

  return s;
}

/* Retourne la liste des mots contenus dans un trie. Ceci est en fait
   vrai pour acc == NULL et list == NULL, la fonction étant récursive
   et se servant d'accumulateurs (acc et list).  list est
   l'accumulateur des mots déja trouvés, et acc l'accumulateur des
   lettres au début du mot en cours */ 
Words* explore(Trie* t, Word* acc, Words* list) {
  if(t == NULL || t->letters == NULL) return list;
  Letter* l;
  for(l = t->letters; l != NULL; l = l->tail) {
    if(l->next != NULL) {
      Word* acc_ = cons(l->letter, acc);
      if(l->next->is_word) {
	list = cons_word(to_string(acc_), list);
      }
      
      list = explore(l->next, acc_, list);
      free(acc_);
    }
  }
  return list;
}

/* Fonction utilisée directement par la bibliothèque readline. Elle
   est utilisée pour récupérer les différentes possibilités de
   completion *pour les mots du trie*. Le mot à compléter est
   text. Lorsque la fonction est appellée pour la première fois, state
   == 0. Elle calcule alors les différentes colpmétions possibles
   grâce à explore(). words_generator() est ensuite appellée plusieurs
   fois, et doit retourner à chaque appel une nouvelle possibilité de
   completion (qu'elle pioche parmi celles précalculées). Quand il n'y
   en a plus, elle retourne NULL. Les complétions sont libérées par la
   bibliothèque readline */
char* words_generator(const char* text, int state) {
  /* Grâce au mot clef static, la valeur de list est conservée d'un
     appel sur l'autre */
  static Words* list;

  if(state == 0) {
    Trie* t = towns;
    Word* acc = NULL;
    list = NULL;
    int i;

    if(t == NULL || t->letters == NULL)
      return NULL;

    /* Calcul de l'accumulateur initial acc, correspondant aux lettres
       du préfixe que l'on nous a donné (text). On parcourt donc le
       trie, jusqu'à obtenir le sous-trie qu'il faudra explorer pour
       connaître les possibilités de completion */
    for(i = 0; text[i] != '\0'; i++) {
      if(t == NULL || t->letters == NULL) {
	return NULL;
      } else {
	acc = cons(text[i], acc);
	Letter* l = trie_getLetter(t, text[i]);
	if(l == NULL) 
	  return NULL;
	t = l->next;
      }
    }

    if(t != NULL && t->is_word) {
      list = cons_word(to_string(acc), list);
    }
    list = explore(t, acc, list);
  
    while(acc != NULL) {
      Word* n = acc->tail;
      free(acc);
      acc = n;
    }
  }

  /* À chaque appel, on retourne une nouvelle possibilité de
     completion en avançant dans list */
  while(list != NULL) {
    char* w = list->word;
    Words* tail = list->tail;
    free(list);    
    list = tail;

    return w;
  }

  return NULL;
}

/* Fonction similaire à words_generator(), celle-ci génère les
   possibilités de completion pour les *commandes*. Il suffit d'itérer
   sur les noms de commandes (commands[]) et de regarder les commandes
   commençant par text */
char* commands_generator(const char* text, int state) {
  static int list_index;
  static size_t len;
  char* name;

  if(state == 0) {
    list_index = 0;
    len = strlen(text);
  }

  while((name = commands[list_index]) != NULL) {
    list_index++;
    
    if(strncmp(name, text, len) == 0) {
      /* On strdup() le nom, car readline essaye de libérer la mémoire
	 des chaines qu'on lui passe, ce qui échouerait si on lui
	 passait uniquement name, qui est une chaine statique (section
	 ROData du binaire) */
      return(strdup(name));
    }
  }

  return NULL;
}

/* Fonction également utilisée directement par readline. Étant donné
   une chaîne à compléter, completion() doit retourner les différentes
   possibilités. Il s'agit en fait de vérifier la nature de la
   complétion (sur une commande/sur un mot du trie) et de se servir
   des *_generator() correspondants. Si l'on renvoie NULL, alors
   readline fera de l'autocomplétion sur les noms de fichiers (utilisé
   pour les commandes load_*).
   start et end indiquent la position de text (le motif à compléter)
   dans la ligne de commande globale (contenue dans rl_line_buffer) */
char** completion(const char* text, int start, int end) {
  char** matches = NULL;
  
  if(start == 0) {
    /* On complète au début sur les commandes */
    matches = rl_completion_matches(text, commands_generator);
  } else {
    if(!strncmp(swhite_atBegin(rl_line_buffer), "add", 3)) {
      /* add ajoute un mot du trie */
      matches = rl_completion_matches(text, words_generator);
    } else if(!strncmp(swhite_atBegin(rl_line_buffer), "help", 4)) {
      /* help prend une commande en argument */
      matches = rl_completion_matches(text, commands_generator);
    }
  }

  return matches;
}

/* Renseigne la fonction pour la complétion à utiliser */
void initialize_readline() {
  rl_attempted_completion_function = (rl_completion_func_t *)completion;
}
