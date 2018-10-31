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
}

int main() {
    DAG_map * map;
    map = malloc(sizeof(map));
    Command * cmd = NULL;
    char** deps = NULL;
    int depsLen = 0;
    BuildSpecList specs;
    specs.len = 5;
    specs.list = malloc(sizeof(BuildSpec*)*5);
    specs.list[0] = createSpec(cmd, "TEST0", deps, depsLen);
    specs.list[1] = createSpec(cmd, "TESTe1", deps, depsLen);
    specs.list[2] = createSpec(cmd, "TEST2", deps, depsLen);
    specs.list[3] = createSpec(cmd, "TEST3", deps, depsLen);
    specs.list[4] = createSpec(cmd, "TEST4", deps, depsLen);
    createMap(map, specs.len, &specs); 
    populateMap(map, &specs);
    printMap(map);
}
