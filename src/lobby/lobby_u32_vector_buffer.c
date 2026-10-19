#include "global.h"

#include "dwc/net_common_internal.h"
#include "ppw_lobby_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

static SplitBuf *LobbyU32VectorBuffer_Deallocate(SplitBuf *b);

SplitBuf *LobbyU32VectorBuffer_Destroy(SplitBuf *b) {
    b->size = 0;
    LobbyU32VectorBuffer_Deallocate(b);
    return b;
}

void LobbyU32VectorBuffer_Allocate(SplitBuf *b, u32 cap, Tag unused) {
    if (cap > 0x3fffffff) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    b->ptr = LobbyMem_Alloc(cap * 4);
    b->cap = cap;
}

static SplitBuf *LobbyU32VectorBuffer_Deallocate(SplitBuf *b) {
    if (b->ptr) {
        u32 old = b->size;

        struct {
            volatile u8 zero;
        } init = { 0 };

        b->size -= old;
        if (b->ptr) {
            DWC_Free(0, b->ptr, 0);
        }
    }
    return b;
}
