#include "hash_map.h"
#include <stdlib.h>

void createMap(DAG_map * d_map, int size, BuildSpecList specs) {
    if (NULL == (d_map->map = malloc(sizeof(BuildSpecNode*) * (size / load_factor)))) {
        fprintf(stderr, "Memory allocation error");
        exit(-1);
    }
    d_map->size = size;
    // FIXME assert the first element is the root
    d_map->root = specs[0]->target;
}

void initHashMap(DAG_map * map, int size, BuildSpecList specs) {
    createMap(map, size, specs);
    populateMap(map, specs);
}

BuildSpecNode * lookup(DAG_map * map, char* target) {

}
