#ifndef __FREE__STR_H__
#define __FREE__STR_H__
/**
 * Author: Dillon O'Leary
 * Ezra Boley
 * This module frees the different stucts
 * that use malloc
 */
#include "parser.h"
#include "hash_map.h"

/**
 * Free the build spec
 * 0 on success, -1 on failure
 */
void freeSpec(BuildSpec * spec);

/**
 * Free the node and the spec
 * 0 on success, -1 on failure
 */
void freeNode(BuildSpecNode * node);

/**
 * free the DAG_map
 */
void freeHashMap(DAG_map * map);

#endif
