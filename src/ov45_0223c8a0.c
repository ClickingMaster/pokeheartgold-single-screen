#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 a, b;
    void **data;
    u32 start, count, capacity;
} Container;

void ov00_021EC2EC(int, void *, int);
void ov45_02238508(void *);
Container *ov45_0223CA14(Container *);

typedef struct {
    u32 *current, *base, *split, *end;
} Iter;

void ov45_0223C8A0(Iter *, Iter *, Iter *, Iter *);

static inline void copyspan(u32 *first, u32 *last, u32 *dst) {
    u32 *a = first;
    u32 *b = last;
    memmove(dst, a, (b - a) * 4);
}

static inline const int *minref(const int *a, const int *b) {
    return *a < *b ? a : b;
}

static inline void destroy(u8 *allocator, void *p) {
    ov00_021EC2EC(0, p, 0);
    (void)allocator;
}

void ov45_0223C8A0(Iter *out, Iter *first, Iter *last, Iter *dst) {
    while (first->current != last->current) {
        int room = dst->end - dst->current;
        int n;
        if ((first->current >= first->split) == (last->current >= last->split)) {
            int span = last->current - first->current;
            n = *minref(&room, &span);
        } else {
            int span = first->end - first->current;
            n = *minref(&room, &span);
        }
        {
            u32 *end = (u32 *)((u32)first->current + n * 4);
            copyspan(first->current, end, dst->current);
        };
        first->current += n - 1;
        first->current++;
        if (first->current == first->end) {
            first->current = first->base;
        }
        dst->current += n - 1;
        dst->current++;
        if (dst->current == dst->end) {
            dst->current = dst->base;
        }
    }
    out->current = dst->current;
    out->base = dst->base;
    out->split = dst->split;
    out->end = dst->end;
}
