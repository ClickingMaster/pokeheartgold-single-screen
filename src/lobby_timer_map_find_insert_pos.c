#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Node {
    void *left, *right;
    u32 parent, key;
} Node;

typedef struct {
    u32 pad;
    void *root;
} Tree;

Node *LobbyTimerMap_FindInsertPos(Tree *, const u32 *, Node **, u8 *, u8 *);
#pragma force_active on

Node *LobbyTimerMap_FindInsertPos(Tree *t, const u32 *key, Node **last, u8 *left, u8 *first) {
    Node *n;
    Node *parent = (Node *)&t->root;
    *last = NULL;
    n = t->root;
    *left = 1;
    *first = 1;
    while (n) {
        parent = n;
        if (*key < n->key) {
            n = n->left;
            *left = 1;
        } else {
            *last = n;
            n = n->right;
            *left = 0;
            *first = 0;
        }
    }
    return parent;
}
