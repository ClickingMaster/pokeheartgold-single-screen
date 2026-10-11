#ifndef POKEHEARTGOLD_OVERLAY40_TRAINER_CARD_RESOURCES_R19_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY40_TRAINER_CARD_RESOURCES_R19_PRIVATE_H

#include "global.h"

#include "sprite_system.h"

#define PTR(p, off) (*(void **)((u8 *)(p) + (off)))

int BattleRecord_Exists(void);
void BattleRecord_Delete(void);
void BattleRecord_Load(void *saveData, int heapId, int *result, void *setup, int value);
void *BattleRecord_CopyHeader(int heapId);
void *BattleRecord_CopyProfile(int heapId);
void *FrontierCard_New(int heapId);
void FrontierCard_InitFromSave(void *a, void *b);
u32 sub_02074490(void);
u32 sub_0207449C(void);
u32 sub_020744A8(void);

void ov40_0222FCCC(void *p);
void ov40_0222FDC4(void *p);
void ov40_0222FE00(void *p);
void ov40_0222FE68(void *p);

#endif
