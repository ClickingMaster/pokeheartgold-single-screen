#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

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
    return it.current == &channels->head ? NULL : (u8 *)it.current + 0x2c;
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

int LobbyChat_AddChannelUser(LobbyChatManager *manager, Str *name, Str *key2) {
    Tree *channels = (Tree *)manager->channelMap;
    int id;
    Iter it;
    InsertResult ir;
    Tag t1;
    Tag t0;
    id = LobbyChat_GetUserIdFromNick(name);
    if (id == -1) {
        return -1;
    }
    LobbyChannelMap_Find(&it, channels, key2);
    if (it.current == &channels->head) {
        return -1;
    }
    LobbyUserIdSet_Insert(&ir, (Tree *)((u8 *)it.current + 0x1c), &id);
    t1 = (Tag) { 0 };
    t0 = (Tag) { 0 };
    LobbyString_Assign(&LobbyUserMap_GetOrInsert((Tree *)manager->userMap, &id)->val, name);
    return id;
}

IntStrPair *LobbyUserMap_GetOrInsert(Tree *t, int *key) {
    Node *pos;
    u8 a;
    u8 b;
    Node *parent = LobbyUserMap_FindInsertPos2(t, key, &pos, &a, &b);
    if (pos == NULL || pos->key.i < *key) {
        Str empty;
        IntStrPair pair;
        Node *n;
        StrInit(&empty);
        PairInit(&pair, key, &empty);
        n = LobbyUserMap_InsertNode(t, parent, a, b, &pair);
        LobbyString_Destroy(&pair.val);
        LobbyString_Destroy(&empty);
        return (IntStrPair *)&n->key;
    }
    return (IntStrPair *)&pos->key;
}

Node *LobbyUserMap_FindInsertPos2(Tree *t, int *key, Node **pos, u8 *a, u8 *b) {
    Node *parent;
    Node *n;
    *pos = NULL;
    n = t->head.left;
    *a = 1;
    *b = 1;
    parent = &t->head;
    while (n != NULL) {
        parent = n;
        if (*key < n->key.i) {
            n = n->left;
            *a = 1;
        } else {
            *pos = n;
            n = n->right;
            *a = 0;
            *b = 0;
        }
    }
    return parent;
}
