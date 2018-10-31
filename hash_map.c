#include "hash_map.h"
#include "parser.h"
#include <stdlib.h>

void createMap(DAG_map * d_map, int size, BuildSpecList * specs) {
    int arr_size = (int)(size / LOAD_FACTOR);
    if (NULL == (d_map->map = malloc(sizeof(BuildSpecNode*) * arr_size))) {
        fprintf(stderr, "Memory allocation error");
        exit(-1);
    }
    //d_map->size = size;
    d_map->size = arr_size;
    // FIXME assert the first element is the root
    d_map->root = specs->list[0]->target;
}

// FIXME can I use this code?? its from http://www.cse.yorku.ca/~oz/hash.html
unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int getIndex(char * str, int size) {
    // FIXME is this the correct casting?
    return hash( (unsigned char*) str) % size;
}

void populateMap(DAG_map * map, BuildSpecList* specs) {
    for(int i=0; i<specs->len; i++) {
        BuildSpecNode * node = malloc(sizeof(BuildSpecNode));
        node->data = specs->list[i];
        node->tempMark = node->permMark = 0;
        node->children = specs->list[i]->deps;
        //map->map[i] = node;
        map->map[getIndex(specs->list[i]->target, map->size)] = node;
    }
}

void initHashMap(DAG_map * map, int size, BuildSpecList* specs) {
    createMap(map, size, specs);
    populateMap(map, specs);
}

BuildSpecNode * lookup(DAG_map * map, char* target) {
    return NULL;
}
