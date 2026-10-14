#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct {
    int *current, *begin, *pivot, *end;
} Iter;

int LobbyEventDequeMapIterator_Distance(Iter *, Iter *);

int LobbyEventDequeMapIterator_Distance(Iter *a, Iter *b) {
    int *pivot;
    int *other;
    int *current;
    int count;
    pivot = a->pivot;
    other = b->current;
    current = a->current;
    count = current - other;
    if (current >= pivot) {
        if (other < pivot) {
            count -= a->end - a->begin;
        }
    } else {
        if (other >= pivot) {
            count += a->end - a->begin;
        }
    }
    return count;
}
