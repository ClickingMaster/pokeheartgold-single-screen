#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_timer.h"

LobbyTimerMapNode *LobbyTimerMap_FindInsertPos(LobbyTimerMap *map, const u32 *key, LobbyTimerMapNode **last, u8 *left, u8 *first) {
    LobbyTimerMapNode *n;
    LobbyTimerMapNode *parent = (LobbyTimerMapNode *)&map->root;
    *last = NULL;
    n = map->root;
    *left = 1;
    *first = 1;
    while (n) {
        parent = n;
        if (*key < n->id) {
            n = n->left;
            *left = 1;
        } else {
            *last = n;
            n = n->right;
            *left = 0;
            *first = 0;
        }
    }
    return parent;
}
