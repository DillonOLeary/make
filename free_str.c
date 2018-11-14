#include "free_str.h"
#include "parser.h"
#include "hash_map.h"
#include <stdlib.h>

/**
 * This will free all the commands in the
 * list
 */
void freeCommandList(CommandList * list) {
    if (list != NULL) {
        if (list->frstCmd != NULL) {
            Command * curr = list->frstCmd;
            for (int i=0; i<list->len; i++) {
                free(curr->argv);
                curr = curr->nxtCmd;
            }
            //if (curr != NULL) {
           //     fprintf(stderr, "the next command is not null! There is AN ISSUE WITH THE COMMANDS, MAYBE THE LENGTH\n");
           //     exit(-1);
            //}
        }
        free(list);
    }
}

void freeBuildSpec(BuildSpec * spec) {
    if (spec != NULL) {
        freeCommandList(spec->cmds);
        free(spec->deps);
        free(spec);
    }
}

void freeNode(BuildSpecNode * node) {
    if (node != NULL) {
        freeBuildSpec(node->data);
        free(node);
    }
}

void freeHashMap(DAG_map * map) {
    if (map == NULL) {
        fprintf(stderr, "Map is null, ending program...\n");
    }
    int i;
    for(i=0; i<map->size;i++) {
        freeNode(map->map[i]);
    }
}
