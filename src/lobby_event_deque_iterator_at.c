#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 **cur;
    u8 **first;
    u8 **mid;
    u8 **last;
} MapIt;

typedef struct {
    MapIt map;
    u8 *ptr;
} DequeIt;

typedef struct {
    u32 start;
    u32 size;
    u8 **map;
    u32 head;
    u32 count;
    u32 tail;
} Deque;

void LobbyEventDequeMapIterator_Advance(MapIt *it, u32 n);
void LobbyEventDeque_IteratorAt(DequeIt *out, Deque *d, u32 idx);

void LobbyEventDeque_IteratorAt(DequeIt *out, Deque *d, u32 idx) {
    MapIt it;
    u8 *p;
    u8 **map;
    u8 **cur;
    idx += d->start;
    map = d->map;
    cur = map + d->head;
    it.last = map + d->tail;
    it.cur = cur;
    it.first = map;
    it.mid = cur;
    LobbyEventDequeMapIterator_Advance(&it, idx >> 7);
    {
        u8 **c = it.cur, **f = it.first, **m = it.mid, **l = it.last;
        p = (d->count == 0) ? NULL : *c;
        out->map.cur = c;
        out->map.first = f;
        out->map.mid = m;
        out->map.last = l;
    }
    out->ptr = p + (idx & 0x7f) * 8;
}
