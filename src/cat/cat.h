#ifndef MYCAT_H
#define MYCAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int b;
  int E;
  int n;
  int s;
  int T;
  int v;
  int indexFile;
  int err;
} catFlags;  // FLAG;

catFlags whatCatFlags(int argc, char* argv[]);

void writeText(catFlags flags, char* fileName);

int insertDollar(char ch);

void flagV(int* ch);

int swapTab(char ch);

int numberingNotEmptyLine(size_t* countLine, size_t* countLineNow, char ch);

int numberingLine(size_t* countLine, size_t* countLineNow, char ch);

size_t checkSecondEmptyLine(size_t* emptyLineCount, char ch);

#endif