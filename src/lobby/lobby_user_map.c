#include "global.h"

#include "dwc/net_common_internal.h"
#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern int printf(const char *, ...);
extern void abort(void);

extern u8 ov45_02255358[];
extern char sStr_TreeInsertLengthError_4[];
extern void *LobbyMem_Alloc(u32);
extern void LobbyTree_InsertRebalance(void *, void *);
extern void *LobbyTree_EraseRebalance(void *, void *);
extern void LobbyString_Destroy(void *);
extern void LobbyString_InitCopy(void *, void *);

LobbyUserIdSetNode *LobbyUserIdSet_InsertNode(LobbyUserIdSet *set, LobbyUserIdSetNode *parent, int left, int last, const int *key) {
    struct {
        u8 byte;
        u8 pad[3];
        LobbyUserIdSetNode *a, *b;
        void *owner;
        LobbyUserIdSetNode *volatile cleanup;
    } f;

    LobbyUserIdSetNode *n;
    int *userId;
    u32 *link;
    if (set->count == -1) {
        printf(sStr_TreeInsertLengthError_4);
        abort();
    }
    f.byte = 0;
    n = LobbyMem_Alloc(16);
    f.owner = &set->root;
    userId = &n->userId;
    f.cleanup = n;
    f.a = n;
    if (userId) {
        *userId = *key;
    }
    n->right = 0;
    f.cleanup = 0;
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
    set->count++;
    f.b = n;
    LobbyTree_InsertRebalance(n, set->root);
    if (last) {
        set->first = n;
    }
    {
        LobbyUserIdSetNode *cleanup = f.cleanup;
        if (cleanup) {
            DWC_Free(0, cleanup, 0);
        }
    };
    return n;
}

void *LobbyUserIdSet_Destroy(LobbyUserIdSet *set) {
    if (set->root) {
        LobbyUserIdSet_DestroyNodes(set, set->root);
    }
    return set;
}

void LobbyUserMap_Insert(LobbyUserMapInsertResult *out, LobbyUserMap *map, LobbyUserMapEntry *entry) {
    LobbyUserMapNode *prev;
    u8 left, first;
    LobbyUserMapNode *last;
    last = LobbyUserMap_FindInsertPos(map, &entry->userId, &prev, &left, &first);
    if (!prev || prev->entry.userId < entry->userId) {
        out->node = LobbyUserMap_InsertNode(map, last, left, first, entry);
        out->inserted = ov45_02255358[0];
    } else {
        out->node = prev;
        out->inserted = ov45_02255358[1];
    }
}

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
        u8 flag;
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
