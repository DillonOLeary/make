/**
 *  This module is responsible for parsing a makefile
 *  and returning a build specifications list
 */
#ifndef __parser_h__
#define __parser_h__
 
// A command to execute
typedef struct {
    char * execName;
    char ** args;
    int argsLen;
} Command;

// Info for a build spec
typedef struct {
    Command * cmd;
    char * target;
    char ** deps;
    int depsLen;
} BuildSpec;

// A list of build specs
typedef struct {
    BuildSpec ** list;
    int len;
} BuildSpecList;

// FIXME undefine exit behavior. Should just exit.
/** This will parse the text and create the
 *  spec list, making the syntax is right
 *  returns 0 on success and -1 on failure, or exits.
 */
int getBuildSpecList(BuildSpecList* specs, FILE* fp);

#endif
