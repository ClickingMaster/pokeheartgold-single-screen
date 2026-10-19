#include "global.h"

#include "dwc/net_common_internal.h"

#include "lobby_tree.h"

extern int printf(const char *, ...);
extern void abort(void);

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern void LobbyString_Destroy(void *);
extern LobbyUserMapNode *LobbyTree_EraseRebalance(LobbyUserMapNode *, void *);
extern void *LobbyMem_Alloc(int);
extern void LobbyString_InitCopy(void *, void *);
extern void LobbyTree_InsertRebalance(LobbyUserMapNode *, void *);
extern char sStr_TreeInsertLengthError_4[];

void LobbyUserMap_Erase(Iterator *out, LobbyUserMap *map, Iterator node) {
    LobbyUserMapNode *old = node.current;
    if (old == map->first) {
        LobbyTreeIterator_Advance(&node);
        map->first = node.current;
    }
    out->current = LobbyTree_EraseRebalance(old, map->root);
    LobbyString_Destroy(&old->entry.nick);
    if (old) {
        DWC_Free(0, old, 0);
    }
    map->count--;
}

LobbyUserMapNode *LobbyUserMap_InsertNode(LobbyUserMap *map, LobbyUserMapNode *parent, int left, int first, LobbyUserMapEntry *entry) {
    struct {
        volatile u8 flag;
        LobbyUserMapNode *a, *b;
        LobbyUserMapNode **root;
        LobbyUserMapNode *volatile owned;
    } guard;

    LobbyUserMapNode *n, *owned;
    LobbyUserMapEntry *dest;
    u32 *link;
    if (map->count == -1) {
        printf(sStr_TreeInsertLengthError_4);
        abort();
    }
    guard.flag = 0;
    n = LobbyMem_Alloc(28);
    guard.a = n;
    guard.b = n;
    guard.root = &map->root;
    guard.owned = n;
    dest = &n->entry;
    if (dest) {
        dest->userId = entry->userId;
        LobbyString_InitCopy(&dest->nick, &entry->nick);
    }
    n = guard.owned;
    n->right = 0;
    guard.owned = 0;
    n->left = 0;
    link = &n->parent;
    if (link) {
        *link = (u32)parent;
    }
    if (left) {
        parent->left = n;
    } else {
        parent->right = n;
    }
    map->count++;
    LobbyTree_InsertRebalance(n, map->root);
    if (first) {
        map->first = n;
    }
    owned = guard.owned;
    if (owned) {
        DWC_Free(0, owned, 0);
    }
    return n;
}
