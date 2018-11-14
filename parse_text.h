/**
 * authors: Dillon O'Leary
 * Ezra Boley
 */
#ifndef __PARSE_TEXT__H__
#define __PARSE_TEXT__H__
#include <stdio.h>
#include <stdbool.h>
#include "parser.h"

// This file is more of the low level implentation of the parser
// than the parser.h which is more in charge of API

/* Splits the strings into individual words */
char **tokenize(char *line, int *depsLen);//, BuildSpecList *buildSpecList);

/*
* Opens up a makefile, Makefile, or custom filename
*/
FILE **open_makefile(bool is_custom_name, char *filename);

/* 
 * The overall wrapper that handles parsing the entire build file.
 * It sets up all of the commands that will need to be fulfilled
 */
int get_build_spec_list(BuildSpecList *specs, bool fflag, char *filename);

/* Gets a single line from an open file */
void get_file_line(FILE *fp, bool *isEnd, int lineNum, char *fileLine);

/* Breaks apart the line fills in the build spec list based on
 * that file line
 */
void parse_line(char *file_line, BuildSpecList *bsl, int lineNum);

#endif
