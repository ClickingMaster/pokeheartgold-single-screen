#ifndef POKEHEARTGOLD_WIFI_PLAZA_INSTRUMENT_H
#define POKEHEARTGOLD_WIFI_PLAZA_INSTRUMENT_H

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "model_animation.h"
#include "wifi_plaza_camera.h"
#include "wifi_plaza_map.h"
#include "wifi_plaza_obj.h"

// Tap Toy (instrument) effects of the Wi-Fi Plaza room: one effect slot per player (20), each drawing up to
// 18 models from NARC 0xD1 above the player's object. There are 27 effect types (9 kinds x 3 levels), each
// with an init and an update handler (tables sWiFiPlazaInstrumentInitFuncs / sWiFiPlazaInstrumentUpdateFuncs)
// and a sound effect played when it starts.

#define WIFI_PLAZA_INSTRUMENT_PLAYERS       20
#define WIFI_PLAZA_INSTRUMENT_EFFECT_TYPES  27
#define WIFI_PLAZA_INSTRUMENT_EFFECT_MODELS 18
#define WIFI_PLAZA_INSTRUMENT_NO_ANIM       17 // WiFiPlazaInstrumentModelDesc.anims: unused slot

// Linear move of a position over `duration` frames
typedef struct WiFiPlazaInstrumentTween {
    s32 duration;
    fx32 x;
    fx32 y;
    fx32 z;
    fx32 deltaX;
    fx32 startX;
    fx32 deltaY;
    fx32 startY;
    fx32 deltaZ;
    fx32 startZ;
} WiFiPlazaInstrumentTween;

// Sine wave: value = sin(angle) * amplitude, angle advanced by `step` every update
typedef struct WiFiPlazaInstrumentWave {
    u16 angle;
    u16 step;
    fx32 amplitude;
    fx32 value;
} WiFiPlazaInstrumentWave;

// Entry of sWiFiPlazaInstrumentModelDescs: which model, texture file and animations a model slot uses
typedef struct WiFiPlazaInstrumentModelDesc {
    u8 model;   // index into WiFiPlazaInstrumentManager.models
    u8 texture; // index into WiFiPlazaInstrumentManager.textures
    u8 anims[2];
} WiFiPlazaInstrumentModelDesc;

// Four models flying out of the player and back (effect types 0-8)
typedef struct WiFiPlazaInstrumentBurst {
    s16 frame;                          // 0x00
    u16 kind;                           // 0x02 sWiFiPlazaInstrumentBurstStartFuncs / UpdateFuncs index (< 3)
    u16 count;                          // 0x04
    u8 active;                          // 0x06
    u8 phase;                           // 0x07
    WiFiPlazaInstrumentTween tweens[4]; // 0x08
    WiFiPlazaInstrumentWave waves[4];   // 0xA8
    ModelInstance *models[4];           // 0xD8
    WiFiPlazaObj *obj;                  // 0xE8
} WiFiPlazaInstrumentBurst;

// Schedule entry: at `frame`, start burst `burst` with `count` models
typedef struct WiFiPlazaInstrumentBurstEvent {
    u16 frame;
    u8 burst;
    u8 count;
} WiFiPlazaInstrumentBurstEvent;

// Bursts started on a schedule (effect types 0-8)
typedef struct WiFiPlazaInstrumentBurstWork {
    WiFiPlazaInstrumentBurst bursts[4];          // 0x000
    const WiFiPlazaInstrumentBurstEvent *events; // 0x3B0
    u16 eventCount;                              // 0x3B4
    u16 duration;                                // 0x3B6
    u16 flashActive;                             // 0x3B8 models 16 and 17
    u16 flashFrame;                              // 0x3BA
} WiFiPlazaInstrumentBurstWork;

typedef struct WiFiPlazaInstrumentEffect {
    u8 type;                                                                        // 0x000
    u8 phase;                                                                       // 0x001
    s16 frame;                                                                      // 0x002
    u32 polygonID;                                                                  // 0x004 object parameter 5 + 0x28
    WiFiPlazaObj *obj;                                                              // 0x008 NULL: slot free
    ModelInstance models[WIFI_PLAZA_INSTRUMENT_EFFECT_MODELS];                      // 0x00C
    const WiFiPlazaInstrumentModelDesc *descs[WIFI_PLAZA_INSTRUMENT_EFFECT_MODELS]; // 0x87C
    fx32 animFrames[WIFI_PLAZA_INSTRUMENT_EFFECT_MODELS][2];                        // 0x8C4

    union {
        u8 raw[0x3BC];

        WiFiPlazaInstrumentBurstWork burst; // types 0-8

        struct { // types 9-11
            s8 count;
            s8 shown;
        } rise;

        struct { // types 12-14
            int lastStep;
        } sequence;

        struct { // types 15-17
            s16 shown;
            s16 count;
        } stack;

        struct { // types 18-20
            s32 delay;
            u32 fade[3];
        } fade;

        struct { // types 21-23
            s8 tick;
            s8 rows;
            u8 totalRows;
            u8 shown;
            s8 timers[16];
            WiFiPlazaInstrumentTween tweens[16];
        } shower;

        struct {                                // types 24-26
            s8 stage;                           // 0x954
            s8 count;                           // 0x955
            s16 timer;                          // 0x956
            fx32 baseY;                         // 0x958
            u8 animTimers[4];                   // 0x95C
            u8 animKinds[4];                    // 0x960
            u8 tick;                            // 0x964
            u8 onTile42;                        // 0x965 the object stands on map attribute 42
            WiFiPlazaInstrumentWave waves[3];   // 0x968
            WiFiPlazaInstrumentTween tweens[4]; // 0x98C [3] moves the player's object
            WiFiPlazaInstrumentWave objWave;    // 0xA2C
        } hop;
    } work; // 0x954
} WiFiPlazaInstrumentEffect;

typedef struct WiFiPlazaInstrumentManager {
    struct WiFiPlazaWork *lobby;                                      // 0x00000 OVY_45 lobby
    WiFiPlazaObjManager *objManager;                                  // 0x00004
    WiFiPlazaCamera *camera;                                          // 0x00008
    WiFiPlazaMap *map;                                                // 0x0000C
    WiFiPlazaInstrumentEffect effects[WIFI_PLAZA_INSTRUMENT_PLAYERS]; // 0x00010
    ModelResource models[15];                                         // 0x10550 NARC 0xD1 members 0x81-0x8F
    NNSG3dResFileHeader *textures[39];                                // 0x10640 members 0x90-0xB6
    ModelAnimation anims[16];                                         // 0x106DC members 0xB7-0xC6
    NNSFndAllocator allocator;                                        // 0x1081C
} WiFiPlazaInstrumentManager;

typedef void (*WiFiPlazaInstrumentInitFunc)(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
typedef BOOL (*WiFiPlazaInstrumentUpdateFunc)(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);

WiFiPlazaInstrumentManager *WiFiPlazaInstrument_Create(struct WiFiPlazaWork *lobby, WiFiPlazaMap *map, WiFiPlazaCamera *camera, WiFiPlazaObjManager *objManager, enum HeapID heapID, enum HeapID gfxHeapID);
void WiFiPlazaInstrument_Delete(WiFiPlazaInstrumentManager *manager);
void WiFiPlazaInstrument_Update(WiFiPlazaInstrumentManager *manager);
void WiFiPlazaInstrument_Draw(WiFiPlazaInstrumentManager *manager);
void WiFiPlazaInstrument_Start(WiFiPlazaInstrumentManager *manager, u32 index, u32 type);
BOOL WiFiPlazaInstrument_IsFinished(WiFiPlazaInstrumentManager *manager, u32 index);

void WiFiPlazaInstrumentTween_Init(WiFiPlazaInstrumentTween *tween, fx32 x0, fx32 x1, fx32 y0, fx32 y1, fx32 z0, fx32 z1, s32 duration);
BOOL WiFiPlazaInstrumentTween_Update(WiFiPlazaInstrumentTween *tween, s32 frame);
void WiFiPlazaInstrumentTween_GetPosition(WiFiPlazaInstrumentTween *tween, fx32 *x, fx32 *y, fx32 *z);
void WiFiPlazaInstrumentWave_Init(WiFiPlazaInstrumentWave *wave, int angle, int step, int amplitude);
void WiFiPlazaInstrumentWave_Update(WiFiPlazaInstrumentWave *wave);
void WiFiPlazaInstrumentWave_GetValue(WiFiPlazaInstrumentWave *wave, fx32 *value);

// Internal to src/wifi_plaza_instrument.c
void WiFiPlazaInstrument_PlaySE(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 seqNo);
void WiFiPlazaInstrument_LoadModels(WiFiPlazaInstrumentManager *manager, NARC *narc, enum HeapID heapID);
void WiFiPlazaInstrument_FreeModels(WiFiPlazaInstrumentManager *manager);
void WiFiPlazaInstrument_LoadTextures(WiFiPlazaInstrumentManager *manager, NARC *narc, enum HeapID heapID);
void WiFiPlazaInstrument_FreeTextures(WiFiPlazaInstrumentManager *manager);
void WiFiPlazaInstrument_LoadAnimations(WiFiPlazaInstrumentManager *manager, NARC *narc, enum HeapID heapID);
void WiFiPlazaInstrument_FreeAnimations(WiFiPlazaInstrumentManager *manager);
void WiFiPlazaInstrument_StartEffect(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, WiFiPlazaObj *obj, u32 type);
void WiFiPlazaInstrument_UpdateEffect(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_DrawEffect(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_ClearEffect(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_IsEffectActive(WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_HasModel(WiFiPlazaInstrumentEffect *effect, u32 index);
void WiFiPlazaInstrument_SetModel(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, const WiFiPlazaInstrumentModelDesc *desc);
void WiFiPlazaInstrument_DrawModel(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index);
void WiFiPlazaInstrument_LoopAnim(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim);
BOOL WiFiPlazaInstrument_PlayAnim(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim);
void WiFiPlazaInstrument_LoopAnimBy(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim, fx32 step);
BOOL WiFiPlazaInstrument_PlayAnimBy(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, int index, int anim, int step);
void WiFiPlazaInstrument_SetAnimFrame(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim, fx32 frame);
fx32 WiFiPlazaInstrument_GetAnimFrame(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim);
void WiFiPlazaInstrument_InitRipple01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitRipple02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitRipple03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitWhirl01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitWhirl02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitWhirl03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitSignal(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitPop01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitPop02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitPop03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitBalloon01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitBalloon02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitBalloon03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitSparkle01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitSparkle02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitSparkle03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitBell01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitBell02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitBell03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitDrum01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitDrum02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitDrum03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitCymbal01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitCymbal02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitCymbal03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateRipple(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateSignal01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateSignal02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateSignal03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateWhirl(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdatePop(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateBalloon(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateSparkle(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateBellDrum(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateCymbal(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_DrawEffectModels(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitBalloon(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, int count);
void WiFiPlazaInstrument_StartBalloonRise(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_StartBalloonLift(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateBalloonUp(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateBalloonRise(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateBalloonLift(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_StartBalloonDown(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_StartBalloonFall(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_StartBalloonDrop(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateBalloonDown(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateBalloonFall(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_UpdateBalloonDrop(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_PlaceBalloons(WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_PlaceBalloonsLifted(WiFiPlazaInstrumentEffect *effect);
BOOL WiFiPlazaInstrument_SetBalloonAnim(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u8 kind);
void WiFiPlazaInstrument_UpdateBalloonAnim(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index);
void WiFiPlazaInstrument_InitSparkle(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 rows);
BOOL WiFiPlazaInstrument_UpdateSparkleModel(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, int index);
void WiFiPlazaInstrument_InitRipple(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 count);
BOOL WiFiPlazaInstrument_UpdateSignalSequence(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, int kind);
void WiFiPlazaInstrument_InitWhirl(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 count);
void WiFiPlazaInstrument_InitCymbalFlash(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_StartCymbalFlash(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_UpdateCymbalFlash(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect);
void WiFiPlazaInstrument_InitBursts(WiFiPlazaInstrumentBurstWork *work, const WiFiPlazaInstrumentBurstEvent *events, u16 eventCount, u16 duration, u32 kind, WiFiPlazaInstrumentEffect *effect);
int WiFiPlazaInstrument_UpdateBursts(WiFiPlazaInstrumentBurstWork *work, u32 frame);
void WiFiPlazaInstrumentBurst_Init(WiFiPlazaInstrumentBurst *burst, ModelInstance *model0, ModelInstance *model1, ModelInstance *model2, ModelInstance *model3, WiFiPlazaObj *obj, u32 kind);
void WiFiPlazaInstrumentBurst_Start(WiFiPlazaInstrumentBurst *burst, u32 count);
BOOL WiFiPlazaInstrumentBurst_Update(WiFiPlazaInstrumentBurst *burst);
void WiFiPlazaInstrumentBurst_StartBell(WiFiPlazaInstrumentBurst *burst, WiFiPlazaObj *obj);
BOOL WiFiPlazaInstrumentBurst_UpdateBell(WiFiPlazaInstrumentBurst *burst);
void WiFiPlazaInstrumentBurst_StartDrum(WiFiPlazaInstrumentBurst *burst, WiFiPlazaObj *obj);
BOOL WiFiPlazaInstrumentBurst_UpdateDrum(WiFiPlazaInstrumentBurst *burst);
void WiFiPlazaInstrumentBurst_StartCymbal(WiFiPlazaInstrumentBurst *burst, WiFiPlazaObj *obj);
BOOL WiFiPlazaInstrumentBurst_UpdateCymbal(WiFiPlazaInstrumentBurst *burst);
void WiFiPlazaInstrumentBurst_StartHop(WiFiPlazaInstrumentBurst *burst, u32 index, VecFx32 *origin, fx32 dx01, fx32 dy01, fx32 dx23, fx32 dy23, s32 duration);

#endif // POKEHEARTGOLD_WIFI_PLAZA_INSTRUMENT_H
