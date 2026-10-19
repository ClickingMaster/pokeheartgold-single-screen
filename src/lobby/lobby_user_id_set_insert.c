#include "global.h"

#include "lobby_chat_manager.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>


extern u8 ov45_02255358[];

void *LobbyChannelMap_Destroy(LobbyChannelMap *map) {
    if (map->root) {
        LobbyChannelMap_DestroyNodes(map, map->root);
    }
    return map;
}

void LobbyUserIdSet_Insert(LobbyUserIdSetInsertResult *out, LobbyUserIdSet *set, int *key) {
    LobbyUserIdSetNode *prev;
    u8 left, first;
    LobbyUserIdSetNode *last;
    last = LobbyUserIdSet_FindInsertPos(set, key, &prev, &left, &first);
    if (!prev || prev->userId < *key) {
        out->node = LobbyUserIdSet_InsertNode(set, last, left, first, key);
        out->inserted = ov45_02255358[3];
    } else {
        out->node = prev;
        out->inserted = ov45_02255358[7];
    }
}
