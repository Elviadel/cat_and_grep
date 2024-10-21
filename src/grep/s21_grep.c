#include "s21_grep.h"

#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  regex_t *regex;
  GREP_SCT *grep_sct;
  int havePattern = TreatmentIput(&grep_sct, argc, argv, &regex);
  if (havePattern) {
    SerchStr(grep_sct, regex);
  }
  regfree(regex);
  for (int i = 0; i < grep_sct->countName; i++) {
    if (grep_sct->name[i] != NULL) free(grep_sct->name[i]);
  }
  free(regex);
  free(grep_sct->name);
  free(grep_sct);
  return 0;
}

char *CreateNewPattern(int j, char *str, int *review) {
  int len = strlen(str);
  int count = 0;
  char *newPattern = calloc(len - sizeof(char) * (j), sizeof(char));
  for (int i = j + 1; i < len; i++) {
    newPattern[count] = str[i];
    count++;
    *review = 0;
  }
  return newPattern;
}

int FlagF(char *nameFile, char **strRegex, char *strf, int j) {
  int number = 10;
  int review = 1;
  int isFile = 1;
  char *newPattern = CreateNewPattern(j, strf, &review);
  if (review) {
    free(newPattern);
    newPattern = nameFile;
  }
  FILE *fp = fopen(newPattern, "r");
  if (fp == NULL) {
    fprintf(stderr, "grep: %s: No such file or directory", newPattern);
    isFile = 0;
  }
  if (isFile) {
    int length = StrLength(fp);
    char *str = calloc(length + 2, sizeof(char));
    while (number != EOF) {
      fgets(str, length + 1, fp);
      *strRegex = FillingStrRegex(*strRegex, str);

      number = fgetc(fp);
      length = StrLength(fp);
      free(str);
      str = calloc(length + 2, sizeof(char));
    }
    fclose(fp);
    free(str);
  }
  if (!review) {
    free(newPattern);
  }
  return review;
}

int StrLength(FILE *file) {
  int number;
  int count = 0;
  long set = ftell(file);
  while (((number = fgetc(file)) != '\n') && number != EOF) {
    count++;
  }
  rewind(file);
  fseek(file, set, SEEK_CUR);
  return count;
}

void SerchStr(GREP_SCT *grep, regex_t *regex) {
  char *a = NULL;
  int count = 0;
  int countLine = 0;
  int matchFound = 0;
  for (int i = 0; i < grep->countName; i++) {
    count = 0;
    int number = 0;
    countLine = 0;
    FILE *file = fopen(grep->name[i], "r");
    if (file == NULL) {
      if (grep->s == 0) {
        fprintf(stderr, "grep: %s: No such file or directory", grep->name[i]);
      }
      continue;
    }

    while (number != EOF) {
      countLine++;
      int length = StrLength(file);
      a = calloc((length + 2), sizeof(char));
      fgets(a, length + 1, file);
      number = fgetc(file);
      matchFound = Match(a, grep, regex);
      count += matchFound;
      if (!grep->l) {
        FailPrint(grep, matchFound, countLine, number, i, count, a, regex);
        matchFound = 0;
      }
      free(a);
    }
    if (grep->l && count > 0) {
      printf("%s\n", grep->name[i]);
    }
    fclose(file);
  }
}

void FailPrint(GREP_SCT *grep, int matchFound, int countLine, int number, int i,
               int count, char *a, regex_t *regex) {
  if (grep->countName > 1 && matchFound && !grep->c && !grep->h && !grep->o) {
    printf("%s:", grep->name[i]);
  }
  if (grep->n && matchFound && !grep->o && !grep->c) {
    printf("%d:", countLine);
  }
  if (grep->c && number == EOF) {
    if (grep->countName > 1 && !grep->h) {
      printf("%s:", grep->name[i]);
    }
    printf("%d\n", count);
  }
  if (matchFound && grep->c == 0 && !grep->o) {
    printf("%s", a);
  }
  if (grep->o == 1 && grep->v == 0 && grep->c == 0) {
    PrintOFlags(a, regex, i, countLine, grep);
  }
  if ((matchFound && !grep->c && number != EOF && !grep->o)) {
    printf("\n");
  }
  if ((strlen(a) > 0 && number == EOF) && matchFound && !grep->c && !grep->o) {
    printf("\n");
  }
}

void PrintOFlags(char *a, regex_t *regex, int i, int countLine,
                 GREP_SCT *grep) {
  char *original_a = a;
  regmatch_t matches[1];
  while (regexec(regex, a, 1, matches, 0) == 0) {
    if (grep->countName > 1 && !grep->h) {
      printf("%s:", grep->name[i]);
    }
    if (grep->n) {
      printf("%d:", countLine);
    }
    for (int i = matches[0].rm_so; i < matches[0].rm_eo; i++) {
      printf("%c", a[i]);
    }
    printf("\n");
    a += matches[0].rm_eo;
  }
  a = original_a;
}

int Match(char *str, GREP_SCT *origin, regex_t *regex) {
  int matchFound = 0;
  if (!regexec(regex, str, 0, NULL, 0)) {
    matchFound = 1;
  }
  if (origin->v) {
    matchFound ? matchFound-- : matchFound++;
  }
  return matchFound;
}

int EorFFlags(int argc, char *argv[]) {
  int arguments = 0;
  int review = 0;
  for (int i = 1; i < argc; i++) {
    if (strstr(argv[i], "-")) {
      arguments--;
      if (strstr(argv[i], "e") || strstr(argv[i], "f")) {
        if (!arguments) {
          review = 1;
          i = argc;
          continue;
        }
        continue;
      }
    }
    arguments++;
  }
  return review;
}

int TreatmentIput(GREP_SCT **origin, int argc, char *argv[], regex_t **regex) {
  GREP_SCT *params = calloc(1, sizeof(GREP_SCT));
  *regex = (regex_t *)calloc(1, sizeof(regex_t));
  char *strRegex = calloc(1, sizeof(char));
  int review = 0, nextParams = 1, firstNameFiles = 0, reviewf = 0;
  if (EorFFlags(argc, argv)) {
    firstNameFiles = 1;
    nextParams = 0;
  }
  for (int i = 1; i < argc && review == 0; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; j < (int)strlen(argv[i]); j++) {
        review = OutFlags(params, argv[i][j]);
        if (params->e == 1) {
          nextParams = TreatmentEFlags(&params, j, argv[i], &strRegex);
          firstNameFiles = 0;
          j = strlen(argv[i]);
          continue;
        } else if (params->f == 1) {
          reviewf = FlagF(argv[i + 1], &strRegex, argv[i], j);
          reviewf ? i++ : i;
          nextParams = 0;
          j = strlen(argv[i]);
          continue;
        }
      }
      if (review != 0) {
        fprintf(
            stderr,
            "grep: invalid option -- '%c'\nUsage: grep [OPTION]... PATTERNS "
            "[FILE]...\nTry 'grep --help' for more information.\n",
            review);
      }
    } else if (nextParams && !firstNameFiles) {
      strRegex = FillingStrRegex(strRegex, argv[i]);
      nextParams = 0;
    } else {
      params->name = HighlightName(argv[i], params->name, &params->countName);
    }
  }
  int havePattern = 0;
  if (strlen(strRegex) > 0) {
    params->i ? regcomp(*regex, strRegex, REG_EXTENDED | REG_ICASE)
              : regcomp(*regex, strRegex, REG_EXTENDED);
    havePattern = 1;
  }
  *origin = params;
  free(strRegex);
  return havePattern;
}

int TreatmentEFlags(GREP_SCT **origin, int j, char *str, char **strRegex) {
  int review = 1;
  char *newPattern = CreateNewPattern(j, str, &review);
  if (!review) {
    *strRegex = FillingStrRegex(*strRegex, newPattern);
  }
  (*origin)->e = 0;
  free(newPattern);
  return review;
}

char *FillingStrRegex(char *strRegex, char *str) {
  if (strlen(strRegex)) {
    strRegex = realloc(strRegex, sizeof(char) * (strlen(strRegex) + 2));
    strcat(strRegex, "|");
  }
  strRegex = realloc(strRegex, strlen(strRegex) + strlen(str) + 1);
  strcat(strRegex, str);
  return strRegex;
}

int OutFlags(GREP_SCT *origin, char symbol) {
  int review = 0;
  switch (symbol) {
    case 'e':
      origin->e = 1;
      break;
    case 'i':
      origin->i = 1;
      break;
    case 'v':
      origin->v = 1;
      break;
    case 'c':
      origin->c = 1;
      break;
    case 'l':
      origin->l = 1;
      break;
    case 'n':
      origin->n = 1;
      break;
    case 'h':
      origin->h = 1;
      break;
    case 's':
      origin->s = 1;
      break;
    case 'f':
      origin->f = 1;
      break;
    case 'o':
      origin->o = 1;
      break;
    default:
      review = symbol;
  }
  return review;
}