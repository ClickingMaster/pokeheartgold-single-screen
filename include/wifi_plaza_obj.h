#ifndef POKEHEARTGOLD_WIFI_PLAZA_OBJ_H
#define POKEHEARTGOLD_WIFI_PLAZA_OBJ_H

#include "global.h"

#include "heap.h"
#include "overlay_42.h"
#include "wifi_plaza_map.h"

// Objects of the Wi-Fi Plaza room: an OVY_42 grid object (cell position, direction, queued moves) paired with an
// OVY_45 billboard avatar, driven by a per-object handler (table sWiFiPlazaObjHandlers).
// Directions: 0 up, 1 down, 2 left, 3 right. Move commands: 1 turn, 2 walk, 3 run.

typedef struct WiFiPlazaObj WiFiPlazaObj;
typedef struct WiFiPlazaObjManager WiFiPlazaObjManager;

typedef void (*WiFiPlazaObjFunc)(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);

typedef struct WiFiPlazaObjTween {
    s32 value;
    s32 start;
    s32 delta;
    u32 duration;
} WiFiPlazaObjTween;

struct WiFiPlazaObj {
    UnkStruct_ov42_02228110 *gridObj; // 0x00
    void *avatar;                     // 0x04
    u16 step;                         // 0x08
    u8 finished;                      // 0x0A
    u8 handler;                       // 0x0B

    union {
        u8 raw[0x18];

        struct {
            s16 timer;
            s16 direction;
        } wander;

        struct {
            WiFiPlazaObjTween tween;
            u32 frame;
        } lift;

        struct {
            WiFiPlazaObjTween tween;
            s32 frame;
            s32 start;
        } slide;

        struct {
            s32 frame;
            VecFx32 origin;
        } hop;
    } work; // 0x0C

    WiFiPlazaObjFunc update; // 0x24
};

// Brightness of the avatars, faded over 60 frames when the room's light set changes
typedef struct WiFiPlazaObjLight {
    u8 active;
    u8 value;
    s8 start;
    s8 delta;
    s16 frame;
    s16 duration;
} WiFiPlazaObjLight;

struct WiFiPlazaObjManager {
    UnkStruct_ov42_022280A8 *gridObjs; // 0x00
    void *avatars;                     // 0x04 OVY_45 avatar manager
    WiFiPlazaObj *objs;                // 0x08
    u16 count;                         // 0x0C
    u16 unk_0E;                        // 0x0E nonzero: own player avatar 0x61
    UnkStruct_ov42_02229A40 *commands; // 0x10 queued move commands
    WiFiPlazaMap *map;                 // 0x14
    WiFiPlazaObjLight light;           // 0x18
    WiFiPlazaObj *ownPlayer;           // 0x20
    WiFiPlazaObj *special;             // 0x24 object spawned on map tag 97
};

WiFiPlazaObjManager *WiFiPlazaObj_CreateManager(u32 count, u32 unk_0E, WiFiPlazaMap *map, enum HeapID heapID, u32 arg4);
void WiFiPlazaObj_DeleteManager(WiFiPlazaObjManager *manager);
void ov49_02258B44(WiFiPlazaObjManager *manager);
void WiFiPlazaObj_UpdateManager(WiFiPlazaObjManager *manager);
u32 ov49_02258BD4(WiFiPlazaObjManager *manager);
u32 ov49_02258BE0(WiFiPlazaObjManager *manager);
void WiFiPlazaObj_SetLightSet(WiFiPlazaObjManager *manager, int set);
void WiFiPlazaObj_FadeToLightSet(WiFiPlazaObjManager *manager, int set);
void WiFiPlazaObj_PushCommand(WiFiPlazaObjManager *manager, UnkStruct_ov42_02228CDC *command);
WiFiPlazaObj *WiFiPlazaObj_CreateOwnPlayer(WiFiPlazaObjManager *manager, int id);
WiFiPlazaObj *WiFiPlazaObj_CreateOwnPlayerAt(WiFiPlazaObjManager *manager, int id, int x, int y);
WiFiPlazaObj *WiFiPlazaObj_CreatePlayer(WiFiPlazaObjManager *manager, int id, int avatar);
void WiFiPlazaObj_Delete(WiFiPlazaObj *obj);
WiFiPlazaObj *WiFiPlazaObj_FindById(WiFiPlazaObjManager *manager, int id);
WiFiPlazaObj *WiFiPlazaObj_GetOwnPlayer(WiFiPlazaObjManager *manager);
WiFiPlazaObj *WiFiPlazaObj_GetSpecial(WiFiPlazaObjManager *manager);
void WiFiPlazaObj_SetGridPos(WiFiPlazaObj *obj, UnkStruct_ov44_02232914 pos);
void WiFiPlazaObj_SetGridPosAndDir(WiFiPlazaObj *obj, UnkStruct_ov44_02232914 pos, s32 direction);
UnkStruct_ov44_02232914 WiFiPlazaObj_GetGridPos(WiFiPlazaObj *obj);
s32 WiFiPlazaObj_GetParam(WiFiPlazaObj *obj, s32 param);
void WiFiPlazaObj_QueueCommand(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, s32 command, s32 direction);
void ov49_02258EAC(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, u16 value, u8 kind);
void WiFiPlazaObj_SetHandler(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, int handler);
u8 WiFiPlazaObj_IsFinished(WiFiPlazaObj *obj);
u8 WiFiPlazaObj_GetHandler(WiFiPlazaObj *obj);
WiFiPlazaObj *WiFiPlazaObj_FindFacing(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj);
u32 ov49_02258F70(WiFiPlazaObj *obj);
WiFiPlazaObj *WiFiPlazaObj_FindAt(WiFiPlazaObjManager *manager, s32 x, s32 y);
BOOL WiFiPlazaObj_IsOnCell(WiFiPlazaObj *obj, s32 x, s32 y);
BOOL WiFiPlazaObj_FindFreeNeighbor(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, s32 *direction, UnkStruct_ov44_02232914 *position);
void ov49_02259130(WiFiPlazaObj *obj, int enable);
void ov49_0225913C(WiFiPlazaObj *obj, void *out);
void WiFiPlazaObj_SetPosition(WiFiPlazaObj *obj, VecFx32 *position);
void WiFiPlazaObj_GetPosition(WiFiPlazaObj *obj, VecFx32 *position);
void WiFiPlazaObj_SetDirection(WiFiPlazaObj *obj, u32 direction);
void ov49_0225916C(WiFiPlazaObj *obj, u32 enable);
void ov49_02259184(WiFiPlazaObj *obj, u32 enable);
void ov49_0225919C(WiFiPlazaObj *obj, u32 enable);
void ov49_022591B4(WiFiPlazaObj *obj, u32 value);
void ov49_022591C0(WiFiPlazaObj *obj, u32 value);
u32 ov49_022591CC(WiFiPlazaObj *obj);
void WiFiPlazaObj_SpawnMapObjects(WiFiPlazaObjManager *manager);
void WiFiPlazaObj_QueueMove(WiFiPlazaObjManager *manager, int command, int direction, int id);
u8 WiFiPlazaObj_RandomPick(const u8 *values, u32 count);
BOOL WiFiPlazaObj_IsTagInDirection(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, s32 direction, int tag);
void WiFiPlazaObj_InitTween(WiFiPlazaObjTween *tween, s32 start, s32 end, u32 duration);
BOOL WiFiPlazaObj_UpdateTween(WiFiPlazaObjTween *tween, u32 frame);
s32 WiFiPlazaObj_GetTweenValue(WiFiPlazaObjTween *tween);
WiFiPlazaObj *WiFiPlazaObj_AllocObj(WiFiPlazaObjManager *manager);
BOOL WiFiPlazaObj_IsFree(WiFiPlazaObj *obj);
void WiFiPlazaObj_HandlerIdle(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerPlayerInput(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerWander(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerRiseOut(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerDropIn(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerSlideLeft(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerSlideRight(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerSlideUp(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerSlideDown(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_SlideOneCell(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager, u32 direction);
void WiFiPlazaObj_HandlerHop(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_HandlerResetHeight(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);
void WiFiPlazaObj_InitLight(WiFiPlazaObjLight *light, void *avatars, u32 value);
void WiFiPlazaObj_StartLightFade(WiFiPlazaObjLight *light, u32 value);
BOOL WiFiPlazaObj_UpdateLight(WiFiPlazaObjLight *light, void *avatars);

// OVY_45 avatar manager (no header yet), with the types this overlay calls them with
void *WiFiPlazaPlayerObjects_New(u32 count, u32 arg1, enum HeapID heapID, u32 arg3);
void WiFiPlazaPlayerObjects_Delete(void *avatars);
void WiFiPlazaPlayerObjects_Update(void *avatars);
u32 WiFiPlazaPlayerObjects_Draw(void *avatars);
u32 ov45_022306F4(void *avatars);
void WiFiPlazaPlayerObjects_SetModelAlpha(void *avatars, int value);
void *WiFiPlazaPlayerObjects_Add(void *avatars, UnkStruct_ov42_02228110 *gridObj);
void WiFiPlazaPlayerObject_Delete(void *avatar);
void WiFiPlazaPlayerObject_SetMode(void *avatar, int enable);
u32 WiFiPlazaPlayerObject_SetLightFlags(void *avatar, u32 value);
u32 WiFiPlazaPlayerObject_IsCulled(void *avatar);
void WiFiPlazaPlayerObject_SetGridPosition(void *avatar, void *out);
void WiFiPlazaPlayerObject_SetPosition(void *avatar, VecFx32 *position);
void WiFiPlazaPlayerObject_GetPosition(void *avatar, VecFx32 *position);
void WiFiPlazaPlayerObject_SetStandingSequence(void *avatar, u32 direction);
void WiFiPlazaPlayerObject_StartEffect(void *avatar, int kind);
void WiFiPlazaPlayerObject_StopEffect(void *avatar);
void WiFiPlazaPlayerObject_SetEffectStep(void *avatar, u32 value);
void WiFiPlazaPlayerObject_SetVisible(void *avatar, u32 value);
u32 WiFiPlazaPlayerObject_IsVisible(void *avatar);

#endif // POKEHEARTGOLD_WIFI_PLAZA_OBJ_H
