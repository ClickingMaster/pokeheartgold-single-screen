#ifndef POKEHEARTGOLD_WIFI_PLAZA_MODELS_H
#define POKEHEARTGOLD_WIFI_PLAZA_MODELS_H

#include "global.h"

#include "wifi_plaza/wifi_plaza_map.h"

#include "filesystem.h"
#include "heap.h"
#include "model_animation.h"

// 3D scenery of the Wi-Fi Plaza room: the base scene, props placed on the map cells that carry a spawn tag, and
// movers whose position, angle and one-shot animations are driven by the plaza events.

#define WIFI_PLAZA_PROP_KIND_COUNT  18
#define WIFI_PLAZA_PROP_ANIM_COUNT  3
#define WIFI_PLAZA_MOVER_ANIM_COUNT 4
#define WIFI_PLAZA_BASE_ANIM_COUNT  5

// Animation of the paired props (kinds 11 and 12)
enum WiFiPlazaPairAnimMode {
    WIFI_PLAZA_PAIR_ANIM_REST,
    WIFI_PLAZA_PAIR_ANIM_OPEN,
    WIFI_PLAZA_PAIR_ANIM_CLOSE,
    WIFI_PLAZA_PAIR_ANIM_LOOP,
};

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
    u32 propModels[WIFI_PLAZA_PROP_KIND_COUNT];
    u32 propAnims[WIFI_PLAZA_PROP_KIND_COUNT][WIFI_PLAZA_PROP_ANIM_COUNT];
    u32 moverModels[2];
    u32 moverTextures[2][3];
    u32 moverAnims[2][WIFI_PLAZA_MOVER_ANIM_COUNT];
    u32 moverNodeCounts[2];
    u32 moverAnimNodes[2][WIFI_PLAZA_MOVER_ANIM_COUNT - 1]; // the only node animations 1-3 move
    u32 baseModels[2];
    u32 unk_188;
    u32 baseAnims[WIFI_PLAZA_BASE_ANIM_COUNT]; // animation 3 belongs to model 1
} WiFiPlazaModelsLayout;

typedef struct WiFiPlazaBaseResources {
    ModelResource models[2];
    ModelAnimation anims[WIFI_PLAZA_BASE_ANIM_COUNT];
    BOOL animLoaded[WIFI_PLAZA_BASE_ANIM_COUNT];
} WiFiPlazaBaseResources;

typedef struct WiFiPlazaMoverResources {
    ModelResource models[2]; // texture swapped in before drawing
    NNSG3dResFileHeader *textures[2][3];
    ModelAnimation anims[2][WIFI_PLAZA_MOVER_ANIM_COUNT];
} WiFiPlazaMoverResources;

typedef struct WiFiPlazaPropResources {
    ModelResource models[WIFI_PLAZA_PROP_KIND_COUNT];
    void *animFiles[WIFI_PLAZA_PROP_KIND_COUNT][WIFI_PLAZA_PROP_ANIM_COUNT]; // NULL = none
} WiFiPlazaPropResources;

// Animations 0 and 2 loop all the time, 1 and 3 are played once on request.
typedef struct WiFiPlazaMover {
    u8 active;
    u8 texture;
    u8 model;
    ModelInstance instance;
    BOOL animPlaying[WIFI_PLAZA_MOVER_ANIM_COUNT];
    fx32 animFrames[WIFI_PLAZA_MOVER_ANIM_COUNT];
    VecFx32 position;
    VecFx32 offset; // added to position
} WiFiPlazaMover;

struct WiFiPlazaProp {
    u16 active;
    u16 kind;
    ModelInstance instance;
    ModelAnimation anims[WIFI_PLAZA_PROP_ANIM_COUNT];
    u8 animActive[WIFI_PLAZA_PROP_ANIM_COUNT];
    u8 unk_BB;
    u8 animModes[WIFI_PLAZA_PROP_ANIM_COUNT];
    u8 unk_BF;
    fx32 animFrames[WIFI_PLAZA_PROP_ANIM_COUNT];
    u8 randomDelay; // delays are rolled below this
    u8 animDelays[WIFI_PLAZA_PROP_ANIM_COUNT];
    WiFiPlazaPropCallback animCallbacks[WIFI_PLAZA_PROP_ANIM_COUNT];
    fx32 animSpeed; // frames per update
    u8 alphaOverride;
    u8 alpha;        // model alpha while drawing
    u8 restoreAlpha; // model alpha afterwards
};

struct WiFiPlazaModelSet {
    BOOL baseVisible;
    ModelInstance baseInstances[2];
    BOOL baseAnimActive[WIFI_PLAZA_BASE_ANIM_COUNT];
    fx32 baseAnimFrames[WIFI_PLAZA_BASE_ANIM_COUNT];
    WiFiPlazaMover *movers;
    WiFiPlazaProp *props;
    u8 moverCount;
    u8 propCount;
    u8 layoutVariant;
    u8 layoutGroup;
    BOOL loaded;
    WiFiPlazaBaseResources baseRes;
    WiFiPlazaMoverResources moverRes;
    WiFiPlazaPropResources propRes;
    NNSFndAllocator allocator;
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
    WiFiPlazaMap *map;
    WiFiPlazaModelSet *set;
    WiFiPlazaProp *props[128];
    WiFiPlazaMover *movers[9];
    WiFiPlazaProp *matchBooths[3]; // props of kinds 6-8, one per matchmaking session
    WiFiPlazaProp *matchSigns[3];  // kind 10 props (tags 0x49-0x4B): player count display
    BOOL pairAnimState;
    WiFiPlazaPropShow effectShows[24]; // kind 13
    WiFiPlazaPropShow stageShows[24];  // kind 14
    WiFiPlazaProp *effectLoops[24];    // kind 15
    WiFiPlazaPropShow stageBursts[24]; // kind 16
    u8 effectShowCount;
    u8 stageShowCount;
    u8 effectLoopCount;
    u8 stageBurstCount;
    u32 blinkTimer;
    u8 layoutGroup;
    u8 layoutVariant; // layout file variant + group * 5
    u8 propCount;
    u8 moverCount;
} WiFiPlazaModels;

typedef struct WiFiPlazaModelsPos {
    s16 x;
    s16 y;
} WiFiPlazaModelsPos;

void WiFiPlazaModelSet_StartPropAnim(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot, s32 mode);
void WiFiPlazaModelSet_SetPropAnimFrame(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, int slot, fx32 frame);

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

#endif // POKEHEARTGOLD_WIFI_PLAZA_MODELS_H
