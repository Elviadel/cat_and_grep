GCC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 -g

FILES = s21_cat.c
COMMON = ../common/

TARGET = s21_cat

all: $(TARGET) 

$(TARGET): obj
	$(GCC) $(FLAGS) s21_cat.o common.o -o $(TARGET)


obj:
	$(GCC) $(FLAGS) $(FILES) $(COMMON)common.c -c

clean:
	rm -rf $(TARGET) s21_cat.o common.o