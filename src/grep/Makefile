CC=gcc
FLAGS=-Wall -Wextra -Werror -std=c11
SOURCES_GREP=grep.c

all: s21_grep

s21_grep: $(SOURCES_GREP)
	$(CC) $(FLAGS) $^ -o s21_grep

grepTest: rebuild
	sh testGrep.sh

clangTest:
	clang-format -style=Google -n *.c *.h

rebuild: clean s21_grep

clean:
	rm -rf s21_grep
