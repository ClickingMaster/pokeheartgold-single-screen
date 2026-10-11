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

extern int BattlePartyMenu_GetMonMenuInput(Ov08Ctx *);
extern int BattlePartyMenu_GetSummaryInput(Ov08Ctx *);
extern int BattlePartyMenu_GetCheckMovesInput(Ov08Ctx *);
extern int BattlePartyMenu_GetMoveDetailInput(Ov08Ctx *);
extern int BattlePartyMenu_FindNextMon(Ov08Ctx *, int, int);
extern int BattlePartyMenu_CanSwitchToSelectedMon(void *);
extern int BattlePartyMenu_IsSelectedMonEgg(Ov08Ctx *);
extern void BattlePartyMenu_StartButtonAnim(Ov08Ctx *, int);
extern void PlaySE(int);
int BattlePartyMenu_HandleMonMenuInput(Ov08Ctx *);
int BattlePartyMenu_HandleSummaryInput(Ov08Ctx *);
int BattlePartyMenu_HandleCheckMovesInput(Ov08Ctx *);
int BattlePartyMenu_HandleMoveDetailInput(Ov08Ctx *);

extern u32 PaletteData_GetSelectedBuffersBitmask(void *);
extern int BattlePartyMenu_HandlePartyListSelection(Ov08Ctx *);
extern int BattlePartyMenu_TryUseItemOnMon(Ov08Ctx *);
int BattlePartyMenu_HandlePartyListInput(Ov08Ctx *);

extern void *CreateSysTaskAndEnvironment(void *, u32, u32, u32);
extern void *SysTask_GetData(void *);
extern void *BattleSystem_GetBgConfig(void *);
extern void *BattleSystem_GetPaletteData(void *);
extern u8 ov12_0223AB0C(void *, int);
extern void *memset(void *, int, u32);
extern void BattlePartyMenu_Task(void *, void *);
int BattlePartyMenu_Create(void *);

extern void BattlePartyMenu_LoadMonData(Ov08Ctx *);
extern void BattlePartyMenu_InitBgs(Ov08Ctx *);
extern void BattlePartyMenu_LoadBgGraphics(Ov08Ctx *);
extern void BattlePartyMenu_InitText(Ov08Ctx *);
extern void FontID_Alloc(int, int);
extern void BattlePartyMenu_LoadPageTilemaps(Ov08Ctx *, int);
extern void BattlePartyMenu_DrawPageButtons(Ov08Ctx *, int);
extern void BattlePartyMenu_LoadPagePalette(Ov08Ctx *, int);
extern void BattlePartyMenu_InitSprites(Ov08Ctx *);
extern void BattlePartyMenu_ShowPageSprites(Ov08Ctx *, int);
extern void BattlePartyMenu_AddWindows(Ov08Ctx *);
extern void BattlePartyMenu_PrintPageText(Ov08Ctx *, int);
extern void BattleDpadCursor_SetActive(void *, int);
extern int BattlePartyMenu_IsPartnerMon(void *, u32);
extern void BattlePartyMenu_SetPageCursor(Ov08Ctx *, int);
extern void BattlePartyMenu_DrawExpBar(Ov08Ctx *, int);
extern u8 PaletteData_BeginPaletteFade(void *, u16, u16, s8, u8, u8, u16);
extern void *BattleDpadCursor_New(int);
int BattlePartyMenu_Setup(Ov08Ctx *);
int BattlePartyMenu_Setup(Ov08Ctx *work_) {
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
    BattlePartyMenu_LoadMonData(work_);
    BattlePartyMenu_InitBgs(work_);
    BattlePartyMenu_LoadBgGraphics(work_);
    BattlePartyMenu_InitText(work_);
    FontID_Alloc(4, work->owner->heap);
    BattlePartyMenu_LoadPageTilemaps(work_, work->variant);
    BattlePartyMenu_DrawPageButtons(work_, work->variant);
    BattlePartyMenu_LoadPagePalette(work_, work->variant);
    BattlePartyMenu_InitSprites(work_);
    BattlePartyMenu_ShowPageSprites(work_, work->variant);
    BattlePartyMenu_AddWindows(work_);
    BattlePartyMenu_PrintPageText(work_, work->variant);
    if (work->owner->flag != 0) {
        BattleDpadCursor_SetActive(work->small, 1);
    }
    if (work->variant == 0 && BattlePartyMenu_IsPartnerMon(work_, 0) == 1) {
        work->owner->battler = 1;
    }
    BattlePartyMenu_SetPageCursor(work_, work->variant);
    BattlePartyMenu_DrawExpBar(work_, work->variant);
    PaletteData_BeginPaletteFade(work->palette, 10, 0xFFFF, -8, 0x10, 0, 0);
    return ret;
}

int BattlePartyMenu_HandlePartyListInput(Ov08Ctx *work_) {
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
    if (BattlePartyMenu_HandlePartyListSelection(work_) == 1) {
        if (work->owner->battler == 6) {
            if (work->owner->mode != 1) {
                PlaySE(0x5DD);
                BattlePartyMenu_StartButtonAnim(work_, 6);
                return 0x19;
            }
        } else {
            PlaySE(0x5DD);
            BattlePartyMenu_StartButtonAnim(work_, work->owner->battler);
            if (work->owner->mode == 2) {
                return BattlePartyMenu_TryUseItemOnMon(work_);
            }
            work->state = 7;
            return 0x16;
        }
    }
    return 1;
}
