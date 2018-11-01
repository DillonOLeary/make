#include "parser.h"
#include "test_create_specs.h"
#include <stdlib.h>

BuildSpec * createSpec(Command * cmd, char * target, char ** deps, int depsLen) {
    BuildSpec * spec;
    spec = malloc(sizeof(BuildSpec));
    spec->cmd = cmd;
    spec->target = target;
    spec->deps = deps;
    spec->depsLen = depsLen;
    return spec;
}


void createSpecList(BuildSpecList * specs) {
    Command * cmd = NULL;
    char** deps;
    specs->len = 4;
    specs->list = malloc(sizeof(BuildSpec*)*specs->len);

    int depsLen = 2;
    deps = malloc(sizeof(char*)*depsLen);
    deps[0] = "b";
    deps[1] = "c";
    specs->list[0] = createSpec(cmd, "a", deps, depsLen);

    depsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    deps[0] = "d";
    specs->list[1] = createSpec(cmd, "b", deps, depsLen);

    depsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    deps[0] = "b";
    specs->list[2] = createSpec(cmd, "c", deps, depsLen);

    depsLen = 0;
    deps = malloc(sizeof(char*)*depsLen);
    specs->list[3] = createSpec(cmd, "d", deps, depsLen);
    /*
    char** deps = NULL;
    int depsLen = 0;
    specs->len = 9;
    specs->list = malloc(sizeof(BuildSpec*)*specs->len);
    specs->list[0] = createSpec(cmd, "TESTe0", deps, depsLen);
    specs->list[1] = createSpec(cmd, "TESTd1", deps, depsLen);
    specs->list[2] = createSpec(cmd, "TESTl2", deps, depsLen);
    specs->list[3] = createSpec(cmd, "TESTs3", deps, depsLen);
    specs->list[4] = createSpec(cmd, "TESTb4", deps, depsLen);
    specs->list[5] = createSpec(cmd, "TESTw5", deps, depsLen);
    specs->list[6] = createSpec(cmd, "TESTy6", deps, depsLen);
    specs->list[7] = createSpec(cmd, "TESTq7", deps, depsLen);
    specs->list[8] = createSpec(cmd, "TESTp8", deps, depsLen);
*/
}
