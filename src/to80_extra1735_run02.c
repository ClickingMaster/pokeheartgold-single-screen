#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int printf(const char *, ...);
extern void abort(void);
#pragma force_active on

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 value;
} Node;

typedef struct {
    int count;
    void *alloc;
    void *last;
} Tree;

extern const char ov45_022553AC[];
extern Node *ov45_02242394(u32);
extern void ov45_02236EC4(Node *, void *);
extern void ov00_021EC2EC(int, void *, int);
Node *ov45_02243E44(Tree *, Node *, int, int, const u32 *);

Node *ov45_02243E44(Tree *t, Node *parent, int left, int last, const u32 *value) {
    struct {
        volatile u8 byte;
        u8 pad[3];
        Node *volatile a, *volatile b;
        void *volatile owner;
        Node *volatile cleanup;
    } f;

    Node *n;
    u32 address;
    if (t->count == -1) {
        printf(ov45_022553AC);
        abort();
    }
    f.byte = 0;
    n = ov45_02242394(16);
    f.owner = &t->alloc;
    address = (u32)n + 12;
    f.cleanup = n;
    f.a = n;
    if (address) {
        *(u32 *)address = *value;
    }
    n->right = 0;
    f.cleanup = 0;
    n->left = 0;
    address = (u32)n + 8;
    if (address) {
        *(u32 *)address = (u32)parent;
    }
    if (left) {
        *(void **)parent = n;
    } else {
        *(void **)((u8 *)parent + 4) = n;
    }
    t->count++;
    f.b = n;
    ov45_02236EC4(n, t->alloc);
    if (last) {
        t->last = n;
    }
    {
        Node *cleanup = f.cleanup;
        if (cleanup) {
            ov00_021EC2EC(0, cleanup, 0);
        }
    };
    return n;
}
