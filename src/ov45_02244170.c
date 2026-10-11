#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Node {
    void *left, *right;
    u32 parent;
    u8 data[4];
} Node;

typedef struct {
    int unused;
    void *root;
} Tree;

extern int ov45_02237F7C(void *, void *);
void *ov45_02244170(Tree *, void *, Node **, u8 *, u8 *);

void *ov45_02244170(Tree *w, void *key, Node **found, u8 *left, u8 *empty) {
    void *parent;
    Node *p;
    _Bool comparison;
    *found = 0;
    p = w->root;
    *left = 1;
    *empty = 1;
    parent = &w->root;
    while (p) {
        parent = p;
        comparison = (_Bool)(ov45_02237F7C(key, p->data) < 0);
        if (comparison) {
            p = p->left;
            *left = 1;
        } else {
            *found = p;
            p = p->right;
            *left = 0;
            *empty = 0;
        }
    }
    return parent;
}
