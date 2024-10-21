#include "s21_cat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  flags *flag = calloc(1, sizeof(flags));
  int review = ArgsFlags(argv, flag, argc);
  if (review == 0 || review == 45) {
    readFile(flag);
  } else {
    fprintf(stderr, "флага - %c не существует", review);
  }
  for (int i = 0; i < flag->countName; i++) {
    free(flag->name[i]);
  }
  free(flag->name);
  free(flag);
  return 0;
}

void readFile(flags *flag) {
  int count = 1;
  int previousNumber = 10, freeStr = 0;
  for (int i = 0; i < flag->countName; i++) {
    FILE *file = fopen(flag->name[i], "r");
    if (file == NULL) {
      fprintf(stderr, "файл: %s не найден", flag->name[i]);
      continue;
    }
    int number;
    while ((number = fgetc(file)) != EOF) {
      int blankSrting = 0;
      if (previousNumber == 10 && number == 10) {
        freeStr++;
        blankSrting = 1;
      } else {
        blankSrting = 0;
        freeStr = 0;
      }
      if (flag->s && freeStr > 1) {
        continue;
      }
      if ((flag->n || (flag->b && blankSrting == 0)) && previousNumber == 10) {
        printf("%6d\t", count);
        count++;
      }
      if (flag->t && number == 9) {
        printf("^I");
        previousNumber = 9;
        continue;
      }
      if (flag->e && number == 10) printf("$");
      if (flag->v)
        v_print(number);
      else
        fputc(number, stdout);

      previousNumber = number;
    }
    fclose(file);
  }
  return;
}
void v_print(unsigned char c) {
  if (c == 9 || c == 10) {
    printf("%c", c);
  } else if (c >= 32 && c < 127) {
    printf("%c", c);
  } else if (c == 127) {
    printf("^?");
  } else if (c >= 160) {
    printf("M-");
    (c < 255) ? printf("%c", c - 128) : printf("^?");
  } else {
    (c > 32) ? printf("M-^%c", c - 64) : printf("^%c", c + 64);
  }
}
int ArgsFlags(char **argv, flags *flag, int argc) {
  int review = 0;
  flag->countName = 0;
  for (int i = 1; i < argc && (review == 0 || review == 45); i++) {
    if (argv[i][0] == '-') {
      review = ParsingFlags(flag, argv[i]);
    } else {
      flag->name = HighlightName(argv[i], flag->name, &flag->countName);
    }
  }
  if (flag->b == flag->n) {
    flag->n = 0;
  }
  return review;
}

int ParsingFlags(flags *flag, char *str) {
  int review = 0;
  if (!strcmp(str, "-b") || !strcmp(str, "--number-nonblank")) {
    flag->b = 1;
  } else if (!strcmp(str, "-e")) {
    flag->e = 1;
    flag->v = 1;
  } else if ((!strcmp(str, "-n") || !strcmp(str, "--number"))) {
    flag->n = 1;
  } else if (!strcmp(str, "-s") || !strcmp(str, "--squeeze-blank")) {
    flag->s = 1;
  } else if (!strcmp(str, "-t")) {
    flag->t = 1;
    flag->v = 1;
  } else if (!strcmp(str, "-v")) {
    flag->v = 1;
  } else if (!strcmp(str, "-T")) {
    flag->t = 1;
  } else if (!strcmp(str, "-E")) {
    flag->e = 1;
  } else {
    review = str[1];
  }
  return review;
}
