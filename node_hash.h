#ifndef __node__hash_h__
#define __node__hash_h__

typedef struct {
    // for tranvesal whether this node is potentially a cycle
    // initialized to 0
    int tempMark;
    // this node is not a cycle, initalized to 0
    int permMark;
    BuildSpec * data;
    Node ** children;
} BuildSpecNode;



#endif
