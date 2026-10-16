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

typedef struct {
    u8 *p;
    u32 extra;
} Iterator;

void *LobbyU32Vector_Data2(Vec *);

void *LobbyU32Vector_Data2(Vec *v) {
    return v->len == 0 ? NULL : v->data;
}
