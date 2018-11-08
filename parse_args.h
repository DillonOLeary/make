#ifndef __PARSE_ARG_H__
#define __PARSE_ARG_H__

#include <stdbool.h>

#define MAX_ARG_LENGTH 256
void parse_args(int argc, char *argv[], bool *fflag, char *filename, char* root);


#endif
