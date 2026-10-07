#ifndef FIELD_OVERLAY_PARTICLE_STORAGE_PRIVATE_H
#define FIELD_OVERLAY_PARTICLE_STORAGE_PRIVATE_H
#include "global.h"
#include "overlay_01_021EFB38_internal.h"
#include "spl.h"
typedef struct FieldOverlayParticleWork {
    u16 state;
    u8 flag, complete;
    void *arg;
    SPLEmitter *scene;
    void *buffer;
    u32 texOffset, plttOffset;
} FieldOverlayParticleWork;
typedef char FieldOverlayParticleWorkExtent[sizeof(FieldOverlayParticleWork)==24?1:-1];
u32 ov01_021F1224(u32 size,BOOL compressed);
u32 ov01_021F1250(u32 size,BOOL fourPalette);
#endif
