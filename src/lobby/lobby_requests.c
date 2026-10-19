#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

typedef struct {
    u8 active;
    LobbyRequestMapNode *allocated, *value;
    LobbyRequestMapNode **owner;
    LobbyRequestMapNode *pending;
} Guard;

extern int printf(const char *, ...);
extern void abort(void);
extern const char sStr_TreeInsertLengthError_3[];
extern void LobbyTree_InsertRebalance(LobbyRequestMapNode *, void *);

LobbyRequest *LobbyRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyGetChannelModeRequest_Destroy(void) {
}

LobbyRequest *LobbyGetChannelModeRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyGetChannelKeysRequest_Destroy(void) {
}

LobbyRequest *LobbyGetChannelKeysRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyEnterChannelRequest_Destroy(void) {
}

LobbyRequest *LobbyEnterChannelRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyEnumUsersRequest_Destroy(void) {
}

LobbyRequest *LobbyEnumUsersRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyEnumChannelsRequest_Destroy(void) {
}

LobbyRequest *LobbyEnumChannelsRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyRequestMap_Insert(LobbyRequestMapInsertResult *out, LobbyRequestMap *map, LobbyRequest **val) {
    LobbyRequestMapNode *pos;
    u8 insertLeft;
    u8 isLeftmost;
    LobbyRequestMapNode *parent = LobbyRequestMap_FindInsertPos(map, val, &pos, &insertLeft, &isLeftmost);
    if (pos == NULL || pos->request->id < (*val)->id) {
        out->node = LobbyRequestMap_InsertNode(map, parent, insertLeft, isLeftmost, val);
        out->inserted = sLobbyRequestMapInsertFlags[1];
        return;
    }
    out->node = pos;
    out->inserted = sLobbyRequestMapInsertFlags[2];
}

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
