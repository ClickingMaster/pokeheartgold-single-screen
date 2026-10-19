#ifndef POKEHEARTGOLD_WIFI_PLAZA_OBJ_H
#define POKEHEARTGOLD_WIFI_PLAZA_OBJ_H

#include "global.h"

#include "wifi_plaza/wifi_plaza_map.h"

#include "heap.h"
#include "overlay_42.h"

// Objects of the Wi-Fi Plaza room: a grid object (cell position, direction, queued moves) paired with a billboard
// avatar. Directions: 0 up, 1 down, 2 left, 3 right. Move commands: 1 turn, 2 walk, 3 run.

typedef struct WiFiPlazaObj WiFiPlazaObj;
typedef struct WiFiPlazaObjManager WiFiPlazaObjManager;

typedef void (*WiFiPlazaObjFunc)(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager);

enum WiFiPlazaObjHandler {
    WIFI_PLAZA_OBJ_HANDLER_IDLE,
    WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT,
    WIFI_PLAZA_OBJ_HANDLER_WANDER,
    WIFI_PLAZA_OBJ_HANDLER_RISE_OUT,
    WIFI_PLAZA_OBJ_HANDLER_DROP_IN,
    WIFI_PLAZA_OBJ_HANDLER_SLIDE_UP,
    WIFI_PLAZA_OBJ_HANDLER_SLIDE_DOWN,
    WIFI_PLAZA_OBJ_HANDLER_SLIDE_LEFT,
    WIFI_PLAZA_OBJ_HANDLER_SLIDE_RIGHT,
    WIFI_PLAZA_OBJ_HANDLER_HOP,
};

typedef struct WiFiPlazaObjTween {
    s32 value;
    s32 start;
    s32 delta;
    u32 duration;
} WiFiPlazaObjTween;

struct WiFiPlazaObj {
    Field2DObject *gridObj;
    void *avatar;
    u16 step;
    u8 finished;
    u8 handler;

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
    } work;

    WiFiPlazaObjFunc update;
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
    Field2DObjectManager *gridObjs;
    void *avatars;
    WiFiPlazaObj *objs;
    u16 count;
    u16 mode; // nonzero: own player avatar 0x61
    Field2DCommandQueue *commands;
    WiFiPlazaMap *map;
    WiFiPlazaObjLight light;
    WiFiPlazaObj *ownPlayer;
    WiFiPlazaObj *special; // object spawned on map tag 97
};

WiFiPlazaObjManager *WiFiPlazaObj_CreateManager(u32 count, u32 mode, WiFiPlazaMap *map, enum HeapID heapID, u32 loadHeapID);
void WiFiPlazaObj_DeleteManager(WiFiPlazaObjManager *manager);
void WiFiPlazaObj_UpdateObjsAndLight(WiFiPlazaObjManager *manager);
void WiFiPlazaObj_UpdateManager(WiFiPlazaObjManager *manager);
u32 WiFiPlazaObj_DrawAvatars(WiFiPlazaObjManager *manager);
u32 WiFiPlazaObj_ClearPendingFree(WiFiPlazaObjManager *manager);
void WiFiPlazaObj_SetLightSet(WiFiPlazaObjManager *manager, int set);
void WiFiPlazaObj_FadeToLightSet(WiFiPlazaObjManager *manager, int set);
void WiFiPlazaObj_PushCommand(WiFiPlazaObjManager *manager, Field2DCommand *command);
WiFiPlazaObj *WiFiPlazaObj_CreateOwnPlayer(WiFiPlazaObjManager *manager, int id);
WiFiPlazaObj *WiFiPlazaObj_CreateOwnPlayerAt(WiFiPlazaObjManager *manager, int id, int x, int y);
WiFiPlazaObj *WiFiPlazaObj_CreatePlayer(WiFiPlazaObjManager *manager, int id, int avatar);
void WiFiPlazaObj_Delete(WiFiPlazaObj *obj);
WiFiPlazaObj *WiFiPlazaObj_FindById(WiFiPlazaObjManager *manager, int id);
WiFiPlazaObj *WiFiPlazaObj_GetOwnPlayer(WiFiPlazaObjManager *manager);
WiFiPlazaObj *WiFiPlazaObj_GetSpecial(WiFiPlazaObjManager *manager);
void WiFiPlazaObj_SetGridPos(WiFiPlazaObj *obj, Field2DPoint pos);
void WiFiPlazaObj_SetGridPosAndDir(WiFiPlazaObj *obj, Field2DPoint pos, s32 direction);
Field2DPoint WiFiPlazaObj_GetGridPos(WiFiPlazaObj *obj);
s32 WiFiPlazaObj_GetParam(WiFiPlazaObj *obj, s32 param);
void WiFiPlazaObj_QueueCommand(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, s32 command, s32 direction);
void WiFiPlazaObj_RunCommand(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, u16 value, u32 kind);
void WiFiPlazaObj_SetHandler(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, int handler);
u8 WiFiPlazaObj_IsFinished(WiFiPlazaObj *obj);
u8 WiFiPlazaObj_GetHandler(WiFiPlazaObj *obj);
WiFiPlazaObj *WiFiPlazaObj_FindFacing(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj);
u32 WiFiPlazaObj_IsCulled(WiFiPlazaObj *obj);
WiFiPlazaObj *WiFiPlazaObj_FindAt(WiFiPlazaObjManager *manager, s32 x, s32 y);
BOOL WiFiPlazaObj_IsOnCell(WiFiPlazaObj *obj, s32 x, s32 y);
BOOL WiFiPlazaObj_FindFreeNeighbor(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, s32 *direction, Field2DPoint *position);
void WiFiPlazaObj_SetAvatarMode(WiFiPlazaObj *obj, int enable);
void WiFiPlazaObj_SetPositionFromGrid(WiFiPlazaObj *obj, void *out);
void WiFiPlazaObj_SetPosition(WiFiPlazaObj *obj, VecFx32 *position);
void WiFiPlazaObj_GetPosition(WiFiPlazaObj *obj, VecFx32 *position);
void WiFiPlazaObj_SetDirection(WiFiPlazaObj *obj, u32 direction);
void WiFiPlazaObj_SetPoseEffect(WiFiPlazaObj *obj, u32 enable);
void WiFiPlazaObj_SetSpinEffect(WiFiPlazaObj *obj, u32 enable);
void WiFiPlazaObj_SetPoseLoopEffect(WiFiPlazaObj *obj, u32 enable);
void WiFiPlazaObj_SetLightFlags(WiFiPlazaObj *obj, u32 value);
void WiFiPlazaObj_SetVisible(WiFiPlazaObj *obj, u32 value);
u32 WiFiPlazaObj_IsVisible(WiFiPlazaObj *obj);
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

// Avatar manager
void *WiFiPlazaPlayerObjects_New(u32 count, u32 mode, enum HeapID heapID, u32 loadHeapID);
void WiFiPlazaPlayerObjects_Delete(void *avatars);
void WiFiPlazaPlayerObjects_Update(void *avatars);
u32 WiFiPlazaPlayerObjects_Draw(void *avatars);
u32 WiFiPlazaPlayerObjects_ClearPendingFree(void *avatars);
void WiFiPlazaPlayerObjects_SetModelAlpha(void *avatars, int value);
void *WiFiPlazaPlayerObjects_Add(void *avatars, Field2DObject *gridObj);
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
