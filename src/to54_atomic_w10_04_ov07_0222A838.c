#include "global.h"

#include "bg_window.h"
#include "pokepic.h"
#include "sprite_system.h"

typedef struct W10Work {
    u8 pad00[0x14];
    void *owner14;
    u8 pad18[0x8];
    SpriteManager *sprites20;
    u8 pad24[0x14];
    ManagedSprite *sprite38;
    u8 pad3C[0x98];
    s32 rotationD4;
} W10Work;

BOOL BattleAnimUtil_UpdateLinearSinMotionSprite(void *a, void *b, ManagedSprite *sprite);
BOOL BattleAnimUtil_UpdateValueMotion(void *a);
void BattleAnimSystem_DestroyTask(void *owner, int arg);
void BattleAnimUtil_FreeWork(void *work);
void ov07_0222928C(int arg0, W10Work *work);

typedef struct W10EndWork {
    u8 pad00[8];
    void *owner08;
    u8 pad0C[0x14];
    ManagedSprite *sprite20;
    ManagedSprite *sprite24;
    u8 pad28[0x2C];
} W10EndWork;

void *BattleAnimUtil_AllocWork(void *owner, u32 size);
void BattleAnimUtil_InitCommonWork(void *owner, void *work);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(void *owner, int index);
void BattleAnimUtil_StartBlendAlphaMotion(void *work, int a, int b, int c, int d, int e);
void BattleAnimBackground_LoadBattleBgToLayerDefault(void *owner, int arg);
void BattleAnimSystem_CreateTaskDefaultPriority(void *owner, void (*callback)(int, void *), void *work);
void ov07_0222A710(int arg0, void *work);
void ov07_0222A838(void *owner);

void ov07_0222A838(void *owner) {
    W10EndWork *work = BattleAnimUtil_AllocWork(owner, sizeof(W10EndWork));
    BattleAnimUtil_InitCommonWork(owner, (u8 *)work + 4);
    work->sprite20 = BattleAnimSystem_GetBattlerSprite(work->owner08, 0);
    ManagedSprite_SetOamMode(work->sprite20, GX_OAM_MODE_OBJWND);
    *(volatile u32 *)0x04000000 = (*(volatile u32 *)0x04000000 & 0xFFFF1FFF) | (0x04000000U >> 11);
    *(volatile u16 *)0x0400004A = (*(volatile u16 *)0x0400004A & ~0x3F) | 0x1B;
    *(volatile u16 *)0x0400004A = (*(volatile u16 *)0x0400004A & ~0x3F00) | 0x1700;
    work->sprite24 = BattleAnimSystem_GetBattlerSprite(work->owner08, 1);
    ManagedSprite_SetOamMode(work->sprite24, GX_OAM_MODE_XLU);
    BattleAnimUtil_StartBlendAlphaMotion((u8 *)work + 0x2C, 0x10, 2, 2, 0x10, 0x10);
    BattleAnimBackground_LoadBattleBgToLayerDefault(work->owner08, 2);
    ToggleBgLayer(2, 1);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner08, ov07_0222A710, work);
}

void ov07_0222A4B4(int arg0, void *arg1);

Pokepic *BattleAnimSystem_GetBattlerPokepic(void *owner, int index);
int BattleAnimSystem_GetAttacker(void *owner);
BOOL BattleAnimUtil_IsBlendAlphaMotionDone(void *work);
int ov07_0221FAF8(void *owner, int index);
int BattleAnimUtil_PackXY(int a, int b);
int BattleAnimSystem_GetHeapID(void *owner);
void *BattleAnimScanline_NewWave(int a, int b, int c, int d, int e, int f, int g, int h, int i);
void BattleAnimScanline_FreeWave(void *arg);
void BattleAnimBackground_ResetLayer(void *owner, int arg);
void ov07_0222A710(int arg0, void *arg1);
