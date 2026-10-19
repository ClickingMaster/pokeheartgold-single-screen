#include <nitro/types.h>

#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef Iterator It;
void LobbyUserIdSet_Find(It *, LobbyUserIdSet *, void *);

int LobbyUserIdSet_EraseKey(LobbyUserIdSet *set, void *k) {
    It it, out;
    LobbyUserIdSet_Find(&it, set, k);
    if (it.current == &set->root) {
        return 0;
    }
    LobbyUserIdSet_Erase(&out, set, (Iterator) { it.current });
    return 1;
}
