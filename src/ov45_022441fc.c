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
extern void ov45_02242C74(void **, void *, void *);
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void *ArrayNew(int, int, void (*)(void *));
extern char ov45_02255458[];
extern char ov45_02255468[];
extern char ov45_02255438[];
extern int LobbyString_Compare(void *, void *);
extern void ov45_02243780(void *p, int a, void *b);
extern int ov45_02243794(void *p, void *key);
extern int ov45_022437CC(void *p, void *key);
extern int ov45_02243808(void *p, void *key);
extern void *ov45_02243F1C(void *p);
extern void *ov45_02244130(void *p);
extern void *ov45_02244150(void *p);
extern void *ov45_02244170(void *p, void *key, void **prev, u8 *left, u8 *first);
extern Node *ov45_022441FC(Tree *p, int *key, Node **prev, u8 *left, u8 *first);
extern Node *ov45_02244268(Tree *p, int *key, Node **prev, u8 *left, u8 *first);
extern void ov45_022442D4(void *p, void *node);
extern void *ov45_02244330(void *p);
extern void ov45_02244350(Tree *p, Tree *q);
extern void ov45_022443DC(void *p, void *node);
extern void ov45_02244428(void *p, void *node);
extern void ov45_0224447C(void *p, void *node);
extern void ov45_022444C8(void *p);
extern int ciInitCallbacks(void *p);
extern int ArrayLength(void *);
extern void *ArrayNth(void *, int);
extern void ArrayFree(void *);
extern void ciFreeCallbackData(void *);
extern char ov45_022554F0[];
extern char ov45_0225540C[];
extern char ov45_02255478[];
extern char ov45_02255488[];
extern char ov45_022554A4[];
extern char ov45_022554BC[];
extern char ov45_022554DC[];
extern void *ov45_02243A98(void *p);
extern void ciCleanupCallbacks(void *p);

typedef struct {
    Node *node;
    u8 ok;
} InsertResult;

extern Node *ov45_02243E44(Tree *, Node *, int, int, int *);
extern Node *ov45_0224404C(Tree *, Node *, int, int, int *);
extern u8 ov45_02255358[];
extern void ov45_02243AB8(InsertResult *, Tree *, int *);
extern void ov45_02243F3C(InsertResult *, Tree *, int *);

Node *ov45_022441FC(Tree *p, int *key, Node **prev, u8 *left, u8 *first) {
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

Node *ov45_02244268(Tree *p, int *key, Node **prev, u8 *left, u8 *first) {
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

void ov45_022442D4(void *p, void *node) {
    if (P(node, 0)) {
        ov45_022442D4(p, P(node, 0));
    }
    if (P(node, 4)) {
        ov45_022442D4(p, P(node, 4));
    }
    ov45_02243F1C(A(node, 28));
    LobbyString_Destroy(A(node, 12));
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void *ov45_02244330(void *p) {
    if (P(p, 4)) {
        ov45_022443DC(p, P(p, 4));
    }
    return p;
}

void ov45_02244350(Tree *p, Tree *q) {
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

void ov45_022443DC(void *p, void *node) {
    if (P(node, 0)) {
        ov45_022443DC(p, P(node, 0));
    }
    if (P(node, 4)) {
        ov45_022443DC(p, P(node, 4));
    }
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void ov45_02244428(void *p, void *node) {
    if (P(node, 0)) {
        ov45_02244428(p, P(node, 0));
    }
    if (P(node, 4)) {
        ov45_02244428(p, P(node, 4));
    }
    LobbyString_Destroy(A(node, 16));
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void ov45_0224447C(void *p, void *node) {
    if (P(node, 0)) {
        ov45_0224447C(p, P(node, 0));
    }
    if (P(node, 4)) {
        ov45_0224447C(p, P(node, 4));
    }
    if (node) {
        DWC_Free(0, node, 0);
    }
}

void ov45_022444C8(void *p) {
    if (!p) {
        __msl_assertion_failed(ov45_02255458, ov45_02255468, ov45_02255438, 164);
    }
    DWCi_GsFree(P(p, 20));
}
