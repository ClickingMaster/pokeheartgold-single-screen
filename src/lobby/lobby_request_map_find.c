#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

static LobbyRequestMapNode *LobbyRequestMap_LowerBound(LobbyRequestMap *map, LobbyRequest **key, LobbyRequestMapNode *node, LobbyRequestMapNode *best);

u32 LobbyRequest_GetParam(LobbyRequest *request) {
    return request->param;
}

void *LobbyU32Vector_Data(LobbyU32Vector *vector) {
    if (vector->size == 0) {
        return 0;
    }
    return vector->data;
}

void LobbyRequest_Destroy(void) {
}

void LobbyRequestMap_Find(Iter *out, LobbyRequestMap *map, LobbyRequest **key) {
    LobbyRequestMapNode *p = LobbyRequestMap_LowerBound(map, key, map->root, (LobbyRequestMapNode *)&map->root);
    if (p == (LobbyRequestMapNode *)&map->root || (*key)->id < p->request->id) {
        out->current = &map->root;
    } else {
        out->current = p;
    }
}

static LobbyRequestMapNode *LobbyRequestMap_LowerBound(LobbyRequestMap *map, LobbyRequest **key, LobbyRequestMapNode *node, LobbyRequestMapNode *best) {
    if (node) {
        u32 n = (*key)->id;
        do {
            if (node->request->id >= n) {
                best = node;
                node = node->left;
            } else {
                node = node->right;
            }
        } while (node);
    }
    return best;
}

void LobbyChat_OnEnumChannelsEach(void *a, int n) {
    if (n) {
        return;
    }
}
