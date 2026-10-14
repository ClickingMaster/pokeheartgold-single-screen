#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define A(p, o) ((void *)((char *)(p) + (o)))

typedef struct Node {
    void *left, *right;
    u32 parent;
    int key;
} Node;

typedef struct {
    int count;
    void *root, *last;
} Tree;

extern void DWC_Free(int, void *, int);
extern void DWCi_GsFree(void *);
extern void LobbyString_InitCopy(void *, void *);
extern void LobbyString_Destroy(void *);
extern void LobbyChannelMap_Find(void **, void *, void *);
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void *ArrayNew(int, int, void (*)(void *));
extern char sStr_DataNeNULL[];
extern char sStr_ChatCallbacksC[];
extern char sStr_ciCallbacksArrayElementFreeFn[];
extern int LobbyString_Compare(void *, void *);
extern void LobbyUserMap_MakeEntry(void *p, int a, void *b);
extern int LobbyChat_AreChannelUsersEnumerated(void *p, void *key);
extern int LobbyChat_SetChannelUsersEnumerated(void *p, void *key);
extern int LobbyChat_GetChannelUserLimit(void *p, void *key);
extern void *LobbyUserIdSet_Destroy(void *p);
extern void *LobbyUserMap_Destroy(void *p);
extern void *LobbyRequestMap_Destroy(void *p);
extern void *LobbyChannelMap_FindInsertPos(void *p, void *key, void **prev, u8 *left, u8 *first);
extern Node *LobbyUserIdSet_FindInsertPos(Tree *p, int *key, Node **prev, u8 *left, u8 *first);
extern Node *LobbyUserMap_FindInsertPos(Tree *p, int *key, Node **prev, u8 *left, u8 *first);
extern void LobbyChannelMap_DestroyNodes(void *p, void *node);
extern void *LobbyUserIdSet_Destroy2(void *p);
extern void LobbyUserIdSet_Swap(Tree *p, Tree *q);
extern void LobbyUserIdSet_DestroyNodes(void *p, void *node);
extern void LobbyUserMap_DestroyNodes(void *p, void *node);
extern void LobbyRequestMap_DestroyNodes(void *p, void *node);
extern void ciCallbacksArrayElementFreeFn(void *p);
extern int ciInitCallbacks(void *p);
extern int ArrayLength(void *);
extern void *ArrayNth(void *, int);
extern void ArrayFree(void *);
extern void ciFreeCallbackData(void *);
extern char sStr_ChatNeNULL[];
extern char sStr_ciCleanupCallbacks[];
extern char sStr_DataTypeGe0[];
extern char sStr_DataTypeLtCALLBACK_NUM[];
extern char sStr_DataCallbackNeNULL[];
extern char sStr_DataCallbackParamsNeNULL[];
extern char sStr_DataID_Ge0[];
extern void *LobbyChannelMap_Destroy(void *p);
extern void ciCleanupCallbacks(void *p);

typedef struct {
    Node *node;
    u8 ok;
} InsertResult;

extern Node *LobbyUserIdSet_InsertNode(Tree *, Node *, int, int, int *);
extern Node *LobbyUserMap_InsertNode(Tree *, Node *, int, int, int *);
extern u8 ov45_02255358[];
extern void LobbyUserIdSet_Insert(InsertResult *, Tree *, int *);
extern void LobbyUserMap_Insert(InsertResult *, Tree *, int *);

Node *LobbyUserIdSet_FindInsertPos(Tree *p, int *key, Node **prev, u8 *left, u8 *first) {
    Node *node, *last;
    *prev = 0;
    node = p->root;
    *left = 1;
    *first = 1;
    last = (Node *)&p->root;
    while (node) {
        last = node;
        if (*key < node->key) {
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

Node *LobbyUserMap_FindInsertPos(Tree *p, int *key, Node **prev, u8 *left, u8 *first) {
    Node *node, *last;
    *prev = 0;
    node = p->root;
    *left = 1;
    *first = 1;
    last = (Node *)&p->root;
    while (node) {
        last = node;
        if (*key < node->key) {
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

void LobbyChannelMap_DestroyNodes(void *p, void *node) {
    if (P(node, 0)) {
        LobbyChannelMap_DestroyNodes(p, P(node, 0));
    }
    if (P(node, 4)) {
        LobbyChannelMap_DestroyNodes(p, P(node, 4));
    }
    LobbyUserIdSet_Destroy(A(node, 28));
    LobbyString_Destroy(A(node, 12));
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void *LobbyUserIdSet_Destroy2(void *p) {
    if (P(p, 4)) {
        LobbyUserIdSet_DestroyNodes(p, P(p, 4));
    }
    return p;
}

void LobbyUserIdSet_Swap(Tree *p, Tree *q) {
    int n;
    Node *v;
    if (p == q) {
        return;
    }
    n = p->count;
    p->count = q->count;
    q->count = n;
    v = p->root;
    p->root = q->root;
    q->root = v;
    v = p->last;
    p->last = q->last;
    q->last = v;
    if (!p->root) {
        p->last = (Node *)&p->root;
    } else {
        ((Node *)p->root)->parent = (u32)&p->root | (((Node *)p->root)->parent & 1);
    }
    if (!q->root) {
        q->last = (Node *)&q->root;
    } else {
        ((Node *)q->root)->parent = (u32)&q->root | (((Node *)q->root)->parent & 1);
    }
}

void LobbyUserIdSet_DestroyNodes(void *p, void *node) {
    if (P(node, 0)) {
        LobbyUserIdSet_DestroyNodes(p, P(node, 0));
    }
    if (P(node, 4)) {
        LobbyUserIdSet_DestroyNodes(p, P(node, 4));
    }
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void LobbyUserMap_DestroyNodes(void *p, void *node) {
    if (P(node, 0)) {
        LobbyUserMap_DestroyNodes(p, P(node, 0));
    }
    if (P(node, 4)) {
        LobbyUserMap_DestroyNodes(p, P(node, 4));
    }
    LobbyString_Destroy(A(node, 16));
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void LobbyRequestMap_DestroyNodes(void *p, void *node) {
    if (P(node, 0)) {
        LobbyRequestMap_DestroyNodes(p, P(node, 0));
    }
    if (P(node, 4)) {
        LobbyRequestMap_DestroyNodes(p, P(node, 4));
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
