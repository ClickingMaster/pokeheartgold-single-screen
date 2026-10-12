#ifndef PRIVATE_OVERLAY_01_021EFB38_H
#define PRIVATE_OVERLAY_01_021EFB38_H

#include <nitro/os/tick.h>

#include "global.h"

#include "poke_overlay.h"

typedef void (*OverlayEffectCallback)(void *, void *);
typedef struct OverlayEffectGlobal {
    void *unk0;
    s32 callbackIndex;
    u32 callCount;
} OverlayEffectGlobal;

extern u8 SDK_OVERLAY_OVY_114_ID;
extern u8 SDK_OVERLAY_OVY_115_ID;
extern u8 SDK_OVERLAY_OVY_116_ID;
extern u8 SDK_OVERLAY_OVY_117_ID;
extern u8 SDK_OVERLAY_OVY_118_ID;
extern u8 SDK_OVERLAY_OVY_119_ID;
extern u8 SDK_OVERLAY_OVY_120_ID;
#define LOCAL_OVERLAY_ID(name) ((FSOverlayID) & SDK_OVERLAY_##name##_ID)

extern OverlayEffectGlobal sEncounterEffectState;
extern const OverlayEffectCallback sEncounterEffectFuncs[];

void Task_RunEncounterEffect(void *, void *);
void EncounterEffect_LoadOverlay(s32);
void EncounterEffect_UnloadOverlay(s32);

#endif
