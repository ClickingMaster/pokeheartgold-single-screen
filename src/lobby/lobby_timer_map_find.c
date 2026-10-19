#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_timer.h"

void LobbyTimerMap_Find(LobbyTimerMapNode **out, LobbyTimerMap *map, u32 *key) {
    LobbyTimerMapNode *n = LobbyTimerMap_LowerBound(map, key, map->root, (LobbyTimerMapNode *)&map->root);
    if (n == (LobbyTimerMapNode *)&map->root || *key < n->id) {
        *out = (LobbyTimerMapNode *)&map->root;
    } else {
        *out = n;
    }
}

LobbyTimerMapNode *LobbyTimerMap_LowerBound(LobbyTimerMap *map, u32 *key, LobbyTimerMapNode *n, LobbyTimerMapNode *best) {
    if (n) {
        u32 x = *key;
        do {
            if (n->id >= x) {
                best = n;
                n = n->left;
            } else {
                n = n->right;
            }
        } while (n);
    }
    return best;
}
