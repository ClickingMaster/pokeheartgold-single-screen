#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyUserIdSet_Find(Iter *out, LobbyUserIdSet *set, int *key) {
    LobbyUserIdSetNode *n = LobbyUserIdSet_LowerBound(set, key, set->root, (LobbyUserIdSetNode *)&set->root);
    if (n == (LobbyUserIdSetNode *)&set->root || *key < n->userId) {
        out->current = &set->root;
        return;
    }
    out->current = n;
}

LobbyUserIdSetNode *LobbyUserIdSet_LowerBound(LobbyUserIdSet *set, int *key, LobbyUserIdSetNode *node, LobbyUserIdSetNode *res) {
    while (node != NULL) {
        if (node->userId >= *key) {
            res = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return res;
}

BOOL LobbyUserMap_EraseKey(LobbyUserMap *map, int *key) {
    Iter it;
    Iter res;
    Iter tmp;
    LobbyUserMap_Find(&it, map, key);
    if (it.current == &map->root) {
        return FALSE;
    }
    tmp.current = it.current;
    LobbyUserMap_Erase(&res, map, tmp);
    return TRUE;
}

void LobbyUserMap_Find(Iter *out, LobbyUserMap *map, int *key) {
    LobbyUserMapNode *n = LobbyUserMap_LowerBound(map, key, map->root, (LobbyUserMapNode *)&map->root);
    if (n == (LobbyUserMapNode *)&map->root || *key < n->entry.userId) {
        out->current = &map->root;
        return;
    }
    out->current = n;
}

LobbyUserMapNode *LobbyUserMap_LowerBound(LobbyUserMap *map, int *key, LobbyUserMapNode *node, LobbyUserMapNode *res) {
    while (node != NULL) {
        if (node->entry.userId >= *key) {
            res = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return res;
}

BOOL LobbyChat_RemoveChannelUser(LobbyChatManager *manager, Str *nick, Str *channel) {
    LobbyChannelMap *channels = &manager->channelMap;
    int id;
    Iter it;
    id = LobbyChat_GetUserIdFromNick(nick);
    if (id == -1) {
        return FALSE;
    }
    LobbyChannelMap_Find(&it, channels, channel);
    if (it.current == &channels->root) {
        return FALSE;
    }
    if (!LobbyUserIdSet_EraseKey(&((LobbyChannelMapNode *)it.current)->entry.channel.users, &id)) {
        return FALSE;
    }
    LobbyChat_RemoveUserIfUnused(manager, id);
    return TRUE;
}

int LobbyUserIdSet_EraseKey(LobbyUserIdSet *set, void *key) {
    Iter it, out;
    LobbyUserIdSet_Find(&it, set, key);
    if (it.current == &set->root) {
        return 0;
    }
    LobbyUserIdSet_Erase(&out, set, (Iterator) { it.current });
    return 1;
}
