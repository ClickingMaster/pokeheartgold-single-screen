#ifndef TO53_OV96_R48_PRIVATE_H
#define TO53_OV96_R48_PRIVATE_H

#include "global.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "poke_overlay.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_0203A3B0.h"
#include "overlay_98_helpers_internal.h"
#include "sound_02004A44.h"

typedef struct Ov96R48Work {
    void *course;
    BgConfig *bgConfig;
    NARC *narc;
    enum HeapID heapId;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u32 screenMode;
    s32 animCounter;
    u8 selection;
    u8 animIndex;
    u8 participant;
    u8 padding_23;
    u32 mode;
    u32 showRecord;
    u32 flag2C;
    Ov98TextContext *message;
    void *unk34;
    ManagedSprite *sprites[4];
} Ov96R48Work;

extern const u8 ov96_0221BA18[];
extern const GraphicsBanks ov96_0221BAC0;
FS_EXTERN_OVERLAY(OVY_98);

void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void PokeathlonCourse_FreePtr4HeapAlloc(void *course);
void ov96_021EE5E0(void *arg);
void DoScheduledBgGpuUpdates(BgConfig *bgConfig);
BOOL sub_02005B78(u16 seqNo, u32 variable, s16 *value);
BOOL ov96_021EF19C(void *course);
void ov96_021EF23C(Ov96R48Work *work);
void ov96_021EF260(void);
BOOL ov96_021EF280(u32 variable, s16 threshold);

#endif
