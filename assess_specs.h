/**
 * This module takes in a list of build specs as input,
 * assuming the fist element in the list is the root target,
 * and does a deep check of the build specs and logic of the
 * list. It will create a tree to interpret the build specs.
 * This module will return a list of Command structs for 
 * the execute module to deal with.
 */
#ifndef __assess__specs_h__
#define __assess__specs_h__
#include "parser.h"
// The list of commands
typedef struct {
    Command ** list;
    int len;
} CommandList;

/**
 * This function will create a list of command to execute and
 * check the logic of the spec list provided
 */
CommandList * getCommandList(CommandList * cmdList, BuildSpecList * specs);

#endif
