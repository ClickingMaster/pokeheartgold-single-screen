#include <nitro/types.h>

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};

extern "C" void DWC_Free(int, void *, u32);
extern "C" void LobbyU32Vector_Allocate(LobbyU32Vector *v, u32 n, Tag t);

struct Alloc {
    void release(u32 *p) {
        if (p) {
            DWC_Free(0, p, 0);
        }
    }
};

extern "C" void LobbyU32Vector_Reallocate(LobbyU32Vector *v, u32 size, u32 cap);

extern "C" void LobbyU32Vector_Reallocate(LobbyU32Vector *v, u32 size, u32 cap) {
    u32 n = v->size;
    Alloc a = Alloc();
    v->size -= n;
    if (v->data) {
        a.release(v->data);
        v->data = 0;
        v->capacity = 0;
    }
    LobbyU32Vector_Allocate(v, cap, Tag(Tag()));
}
