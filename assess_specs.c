#include "assess_specs.h"

void visitNode(BuildSpecNode * node, CommandList * cmdList)
//    if n has a permanent mark then return
//    if n has a temporary mark then stop   (not a DAG)
//    mark n temporarily
//    for each node m with an edge from n to m do
//        visit(m)
//    mark n permanently
//    add n to head of L

int getCommandList(CommandList * cmdList, BuildSpecNode * root) {
    Node * root;
    // FIXME should I create a list or execute commands as I go?
    visitNode(root, cmdList);
}

