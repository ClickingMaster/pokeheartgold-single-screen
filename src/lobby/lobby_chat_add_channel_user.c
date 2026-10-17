#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

static inline NamedEntry *ChannelNodeEntry(Node *node) {
    return (NamedEntry *)&node->key;
}

void LobbyChannelMap_MakeEntry(NamedEntry *dst, Str *name, Entry *src) {
    LobbyString_InitCopy(&dst->name, name);
    dst->e.a = src->a;
    dst->e.t.count = 0;
    dst->e.t.root = NULL;
    dst->e.t.leftmost = (Node *)&dst->e.t.root;
    LobbyUserIdSet_Assign(&dst->e.t, &src->t);
    dst->e.x10 = src->x10;
    dst->e.callbacks = src->callbacks;
}

void *LobbyChat_GetChannelCallbacks(LobbyChatManager *manager, Str *key) {
    Tree *channels = (Tree *)manager->channelMap;
    Iter it;
    LobbyChannelMap_Find(&it, channels, key);
    return it.current == &channels->head ? NULL : &ChannelNodeEntry(it.current)->e.callbacks;
}

void LobbyChannelMap_Find(Iter *out, Tree *t, Str *key) {
    Node *n = LobbyChannelMap_LowerBound(t, key, t->head.left, &t->head);
    if (n == &t->head || StrLess(key, &n->key.str)) {
        out->current = &t->head;
        return;
    }
    out->current = n;
}

Node *LobbyChannelMap_LowerBound(Tree *t, Str *key, Node *n, Node *res) {
    while (n != NULL) {
        if (!StrLess(&n->key.str, key)) {
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
    Tree *channels = (Tree *)manager->channelMap;
    int id;
    Iter it;
    InsertResult ir;
    Tag t1;
    Tag t0;
    id = LobbyChat_GetUserIdFromNick(nick);
    if (id == -1) {
        return -1;
    }
    LobbyChannelMap_Find(&it, channels, channel);
    if (it.current == &channels->head) {
        return -1;
    }
    LobbyUserIdSet_Insert(&ir, (Tree *)&ChannelNodeEntry(it.current)->e.t, &id);
    t1 = (Tag) { 0 };
    t0 = (Tag) { 0 };
    LobbyString_Assign(&LobbyUserMap_GetOrInsert((Tree *)manager->userMap, &id)->val, nick);
    return id;
}

IntStrPair *LobbyUserMap_GetOrInsert(Tree *t, int *key) {
    Node *pos;
    u8 insertLeft;
    u8 isLeftmost;
    Node *parent = LobbyUserMap_FindInsertPos2(t, key, &pos, &insertLeft, &isLeftmost);
    if (pos == NULL || pos->key.i < *key) {
        Str empty;
        IntStrPair pair;
        Node *n;
        StrInit(&empty);
        PairInit(&pair, key, &empty);
        n = LobbyUserMap_InsertNode(t, parent, insertLeft, isLeftmost, &pair);
        LobbyString_Destroy(&pair.val);
        LobbyString_Destroy(&empty);
        return (IntStrPair *)&n->key;
    }
    return (IntStrPair *)&pos->key;
}

Node *LobbyUserMap_FindInsertPos2(Tree *t, int *key, Node **pos, u8 *insertLeft, u8 *isLeftmost) {
    Node *parent;
    Node *n;
    *pos = NULL;
    n = t->head.left;
    *insertLeft = 1;
    *isLeftmost = 1;
    parent = &t->head;
    while (n != NULL) {
        parent = n;
        if (*key < n->key.i) {
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
