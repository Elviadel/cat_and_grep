#include <stdlib.h>
#include <string.h>

char **HighlightName(char *str, char **name, int *countName) {
  name = realloc(name, sizeof(char *) * (*countName + 1));
  name[*countName] = calloc(strlen(str) + 1, sizeof(char));
  strcpy(name[(*countName)++], str);
  return name;
}