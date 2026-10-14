#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Vec {
    u32 *ptr;
    u32 size, cap;
};

struct Tag {};

extern "C" void DWC_Free(int, void *, u32);
extern "C" void LobbyU32Vector_Allocate(Vec *v, u32 n, Tag t);

struct Alloc {
    void release(u32 *p) {
        if (p) {
            DWC_Free(0, p, 0);
        }
    }
};

inline void Allocate(Vec *v, u32 n, Tag t) {
    LobbyU32Vector_Allocate(v, n, t);
}

extern "C" void LobbyU32Vector_Reallocate(Vec *v, u32 size, u32 cap);

extern "C" void LobbyU32Vector_Reallocate(Vec *v, u32 size, u32 cap) {
    u32 n = v->size;
    Alloc a = Alloc();
    v->size -= n;
    if (v->ptr) {
        a.release(v->ptr);
        v->ptr = 0;
        v->cap = 0;
    }
    LobbyU32Vector_Allocate(v, cap, Tag(Tag()));
}
