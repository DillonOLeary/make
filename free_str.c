#include "free_str.h"
#include "parser.h"
#include "hash_map.h"
#include <stdlib.h>

int freeCommandList(CommandList * list) {
    
}

int freeBuildSpec(BuildSpec * spec) {
    if (spec == NULL)
        return;


}

int freeNode(BuildSpecNode * node) {
    if (node != NULL) {
        freeBuildSpec(node->data);
        free(node);
    }
}

int freeHashMap(DAG_map * map) {
    for(int i=0; i<map->size;i++) {
        freeNode(map->map[i]);
    }
    free(map);
}
