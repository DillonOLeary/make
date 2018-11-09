/**
 * authors: Dillon O'Leary
 * Ezra Boley
 */
/**
 *  This module is responsible for parsing a makefile
 *  and returning a build specifications list
 */
#ifndef __parser_h__
#define __parser_h__
#include <stdio.h>
 
// A command to execute
typedef struct Command {
    char ** argv;
    int argc;
    struct Command * nxtCmd;
    char* output;
    // the set ints are boolean flags, input
    // and output should only be set once
    int outputSet;
    int inputSet;
    char* input;
} Command;

typedef struct CommandList {
    Command *lstCmd;
    Command *frstCmd;
    int len;
} CommandList;

// Info for a build spec
typedef struct BuildSpec {
    CommandList *cmds;
    char * target;
    char ** deps;
    int depsLen;
    struct BuildSpec * nxtBuildSpec;
} BuildSpec;

// A list of build specs
typedef struct {
    BuildSpec * frstBS;
    BuildSpec * lstBS;
    int len;
    char* root;
} BuildSpecList;

/** This will parse the text and create the
 *  spec list, making the syntax is right
 *  returns 0 on success, exits on failure.
 */
int getBuildSpecList(BuildSpecList* specs, FILE* fp);

#endif
