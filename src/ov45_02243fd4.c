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
extern void LobbyString_Destroy(void *);
extern void DWC_Free(int, void *, int);
extern Node *LobbyTree_EraseRebalance(Node *, void *);
void LobbyUserMap_Erase(Iterator *, List *, Iterator);
extern void *LobbyMem_Alloc(int);
extern void LobbyString_InitCopy(void *, void *);
extern void LobbyTree_InsertRebalance(Node *, void *);
extern char ov45_022553AC[];
Node *LobbyUserMap_InsertNode(List *, Node *, int, int, u32 *);

void LobbyUserMap_Erase(Iterator *out, List *list, Iterator node) {
    Node *old = node.current;
    if (old == list->current) {
        ov45_02236E58(&node);
        list->current = node.current;
    }
    out->current = LobbyTree_EraseRebalance(old, list->head);
    LobbyString_Destroy(old->str);
    if (old) {
        DWC_Free(0, old, 0);
    }
    list->count--;
}

Node *LobbyUserMap_InsertNode(List *list, Node *link, int before, int current, u32 *value) {
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
    n = LobbyMem_Alloc(28);
    guard.a = n;
    guard.b = n;
    guard.head = &list->head;
    guard.owned = n;
    dest = (void *)((u8 *)n + 12);
    if (dest) {
        dest->data = *value;
        LobbyString_InitCopy(dest->str, value + 1);
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
    LobbyTree_InsertRebalance(n, list->head);
    if (current) {
        list->current = n;
    }
    owned = guard.owned;
    if (owned) {
        DWC_Free(0, owned, 0);
    }
    return n;
}
