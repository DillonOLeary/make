/**
 * authors: Dillon O'Leary
 * Ezra Boley
 */
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
#include "hash_map.h"
// The list of commands
//typedef struct {
//    Command ** list;
//    size_t used;
//    size_t size;
//} CommandListFinal;
// replaced this with
// Ezra command list, this one hold all the commands
// that need to run whereas Ezras holds the commands
// for each buildspec

/**
 * This function will create a list of command to execute and
 * check the logic of the spec list provided
 * Returns 0 on success, exits on failure
 */
int getCommandList(DAG_map * map, CommandList * cmdList, BuildSpecList * list);

#endif
