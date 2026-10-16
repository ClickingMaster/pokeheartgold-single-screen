#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

Vec *LobbyEventDequeMapBuffer_Destroy(Vec *v) {
    if (v->ptr != NULL) {
        u32 n = *(volatile u32 *)&v->cap;
        volatile Tag a = { 0 };
        *(volatile u32 *)&v->cap -= n;
        if (v->ptr != NULL) {
            DWC_Free(0, v->ptr, 0);
        }
    }
    return v;
}

void LobbyEventDequeMapBuffer_Allocate(DequeBuf *d, u32 n) {
    if (!(gLobbyEventDequeMapBufferMaxSizeSet & 1)) {
        gLobbyEventDequeMapBufferMaxSize = 0x3FFFFFFE;
        gLobbyEventDequeMapBufferMaxSizeSet |= 1;
    }
    if (n > gLobbyEventDequeMapBufferMaxSize) {
        LobbyEventDequeMap_LengthError();
    }
    d->ptr = LobbyMem_Alloc((n + 1) * 4);
    d->cap = n + 1;
}

void LobbyEventDequeMapBuffer_PrependRange(DequeBuf *d, Iter *a, Iter *b) {
    u32 **dst = d->ptr + d->begin;
    if (a->cur <= b->cur) {
        int n = b->cur - a->cur;
        int m;
        memmove(dst - n, a->cur, n * 4);
        m = b->cur - a->cur;
        d->size += m;
        d->begin -= m;
    } else {
        int n1 = b->cur - b->first;
        u32 **p = dst - n1;
        int n2;
        int total;
        memmove(p, b->first, n1 * 4);
        n2 = a->last - a->cur;
        memmove(p - n2, a->cur, n2 * 4);
        total = (b->cur - b->first) + (a->last - a->cur);
        d->begin -= total;
        d->size += total;
    }
}

void LobbyEventDequeMapBuffer_AppendRange(DequeBuf *d, Iter *a, Iter *b) {
    u32 **dst = d->ptr + (d->begin + d->size) % d->cap;
    if (a->cur <= b->cur) {
        memmove(dst, a->cur, (b->cur - a->cur) * 4);
        d->size += b->cur - a->cur;
    } else {
        int n1 = a->last - a->cur;
        memmove(dst, a->cur, n1 * 4);
        memmove(dst + n1, b->first, (b->cur - b->first) * 4);
        d->size += (a->last - a->cur) + (b->cur - b->first);
    }
}

void LobbyEventDequeMapIterator_Advance(Iter *it, int n) {
    int size = it->last - it->first;
    n += it->cur - it->first;
    if (n < 0) {
        n += size;
    } else if (n >= size) {
        n -= size;
    }
    it->cur = it->first + n;
}
