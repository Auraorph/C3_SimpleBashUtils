CC=gcc
FLAGS=-Wall -Wextra -Werror -std=c11
SOURCES_CAT=cat.c

all: s21_cat

s21_cat: $(SOURCES_CAT)
	$(CC) -g $(FLAGS) $^ -o s21_cat

catTest: rebuild
	sh testCat.sh

clangTest:
	clang-format -style=Google -n *.c *.h

rebuild: clean s21_cat

clean:
	rm -rf s21_cat
