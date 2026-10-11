#ifndef POKEHEARTGOLD_FIELD_EFFECT_GROUND_MARKER_INTERNAL_H
#define POKEHEARTGOLD_FIELD_EFFECT_GROUND_MARKER_INTERNAL_H

// Thumb comes from the SOURCE: global.h -> nitro/code16.h -> #pragma thumb on
#include "global.h"

#include "constants/sndseq.h"

#include "nnsys/g3d/binres/res_struct_accessor.h"

#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "field/field_effect_manager.h"
#include "task.h"
#include "billboard_lists.h"
#include "field_effects.h"
#include "vram_transfer_manager.h"

// Not declared in any tracked header (field_effect_manager_4.c declares these
// locally; field_effect_animation_internal.h has only K&R `extern void f();`
// forms which conflict with field/field_effect_manager.h, so we re-declare here).
void ov01_021F18D4(FieldEffectManager *manager, int a1, int a2);
void ov01_021F1908(FieldEffectManager *manager, int a1, int a2);
void FieldEffectManager_LoadTexResource(FieldEffectManager *manager, int id, u32 fileId, BOOL copyWithoutTex);
void *FieldEffectManager_AddRecord(FieldEffectManager *manager, u32 key, int a2, int a3, int objId, int a5, const void *a6);
void *ov01_021F18FC(FieldEffectManager *manager, int a1);
void *ov01_021F1924(FieldEffectManager *manager, int a1);
void FieldEffectManager_FreeTexResource(FieldEffectManager *manager, int id);
void FieldEffectManager_ReleaseRecord(FieldEffectManager *manager, u32 id);
void *FieldEffectManager_CreateBillboardFromRecord(FieldSystem *fieldSystem, u32 id, const VecFx32 *position);
void *FieldEffectManager_LoadNarcMember(FieldSystem *fieldSystem, u32 fileId, int atEnd);
NNSG3dResTex *FieldEffectManager_GetTexture(FieldSystem *fieldSystem, int id);

void *BillboardObject_Delete(void *a0);
void BillboardObject_SetPosition(void *a0, const VecFx32 *a1);
void BillboardGfxSequence_Init(void *a0, void *a1);
void *FieldEffect_GetParam(void *a0);
void FieldEffect_SetPosition(void *a0, VecFx32 *a1);
void *Texture_GetImageData(NNSG3dResTex *tex, int a1);
void *Texture_GetPaletteData(NNSG3dResTex *tex, int a1);
u32 Texture_GetImageVramOffsetByIndex(NNSG3dResTex *tex, int a1);
u32 Texture_GetImageSizeByIndex(NNSG3dResTex *tex, int a1);
void GF_AssertFail(void);
void PlaySE(u16 seq);

// The 8-byte module object, allocated by ov01_02203A18 and recovered from the
// field-effect manager slot 0x12 by FieldEffectManager_GetRendererData.
typedef struct Ov01_02203A18_Work {
    FieldEffectManager *unk0;
    BOOL unk4;
} Ov01_02203A18_Work;

// The user-data block handed to FieldEffectManager_CreateEffect as a4 and recovered via FieldEffect_GetUserData.
typedef struct Ov01_02203A18_Args {
    FieldSystem *unk0;
    Ov01_02203A18_Work *unk4;
    LocalMapObject *unk8;
} Ov01_02203A18_Args; // 0xc

// TaskManager_Call environment built by ov01_02203AB4.
typedef struct Ov01_02203A18_TaskEnv {
    int unk0;
    LocalMapObject *unk4;
} Ov01_02203A18_TaskEnv; // 0x8

// Texture-animation state, initialised by BillboardGfxSequence_Init.
typedef struct Ov01_02203A18_TexAnim {
    u16 *unk0;
    u8 *unk4;
    u32 unk8;
    u32 unkC;
    NNSG3dResTex *unk10;
    void *unk14;
    void *unk18;
    u32 unk1C;
    u32 unk20;
    u16 unk24;
    u16 unk26;
} Ov01_02203A18_TexAnim; // 0x28

// The per-instance work block; size 0x68 is the first word of ov01_02209518.
typedef struct Ov01_02203A18_Inst {
    int unk0;
    int unk4;
    u32 unk8;
    u32 unkC;
    int unk10;
    int unk14;
    VecFx32 unk18;
    u8 unk24[4];
    fx32 unk28;
    u8 unk2C[4];
    Ov01_02203A18_Args unk30;
    void *unk3C;
    Ov01_02203A18_TexAnim unk40;
} Ov01_02203A18_Inst; // 0x68

// The animation-advance state read by ov01_02203DF8.
typedef struct Ov01_02203A18_Anim {
    u16 *durations;
    u8 *frames;
    u32 unk8;
    u32 frameCount;
    NNSG3dResTex *tex;
    void *unk14;
    void *unk18;
    u32 unk1C;
    u32 unk20;
    u16 timer;
    u16 index;
} Ov01_02203A18_Anim; // 0x28

typedef struct Ov01_02203A18_FileIds {
    u32 unk0;
    u32 unk4;
} Ov01_02203A18_FileIds; // 0x8

#endif // POKEHEARTGOLD_FIELD_EFFECT_GROUND_MARKER_INTERNAL_H
