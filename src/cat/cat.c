#define _GNU_SOURCE

#include "cat.h"

int main(int argc, char* argv[]) {
  catFlags flags = whatCatFlags(argc, argv);
  // printf("b = %d E = %d n = %d s = %d T = %d v = %d index = %d\n", flags.b,
  // flags.E, flags.n, flags.s, flags.T, flags.v, flags.indexFile); printf("argc
  // = %d\n", argc);
  if (flags.err == 0) {
    while (flags.indexFile < argc) {
      writeText(flags, argv[flags.indexFile]);
      flags.indexFile++;
    }
  }
  return 0;
}

catFlags whatCatFlags(int argc, char* argv[]) {
  const struct option longFlags[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  char* shortFlags = "+beEnstTv";

  int indexLongFlag;

  catFlags flags = {0};
  char flag;
  while ((flag = getopt_long(argc, argv, shortFlags, longFlags,
                             &indexLongFlag)) != -1 &&
         flags.err == 0) {
    switch (flag) {
      case 'b':
        flags.b = 1;
        break;
      case 'e':
        flags.E = 1;
        flags.v = 1;
        break;
      case 'E':
        flags.E = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 's':
        flags.s = 1;
        break;
      case 't':
        flags.T = 1;
        flags.v = 1;
        break;
      case 'T':
        flags.T = 1;
        break;
      case 'v':
        flags.v = 1;
        break;
      default:
        flags.err = 1;
        break;
    }
  }
  if (flags.b == 1 && flags.n == 1) flags.n = 0;

  flags.indexFile = optind;
  return flags;
}

void writeText(catFlags flags, char* fileName) {
  FILE* stream;
  size_t emptyLineCount = 0;  // for -s

  size_t countLine = 0;     // for -n and -b
  size_t countLineNow = 0;  // for -n and -b

  stream = fopen(fileName, "r");
  if (stream == NULL) {
    printf("%s file not found.\n", fileName);
    return;
  }

  int ch = 0;
  while ((ch = fgetc(stream)) != EOF) {
    if (flags.s && checkSecondEmptyLine(&emptyLineCount, ch) > 2) continue;
    if (flags.n && numberingLine(&countLine, &countLineNow, ch))
      printf("%6lu\t", countLine);
    if (flags.b && numberingNotEmptyLine(&countLine, &countLineNow, ch))
      printf("%6lu\t", countLine);
    if (flags.E && insertDollar(ch)) printf("%c", '$');
    if (flags.T && swapTab(ch)) {
      printf("%c", '^');
      ch = 'I';
    }
    if (flags.v) flagV(&ch);

    printf("%c", ch);
  }
  fclose(stream);
}

void flagV(int* ch) {
  if ((*ch) > 127 && (*ch) < 160) printf("M-^");
  if (((*ch) < 32 && (*ch) != '\n' && (*ch) != '\t') || (*ch) == 127)
    printf("^");
  if (((*ch) < 32 || ((*ch) > 126 && (*ch) < 160)) && (*ch) != '\n' &&
      (*ch) != '\t')
    (*ch) = (*ch) > 126 ? (*ch) - 128 + 64 : (*ch) + 64;
}

int swapTab(char ch) {
  int ret = 0;
  if (ch == '\t') ret = 1;
  return ret;
}

int insertDollar(char ch) {
  int ret = 0;
  if (ch == '\n') ret = 1;
  return ret;
}

int numberingNotEmptyLine(size_t* countLine, size_t* countLineNow, char ch) {
  int ret = 0;
  if (*countLine == *countLineNow && ch != '\n') {
    (*countLine)++;
    ret = 1;
  }
  if (ch == '\n') *countLineNow = *countLine;
  return ret;
}

int numberingLine(size_t* countLine, size_t* countLineNow, char ch) {
  int ret = 0;
  if (*countLine == *countLineNow) {
    (*countLine)++;
    ret = 1;
  }
  if (ch == '\n') *countLineNow = *countLine;
  return ret;
}

size_t checkSecondEmptyLine(size_t* emptyLineCount, char ch) {
  if (ch == '\n')
    (*emptyLineCount)++;
  else
    *emptyLineCount = 0;
  return *emptyLineCount;
}