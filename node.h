#ifndef NODE_H
#define NODE_H

#include "TimedSharedPtr.h"

struct Node {
    int val;
    TimedSharedPtr<Node> next;

    Node(int v) : val{v} {}  // Inline constructor
};

#endif // NODE_H
