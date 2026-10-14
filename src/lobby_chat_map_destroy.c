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
extern char ov45_02255458[];
extern char ov45_02255468[];
extern char ov45_02255438[];
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
extern char ov45_022554F0[];
extern char ov45_0225540C[];
extern char ov45_02255478[];
extern char ov45_02255488[];
extern char ov45_022554A4[];
extern char ov45_022554BC[];
extern char ov45_022554DC[];
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

void *LobbyUserMap_Destroy(void *p) {
    if (P(p, 4)) {
        LobbyUserMap_DestroyNodes(p, P(p, 4));
    }
    return p;
}

void *LobbyRequestMap_Destroy(void *p) {
    if (P(p, 4)) {
        LobbyRequestMap_DestroyNodes(p, P(p, 4));
    }
    return p;
}
