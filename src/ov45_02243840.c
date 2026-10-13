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
extern void *ov45_02244170(Tree *, void *, void **, u8 *, u8 *);
extern int LobbyString_Compare(void *, void *);
void *ov45_02243970(Tree *, void *, u8, u8, void *);
extern void ov45_02236E58(Iterator *);
extern void *ov45_022370D0(void *, void *);
extern void ov45_02243F1C(void *);
extern void LobbyString_Destroy(void *);
extern void DWC_Free(int, void *, int);

static inline int isLess(void *a, void *b) {
    return LobbyString_Compare(a, b) < 0;
}

void ov45_02243840(Result *, Tree *, void *);
void ov45_022438F0(Iterator *, Tree *, Iterator);

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

extern const char ov45_022553AC[];
extern int printf(const char *, ...);
extern void abort(void);
extern void *LobbyMem_Alloc(int);
extern void LobbyString_InitCopy(Payload *, Payload *);
extern void ov45_02243BC0(void *, void *);
extern void ov45_02236EC4(void *, void *);

void ov45_02243840(Result *out, Tree *t, void *key) {
    u8 left, first;
    void *previous;
    void *parent = ov45_02244170(t, key, &previous, &left, &first);
    if (previous == 0 || isLess((u8 *)previous + 12, key)) {
        out->node = ov45_02243970(t, parent, left, first, key);
        out->flag = ov45_02255358[4];
    } else {
        out->node = previous;
        out->flag = ov45_02255358[5];
    }
}

void ov45_022438F0(Iterator *out, Tree *t, Iterator node) {
    void *saved = node.current;
    if (saved == t->first) {
        ov45_02236E58(&node);
        t->first = node.current;
    }
    out->current = ov45_022370D0(saved, t->anchor);
    ov45_02243F1C((u8 *)saved + 28);
    LobbyString_Destroy((u8 *)saved + 12);
    if (saved) {
        DWC_Free(0, saved, 0);
    }
    t->count--;
}
