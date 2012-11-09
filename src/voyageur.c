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

#include "vector.h"
#include "trie.h"
#include "tsp.h"

#include "towns.h"
#include "utils.h"
#include "colors.h"
#include "parsers.h"
#include "voyageur.h"
#include "commands.h"

#define MESSAGE "\
         Welcome to the TSP Solver\n\
                   v 0.1          \n\
  2012 - Armaël Guéneau & Valentin Gledel \n\
\n\
 You can type '%shelp%s' to have a list of commands,\n\
 and '%shelp <command>%s' for more exhaustive informations\n\n", \
    ANSI_BLUE, ANSI_NORMAL, ANSI_BLUE, ANSI_NORMAL

/* Informations sur les commandes disponibles (utilisé par
 * l'autocomplétion et l'aide sur les commandes)
 */

static Env env = { 0, NULL, NULL,
		   {
		     { "load_edgelist",
		       "Loads a file containing a list of edges",
		       "Loads a file describing the list of edges between \
towns (1st and simpler file format). Directly adds them to the graph, without \
using the database.",
		       load_edgelist },
		     { "load_coordinates",
		       "Loads a file containing towns coordinates",
		       "Loads a file describing the list of towns coordinates \
(2nd file format). Directly adds them to the graph, without using the database.",
		       load_coordinates },
		     { "load_db",
		       "Loads a file containing towns names & coordinates. Fills the DB",
		       "Loads a file describing the named towns with their \
coordinates (3rd file format), and fills the database.			\
The graph remains empty, towns had to be added with the			\
command \"add\".",
		       load_db },
		     { "add",
		       "Adds a town from the DB",
		       "Given it's name, adds a town from the database to \
the graph, to be used by the algorithm.",
		       add },
		     { "add_from_file",
		       "Adds the towns listed in a file from the DB",
		       "Calls add for each town name listed in the file \
given in argument\n",
		       add_from_file },
		     { "list",
		       "List the currently added towns",
		       "Lists the towns currently added to the graph",
		       list },
		     { "solve",
		       "Solves the salesman's problem. Can output the result in a file",
		       "Solves the business traveler's problem for the towns \
currently in the graph. A filename can be passed, in this		\
case, the path will be writen in it.",
		       solve },
		     { "reset",
		       "Removes all towns in the graph",
		       "Removes all towns in the graph.",
		       reset },
		     { "help",
		       "Displays help about commands",
		       "Given a command name, displays some help about it. If no command is given, display short description of all commands.",
		       help },
		     { "quit",
		       "Quits the program",
		       "Quits the program.",
		       quit },
		     { NULL,
		       NULL,
		       NULL,
		       NULL }
		   }
};

/* Fonctions écrites après le main de ce même fichier
 */
/* Exécute une ligne entrée par l'utilisateur, comportant une commande
   et d'éventuels arguments */
void execute_line(char* line);
/* Initialise la bibliothèque readline pour l'autocomplétion */
void initialize_readline();

void env_freeTowns(Env* env) {
  if(env->towns) {
    int u;
    for(u = 0; u < env->cur_towns->nb; u++) {
      free(vector_get(env->cur_towns->names, u).p);
    }
    towns_free(env->cur_towns);
  }
}

/* Réinitialise le graphe, libère la mémoire et le prépare à un nouvel
   usage */
void env_resetTowns(Env* env) {
  env_freeTowns(env);
  
  env->cur_towns = towns_new();
}

int main() {
  char *s, *line;

  printf(MESSAGE);

  env_resetTowns(&env);

  initialize_readline();

  while(env.done == 0) {
    char prompt[40];
    if(env.cur_towns->nb) {
      snprintf(prompt, 40, "(%d towns) %s>%s ", env.cur_towns->nb,
	       ANSI_GREEN, ANSI_NORMAL);
    } else {
      snprintf(prompt, 40, "%s>%s ", ANSI_GREEN, ANSI_NORMAL);
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
  line = stripwhite(line);

  char** args;
  int args_nb = 0;
  unsigned int i, toggle;
  size_t len = strlen(line);
  for(i=0, toggle=0; line[i] != '\0'; i++) {
    if(is_white(line[i])) {
      if(toggle) toggle = 0;
      line[i] = '\0';
    } else {
      if(!toggle) toggle = 1;
      args_nb++;
    }
  }
  
  args = malloc((args_nb+1) * sizeof(char*));
  
  int a = 0;
  for(i=0, toggle=0; i < len; i++) {
    if(line[i] == '\0') {
      if(toggle) toggle = 0;
    } else {
      if(!toggle) {
	toggle = 1;
	args[a++] = &line[i];
      }
    }
  }
  args[a] = NULL;

  /* Comparaisons pour trouver de quelle commande il s'agit */

  for(i=0; env.commands[i].name != NULL; i++) {
    if(!strcmp(args[0], env.commands[i].name)) {
      env.commands[i].f(args, &env);
      free(args);
      return;
    }
  }

  printf("Command not found : %s\n", args[0]);
  free(args);
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
    Trie* t = env.towns;
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

  while((name = env.commands[list_index].name) != NULL) {
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
