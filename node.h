#ifndef NODE_H
#define NODE_H

#include "TimedSharedPtr.h"

struct Node {
    int val;
    TimedSharedPtr<Node> next;

    Node(int v) : val{v} {} 
};

#endif // NODE_H
