#include <stdarg.h>

#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 *data;
    u32 len, cap;
} Vec;

extern void *LobbyCharVector_RecommendSize(Vec *, u32);
extern void LobbyCharVector_Reallocate(Vec *, u32, void *);

void LobbyCharVector_AssignRange(Vec *, u8 *, u8 *, int);

void LobbyCharVector_AssignRange(Vec *v, u8 *first, u8 *last, int unused) {
    u32 len = last - first;
    if (len > v->cap) {
        volatile u8 tag = 0;
        LobbyCharVector_Reallocate(v, len, LobbyCharVector_RecommendSize(v, len - v->cap));
    }
    memmove(v->data, first, (u32)last - (u32)first);
    v->len = len;
}
