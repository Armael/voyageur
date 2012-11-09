#include <math.h>
#include <string.h>

#include "utils.h"

float dist2(float x1, float y1, float x2, float y2) {
  return powf(x2 - x1, 2) + powf(y2 - y1, 2);
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
