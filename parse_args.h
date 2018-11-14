/**
 * authors: Dillon O'Leary
 * Ezra Boley
 */
#ifndef __PARSE_ARG_H__
#define __PARSE_ARG_H__

#include <stdbool.h>

#define MAX_ARG_LENGTH 1024

/* 
 * Handles just the flag based arguement parsing, to make sure that
 * valid input is given
 */
void parse_args(int argc, char *argv[], bool *fflag, char *filename);


#endif
