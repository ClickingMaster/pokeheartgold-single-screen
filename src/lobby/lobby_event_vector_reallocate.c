#include "global.h"

#include "dwc/net_common_internal.h"
#include "lobby_event_deque.h"
#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
extern void LobbyEventVector_Allocate(LobbyEventVector *, u32, LobbyTag);
void LobbyEventVector_Reallocate(LobbyEventVector *, LobbyTag, u32);
u32 LobbyEventVector_RecommendSize(LobbyEventVector *, u32);

void LobbyEventVector_Reallocate(LobbyEventVector *v, LobbyTag incoming, u32 n) {
    u32 size = v->size;
    LobbyTag old = { 0 };
    volatile LobbyTag copy;
    (void)incoming;
    (void)old;
    v->size -= size;
    if (v->data) {
        if (v->data) {
            DWC_Free(0, v->data, 0);
        }
        v->data = 0;
        v->capacity = 0;
    }
    LobbyTag fresh = { 0 };
    copy.unused = 0;
    LobbyEventVector_Allocate(v, n, fresh);
}

u32 LobbyEventVector_RecommendSize(LobbyEventVector *v, u32 n) {
    u32 used = v->capacity, a, b;
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
