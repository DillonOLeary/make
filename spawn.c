#include "parser.h"
#include "list_utils.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include<sys/wait.h>

void create_child(Command *cmd) {
    uint32_t child_pid;
    int stat;
    if ((child_pid = fork()) == 0) {
        // Child
        char *proc = cmd->argv[0];
        for (int i = 0; i < cmd->argc; i++) cmd->argv[i - 1] = cmd->argv[i];
        if (-1 == execvp(proc, cmd->argv)) {
            exit(-1);
        }
        
    } else {
        // Parent
        wait(&stat);
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
