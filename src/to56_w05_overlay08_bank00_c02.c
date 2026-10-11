#include "global.h"

#pragma force_active on

typedef struct Ov08Ctx {
    u8 raw[0x2090];
} Ov08Ctx;
static inline void W8(Ov08Ctx *p, u32 o, u8 v) {
    p->raw[o] = v;
}
static inline u8 R8(Ov08Ctx *p, u32 o) {
    return p->raw[o];
}
static inline void *RP(Ov08Ctx *p, u32 o) {
    return *(void **)&p->raw[o];
}

extern int ov08_0221D4B0(Ov08Ctx *);
extern int ov08_0221D4F8(Ov08Ctx *);
extern int ov08_0221D540(Ov08Ctx *);
extern int ov08_0221D588(Ov08Ctx *);
extern int ov08_0221D614(Ov08Ctx *, int, int);
extern int ov08_0221D91C(void *);
extern int ov08_0221DAC4(Ov08Ctx *);
extern void ov08_022220AC(Ov08Ctx *, int);
extern void PlaySE(int);
int ov08_0221C318(Ov08Ctx *);
int ov08_0221C3C8(Ov08Ctx *);
int ov08_0221C488(Ov08Ctx *);
int ov08_0221C58C(Ov08Ctx *);

extern u32 PaletteData_GetSelectedBuffersBitmask(void *);
extern int ov08_0221D438(Ov08Ctx *);
extern int ov08_0221C1C8(Ov08Ctx *);
int ov08_0221C14C(Ov08Ctx *);

extern void *CreateSysTaskAndEnvironment(void *, u32, u32, u32);
extern void *SysTask_GetData(void *);
extern void *BattleSystem_GetBgConfig(void *);
extern void *BattleSystem_GetPaletteData(void *);
extern u8 ov12_0223AB0C(void *, int);
extern void *memset(void *, int, u32);
extern void ov08_0221BE98(void *, void *);
int ov08_0221BE20(void *);

extern void ov08_0221D184(Ov08Ctx *);
extern void ov08_0221CDF8(Ov08Ctx *);
extern void ov08_0221CF38(Ov08Ctx *);
extern void ov08_0221D0F4(Ov08Ctx *);
extern void FontID_Alloc(int, int);
extern void ov08_0221D8B0(Ov08Ctx *, int);
extern void ov08_022221CC(Ov08Ctx *, int);
extern void ov08_02222524(Ov08Ctx *, int);
extern void ov08_022205E0(Ov08Ctx *);
extern void ov08_02220C5C(Ov08Ctx *, int);
extern void ov08_0221DC00(Ov08Ctx *);
extern void ov08_0221DD70(Ov08Ctx *, int);
extern void BattleDpadCursor_SetActive(void *, int);
extern int ov08_0221DB24(void *, u32);
extern void ov08_0222171C(Ov08Ctx *, int);
extern void ov08_0221D6CC(Ov08Ctx *, int);
extern u8 PaletteData_BeginPaletteFade(void *, u16, u16, s8, u8, u8, u16);
extern void *BattleDpadCursor_New(int);
int ov08_0221C048(Ov08Ctx *);
int ov08_0221C048(Ov08Ctx *work_) {
    typedef struct Owner048 {
        u8 pad00[8];
        void *battleSystem;
        int heap;
        u8 pad10;
        u8 battler;
        u8 pad12[0x20];
        u8 flag;
        u8 pad33[2];
        u8 mode;
    } Owner048;
    typedef struct Work048 {
        Owner048 *owner;
        u8 pad04[0x1E4];
        void *palette;
        u8 pad1EC[0x1E8E];
        u8 variant;
        u8 pad207B[0xD];
        void *small;
    } Work048;
    Work048 *work = (Work048 *)work_;
    int ret;
    *(vu16 *)0x04001050 = 0;
    if (work->owner->mode == 3) {
        work->variant = 6;
        ret = 0x13;
    } else {
        work->variant = 0;
        ret = 1;
    }
    work->small = BattleDpadCursor_New(work->owner->heap);
    ov08_0221D184(work_);
    ov08_0221CDF8(work_);
    ov08_0221CF38(work_);
    ov08_0221D0F4(work_);
    FontID_Alloc(4, work->owner->heap);
    ov08_0221D8B0(work_, work->variant);
    ov08_022221CC(work_, work->variant);
    ov08_02222524(work_, work->variant);
    ov08_022205E0(work_);
    ov08_02220C5C(work_, work->variant);
    ov08_0221DC00(work_);
    ov08_0221DD70(work_, work->variant);
    if (work->owner->flag != 0) {
        BattleDpadCursor_SetActive(work->small, 1);
    }
    if (work->variant == 0 && ov08_0221DB24(work_, 0) == 1) {
        work->owner->battler = 1;
    }
    ov08_0222171C(work_, work->variant);
    ov08_0221D6CC(work_, work->variant);
    PaletteData_BeginPaletteFade(work->palette, 10, 0xFFFF, -8, 0x10, 0, 0);
    return ret;
}

int ov08_0221C14C(Ov08Ctx *work_) {
    typedef struct Owner14 {
        u8 pad00[0x11];
        u8 battler;
        u8 pad12[0x22];
        u8 selected;
        u8 mode;
    } Owner14;
    typedef struct Work14 {
        Owner14 *owner;
        u8 pad04[0x1E4];
        void *palette;
        u8 pad1EC[0x1E8D];
        u8 state;
    } Work14;
    Work14 *work = (Work14 *)work_;
    if (PaletteData_GetSelectedBuffersBitmask(work->palette) != 0) {
        return 1;
    }
    if (ov08_0221D438(work_) == 1) {
        if (work->owner->battler == 6) {
            if (work->owner->mode != 1) {
                PlaySE(0x5DD);
                ov08_022220AC(work_, 6);
                return 0x19;
            }
        } else {
            PlaySE(0x5DD);
            ov08_022220AC(work_, work->owner->battler);
            if (work->owner->mode == 2) {
                return ov08_0221C1C8(work_);
            }
            work->state = 7;
            return 0x16;
        }
    }
    return 1;
}
