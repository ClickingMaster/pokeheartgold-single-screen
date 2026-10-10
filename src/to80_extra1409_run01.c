#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Node {
    struct Node *prev, *next;
    void *key, *value;
    int z1, z2;
} Node;

extern int ov38_02224CD4(void *, void *);
extern Node *(*ov38_02226218)(void *, int);
extern int ov38_02226214;

static inline void insert(Node **list, Node *n) {
    if (*list) {
        n->prev = (*list)->prev;
        n->next = *list;
        (*list)->prev->next = n;
        (*list)->prev = n;
    } else {
        n->next = n;
        n->prev = n;
        *list = n;
    }
}

int ov38_02221B48(Node **, void *, void *, void *);

int ov38_02221B48(Node **list, void *key, void *value, void *alloc) {
    Node *first = *list;
    int found = 0;
    Node *p;
    if (first) {
        p = first;
        if (ov38_02224CD4(key, first->key) != 0) {
            for (p = first->next; p != *list; p = p->next) {
                if (ov38_02224CD4(key, p->key) == 0) {
                    found = 1;
                    break;
                }
            }
        } else {
            found = 1;
        }
    }
    if (found) {
        p->value = value;
    } else {
        Node *n = ov38_02226218(alloc, 4);
        if (!n) {
            ov38_02226214 = 1;
            return 0;
        }
        n->key = key;
        n->value = value;
        n->z1 = 0;
        n->z2 = 0;
        insert(list, n);
    }
    return 1;
}
