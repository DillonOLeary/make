#include "hash_map.h"
#include "parser.h"
#include "test_create_specs.h"
#include <stdlib.h>

void printMap(DAG_map * map) {
    for (int i = 0; i < map->size; i++) {
        printf("loop %d\n", i );
        if (map->map[i] == NULL)
            continue;
        printf("Element at %d: %s, with dependeny 0: %s, and cmd: %s\n", i, map->map[i]->data->target, "NI","NI"); 
    }
    printf("Using lookup:\n");
    lookup(map, "TESTe0");
    lookup(map, "TESTd1");
    lookup(map, "TESTl2");
    lookup(map, "TESTs3");
    lookup(map, "TESTb4");
    lookup(map, "TESTe0");
}

int main() {
    DAG_map * map;
    map = malloc(sizeof(map));
    BuildSpecList specs;
    createSpecList(&specs);
    createMap(map, &specs); 
    populateMap(map, &specs);
    printMap(map);
}
