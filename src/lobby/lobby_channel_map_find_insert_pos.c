#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"

extern int LobbyString_Compare(Str *, Str *);

LobbyChannelMapNode *LobbyChannelMap_FindInsertPos(LobbyChannelMap *map, Str *key, LobbyChannelMapNode **found, u8 *left, u8 *empty) {
    LobbyChannelMapNode *parent;
    LobbyChannelMapNode *p;
    _Bool comparison;
    *found = 0;
    p = map->root;
    *left = 1;
    *empty = 1;
    parent = (LobbyChannelMapNode *)&map->root;
    while (p) {
        parent = p;
        comparison = (_Bool)(LobbyString_Compare(key, &p->entry.name) < 0);
        if (comparison) {
            p = p->left;
            *left = 1;
        } else {
            *found = p;
            p = p->right;
            *left = 0;
            *empty = 0;
        }
    }
    return parent;
}
