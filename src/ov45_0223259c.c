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

extern void *ov45_02237890(Vec *, u32);
extern void ov45_02237820(Vec *, u32, void *);

typedef struct {
    u8 *p;
    u32 extra;
} Iterator;

void ov45_022327B8(Vec *, ...);
void *ov45_0223259C(Vec *);
void ov45_02232760(Vec *, u8 *, u8 *, int);
void LobbyTimerMap_MakeEntry(Record *, u32, Six *);

void *ov45_0223259C(Vec *v) {
    return v->len == 0 ? NULL : v->data;
}
