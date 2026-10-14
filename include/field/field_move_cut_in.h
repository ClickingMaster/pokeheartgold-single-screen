#ifndef POKEHEARTGOLD_FIELD_FIELD_MOVE_CUT_IN_H
#define POKEHEARTGOLD_FIELD_FIELD_MOVE_CUT_IN_H

#include "field/field_effect_manager.h"
#include "field/field_3d_model.h"
#include "field/sprite_gfx_manager.h"

#include "field_system.h"
#include "pokemon.h"
#include "sprite.h"
#include "sys_task.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"

// The field move cut-in of overlay 2: the banner with the Pokemon flying across the screen when a field move is
// used, and the Fly take-off animations (the trainer, or the trainer with another map object, carried away by the
// flying Pokemon). One heap block of this type drives one cut-in SysTask from creation to Delete.

// The 3D model shown at the second map object when it disappears (Fly with another object).
typedef struct FieldMoveCutInModel {
    NNSFndAllocator allocator;            // 0x000
    void *modelData;                      // 0x010
    Field3dModel model;                   // 0x014
    Field3dObject object;                 // 0x024
    Field3DModelAnimation animations[2];  // 0x09C
} FieldMoveCutInModel;

typedef struct FieldMoveCutIn {
    int state;                           // 0x000, index into the kind's step table
    BOOL done;                           // 0x004
    int timer;                           // 0x008
    u16 playerGender;                    // 0x00C
    u16 trainerGraphics;                 // 0x00E, player gender, or 2 for no trainer
    BOOL drawSprites;                    // 0x010
    int scrollSpriteMode;                // 0x014, 1: banner sprites hidden, 2: drawn inside the window only
    BOOL fadeStarted;                    // 0x018
    BOOL flyStarted;                     // 0x01C, the trainer sprite is no longer clipped to the window
    int kind;                            // 0x020, 0: banner only, 1: Fly, 2: Fly with the follower, 3: Fly with an object
    u16 bg0Priority;                     // 0x024
    u16 bg3Priority;                     // 0x026
    int unk_28;                          // 0x028
    BOOL windowUpdate;                   // 0x02C, the HBlank window task writes the registers
    BOOL windowEnabled;                  // 0x030
    int windowInsidePlanes;              // 0x034
    BOOL windowInsideEffects;            // 0x038
    int windowOutsidePlanes;             // 0x03C
    BOOL windowOutsideEffects;           // 0x040
    fx32 windowLeft;                     // 0x044
    fx32 windowRight;                    // 0x048
    fx32 windowTop;                      // 0x04C
    fx32 windowBottom;                   // 0x050
    fx32 windowSpeed;                    // 0x054
    fx32 monSpeed;                       // 0x058
    Pokemon *mon;                        // 0x05C
    FieldSystem *fieldSystem;            // 0x060
    NNSG2dScreenData *bannerScreen;      // 0x064
    NNSG2dCharacterData *bannerChar;     // 0x068
    NNSG2dPaletteData *bannerPltt;       // 0x06C
    SpriteList *spriteList;              // 0x070
    G2dRenderer renderer;                // 0x074
    GF_2DGfxResMan *resMan[4];           // 0x19C, char, pltt, cell, anim
    SpriteResource *charResources[4];    // 0x1AC
    SpriteResource *plttResources[3];    // 0x1BC
    SpriteResource *cellResources[4];    // 0x1C8
    SpriteResource *animResources[2];    // 0x1D8
    void *effectPool;                    // 0x1E0
    Sprite *trainerSprite;               // 0x1E4
    Sprite *monSprite;                   // 0x1E8
    FieldEffect *flyerEffect;            // 0x1EC
    FieldEffect *modelEffect;            // 0x1F0
    FieldEffect *trainerClipEffect;      // 0x1F4
    PokepicTemplate pokepicTemplate;     // 0x1F8
    LocalMapObject *playerObject;        // 0x208
    LocalMapObject *otherObject;         // 0x20C
    int transferStep;                    // 0x210
    BOOL transferDone;                   // 0x214
    void *monCharData;                   // 0x218
    void *monPlttData;                   // 0x21C
    SysTask *transferTask;               // 0x220
    SysTask *windowTask;                 // 0x224
    FieldMoveCutInModel model;           // 0x228
    VecFx32 otherObjectOffset;           // 0x2EC, x and z: screen offset of the second map object
} FieldMoveCutIn;

// The first half of the cut-in (state machines, banner, sprite resources).
Sprite *FieldMoveCutIn_CreateSprite(FieldMoveCutIn *cutIn, VecFx32 *pos, int charId, int plttId, int cellId, int animId, int priority, int drawPriority);
Sprite *FieldMoveCutIn_CreateEffectSprite(FieldMoveCutIn *cutIn, VecFx32 *pos, int drawPriority, int animSeq);
Sprite *FieldMoveCutIn_CreateFlyerSprite(FieldMoveCutIn *cutIn, VecFx32 *pos, int drawPriority, int unused);

// field_move_cut_in_effects.c
void Task_FieldMoveCutIn_UpdateWindow(SysTask *task, void *data);
void FieldMoveCutIn_GetMonPicIds(FieldMoveCutIn *cutIn, PokepicTemplate *template);
void *FieldMoveCutIn_LoadMonChar(Pokemon *mon, PokepicTemplate *template, enum HeapID heapID);
void *FieldMoveCutIn_LoadMonPltt(PokepicTemplate *template, enum HeapID heapID);
SpriteResource *FieldMoveCutIn_AddMonCharResource(FieldMoveCutIn *cutIn, NARC *narc);
void FieldMoveCutIn_TransferMonChar(FieldMoveCutIn *cutIn, void *data);
SpriteResource *FieldMoveCutIn_AddMonPlttResource(FieldMoveCutIn *cutIn, NARC *narc);
void FieldMoveCutIn_TransferMonPltt(FieldMoveCutIn *cutIn, void *data);
void FieldMoveCutIn_FreeMonResources(FieldMoveCutIn *cutIn);
Sprite *FieldMoveCutIn_CreateMonSprite(FieldMoveCutIn *cutIn, VecFx32 *pos);
void FieldMoveCutIn_CreateScrollSprites(FieldMoveCutIn *cutIn, int mode);
void FieldMoveCutIn_CreateScrollSprite(FieldMoveCutIn *cutIn, const VecFx32 *position, const VecFx32 *velocity, u32 drawPriority, u32 animSeq, int mode);
void FieldMoveCutIn_CreateFlyer(FieldMoveCutIn *cutIn);
u8 FieldMoveCutIn_GetFlyerStatus(FieldMoveCutIn *cutIn);
void FieldMoveCutIn_DestroyFlyer(FieldMoveCutIn *cutIn);
void FieldMoveCutIn_StartFlyerTakeOff(FieldMoveCutIn *cutIn);
void FieldMoveCutIn_StartFlyerPickUp(FieldMoveCutIn *cutIn);
FieldEffect *FieldMoveCutIn_CreateTrainerJumpEffect(void *effectPool, Sprite *trainerSprite);
void TrainerJumpEffect_Stop(FieldEffect *effect);
void TrainerJumpEffect_StartJump(FieldEffect *effect);
void TrainerJumpEffect_StartHop(FieldEffect *effect);
void FieldMoveCutIn_SetPlayerVisible(FieldMoveCutIn *cutIn, BOOL visible);
void FieldMoveCutIn_CreateTrainerClipEffect(FieldMoveCutIn *cutIn);
void FieldMoveCutIn_DestroyTrainerClipEffect(FieldMoveCutIn *cutIn);
void FieldMoveCutIn_CreateModelEffect(FieldMoveCutIn *cutIn);
void FieldMoveCutIn_LoadModel(FieldMoveCutIn *cutIn);
void FieldMoveCutIn_UnloadModel(FieldMoveCutIn *cutIn);

#endif // POKEHEARTGOLD_FIELD_FIELD_MOVE_CUT_IN_H
