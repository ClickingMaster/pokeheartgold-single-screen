#include "global.h"

#include "field/field_camera_zoom.h"
#include "field/field_effect_helpers.h"
#include "field/fly_landing_effect.h"
#include "field/sprite_gfx_manager.h"

#include "field_effects.h"
#include "field_system.h"
#include "filesystem.h"
#include "map_object.h"
#include "math_util.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "sprite.h"

typedef BOOL (*FlyLandingStepFunc)(FlyLandingEffect *effect);

// Defined in field_effects.c, which has no public prototypes for its pool.
FieldEffect *FieldEffectPool_AddBasicEffect(struct FieldEffectPool *pool, const FieldEffectBasicTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority);

// Defined with overlay 2's fly arrival task: a field effect that keeps an object on the trainer sprite.
FieldEffect *FieldMoveCutIn_CreateTrainerJumpEffect(struct FieldEffectPool *pool, Sprite *trainerSprite);
void TrainerJumpEffect_StartHop(FieldEffect *effect);

// Callbacks of sFlyLandingEffectTemplate and steps of sFlyLandingPhases (overlay 2's data block).
BOOL FlyLandingEffect_Init(FieldEffect *effect, FlyLandingEffect *landing);
void FlyLandingEffect_Destroy(FieldEffect *effect, FlyLandingEffect *landing);
void FlyLandingEffect_Update(FieldEffect *effect, FlyLandingEffect *landing);
BOOL FlyLandingEffect_Idle(FlyLandingEffect *landing);
BOOL FlyLandingEffect_SwoopIn(FlyLandingEffect *landing);
BOOL FlyLandingEffect_StartClimb(FlyLandingEffect *landing);
BOOL FlyLandingEffect_Climb(FlyLandingEffect *landing);
BOOL FlyLandingEffect_HideFlyer(FlyLandingEffect *landing);
BOOL FlyLandingEffect_StartFlyAway(FlyLandingEffect *landing);
BOOL FlyLandingEffect_FlyAway(FlyLandingEffect *landing);
BOOL FlyLandingEffect_ShowPlayer(FlyLandingEffect *landing);
BOOL FlyLandingEffect_Done(FlyLandingEffect *landing);

extern const VecFx32 sFlyLandingFlyerScale;
extern const FieldEffectBasicTemplate sFlyLandingEffectTemplate;
extern const FlyLandingStepFunc *const sFlyLandingPhases[];
extern const VecFx32 sFlyLandingSwoopCenter;
extern const VecFx32 sFlyLandingSwoopScale;
extern const VecFx32 sFlyLandingTrainerScale;
extern const VecFx32 sFlyLandingFlyAwayCenter;
extern const VecFx32 sFlyLandingFlyAwayScale;

Sprite *FlyLanding_CreateFlyerSprite(SpriteGfxManager *gfxManager) {
    VecFx32 pos = { 0, 0, 0 };
    VecFx32 affineMatrix = { 0, 0, 0 };
    VecFx32 affineScale;
    Sprite *sprite;

    affineScale = sFlyLandingFlyerScale;
    sprite = SpriteGfxManager_CreateSprite(gfxManager, &pos, 0, 0, 0, 2, 0, 0x84);
    Sprite_SetAffineOverwriteMode(sprite, 2);
    Sprite_SetAffineMatrix(sprite, &affineMatrix);
    Sprite_SetAffineScale(sprite, &affineScale);
    Sprite_SetAffineZRotation(sprite, GF_DegreeToSinCosIdx(0));
    return sprite;
}

Sprite *FlyLanding_CreateTrainerSprite(SpriteGfxManager *gfxManager, int gender) {
    VecFx32 pos = { 0, 0, 0 };
    Sprite *sprite;
    int plttId;

    plttId = 0;
    if (gender == PLAYER_GENDER_FEMALE) {
        plttId = 1;
    }
    sprite = SpriteGfxManager_CreateSprite(gfxManager, &pos, 2, plttId, 2, 1, 0, 0x83);
    Sprite_SetDrawFlag(sprite, FALSE);
    Sprite_SetAnimCtrlSeq(sprite, 6);
    return sprite;
}

FieldEffect *FlyLandingEffect_Create(FieldSystem *fieldSystem, struct FieldEffectPool *pool, SpriteGfxManager *gfxManager, Sprite *trainerSprite) {
    VecFx32 pos = { 0, 0, 0 };
    FlyLandingEffectArgs args;

    args.pool = pool;
    args.fieldSystem = fieldSystem;
    args.gfxManager = gfxManager;
    args.trainerSprite = trainerSprite;
    return FieldEffectPool_AddBasicEffect(pool, &sFlyLandingEffectTemplate, &pos, 0, &args, 0x82);
}

u8 FlyLandingEffect_GetStatus(FieldEffect *effect) {
    FlyLandingEffect *landing = FieldEffect_GetState(effect);
    return landing->status;
}

BOOL FlyLandingEffect_Init(FieldEffect *effect, FlyLandingEffect *landing) {
    landing->args = *(FlyLandingEffectArgs *)FieldEffect_GetUserData(effect);
    landing->flyerSprite = FlyLanding_CreateFlyerSprite(landing->args.gfxManager);
    return TRUE;
}

void FlyLandingEffect_Delete(FieldEffect *effect) {
    FlyLandingEffect *landing = FieldEffect_GetState(effect);
    if (landing->trainerEffect != NULL) {
        FieldEffect_Destroy((int)landing->trainerEffect);
    }
    if (landing->cameraZoom != NULL) {
        FieldCameraZoom_End(landing->cameraZoom);
    }
    FieldEffect_Destroy((int)effect);
}

void FlyLandingEffect_Destroy(FieldEffect *effect, FlyLandingEffect *landing) {
    Sprite_Delete(landing->flyerSprite);
}

void FlyLandingEffect_Update(FieldEffect *effect, FlyLandingEffect *landing) {
    const FlyLandingStepFunc *steps = sFlyLandingPhases[landing->phase];
    while (steps[landing->step](landing) == TRUE) {
    }
}

BOOL FlyLandingEffect_Idle(FlyLandingEffect *landing) {
    landing->status = FLY_LANDING_RUNNING;
    Sprite_SetDrawFlag(landing->flyerSprite, FALSE);
    return FALSE;
}

void FlyLandingEffect_Start(FieldEffect *effect) {
    VecFx32 position = sFlyLandingSwoopCenter;
    VecFx32 scale = sFlyLandingSwoopScale;
    VecFx32 matrix;
    VecFx32 trainerScale;
    FlyLandingEffect *landing = FieldEffect_GetState(effect);
    Sprite *sprite;

    landing->phase = 1;
    landing->status = FLY_LANDING_RUNNING;
    landing->step = 0;
    landing->timer = 0;
    landing->center = position;
    landing->offset.x = 0;
    landing->offset.y = 0;
    landing->offset.z = 0;
    landing->rotation = FX32_CONST(350);
    landing->scale = scale;
    landing->scaleSpeed = 0x400;
    landing->angle = FX32_CONST(45);
    landing->radius = FX32_CONST(192);
    landing->radiusSpeed = FX32_CONST(32);
    landing->offset.x = GF_CosDeg(45) * (landing->radius / FX32_ONE);
    landing->offset.y = GF_SinDeg((u16)(landing->angle / FX32_ONE)) * (landing->radius / FX32_ONE);
    position.x = landing->center.x + landing->offset.x;
    position.y = landing->center.y + landing->offset.y;
    Sprite_SetMatrix(landing->flyerSprite, &position);
    Sprite_SetAffineScale(landing->flyerSprite, &scale);
    Sprite_SetAffineZRotation(landing->flyerSprite, GF_DegreeToSinCosIdx((u16)(landing->rotation / FX32_ONE)));
    Sprite_SetDrawPriority(landing->flyerSprite, 0x84);
    Sprite_SetDrawFlag(landing->flyerSprite, TRUE);
    landing->trainerEffect = FieldMoveCutIn_CreateTrainerJumpEffect(landing->args.pool, landing->args.trainerSprite);
    landing->cameraZoom = FieldCameraZoom_Start(landing->args.fieldSystem, HEAP_ID_FIELD1);
    FieldCameraZoom_Set(landing->cameraZoom, 1, FX32_CONST(-120), 12);
    {
        fx32 *zero = (fx32 *)&matrix;
        zero[0] = 0;
        zero[1] = 0;
        zero[2] = 0;
    }
    trainerScale = sFlyLandingTrainerScale;
    sprite = landing->args.trainerSprite;
    Sprite_SetAffineOverwriteMode(sprite, 2);
    Sprite_SetAffineMatrix(sprite, &matrix);
    Sprite_SetAffineScale(sprite, &trainerScale);
    Sprite_SetAffineZRotation(sprite, GF_DegreeToSinCosIdx(0));
    Sprite_SetAnimCtrlSeq(sprite, 2);
    FieldMove_FadeInFromWhite();
}

// The flyer comes down along the arc, carrying the (twice as large) trainer sprite.
BOOL FlyLandingEffect_SwoopIn(FlyLandingEffect *landing) {
    Sprite *flyer = landing->flyerSprite;
    Sprite *trainer = landing->args.trainerSprite;
    VecFx32 pos;

    landing->radius -= landing->radiusSpeed;
    if (landing->radius < 0) {
        landing->radius = 0;
    }
    if (landing->radiusSpeed > FX32_CONST(0.5)) {
        landing->radiusSpeed -= FX32_CONST(1.75);
    }
    if (landing->radiusSpeed < FX32_ONE) {
        landing->radiusSpeed = FX32_ONE;
    }
    landing->offset.x = GF_CosDeg(45) * (landing->radius / FX32_ONE);
    landing->offset.y = GF_SinDeg((u16)(landing->angle / FX32_ONE)) * (landing->radius / FX32_ONE);
    if (landing->angle / FX32_ONE < 90) {
        landing->angle += FX32_CONST(4);
    }
    landing->scale.x -= landing->scaleSpeed;
    if (landing->scale.x < FX32_ONE) {
        landing->scale.x = FX32_ONE;
    }
    landing->scale.y -= landing->scaleSpeed;
    if (landing->scale.y < FX32_ONE) {
        landing->scale.y = FX32_ONE;
    }
    Sprite_SetAffineScale(flyer, &landing->scale);
    Sprite_SetAffineScale(trainer, &landing->scale);
    pos.x = landing->center.x + landing->offset.x;
    pos.y = landing->center.y + landing->offset.y;
    Sprite_SetMatrix(flyer, &pos);
    pos.y -= FX32_CONST(18);
    Sprite_SetMatrix(trainer, &pos);
    if (landing->radius == 0) {
        landing->timer = 0;
        landing->step++;
    } else {
        landing->timer++;
    }
    return FALSE;
}

BOOL FlyLandingEffect_StartClimb(FlyLandingEffect *landing) {
    TrainerJumpEffect_StartHop(landing->trainerEffect);
    FieldCameraZoom_Set(landing->cameraZoom, 2, 0, 12);
    landing->scaleSpeed = 0x400;
    landing->angle = FX32_CONST(128);
    landing->radius = 0;
    landing->radiusSpeed = FX32_CONST(1.5);
    landing->step++;
    return TRUE;
}

// Having dropped the trainer, the flyer climbs away, spinning and growing back to full size.
BOOL FlyLandingEffect_Climb(FlyLandingEffect *landing) {
    Sprite *flyer = landing->flyerSprite;
    VecFx32 pos;

    landing->radius += landing->radiusSpeed;
    landing->radiusSpeed += FX32_ONE;
    if (landing->radiusSpeed > FX32_CONST(16)) {
        landing->radiusSpeed = FX32_CONST(16);
    }
    landing->offset.x = GF_CosDeg((u16)(landing->angle / FX32_ONE)) * (landing->radius / FX32_ONE);
    landing->offset.y = GF_SinDeg(128) * (landing->radius / FX32_ONE);
    if (landing->angle < FX32_CONST(160)) {
        landing->angle += FX32_ONE;
    }
    landing->rotation += FX32_CONST(2);
    Sprite_SetAffineZRotation(flyer, GF_DegreeToSinCosIdx((u16)(landing->rotation / FX32_ONE)));
    landing->scale.x += landing->scaleSpeed;
    if (landing->scale.x > FX32_ONE) {
        landing->scale.x = FX32_ONE;
    }
    landing->scale.y += landing->scaleSpeed;
    if (landing->scale.y > FX32_ONE) {
        landing->scale.y = FX32_ONE;
    }
    Sprite_SetAffineScale(flyer, &landing->scale);
    pos.x = landing->center.x + landing->offset.x;
    pos.y = landing->center.y - landing->offset.y;
    Sprite_SetMatrix(flyer, &pos);
    if (pos.y / FX32_ONE <= -16) {
        landing->step++;
    }
    return FALSE;
}

BOOL FlyLandingEffect_HideFlyer(FlyLandingEffect *landing) {
    Sprite_SetDrawFlag(landing->flyerSprite, FALSE);
    Sprite_SetAnimCtrlSeq(landing->args.trainerSprite, 1);
    landing->timer = 0;
    landing->step++;
    return TRUE;
}

BOOL FlyLandingEffect_StartFlyAway(FlyLandingEffect *landing) {
    VecFx32 center;
    VecFx32 scale;

    if (++landing->timer < 20) {
        return FALSE;
    }
    center = sFlyLandingFlyAwayCenter;
    scale = sFlyLandingFlyAwayScale;
    landing->timer = 0;
    landing->center = center;
    landing->offset.x = 0;
    landing->offset.y = 0;
    landing->offset.z = 0;
    landing->rotation = 0;
    landing->scale = scale;
    landing->scaleSpeed = 0x200;
    landing->angle = FX32_CONST(315);
    landing->radius = FX32_CONST(128);
    landing->radiusSpeed = FX32_CONST(2);
    Sprite_SetMatrix(landing->flyerSprite, &center);
    Sprite_SetAffineScale(landing->flyerSprite, &scale);
    Sprite_SetAffineZRotation(landing->flyerSprite, GF_DegreeToSinCosIdx((u16)(landing->rotation / FX32_ONE)));
    Sprite_SetDrawFlag(landing->flyerSprite, TRUE);
    Sprite_SetAnimCtrlSeq(landing->args.trainerSprite, 6);
    Sprite_SetAnimActiveFlag(landing->args.trainerSprite, TRUE);
    landing->step++;
    return TRUE;
}

// The flyer shrinks into the distance.
BOOL FlyLandingEffect_FlyAway(FlyLandingEffect *landing) {
    Sprite *flyer = landing->flyerSprite;
    VecFx32 pos;

    landing->radius -= landing->radiusSpeed;
    if (landing->radiusSpeed < FX32_CONST(16)) {
        landing->radiusSpeed += FX32_CONST(2);
    }
    if (landing->radius < 0) {
        landing->radius = 0;
    }
    landing->offset.x = GF_CosDeg(315) * (landing->radius / FX32_ONE);
    landing->offset.y = GF_SinDeg((u16)(landing->angle / FX32_ONE)) * (landing->radius / FX32_ONE);
    if (landing->angle / FX32_ONE < 180) {
        landing->angle -= FX32_CONST(4);
    }
    landing->scale.x -= landing->scaleSpeed;
    if (landing->scale.x < FX32_CONST(0.25)) {
        landing->scale.x = FX32_CONST(0.25);
    }
    landing->scale.y -= landing->scaleSpeed;
    if (landing->scale.y < FX32_CONST(0.25)) {
        landing->scale.y = FX32_CONST(0.25);
    }
    Sprite_SetAffineScale(flyer, &landing->scale);
    landing->rotation += FX32_CONST(6);
    if (landing->rotation / FX32_ONE > 60) {
        landing->rotation = FX32_CONST(60);
    }
    Sprite_SetAffineZRotation(flyer, GF_DegreeToSinCosIdx((u16)(landing->rotation / FX32_ONE)));
    pos.x = landing->center.x + landing->offset.x;
    pos.y = landing->center.y + landing->offset.y;
    Sprite_SetMatrix(flyer, &pos);
    if (landing->radius <= 0) {
        Sprite_SetDrawFlag(flyer, FALSE);
        landing->step++;
    }
    return FALSE;
}

BOOL FlyLandingEffect_ShowPlayer(FlyLandingEffect *landing) {
    if (++landing->timer == 8) {
        Sprite_SetAnimCtrlSeq(landing->args.trainerSprite, 1);
        FieldMove_SetPlayerVisible(landing->args.fieldSystem, FALSE);
    }
    if (landing->timer == 10) {
        Sprite_SetDrawFlag(landing->args.trainerSprite, FALSE);
    }
    if (landing->timer > 15 && FieldCameraZoom_IsDone(landing->cameraZoom) == TRUE) {
        landing->timer = 0;
        landing->step++;
        landing->status = FLY_LANDING_DONE;
    }
    return FALSE;
}

BOOL FlyLandingEffect_Done(FlyLandingEffect *landing) {
    return FALSE;
}

NARC *FlyLanding_OpenNarc(void) {
    return NARC_New(NARC_application_choose_starter_choose_starter_sub_res, HEAP_ID_FIELD1);
}

void FieldMove_FadeInFromWhite(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 1, HEAP_ID_FIELD1);
}

void FieldMove_FadeOutToWhite(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 6, 1, HEAP_ID_FIELD1);
}

void FieldMove_SetPlayerVisible(FieldSystem *fieldSystem, BOOL visible) {
    MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), visible);
}
