#ifndef TO57_NEW35_H
#define TO57_NEW35_H
#include "global.h"

#include "pokepic.h"
#include "sprite.h"

typedef struct Motion24 {
    s16 x;
    s16 y;
    u8 raw[0x20];
} Motion24;
typedef struct Scalar14 {
    int value;
    u8 raw[0x10];
} Scalar14;
typedef struct W35 {
    void *owner;
    void *spriteSystem;
    int state;
    int counter;
    ManagedSprite *sprite;
    Motion24 anim;
    Scalar14 scalar;
    int mirror;
} W35;
int BattleAnimSystem_GetHeapID(void *);

void *BattleAnimSystem_GetBattlerSpriteManager(void *);
int ov07_0221BFC0(void *);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(void *, int);
void BattleAnimUtil_InitScaleMotion(void *, s16, int, s16, int);
void BattleAnimUtil_InitValueMotionFx(void *, int, int, int);
void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void ov07_0222CBFC(int, W35 *);
void ov07_0222CE10(void *);
BOOL BattleAnimUtil_UpdateValueMotionFx(void *);
BOOL BattleAnimUtil_UpdateScaleMotion(void *);
void BattleAnimUtil_GetScaleFloat(void *, f32 *, f32 *);
void BattleAnimSystem_DestroyTask(void *, int);
void SpriteSystem_DrawSprites(void *);
void ov07_0222CBFC(int, W35 *);
void ManagedSprite_SetDrawFlag(ManagedSprite *, int);
void ManagedSprite_SetOamMode(ManagedSprite *, int);
void ManagedSprite_SetAffineOverwriteMode(ManagedSprite *, int);
void ManagedSprite_SetAffineScale(ManagedSprite *, f32, f32);
void ManagedSprite_SetDrawPriority(ManagedSprite *, int);
void ManagedSprite_SetPriority(ManagedSprite *, int);

typedef struct W35Fade {
    void *owner;
    int state;
    Pokepic *pic;
    ManagedSprite *sprite;
    int special;
    Motion24 motion;
    s16 x;
    s16 y;
    int delta;
} W35Fade;
int BattleAnimSystem_GetDefender(void *);
Pokepic *BattleAnimSystem_GetBattlerPokepic(void *, void *);
int ov07_02222004(void *, int);
void BattleAnimUtil_InitShake(void *, int, int, int, int);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(void *, int);
int BattleAnimSystem_GetAttacker(void *);
int BattleAnimUtil_GetBattlerType(void *, int);
int ov07_02221FF0(ManagedSprite *);
void *BattleAnimSystem_GetPaletteData(void *);
int BattleAnimSystem_IsBattlerSemiInvulnerable(void *, int);
void ov07_0222CCBC(int, W35Fade *);
void BattleAnimUtil_UpdateShake(void *);
void ov07_022220B8(void *, Pokepic *, s16, s16);
void ov07_0222207C(void *, ManagedSprite *, s16, s16);
BOOL Pokepic_ResumePaletteFade(Pokepic *);
void ManagedSprite_SetPositionXY(ManagedSprite *, s16, s16);
void ov07_0222CEC0(void *);
void PaletteData_BeginPaletteFade(void *, int, int, int, int, int, int);
#endif
