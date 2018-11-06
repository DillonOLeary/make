#include "list_utils.h"


void append_build_spec(BuildSpecList *bsl, BuildSpec *bs) {
    if (bsl->len == 0) {
        bsl->frstBS = bs;
        bsl->lstBS = bs;
    } else {
        bsl->lstBS->nxtBuildSpec = bs;
        bsl->lstBS = bs;
    }
    bsl->len++;
}


BuildSpec *get_last_build_spec(BuildSpecList *bsl) {
    return bsl->lstBS;
}


void append_cmd_to_buildspec(BuildSpec *bs, Command *cmd) {
    if (bs->cmds->len == 0) {
        bs->cmds->frstCmd = cmd;
        bs->cmds->lstCmd = cmd;
    } else {
        bs->cmds->lstCmd->nxtCmd = cmd;
        bs->cmds->lstCmd = cmd;
    }
    bs->cmds->len++;
}

Command *get_last_cmd(BuildSpec *bs) {
    return bs->cmds->lstCmd;
}

void append_cmd_to_cmdlist(CommandList *cmdlist, Command *cmd) {
    if (cmdlist->len == 0) {
        cmdlist->frstCmd = cmd;
        cmdlist->lstCmd = cmd;
    } else {
        cmdlist->lstCmd->nxtCmd = cmd;
        cmdlist->lstCmd = cmd;
    }
    cmdlist->len++;
}

Command *next_cmd (Command *cmd) {
    return cmd->nxtCmd;
}

BuildSpec *next_BS (BuildSpec *bs) {
    return bs->nxtBuildSpec;
}

void init_cmd_list(CommandList *cmdList) {
    cmdList->len = 0;
}
