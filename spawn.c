#include "parser.h"
#include "list_utils.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>

void create_child(Command *cmd) {
    uint32_t child_pid;
    int stat;
    char *childArgv[cmd->argc + 1]; // add one for a null terminator
    for (int i = 0; i < cmd->argc; i++) childArgv[i] = cmd->argv[i];
    childArgv[cmd->argc] = NULL;
    //printf("CMD[0] -> %s\n", cmd->argv[2]);
    if ((child_pid = fork()) == 0) {
        // Child
        
        //cmd->argv[cmd->argc - 1] = '\0';
        //printf("PROC: %s || cmd->argv[0]: %s\n", childArgv[0], childArgv[1]); 
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
