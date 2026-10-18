#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define U(p, n) (*(unsigned *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

static void *LobbyRequestMap_LowerBound(void *c, void *key, void *node, void *best);

int LobbyRequest_GetParam(void *p) {
    return W(p, 8);
}

void *LobbyU32Vector_Data(void *p) {
    if (W(p, 4) == 0) {
        return 0;
    }
    return P(p, 0);
}

void LobbyRequest_Destroy(void) {
}

void LobbyRequestMap_Find(Iter *out, Tree *tree, void *key) {
    void *p = LobbyRequestMap_LowerBound(tree, key, P(tree, 4), (u8 *)tree + 4);
    if (p == (u8 *)tree + 4 || U(P(key, 0), 4) < U(P(p, 12), 4)) {
        out->current = (u8 *)tree + 4;
    } else {
        out->current = p;
    }
}

static void *LobbyRequestMap_LowerBound(void *c, void *key, void *node, void *best) {
    if (node) {
        unsigned n = U(P(key, 0), 4);
        do {
            if (U(P(node, 12), 4) >= n) {
                best = node;
                node = P(node, 0);
            } else {
                node = P(node, 4);
            }
        } while (node);
    }
    return best;
}

void LobbyChat_OnEnumChannelsEach(void *a, int n) {
    if (n) {
        return;
    }
}
