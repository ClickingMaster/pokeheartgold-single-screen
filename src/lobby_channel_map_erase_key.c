#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "../lib/NitroDWC/src/tree-iterator4.h"
#pragma force_active on

struct S {
    int a;
    void *anchor;
};

typedef Iterator It;
void LobbyUserIdSet_Find(It *, struct S *, void *);
void LobbyChannelMap_Find(It *, struct S *, void *);
void LobbyUserIdSet_Erase(It *, struct S *, Iterator);
void LobbyChannelMap_Erase(It *, struct S *, Iterator);
int LobbyUserIdSet_EraseKey(struct S *, void *);
int LobbyChannelMap_EraseKey(struct S *, void *);

int LobbyChannelMap_EraseKey(struct S *s, void *k) {
    It it, out;
    LobbyChannelMap_Find(&it, s, k);
    if (it.current == &s->anchor) {
        return 0;
    }
    LobbyChannelMap_Erase(&out, s, (Iterator) { it.current });
    return 1;
}
