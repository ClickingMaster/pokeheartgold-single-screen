#include "global.h"

#include "dwc/net_common_internal.h"

#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern LobbyUserIdSetNode *LobbyTree_EraseRebalance(LobbyUserIdSetNode *, void *);

void LobbyUserIdSet_Erase(Iterator *out, LobbyUserIdSet *set, Iterator node) {
    LobbyUserIdSetNode *old = node.current;
    if (node.current == set->first) {
        LobbyTreeIterator_Advance(&node);
        set->first = node.current;
    }
    out->current = LobbyTree_EraseRebalance(old, set->root);
    if (old) {
        DWC_Free(0, old, 0);
    }
    set->count--;
}
