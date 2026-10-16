#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

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

void LobbyEventDequeMapIterator_Advance2(Iter *it, int n) {
    int size = it->last - it->first;
    n += it->cur - it->first;
    if (n < 0) {
        n += size;
    } else if (n >= size) {
        n -= size;
    }
    it->cur = it->first + n;
}
