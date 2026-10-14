#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "../lib/NitroDWC/src/tree-iterator4.h"
#pragma force_active on

typedef struct {
    u32 count;
    void *anchor;
    u32 pad;
    void *first;
} Tree;

typedef struct {
    void *node;
    u8 flag;
} Result;

extern u8 ov45_02255358[];
extern void *LobbyChannelMap_FindInsertPos(Tree *, void *, void **, u8 *, u8 *);
extern int LobbyString_Compare(void *, void *);
void *LobbyChannelMap_InsertNode(Tree *, void *, u8, u8, void *);
extern void ov45_02236E58(Iterator *);
extern void *LobbyTree_EraseRebalance(void *, void *);
extern void LobbyUserIdSet_Destroy(void *);
extern void LobbyString_Destroy(void *);
extern void DWC_Free(int, void *, int);

static inline int isLess(void *a, void *b) {
    return LobbyString_Compare(a, b) < 0;
}

void LobbyChannelMap_Insert(Result *, Tree *, void *);
void LobbyChannelMap_Erase(Iterator *, Tree *, Iterator);

typedef struct {
    u32 prefix[3];
    void *owner;
    u32 start;
    void *head, *tail;
    u32 value;
    u32 data[6];
} Payload;

typedef struct {
    void *left, *right, *parent;
    Payload payload;
} Node;

typedef struct {
    u8 pad, flag;
    u16 pad2;
    void *a, *b, *anchor;
    void *volatile current;
} Guard;

static inline void initializeGuard(volatile Guard *g, Node *n, void *anchor) {
    g->a = n;
    g->b = n;
    g->anchor = anchor;
    g->current = n;
}

extern const char sStr_TreeInsertLengthError_4[];
extern int printf(const char *, ...);
extern void abort(void);
extern void *LobbyMem_Alloc(int);
extern void LobbyString_InitCopy(Payload *, Payload *);
extern void LobbyUserIdSet_Assign(void *, void *);
extern void LobbyTree_InsertRebalance(void *, void *);

void LobbyChannelMap_Insert(Result *out, Tree *t, void *key) {
    u8 left, first;
    void *previous;
    void *parent = LobbyChannelMap_FindInsertPos(t, key, &previous, &left, &first);
    if (previous == 0 || isLess((u8 *)previous + 12, key)) {
        out->node = LobbyChannelMap_InsertNode(t, parent, left, first, key);
        out->flag = ov45_02255358[4];
    } else {
        out->node = previous;
        out->flag = ov45_02255358[5];
    }
}

void LobbyChannelMap_Erase(Iterator *out, Tree *t, Iterator node) {
    void *saved = node.current;
    if (saved == t->first) {
        ov45_02236E58(&node);
        t->first = node.current;
    }
    out->current = LobbyTree_EraseRebalance(saved, t->anchor);
    LobbyUserIdSet_Destroy((u8 *)saved + 28);
    LobbyString_Destroy((u8 *)saved + 12);
    if (saved) {
        DWC_Free(0, saved, 0);
    }
    t->count--;
}
