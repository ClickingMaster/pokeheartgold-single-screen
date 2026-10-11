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

void ov07_0222A710(int arg0, void *arg1) {
    W10EndWork *work = arg1;
    int a, b;
    switch (((u8 *)work)[4]) {
    case 0:
        Pokepic_SetAttr(BattleAnimSystem_GetBattlerPokepic(work->owner08, BattleAnimSystem_GetAttacker(work->owner08)), 6, 1);
        ((u8 *)work)[4]++;
        break;
    case 1:
        if (ov07_02222AF4((u8 *)work + 0x2C)) {
            *(int *)work = 0;
            a = ov07_0221FAF8(work->owner08, 1);
            b = ov07_02222D88(0, 0);
            *(void **)((u8 *)work + 0x28) = ov07_02222CCC(0, 0xA0, 0x16C, 0xC000, 0xC8, a, 0, b, BattleAnimSystem_GetHeapID(work->owner08));
            ((u8 *)work)[4]++;
        }
        break;
    case 2:
        (*(int *)work)++;
        if (*(int *)work >= 0x78) {
            ov07_02222D3C(*(void **)((u8 *)work + 0x28));
            ov07_02222AC4((u8 *)work + 0x2C, 2, 0x10, 0x10, 2, 0x10);
            ((u8 *)work)[4]++;
        }
        break;
    case 3:
        if (ov07_02222AF4((u8 *)work + 0x2C)) {
            Pokepic_SetAttr(BattleAnimSystem_GetBattlerPokepic(work->owner08, BattleAnimSystem_GetAttacker(work->owner08)), 6, 0);
            ((u8 *)work)[4]++;
        }
        break;
    default:
        *(volatile u32 *)0x04000000 &= 0xFFFF1FFF;
        BattleAnimBackground_ResetLayer(work->owner08, 2);
        BattleAnimSystem_DestroyTask(work->owner08, arg0);
        Heap_Free(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprite24);
    ManagedSprite_TickFrame(work->sprite20);
    SpriteSystem_DrawSprites(*(SpriteManager **)((u8 *)work + 0x10));
}
