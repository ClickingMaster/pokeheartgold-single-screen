#include "global.h"

#include "heap.h"
void Heap_Free(void *);
void Sprite_DeleteAndFreeResources(void *);
void SpriteSystem_DrawSprites(void *);
void ManagedSprite_SetDrawFlag(void *, int);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ManagedSprite_SetAffineOverwriteMode(void *, int);
int Pokepic_GetAttr(void *, int);
void Pokepic_SetAttr(void *, int, int);
void BattleAnimSystem_DestroyTask(void *, void *);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
int ov07_02223CE4(void *);
int ov07_02223D28(void *);
void ov07_02223CAC(void *, s16, s16);
void *BattleAnimUtil_AllocWork(void *, int);
u16 BattleAnimSystem_GetAttacker(void *);
u16 BattleAnimSystem_GetDefender(void *);
int ov07_02222004(void *, int);
void *BattleAnimSystem_GetBattlerPokepic(void *, int);
void ov07_02222508(void *, int, int, int, int);
int ov07_02222748(void *, void *);
int ov07_02222240(void *, int, int, void *);
int ov07_02222440(void *);
void ov07_022223F0(void *, int, int, int);
void ov07_02223A1C(void *, void *);
void ov07_02223B70(void *, void *);
int BattleAnimSystem_GetHeapID(void *);

extern u32 BattleAnimSystem_GetArg(void *p, int i);
void ov07_02222A44(void *, int, int);
u16 BattleAnimSystem_GetAttacker(void *);
u16 BattleAnimSystem_GetDefender(void *);
int BattleAnimUtil_GetPartnerBattler(void *, int);
void *BattleAnimSystem_GetBattlerPokepic(void *, int);
void ov07_02223BC0(void *);
