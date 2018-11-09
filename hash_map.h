/** This module is used by the assess_specs
 * module to speed up the parsing through the
 * tree. It assumes that the map will remain at
 * a fixed length, set at the beginning, and 
 * can only insert and lookup items.
 */
#ifndef __hash__map_h__
#define __hash__map_h__
#include "parser.h"
#define LOAD_FACTOR .5
// each node in the map
typedef struct BuildSpecNode {
    // for tranvesal whether this node is potentially a cycle
    // initialized to 0
    int tempMark;
    // this node is not a cycle, initalized to 0
    int permMark;
    BuildSpec * data;
    char ** children;
    int hasExec;
} BuildSpecNode;

// Directed Acyclic Graph mapped with a
// hash map
typedef struct {
    BuildSpecNode** map;
    int size;
    char* root;
} DAG_map;

void createMap(DAG_map * d_map, BuildSpecList * specs);

void populateMap(DAG_map * map, BuildSpecList* specs);

/**
 * This creates the hash map.
 */
void initHashMap(DAG_map * map, BuildSpecList * specs);

/**
 * This looks up an item in the map. Returns null if the item 
 * doesn't exist
 */
BuildSpecNode * lookup(DAG_map * map, char* target); 

#endif
