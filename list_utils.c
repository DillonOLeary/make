#include "list_utils.h"


void append_build_spec(BuildSpecList *bsl, BuildSpec *bs) {
    bsl->lstBS->nxtBuildSpec = bs;
    bsl->lstBS = bs;
}


BuildSpec *get_last_build_spec(BuildSpecList *bsl) {
    return bsl->lstBS;
}


void append_cmd(BuildSpec *bs, Command *cmd) {
    bs->cmds->lstCmd->nxtCmd = cmd;
    bs->cmds->lstCmd = cmd;
}


Command *get_last_cmd(BuildSpec *bs) {
    return bs->cmds->lstCmd;
}
