#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 wmspW[];

typedef struct {
    u8 pad[0x54c];
    u8 *iface;
    u8 *state;
} To90W37015Global;

#define G       ((To90W37015Global *)(wmspW + 0x1000))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);

typedef struct {
    u32 mask;
    u16 freq, min, max, parent, child, interval, vcount, poll1, poll2, retry;
    u8 single, ignore, fatal, mode;
} To90W37015Params;

extern void WMSP_SetParentSize(u16);
extern void WMSP_SetChildSize(u16);
u32 WMSP_SetMPParameterCore(To90W37015Params *, To90W37015Params *);

u32 WMSP_SetMPParameterCore(To90W37015Params *input, To90W37015Params *old) {
    u32 irq;
    u32 result = 0;
    u32 mask;
    u8 *state = G->state;
    mask = input->mask;
    if ((u16)(H(state, 0) + (u16)-9) <= 1 && (mask & 0x2c00)) {
        mask &= ~0x2c00;
        result = 3;
    }
    irq = __VENEER_OS_DisableInterrupts();
    if (old) {
        old->mask = 0x3fff;
        old->freq = H(state, 0x58);
        old->min = H(state, 0x5a);
        old->max = H(state, 0x5c);
        old->parent = H(state, 0x30);
        old->child = H(state, 0x32);
        old->interval = H(state, 0x44);
        old->vcount = H(state, 0x46);
        old->poll1 = H(state, 0x40);
        old->poll2 = H(state, 0x42);
        old->retry = H(state, 0x98);
        old->single = H(state, 0x92);
        old->ignore = H(state, 0x94);
        old->fatal = H(state, 0x9a);
        old->mode = H(state, 0x9c);
    }
    if (mask & 1) {
        u16 value = input->freq;
        if (value == 0) {
            value = 16;
        }
        H(state, 0x58) = value;
        H(state, 0x7c0) = value;
    }
    if (mask & 2) {
        u16 value = input->min;
        if (value == 0) {
            value = 16;
        }
        H(state, 0x5a) = value;
        H(state, 0x7c2) = value;
        if (*(s16 *)(state + 0x62) > (s32)value) {
            H(state, 0x62) = value;
        }
    }
    if (mask & 4) {
        u16 value = input->max;
        if (value == 0) {
            value = 16;
        }
        H(state, 0x5c) = value;
        H(state, 0x7c4) = value;
        if (*(s16 *)(state + 0x62) > (s32)value) {
            H(state, 0x62) = value;
        }
    }
    if (mask & 8) {
        u16 value = input->parent;
        if (H(state, 0x34) < (u16)((value + 1) & ~1)) {
            result = 6;
        } else {
            WMSP_SetParentSize(value);
        }
    }
    if (mask & 16) {
        u16 value = input->child;
        if ((u16)((value + 1) & ~1) > 512) {
            result = 6;
        } else {
            WMSP_SetChildSize(value);
        }
    }
    if (mask & 32) {
        u32 value = input->interval;
        if (value > 10000) {
            result = 6;
        } else {
            H(state, 0x44) = value;
            *(u64 *)(state + 0x48) = ((u64)value * 0x82ea / 64) / 1024;
        }
    }
    if (mask & 64) {
        u32 value = input->vcount;
        if (value > 10000) {
            result = 6;
        } else {
            H(state, 0x46) = value;
            *(u64 *)(state + 0x50) = ((u64)value * 0x82ea / 64) / 1024;
        }
    }
    if (mask & 128) {
        u32 value = input->poll1;
        if (value <= 190 || (value >= 220 && value <= 262)) {
            H(state, 0x40) = value;
        } else {
            result = 6;
        }
    }
    if (mask & 256) {
        u32 value = input->poll2;
        if (value <= 190 || (value >= 220 && value <= 262)) {
            H(state, 0x42) = value;
        } else {
            result = 6;
        }
    }
    if (mask & 512) {
        u32 value = input->retry;
        H(state, 0x98) = value;
        H(state, 0x7ca) = value;
    }
    if (mask & 1024) {
        H(state, 0x92) = input->single;
    }
    if (mask & 2048) {
        H(state, 0x94) = input->ignore;
    }
    if (mask & 4096) {
        H(state, 0x9a) = input->fatal;
        H(state, 0x7cc) = H(state, 0x9a);
    }
    if (mask & 8192) {
        H(state, 0x9c) = input->mode;
    }
    __VENEER_OS_RestoreInterrupts(irq);
    return result;
}
