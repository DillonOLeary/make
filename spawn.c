#include "parser.h"
#include "list_utils.h"
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include<sys/wait.h>

void create_child(Command *cmd) {
    uint32_t child_pid;
    if ((child_pid = fork()) == 0) {
        // Child
        char *proc = cmd->argv[0];
        for (int i = 0; i < cmd->argc; i++) cmd->argv[i - 1] = cmd->argv[i];
        execvp(proc, cmd->argv);
        
    } else {
        // Parent
        wait();
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
