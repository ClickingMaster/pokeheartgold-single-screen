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

void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
u32 LobbyEventVector_RecommendSize(Vec *, u32);

u32 LobbyEventVector_RecommendSize(Vec *p, u32 n) {
    u32 used = p->capacity, a, b;
    if (n > 0x1fffffff - used) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    if (used < 0xaaaaaaa) {
        a = (used + 1) * 3 / 5;
        return used + *(n > a ? &n : &a);
    }
    if (used < 0x15555554) {
        b = (used + 1) / 2;
        return used + *(n > b ? &n : &b);
    }
    return 0x1fffffff;
}
