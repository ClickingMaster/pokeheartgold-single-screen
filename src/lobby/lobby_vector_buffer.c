#include "global.h"

#include "dwc/net_common_internal.h"
#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern void *LobbyMem_Alloc(u32);
extern int LobbyString_Destroy();
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void LobbyStringVectorBuffer_EraseAtEnd(LobbyStringVectorBuffer *, u32, LobbyTag, u32);

static LobbyCharVectorBuffer *LobbyCharVectorBuffer_Deallocate(LobbyCharVectorBuffer *p);

LobbyStringVectorBuffer *LobbyStringVectorBuffer_Destroy(LobbyStringVectorBuffer *p) {
    Str *first = p->data + p->start, *end = first + p->size;
    while (end > first) {
        end--;
        LobbyString_Destroy(end);
    }
    p->size = 0;
    LobbyStringVectorBuffer_Deallocate(p);
    return p;
}

LobbyCharVectorBuffer *LobbyCharVectorBuffer_Destroy(LobbyCharVectorBuffer *p) {
    p->size = 0;
    LobbyCharVectorBuffer_Deallocate(p);
    return p;
}

void LobbyCharVectorBuffer_Allocate(LobbyCharVectorBuffer *p, u32 n, LobbyTag tag) {
    p->data = LobbyMem_Alloc(n);
    p->capacity.value = n;
}

static LobbyCharVectorBuffer *LobbyCharVectorBuffer_Deallocate(LobbyCharVectorBuffer *p) {
    if (p->data) {
        u32 old = p->size;

        struct {
            volatile u8 zero;
        } init = { 0 };

        p->size -= old;
        if (p->data) {
            DWC_Free(0, p->data, 0);
        }
    }
    return p;
}

void LobbyStringVectorBuffer_Allocate(LobbyStringVectorBuffer *p, u32 n, LobbyTag tag) {
    if (n > 0x15555555) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    p->data = LobbyMem_Alloc(n * sizeof(Str));
    p->capacity.value = n;
}

LobbyStringVectorBuffer *LobbyStringVectorBuffer_Deallocate(LobbyStringVectorBuffer *p) {
    if (p->data) {
        u32 current = p->size;
        LobbyTag t = { 0 };
        LobbyStringVectorBuffer_EraseAtEnd(p, current, t, 0);
        if (p->data) {
            DWC_Free(0, p->data, 0);
        }
    }
    return p;
}
