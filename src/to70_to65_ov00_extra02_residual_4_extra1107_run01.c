#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    u8 first, second, a, b;
    u32 value, zero0, zero1;
    u64 tick0, tick1;
} Init;

extern union {
    Init *ptr;
    u8 raw[32];
} ov00_0221A928;

extern u8 *ov00_021F955C(void);
extern void ov00_021F954C(int);
extern void ov00_021F956C(int);
extern void ov00_021F7AD8(void);
extern void ov00_021F381C(int);
extern void *ov00_021EC2A8(int, int);

typedef struct {
    u8 value;
    u8 pad[11];
} ByteColumn;

typedef struct {
    u16 value;
    u8 pad[10];
} HalfColumn;

typedef struct {
    char *value;
    u8 pad[8];
} NameColumn;

typedef struct {
    void *value;
    u8 pad[8];
} PtrColumn;

extern ByteColumn ov00_0221A948[];
extern ByteColumn ov00_0221A949[];
extern HalfColumn ov00_0221A94A[];
extern NameColumn ov00_0221A94C[];
extern PtrColumn ov00_0221A950[];
extern u32 ov00_021F7A98(void);
extern void ov00_021EC2EC(int, void *);
extern void ov00_021F2EE4(int, int);
extern void ov00_02212008(u32, const char *);
u32 ov00_021F13A4(u32, const char *, void *);

static inline void ClearEnd(int zero, u8 *raw) {
    raw[28] = zero;
    raw[29] = zero;
    *(u16 *)(raw + 30) = zero;
}

int ov00_021F14D4(int, Init *);
void ov00_021F1620(int, u32, u32, u32, u32, u32, u32, u32);

int ov00_021F14D4(int mode, Init *input) {
    if (ov00_021F955C() == NULL) {
        return 1;
    }
    if (input == NULL) {
        return 3;
    }
    switch (mode) {
    case 0:
        if (*(u32 *)(ov00_021F955C() + 0x1a0) == 19) {
            return 1;
        }
        if (input->first && input->second <= 1) {
            return 3;
        }
        if (ov00_0221A928.ptr == NULL) {
            ov00_0221A928.ptr = ov00_021EC2A8(4, 32);
            if (ov00_0221A928.ptr == NULL) {
                return 4;
            }
        }
        ov00_0221A928.ptr->first = input->first;
        ov00_0221A928.ptr->second = input->second;
        ov00_0221A928.ptr->a = 0;
        ov00_0221A928.ptr->b = 0;
        ov00_0221A928.ptr->value = input->value;
        ov00_0221A928.ptr->zero0 = 0;
        ov00_0221A928.ptr->zero1 = 0;
        ov00_0221A928.ptr->tick0 = OS_GetTick();
        ov00_0221A928.ptr->tick1 = OS_GetTick();
        return 0;
    case 1:
        if (*(u32 *)input) {
            ov00_0221A928.raw[28] = 1;
        } else {
            ov00_0221A928.raw[28] = 0;
        }
        ov00_0221A928.raw[29] = 0;
        return 0;
    default:
        return 2;
    }
}

void ov00_021F1620(int mode, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    ov00_021F954C(mode);
    *(u32 *)(ov00_021F955C()) = a;
    *(u32 *)(ov00_021F955C() + 4) = b;
    *(u32 *)(ov00_021F955C() + 8) = c;
    *(u32 *)(ov00_021F955C() + 0x10) = 0;
    *(u32 *)(ov00_021F955C() + 0x1c) = 0;
    *(u16 *)(ov00_021F955C() + 0x1a) = 0;
    *(u32 *)(ov00_021F955C() + 0xe4) = 0;
    ov00_021F956C(0);
    *(u8 *)(ov00_021F955C() + 0xf) = 0;
    *(u8 *)(ov00_021F955C() + 0x19) = 0;
    *(u8 *)(ov00_021F955C() + 0x1af) = 0;
    *(u16 *)(ov00_021F955C() + 0x1b6) = 0;
    *(u32 *)(ov00_021F955C() + 0x1dc) = 0;
    *(u32 *)(ov00_021F955C() + 0x200) = 0;
    *(u32 *)(ov00_021F955C() + 0x2f4) = d;
    *(u32 *)(ov00_021F955C() + 0x2f8) = e;
    *(u32 *)(ov00_021F955C() + 0x2fc) = f;
    *(u32 *)(ov00_021F955C() + 0x300) = g;
    MI_CpuFill8(ov00_021F955C() + 0x304, 0, 64);
    *(u32 *)(ov00_021F955C() + 0x344) = 0;
    *(u32 *)(ov00_021F955C() + 0x464) = 0;
    *(u32 *)(ov00_021F955C() + 0x468) = 0;
    *(u32 *)(ov00_021F955C() + 0x474) = 0;
    *(u32 *)(ov00_021F955C() + 0x478) = 0;
    *(u32 *)(ov00_021F955C() + 0x47c) = 0;
    *(u32 *)(ov00_021F955C() + 0x480) = 0;
    {
        u8 *w = ov00_021F955C();
        *(u32 *)(w + 0x174) = 0;
        *(u32 *)(w + 0x178) = 0;
    }
    ov00_021F7AD8();
    ClearEnd(0, ov00_0221A928.raw);
    ov00_021F381C(0);
}
