#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "../lib/NitroDWC/src/tree-iterator4.h"

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 data;
} Node;

typedef struct {
    int count;
    void *root, *last;
} Tree;

extern void ov45_02236E58(Iterator *);
extern Node *ov45_022370D0(Node *, void *);
extern void ov00_021EC2EC(int, void *, int);
extern Node *ov45_02242394(u32);

typedef struct {
    Node *first, *second;
    Node **root;
    Node *volatile cleanup;
} Guard;

void ov45_02243CF8(Tree *, Node *, Node **, int);
void ov45_02243B50(Iterator *, Tree *, Iterator);

void ov45_02243B50(Iterator *out, Tree *tree, Iterator node) {
    Node *old = node.current;
    if (node.current == tree->last) {
        ov45_02236E58(&node);
        tree->last = node.current;
    }
    out->current = ov45_022370D0(old, tree->root);
    if (old) {
        ov00_021EC2EC(0, old, 0);
    }
    tree->count--;
}
