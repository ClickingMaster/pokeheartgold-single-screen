#ifndef POKEHEARTGOLD_WIFI_PLAZA_EVENT_H
#define POKEHEARTGOLD_WIFI_PLAZA_EVENT_H

#include "global.h"

#include "heap.h"
#include "wifi_plaza_map.h"
#include "wifi_plaza_room.h"

// Light and material colours of the room, each faded over 60 frames to a new colour set.
// Colours are 5-bit r, g, b components (4th byte unused).
typedef struct WiFiPlazaLight {
    u8 lights[2][4];        // 0x00 lights 0 and 1
    u8 materials[4][4];     // 0x08 diffuse, ambient, specular, emission
    u8 lightsFrom[2][4];    // 0x18
    u8 materialsFrom[4][4]; // 0x20
    u8 lightsTo[2][4];      // 0x30
    u8 materialsTo[4][4];   // 0x38
    s32 lightTimers[2];     // 0x48 frame of the fade, 60 = done
    s32 materialTimers[4];  // 0x50
} WiFiPlazaLight;

typedef struct WiFiPlazaEventModelList {
    const u8 *ids;
    u32 count;
} WiFiPlazaEventModelList;

// Plaza-wide effect selected by the lobby (6 kinds, init and per-frame tables in rodata)
typedef struct UnkStruct_ov49_02268A0C {
    WiFiPlazaWork *lobby;
    void *models;
    u8 effect;       // 0x08
    u8 seLooping;    // 0x09 SEQ 0x5B5 is playing
    u16 activeCount; // 0x0A
    u32 step;        // 0x0C
} UnkStruct_ov49_02268A0C;

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

typedef struct UnkStruct_ov49_02268FAC_Point {
    s16 x;
    s16 z;
} UnkStruct_ov49_02268FAC_Point;

// Nine models whose x position follows lobby values; three markers from the map grid
typedef struct UnkStruct_ov49_02268FAC {
    WiFiPlazaRoomWork *room;                 // 0x00
    WiFiPlazaWork *lobby;                    // 0x04
    void *models;                            // 0x08
    WiFiPlazaMap *map;                       // 0x0C
    u8 states[9][3];                         // 0x10
    u8 unk_2B;                               // 0x2B
    WiFiPlazaEventWobble wobbles[9];         // 0x2C
    UnkStruct_ov49_02268FAC_Point points[3]; // 0xAA cell positions (pixels) of the map tags
    u8 unk_B6[6];                            // 0xB6
    fx32 baseX;                              // 0xBC
    fx32 range;                              // 0xC0
} UnkStruct_ov49_02268FAC;

typedef struct WiFiPlazaEvent {
    WiFiPlazaRoomWork *room;
    WiFiPlazaWork *lobby;
    void *models;
    void *objManager;
    WiFiPlazaLight *light;
    UnkStruct_ov49_02268FAC *unk14;
    UnkStruct_ov49_02268A0C *unk18;
} WiFiPlazaEvent;

WiFiPlazaLight *WiFiPlazaLight_Create(enum HeapID heapID, u32 colorSet, u32 light1Set, u32 thirdSet);
void WiFiPlazaLight_Delete(WiFiPlazaLight *light);
void WiFiPlazaLight_Update(WiFiPlazaLight *light);
void WiFiPlazaLight_FadeToColorSet(WiFiPlazaLight *light, int set);
void WiFiPlazaLight_FadeLight1(WiFiPlazaLight *light, int set);
void ov49_02268620(WiFiPlazaLight *light, int set);
void WiFiPlazaLight_PackColor(const s8 *rgb, u16 *color);
void WiFiPlazaLight_UnpackColor(s8 *rgb, const u16 *color);
void WiFiPlazaLight_LerpColor(const s8 *start, const s8 *end, s8 *dest, s32 step, s32 total);
void WiFiPlazaLight_UnpackColorSet(s8 *dest, const u16 *colors);
void WiFiPlazaLight_UnpackLight1(WiFiPlazaLight *light, const u16 *color);
void WiFiPlazaLight_Apply(WiFiPlazaLight *light);

WiFiPlazaEvent *WiFiPlazaEvent_Create(enum HeapID heapID, WiFiPlazaRoomWork *room);
void WiFiPlazaEvent_Delete(WiFiPlazaEvent *event);
void WiFiPlazaEvent_Update(WiFiPlazaEvent *event);
u8 ov49_02268968(WiFiPlazaEvent *event, u32 index, int field);
int WiFiPlazaEvent_GetColorSet(int value);
int WiFiPlazaEvent_GetLight1Set(int value);
int ov49_022689D4(int value);
void WiFiPlazaEvent_TurnOffLight2(WiFiPlazaEvent *event);
UnkStruct_ov49_02268A0C *ov49_02268A0C(WiFiPlazaWork *lobby, void *models, enum HeapID heapID);
void ov49_02268A6C(UnkStruct_ov49_02268A0C *effect);
void ov49_02268A7C(UnkStruct_ov49_02268A0C *effect);
void ov49_02268ADC(UnkStruct_ov49_02268A0C *effect, int index, int arg);
void ov49_02268B04(UnkStruct_ov49_02268A0C *effect, u32 arg);
void ov49_02268B08(UnkStruct_ov49_02268A0C *effect, u32 arg);
void ov49_02268B0C(UnkStruct_ov49_02268A0C *effect, u32 tick);
void ov49_02268B90(UnkStruct_ov49_02268A0C *effect, u32 tick);
void ov49_02268C2C(UnkStruct_ov49_02268A0C *effect, u32 value);
void ov49_02268C74(UnkStruct_ov49_02268A0C *effect, u32 index, u32 arg);
void ov49_02268CAC(UnkStruct_ov49_02268A0C *effect);
void ov49_02268CBC(UnkStruct_ov49_02268A0C *effect);
void ov49_02268CCC(UnkStruct_ov49_02268A0C *effect);
void ov49_02268CDC(UnkStruct_ov49_02268A0C *effect);
void ov49_02268CEC(UnkStruct_ov49_02268A0C *effect);
void WiFiPlazaEvent_GetModelList(u32 kind, WiFiPlazaEventModelList *list);
void WiFiPlazaEvent_StartLoopSE(UnkStruct_ov49_02268A0C *effect);
void WiFiPlazaEvent_StopLoopSE(UnkStruct_ov49_02268A0C *effect);
BOOL ov49_02268DCC(void *state, WiFiPlazaRoomWork *room);
UnkStruct_ov49_02268FAC *ov49_02268FAC(WiFiPlazaRoomWork *room, enum HeapID heapID);
void ov49_02269090(UnkStruct_ov49_02268FAC *work);
void ov49_02269098(UnkStruct_ov49_02268FAC *work);
u8 ov49_02269154(UnkStruct_ov49_02268FAC *work, u32 index, int field);
void ov49_02269178(UnkStruct_ov49_02268FAC *work, int index);
void ov49_02269240(UnkStruct_ov49_02268FAC *work, u32 index, BOOL hasOwnSelection, int isOwnIndex, int ownColumn);
void ov49_022693A4(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 seqNo);
void ov49_022693D4(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 species);
BOOL ov49_022693F8(WiFiPlazaEventWobble *wobble, int kind, void *models, int index);
BOOL ov49_02269430(WiFiPlazaEventWobble *wobble, void *models, int index);
void ov49_02269474(WiFiPlazaEventWobble *wobble);
void ov49_02269494(WiFiPlazaEventWobble *wobble);
BOOL ov49_022694B4(WiFiPlazaEventWobble *wobble, void *models, int index);
void ov49_022695C4(UnkStruct_ov49_02268FAC *work, int index);

#endif // POKEHEARTGOLD_WIFI_PLAZA_EVENT_H
