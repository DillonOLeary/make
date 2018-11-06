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
    cmd1->argv = malloc(sizeof(char*)*4);
    cmd1->argv[0] = "test_a_cmd_1";
    cmd1->argv[1] = "y";
    cmd1->argv[2] = "z";
    cmd1->argv[3] = "\0";
    Command * cmd2 = malloc(sizeof(Command));
    cmd2->argv = malloc(sizeof(char*)*3);
    cmd2->argv[0] = "test_a_cmd_2";
    cmd2->argv[1] = "w";
    cmd2->argv[2] = "\0";
    cmds[0] = cmd1;
    cmds[1] = cmd2;
    deps[0] = "test_b.txt";
    deps[1] = "test_c.txt";
    specs->list[0] = createSpec(cmds,cmdsLen, "test_a.txt", deps, depsLen);

    depsLen = 1;
    cmdsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    cmds = malloc(sizeof(Command*)*cmdsLen);
    cmd = malloc(sizeof(Command));
    cmd->argv = malloc(sizeof(char*)*2);
    cmd->argv[0] = "test_b_cmd";
    cmd->argv[1] = "\0";
    cmds[0] = cmd;
    deps[0] = "test_d.txt";
    specs->list[1] = createSpec(cmds,cmdsLen, "test_b.txt", deps, depsLen);

    depsLen = 1;
    cmdsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    cmds = malloc(sizeof(Command*)*cmdsLen);
    cmd = malloc(sizeof(Command));
    cmd->argv = malloc(sizeof(char*)*2);
    cmd->argv[0] = "test_c_cmd";
    cmd->argv[1] = "\0";
    cmds[0] = cmd;
    deps[0] = "test_b.txt";
    specs->list[2] = createSpec(cmds, cmdsLen, "test_c.txt", deps, depsLen);

    depsLen = 0;
    cmdsLen = 1;
    deps = malloc(sizeof(char*)*depsLen);
    cmds = malloc(sizeof(Command*)*cmdsLen);
    cmd = malloc(sizeof(Command));
    cmd->argv = malloc(sizeof(char*)*1);
    cmd->argv[0] = "\0";
    cmds[0] = cmd;
    specs->list[3] = createSpec(cmds, cmdsLen, "test_d.txt", deps, depsLen);
}
