#include "hash_map.h"
#include "parser.h"
#include <stdlib.h>

void createMap(DAG_map * d_map, int size, BuildSpecList * specs) {
    if (NULL == (d_map->map = malloc(sizeof(BuildSpecNode*) * (int)((double)size / LOAD_FACTOR)))) {
        fprintf(stderr, "Memory allocation error");
        exit(-1);
    }
    d_map->size = size;
    // FIXME assert the first element is the root
    d_map->root = specs->list[0]->target;
}

void populateMap(DAG_map * map, BuildSpecList* specs) {
    for(int i=0; i<specs->len; i++) {
        map->map[i]->data = specs->list[i];
    }
}

void initHashMap(DAG_map * map, int size, BuildSpecList* specs) {
    createMap(map, size, specs);
    populateMap(map, specs);
}

BuildSpecNode * lookup(DAG_map * map, char* target) {
    return NULL;
}
