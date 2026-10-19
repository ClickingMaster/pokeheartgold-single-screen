#include <nitro/types.h>

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};

extern "C" void *memmove(void *, const void *, u32);
extern "C" u32 LobbyU32Vector_RecommendSize(LobbyU32Vector *v, u32 n);
extern "C" void LobbyU32Vector_Reallocate(LobbyU32Vector *v, u32 n, u32 cap);

struct Rec {
    u32 recommend(LobbyU32Vector *v, u32 n) {
        return LobbyU32Vector_RecommendSize(v, n);
    }
};

inline void Copy(u32 *f, u32 *l, u32 *d) {
    memmove(d, f, (l - f) * sizeof(u32));
}

extern "C" void LobbyU32Vector_AssignRange(LobbyU32Vector *v, u32 *first, u32 *last);
#pragma opt_common_subs off

extern "C" void LobbyU32Vector_AssignRange(LobbyU32Vector *v, u32 *first, u32 *last) {
    u32 n = last - first;
    u32 cap = v->capacity;
    if (cap < n) {
        Rec r = Rec();
        LobbyU32Vector_Reallocate(v, n, r.recommend(v, n - cap));
    }
    Copy(first, last, v->data);
    v->size = n;
}

#pragma opt_common_subs reset
