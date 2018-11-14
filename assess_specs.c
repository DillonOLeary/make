/**
 * authors: Dillon O'Leary
 * Ezra Boley
 */
#include <stdlib.h>
#include "assess_specs.h"
#include "hash_map.h"
#include "list_utils.h"
#include "free_str.h"
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


time_t getLastMod(char* filename) {
    struct stat buf;
    if (-1 == stat(filename, &buf)) {
        fprintf(stderr, "Cannot open target stat file: '%s', file many not exist\n", filename);
        //exit(-1);
        return 0;
    }
    return buf.st_mtime;
}


/**
 * check the deps of the node. assume that it is file
 * and  a target execute if any of the deps 
 * are files that are modified more recently or
 * if any are builds specs that have executed
 * return 1 for yes, -1 for no
 */
int checkDeps(DAG_map * map, BuildSpecNode* node) {
    for (int i=0; i<node->data->depsLen; i++) {
        // in this case, one of the deps is a target which has 
        // added its cmds to the cmd list
        if (1 == lookup(map, node->data->deps[i])->hasExec) {
            return 1;
        }
        if (getLastMod(node->data->target) < getLastMod(node->data->deps[i])) {
            return 1;
        }
    }
    return -1;
}

/**
 * Return 1 for yes, -1 for no
 * Execute if target is not file.
 * If it is check deps.
 */
int shouldExec(DAG_map* map, BuildSpecNode * node) {
    // check if it's a file
    struct stat buf;
    if (-1 == stat(node->data->target, &buf)) {
        return 1;
    }
    // check the dependencies
    return checkDeps(map, node);
}

void add_cmd_to_list(BuildSpecNode * node, CommandList* cmdList) {
    node->hasExec = 1;
    Command* currCmd = node->data->cmds->frstCmd;
    for (int i=0; i<node->data->cmds->len; i++) {
        append_cmd_to_cmdlist(cmdList, currCmd);
        currCmd = next_cmd(currCmd);
    }
}

/**
 * This funcion visits a node and implements the postorder transversal
 * it also adds commands to a list if it is appropriate
 */
void visitNode(DAG_map * map, BuildSpecNode * node, CommandList * cmdList) {
    //printf("Visit node %s\n", node->data->target);
//    if n has a permanent mark then return
    if (node->permMark == 1) {
        return;
    }
//    if n has a temporary mark then stop   (not a DAG)
    if (node->tempMark == 1) {
        fprintf(stderr, "Error: This makefile contains a cycle!\n");
        exit(-1);
    }
//    mark n temporarily
    node->tempMark = 1;
//    for each node m with an edge from n to m do
//        visit(m)
    for (int i=0; i<node->data->depsLen; i++) {
        BuildSpecNode * temp = lookup(map, node->data->deps[i]);
        visitNode(map, temp, cmdList);
        if (temp->isDummy == 1) {
            freeNode(temp);
        }
    }
//    mark n permanently
    node->permMark = 1;
//    add n to head of L
    //Command** cmds = node->data->cmds;
    if (-1 == shouldExec(map, node)) {
        return;
    }
    add_cmd_to_list(node, cmdList);
    return;
}
int getCommandList(DAG_map * map, CommandList * cmdList, BuildSpecList * list) {
    init_cmd_list(cmdList);
    initHashMap(map, list);
    free(list);
    // create a list of commands
    visitNode(map, lookup(map, map->root), cmdList);
    freeHashMap(map);
    return 0;
}

