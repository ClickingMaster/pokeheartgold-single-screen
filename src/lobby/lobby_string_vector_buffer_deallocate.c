#include "global.h"

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void LobbyStringVectorBuffer_EraseAtEnd(LobbyStringVectorBuffer *, u32, LobbyTag, u32);
void DWC_Free(u32, void *, u32);

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
