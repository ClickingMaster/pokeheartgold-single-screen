#include "global.h"

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

extern void *LobbyMem_Alloc(u32);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
#include "overlay45_string_search.h"

void LobbyStringVectorBuffer_Allocate(LobbyStringVectorBuffer *p, u32 n, LobbyTag tag) {
    if (n > 0x15555555) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    p->data = LobbyMem_Alloc(n * sizeof(Str));
    p->capacity.value = n;
}
