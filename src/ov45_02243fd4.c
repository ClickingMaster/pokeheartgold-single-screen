#pragma force_active on
#include "global.h"
extern int printf(const char *, ...);
extern void abort(void);

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "../lib/NitroDWC/src/tree-iterator4.h"
typedef struct Node Node;

typedef struct {
    int count;
    void *head;
    u32 unused;
    void *current;
} List;

struct Node {
    void *next, *prev;
    u32 link;
    u32 value;
    u8 str[12];
};

extern void ov45_02236E58(Iterator *);
extern void ov45_02237BC8(void *);
extern void ov00_021EC2EC(int, void *, int);
extern Node *ov45_022370D0(Node *, void *);
void ov45_02243FD4(Iterator *, List *, Iterator);
extern void *ov45_02242394(int);
extern void ov45_02237A24(void *, void *);
extern void ov45_02236EC4(Node *, void *);
extern char ov45_022553AC[];
Node *ov45_0224404C(List *, Node *, int, int, u32 *);

void ov45_02243FD4(Iterator *out, List *list, Iterator node) {
    Node *old = node.current;
    if (old == list->current) {
        ov45_02236E58(&node);
        list->current = node.current;
    }
    out->current = ov45_022370D0(old, list->head);
    ov45_02237BC8(old->str);
    if (old) {
        ov00_021EC2EC(0, old, 0);
    }
    list->count--;
}

Node *ov45_0224404C(List *list, Node *link, int before, int current, u32 *value) {
    struct {
        volatile u8 flag;
        Node *a, *b;
        void **head;
        Node *volatile owned;
    } guard;

    Node *n, *owned;

    struct {
        u32 data;
        u8 str[12];
    } *dest;

    u32 *lp;
    if (list->count == -1) {
        printf(ov45_022553AC);
        abort();
    }
    guard.flag = 0;
    n = ov45_02242394(28);
    guard.a = n;
    guard.b = n;
    guard.head = &list->head;
    guard.owned = n;
    dest = (void *)((u8 *)n + 12);
    if (dest) {
        dest->data = *value;
        ov45_02237A24(dest->str, value + 1);
    }
    n = guard.owned;
    n->prev = 0;
    guard.owned = 0;
    n->next = 0;
    lp = (u32 *)((u8 *)n + 8);
    if (lp) {
        *lp = (u32)link;
    }
    if (before) {
        link->next = n;
    } else {
        link->prev = n;
    }
    list->count++;
    ov45_02236EC4(n, list->head);
    if (current) {
        list->current = n;
    }
    owned = guard.owned;
    if (owned) {
        ov00_021EC2EC(0, owned, 0);
    }
    return n;
}
