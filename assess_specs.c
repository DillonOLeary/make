#include <stdlib.h>
#include "assess_specs.h"
#include "hash_map.h"

// TODO I need a way to add to the command list without knowing
// how many times, but I know I wont remove
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
}

int getCommandList(CommandList * cmdList, BuildSpecList * list) {
    DAG_map map;
    initHashMap(&map, list);
    // create a list of commands
    visitNode(&map, lookup(&map, map.root), cmdList);
    return 0;
}

