#include "global.h"

#include "ppw_lobby_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void LobbyEventDequeMap_Allocate(DequeBuf *d, u32 n, Tag unused) {
    if (!(gLobbyEventDequeMapMaxSizeSet & 1)) {
        gLobbyEventDequeMapMaxSize = 0x3FFFFFFE;
        gLobbyEventDequeMapMaxSizeSet |= 1;
    }
    if (n > gLobbyEventDequeMapMaxSize) {
        LobbyEventDequeMap_LengthError();
    }
    d->ptr = LobbyMem_Alloc((n + 1) * 4);
    d->cap = n + 1;
}

Iter *LobbyEventDequeMapIterator_Advance2(Iter *it, int n) {
    int size = it->last - it->first;
    n += it->cur - it->first;
    if (n < 0) {
        n += size;
    } else if (n >= size) {
        n -= size;
    }
    it->cur = it->first + n;
    return it;
}

int LobbyEventDequeMapIterator_Distance2(Iter *x, Iter *y) {
    int n;
    u32 *m = x->mid;
    n = x->cur - y->cur;
    if (x->cur >= m) {
        if (y->cur < m) {
            n -= x->last - x->first;
        }
    } else {
        if (y->cur >= m) {
            n += x->last - x->first;
        }
    }
    return n;
}
