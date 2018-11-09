/**
 * authors: Dillon O'Leary
 * Ezra Boley
 */
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
    for (int i=0;i<cmd->argc;i++) {
        // assume there is a spce between the < > and the filenames
        if (cmd->argv[i][0] == '>') {
            if (cmd->outputSet == 1) {
                fprintf(stderr, "Cannot set output twice!\n");
                exit(-1);
            }
            cmd->outputSet = 1;
            if ( (i + 1) > cmd->argc) {
                fprintf(stderr, "No output file specified\n");
                exit(-1);
            }
            cmd->output = cmd->argv[i++ + 1]; 
        } else if (cmd->argv[i][0] == '<') {
            if (cmd->inputSet == 1) {
                fprintf(stderr, "Cannot set input twice!\n");
                exit(-1);
            }
            cmd->inputSet = 1;
            if ( (i + 1) > cmd->argc) {
                fprintf(stderr, "No input file specified\n");
                exit(-1);
            }
            cmd->input = cmd->argv[i++ + 1]; 

        } else {
            newArgv[newArrLen++] = cmd->argv[i];
        }
    }
    if ( newArrLen == 0) {
        newArgv[newArrLen++] = NULL;
    } else if ( newArgv[newArrLen - 1] != NULL) {
        newArgv[newArrLen++] = NULL;
    }
    cmd->argv = newArgv;
    cmd->argc = newArrLen;
}

/**
 * This changes redirection based on the command
 */
void setIO(Command * cmd) {
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
    if (childArgv == NULL) return;
    if (childArgv[0] == NULL) return;
    if ((child_pid = fork()) == 0) {
        // Child
        setIO(cmd);
        
        if (-1 == execvp(childArgv[0], childArgv)) {
            fprintf(stderr, "Error, quiting\n");
            exit(1);
        }
        
    } else {
        // Parent
        while (wait(&stat) != (int) child_pid);
    }
    
    if (WEXITSTATUS(&stat) == WEXITSTATUS(-1)) {
        fprintf(stderr, "Error executing command, ending program...\n"); 
        exit(-1);
    }
}

/* Creates each process (or child) */
void create_family(CommandList *cmdlist) {
    Command *cmd;
    cmd = cmdlist->frstCmd;

    for (int i = 0; i < cmdlist->len; i++) {
        create_child(cmd);
        cmd = next_cmd(cmd);
    }
}
