#ifndef POKEHEARTGOLD_WIFI_PLAZA_MODELS_H
#define POKEHEARTGOLD_WIFI_PLAZA_MODELS_H

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "model_animation.h"
#include "wifi_plaza_map.h"

// 3D scenery of the Wi-Fi Plaza room (models from NARC a/2/0/1, layout from NARC a/2/0/0):
// - the base scene: two room models with up to five looping animations,
// - props: up to 128 animated models of 18 kinds, placed on the map cells that carry a spawn tag,
// - movers: nine models (2 shapes x 3 textures) whose position, angle and one-shot animations
//   are driven by the plaza events.

#define WIFI_PLAZA_PROP_KIND_COUNT  18
#define WIFI_PLAZA_PROP_ANIM_COUNT  3
#define WIFI_PLAZA_MOVER_ANIM_COUNT 4
#define WIFI_PLAZA_BASE_ANIM_COUNT  5

// Prop animation modes (WiFiPlazaModelSet_StartPropAnim)
enum WiFiPlazaPropAnimMode {
    WIFI_PLAZA_PROP_ANIM_LOOP,
    WIFI_PLAZA_PROP_ANIM_ONCE,   // then the callback runs
    WIFI_PLAZA_PROP_ANIM_MANUAL, // frame set by WiFiPlazaModelSet_SetPropAnimFrame
    WIFI_PLAZA_PROP_ANIM_LOOP_BACKWARD,
    WIFI_PLAZA_PROP_ANIM_ONCE_BACKWARD, // then the callback runs
    WIFI_PLAZA_PROP_ANIM_ONCE_REPEATED, // plays once, waits a random delay, repeats
    WIFI_PLAZA_PROP_ANIM_LOOP_DELAYED,  // loops after a random start delay
};

typedef struct WiFiPlazaModelSet WiFiPlazaModelSet;
typedef struct WiFiPlazaProp WiFiPlazaProp;

typedef void (*WiFiPlazaPropCallback)(WiFiPlazaModelSet *set, WiFiPlazaProp *prop);

// Model file ids of a room layout (NARC a/2/0/0 members 1-25). An animation id equal to the
// section's first model id means "none".
typedef struct WiFiPlazaModelsLayout {
    u32 propModels[WIFI_PLAZA_PROP_KIND_COUNT];                            // 0x000
    u32 propAnims[WIFI_PLAZA_PROP_KIND_COUNT][WIFI_PLAZA_PROP_ANIM_COUNT]; // 0x048
    u32 moverModels[2];                                                    // 0x120
    u32 moverTextures[2][3];                                               // 0x128
    u32 moverAnims[2][WIFI_PLAZA_MOVER_ANIM_COUNT];                        // 0x140
    u32 moverNodeCounts[2];                                                // 0x160
    u32 moverAnimNodes[2][WIFI_PLAZA_MOVER_ANIM_COUNT - 1];                // 0x168 the only node animations 1-3 move
    u32 baseModels[2];                                                     // 0x180
    u32 unk_188;                                                           // 0x188
    u32 baseAnims[WIFI_PLAZA_BASE_ANIM_COUNT];                             // 0x18C animation 3 belongs to model 1
} WiFiPlazaModelsLayout;

typedef struct WiFiPlazaBaseResources {
    ModelResource models[2];                          // 0x00
    ModelAnimation anims[WIFI_PLAZA_BASE_ANIM_COUNT]; // 0x20
    BOOL animLoaded[WIFI_PLAZA_BASE_ANIM_COUNT];      // 0x84
} WiFiPlazaBaseResources;

typedef struct WiFiPlazaMoverResources {
    ModelResource models[2];                              // 0x00 texture swapped in before drawing
    NNSG3dResFileHeader *textures[2][3];                  // 0x20
    ModelAnimation anims[2][WIFI_PLAZA_MOVER_ANIM_COUNT]; // 0x38
} WiFiPlazaMoverResources;

typedef struct WiFiPlazaPropResources {
    ModelResource models[WIFI_PLAZA_PROP_KIND_COUNT];                        // 0x000
    void *animFiles[WIFI_PLAZA_PROP_KIND_COUNT][WIFI_PLAZA_PROP_ANIM_COUNT]; // 0x120 NULL = none
} WiFiPlazaPropResources;

// Animations 0 and 2 loop all the time, 1 and 3 are played once on request.
typedef struct WiFiPlazaMover {
    u8 active;                                     // 0x00
    u8 texture;                                    // 0x01
    u8 model;                                      // 0x02
    ModelInstance instance;                        // 0x04
    BOOL animPlaying[WIFI_PLAZA_MOVER_ANIM_COUNT]; // 0x7C
    fx32 animFrames[WIFI_PLAZA_MOVER_ANIM_COUNT];  // 0x8C
    VecFx32 position;                              // 0x9C
    VecFx32 offset;                                // 0xA8 added to position
} WiFiPlazaMover;

struct WiFiPlazaProp {
    u16 active;                                                      // 0x00
    u16 kind;                                                        // 0x02
    ModelInstance instance;                                          // 0x04
    ModelAnimation anims[WIFI_PLAZA_PROP_ANIM_COUNT];                // 0x7C
    u8 animActive[WIFI_PLAZA_PROP_ANIM_COUNT];                       // 0xB8
    u8 unk_BB;                                                       // 0xBB
    u8 animModes[WIFI_PLAZA_PROP_ANIM_COUNT];                        // 0xBC
    u8 unk_BF;                                                       // 0xBF
    fx32 animFrames[WIFI_PLAZA_PROP_ANIM_COUNT];                     // 0xC0
    u8 randomDelay;                                                  // 0xCC delays are rolled below this
    u8 animDelays[WIFI_PLAZA_PROP_ANIM_COUNT];                       // 0xCD
    WiFiPlazaPropCallback animCallbacks[WIFI_PLAZA_PROP_ANIM_COUNT]; // 0xD0
    fx32 animSpeed;                                                  // 0xDC frames per update
    u8 alphaOverride;                                                // 0xE0
    u8 alpha;                                                        // 0xE1 model alpha while drawing
    u8 restoreAlpha;                                                 // 0xE2 model alpha afterwards
};

struct WiFiPlazaModelSet {
    BOOL baseVisible;                                // 0x000
    ModelInstance baseInstances[2];                  // 0x004
    BOOL baseAnimActive[WIFI_PLAZA_BASE_ANIM_COUNT]; // 0x0F4
    fx32 baseAnimFrames[WIFI_PLAZA_BASE_ANIM_COUNT]; // 0x108
    WiFiPlazaMover *movers;                          // 0x11C
    WiFiPlazaProp *props;                            // 0x120
    u8 moverCount;                                   // 0x124
    u8 propCount;                                    // 0x125
    u8 layoutVariant;                                // 0x126
    u8 layoutGroup;                                  // 0x127
    BOOL loaded;                                     // 0x128
    WiFiPlazaBaseResources baseRes;                  // 0x12C
    WiFiPlazaMoverResources moverRes;                // 0x1C4
    WiFiPlazaPropResources propRes;                  // 0x29C
    NNSFndAllocator allocator;                       // 0x494
};

// Prop that the plaza effects show and hide: shown with all its animations, hidden by speeding
// the animations up while fading out and growing for 10 frames.
typedef struct WiFiPlazaPropShow {
    WiFiPlazaProp *prop;
    u16 state; // 0 hidden, 1 show requested, 2 shown, 3 hide requested, 4 fading out
    s16 fadeTimer;
    fx32 animSpeed;
} WiFiPlazaPropShow;

typedef struct WiFiPlazaModels {
    WiFiPlazaMap *map;                 // 0x000
    WiFiPlazaModelSet *set;            // 0x004
    WiFiPlazaProp *props[128];         // 0x008
    WiFiPlazaMover *movers[9];         // 0x208
    WiFiPlazaProp *matchBooths[3];     // 0x22C props of kinds 6-8, one per matchmaking session
    WiFiPlazaProp *matchSigns[3];      // 0x238 kind 10 props (tags 0x49-0x4B): player count display
    BOOL pairAnimState;                // 0x244 WiFiPlazaModels_SetPairAnim
    WiFiPlazaPropShow effectShows[24]; // 0x248 kind 13
    WiFiPlazaPropShow stageShows[24];  // 0x368 kind 14
    WiFiPlazaProp *effectLoops[24];    // 0x488 kind 15
    WiFiPlazaPropShow stageBursts[24]; // 0x4E8 kind 16
    u8 effectShowCount;                // 0x608
    u8 stageShowCount;                 // 0x609
    u8 effectLoopCount;                // 0x60A
    u8 stageBurstCount;                // 0x60B
    u32 blinkTimer;                    // 0x60C
    u8 layoutGroup;                    // 0x610
    u8 layoutVariant;                  // 0x611 layout file variant + group * 5
    u8 propCount;                      // 0x612
    u8 moverCount;                     // 0x613
} WiFiPlazaModels;

typedef struct WiFiPlazaModelsPos {
    s16 x;
    s16 y;
} WiFiPlazaModelsPos;

WiFiPlazaModelSet *WiFiPlazaModelSet_Create(int moverCount, int propCount, enum HeapID heapID, enum HeapID unused);
void WiFiPlazaModelSet_Delete(WiFiPlazaModelSet *set);
void WiFiPlazaModelSet_Update(WiFiPlazaModelSet *set);
void WiFiPlazaModelSet_Draw(WiFiPlazaModelSet *set);
void WiFiPlazaModelSet_VBlank(WiFiPlazaModelSet *set);
void WiFiPlazaModelSet_Load(WiFiPlazaModelSet *set, u32 variant, u32 group, enum HeapID heapID, enum HeapID dataHeapID);
void WiFiPlazaModelSet_Unload(WiFiPlazaModelSet *set);
void WiFiPlazaModelSet_StartBaseAnim4(WiFiPlazaModelSet *set);
void WiFiPlazaModelSet_StopBaseAnim4(WiFiPlazaModelSet *set);
WiFiPlazaMover *WiFiPlazaModelSet_AddMover(WiFiPlazaModelSet *set, s32 model, s32 texture, const VecFx32 *position);
void WiFiPlazaModelSet_RemoveMover(WiFiPlazaMover *mover);
void WiFiPlazaMover_SetPosition(WiFiPlazaMover *mover, const VecFx32 *position);
void WiFiPlazaMover_SetOffset(WiFiPlazaMover *mover, const VecFx32 *offset);
void WiFiPlazaMover_GetPosition(WiFiPlazaMover *mover, VecFx32 *position);
void WiFiPlazaMover_SetVisible(WiFiPlazaMover *mover, BOOL visible);
BOOL WiFiPlazaMover_StartAnim1(WiFiPlazaMover *mover);
BOOL WiFiPlazaMover_StartAnim3(WiFiPlazaMover *mover);
void WiFiPlazaMover_SetAngle(WiFiPlazaMover *mover, u16 angle);
BOOL WiFiPlazaMover_IsAnim1Playing(WiFiPlazaMover *mover);
BOOL WiFiPlazaMover_IsAnim3Playing(WiFiPlazaMover *mover);
WiFiPlazaProp *WiFiPlazaModelSet_AddProp(WiFiPlazaModelSet *set, u32 kind, u32 x, u32 y);
void WiFiPlazaModelSet_RemoveProp(WiFiPlazaModelSet *set, WiFiPlazaProp *prop);
u16 WiFiPlazaProp_GetKind(WiFiPlazaProp *prop);
void WiFiPlazaProp_SetPos(WiFiPlazaProp *prop, WiFiPlazaModelsPos pos);
u32 WiFiPlazaProp_GetPos(WiFiPlazaProp *prop);
void WiFiPlazaModelSet_StartPropAnim(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot, s32 mode);
void WiFiPlazaModelSet_StartPropAnimWithCallback(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot, s32 mode, WiFiPlazaPropCallback callback);
void WiFiPlazaModelSet_StopPropAnim(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot);
void WiFiPlazaModelSet_StopPropAnims(WiFiPlazaModelSet *set, WiFiPlazaProp *prop);
BOOL WiFiPlazaModelSet_IsPropAnimActive(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot);
void WiFiPlazaModelSet_SetPropAnimFrame(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, int slot, fx32 frame);
BOOL WiFiPlazaProp_IsAnimActive(WiFiPlazaProp *prop, s32 slot);
fx32 WiFiPlazaProp_GetAnimFrame(WiFiPlazaProp *prop, s32 slot);
void WiFiPlazaProp_SetVisible(WiFiPlazaProp *prop, BOOL visible);
void WiFiPlazaModelSet_SetPropLights(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, u32 lightFlags);
void WiFiPlazaProp_SetAnimSpeed(WiFiPlazaProp *prop, fx32 speed);
void WiFiPlazaProp_SetAlpha(WiFiPlazaProp *prop, u8 alpha, u8 restoreAlpha);
void WiFiPlazaProp_ClearAlpha(WiFiPlazaProp *prop);
void WiFiPlazaProp_SetScale(WiFiPlazaProp *prop, fx32 x, fx32 y, fx32 z);
WiFiPlazaModelsLayout *WiFiPlazaModels_LoadLayout(u32 variant, u32 group, enum HeapID heapID);
void WiFiPlazaModels_FreeLayout(WiFiPlazaModelsLayout *layout);
void WiFiPlazaModels_LoadModel(ModelResource *model, NARC *narc, int fileId, enum HeapID heapID);
void WiFiPlazaModels_FreeModel(ModelResource *model);
void WiFiPlazaModels_AdvanceFrameLooped(fx32 *frame, ModelAnimation *anim, fx32 step);
BOOL WiFiPlazaModels_AdvanceFrame(fx32 *frame, ModelAnimation *anim, fx32 step);
void WiFiPlazaModels_RewindFrameLooped(fx32 *frame, ModelAnimation *anim, fx32 step);
BOOL WiFiPlazaModels_RewindFrame(fx32 *frame, ModelAnimation *anim, fx32 step);

void WiFiPlazaModelSet_LoadBase(WiFiPlazaBaseResources *res, NARC *narc, WiFiPlazaModelsLayout *layout, enum HeapID heapID, NNSFndAllocator *allocator);
void WiFiPlazaModelSet_FreeBase(WiFiPlazaBaseResources *res, NNSFndAllocator *allocator);
void WiFiPlazaModelSet_InitBase(WiFiPlazaModelSet *set, WiFiPlazaBaseResources *res);
void WiFiPlazaModelSet_ResetBase(WiFiPlazaModelSet *set, WiFiPlazaBaseResources *res);
void WiFiPlazaModelSet_UpdateBase(WiFiPlazaModelSet *set, WiFiPlazaBaseResources *res);
void WiFiPlazaModelSet_DrawBase(WiFiPlazaModelSet *set, WiFiPlazaBaseResources *unused);
WiFiPlazaMover *WiFiPlazaModelSet_AllocMover(WiFiPlazaModelSet *set);
void WiFiPlazaModelSet_LoadMovers(WiFiPlazaMoverResources *res, NARC *narc, NNSFndAllocator *allocator, WiFiPlazaModelsLayout *layout, enum HeapID heapID);
void WiFiPlazaModelSet_FreeMovers(WiFiPlazaMoverResources *res, NNSFndAllocator *allocator);
void WiFiPlazaModelSet_UpdateMover(WiFiPlazaMover *mover, WiFiPlazaMoverResources *res);
void WiFiPlazaModelSet_DrawMover(WiFiPlazaMover *mover, WiFiPlazaMoverResources *res);
WiFiPlazaProp *WiFiPlazaModelSet_AllocProp(WiFiPlazaModelSet *set);
void WiFiPlazaModelSet_LoadProps(WiFiPlazaPropResources *res, NARC *narc, NNSFndAllocator *allocator, WiFiPlazaModelsLayout *layout, volatile enum HeapID heapID);
void WiFiPlazaModelSet_FreeProps(WiFiPlazaPropResources *res, NNSFndAllocator *allocator);
void WiFiPlazaModelSet_DrawProp(WiFiPlazaPropResources *res, WiFiPlazaProp *prop);
void WiFiPlazaModelSet_UpdateProp(WiFiPlazaModelSet *set, WiFiPlazaProp *prop);

WiFiPlazaModels *WiFiPlazaModels_Create(int group, int variant, WiFiPlazaMap *map, enum HeapID heapID, enum HeapID dataHeapID);
void WiFiPlazaModels_Delete(WiFiPlazaModels *models);
void WiFiPlazaModels_Update(WiFiPlazaModels *models);
void WiFiPlazaModels_Draw(WiFiPlazaModels *models);
void WiFiPlazaModels_VBlank(WiFiPlazaModels *models);
void WiFiPlazaModels_SetMoverX(WiFiPlazaModels *models, int index, fx32 x);
void WiFiPlazaModels_SetMoverOffset(WiFiPlazaModels *models, int index, const VecFx32 *offset);
void WiFiPlazaModels_GetMoverFrontPos(WiFiPlazaModels *models, int index, int side, VecFx32 *position);
BOOL WiFiPlazaModels_StartMoverAnim1(WiFiPlazaModels *models, int index);
BOOL WiFiPlazaModels_StartMoverAnim3(WiFiPlazaModels *models, int index);
void WiFiPlazaModels_SetMoverVisible(WiFiPlazaModels *models, int index, BOOL visible);
void WiFiPlazaModels_SetMoverAngle(WiFiPlazaModels *models, int index, u16 angle);
BOOL WiFiPlazaModels_IsMoverAnim1Playing(WiFiPlazaModels *models, int index);
BOOL WiFiPlazaModels_IsMoverAnim3Playing(WiFiPlazaModels *models, int index);
void WiFiPlazaModels_StartBaseAnim4(WiFiPlazaModels *models);
void WiFiPlazaModels_StopBaseAnim4(WiFiPlazaModels *models);
void WiFiPlazaModels_TouchPropsAt(WiFiPlazaModels *models, u8 x, u8 y);
void WiFiPlazaModels_SetEffectActive(WiFiPlazaModels *models, BOOL active);
void WiFiPlazaModels_TriggerStageBursts(WiFiPlazaModels *models, fx32 animSpeed);
void WiFiPlazaModels_EnableKind1To4Lights(WiFiPlazaModels *models);
void WiFiPlazaModels_SetPairAnim(WiFiPlazaModels *models, u32 mode);
BOOL WiFiPlazaModels_GetPairAnimState(WiFiPlazaModels *models);
void WiFiPlazaModels_ShowEffectProps(WiFiPlazaModels *models, BOOL show, fx32 animSpeed);
BOOL WiFiPlazaModels_ShowEffectProp(WiFiPlazaModels *models, u32 index, BOOL show, fx32 animSpeed);
void WiFiPlazaModels_TriggerStageShows(WiFiPlazaModels *models, fx32 animSpeed);
void WiFiPlazaModels_UpdateMatchSign(WiFiPlazaModels *models, int match, u32 playerCount, BOOL exists, BOOL recruiting, BOOL stop);
BOOL WiFiPlazaProp_IsAt(WiFiPlazaProp *prop, u8 x, u8 y);
void WiFiPlazaModels_OnPairAnimOpened(WiFiPlazaModelSet *set, WiFiPlazaProp *prop);
void WiFiPlazaModels_OnPairAnimClosed(WiFiPlazaModelSet *set, WiFiPlazaProp *prop);
void WiFiPlazaModels_TouchProp(WiFiPlazaModels *models, WiFiPlazaProp *prop);
void WiFiPlazaModels_TouchPropQuiet(WiFiPlazaModels *models, WiFiPlazaProp *prop);
void WiFiPlazaModels_PlayPropAnimsOnce(WiFiPlazaModels *models, WiFiPlazaProp *prop);
void WiFiPlazaPropShow_Init(WiFiPlazaPropShow *show, WiFiPlazaProp *prop);
void WiFiPlazaPropShow_UpdateEffect(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set);
void WiFiPlazaPropShow_RequestShow(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set, fx32 animSpeed);
void WiFiPlazaPropShow_RequestHide(WiFiPlazaPropShow *show);
void WiFiPlazaPropShow_ShowEffect(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set);
void WiFiPlazaPropShow_HideEffect(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set);
void WiFiPlazaPropShow_StartFade(WiFiPlazaPropShow *show);
BOOL WiFiPlazaPropShow_UpdateFade(WiFiPlazaPropShow *show);
void WiFiPlazaPropShow_Trigger(WiFiPlazaPropShow *show, fx32 animSpeed);
void WiFiPlazaPropShow_UpdateStage(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set);
void WiFiPlazaPropShow_ShowStage(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set);
void WiFiPlazaPropShow_UpdateBurst(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set);
void WiFiPlazaPropShow_ShowBurst(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set);
void WiFiPlazaProp_ApplyTagShift(WiFiPlazaProp *prop, int tag);

#endif // POKEHEARTGOLD_WIFI_PLAZA_MODELS_H
