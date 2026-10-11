#ifndef TO55_T14_PRIVATE_H
#define TO55_T14_PRIVATE_H

#include "global.h"

typedef struct Ov07CallbackWork {
    void *owner;
    void *resource;
    s32 state;
    void *picture;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1C;
    s32 field20;
    s32 field24;
    s32 field28;
    s32 field2C;
    u32 field30;
} Ov07CallbackWork;

typedef struct Ov07FadeWork {
    void *owner;
    s32 state;
    s32 timer;
    s16 transition;
} Ov07FadeWork;

void *BattleAnimUtil_AllocWork(void *, u32);
void ov07_02230170(Ov07CallbackWork *);
void ov07_022301C4(Ov07CallbackWork *);
void ov07_022302A8(void *, Ov07CallbackWork *);
void ov07_022303A4(void *);
void ov07_0223049C(void *, Ov07FadeWork *);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void *BattleAnimSystem_GetDefender(void *);
void *BattleAnimSystem_GetBattlerPokepic(void *, void *);
int BattleAnimSystem_IsBattlerSemiInvulnerable(void *, void *);
u32 BattleAnimSystem_GetBgLayer(void *, int);
u32 BattleAnimSystem_GetBgPriority(void *, int);
int ov07_0221FAE8(void *);
void *ov07_0221FAF8(void *, int);
void *BattleAnimSystem_GetPaletteData(void *);
u32 BattleAnimSystem_GetBattleBgPaletteMask(void *);
void *BattleAnimSystem_GetHeapID(void *);
int BattleAnimUtil_GetBattlerType(void *, void *);
void *BattleAnimSystem_GetBgConfig(void *);
void *ov07_02222C84(void *);
void ov07_02222C98(void *);
void ov07_02222C60(void *);
u32 ov07_02222D88(u16, u16);
void *ov07_02222D90(void *);
void *ov07_02222BE4(void *, u32, void *);
BOOL ov07_022222B4(void *);
void ov07_02222268(void *, int, int, int, int, int);
void BattleAnimUtil_SetWindowInsidePlanes(void *, int, int);
int Pokepic_GetAttr(void *, int);
void Pokepic_SetAttr(void *, int, int);
void SetBgPriority(u8, u8);
void BG_ClearCharDataRange(u8, u32, u32, void *);
void BgClearTilemapBufferAndCommit(void *, u8);
void Heap_Free(void *);
void PaletteData_BeginPaletteFade(void *, int, u16, int, int, int, u16);
u32 PaletteData_GetSelectedBuffersBitmask(void *);

#endif
