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
extern void ov07_02222FC4(void *);
extern void ov07_02222FF4(void *);
extern int ov07_02222EE8(void *);
extern void ov07_02222EF8(void *);
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
extern int ov07_02222240(void *, s16, s16, void *);
extern int ov07_022222B4(void *);
extern void Pokepic_SetAttr(void *, int, int);
extern void *BattleAnimSystem_GetAttacker(void *);
extern void *BattleAnimSystem_GetDefender(void *);
extern void *BattleAnimUtil_GetPartnerBattler(void *, void *);
extern void GF_AssertFail(void);

void ov07_02226C90(void *arg);
void ov07_02225810(void *task, CallbackWork *work);
void ov07_02226804(void *task, CallbackWork *work);
void ov07_02226A68(void *task, CallbackWork *work);
void ov07_022272D0(void *arg);
void ov07_02225444(SysTask *task, void *data);
void ov07_02225504(void *task, Work25444 *work);
void *ov07_02225AE0(void *arg, int kind);
void ov07_0222719C(void *task, Work2719C *work);

void ov07_02226A68(void *task, CallbackWork *work) {
    if (ManagedSprite_IsAnimated(work->sprite) == 0) {
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
    } else {
        ManagedSprite_TickFrame(work->sprite);
        SpriteSystem_DrawSprites(work->spriteSystem);
    }
}
