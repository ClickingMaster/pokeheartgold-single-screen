#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    int a, b;
} Pair;

typedef struct {
    Pair *data;
    u32 size, capacity;
} Vec;

u32 LobbyVipVector_RecommendSize(Vec *, u32);
void LobbyVipVector_AssignRange(Vec *, Pair *, Pair *);
void LobbyVipVector_Reallocate(Vec *, u32, u32);

void LobbyVipVector_AssignRange(Vec *p, Pair *first, Pair *last) {
    u32 n = last - first, growth;
    Pair *end, *dest;
    volatile u8 a, c, d, b;
    if (p->capacity < n) {
        a = 0;
        growth = LobbyVipVector_RecommendSize(p, n - p->capacity);
    } else {
        end = first;
        end += *(p->size < n ? &p->size : &n);
        dest = p->data;
        while (first < end) {
            dest->a = first->a;
            dest->b = first->b;
            ++first;
            ++dest;
        }
        if (n < p->size) {
            u32 old = p->size;
            c = 0;
            p->size = *(volatile u32 *)&p->size - (old - n);
            return;
        }
        if (p->size >= n) {
            return;
        }
        d = 0;
        dest = *(Pair *volatile *)&p->data + *(volatile u32 *)&p->size;
        while (end != last) {
            if (dest) {
                *dest = *end;
            }
            ++p->size;
            ++end;
            ++dest;
        }
        return;
    }
    LobbyVipVector_Reallocate(p, n, growth);
    b = 0;
    dest = p->data + *(volatile u32 *)&p->size;
    for (; first != last; ++first, ++dest) {
        if (dest) {
            *dest = *first;
        }
        ++p->size;
    }
}
