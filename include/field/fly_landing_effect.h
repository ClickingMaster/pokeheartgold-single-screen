#ifndef POKEHEARTGOLD_FIELD_FLY_LANDING_EFFECT_H
#define POKEHEARTGOLD_FIELD_FLY_LANDING_EFFECT_H

#include "field/field_effect_manager.h"
#include "field/sprite_gfx_manager.h"
#include "field_system.h"
#include "filesystem.h"
#include "sprite.h"

// Fly arrival (FlyLanding_Start's task): the flying Pokemon swoops in carrying the trainer sprite, drops them, flies off
// into the distance, then the trainer sprite is swapped for the player object. A field effect of the task's pool;
// its update runs the step table of the current phase until a step returns FALSE.

enum FlyLandingStatus {
    FLY_LANDING_RUNNING = 0,
    FLY_LANDING_STARTED,
    FLY_LANDING_DONE,
};

// Effect user data, copied into the state by FlyLandingEffect_Init.
typedef struct FlyLandingEffectArgs {
    void *effectPool; // the FieldEffectPool the effect runs in
    FieldSystem *fieldSystem;
    Sprite *trainerSprite;
    SpriteGfxManager *gfxManager;
} FlyLandingEffectArgs;

typedef struct FlyLandingEffect {
    u8 phase; // 0: idle, 1: animating
    u8 step;
    u8 status; // enum FlyLandingStatus
    int timer;
    VecFx32 center;
    VecFx32 offset; // from center, along the flight arc
    u8 unused_20[0xC];
    VecFx32 scale;
    fx32 rotation; // degrees
    u8 unused_3C[4];
    fx32 angle; // degrees, direction of the arc
    u8 unused_44[4];
    fx32 radius;
    fx32 radiusSpeed;
    fx32 scaleSpeed;
    u8 unused_54[4];
    FlyLandingEffectArgs args;
    Sprite *flyerSprite;
    FieldEffect *trainerJumpEffect;
    SysTask *cameraZoom;
} FlyLandingEffect;

Sprite *FlyLanding_CreateFlyerSprite(SpriteGfxManager *gfxManager);
Sprite *FlyLanding_CreateTrainerSprite(SpriteGfxManager *gfxManager, int gender);
FieldEffect *FlyLandingEffect_Create(FieldSystem *fieldSystem, void *effectPool, SpriteGfxManager *gfxManager, Sprite *trainerSprite);
u8 FlyLandingEffect_GetStatus(FieldEffect *effect);
void FlyLandingEffect_Delete(FieldEffect *effect);
void FlyLandingEffect_Start(FieldEffect *effect);
NARC *FlyLanding_OpenNarc(void);
void FieldMove_FadeInFromWhite(void);
void FieldMove_FadeOutToWhite(void);
void FieldMove_SetPlayerVisible(FieldSystem *fieldSystem, BOOL visible);

#endif // POKEHEARTGOLD_FIELD_FLY_LANDING_EFFECT_H
