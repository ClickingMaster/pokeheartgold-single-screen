#ifndef TO56_R2_W13_OV07_PRIVATE_H
#define TO56_R2_W13_OV07_PRIVATE_H
#include "global.h"
typedef struct R2W13Transition {
    void *owner;
    int state;
    u8 pad08[8];
    void *pokepic;
    u8 pad14[0x34];
    void *sprite;
} R2W13Transition;
void ov07_0222D050(void *, R2W13Transition *);
BOOL ov07_0222CFCC(R2W13Transition *);
BOOL ManagedSprite_GetDrawFlag(void *);
int BattleAnimSystem_GetDefender(void *);
int BattleAnimSystem_IsBattlerSemiInvulnerable(void *, int);
void Pokepic_SetAttr(void *, int, int);
int BattleAnimSystem_GetBgLayer(void *, int);
int BattleAnimSystem_GetBgPriority(void *, int);
void SetBgPriority(int, int);
int ov07_0221FAE8(void *);
void *BattleAnimSystem_GetHeapID(void *);
void BG_ClearCharDataRange(int, int, int, void *);
void *BattleAnimSystem_GetBgConfig(void *);
void BgClearTilemapBufferAndCommit(void *, int);
void BattleAnimSystem_DestroyTask(void *, void *);
void Heap_Free(void *);

typedef struct R2W13Spin {
    void *owner;
    void *arg1;
    int direction;
    int state;
    void *sprite;
    u8 scaleAnim[0x24];
    u8 translationAnim[0x14];
    int timer;
    int x;
    int sign;
} R2W13Spin;
void ov07_0222D3BC(void *, void *, int, void *);
void *BattleAnimUtil_AllocWork(void *, int);
int BattleAnimSystem_GetAttacker(void *);
int ov07_02222004(void *, int);
int ov07_0221FB78(void *, int);
int ov07_02221F80(void *, int, int);
void ManagedSprite_SetAffineOverwriteMode(void *, int);
void ManagedSprite_SetAffineScale(void *, f32, f32);
void ManagedSprite_SetPositionXY(void *, int, int);
void ManagedSprite_OffsetPositionXY(void *, s16, s16);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ManagedSprite_SetAffineTranslation(void *, s16, s16);
int BattleAnimUtil_GetBattlerSide(void *, int);
void ManagedSprite_SetAnim(void *, int);
void BattleAnimUtil_InitScaleMotion(void *, int, int, int, int);
void BattleAnimUtil_InitValueMotion(void *, int, int, int);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void ov07_0222D2B0(void *, R2W13Spin *);
#endif
