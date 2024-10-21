#ifndef CAT_H
#define CAT_H

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  char **name;
  int countName;
} flags;
char **HighlightName(char *str, char **name, int *countName);
int ArgsFlags(char **argv, flags *parrams, int argc);
void readFile(flags *flag);
void v_print(unsigned char c);
int ParsingFlags(flags *flag, char *str);
#endif