#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyUserIdSet_Find(Iter *out, Tree *t, int *key) {
    Node *n = LobbyUserIdSet_LowerBound(t, key, t->head.left, &t->head);
    if (n == &t->head || *key < n->key.i) {
        out->current = &t->head;
        return;
    }
    out->current = n;
}

Node *LobbyUserIdSet_LowerBound(Tree *t, int *key, Node *node, Node *res) {
    while (node != NULL) {
        if (node->key.i >= *key) {
            res = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return res;
}

BOOL LobbyUserMap_EraseKey(Tree *t, int *key) {
    Iter it;
    Iter res;
    Iter tmp;
    LobbyUserMap_Find(&it, t, key);
    if (it.current == &t->head) {
        return FALSE;
    }
    tmp.current = it.current;
    LobbyUserMap_Erase(&res, t, tmp);
    return TRUE;
}

void LobbyUserMap_Find(Iter *out, Tree *t, int *key) {
    Node *n = LobbyUserMap_LowerBound(t, key, t->head.left, &t->head);
    if (n == &t->head || *key < n->key.i) {
        out->current = &t->head;
        return;
    }
    out->current = n;
}

Node *LobbyUserMap_LowerBound(Tree *t, int *key, Node *node, Node *res) {
    while (node != NULL) {
        if (node->key.i >= *key) {
            res = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return res;
}

BOOL LobbyChat_RemoveChannelUser(LobbyChatManager *manager, Str *nick, Str *channel) {
    Tree *channels = (Tree *)manager->channelMap;
    int id;
    Iter it;
    id = LobbyChat_GetUserIdFromNick(nick);
    if (id == -1) {
        return FALSE;
    }
    LobbyChannelMap_Find(&it, channels, channel);
    if (it.current == &channels->head) {
        return FALSE;
    }
    if (!LobbyUserIdSet_EraseKey((Tree *)((u8 *)it.current + 0x1c), &id)) {
        return FALSE;
    }
    LobbyChat_RemoveUserIfUnused(manager, id);
    return TRUE;
}
