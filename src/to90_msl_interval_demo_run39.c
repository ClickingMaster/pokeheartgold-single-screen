#pragma force_active on
#include "global.h"
#pragma force_active on

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad0[0x14];
    u32 current, frame;
    u8 pad1c[0x38 - 0x1c];
    u32 stacked;
    u8 pad3c[0x48 - 0x3c];
    u32 unstacked;
    u8 pad4c[0x60 - 0x4c];
    u32 decoded, extra;
    u16 size;
    u8 flag20, flag40, flag80;
} Context;

typedef struct {
    u32 pad;
    u8 *info;
} Exception;

extern u8 *_Z22__DecodeUnsignedNumberPcPm(u8 *, u32 *);
void _Z16__SetupFrameInfoP12ThrowContextP13ExceptionInfo(Context *, Exception *);

void _Z16__SetupFrameInfoP12ThrowContextP13ExceptionInfo(Context *c, Exception *e) {
    u8 *src = e->info;
    u32 flags = src[0], forty = flags & 64, eighty;
    c->flag40 = forty != 0;
    c->flag20 = (flags & 32) != 0;
    eighty = flags & 128;
    c->flag80 = eighty != 0;
    c->size = src[1] << 4;
    c->size |= 0x4000;
    src = _Z22__DecodeUnsignedNumberPcPm(src + 2, &c->decoded);
    if (forty) {
        src = _Z22__DecodeUnsignedNumberPcPm(src, &c->extra);
    }
    if (forty) {
        if (eighty) {
            c->frame = c->stacked;
        } else {
            c->frame = c->unstacked;
        }
    } else {
        c->frame = c->current;
    }
}
