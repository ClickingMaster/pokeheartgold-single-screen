#ifndef POKEHEARTGOLD_OVERLAY_12_0226ADE0_H
#define POKEHEARTGOLD_OVERLAY_12_0226ADE0_H

#include "battle/battle.h"

typedef struct PartyGaugeArrow {
    ManagedSprite *managedSprite;
    SysTask *task;
    u32 hideType;
    u32 side;
    u32 position;
    s32 x;
    s16 alpha;
    u8 state;
    u8 delay;
} PartyGaugeArrow;
    
typedef struct {
    ManagedSprite *managedSprite;
    SysTask *task;
    u32 side;
    u32 position;
    u32 hideType;
    s8 *pokeballCount;
    s16 *arrowAlpha;
    s32 xStart;
    s32 xEnd;
    s32 xOverflow;
    s16 delay;
    u16 sdatID;
    u8 state;
    u8 ballSlot;
    u8 flipAnimation;
    u8 startDelay;
} PartyGaugePokeballs;
    
typedef struct PartyGauge {
    PartyGaugeArrow arrow;
    PartyGaugePokeballs pokeballs[6];
    s8 numBalls;
} PartyGauge;

void ov12_0226ADE0(SpriteSystem *spriteSystem, SpriteManager *spriteManager, PaletteData  *paletteData);
void ov12_0226AE78(SpriteManager *spriteManager);
PartyGauge *ov12_0226AEE0(u8 *a0, s32 a1, s32 a2, s32 a3, SpriteSystem *arg4, SpriteManager *arg5);
s32 ov12_0226AF48(PartyGauge* a0);
void ov12_0226AF6C(PartyGauge *a0, s32 a1, s32 a2);
u32 ov12_0226AFA4(PartyGauge* a0);
void ov12_0226AFC8(PartyGauge* a0);
void ov12_0226B098(SysTask *task, void *data);
void ov12_0226B144(PartyGaugeArrow *arrow, u32 type);
void ov12_0226B180(SysTask *task, void *data);
void ov12_0226B29C(PartyGaugePokeballs *pokeballs, s8 *numBalls, u32 side, u32 type, u32 pos, int slot, int frame, SpriteSystem *spriteSystem, SpriteManager *spriteManager);
void ov12_0226B3B0(SysTask *task, void *data);
void ov12_0226B5B0(SysTask *task, void *data);
void ov12_0226B694(PartyGaugePokeballs *pokeballs, int slot, u32 type, s16 *arrowAlpha);
void ov12_0226B6F8(SysTask *task, void *data);
void ov12_0226B82C(SysTask *task, void *data);
s32 ov12_0226B884(u32 arg0, u32 arg1);
u32 ov12_0226B8C4(u32 a0);

#endif