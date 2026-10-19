#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_request.h"

typedef struct {
    u8 active;
    LobbyRequestMapNode *allocated, *value;
    LobbyRequestMapNode **owner;
    LobbyRequestMapNode *pending;
} Guard;

extern int printf(const char *, ...);
extern void abort(void);
extern const char sStr_TreeInsertLengthError_3[];
extern void *LobbyMem_Alloc(u32);
extern void LobbyTree_InsertRebalance(LobbyRequestMapNode *, void *);
LobbyRequestMapNode *LobbyRequestMap_InsertNode(LobbyRequestMap *map, LobbyRequestMapNode *parent, int left, int leftmost, LobbyRequest *const *request) {
    LobbyRequestMapNode *node, *pending;
    LobbyRequest **dest;
    LobbyRequestMapNode **parentField;
    volatile Guard guard;
    if (map->count == -1) {
        printf(sStr_TreeInsertLengthError_3);
        abort();
    }
    guard.active = 0;
    node = LobbyMem_Alloc(sizeof(LobbyRequestMapNode));
    guard.owner = &map->root;
    dest = &node->request;
    guard.pending = node;
    guard.allocated = node;
    if (dest) {
        *dest = *request;
    }
    node->right = NULL;
    guard.pending = NULL;
    node->left = NULL;
    parentField = &node->parent;
    if (parentField) {
        *parentField = parent;
    }
    if (left) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    map->count++;
    guard.value = node;
    LobbyTree_InsertRebalance(node, map->root);
    if (leftmost) {
        map->leftmost = node;
    }
    pending = guard.pending;
    if (pending) {
        DWC_Free(0, pending, 0);
    }
    return node;
}
