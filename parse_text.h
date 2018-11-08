#ifndef __PARSE_TEXT__H__
#define __PARSE_TEXT__H__
#include <stdio.h>
#include <stdbool.h>
#include "parser.h"

char **tokenize();

FILE *open_makefile(bool is_custom_name, char *filename);

char **get_target_list();

char **get_deps();

BuildSpecList parse_makefile(bool fflag, char *filename);

char *get_file_line(FILE *fp, bool *isEnd, int lineNum);

void parse_line(char *file_line, BuildSpecList *bsl, int lineNum);

#endif
