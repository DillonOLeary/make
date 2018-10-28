/**
 *  This module is responsible for parsing a makefile
 *  and returning a build specifications list
 */
 
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
    BuildSpec ** bSList;
    int len;
} BuildSpecList;

BuildSpecList * getBuildSpecList(FILE* fp);
