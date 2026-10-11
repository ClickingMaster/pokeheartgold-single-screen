#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    u32 *cur, *begin, *divider, *end;
} Iter;

u32 *ov45_0223D0A8(Iter a, Iter b, u32 *out);

static inline u32 *nextPointer(Iter *p, int n) {
    return p->cur + n;
}

typedef struct {
    u32 *data;
    u32 start, size, capacity;
} Ring;

extern int ov45_0223D208(Iter *, Iter *);
extern Iter *ov45_0223D1C4(Iter *, int);
void ov45_0223CF30(Ring *, Iter *, Iter *);

void ov45_0223CF30(Ring *r, Iter *first, Iter *last) {
    u32 *target;
    int count, chunk;
    Iter b, d, a, c;
    if (!r->data) {
        return;
    }
    target = r->data + (r->start + r->size) % r->capacity;
    count = ov45_0223D208(last, first);
    chunk = (r->data + r->capacity) - target;
    chunk = *(count < chunk ? &count : &chunk);
    r->size += count;
    a.cur = first->cur;
    a.begin = first->begin;
    a.divider = first->divider;
    a.end = first->end;
    {
        Iter *p = ov45_0223D1C4(&a, chunk);
        b.cur = p->cur;
        b.begin = p->begin;
        b.divider = p->divider;
        b.end = p->end;
    }
    ov45_0223D0A8(*first, b, target);
    count -= chunk;
    if (!count) {
        return;
    }
    c.cur = first->cur;
    c.begin = first->begin;
    c.divider = first->divider;
    c.end = first->end;
    {
        Iter *p = ov45_0223D1C4(&c, chunk);
        d.cur = p->cur;
        d.begin = p->begin;
        d.divider = p->divider;
        d.end = p->end;
    }
    ov45_0223D0A8(d, *last, r->data);
}
