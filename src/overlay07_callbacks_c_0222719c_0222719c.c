#include "sys_task.h"
#include "global.h"

typedef struct CallbackWork {
    u8 pad00[4];
    void *owner;
    u8 pad08[8];
    void *spriteSystem;
    u8 pad14[8];
    void *sprite;
    u8 pad20[0x10];
    void *object30;
} CallbackWork;

typedef struct Work272D0 {
    u8 pad00[4];
    void *value04;
    u8 nested08[4];
    void *task0C;
} Work272D0;

typedef struct Work25444 {
    u8 state;
    u8 pad01[3];
    void *owner;
    u8 pad08[0x18];
    s16 x;
    s16 y;
    void *pokepic;
    u8 pad28[8];
    u8 movement[0x10];
} Work25444;

typedef struct Work2719C {
    u8 pad00[4];
    void *owner;
    u8 pad08[4];
    void *spriteSystem;
    u8 pad10[0x0C];
    void *sprites[4];
    s32 timer;
} Work2719C;


extern u32 BattleAnimSystem_GetArg(void *p, int i);
extern void BattleAnimSystem_ApplyGrayscale(void *);
extern void BattleAnimSystem_RestoreFromGrayscale(void *);
extern int BattleAnimUtil_IsPaletteBlendActive(void *);
extern void BattleAnimUtil_FreePaletteBlend(void *);
extern void BattleAnimSystem_DestroyTask(void *, void *);
extern void Heap_Free(void *);
extern int ManagedSprite_IsAnimated(void *);
extern void Sprite_DeleteAndFreeResources(void *);
extern void ManagedSprite_TickFrame(void *);
extern void SpriteSystem_DrawSprites(void *);
extern void *BattleAnimUtil_AllocWork(void *, int);
extern void BattleAnimUtil_InitCommonWork(void *, void *);
extern void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
extern void ov07_02227248(void *, void *);
extern int BattleAnimUtil_UpdateSinMotionPokepic(void *, s16, s16, void *);
extern int BattleAnimUtil_UpdateLinearMotion(void *);
extern void Pokepic_SetAttr(void *, int, int);
extern void *BattleAnimSystem_GetAttacker(void *);
extern void *BattleAnimSystem_GetDefender(void *);
extern void *BattleAnimUtil_GetPartnerBattler(void *, void *);
extern void GF_AssertFail(void);

void ov07_02226C90(void *arg);
void BattleAnimTask_WaitPaletteBlend(void *task, CallbackWork *work);
void BattleAnimTask_PlaySpriteAnimOnce(void *task, CallbackWork *work);
void BattleAnimTask_PlaySpriteAnimOnce2(void *task, CallbackWork *work);
void ov07_022272D0(void *arg);
void BattleAnimTask_MoveBattlerSin(SysTask *task, void *data);
void BattleAnimTask_MoveBattlerLinear(void *task, Work25444 *work);
void *BattleAnimUtil_GetRoleBattler(void *arg, int kind);
void ov07_0222719C(void *task, Work2719C *work);

void ov07_0222719C(void *task, Work2719C *work) {
    int i;

    if (work->timer == 0) {
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
    work->timer--;
    for (i = 0; i < 4; i++) {
        if (work->sprites[i] != NULL) {
            ManagedSprite_TickFrame(work->sprites[i]);
        }
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}
