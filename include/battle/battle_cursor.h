#ifndef POKEHEARTGOLD_BATTLE_CURSOR_H
#define POKEHEARTGOLD_BATTLE_CURSOR_H

#include "battle/battle.h"

#include "sys_task_api.h"

// typedef'd in battle.h
struct BattleCursorPosition {
    s8 commandX;
    s8 commandY;
    s8 moveX;
    s8 moveY;
    s8 targetX;
    s8 targetY;
    s8 monTargetType;
    u8 unk_07[1];
};

typedef struct BattleCursor {
    void *sprites[5];
    SysTask *task;
} BattleCursor;

void BattleCursor_LoadResources(SpriteSystem *renderer, SpriteManager *gfxHandler, PaletteData *plttData, enum HeapID heapID, u32 character, u32 pal, u32 cell, u32 animation);
void BattleCursor_FreeResources(SpriteManager *gfxHandler, u32 character, u32 pal, u32 cell, u32 animation);
BattleCursor *BattleCursor_New(SpriteSystem *renderer, SpriteManager *gfxHandler, enum HeapID heapID, u32 character, u32 pal, u32 cell, u32 animation, u32 a7, u32 a8);
void BattleCursor_Delete(BattleCursor *cursor);
void BattleCursor_SetCornersWithOffset(BattleCursor *cursor, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, fx32 a9);
void BattleCursor_SetCorners(BattleCursor *cursor, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
void BattleCursor_SetRect(BattleCursor *cursor, int left, int right, int top, int bottom);
void BattleCursor_SetRectWithOffset(BattleCursor *cursor, int left, int right, int top, int bottom, fx32 a5);
void BattleCursor_ShowExtraSpriteWithOffset(BattleCursor *cursor, int x, int y, int a3, fx32 a4);
void BattleCursor_Disable(BattleCursor *cursor);
void BattleCursor_HideExtraSprite(BattleCursor *cursor);

#endif
