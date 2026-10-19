#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"

extern u8 ov45_02255358[];
extern int LobbyString_Compare(Str *, Str *);
extern void *LobbyTree_EraseRebalance(void *, void *);
extern void LobbyString_Destroy(Str *);

static inline int isLess(Str *a, Str *b) {
    return LobbyString_Compare(a, b) < 0;
}

void LobbyChannelMap_Insert(LobbyChannelMapInsertResult *out, LobbyChannelMap *map, LobbyChannelMapEntry *entry) {
    u8 left, first;
    LobbyChannelMapNode *previous;
    LobbyChannelMapNode *parent = LobbyChannelMap_FindInsertPos(map, &entry->name, &previous, &left, &first);
    if (previous == 0 || isLess(&previous->entry.name, &entry->name)) {
        out->node = LobbyChannelMap_InsertNode(map, parent, left, first, entry);
        out->inserted = ov45_02255358[4];
    } else {
        out->node = previous;
        out->inserted = ov45_02255358[5];
    }
}

void LobbyChannelMap_Erase(Iterator *out, LobbyChannelMap *map, Iterator node) {
    LobbyChannelMapNode *saved = node.current;
    if (saved == map->first) {
        LobbyTreeIterator_Advance(&node);
        map->first = node.current;
    }
    out->current = LobbyTree_EraseRebalance(saved, map->root);
    LobbyUserIdSet_Destroy(&saved->entry.channel.users);
    LobbyString_Destroy(&saved->entry.name);
    if (saved) {
        DWC_Free(0, saved, 0);
    }
    map->count--;
}
