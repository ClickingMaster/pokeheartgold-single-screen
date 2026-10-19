#include <nitro/types.h>

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" void *memmove(void *, const void *, unsigned);

class Alloc {};

class TrueType {};

extern "C" void LobbyCharVector_Allocate(LobbyCharVector *, u32, Alloc);
extern "C" void LobbyCharVector_InitRange(LobbyCharVector *, char *, char *);

inline void CopyRange(char *first, char *last, char *out, TrueType) {
    memmove(out, first, last - first);
}

#pragma opt_common_subs off

extern "C" void LobbyCharVector_InitRange(LobbyCharVector *v, char *first, char *last) {
    if (last - first != 0) {
        LobbyCharVector_Allocate(v, last - first, Alloc());
        CopyRange(first, last, v->data + v->size, TrueType());
        v->size += last - first;
    }
}

#pragma opt_common_subs reset
