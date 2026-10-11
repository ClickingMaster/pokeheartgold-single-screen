#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct AlignmentProbe {
    u32 prefix;
    u64 value;
} AlignmentProbe;

typedef char WordAlignedU64[(sizeof(AlignmentProbe) == 12) ? 1 : -1];

typedef struct Body {
    u64 a, b;
    u32 c, d;
} Body;

typedef struct Record {
    int type;
    Body payload;
} Record;

void ov45_02232AE8(Record *, int, Body *);

void ov45_02232AE8(Record *p, int type, Body *q) {
    u8 *base = (u8 *)&p->payload;
    p->type = type;
    *(u64 *)base = q->a;
    *(u64 *)(base + 8) = q->b;
    *(u32 *)(base + 16) = q->c;
    *(u32 *)(base + 20) = q->d;
}
