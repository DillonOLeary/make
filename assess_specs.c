#include <stdlib.h>
#include "assess_specs.h"
#include "hash_map.h"
#include "list_utils.h"
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
    printf("File: %s Last mod: %s\n", filename, ctime(&buf.st_mtime));
    return buf.st_mtime;
}

/**
 * Return 1 if the target is older then the deps
 * TODO should also return true if the target needs
 * to be recompiled, as in it doesn't exist
 *
int targetOlderThanDeps(char* target, char** deps, int depsLen) {
    // FIXME this isn't right!! it can have no dependencies and not be a file! i can have
    // no commands and still not be a file! I need some sort of flag...
    if (depsLen == 0) {
        printf("%s is a file!!\n", target);
        // FIXME return 0 or 1??
        return 0;
    }
    for (int i=0; i<depsLen; i++) {
        //if (getLastMod(target) > getLastMod(deps[i]))
            return 0;
    }
    return 0;
}
*/

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
        if (getLastMod(node->data->target) > getLastMod(node->data->deps[i])) {
            printf("The target file %s was modified before the dep file %s !\n", node->data->target, node->data->deps[i]);
            return 1;
        }
    }
    printf("None of the deps have been modified since the target %s was modified. Deps len %d\n", node->data->target, node->data->depsLen);
    return -1;
}

/**
 * Return 1 for yes, -1 for no
 * Execute if target is not file.
 * If it is check deps.
 */
int shouldExec(DAG_map* map, BuildSpecNode * node) {
    // check if it's a file
    if (NULL != fopen(node->data->target, "r")) {
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
    printf("Visit node %s\n", node->data->target);
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
        visitNode(map, lookup(map, node->data->deps[i]), cmdList);
    }
//    mark n permanently
    node->permMark = 1;
//    add n to head of L
    //Command** cmds = node->data->cmds;
    // FIXME should we not run this if it is the root node?
    if (-1 == shouldExec(map, node)) {
        return;
    }
    add_cmd_to_list(node, cmdList);
    return;
    //if (targetOlderThanDeps(node->data->target, node->data->deps,
    //            node->data->depsLen)) {
    //    return;
    //}
}
int getCommandList(CommandList * cmdList, BuildSpecList * list) {
    init_cmd_list(cmdList);
    DAG_map map;
    initHashMap(&map, list);
    // create a list of commands
    visitNode(&map, lookup(&map, map.root), cmdList);
    Command* currCmd = cmdList->frstCmd;
    for (int i=0; i<cmdList->len; i++) {
        for (int j=0; j < currCmd->argc; j++) {
            printf("%s", currCmd->argv[j]);
        }
        currCmd = currCmd->nxtCmd;
        printf("\nThe %d command above\n", i); 
    }
    return 0;
}

