#ifndef MYGREP_H
#define MYGREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int eori;
  int e;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char pattern[10000];
  int indexFile;
  int err;
} grepFlags;  // FLAG;

grepFlags whatGrepFlags(int argc, char* argv[]);

void mainGrepFunc(int argc, char* argv[], grepFlags flags);

void writeTextStandartFormat(FILE* file, grepFlags flags, const regex_t* preg,
                             int countFiles, char* fileName);

void grepC(FILE* file, grepFlags flags, const regex_t* preg, char* fileName,
           int countFiles);

void grepL(FILE* file, grepFlags flags, const regex_t* preg, char* fileName);

void grepO(char* line, regmatch_t match, const regex_t* preg);

void grepF(char* optarg, grepFlags* flags);

#endif