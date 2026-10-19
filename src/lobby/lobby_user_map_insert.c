#include "global.h"

#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern u8 ov45_02255358[];

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
