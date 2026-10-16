#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

void LobbyVipVector_Allocate(Vec *v, u32 n) {
    if (n > 0x1FFFFFFF) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    v->ptr = LobbyMem_Alloc(n * 8);
    v->cap = n;
}

void LobbyEventVector_Allocate(Vec *v, u32 n) {
    if (n > 0x1FFFFFFF) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    v->ptr = LobbyMem_Alloc(n * 8);
    v->cap = n;
}

void LobbyEventDequeMap_InitCopy(DequeBuf *d, Iter a, Iter *b) {
    Iter tb;
    Iter ta;
    int n;
    tb = *b;
    ta = a;
    n = LobbyEventDequeMapIterator_Distance2(&tb, &ta);
    if (n == 0) {
        return;
    }
    {
        Tag t1 = { 0 };
        LobbyEventDequeMap_Allocate(d, n, t1);
    }
    {
        Tag t0 = { 0 };
        LobbyEventDequeMap_AppendRange(d, &a, b, t0);
    }
}
