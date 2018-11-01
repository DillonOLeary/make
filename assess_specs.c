#include <stdlib.h>
#include "assess_specs.h"
#include "hash_map.h"

//FIXME should i use const?
#define INIT_SIZE 10

// FIXME THIS CODE IS NOT MY OWN!!! THIS IS FROM
// https://stackoverflow.com/questions/3536153/c-dynamically-growing-array

void initCmdList(CommandList *a, size_t initialSize) {
  a->list = (Command**)malloc(initialSize * sizeof(Command*));
  a->used = 0;
  a->size = initialSize;
}

void insertCmdList(CommandList *a, Command * element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size
  if (a->used == a->size) {
    a->size *= 2;
    a->list = (Command**)realloc(a->list, a->size * sizeof(Command*));
  }
  a->list[a->used++] = element;
}
// FIXME THIS CODE ABOVE IS NOT MY CODE IT IS FROM 
// https://stackoverflow.com/questions/3536153/c-dynamically-growing-array

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
    Command** cmds = node->data->cmds;
    for (int i=0; cmds[i] != 0; i++) {
        insertCmdList(cmdList, cmds[i]);
    }

}
int getCommandList(CommandList * cmdList, BuildSpecList * list) {
    initCmdList(cmdList, INIT_SIZE);
    DAG_map map;
    initHashMap(&map, list);
    // create a list of commands
    visitNode(&map, lookup(&map, map.root), cmdList);

    return 0;
}

