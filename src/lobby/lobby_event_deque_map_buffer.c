#include "global.h"

#include "ppw_lobby_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

int LobbyEventDequeMapIterator_Distance(Iter *, Iter *);
u32 **LobbyEventDequeMap_CopyToArray(Iter a, Iter b, u32 **out);

Vec *LobbyEventDequeMapBuffer_Destroy(Vec *v) {
    if (v->ptr != NULL) {
        u32 n = v->cap;
        Tag a = { 0 };
        v->cap -= n;
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

int LobbyEventDequeMapIterator_Distance(Iter *a, Iter *b) {
    u32 *mid;
    u32 *other;
    u32 *cur;
    int count;
    mid = a->mid;
    other = b->cur;
    cur = a->cur;
    count = cur - other;
    if (cur >= mid) {
        if (other < mid) {
            count -= a->last - a->first;
        }
    } else {
        if (other >= mid) {
            count += a->last - a->first;
        }
    }
    return count;
}

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

void LobbyEventDequeMap_AppendRange(DequeBuf *d, Iter *first, Iter *last, Tag unused) {
    u32 **target;
    int count, chunk;
    Iter b, e, a, c;
    if (!d->ptr) {
        return;
    }
    target = d->ptr + (d->begin + d->size) % d->cap;
    count = LobbyEventDequeMapIterator_Distance2(last, first);
    chunk = (d->ptr + d->cap) - target;
    chunk = *(count < chunk ? &count : &chunk);
    d->size += count;
    a.cur = first->cur;
    a.first = first->first;
    a.mid = first->mid;
    a.last = first->last;
    {
        Iter *p = LobbyEventDequeMapIterator_Advance2(&a, chunk);
        b.cur = p->cur;
        b.first = p->first;
        b.mid = p->mid;
        b.last = p->last;
    }
    LobbyEventDequeMap_CopyToArray(*first, b, target);
    count -= chunk;
    if (!count) {
        return;
    }
    c.cur = first->cur;
    c.first = first->first;
    c.mid = first->mid;
    c.last = first->last;
    {
        Iter *p = LobbyEventDequeMapIterator_Advance2(&c, chunk);
        e.cur = p->cur;
        e.first = p->first;
        e.mid = p->mid;
        e.last = p->last;
    }
    LobbyEventDequeMap_CopyToArray(e, *last, d->ptr);
}
