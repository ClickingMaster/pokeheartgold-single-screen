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

int BattlePartyMenu_HandleMonMenuInput(Ov08Ctx *work) {
    switch (BattlePartyMenu_GetMonMenuInput(work)) {
    case 0:
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 7);
        if (BattlePartyMenu_CanSwitchToSelectedMon(work) == 1) {
            return 0x19;
        }
        W8(work, 0x2079, 0xF);
        return 0x16;
    case 1:
        if (BattlePartyMenu_IsSelectedMonEgg(work) == 1) {
            break;
        }
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 8);
        W8(work, 0x2079, 8);
        return 0x16;
    case 2:
        if (BattlePartyMenu_IsSelectedMonEgg(work) == 1) {
            break;
        }
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 10);
        W8(work, 0x2079, 9);
        return 0x16;
    case 3:
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 6);
        W8(work, 0x2079, 6);
        return 0x16;
    }
    return 2;
}

int BattlePartyMenu_HandleSummaryInput(Ov08Ctx *work_) {
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
    switch (BattlePartyMenu_GetSummaryInput(work_)) {
    case 0:
        choice = BattlePartyMenu_FindNextMon(work_, work->owner->battler, -1);
        if (choice == 0xFF) {
            break;
        }
        work->owner->battler = choice;
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work_, 0xC);
        work->state = 0xE;
        return 0x16;
    case 1:
        choice = BattlePartyMenu_FindNextMon(work_, work->owner->battler, 1);
        if (choice == 0xFF) {
            break;
        }
        work->owner->battler = choice;
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work_, 0xD);
        work->state = 0xE;
        return 0x16;
    case 2:
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work_, 0xB);
        work->state = 9;
        return 0x16;
    case 3:
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work_, 6);
        work->mode = 1;
        work->state = 7;
        return 0x16;
    }
    return 3;
}

int BattlePartyMenu_HandleCheckMovesInput(Ov08Ctx *work_) {
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
    int choice = BattlePartyMenu_GetCheckMovesInput(work_);
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
        BattlePartyMenu_StartButtonAnim(work_, (u8)(choice + 0xE));
        work->owner->selection = choice;
        W8(work_, 0x2079, 10);
        return 0x16;
    case 4:
        result = BattlePartyMenu_FindNextMon(work_, work->owner->battler, -1);
        if (result == 0xFF) {
            break;
        }
        work->owner->battler = result;
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work_, 0xC);
        W8(work_, 0x2079, 0xE);
        return 0x16;
    case 5:
        result = BattlePartyMenu_FindNextMon(work_, work->owner->battler, 1);
        if (result == 0xFF) {
            break;
        }
        work->owner->battler = result;
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work_, 0xD);
        W8(work_, 0x2079, 0xE);
        return 0x16;
    case 6:
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work_, 9);
        W8(work_, 0x2079, 8);
        return 0x16;
    case 7:
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work_, 6);
        W8(work_, 0x208C, 2);
        W8(work_, 0x2079, 7);
        return 0x16;
    }
    return 4;
}

int BattlePartyMenu_HandleMoveDetailInput(Ov08Ctx *work_) {
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
    int choice = BattlePartyMenu_GetMoveDetailInput(work_);
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
        BattlePartyMenu_StartButtonAnim(work_, 6);
        W8(work_, 0x2079, 9);
        return 0x16;
    }
    return 5;
}


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
