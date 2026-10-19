#include "global.h"

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

extern void *LobbyMem_Alloc(u32);
extern int LobbyString_Destroy();
#include "overlay45_string_search.h"
u8 *LobbyCharVectorBuffer_Deallocate(u8 *p);

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
    LobbyCharVectorBuffer_Deallocate((u8 *)p);
    return p;
}

void LobbyCharVectorBuffer_Allocate(LobbyCharVectorBuffer *p, u32 n, LobbyTag tag) {
    p->data = LobbyMem_Alloc(n);
    p->capacity.value = n;
}
