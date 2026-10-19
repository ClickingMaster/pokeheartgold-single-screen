#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 byte;
} Allocator;

typedef struct {
    int a, b;
} VipRecord;

typedef struct {
    VipRecord *data;
    u32 size;
    u32 capacity;
} VipVector;

extern void LobbyVipVector_Allocate(VipVector *, u32, Allocator);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void LobbyVipVector_Reallocate(VipVector *, Allocator, u32);
u32 LobbyVipVector_RecommendSize(VipVector *, u32);

void LobbyVipVector_Reallocate(VipVector *b, Allocator incoming, u32 n) {
    u32 size = b->size;
    Allocator old = { 0 };
    volatile Allocator copy;
    (void)incoming;
    (void)old;
    b->size -= size;
    if (b->data) {
        if (b->data) {
            DWC_Free(0, b->data, 0);
        }
        b->data = 0;
        b->capacity = 0;
    }
    Allocator fresh = { 0 };
    copy.byte = 0;
    LobbyVipVector_Allocate(b, n, fresh);
}

u32 LobbyVipVector_RecommendSize(VipVector *p, u32 n) {
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
