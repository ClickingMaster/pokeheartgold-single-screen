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

int ov08_0221C318(Ov08Ctx *work) {
    switch (ov08_0221D4B0(work)) {
    case 0:
        PlaySE(0x5DD);
        ov08_022220AC(work, 7);
        if (ov08_0221D91C(work) == 1) {
            return 0x19;
        }
        W8(work, 0x2079, 0xF);
        return 0x16;
    case 1:
        if (ov08_0221DAC4(work) == 1) {
            break;
        }
        PlaySE(0x5DD);
        ov08_022220AC(work, 8);
        W8(work, 0x2079, 8);
        return 0x16;
    case 2:
        if (ov08_0221DAC4(work) == 1) {
            break;
        }
        PlaySE(0x5DD);
        ov08_022220AC(work, 10);
        W8(work, 0x2079, 9);
        return 0x16;
    case 3:
        PlaySE(0x5DD);
        ov08_022220AC(work, 6);
        W8(work, 0x2079, 6);
        return 0x16;
    }
    return 2;
}

int ov08_0221C3C8(Ov08Ctx *work_) {
    typedef struct Owner38 {
        u8 pad00[0x11];
        u8 battler;
    } Owner38;
    typedef struct Work38 {
        Owner38 *owner;
        u8 pad04[0x2075];
        u8 state;
        u8 pad207A[0x12];
        u8 mode;
    } Work38;
    Work38 *work = (Work38 *)work_;
    int choice;
    switch (ov08_0221D4F8(work_)) {
    case 0:
        choice = ov08_0221D614(work_, work->owner->battler, -1);
        if (choice == 0xFF) {
            break;
        }
        work->owner->battler = choice;
        PlaySE(0x5DD);
        ov08_022220AC(work_, 0xC);
        work->state = 0xE;
        return 0x16;
    case 1:
        choice = ov08_0221D614(work_, work->owner->battler, 1);
        if (choice == 0xFF) {
            break;
        }
        work->owner->battler = choice;
        PlaySE(0x5DD);
        ov08_022220AC(work_, 0xD);
        work->state = 0xE;
        return 0x16;
    case 2:
        PlaySE(0x5DD);
        ov08_022220AC(work_, 0xB);
        work->state = 9;
        return 0x16;
    case 3:
        PlaySE(0x5DD);
        ov08_022220AC(work_, 6);
        work->mode = 1;
        work->state = 7;
        return 0x16;
    }
    return 3;
}

int ov08_0221C488(Ov08Ctx *work_) {
    typedef struct Owner48 {
        u8 pad00[0x11];
        u8 battler;
        u8 pad12[0x22];
        u8 selection;
    } Owner48;
    typedef struct Entry48 {
        u16 value;
        u8 pad02[6];
    } Entry48;
    typedef struct Work48 {
        Owner48 *owner;
        u8 pad04[0x30];
        Entry48 entry[6][10];
    } Work48;
    Work48 *work = (Work48 *)work_;
    int choice = ov08_0221D540(work_);
    int result;
    switch (choice) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (work->entry[work->owner->battler][choice].value == 0) {
            break;
        }
        PlaySE(0x5DD);
        ov08_022220AC(work_, (u8)(choice + 0xE));
        work->owner->selection = choice;
        W8(work_, 0x2079, 10);
        return 0x16;
    case 4:
        result = ov08_0221D614(work_, work->owner->battler, -1);
        if (result == 0xFF) {
            break;
        }
        work->owner->battler = result;
        PlaySE(0x5DD);
        ov08_022220AC(work_, 0xC);
        W8(work_, 0x2079, 0xE);
        return 0x16;
    case 5:
        result = ov08_0221D614(work_, work->owner->battler, 1);
        if (result == 0xFF) {
            break;
        }
        work->owner->battler = result;
        PlaySE(0x5DD);
        ov08_022220AC(work_, 0xD);
        W8(work_, 0x2079, 0xE);
        return 0x16;
    case 6:
        PlaySE(0x5DD);
        ov08_022220AC(work_, 9);
        W8(work_, 0x2079, 8);
        return 0x16;
    case 7:
        PlaySE(0x5DD);
        ov08_022220AC(work_, 6);
        W8(work_, 0x208C, 2);
        W8(work_, 0x2079, 7);
        return 0x16;
    }
    return 4;
}

int ov08_0221C58C(Ov08Ctx *work_) {
    typedef struct Owner58 {
        u8 pad00[0x11];
        u8 battler;
        u8 pad12[0x22];
        u8 selection;
    } Owner58;
    typedef struct Entry58 {
        u16 value;
        u8 pad02[6];
    } Entry58;
    typedef struct Work58 {
        Owner58 *owner;
        u8 pad04[0x30];
        Entry58 entry[6][10];
    } Work58;
    Work58 *work = (Work58 *)work_;
    int choice = ov08_0221D588(work_);
    switch (choice) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (work->owner->selection != choice) {
            if (work->entry[work->owner->battler][choice].value == 0) {
                break;
            }
        }
        PlaySE(0x5DD);
        work->owner->selection = choice;
        return 10;
    case 4:
        PlaySE(0x5DD);
        ov08_022220AC(work_, 6);
        W8(work_, 0x2079, 9);
        return 0x16;
    }
    return 5;
}


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
extern void ov08_02224B90(void *, int);
extern int ov08_0221DB24(void *, u32);
extern void ov08_0222171C(Ov08Ctx *, int);
extern void ov08_0221D6CC(Ov08Ctx *, int);
extern u8 PaletteData_BeginPaletteFade(void *, u16, u16, s8, u8, u8, u16);
extern void *ov08_02224B64(int);
int ov08_0221C048(Ov08Ctx *);
