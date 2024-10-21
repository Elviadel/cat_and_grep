GCC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 -g

FILES = s21_grep.c
COMMON = ../common/

TARGET = s21_grep

ARGV = -e regex -e ^print s21_grep.c -v -l -f test_files/test_ptrn_grep.txt

all: $(TARGET) 

$(TARGET): obj
	$(GCC) $(FLAGS) s21_grep.o common.o -o $(TARGET)


obj:
	$(GCC) $(FLAGS) $(FILES) $(COMMON)common.c -c

diff: $(TARGET)
	./$(TARGET) $(ARGV) >> 123.txt
	grep $(ARGV) >> 1234.txt
	diff 123.txt 1234.txt

clean:
	rm -rf $(TARGET) s21_grep.o 1234.txt 123.txt common.o