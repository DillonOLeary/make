#include "hash_map.h"
#include "parser.h"
#include <stdlib.h>

BuildSpec * createSpec(Command * cmd, char * target, char ** deps, int depsLen) {
    BuildSpec * spec;
    spec = malloc(sizeof(BuildSpec));
    spec->cmd = cmd;
    spec->target = target;
    spec->deps = deps;
    spec->depsLen = depsLen;
    return spec;
}

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
    Command * cmd = NULL;
    char** deps = NULL;
    int depsLen = 0;
    BuildSpecList specs;
    specs.len = 9;
    specs.list = malloc(sizeof(BuildSpec*)*specs.len);
    specs.list[0] = createSpec(cmd, "TESTe0", deps, depsLen);
    specs.list[1] = createSpec(cmd, "TESTd1", deps, depsLen);
    specs.list[2] = createSpec(cmd, "TESTl2", deps, depsLen);
    specs.list[3] = createSpec(cmd, "TESTs3", deps, depsLen);
    specs.list[4] = createSpec(cmd, "TESTb4", deps, depsLen);
    specs.list[5] = createSpec(cmd, "TESTw5", deps, depsLen);
    specs.list[6] = createSpec(cmd, "TESTy6", deps, depsLen);
    specs.list[7] = createSpec(cmd, "TESTq7", deps, depsLen);
    specs.list[8] = createSpec(cmd, "TESTp8", deps, depsLen);
    createMap(map, specs.len, &specs); 
    populateMap(map, &specs);
    printMap(map);
}
