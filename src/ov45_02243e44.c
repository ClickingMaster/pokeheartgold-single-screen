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

extern const char sStr_TreeInsertLengthError_4[];
extern Node *LobbyMem_Alloc(u32);
extern void LobbyTree_InsertRebalance(Node *, void *);
extern void DWC_Free(int, void *, int);
Node *LobbyUserIdSet_InsertNode(Tree *, Node *, int, int, const u32 *);

Node *LobbyUserIdSet_InsertNode(Tree *t, Node *parent, int left, int last, const u32 *value) {
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
        printf(sStr_TreeInsertLengthError_4);
        abort();
    }
    f.byte = 0;
    n = LobbyMem_Alloc(16);
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
    LobbyTree_InsertRebalance(n, t->alloc);
    if (last) {
        t->last = n;
    }
    {
        Node *cleanup = f.cleanup;
        if (cleanup) {
            DWC_Free(0, cleanup, 0);
        }
    };
    return n;
}
