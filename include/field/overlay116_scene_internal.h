#ifndef OVERLAY_116_PRIVATE_H
#define OVERLAY_116_PRIVATE_H

#include "global.h"

#include "field/encounter_effect.h"
#include "field/field_motion_blur.h"

#include "camera.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade_legacy.h"

typedef struct {
    u32 distance;
    u16 angleX;
    u16 angleY;
    u16 perspective;
    u16 delay;
} Ov116CameraKey;

typedef struct {
    u8 perspectiveDuration;
    u8 delay;
    u8 duration;
    u8 fadeFrames;
    s32 perspectiveDelta;
    s32 distanceDelta;
    s32 distanceDuration;
    u16 angleX;
    u16 angleY;
} Ov116Sequence;

typedef struct {
    FieldMotionBlur *motionBlur;
    u32 unused04;
    u32 index;
    s32 delay;
} Ov116PanWork;

typedef struct {
    FieldMotionBlur *motionBlur;
    u8 pad04[4];
    EncounterEffectInterp perspective;
    EncounterEffectQuadInterp distance;
    s32 delay;
} Ov116MoveWork;

void EncounterEffect_Legendary(SysTask *, EncounterEffectTask *);
void EncounterEffect_HoOh(SysTask *, EncounterEffectTask *);
void EncounterEffect_Lugia(SysTask *, EncounterEffectTask *);

#endif
