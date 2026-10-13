#ifndef POKEHEARTGOLD_BATTLE_FINGER_H
#define POKEHEARTGOLD_BATTLE_FINGER_H

#include "battle/battle.h"

#include "sys_task_api.h"

// This file handles the finger that comes up during the catching tutorial

typedef struct BattleFinger {
    void *sprite;
    SysTask *task;
    int x;
    int y;
    int angle;
    fx32 yOffset;
    s16 delay;
    u8 touchRequest;
    u8 touchOccurrence;
    u8 touchAnimationFlag;
    u8 touchAnimationState;
    u8 touchAnimationDelay;
} BattleFinger;

void BattleFinger_LoadResources(SpriteSystem *renderer, SpriteManager *gfxHandler, enum HeapID heapID, PaletteData *plttData, u32 character, u32 pal, u32 cell, u32 animation);
void BattleFinger_FreeResources(SpriteManager *gfxHandler, u32 character, u32 pal, u32 cell, u32 animation);
BattleFinger *BattleFinger_New(SpriteSystem *renderer, SpriteManager *gfxHandler, enum HeapID heapID, u32 character, u32 pal, u32 cell, u32 animation, u32 spritePriority, u32 bgPriority);
void BattleFinger_Delete(BattleFinger *finger);
void BattleFinger_EnableWithOffset(BattleFinger *finger, int x, int y, fx32 yOffset);
void BattleFinger_Enable(BattleFinger *finger, int x, int y);
void BattleFinger_Disable(BattleFinger *finger);
void BattleFinger_SetTouchDelay(BattleFinger *finger, int delay);
BOOL BattleFinger_HasTouched(BattleFinger *finger);

#endif
