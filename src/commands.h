#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

void load_db(char** args, Env* env);
void load_edgelist(char** args, Env* env);
void load_coordinates(char** args, Env* env);
void add(char** args, Env* env);
void add_from_file(char** args,Env* env);
void list(char** args, Env* env);
void solve(char** args, Env* env);
void reset(char** args, Env* env);
void quit(char** args, Env* env);
void help(char** args, Env* env);

#endif
