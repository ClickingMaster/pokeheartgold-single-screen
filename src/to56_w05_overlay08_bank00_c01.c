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
int BattlePartyMenu_Create(void *owner_) {
    typedef struct OwnerBE {
        u8 pad00[8];
        void *battleSystem;
        u32 heapId;
        u8 pad10;
        u8 battler;
        u8 pad12[0x16];
        int slot;
    } OwnerBE;
    typedef struct EnvBE {
        OwnerBE *owner;
        u8 pad04[0x1E0];
        void *bgConfig;
        void *palette;
        u8 pad1EC[0x1E8A];
        u8 battler;
        u8 low : 4;
        u8 high : 4;
        u8 state;
        u8 pad2079[0x16];
        u8 slot;
    } EnvBE;
    OwnerBE *owner = owner_;
    void *task;
    EnvBE *env;
    if (owner->battler > 5) {
        owner->battler = 0;
    }
    task = CreateSysTaskAndEnvironment(BattlePartyMenu_Task, sizeof(EnvBE), 0, owner->heapId);
    env = SysTask_GetData(task);
    memset(env, 0, sizeof(EnvBE));
    env->owner = owner;
    env->bgConfig = BattleSystem_GetBgConfig(owner->battleSystem);
    env->palette = BattleSystem_GetPaletteData(owner->battleSystem);
    env->state = 0;
    env->battler = owner->battler;
    env->high = 0;
    env->slot = ov12_0223AB0C(owner->battleSystem, owner->slot);
}

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
