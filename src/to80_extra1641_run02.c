#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0xe6];
    u8 kind : 2;
    u8 mode : 6;
    u8 gap[14];
    u8 flag5, flag6;
} Context;

Context *ov13_02239FF0(u32, void *);
void ov13_02234314(const void *, void *);
void ov13_02234468(const void *, void *);
void ov13_02233E8C(u32, void *);

void ov13_02233E8C(u32 type, void *out) {
    Context *c = ov13_02239FF0(type, out);
    switch (type) {
    case 0:
        ov13_02234314((u8 *)c + 0x40, out);
        return;
    case 1: {
        int len;
        u8 buf[33];
        switch (c->kind) {
        case 0:
            return;
        case 1:
            len = 10;
            break;
        case 2:
            len = 26;
            break;
        case 3:
            len = 32;
            break;
        }
        if (c->mode == 1) {
            len /= 2;
        }
        MI_CpuFill8(buf, 0, 33);
        memset(buf, 42, len);
        ov13_02234314(buf, out);
        return;
    }
    case 3:
        if (c->flag5) {
            return;
        }
        ov13_02234468((u8 *)c + 0xc0, out);
        return;
    case 4:
        if (c->flag5) {
            return;
        }
        ov13_02234468((u8 *)c + 0xf0, out);
        return;
    case 5:
        if (c->flag5) {
            return;
        }
        ov13_02234468((u8 *)c + 0xc4, out);
        return;
    case 7:
        if (c->flag6) {
            return;
        }
        ov13_02234468((u8 *)c + 0xc8, out);
        return;
    case 8:
        if (c->flag6) {
            return;
        }
        ov13_02234468((u8 *)c + 0xcc, out);
    }
}
