#ifndef POKEHEARTGOLD_WIFI_PLAZA_EVENT_H
#define POKEHEARTGOLD_WIFI_PLAZA_EVENT_H

#include "global.h"

#include "heap.h"
#include "wifi_plaza_light.h"
#include "wifi_plaza_map.h"
#include "wifi_plaza_room.h"

typedef struct WiFiPlazaEventModelList {
    const u8 *ids;
    u32 count;
} WiFiPlazaEventModelList;

// Plaza-wide effect selected by the lobby (6 kinds, init and per-frame tables in rodata)
typedef struct WiFiPlazaEventEffect {
    WiFiPlazaWork *lobby;
    WiFiPlazaModels *models;
    u8 effect;       // 0x08
    u8 seLooping;    // 0x09 SEQ 0x5B5 is playing
    u16 activeCount; // 0x0A
    u32 step;        // 0x0C
} WiFiPlazaEventEffect;

// Rocking / hopping animation of one model (angle and height follow a sine over `duration` frames)
typedef struct WiFiPlazaEventWobble {
    u16 active;
    u16 kind;
    s16 frame;
    s16 duration;
    u16 maxAngle;
    s16 maxHeight;
    s16 repeats;
} WiFiPlazaEventWobble;

typedef struct WiFiPlazaMoverMarker {
    s16 x;
    s16 z;
} WiFiPlazaMoverMarker;

// Nine models whose x position follows lobby values; three markers from the map grid
typedef struct WiFiPlazaEventMovers {
    WiFiPlazaRoomWork *room;                 // 0x00
    WiFiPlazaWork *lobby;                    // 0x04
    WiFiPlazaModels *models;                 // 0x08
    WiFiPlazaMap *map;                       // 0x0C
    u8 states[9][3];                         // 0x10
    u8 unk_2B;                               // 0x2B
    WiFiPlazaEventWobble wobbles[9];         // 0x2C
    WiFiPlazaMoverMarker points[3]; // 0xAA cell positions (pixels) of the map tags
    u8 unk_B6[6];                            // 0xB6
    fx32 baseX;                              // 0xBC
    fx32 range;                              // 0xC0
} WiFiPlazaEventMovers;

typedef struct WiFiPlazaEvent {
    WiFiPlazaRoomWork *room;
    WiFiPlazaWork *lobby;
    WiFiPlazaModels *models;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaLight *light;
    WiFiPlazaEventMovers *movers;
    WiFiPlazaEventEffect *effect;
} WiFiPlazaEvent;

WiFiPlazaEvent *WiFiPlazaEvent_Create(enum HeapID heapID, WiFiPlazaRoomWork *room);
void WiFiPlazaEvent_Delete(WiFiPlazaEvent *event);
void WiFiPlazaEvent_Update(WiFiPlazaEvent *event);
u8 WiFiPlazaEvent_GetMoverState(WiFiPlazaEvent *event, u32 index, int field);
int WiFiPlazaEvent_GetColorSet(int value);
int WiFiPlazaEvent_GetLight1Set(int value);
int WiFiPlazaEvent_GetThirdSet(int value);
void WiFiPlazaEvent_TurnOffLight2(WiFiPlazaEvent *event);
WiFiPlazaEventEffect *WiFiPlazaEventEffect_Create(WiFiPlazaWork *lobby, WiFiPlazaModels *models, enum HeapID heapID);
void WiFiPlazaEventEffect_Delete(WiFiPlazaEventEffect *effect);
void WiFiPlazaEventEffect_Update(WiFiPlazaEventEffect *effect);
void WiFiPlazaEventEffect_RunPhase(WiFiPlazaEventEffect *effect, int index, int arg);
void WiFiPlazaEventEffect_UpdateNone(WiFiPlazaEventEffect *effect, u32 arg);
void WiFiPlazaEventEffect_UpdatePhase2(WiFiPlazaEventEffect *effect, u32 arg);
void WiFiPlazaEventEffect_UpdatePhase3(WiFiPlazaEventEffect *effect, u32 tick);
void WiFiPlazaEventEffect_UpdatePhase4(WiFiPlazaEventEffect *effect, u32 tick);
void WiFiPlazaEventEffect_UpdatePhase5(WiFiPlazaEventEffect *effect, u32 value);
void WiFiPlazaEventEffect_StartPhase(WiFiPlazaEventEffect *effect, u32 index, u32 arg);
void WiFiPlazaEventEffect_StartHideProps(WiFiPlazaEventEffect *effect);
void WiFiPlazaEventEffect_StartPhase2(WiFiPlazaEventEffect *effect);
void WiFiPlazaEventEffect_StartPhase3(WiFiPlazaEventEffect *effect);
void WiFiPlazaEventEffect_StartPhase4(WiFiPlazaEventEffect *effect);
void WiFiPlazaEventEffect_StartPhase5(WiFiPlazaEventEffect *effect);
void WiFiPlazaEvent_GetModelList(u32 kind, WiFiPlazaEventModelList *list);
void WiFiPlazaEvent_StartLoopSE(WiFiPlazaEventEffect *effect);
void WiFiPlazaEvent_StopLoopSE(WiFiPlazaEventEffect *effect);
BOOL WiFiPlazaTalk_Survey(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room);
WiFiPlazaEventMovers *WiFiPlazaEventMovers_Create(WiFiPlazaRoomWork *room, enum HeapID heapID);
void WiFiPlazaEventMovers_Delete(WiFiPlazaEventMovers *work);
void WiFiPlazaEventMovers_Update(WiFiPlazaEventMovers *work);
u8 WiFiPlazaEventMovers_GetState(WiFiPlazaEventMovers *work, u32 index, int field);
void WiFiPlazaEventMovers_UpdateX(WiFiPlazaEventMovers *work, int index);
void WiFiPlazaEventMovers_StartActions(WiFiPlazaEventMovers *work, u32 index, BOOL hasOwnSelection, int isOwnIndex, int ownColumn);
void WiFiPlazaEventMovers_PlaySE(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 seqNo);
void WiFiPlazaEventMovers_PlayCry(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 species);
BOOL WiFiPlazaEventWobble_Start(WiFiPlazaEventWobble *wobble, int kind, WiFiPlazaModels *models, int index);
BOOL WiFiPlazaEventWobble_Update(WiFiPlazaEventWobble *wobble, WiFiPlazaModels *models, int index);
void WiFiPlazaEventWobble_InitSmall(WiFiPlazaEventWobble *wobble);
void WiFiPlazaEventWobble_InitLarge(WiFiPlazaEventWobble *wobble);
BOOL WiFiPlazaEventWobble_Step(WiFiPlazaEventWobble *wobble, WiFiPlazaModels *models, int index);
void WiFiPlazaEvent_UpdateMoverMarkers(WiFiPlazaEventMovers *work, int index);

#endif // POKEHEARTGOLD_WIFI_PLAZA_EVENT_H
