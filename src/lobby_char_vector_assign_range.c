#pragma force_active on
#include <stdarg.h>

#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 *data;
    u32 len, cap;
} Vec;

typedef struct {
    u32 a, b;
} Pair;

typedef struct {
    Pair first, second;
    u32 e, f;
} Six;

typedef struct {
    u32 type;
    Six value;
} Record;

extern void *LobbyCharVector_RecommendSize(Vec *, u32);
extern void LobbyCharVector_Reallocate(Vec *, u32, void *);

typedef struct {
    u8 *p;
    u32 extra;
} Iterator;

void LobbyCharVector_AssignIterRange(Vec *, ...);
void *LobbyU32Vector_Data2(Vec *);
void LobbyCharVector_AssignRange(Vec *, u8 *, u8 *, int);
void LobbyTimerMap_MakeEntry(Record *, u32, Six *);

void LobbyCharVector_AssignRange(Vec *v, u8 *first, u8 *last, int unused) {
    u32 len = last - first;
    if (len > v->cap) {
        volatile u8 tag = 0;
        LobbyCharVector_Reallocate(v, len, LobbyCharVector_RecommendSize(v, len - v->cap));
    }
    memmove(v->data, first, (u32)last - (u32)first);
    v->len = len;
}
