#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChannelMap_MakeEntry(LobbyChannelMapEntry *dst, Str *name, LobbyChannel *src) {
    LobbyString_InitCopy(&dst->name, name);
    dst->channel.usersEnumerated = src->usersEnumerated;
    dst->channel.users.count = 0;
    dst->channel.users.root = NULL;
    dst->channel.users.first = (LobbyUserIdSetNode *)&dst->channel.users.root;
    LobbyUserIdSet_Assign(&dst->channel.users, &src->users);
    dst->channel.userLimit = src->userLimit;
    dst->channel.callbacks = src->callbacks;
}

LobbyChannelCallbacks *LobbyChat_GetChannelCallbacks(LobbyChatManager *manager, Str *key) {
    LobbyChannelMap *channels = &manager->channelMap;
    Iter it;
    LobbyChannelMapNode *node;
    LobbyChannelMap_Find(&it, channels, key);
    node = it.current;
    return node == (LobbyChannelMapNode *)&channels->root ? NULL : &node->entry.channel.callbacks;
}

void LobbyChannelMap_Find(Iter *out, LobbyChannelMap *map, Str *key) {
    LobbyChannelMapNode *n = LobbyChannelMap_LowerBound(map, key, map->root, (LobbyChannelMapNode *)&map->root);
    if (n == (LobbyChannelMapNode *)&map->root || StrLess(key, &n->entry.name)) {
        out->current = &map->root;
        return;
    }
    out->current = n;
}

LobbyChannelMapNode *LobbyChannelMap_LowerBound(LobbyChannelMap *map, Str *key, LobbyChannelMapNode *n, LobbyChannelMapNode *res) {
    while (n != NULL) {
        if (!StrLess(&n->entry.name, key)) {
            res = n;
            n = n->left;
        } else {
            n = n->right;
        }
    }
    return res;
}

// Add the user to the channel's user set and record its nick; returns the user ID or -1.
int LobbyChat_AddChannelUser(LobbyChatManager *manager, Str *nick, Str *channel) {
    LobbyChannelMap *channels = &manager->channelMap;
    int id;
    Iter it;
    LobbyChannelMapNode *node;
    LobbyUserIdSetInsertResult ir;
    LobbyTag t1;
    LobbyTag t0;
    id = LobbyChat_GetUserIdFromNick(nick);
    if (id == -1) {
        return -1;
    }
    LobbyChannelMap_Find(&it, channels, channel);
    node = it.current;
    if (node == (LobbyChannelMapNode *)&channels->root) {
        return -1;
    }
    LobbyUserIdSet_Insert(&ir, &node->entry.channel.users, &id);
    t1 = (LobbyTag) { 0 };
    t0 = (LobbyTag) { 0 };
    LobbyString_Assign(&LobbyUserMap_GetOrInsert(&manager->userMap, &id)->nick, nick);
    return id;
}

LobbyUserMapEntry *LobbyUserMap_GetOrInsert(LobbyUserMap *map, int *key) {
    LobbyUserMapNode *pos;
    u8 insertLeft;
    u8 isLeftmost;
    LobbyUserMapNode *parent = LobbyUserMap_FindInsertPos2(map, key, &pos, &insertLeft, &isLeftmost);
    if (pos == NULL || pos->entry.userId < *key) {
        Str empty;
        LobbyUserMapEntry pair;
        LobbyUserMapNode *n;
        StrInit(&empty);
        PairInit(&pair, key, &empty);
        n = LobbyUserMap_InsertNode(map, parent, insertLeft, isLeftmost, &pair);
        LobbyString_Destroy(&pair.nick);
        LobbyString_Destroy(&empty);
        return &n->entry;
    }
    return &pos->entry;
}

LobbyUserMapNode *LobbyUserMap_FindInsertPos2(LobbyUserMap *map, int *key, LobbyUserMapNode **pos, u8 *insertLeft, u8 *isLeftmost) {
    LobbyUserMapNode *parent;
    LobbyUserMapNode *n;
    *pos = NULL;
    n = map->root;
    *insertLeft = 1;
    *isLeftmost = 1;
    parent = (LobbyUserMapNode *)&map->root;
    while (n != NULL) {
        parent = n;
        if (*key < n->entry.userId) {
            n = n->left;
            *insertLeft = 1;
        } else {
            *pos = n;
            n = n->right;
            *insertLeft = 0;
            *isLeftmost = 0;
        }
    }
    return parent;
}
