


/* Creates da kids */
void create_family(CommandList *cmdlist) {
    Command *cmd;
    cmd = cmdlist->frstCmd;

    for (int i = 0; i < cmdlist->len; i++) {
        create_child(cmd);
        cmd = next_cmd(cmd);
    }
}

void create_child(Commmand *cmd) {
    uint32_t child_pid;
    if ((child_pid = fork()) == 0) {
        // Child
        char *proc = argv[0];
        for (int i = 0; i < cmd->argc; i++) argv[i - 1] = argv[i];
        execvp(proc, argv);
        
    } else {
        // Parent
        wait();
    }
}
