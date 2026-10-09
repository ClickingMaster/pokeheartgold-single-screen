#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 pad; u8 nameLen; u16 name[10]; } MbUser;
typedef struct { u16 dma; u16 pad; u32 pad4; u8 *work; u8 *parent; } MbGlobal;
extern MbGlobal ov13_0224CF40;
extern u16 WM_GetNextTgid(void);
extern u16 MBi_GetBeaconPeriodDispersion(void);
int MB_Init(void *work, const MbUser *user, u32 ggid, u32 tgid, u32 dma);
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
u16 ov13_0224599C[10] = {'m', 'u', 'l', 't', 'i', 'b', 'o', 'o', 't', 0};
u16 ov13_0224598E = 40;
u16 ov13_02245992 = 0xffff;
u16 ov13_02245990 = 40;
u16 ov13_0224598C = 5;
u32 ov13_02245998 = 1;
// @fn MB_Init
int MB_Init(void *work, const MbUser *user, u32 ggid, u32 tgid, u32 dma) {
    static u16 *name = ov13_0224599C;
    u8 *w;
    u8 *p;
    OSIntrMode e;
    int i;
    u16 *dst;
    if (ov13_0224CF40.parent != NULL && H(ov13_0224CF40.parent, 0x1316) != 0) {
        return 2;
    }
    w = (u8 *)(((u32)work + 0x1f) & ~0x1f);
    p = (u8 *)(((u32)w + 0x1f + 0x1e00) & ~0x1f);
    if (tgid == 0x10000) {
        tgid = WM_GetNextTgid();
    }
    e = OS_DisableInterrupts();
    ov13_02245992 = 0xffff;
    ov13_0224598C = 5;
    ov13_0224598E = 40;
    ov13_02245990 = 40;
    ov13_02245998 = 1;
    ov13_0224CF40.dma = dma;
    ov13_0224CF40.work = w;
    ov13_0224CF40.parent = p;
    MIi_CpuClear32(0, (u32 *)w, 0x1e00);
    MIi_CpuClear16(0, (u16 *)p, 0x1340);
    dst = (u16 *)(w + 0x530);
    for (i = 0; i < user->nameLen; i++) {
        *dst++ = user->name[i];
    }
    dst = (u16 *)(w + 0x538);
    for (i = 0; i < 16; i++) {
        u16 c = *name;
        if (c == 0) {
            break;
        }
        name++;
        *dst++ = c;
    }
    MI_CpuCopy8(user, p + 0x1300, sizeof(MbUser));
    if (user->nameLen < 10) {
        ((MbUser *)(p + 0x1300))->name[user->nameLen] = 0;
    }
    H(w, 0x500) = 0x100;
    H(w, 0x502) = 8;
    H(w, 0x518) = 0;
    H(w, 0x51a) = 0;
    H(w, 0x52c) = 1;
    W(w, 0x504) = (u32)(p + 0x400);
    H(w, 0xe) = 0;
    H(w, 0x12) = 0;
    H(w, 0x16) = 1;
    H(w, 0x14) = 0;
    W(w, 8) = ggid;
    H(w, 0xc) = tgid;
    H(w, 0x18) = MBi_GetBeaconPeriodDispersion() + 200;
    H(w, 0x10) = 15;
    w[0x50c] = 0;
    w[0x50d] = 0;
    H(p, 0x1316) = 1;
    W(p, 0x131c) = 0;
    OS_RestoreInterrupts(e);
    return 0;
}
