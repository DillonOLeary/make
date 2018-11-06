#ifndef __LIST__UTILS_H__
#define __LIST__UTILS_H__
#include "parser.h"


/**
 * Append to the build spec list
 */
void append_build_spec(BuildSpecList *bsl, BuildSpec *bs);

BuildSpec *get_last_build_spec(BuildSpecList *bsl);

/*
 * Append to the command list WITHIN the
 * build spec
*/ 
void append_cmd_to_buildspec(BuildSpec *bs, Command *cmd);

Command *get_last_cmd(BuildSpec *bs);

Command *next_cmd (Command *cmd);

BuildSpec *next_BS (BuildSpec *bs);

void append_cmd_to_cmdlist(CommandList *cmdlist, Command *cmd);

void init_cmd_list(CommandList *cmdList);

#endif
