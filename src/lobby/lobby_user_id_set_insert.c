#include "global.h"

#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define P(p, o) (*(void **)((char *)(p) + (o)))

extern void LobbyChannelMap_DestroyNodes(void *p, void *node);
extern void *LobbyChannelMap_Destroy(void *p);

extern u8 ov45_02255358[];

void *LobbyChannelMap_Destroy(void *p) {
    if (P(p, 4)) {
        LobbyChannelMap_DestroyNodes(p, P(p, 4));
    }
    return p;
}

void LobbyUserIdSet_Insert(LobbyUserIdSetInsertResult *out, LobbyUserIdSet *set, int *key) {
    LobbyUserIdSetNode *prev;
    u8 left, first;
    LobbyUserIdSetNode *last;
    last = LobbyUserIdSet_FindInsertPos(set, key, &prev, &left, &first);
    if (!prev || prev->userId < *key) {
        out->node = LobbyUserIdSet_InsertNode(set, last, left, first, key);
        out->inserted = ov45_02255358[3];
    } else {
        out->node = prev;
        out->inserted = ov45_02255358[7];
    }
}
