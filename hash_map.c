#include "hash_map.h"
#include "parser.h"
#include "list_utils.h"
#include <string.h>
#include <stdlib.h>

void createMap(DAG_map * d_map, BuildSpecList * specs) {
    int arr_size = (int)(specs->len / LOAD_FACTOR);
    if (NULL == (d_map->map = calloc(arr_size, sizeof(BuildSpecNode*)))) {
        fprintf(stderr, "Memory allocation error");
        exit(-1);
    }
    //d_map->size = size;
    d_map->size = arr_size;
    // FIXME assert the first element is the root
    d_map->root = specs->root;
}

// FIXME THIS IS NOT MY CODE! its from http://www.cse.yorku.ca/~oz/hash.html
// My TA said I could use it
unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int getIndex(char * str, int size) {
    //printf("Hash #: %ld, with mod %d\n", hash( (unsigned char*) str), (int)(hash( (unsigned char*) str) % size));
    // FIXME is this the correct casting?
    return hash( (unsigned char*) str) % size;
}
void insertNode(DAG_map * map, BuildSpecNode * node) {
    int hashIndex = getIndex(node->data->target, map->size);
    // FIXME this assumes that spots that arent' used are null
    // FIXME this also assumes that we will never run out of space
    while (map->map[hashIndex] != NULL) {
        if (strcmp(node->data->target, map->map[hashIndex]->data->target) == 0) {
            fprintf(stderr, "Error, same target twice\n");
            exit(-1);
        }
        hashIndex = (hashIndex + 1) % map->size;
    }
    map->map[hashIndex] = node;
}

void populateMap(DAG_map * map, BuildSpecList* specs) {
    BuildSpec* currSpec = specs->frstBS;
    for(int i=0; i<specs->len; i++) {
        BuildSpecNode * node = malloc(sizeof(BuildSpecNode));
        node->data = currSpec;
        node->tempMark = node->permMark = 0;
        node->children = currSpec->deps;
        node->hasExec = 0;
        insertNode(map, node);
        currSpec = next_BS(currSpec);
    }
}

void initHashMap(DAG_map * map, BuildSpecList* specs) {
    createMap(map, specs);
    populateMap(map, specs);
}

BuildSpecNode * createDummyNode(char* target) {
    BuildSpecNode* node = calloc(1, sizeof(BuildSpecNode));
    BuildSpec* spec = calloc(1, sizeof(BuildSpec));
    spec->cmds = calloc(1, sizeof(CommandList));
    spec->target = target;
    spec->depsLen = 0;
    spec->cmds->len = 0;
    node->data = spec;
    node->hasExec = 0;
    return node;
}

BuildSpecNode * lookup(DAG_map * map, char* target) {
    int hashIndex = getIndex(target, map->size);
    // FIXME this assumes that spots that arent' used are null
    // FIXME this also assumes that we will never run out of space
    if (map->map[hashIndex] == NULL) {
        fprintf(stdout, "This target %s is not in the map! Assume it is a file\n", target);
        return createDummyNode(target);
    }
    while (strcmp(target, map->map[hashIndex]->data->target) != 0) {
        hashIndex = (hashIndex + 1) % map->size;
        if (map->map[hashIndex] == NULL) {
            // TODO if an element is searched for that doesn't
            // exit, instead of throwing an error it might be a
            // file that isn't a build spec. In that case I need to 
            // return a dummy node if it exists, if not throw an
            // error. Maybe I don't even need to check if it exists?
            // That might be handled elsewhere. Couldn't hurt though.
            // Also the dummy node probably needs some flag, but
            // also it might not because it neither has dependencies nor
            // commands
            fprintf(stdout, "This target %s is not in the map! Assume it is a file\n", target);
            return createDummyNode(target);
        }
    }
    return map->map[hashIndex];
}
