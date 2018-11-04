#include <stdlib.h>
#include "assess_specs.h"
#include "hash_map.h"
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//FIXME should i use const?
#define INIT_SIZE 1

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

// FIXME I didn't copy, but i used this https://stackoverflow.com/questions/31633943/compare-two-times-in-c
// to help
time_t getLastMod(char* filename) {
    struct stat buf;
    if (-1 == stat(filename, &buf)) {
        fprintf(stderr, "Cannot open target stat file");
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
        return 1;
    }
    for (int i=0; i<depsLen; i++) {
        if (getLastMod(target) > getLastMod(deps[i]))
            return 1;
    }
    return 0;
}

// TODO I need a way to add to the command list without knowing
// how many times, but I know I wont remove
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
        // FIXME here lookup should return a fake node for that file
        // and chek if the file exists. that file has no deps or commands
        visitNode(map, lookup(map, node->data->deps[i]), cmdList);
    }
//    mark n permanently
    node->permMark = 1;
//    add n to head of L
    Command** cmds = node->data->cmds;
    if (targetOlderThanDeps(node->data->target, node->data->deps,
                node->data->depsLen)) {
        return;
    }
    // TODO fix the case where the node is a file, not a target
    for (int i=0; i<node->data->cmdsLen; i++) {
        insertCmdList(cmdList, cmds[i]);
    }

}
int getCommandList(CommandList * cmdList, BuildSpecList * list) {
    initCmdList(cmdList, INIT_SIZE);
    DAG_map map;
    initHashMap(&map, list);
    // create a list of commands
    visitNode(&map, lookup(&map, map.root), cmdList);
    for (unsigned int i=0; i<cmdList->used; i++) {
        for (unsigned int j=0; 0 != strcmp(cmdList->list[i]->args[j],"\0"); j++) {
            printf("%s", cmdList->list[i]->args[j]);
        }
        printf("\nThe %d command above\n", i); 
    }
    return 0;
}

