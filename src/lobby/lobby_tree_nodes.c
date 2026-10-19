#include "global.h"

#include "dwc/net_common_internal.h"

#include "lobby_chat_manager.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define P(p, o) (*(void **)((char *)(p) + (o)))

extern void __msl_assertion_failed(char *, char *, char *, int);
extern char sStr_DataNeNULL[];
extern char sStr_ChatCallbacksC[];
extern char sStr_ciCallbacksArrayElementFreeFn[];
extern void ciCallbacksArrayElementFreeFn(void *p);

void *LobbyUserMap_Destroy(LobbyUserMap *map) {
    if (map->root) {
        LobbyUserMap_DestroyNodes(map, map->root);
    }
    return map;
}

void *LobbyRequestMap_Destroy(LobbyRequestMap *map) {
    if (map->root) {
        LobbyRequestMap_DestroyNodes(map, map->root);
    }
    return map;
}

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

LobbyUserIdSetNode *LobbyUserIdSet_FindInsertPos(LobbyUserIdSet *set, int *key, LobbyUserIdSetNode **prev, u8 *left, u8 *first) {
    LobbyUserIdSetNode *node, *last;
    *prev = 0;
    node = set->root;
    *left = 1;
    *first = 1;
    last = (LobbyUserIdSetNode *)&set->root;
    while (node) {
        last = node;
        if (*key < node->userId) {
            node = node->left;
            *left = 1;
        } else {
            *prev = node;
            node = node->right;
            *left = 0;
            *first = 0;
        }
    }
    return last;
}

LobbyUserMapNode *LobbyUserMap_FindInsertPos(LobbyUserMap *map, int *key, LobbyUserMapNode **prev, u8 *left, u8 *first) {
    LobbyUserMapNode *node, *last;
    *prev = 0;
    node = map->root;
    *left = 1;
    *first = 1;
    last = (LobbyUserMapNode *)&map->root;
    while (node) {
        last = node;
        if (*key < node->entry.userId) {
            node = node->left;
            *left = 1;
        } else {
            *prev = node;
            node = node->right;
            *left = 0;
            *first = 0;
        }
    }
    return last;
}

void LobbyChannelMap_DestroyNodes(LobbyChannelMap *map, LobbyChannelMapNode *node) {
    if (node->left) {
        LobbyChannelMap_DestroyNodes(map, node->left);
    }
    if (node->right) {
        LobbyChannelMap_DestroyNodes(map, node->right);
    }
    LobbyUserIdSet_Destroy(&node->entry.channel.users);
    LobbyString_Destroy(&node->entry.name);
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void *LobbyUserIdSet_Destroy2(LobbyUserIdSet *set) {
    if (set->root) {
        LobbyUserIdSet_DestroyNodes(set, set->root);
    }
    return set;
}

void LobbyUserIdSet_Swap(LobbyUserIdSet *p, LobbyUserIdSet *q) {
    int n;
    LobbyUserIdSetNode *v;
    if (p == q) {
        return;
    }
    n = p->count;
    p->count = q->count;
    q->count = n;
    v = p->root;
    p->root = q->root;
    q->root = v;
    v = p->first;
    p->first = q->first;
    q->first = v;
    if (!p->root) {
        p->first = (LobbyUserIdSetNode *)&p->root;
    } else {
        p->root->parent = (u32)&p->root | (p->root->parent & 1);
    }
    if (!q->root) {
        q->first = (LobbyUserIdSetNode *)&q->root;
    } else {
        q->root->parent = (u32)&q->root | (q->root->parent & 1);
    }
}

void LobbyUserIdSet_DestroyNodes(LobbyUserIdSet *set, LobbyUserIdSetNode *node) {
    if (node->left) {
        LobbyUserIdSet_DestroyNodes(set, node->left);
    }
    if (node->right) {
        LobbyUserIdSet_DestroyNodes(set, node->right);
    }
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void LobbyUserMap_DestroyNodes(LobbyUserMap *map, LobbyUserMapNode *node) {
    if (node->left) {
        LobbyUserMap_DestroyNodes(map, node->left);
    }
    if (node->right) {
        LobbyUserMap_DestroyNodes(map, node->right);
    }
    LobbyString_Destroy(&node->entry.nick);
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void LobbyRequestMap_DestroyNodes(LobbyRequestMap *map, LobbyRequestMapNode *node) {
    if (node->left) {
        LobbyRequestMap_DestroyNodes(map, node->left);
    }
    if (node->right) {
        LobbyRequestMap_DestroyNodes(map, node->right);
    }
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void ciCallbacksArrayElementFreeFn(void *p) {
    if (!p) {
        __msl_assertion_failed(sStr_DataNeNULL, sStr_ChatCallbacksC, sStr_ciCallbacksArrayElementFreeFn, 164);
    }
    DWCi_GsFree(P(p, 20));
}
