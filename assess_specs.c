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
        exit(-1);
    }
    printf("File: %s Last mod: %s\n", filename, ctime(&buf.st_mtime));
    return buf.st_mtime;
}

/**
 * Return 1 if the target is older then the deps
 * TODO should also return true if the target needs
 * to be recompiled, as in it doesn't exist
 */
int targetOlderThanDeps(char* target, char** deps, int depsLen) {
    // FIXME this isn't right!! it can have no dependencies and not be a file! i can have
    // no commands and still not be a file! I need some sort of flag...
    if (depsLen == 0) {
        printf("%s is a file!!\n", target);
        // FIXME return 0 or 1??
        return 0;
    }
    for (int i=0; i<depsLen; i++) {
        if (getLastMod(target) > getLastMod(deps[i]))
            return 1;
    }
    return 0;
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
    if (targetOlderThanDeps(node->data->target, node->data->deps,
                node->data->depsLen)) {
        return;
    }
    Command* currCmd = node->data->cmds->frstCmd;
    for (int i=0; i<node->data->cmds->len; i++) {
        append_cmd_to_cmdlist(cmdList, currCmd);
        currCmd = next_cmd(currCmd);
    }

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

