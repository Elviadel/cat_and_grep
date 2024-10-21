#ifndef GREP_H
#define GREP_H

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int counteflags;
  char **name;
  int countName;
} GREP_SCT;

typedef struct _IO_FILE FILE;
typedef struct re_pattern_buffer regex_t;

char **HighlightName(char *str, char **name, int *countName);
char *CreateNewPattern(int j, char *str, int *review);
int StrLength(FILE *file);
int Match(char *str, GREP_SCT *origin, regex_t *regex);
int TreatmentEFlags(GREP_SCT **origin, int j, char *str, char **strRegex);
int TreatmentIput(GREP_SCT **origin, int argc, char *argv[], regex_t **regex);
int OutFlags(GREP_SCT *origin, char symbol);
void SerchStr(GREP_SCT *grep, regex_t *regex);
int FlagF(char *nameFile, char **strRegex, char *strf, int j);
char *FillingStrRegex(char *strRegex, char *str);
void FailPrint(GREP_SCT *grep, int matchFound, int countLine, int number, int i,
               int count, char *a, regex_t *regex);
void PrintOFlags(char *a, regex_t *regex, int i, int countLine, GREP_SCT *grep);

#endif