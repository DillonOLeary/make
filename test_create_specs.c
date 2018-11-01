#include "parser.h"
#include "test_create_specs.h"
#include <stdlib.h>

BuildSpec * createSpec(Command ** cmds, int cmdsLen, char * target, char ** deps, int depsLen) {
    BuildSpec * spec;
    spec = malloc(sizeof(BuildSpec));
    spec->cmds = cmds;
    spec->cmdsLen = cmdsLen;
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
    int cmdsLen = 2;
    deps = malloc(sizeof(char*)*depsLen);
    Command ** cmds = malloc(sizeof(Command*)*cmdsLen);
    Command * cmd1 = malloc(sizeof(Command));
    cmd1->args = malloc(sizeof(char*)*4);
    cmd1->args[0] = "x";
    cmd1->args[1] = "y";
    cmd1->args[2] = "z";
    cmd1->args[3] = "\0";
    Command * cmd2 = malloc(sizeof(Command));
    cmd2->args = malloc(sizeof(char*)*3);
    cmd2->args[0] = "v";
    cmd2->args[1] = "w";
    cmd2->args[2] = "\0";
    cmds[0] = cmd1;
    cmds[1] = cmd2;
    deps[0] = "b";
    deps[1] = "c";
    specs->list[0] = createSpec(cmds,cmdsLen, "a", deps, depsLen);

    depsLen = 1;
    cmdsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    cmds = malloc(sizeof(Command*)*cmdsLen);
    cmd = malloc(sizeof(Command));
    cmd->args = malloc(sizeof(char*)*2);
    cmd->args[0] = "r";
    cmd->args[1] = "\0";
    cmds[0] = cmd;
    deps[0] = "d";
    specs->list[1] = createSpec(cmds,cmdsLen, "b", deps, depsLen);

    depsLen = 1;
    cmdsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    cmds = malloc(sizeof(Command*)*cmdsLen);
    cmd = malloc(sizeof(Command));
    cmd->args = malloc(sizeof(char*)*2);
    cmd->args[0] = "w";
    cmd->args[1] = "\0";
    cmds[0] = cmd;
    deps[0] = "b";
    specs->list[2] = createSpec(cmds, cmdsLen, "c", deps, depsLen);

    depsLen = 0;
    cmdsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    cmds = malloc(sizeof(Command*)*cmdsLen);
    cmd = malloc(sizeof(Command));
    cmd->args = malloc(sizeof(char*)*1);
    cmd->args[0] = "\0";
    cmds[0] = cmd;
    specs->list[3] = createSpec(cmds, cmdsLen, "d", deps, depsLen);
    /*

    depsLen = 0;
    deps = malloc(sizeof(char*)*depsLen);
    cmd = malloc(sizeof(Command));
    cmd->args = malloc(sizeof(char**)*1);
    cmd->args[0] = malloc(sizeof(char*)*1);
    cmd->args[0][0] = "\0";
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
