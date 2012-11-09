#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generic.h"
#include "vector.h"
#include "towns.h"
#include "trie.h"
#include "tsp.h"

#include "utils.h"
#include "colors.h"
#include "parsers.h"
#include "voyageur.h"

void load_db(char** args, Env* env) {
  if(args[1] == NULL) {
    printf("Usage : \"load_db <filename>\" where <filename> is the file \
containing the database of towns with their coordinates\n");
    return;
  }

  char* filename = args[1];
  FILE* towns_db_f;
  if((towns_db_f = fopen(filename, "r")) == NULL) {
    printf("Error: Unable to open %s for reading\n", filename);
  } else {
    Trie* t = parse_Towns(towns_db_f);
    if(t == NULL) {
      printf("Syntax error\n");
      return;
    }

    if(env->towns != NULL) {
      trie_free(env->towns);
    }
    env->towns = t;
    fclose(towns_db_f);
  }
}

void load_edgelist(char** args, Env* env) {
  printf("Not implemented. Sorry :(.\n");
}

void load_coordinates(char** args, Env* env) {
  printf("Not implemented. Sorry :(.\n");
}

void add(char** args, Env* env) {
  if(args[1] == NULL) {
    printf("Usage : \"add <town name>\" where <town name> is a town in \
the database to add\n");
    return;
  }

  char* town_name = args[1];
  float x, y;
  if(trie_getCoord(env->towns, town_name, &x, &y) != 0) {
    printf("Error: the town \"%s\" is not in the database\n", town_name);
  } else {
    vector_set(env->cur_towns->x, env->cur_towns->nb, (Generic)x);
    vector_set(env->cur_towns->y, env->cur_towns->nb, (Generic)y);
    vector_set(env->cur_towns->names, env->cur_towns->nb, (Generic)(void*)strdup(town_name));
    
    env->cur_towns->nb++;
  }
}

void add_from_file(char** args, Env* env) {
  if(args[1] == NULL) {
    printf("Usage : \"add_from_file <filename>\" where <filename> is \
a file containing a list of names of towns (separed by \'\\n\') to add \
from the DB\n");
    return;
  }

  char* filename = args[1];
  FILE* f;
  if((f = fopen(filename, "r")) == NULL) {
    printf("Error : unable to open file \"%s\" for reading\n", filename);
  } else {
    char* s = NULL;
    size_t n;
    while(getline(&s, &n, f) != -1) {
      char* add_args[] = { "add", stripwhite(s), NULL };
      add(add_args, env);
    }
  
    free(s);
    fclose(f);
  }
}

void list(char** args, Env* env) {
  int u;
  for(u = 0; u < env->cur_towns->nb; u++) {
    printf("%s (%f, %f)\n", (char*)vector_get(env->cur_towns->names, u).p,
	   vector_get(env->cur_towns->x, u).f, vector_get(env->cur_towns->y, u).f);
  }
}

/* Applique l'algorithme tsp() sur les villes actuellement dans le
   graphe. Si un nom de fichier est passé en argument, écrit la
   tournée dans ce fichier, sinon l'affiche simplement */
void solve(char** args, Env* env) {
  char* fout_name = args[1];
  FILE* fout;
  if(fout_name == NULL) {
    fout = stdout;
  } else {
    if((fout = fopen(fout_name, "w")) == NULL){
      printf("Error: unable to open %s for writing\n", fout_name);
      return;
    }
  }

  printf("Calculating salesman's travel…\n");

  Town* path = tsp(env->cur_towns);

  /* Itération sur la tournée et affichage des villes */
  Town* a_town;
  for(a_town = path; a_town != NULL; a_town = a_town->next) {
    fprintf(fout, "%s\n", (char*)vector_get(env->cur_towns->names, a_town->id).p);
  }

  freeTowns(path);

  if(fout_name != NULL)
    fclose(fout);
}

void reset(char** args, Env* env) {
  env_resetTowns(env);
}

void quit(char** args, Env* env) {
  env_freeTowns(env);
  trie_free(env->towns);
  env->done = 1;
}

void help(char** args, Env* env) {
  char* cmd = args[1];
  unsigned int u, i;
  size_t n;
  if(cmd == NULL) {
    size_t size_max = 0;
    for(u = 0; env->commands[u].name != NULL; u++) {
      n = strlen(env->commands[u].name);
      if(n > size_max)
	size_max = n;
    }

    for(u = 0; env->commands[u].name != NULL; u++) {
      n = strlen(env->commands[u].name);
      printf("%s%s%s", ANSI_RED, env->commands[u].name, ANSI_NORMAL);
      for(i=0; i < size_max - n + 3; i++) putchar(' ');
      printf("%s\n", env->commands[u].shorthelp);
    }
  } else {
    for(u = 0; env->commands[u].name != NULL; u++) {
      if(!strcmp(env->commands[u].name, cmd)) {
	printf("%s\n", env->commands[u].help);
	break;
      }
    }
  }
}

