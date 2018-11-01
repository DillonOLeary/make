#include "assess_specs.h"
#include "hash_map.h"

void visitNode(BuildSpecNode * node, CommandList * cmdList) {
//    if n has a permanent mark then return
//    if n has a temporary mark then stop   (not a DAG)
//    mark n temporarily
//    for each node m with an edge from n to m do
//        visit(m)
//    mark n permanently
//    add n to head of L
}

int getCommandList(CommandList * cmdList, BuildSpecList * list) {
    DAG_map map;
    initHashMap(&map, list);
    // create a list of commands
    visitNode(lookup(&map, map.root), cmdList);
    return 0;
}

