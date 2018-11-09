#include "parser.h"
#include "list_utils.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>

void parseAndCleanCmd(Command* cmd) {
    char** newArgv = malloc(sizeof(char*) * (cmd->argc + 1));
    int newArrLen = 0;
    printf("reached method, argc: %d\n", cmd->argc);
    for (int i=0;i<cmd->argc;i++) {
        // FIXME assume there is a spce between the < > and the filenames
        printf("Inside the loop: part of command%s\n",cmd->argv[i]);
        if (cmd->argv[i][0] == '>') {
            printf("reached a set output\n");
            if (cmd->outputSet == 1) {
                fprintf(stderr, "Cannot set output twice!");
                exit(-1);
            }
            cmd->outputSet = 1;
            if ( (i + 1) > cmd->argc) {
                fprintf(stderr, "No output file specified");
                exit(-1);
            }
            cmd->output = cmd->argv[i++ + 1]; 
        } else if (cmd->argv[i][0] == '<') {
            if (cmd->inputSet == 1) {
                fprintf(stderr, "Cannot set input twice!");
                exit(-1);
            }
            cmd->inputSet = 1;
            if ( (i + 1) > cmd->argc) {
                fprintf(stderr, "No input file specified");
                exit(-1);
            }
            cmd->input = cmd->argv[i++ + 1]; 

        } else {
            newArgv[newArrLen++] = cmd->argv[i];
        }
    }
    if ( newArgv[newArrLen - 1] != NULL) {
        newArgv[newArrLen++] = NULL;
    }
    cmd->argv = newArgv;
    cmd->argc = newArrLen;
    printf("arg1 %s, arg2 %s, len: %d\n", cmd->argv[0], cmd->argv[1], cmd->argc);
}

/**
 * This changes redirection based on the command
 */
void setIO(Command * cmd) {
    printf("setting io!output to  %s\n",cmd->output);
    if (cmd->outputSet == 1)
        freopen(cmd->output, "a+", stdout); 
    if (cmd->inputSet == 1)
        freopen(cmd->input, "a+", stdin);
}

void create_child(Command *cmd) {
    uint32_t child_pid;
    int stat;
    parseAndCleanCmd(cmd);
    char *childArgv[cmd->argc + 1]; // add one for a null terminator
    for (int i = 0; i < cmd->argc; i++) childArgv[i] = cmd->argv[i];
    childArgv[cmd->argc] = NULL;
    if ((child_pid = fork()) == 0) {
        // Child
        
        //cmd->argv[cmd->argc - 1] = '\0';
        //printf("PROC: %s || cmd->argv[0]: %s\n", childArgv[0], childArgv[1]); 
        // FIXME this needs to exit if there is an issue with the execution
        setIO(cmd);
        if (-1 == execvp(childArgv[0], childArgv)) {
            printf("Error, quiting\n");
            exit(-1);
        }
        
    } else {
        // Parent
        while (wait(&stat) != child_pid);
    }
    
    if (WEXITSTATUS(stat) == WEXITSTATUS(-1)) {
        fprintf(stderr, "Error executing command, ending program...\n"); 
        exit(-1);
    }
}

/* Creates da kids */
void create_family(CommandList *cmdlist) {
    Command *cmd;
    cmd = cmdlist->frstCmd;

    for (int i = 0; i < cmdlist->len; i++) {
        create_child(cmd);
        cmd = next_cmd(cmd);
    }
}
