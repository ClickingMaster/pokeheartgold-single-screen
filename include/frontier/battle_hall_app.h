#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_HALL_APP_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_HALL_APP_H

#include "bg_window.h"
#include "heap.h"
#include "overlay_manager.h"
#include "pokemon.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "yes_no_prompt.h"

typedef struct BattleHallSprite {
    u8 unk_00[0xC];
    const u8 *anim;
    Sprite *sprite;
} BattleHallSprite;

typedef struct BattleHallSpriteGfx {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObj[2][4];
} BattleHallSpriteGfx;

BOOL BattleHallApp_Init(OverlayManager *man, int *state);
BOOL BattleHallApp_Main(OverlayManager *man, int *state);
BOOL BattleHallApp_Exit(OverlayManager *man, int *state);

void BattleHallSpriteGfx_Init(BattleHallSpriteGfx *gfx, Pokemon *mon);
void BattleHallSpriteGfx_Free(BattleHallSpriteGfx *gfx);
BattleHallSprite *BattleHallSprite_Create(BattleHallSpriteGfx *gfx, u32 resourceId, u32 animSeq, s32 x, u16 y, const u8 *anim);
u32 BattleHallSprite_Delete(BattleHallSprite *sprite);
void BattleHallSprite_SetVisible(BattleHallSprite *sprite, BOOL visible);
void BattleHallSprite_SetPosition(BattleHallSprite *sprite, s32 x, s32 y);
void BattleHallSprite_SetAnim(BattleHallSprite *sprite, u32 animSeq);
void BattleHallSprite_SetIconPalette(BattleHallSprite *sprite, Pokemon *mon);

void BattleHallApp_AddWindows(BgConfig *bgConfig, Window *windows);
void BattleHallApp_RemoveWindows(Window *windows);
void BattleHallApp_DrawMessageFrame(Window *window, u32 frame);
YesNoPrompt *BattleHallApp_CreateYesNo(enum HeapID heapId);
void BattleHallApp_DestroyYesNo(YesNoPrompt *prompt);
void BattleHallApp_InitYesNo(YesNoPrompt *prompt, BgConfig *bgConfig, u8 ignoreTouch);
u32 BattleHallApp_HandleYesNo(YesNoPrompt *prompt);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_HALL_APP_H
