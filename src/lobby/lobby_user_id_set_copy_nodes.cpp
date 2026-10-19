#include <nitro/types.h>

#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_mem.h"

inline void *operator new(unsigned long, void *p) {
    return p;
}

extern "C" void DWC_Free(int, void *, u32);

enum Color {
    black,
    red_
};

inline void SetParent(LobbyUserIdSetNode *node, LobbyUserIdSetNode *p) {
    node->parent = (u32)p | (node->parent & 1);
}

inline Color GetColor(const LobbyUserIdSetNode *node) {
    return (node->parent & 1) ? red_ : black;
}

inline void SetColor(LobbyUserIdSetNode *node, Color c) {
    if (c) {
        node->parent |= 1;
    } else {
        node->parent &= ~1;
    }
}

struct Alloc {
    LobbyUserIdSetNode *allocate() {
        return (LobbyUserIdSetNode *)LobbyMem_Alloc(sizeof(LobbyUserIdSetNode));
    }
};

struct Holder {
    LobbyUserIdSetNode *a, *b;
    void *alloc;
    LobbyUserIdSetNode *ptr;

    Holder(LobbyUserIdSetNode *p, void *al)
        : a(p)
        , b(p)
        , alloc(al)
        , ptr(p) {
    }

    LobbyUserIdSetNode *release() {
        LobbyUserIdSetNode *r = ptr;
        ptr = 0;
        return r;
    }
};

inline void Destroy(Holder *h) {
    if (h->ptr) {
        DWC_Free(0, h->ptr, 0);
    }
}

inline void Construct(int *p, const int &v) {
    new (p) int(v);
}

#pragma opt_scalarize off
#pragma opt_common_subs off

extern "C" void LobbyUserIdSet_CopyNodes(LobbyUserIdSet *set, LobbyUserIdSetNode *parent, LobbyUserIdSetNode *src, BOOL left) {
    Alloc a = Alloc();
    LobbyUserIdSetNode *p = a.allocate();
    Holder h(p, &set->root);
    p->right = 0;
    p->left = 0;
    SetParent(p, parent);
    if (left) {
        Construct(&h.ptr->userId, src->left->userId);
        set->first = h.ptr;
        parent->left = h.ptr;
        parent = h.release();
        src = src->left;
    } else {
        Construct(&h.ptr->userId, src->right->userId);
        parent->right = h.ptr;
        parent = h.release();
        src = src->right;
    }
    SetColor(parent, GetColor(src));
    ++set->count;
    if (src->right) {
        LobbyUserIdSet_CopyNodes(set, parent, src, FALSE);
    }
    if (src->left) {
        LobbyUserIdSet_CopyNodes(set, parent, src, TRUE);
    }
    Destroy(&h);
}
