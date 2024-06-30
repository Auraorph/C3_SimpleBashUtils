#define _GNU_SOURCE

#include "grep.h"

int main(int argc, char* argv[]) {
  grepFlags flags = whatGrepFlags(argc, argv);
  if (flags.err)
    fprintf(stderr, "Invalid option.");
  else
    mainGrepFunc(argc, argv, flags);
  // printf("%s", flags.pattern);
  // printf("eori = %d v = %d c = %d l = %d n = %d h = %d s = %d o = %d index =
  // %d err = %d\n", flags.eori, flags.v, flags.c, flags.l, flags.n, flags.h,
  // flags.s, flags.o, flags.indexFile, flags.err); printf("argc = %d\n", argc);
  return 0;
}

grepFlags whatGrepFlags(int argc, char* argv[]) {
  char* shortFlags = "e:ivclnhsf:o";
  grepFlags flags = {0};
  char flag;
  while ((flag = getopt_long(argc, argv, shortFlags, NULL, NULL)) != -1 &&
         flags.err == 0) {
    switch (flag) {
      case 'e':
        strcat(flags.pattern, optarg);
        strcat(flags.pattern, "|");
        flags.e += 1;
        flags.eori = flags.eori | REG_EXTENDED;
        break;
      case 'i':
        flags.eori = flags.eori | REG_ICASE;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'c':
        flags.c = 1;
        break;
      case 'l':
        flags.l = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 'h':
        flags.h = 1;
        break;
      case 's':
        flags.s = 1;
        break;
      case 'f':
        flags.f += 1;
        grepF(optarg, &flags);
        flags.eori = flags.eori | REG_EXTENDED;
        break;
      case 'o':
        flags.o = 1;
        break;
      default:
        flags.err = 1;
        break;
    }
  }
  if (flags.e || flags.f) flags.pattern[strlen(flags.pattern) - 1] = '\0';
  flags.indexFile = optind;
  return flags;
}

void grepF(char* optarg, grepFlags* flags) {
  FILE* file = fopen(optarg, "rb");
  if (file == NULL) {
    fprintf(stderr, "%s file not found.", optarg);
    exit(1);
  }
  char* line = 0;
  size_t len = 0;
  while (getline(&line, &len, file) != -1) {
    int len = strlen(line);
    if (line[0] != '\n') {
      if (strstr(line, "\n"))
        line[len - 1] = '|';
      else
        line[len] = '|';
      strcat((*flags).pattern, line);
    }
  }
  // (*flags).pattern[strlen((*flags).pattern) - 1] = '\0';
  free(line);
  fclose(file);
}

void mainGrepFunc(int argc, char* argv[], grepFlags flags) {
  char** pattern = &argv[1];
  char** end = &argv[argc];
  for (; pattern != end && pattern[0][0] == '-'; pattern++)
    ;
  if (pattern == end) {
    if (flags.s == 0) fprintf(stderr, "Pattern not found.");
    return;
  }
  if (strcmp(flags.pattern, "") != 0) *pattern = flags.pattern;

  regex_t preg;

  if (regcomp(&preg, *pattern, flags.eori)) {
    if (flags.s == 0) fprintf(stderr, "Regex fail.");
    regfree(&preg);
    return;
  }

  // countFiles
  int countFiles = 0;
  if (flags.e || flags.f) {
    for (int i = 0; flags.indexFile + i < argc; i++, countFiles++)
      ;
  } else {
    char** patternTemp = &argv[flags.indexFile];
    for (int i = 1; flags.indexFile + i < argc; i++, patternTemp++) {
      if (patternTemp[0][0] != '-') countFiles++;
    }
  }

  if (countFiles == 0) {
    if (flags.s == 0) fprintf(stderr, "File not found.");
    regfree(&preg);
    return;
  }

  for (int i = 0; i < countFiles; i++) {
    char* fileName = (flags.e || flags.f) ? argv[flags.indexFile + i]
                                          : argv[flags.indexFile + i + 1];
    FILE* file = fopen(fileName, "rb");
    if (file == NULL) {
      if (flags.s == 0) fprintf(stderr, "%s file not found.", fileName);
      continue;
    }
    if (flags.c)  // c
      grepC(file, flags, &preg, fileName, countFiles);
    if (flags.l) grepL(file, flags, &preg, fileName);
    if (flags.l == 0 && flags.c == 0) {
      writeTextStandartFormat(file, flags, &preg, countFiles, fileName);
    }
    fclose(file);
    // regfree(&preg);
  }
  regfree(&preg);
}

void grepL(FILE* file, grepFlags flags, const regex_t* preg, char* fileName) {
  char* line = 0;
  size_t len = 0;
  regmatch_t match;
  // int countLine = 0;
  int end = 0;
  // int lena = 0;
  while (getline(&line, &len, file) != -1 && end == 0) {
    if (flags.v) {
      if (regexec(preg, line, 1, &match, 0)) end = 1;
    } else {
      if (!regexec(preg, line, 1, &match, 0)) end = 1;
    }
  }
  if (end == 1) printf("%s\n", fileName);

  free(line);
}

void grepC(FILE* file, grepFlags flags, const regex_t* preg, char* fileName,
           int countFiles) {
  char* line = 0;
  size_t len = 0;
  regmatch_t match;
  int countLine = 0;
  while (getline(&line, &len, file) != -1) {
    if (flags.v) {
      if (regexec(preg, line, 1, &match, 0)) ++countLine;
    } else {
      if (!regexec(preg, line, 1, &match, 0)) ++countLine;
    }
  }
  if (countFiles > 1 && flags.h == 0) printf("%s:", fileName);
  printf("%d\n", countLine);
  fseek(file, 0, SEEK_SET);
  free(line);
}

void writeTextStandartFormat(FILE* file, grepFlags flags, const regex_t* preg,
                             int countFiles, char* fileName) {
  char* line = 0;
  size_t len = 0;
  regmatch_t match;
  int countLine = 0;
  int mainGrepFunc = -1;
  while (getline(&line, &len, file) != -1) {
    countLine++;
    if (flags.v) {
      if (regexec(preg, line, 1, &match, 0)) {
        if (countFiles > 1 && flags.h == 0) printf("%s:", fileName);
        if (flags.n) printf("%d:", countLine);
        printf("%s", line);
        mainGrepFunc = countLine;
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        if (countFiles > 1 && flags.h == 0) printf("%s:", fileName);
        if (flags.n) printf("%d:", countLine);
        if (flags.o) {
          grepO(line, match, preg);
        } else {
          printf("%s", line);
          mainGrepFunc = countLine;
        }
      }
    }
  }
  if (mainGrepFunc == countLine) printf("\n");
  free(line);
}

void grepO(char* line, regmatch_t match, const regex_t* preg) {
  printf("%.*s\n", (int)(match.rm_eo - match.rm_so), line + match.rm_so);

  char* remainingLine = line + match.rm_eo;
  while (!regexec(preg, remainingLine, 1, &match, 0)) {
    printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
           remainingLine + match.rm_so);
    remainingLine += match.rm_eo;
  }
}