#include "hash_map.h"
#include "parser.h"
#include <stdlib.h>

void createMap(DAG_map * d_map, int size, BuildSpecList * specs) {
    int arr_size = (int)(size / LOAD_FACTOR);
    if (NULL == (d_map->map = calloc(arr_size, sizeof(BuildSpecNode*)))) {
        fprintf(stderr, "Memory allocation error");
        exit(-1);
    }
    //d_map->size = size;
    d_map->size = arr_size;
    // FIXME assert the first element is the root
    d_map->root = specs->list[0]->target;
}

// FIXME THIS IS NOT MY CODE! its from http://www.cse.yorku.ca/~oz/hash.html
// My TA said I could use it
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
    printf("Hash #: %ld, with mod %d\n", hash( (unsigned char*) str), (int)(hash( (unsigned char*) str) % size));
    // FIXME is this the correct casting?
    return hash( (unsigned char*) str) % size;
}
void insertNode(DAG_map * map, BuildSpecNode * node) {
    int hashIndex = getIndex(node->data->target, map->size);
    // FIXME this assumes that spots that arent' used are null
    // FIXME this also assumes that we will never run out of space
    while (map->map[hashIndex] != NULL) {
        if (strcmp(node->data->target, map->map[hashIndex]) == 0) {
            fprintf(stderr, "Error, same target twice\n");
            exit(-1);
        }
        hashIndex = (hashIndex + 1) % map->size;
    }
    map->map[hashIndex] = node;
}

void populateMap(DAG_map * map, BuildSpecList* specs) {
    for(int i=0; i<specs->len; i++) {
        BuildSpecNode * node = malloc(sizeof(BuildSpecNode));
        node->data = specs->list[i];
        node->tempMark = node->permMark = 0;
        node->children = specs->list[i]->deps;
        insertNode(map, node);
    }
}

void initHashMap(DAG_map * map, int size, BuildSpecList* specs) {
    createMap(map, size, specs);
    populateMap(map, specs);
}

BuildSpecNode * lookup(DAG_map * map, char* target) {
    int hashIndex = getIndex(target, map->size);
    // FIXME this assumes that spots that arent' used are null
    // FIXME this also assumes that we will never run out of space
    while (strcmp(target, map->map[hashIndex]->data->target) != 0) {
        if (map->map[hashIndex] == NULL) {
            fprintf(stderr, "This target is not in the map!\n");
            exit(-1);
        }
        hashIndex = (hashIndex + 1) % map->size;
    }
    return map->map[hashIndex];
}
