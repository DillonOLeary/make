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


void append_cmd(BuildSpec *bs, Command *cmd) {
    if (bs->cmds->len == 0) {
        bs->cmds->frstCmd = cmd;
        bs->cmds->lstCmd = cmd;
    } else {
        bs->cmds->lstCmd->nxtCmd = cmd;
        bs->cmds->lstCmd = cmd;
    }
    bs->cmds->len++;

    printf("here2\n");
}


Command *get_last_cmd(BuildSpec *bs) {
    return bs->cmds->lstCmd;
}
