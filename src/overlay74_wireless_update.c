#include "global.h"
#define U8AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
typedef struct FourFields {
    u32 a : 8;
    u32 b : 4;
    u32 c : 4;
    u32 d : 16;
} FourFields;
typedef struct InitConfig {
    FourFields fields;
    u16 *srcB;
    u16 *srcA;
} InitConfig;
typedef struct FieldPair {
    FourFields f40;
    FourFields f44;
} FieldPair;
typedef struct InitWork {
    u8 h0, h1, h2, h3;
    u32 h4;
    u32 h8;
    u16 hc;
    u8 pad0e[2];
    u32 x10, x14, x18, x1c, x20, x24, x28, x2c, x30, x34, x38, x3c;
    FieldPair params;
    u16 a48[12];
    u16 b60[12];
    FieldPair copy;
} InitWork;
typedef struct Entry {
    u32 a;
    u16 b;
    u16 index;
    u8 x;
    u8 count;
    u8 y;
    u8 z;
} Entry;
extern void *ov74_02231184(void);
extern void ov74_02231054(void);
extern void *ov74_0223105C(void);
extern u32 ov74_02230A14(void);
extern void ov74_02231194(void);
extern void WM_Init(void *, u32);
extern void WM_GetNextTgid(void);
extern void *ov74_02231154(void);
extern void *ov74_0223115C(void);
extern u32 ov74_02230A74(void);
extern u32 ov74_02230A7C(void);
extern u32 ov74_02231064(void);
extern u32 ov74_02231260(void);
extern void ov74_022312C0(u32, u32, u32, u32, u32);
extern void ov74_02230A4C(u32, u32, u32);
extern void ov74_02230BB4(void);
extern void ov74_02230C10(void);
void ov74_02230CCC(void);
void ov74_02230A84(InitConfig *, u32);
void ov74_02230C10(void);
void ov74_02230BB4(void);
void ov74_02230CEC(void);
void ov74_02230A84(InitConfig *cfg, u32 addr) {
    InitWork *work;
    u16 *srcA;
    u16 *srcB;
    u32 i;
    ov74_02231054();
    work = ov74_0223105C();
    if (addr & 0x1f) {
        addr += 0x20 - (addr & 0x1f);
    }
    work->h0 = 0;
    work->h1 = 0;
    work->h2 = 0;
    work->h3 = 4;
    work->h4 = 0x00400131;
    work->hc = 0;
    work->x34 = addr;
    addr += 0xf00;
    work->x38 = addr;
    work->x2c = ov74_02230A74();
    addr += work->x2c;
    work->x3c = addr;
    work->x30 = ov74_02230A7C();
    addr += work->x30;
    work->x28 = addr;
    work->x24 = addr + 0xc0;
    addr += 0xe0;
    WM_Init((void *)work->x34, 2);
    work->x10 = ov74_02230A14();
    work->x14 = 0;
    work->x18 = 0;
    work->x1c = 0;
    work->x20 = 0;
    work->params.f40.a = 15;
    work->params.f40.b = 1;
    work->params.f40.c = 1;
    work->params.f44.a = cfg->fields.a;
    work->params.f44.b = cfg->fields.b;
    work->params.f44.c = cfg->fields.c;
    work->params.f44.d = cfg->fields.d;
    srcA = cfg->srcA;
    srcB = cfg->srcB;
    for (i = 0; srcA != NULL && i < 12; i++, srcA++, srcB++) {
        work->a48[i] = *srcA;
        work->b60[i] = *srcB;
    }
    work->copy = work->params;
    ov74_02231194();
    U32AT(ov74_02231184(), 8) = addr;
    addr += work->x2c;
    U32AT(ov74_02231184(), 0xc) = addr;
    WM_GetNextTgid();
}
void ov74_02230BB4(void) {
    u8 *p = ov74_02231184();
    u32 v;
    if (U8AT(p, 0x19) == 1) {
        U8AT(p, 0x1a)
        --;
        if (U8AT(p, 0x1a) != 0) {
            return;
        }
        U8AT(p, 0x19) = 0;
    }
    if (U8AT(p, 0x19) == 0) {
        v = 0x40 - ov74_02231260();
        ov74_022312C0(U32AT(p, 8), U32AT(p, 4), v, 3, U8AT(p, 0x1c));
        ov74_02230A4C(U32AT(p, 8), v + ov74_02231260(), 0xffff);
        U8AT(p, 0x19) = 1;
        U8AT(p, 0x1a) = 0x3c;
    }
}
void ov74_02230C10(void) {
    u8 *p = ov74_02231184();
    if (U8AT(p, 0x19) != 0) {
        return;
    }
    if (U8AT(p, 0x1c) == 0xfd) {
        Entry *entries = ov74_0223115C();
        s32 i;
        for (i = 0; i < 8; i++) {
            if (entries[i].count != 0) {
                u16 mask = (u16)(1 << entries[i].index);
                ov74_022312C0(U32AT(p, 8), U32AT(p, 4), 0, 0, 0xfd);
                ov74_02230A4C(U32AT(p, 8), ov74_02231260(), mask);
                entries[i].count--;
                return;
            }
        }
        U8AT(p, 0x1a)
        --;
        if (U8AT(p, 0x1a) == 0) {
            U8AT(p, 0x19) = 2;
        }
        return;
    }
    ov74_02231260();
    ov74_02230A74();
    {
        u32 value = (U32AT(U32AT(p, 0xc), 8) << 16) >> 24;
        if (value == 0) {
            if (U8AT(p, 0x1b) == 0) {
                U8AT(p, 0x19) = 2;
                return;
            }
            U8AT(p, 0x1b)
            --;
        }
        ov74_022312C0(U32AT(p, 8), U32AT(p, 4), 0, value, U8AT(p, 0x1c));
        ov74_02230A4C(U32AT(p, 8), ov74_02231260(), 0xffff);
    }
}
void ov74_02230CCC(void) {
    switch (U8AT(ov74_02231184(), 0x18)) {
    case 1:
        ov74_02230BB4();
        break;
    case 2:
        ov74_02230C10();
        break;
    }
}
void ov74_02230CEC(void) {
    u8 *p = ov74_02231154();
    switch (ov74_02231064()) {
    case 6:
        if (U16AT(p, 0x1c0) != 0) {
            U16AT(p, 0x1c0)
            --;
        }
        break;
    case 11:
        ov74_02230CCC();
        break;
    }
}
