#include "parser.h"
#include "test_create_specs.h"
#include <stdlib.h>

BuildSpec * createSpec(Command * cmd, char * target, char ** deps, int depsLen) {
    BuildSpec * spec;
    spec = malloc(sizeof(BuildSpec));
    spec->cmds = cmd;
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
    cmd = malloc(sizeof(Command));
    cmd->args = malloc(sizeof(char**)*2);
    cmd->args[0] = malloc(sizeof(char*)*4);
    cmd->args[0][0] = "x";
    cmd->args[0][1] = "y";
    cmd->args[0][2] = "z";
    cmd->args[0][3] = "\0";
    cmd->args[1] = malloc(sizeof(char*)*3);
    cmd->args[1][0] = "v";
    cmd->args[1][1] = "w";
    cmd->args[1][2] = "\0";
    deps[0] = "b";
    deps[1] = "c";
    specs->list[0] = createSpec(cmd, "a", deps, depsLen);

    depsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    cmd = malloc(sizeof(char**)*1);
    cmd[0] = (Command)malloc(sizeof(char*)*2);
    cmd[0][0] = "r";
    cmd[0][1] = "\0";
    deps[0] = "d";
    specs->list[1] = createSpec(cmd, "b", deps, depsLen);

    depsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    cmd = malloc(sizeof(char**)*1);
    cmd[0] = (Command)malloc(sizeof(char*)*2);
    cmd[0][0] = "w";
    cmd[0][1] = "\0";
    deps[0] = "b";
    specs->list[2] = createSpec(cmd, "c", deps, depsLen);

    depsLen = 0;
    deps = malloc(sizeof(char*)*depsLen);
    cmd = malloc(sizeof(char**)*1);
    cmd[0] = (Command)malloc(sizeof(char*)*1);
    cmd[0][0] = "\0";
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
