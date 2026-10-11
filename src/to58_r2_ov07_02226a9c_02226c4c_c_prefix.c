#include "global.h"
void *BattleAnimUtil_AllocWork(void *, int);
void BattleAnimUtil_InitCommonWork(void *, void *);
int BattleAnimSystem_GetDefender(void *);
int ov07_02221F80(void *, int, int);
int ov07_0221BFC0(void *);
int BattleAnimUtil_GetBattlerSide(void *, int);
void ManagedSprite_SetPositionXY(void *, int, int);
void ManagedSprite_SetAnim(void *, int);
int BattleAnimSystem_GetBgPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void ov07_02226A68(void);
void ov07_02226A9C(void *, int, int, void *);

void ov07_02226A9C(void *context, int ignored1, int ignored2, void *sprite) {
    u8 *work = BattleAnimUtil_AllocWork(context, 0x38);
    BattleAnimUtil_InitCommonWork(context, work);
    *(void **)(work + 0x1C) = sprite;
    int arg = BattleAnimSystem_GetDefender(context);
    int x = ov07_02221F80(context, arg, 0);
    ov07_02221F80(context, arg, 1);
    int anim;
    int y;
    if (ov07_0221BFC0(context) == 1) {
        x = 0x48;
        y = 0x20;
        anim = 0;
    } else if (BattleAnimUtil_GetBattlerSide(*(void **)(work + 4), arg) == 3) {
        y = 0x7E;
        anim = 1;
    } else {
        y = 0x20;
        anim = 0;
    }
    ManagedSprite_SetPositionXY(*(void **)(work + 0x1C), x, y);
    ManagedSprite_SetAnim(*(void **)(work + 0x1C), anim);
    int priority = BattleAnimSystem_GetBgPriority(*(void **)(work + 4), 2);
    ManagedSprite_SetPriority(*(void **)(work + 0x1C), priority);
    BattleAnimSystem_CreateTaskDefaultPriority(*(void **)(work + 4), (void *)ov07_02226A68, work);
}
