#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define U(p, n) (*(unsigned *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

void *LobbyU32VectorBuffer_Deallocate(void *p);

SplitBuf *LobbyU32VectorBuffer_Destroy(SplitBuf *p) {
    W(p, 4) = 0;
    LobbyU32VectorBuffer_Deallocate(p);
    return p;
}

void LobbyU32VectorBuffer_Allocate(SplitBuf *p, u32 n, Tag unused) {
    if (n > 0x3fffffff) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    P(p, 0) = LobbyMem_Alloc(n * 4);
    U(p, 8) = n;
}
