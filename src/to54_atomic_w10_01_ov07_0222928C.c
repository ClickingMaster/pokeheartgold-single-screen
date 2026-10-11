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

BOOL ov07_022223CC(void *a, void *b, ManagedSprite *sprite);
BOOL ov07_02222440(void *a);
void BattleAnimSystem_DestroyTask(void *owner, int arg);
void BattleAnimUtil_FreeWork(void *work);
void ov07_0222928C(int arg0, W10Work *work);

void ov07_0222928C(int arg0, W10Work *work) {
    if (ov07_022223CC((u8 *)work + 0x8C, (u8 *)work + 0xB0, work->sprite38) == 0) {
        Sprite_DeleteAndFreeResources(work->sprite38);
        BattleAnimSystem_DestroyTask(work->owner14, arg0);
        BattleAnimUtil_FreeWork(work);
        return;
    }
    if (ov07_02222440(&work->rotationD4) == 1) {
        ManagedSprite_SetAffineZRotation(work->sprite38, (u16)work->rotationD4);
    }
    ManagedSprite_TickFrame(work->sprite38);
    SpriteSystem_DrawSprites(work->sprites20);
}

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
void ov07_02222AC4(void *work, int a, int b, int c, int d, int e);
void BattleAnimBackground_LoadBattleBgToLayerDefault(void *owner, int arg);
void BattleAnimSystem_CreateTaskDefaultPriority(void *owner, void (*callback)(int, void *), void *work);
void ov07_0222A710(int arg0, void *work);
void ov07_0222A838(void *owner);

void ov07_0222A4B4(int arg0, void *arg1);

Pokepic *BattleAnimSystem_GetBattlerPokepic(void *owner, int index);
int BattleAnimSystem_GetAttacker(void *owner);
BOOL ov07_02222AF4(void *work);
int ov07_0221FAF8(void *owner, int index);
int ov07_02222D88(int a, int b);
int BattleAnimSystem_GetHeapID(void *owner);
void *ov07_02222CCC(int a, int b, int c, int d, int e, int f, int g, int h, int i);
void ov07_02222D3C(void *arg);
void BattleAnimBackground_ResetLayer(void *owner, int arg);
void ov07_0222A710(int arg0, void *arg1);
