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

static int done = 0;
static Trie* towns = NULL;

static Graph* graph = NULL;

static Vector* townsNames = NULL;
static Vector* townsX = NULL;
static Vector* townsY = NULL;

static int townsNb = 0;

int is_white(char c);
char* stripwhite(char* str);
char* swhite_atBegin(char* str);
void execute_line(char* line);
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

void resetGraph() {
  freeGraph();

  townsNb = 0;

  graph = graph_new();
  townsNames = vector_new();
  townsX = vector_new();
  townsY = vector_new();
}


float dist(float x1, float y1, float x2, float y2) {
  return sqrtf(fabsf(x2 - x1) + fabsf(y2 - y1));
}

Graph* parse_EdgesDistances(FILE* f) {
  if(f != NULL) {
    rewind(f);
    int nodes_nb;
    fscanf(f, "%d!\n", &nodes_nb);
    
    Graph* g = graph_newWithNodes(nodes_nb+1);

    int n1, n2;
    float w;
    int ret;
    while((ret = fscanf(f, "%d %d: %f!\n", &n1, &n2, &w)) && (ret != EOF)) {
      graph_addEdge(g, n1, n2, w);
    }

    return g;
  }
  return NULL;
}

Graph* parse_VerticesCoordinates(FILE* f) {
  if(f != NULL) {
    rewind(f);
    int nodes_nb;
    fscanf(f, "%d!\n", &nodes_nb);
    
    Graph* g = graph_newWithNodes(nodes_nb+1);
    Vector* v = vector_newWithSize(nodes_nb+1);
    vector_fill(v, nodes_nb+1, (Generic)NULL);

    int i;
    size_t n;
    char* pos;
    int ret;
    while((ret = fscanf(f, "%d: ", &i)) && (ret != EOF)) {
      getline(&pos, &n, f);
      vector_set(v, i, (Generic)(void*)pos);
    }

    int j;
    for(i=0; i < nodes_nb+1; i++) {
      for(j=0; j < nodes_nb+1; j++) {
	if(i != j) { /* On ajoute pas d'arête de i vers lui-même */
	  float x1, y1, x2, y2;
	  sscanf(vector_get(v, i).p, "%f; %f!", &x1, &y1);
	  sscanf(vector_get(v, j).p, "%f; %f!", &x2, &y2);
	  graph_addEdge(g, i, j, dist(x1, x2, y1, y2));
	}
      }
    }

    for(i=0; i < nodes_nb+1; i++)
      free(vector_get(v, i).p);

    vector_free(v);

    return g;	
  }
  return NULL;
}

Trie* parse_Towns(FILE* f) {
  Trie* my_towns = trie_new();

  if(f != NULL) {
    rewind(f);

    char* line = NULL;
    size_t n;
    while(getline(&line, &n, f) != -1) {
      strtok(line, ":");
      char* split = strtok(NULL, ":");
      split[-1] = '\0'; /* Élimination du ':' */
      
      float x, y;
      sscanf(split, " %f; %f!", &x, &y);
      my_towns = trie_addTown(my_towns, line, x, y);
    }
    free(line);
  }

  return my_towns;
}

/* Commandes :
- load_edgelist
- load_coordinates
- load_db
- add
- list
- solve
- reset
- quit
- help
*/

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
    
    if(!line)
      break;
    
    s = stripwhite(line);

    if(s[0] != '\0') {
      add_history(s);
      execute_line(s);
    }

    free(line);
  }

  return 0;
}

int is_white(char c) {
  return c == ' ' ||
    c == '\n' ||
    c == '\t';
}

char* stripwhite(char* str) {
  int i = 0;
  int j = (int)strlen(str) - 1;

  while(is_white(str[j])) j--;
  str[j+1] = '\0';

  while(is_white(str[i])) i++;

  return &str[i];
}

char* swhite_atBegin(char* str) {
  int i = 0;
  while(is_white(str[i])) i++;

  return &str[i];
}

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
  } else if(!strncmp(&line[i], "list", j-i-1)) {
    /* Ne prend aucun argument */
    int u;
    for(u = 0; u < townsNb; u++) {
      printf("%s (%f, %f)\n", (char*)vector_get(townsNames, u).p, 
	     vector_get(townsX, u).f, vector_get(townsY, u).f);
    }
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
    
    Town* a_town;
    for(a_town = path; a_town != NULL; a_town = a_town->next) {
      fprintf(fout, "%s\n", (char*)vector_get(townsNames, a_town->id).p);
    }
    
    freeTowns(path);

    if(l-k > 0)
      fclose(fout);

  } else if(!strncmp(&line[i], "reset", j-i-1)) {
    /* Ne prend aucun argument */
    resetGraph();
  } else if(!strncmp(&line[i], "quit", j-i-1)) {
    /* Ne prend aucun argument */
    freeGraph();
    trie_free(towns);

    done = 1;
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

typedef struct word Word;
struct word {
  char letter;
  Word* tail;
};

Word* cons(char letter, Word* w) {
  Word* l = malloc(sizeof(Word));
  l->letter = letter;
  l->tail = w;
  return l;
}

typedef struct words Words;
struct words {
  char* word;
  Words* tail;
};

Words* cons_word(char* word, Words* list) {
  Words* w = malloc(sizeof(Words));
  w->word = word;
  w->tail = list;
  return w;
}

char* to_string(Word* w) {
  int i;
  Word* w_;
  for(i = 0, w_ = w; w_ != NULL; w_ = w_->tail, i++);

  char* s = malloc((i+1)*sizeof(char));
  s[i] = '\0';
  int j;
  for(j = i-1; w != NULL; w = w->tail, j--) {
    s[j] = w->letter;
  }

  return s;
}

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

char* words_generator(const char* text, int state) {
  static Words* list;

  if(state == 0) {
    Trie* t = towns;
    Word* acc = NULL;
    list = NULL;
    int i;

    if(t == NULL || t->letters == NULL)
      return NULL;

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

  while(list != NULL) {
    char* w = list->word;
    Words* tail = list->tail;
    free(list);    
    list = tail;

    return w;
  }

  return NULL;
}

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
      return(strdup(name));
    }
  }

  return NULL;
}

char** completion(const char* text, int start, int end) {
  char** matches = NULL;
  
  if(start == 0) {
    matches = rl_completion_matches(text, commands_generator);
  } else {
    if(!strncmp(swhite_atBegin(rl_line_buffer), "add", 3)) {
      matches = rl_completion_matches(text, words_generator);
    } else if(!strncmp(swhite_atBegin(rl_line_buffer), "help", 4)) {
      matches = rl_completion_matches(text, commands_generator);
    }
  }

  return matches;
}

void initialize_readline() {
  rl_attempted_completion_function = (rl_completion_func_t *)completion;
}
