#include "global.h"
void *BattleAnimUtil_AllocWork(void *, int);
void BattleAnimUtil_InitCommonWork(void *, void *);

extern u32 BattleAnimSystem_GetArg(void *p, int i);
void ManagedSprite_OffsetPositionXY(void *, int, int);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void ov07_02226804(void);
void ov07_02226838(void *, int, int, void *);

void ov07_02226838(void *context, int ignored1, int ignored2, void *sprite) {
    u8 *work = BattleAnimUtil_AllocWork(context, 0x20);
    BattleAnimUtil_InitCommonWork(context, work);
    *(void **)(work + 0x1C) = sprite;
    s16 x = ((s32)BattleAnimSystem_GetArg(context, 0));
    s16 y = ((s32)BattleAnimSystem_GetArg(context, 1));
    ManagedSprite_OffsetPositionXY(sprite, x, y);
    BattleAnimSystem_CreateTaskDefaultPriority(*(void **)(work + 4), (void *)ov07_02226804, work);
}
