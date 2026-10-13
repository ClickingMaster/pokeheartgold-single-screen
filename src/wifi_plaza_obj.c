#include "wifi_plaza_obj.h"

#include <string.h>

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "math_util.h"
#include "overlay_42.h"
#include "sound_playback.h"
#include "system.h"
#include "wifi_plaza_map.h"

typedef struct WiFiPlazaObjSpawn {
    u8 tag;
    u8 unk1;
    u16 unk2;
    u8 unk4;
    u8 pad5[3];
} WiFiPlazaObjSpawn;

// rodata of this overlay (asm/wifi_plaza_map_rodata.s)
extern const u8 ov49_02269654[4];
extern const u8 ov49_02269658[];
extern const u8 ov49_0226965C[];
extern const u32 ov49_02269660[];
extern const WiFiPlazaObjSpawn sWiFiPlazaObjMapSpawns[];
extern const WiFiPlazaObjFunc sWiFiPlazaObjExitHandlers[];
extern const WiFiPlazaObjFunc sWiFiPlazaObjHandlers[10];

WiFiPlazaObjManager *WiFiPlazaObj_CreateManager(u32 count, u32 mode, WiFiPlazaMap *map, enum HeapID heapID, u32 loadHeapID) {
    WiFiPlazaObjManager *manager = Heap_Alloc(heapID, sizeof(WiFiPlazaObjManager));
    u32 bytes;

    memset(manager, 0, sizeof(WiFiPlazaObjManager));
    manager->gridObjs = Field2DObjectManager_New(count, heapID);
    manager->avatars = WiFiPlazaPlayerObjects_New(count, mode, heapID, loadHeapID);
    manager->commands = Field2DCommandQueue_New(0x20, heapID);
    bytes = sizeof(WiFiPlazaObj) * count;
    manager->count = count;
    manager->objs = Heap_Alloc(heapID, bytes);
    memset(manager->objs, 0, bytes);
    manager->map = map;
    manager->unk_0E = mode;
    WiFiPlazaObj_SpawnMapObjects(manager);
    return manager;
}

void WiFiPlazaObj_DeleteManager(WiFiPlazaObjManager *manager) {
    Heap_Free(manager->objs);
    Field2DCommandQueue_Free(manager->commands);
    WiFiPlazaPlayerObjects_Delete(manager->avatars);
    Field2DObjectManager_Free(manager->gridObjs);
    Heap_Free(manager);
}

void WiFiPlazaObj_UpdateObjsAndLight(WiFiPlazaObjManager *manager) {
    Field2DObjectManager_Update(manager->gridObjs);
    WiFiPlazaObj_UpdateLight(&manager->light, manager->avatars);
}

// Runs every object's handler, then executes the queued move commands.
void WiFiPlazaObj_UpdateManager(WiFiPlazaObjManager *manager) {
    int i;
    int offset;
    Field2DMap *grid;
    Field2DCommand command;
    Field2DMove move;

    i = 0;
    if (i < manager->count) {
        offset = 0;
        do {
            if (WiFiPlazaObj_IsFree((WiFiPlazaObj *)((u8 *)manager->objs + offset)) == FALSE) {
                WiFiPlazaObj *obj = (WiFiPlazaObj *)((u8 *)manager->objs + offset);

                obj->update(obj, manager);
            }
            i++;
            offset += sizeof(WiFiPlazaObj);
        } while (i < manager->count);
    }
    grid = WiFiPlazaMap_GetGrid(manager->map);
    if (Field2DCommandQueue_Pop(manager->commands, &command) == 1) {
        do {
            if (Field2DObjectManager_TryCommand(grid, manager->gridObjs, &command, &move) == 1) {
                Field2DObjectManager_StartMove(manager->gridObjs, &move);
            }
        } while (Field2DCommandQueue_Pop(manager->commands, &command) == 1);
    }
    WiFiPlazaPlayerObjects_Update(manager->avatars);
}

u32 WiFiPlazaObj_DrawAvatars(WiFiPlazaObjManager *manager) {
    return WiFiPlazaPlayerObjects_Draw(manager->avatars);
}

u32 ov49_02258BE0(WiFiPlazaObjManager *manager) {
    return ov45_022306F4(manager->avatars);
}

void WiFiPlazaObj_SetLightSet(WiFiPlazaObjManager *manager, int set) {
    WiFiPlazaObj_InitLight(&manager->light, manager->avatars, ov49_02269660[set]);
}

void WiFiPlazaObj_FadeToLightSet(WiFiPlazaObjManager *manager, int set) {
    WiFiPlazaObj_StartLightFade(&manager->light, ov49_02269660[set]);
}

void WiFiPlazaObj_PushCommand(WiFiPlazaObjManager *manager, Field2DCommand *command) {
    Field2DCommandQueue_Push(manager->commands, command);
}

// Creates the own player on the start cell (map tag 3).
WiFiPlazaObj *WiFiPlazaObj_CreateOwnPlayer(WiFiPlazaObjManager *manager, int id) {
    u16 cell[2];

    if (!WiFiPlazaMap_FindTileTag(manager->map, 3, &cell[1], &cell[0], 0)) {
        GF_AssertFail();
    }
    return WiFiPlazaObj_CreateOwnPlayerAt(manager, id, cell[1], cell[0]);
}

WiFiPlazaObj *WiFiPlazaObj_CreateOwnPlayerAt(WiFiPlazaObjManager *manager, int id, int x, int y) {
    Field2DObjectTemplate template;
    WiFiPlazaObj *obj = WiFiPlazaObj_AllocObj(manager);

    template.unk0 = x << 4;
    template.unk2 = y << 4;
    template.unk4 = id;
    template.unk6 = 0;
    template.unk8 = 0;
    if (manager->unk_0E == 0) {
        template.unkA = 0;
    } else {
        template.unkA = 0x61;
    }
    obj->gridObj = Field2DObjectManager_AddObject(manager->gridObjs, &template);
    obj->avatar = WiFiPlazaPlayerObjects_Add(manager->avatars, obj->gridObj);
    manager->ownPlayer = obj;
    WiFiPlazaObj_SetHandler(manager, obj, 0);
    return obj;
}

// Creates another player on the first free cell with map tag id + 4.
WiFiPlazaObj *WiFiPlazaObj_CreatePlayer(WiFiPlazaObjManager *manager, int id, int avatar) {
    Field2DObjectTemplate template;
    u16 cell[2];
    WiFiPlazaObj *obj;
    WiFiPlazaObj *ownPlayer;
    u16 i;
    BOOL found;

    obj = WiFiPlazaObj_AllocObj(manager);
    found = FALSE;
    i = 0;
    do {
        if (!WiFiPlazaMap_FindTileTag(manager->map, id + 4, &cell[1], &cell[0], i)) {
            return NULL;
        }
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(manager);
        if (ownPlayer == NULL) {
            found = TRUE;
        } else if (!WiFiPlazaObj_IsOnCell(ownPlayer, cell[1], cell[0])) {
            found = TRUE;
        }
        i++;
    } while (!found);
    template.unk0 = cell[1] << 4;
    template.unk2 = cell[0] << 4;
    template.unk4 = id;
    template.unk6 = 0;
    template.unk8 = 1;
    template.unkA = avatar;
    obj->gridObj = Field2DObjectManager_AddObject(manager->gridObjs, &template);
    obj->avatar = WiFiPlazaPlayerObjects_Add(manager->avatars, obj->gridObj);
    WiFiPlazaObj_SetHandler(manager, obj, 0);
    return obj;
}

void WiFiPlazaObj_Delete(WiFiPlazaObj *obj) {
    WiFiPlazaPlayerObject_Delete(obj->avatar);
    Field2DObject_Delete(obj->gridObj);
    memset(obj, 0, sizeof(WiFiPlazaObj));
}

WiFiPlazaObj *WiFiPlazaObj_FindById(WiFiPlazaObjManager *manager, int id) {
    int i;
    Field2DObject *gridObj;
    int objId;

    for (i = 0; i < manager->count; i++) {
        gridObj = *(Field2DObject **)((u8 *)manager->objs + i * (int)sizeof(WiFiPlazaObj));
        if (gridObj != NULL) {
            objId = Field2DObject_GetParam(gridObj, 4);
            if (objId == id) {
                return (WiFiPlazaObj *)((u8 *)manager->objs + (int)sizeof(WiFiPlazaObj) * i);
            }
        }
    }
    return NULL;
}

WiFiPlazaObj *WiFiPlazaObj_GetOwnPlayer(WiFiPlazaObjManager *manager) {
    return manager->ownPlayer;
}

WiFiPlazaObj *WiFiPlazaObj_GetSpecial(WiFiPlazaObjManager *manager) {
    return manager->special;
}

void WiFiPlazaObj_SetGridPos(WiFiPlazaObj *obj, Field2DPoint pos) {
    if (obj->gridObj == NULL) {
        GF_AssertFail();
    }
    Field2DObject_SetPos(obj->gridObj, pos);
    Field2DObject_SetPrevPos(obj->gridObj, pos);
    Field2DObject_SetParam(obj->gridObj, 5, 0);
}

void WiFiPlazaObj_SetGridPosAndDir(WiFiPlazaObj *obj, Field2DPoint pos, s32 direction) {
    WiFiPlazaObj_SetGridPos(obj, pos);
    Field2DObject_SetParam(obj->gridObj, 6, direction);
}

#pragma force_active on

Field2DPoint WiFiPlazaObj_GetGridPos(WiFiPlazaObj *obj) {
    GF_ASSERT(obj->gridObj != NULL);
    return Field2DObject_GetDrawPos(obj->gridObj);
}

#pragma force_active reset

s32 WiFiPlazaObj_GetParam(WiFiPlazaObj *obj, s32 param) {
    if (obj->gridObj == NULL) {
        GF_AssertFail();
    }
    return Field2DObject_GetParam(obj->gridObj, param);
}

void WiFiPlazaObj_QueueCommand(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, s32 command, s32 direction) {
    if (Field2DObject_GetParam(obj->gridObj, 5) != 0) {
        GF_AssertFail();
    }
    WiFiPlazaObj_QueueMove(manager, command, direction, Field2DObject_GetParam(obj->gridObj, 4));
}

void WiFiPlazaObj_RunCommand(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, u16 value, u8 kind) {
    Field2DMove data;

    data.unk0 = Field2DObject_GetPos(obj->gridObj);
    data.unk4 = value;
    data.unk6 = kind;
    data.unk7 = Field2DObject_GetParam(obj->gridObj, 4);
    Field2DObjectManager_StartMove(manager->gridObjs, &data);
}

// Runs the old handler's exit function and installs handler `handler` with a cleared work area.
void WiFiPlazaObj_SetHandler(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, int handler) {
    if (handler >= 10) {
        GF_AssertFail();
    }
    if (sWiFiPlazaObjExitHandlers[obj->handler] != NULL) {
        sWiFiPlazaObjExitHandlers[obj->handler](obj, manager);
    }
    obj->step = 0;
    obj->finished = 0;
    obj->handler = handler;
    memset(obj->work.raw, 0, sizeof(obj->work.raw));
    obj->update = sWiFiPlazaObjHandlers[handler];
}

u8 WiFiPlazaObj_IsFinished(WiFiPlazaObj *obj) {
    return obj->finished;
}

u8 WiFiPlazaObj_GetHandler(WiFiPlazaObj *obj) {
    return obj->handler;
}

#pragma force_active on

// Returns the object on the cell obj is facing.
WiFiPlazaObj *WiFiPlazaObj_FindFacing(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj) {
    s32 direction = Field2DObject_GetParam(obj->gridObj, 6);
    Field2DObject *facing = Field2DObject_FindObjectInDirection(obj->gridObj, manager->gridObjs, direction);

    if (facing == NULL) {
        return NULL;
    } else {
        return WiFiPlazaObj_FindById(manager, Field2DObject_GetParam(facing, 4));
    }
}

#pragma force_active reset

u32 WiFiPlazaObj_IsCulled(WiFiPlazaObj *obj) {
    return WiFiPlazaPlayerObject_IsCulled(obj->avatar);
}

WiFiPlazaObj *WiFiPlazaObj_FindAt(WiFiPlazaObjManager *manager, s32 x, s32 y) {
    volatile WiFiPlazaObjManager *volatileManager = manager; // entries and count are reloaded in the loop
    Field2DPoint pos;
    Field2DObject *gridObj;
    u8 *entry;
    int i;

    pos.unk0 = x << 4;
    pos.unk2 = y << 4;
    gridObj = Field2DObjectManager_FindObjectAt(manager->gridObjs, pos);
    if (gridObj == NULL) {
        return NULL;
    }
    i = 0;
    if (i < volatileManager->count) {
        entry = (u8 *)volatileManager->objs;
        do {
            if (*(Field2DObject **)entry == gridObj) {
                return (WiFiPlazaObj *)((u8 *)volatileManager->objs + i * sizeof(WiFiPlazaObj));
            }
            i++;
            entry += sizeof(WiFiPlazaObj);
        } while (i < volatileManager->count);
    }
    GF_ASSERT(FALSE);
    return NULL;
}

// TRUE if obj stands on or is moving to cell (x, y).
BOOL WiFiPlazaObj_IsOnCell(WiFiPlazaObj *obj, s32 x, s32 y) {
    Field2DPoint current = Field2DObject_GetPos(obj->gridObj);
    Field2DPoint previous = Field2DObject_GetPrevPos(obj->gridObj);

    if (current.unk0 == (x << 4) && current.unk2 == (y << 4)) {
        return TRUE;
    }
    if (previous.unk0 == (x << 4) && previous.unk2 == (y << 4)) {
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaObj_FindFreeNeighbor(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, s32 *direction, Field2DPoint *position) {
    Field2DPoint current = Field2DObject_GetPos(obj->gridObj);
    int i;

    for (i = 0; i < 4; i++) {
        Field2DPoint next = Field2D_StepInDirection(current, ov49_02269654[i]);
        int y = next.unk2 / 16;
        int x = next.unk0 / 16;

        if (WiFiPlazaMap_GetCollision(manager->map, x, y) != 1) {
            int attr = WiFiPlazaMap_GetTileAttr(manager->map, x, y);

            if ((attr == 0 || attr == 0x2A) && Field2DObjectManager_FindObjectAt(manager->gridObjs, next) == NULL) {
                *direction = ov49_02269654[i];
                *position = next;
                return TRUE;
            }
        }
    }
    return FALSE;
}

void WiFiPlazaObj_SetAvatarMode(WiFiPlazaObj *obj, int enable) {
    WiFiPlazaPlayerObject_SetMode(obj->avatar, enable);
}

void WiFiPlazaObj_SetPositionFromGrid(WiFiPlazaObj *obj, void *out) {
    WiFiPlazaPlayerObject_SetGridPosition(obj->avatar, out);
}

void WiFiPlazaObj_SetPosition(WiFiPlazaObj *obj, VecFx32 *position) {
    WiFiPlazaPlayerObject_SetPosition(obj->avatar, position);
}

void WiFiPlazaObj_GetPosition(WiFiPlazaObj *obj, VecFx32 *position) {
    WiFiPlazaPlayerObject_GetPosition(obj->avatar, position);
}

void WiFiPlazaObj_SetDirection(WiFiPlazaObj *obj, u32 direction) {
    WiFiPlazaPlayerObject_SetStandingSequence(obj->avatar, direction);
}

// The stop call gets `enable` (0) as its second argument register.
void WiFiPlazaObj_SetPoseEffect(WiFiPlazaObj *obj, u32 enable) {
    if (enable != 0) {
        WiFiPlazaPlayerObject_StartEffect(obj->avatar, 1);
    } else {
        WiFiPlazaPlayerObject_StopEffect(obj->avatar);
    }
}

void WiFiPlazaObj_SetSpinEffect(WiFiPlazaObj *obj, u32 enable) {
    if (enable != 0) {
        WiFiPlazaPlayerObject_StartEffect(obj->avatar, 0);
    } else {
        WiFiPlazaPlayerObject_StopEffect(obj->avatar);
    }
}

void WiFiPlazaObj_SetPoseLoopEffect(WiFiPlazaObj *obj, u32 enable) {
    if (enable != 0) {
        WiFiPlazaPlayerObject_StartEffect(obj->avatar, 2);
    } else {
        WiFiPlazaPlayerObject_StopEffect(obj->avatar);
    }
}

void WiFiPlazaObj_SetLightFlags(WiFiPlazaObj *obj, u32 value) {
    WiFiPlazaPlayerObject_SetLightFlags(obj->avatar, value);
}

void WiFiPlazaObj_SetVisible(WiFiPlazaObj *obj, u32 value) {
    WiFiPlazaPlayerObject_SetVisible(obj->avatar, value);
}

u32 WiFiPlazaObj_IsVisible(WiFiPlazaObj *obj) {
    return WiFiPlazaPlayerObject_IsVisible(obj->avatar);
}

#pragma force_active on

// Creates an object on every cell carrying one of the four spawn table tags.
void WiFiPlazaObj_SpawnMapObjects(WiFiPlazaObjManager *manager) {
    const WiFiPlazaObjSpawn *spawn = sWiFiPlazaObjMapSpawns;
    u32 i;
    u16 n;

    struct {
        struct {
            u16 y;
            u16 x;
        } cell;

        Field2DObjectTemplate template;
    } frame;

    WiFiPlazaObj *obj;
    Field2DObject *gridObj;
    void *avatar;

    for (i = 0; i < 4; i++, spawn++) {
        n = 0;
        while (WiFiPlazaMap_FindTileTag(manager->map, spawn->tag, &frame.cell.x, &frame.cell.y, n) == TRUE) {
            obj = WiFiPlazaObj_AllocObj(manager);
            frame.template.unk0 = frame.cell.x << 4;
            frame.template.unk2 = frame.cell.y << 4;
            frame.template.unk4 = spawn->unk4;
            frame.template.unk6 = 0;
            frame.template.unk8 = spawn->unk1;
            frame.template.unkA = spawn->unk2;
            gridObj = Field2DObjectManager_AddObject(manager->gridObjs, &frame.template);
            obj->gridObj = gridObj;
            avatar = WiFiPlazaPlayerObjects_Add(manager->avatars, gridObj);
            obj->avatar = avatar;
            WiFiPlazaObj_SetHandler(manager, obj, 0);
            if (spawn->tag == 97) {
                manager->special = obj;
            }
            n++;
        }
    }
}

#pragma force_active reset

void WiFiPlazaObj_QueueMove(WiFiPlazaObjManager *manager, int command, int direction, int id) {
    Field2DCommand data;

    data.unk0 = command;
    data.unk2 = direction;
    data.unk3 = id;
    WiFiPlazaObj_PushCommand(manager, &data);
}

u8 WiFiPlazaObj_RandomPick(const u8 *values, u32 count) {
    return values[MTRandom() % count];
}

BOOL WiFiPlazaObj_IsTagInDirection(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, s32 direction, int tag) {
    Field2DPoint position = Field2DObject_GetPos(obj->gridObj);
    u32 result;

    position = Field2D_StepInDirection(position, direction);
    result = WiFiPlazaMap_GetTileTag(manager->map, position.unk0 / 16, position.unk2 / 16);
    if (result == tag) {
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaObj_InitTween(WiFiPlazaObjTween *tween, s32 start, s32 end, u32 duration) {
    tween->value = start;
    tween->start = start;
    tween->delta = end - start;
    tween->duration = duration;
}

BOOL WiFiPlazaObj_UpdateTween(WiFiPlazaObjTween *tween, u32 frame) {
    float frameFx;
    float durationFx;
    s32 scaled;
    s32 delta = tween->delta;

    // fx32 conversions rounded away from zero
    if (frame != 0) {
        frameFx = (float)(frame << 12) + 0.5f;
    } else {
        frameFx = (float)(frame << 12) - 0.5f;
    }
    scaled = (s32)(((s64)delta * (s64)(s32)frameFx + 0x800) >> 12);
    if (tween->duration != 0) {
        durationFx = (float)(tween->duration << 12) + 0.5f;
    } else {
        durationFx = (float)(tween->duration << 12) - 0.5f;
    }
    scaled = FX_Div(scaled, (s32)durationFx);
    scaled += tween->start;
    tween->value = scaled;
    return frame >= tween->duration;
}

s32 WiFiPlazaObj_GetTweenValue(WiFiPlazaObjTween *tween) {
    return tween->value;
}

WiFiPlazaObj *WiFiPlazaObj_AllocObj(WiFiPlazaObjManager *manager) {
    int i;

    for (i = 0; i < manager->count; i++) {
        if (WiFiPlazaObj_IsFree((WiFiPlazaObj *)((u8 *)manager->objs + i * (int)sizeof(WiFiPlazaObj)))) {
            return (WiFiPlazaObj *)((u8 *)manager->objs + i * (int)sizeof(WiFiPlazaObj));
        }
    }
    GF_AssertFail();
    return (WiFiPlazaObj *)((u8 *)manager->objs + i * (int)sizeof(WiFiPlazaObj));
}

BOOL WiFiPlazaObj_IsFree(WiFiPlazaObj *obj) {
    return obj->gridObj == NULL;
}

void WiFiPlazaObj_HandlerIdle(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
}

// D-pad: walk (run while B is held) when already facing that way, otherwise turn.
void WiFiPlazaObj_HandlerPlayerInput(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    int facing = Field2DObject_GetParam(obj->gridObj, 6);
    int id = Field2DObject_GetParam(obj->gridObj, 4);
    int moving = Field2DObject_GetParam(obj->gridObj, 5);
    int command;
    u32 input;

    if (moving != 0) {
        return;
    }
    input = gSystem.heldKeys;
    command = 2;
    if (input & PAD_BUTTON_B) {
        command = 3;
    }
    if (input & PAD_KEY_UP) {
        if (facing == 0) {
            WiFiPlazaObj_QueueMove(manager, command, facing, id);
        } else {
            WiFiPlazaObj_QueueMove(manager, 1, 0, id);
        }
        return;
    }
    if (input & PAD_KEY_DOWN) {
        if (facing == 1) {
            WiFiPlazaObj_QueueMove(manager, command, facing, id);
        } else {
            WiFiPlazaObj_QueueMove(manager, 1, 1, id);
        }
        return;
    }
    if (input & PAD_KEY_LEFT) {
        if (facing == 2) {
            WiFiPlazaObj_QueueMove(manager, command, facing, id);
        } else {
            WiFiPlazaObj_QueueMove(manager, 1, 2, id);
        }
        return;
    }
    if (input & PAD_KEY_RIGHT) {
        if (facing == 3) {
            WiFiPlazaObj_QueueMove(manager, command, facing, id);
        } else {
            WiFiPlazaObj_QueueMove(manager, 1, 3, id);
        }
    }
}

// Waits a random time, then turns or walks in a random direction (walks only onto its own tag, id + 4).
void WiFiPlazaObj_HandlerWander(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    int id;
    int facing;

    switch (obj->step) {
    case 0:
        obj->work.wander.timer = WiFiPlazaObj_RandomPick(ov49_02269658, 4);
        obj->step++;
        break;
    case 1:
        obj->work.wander.timer--;
        if (obj->work.wander.timer == 0) {
            obj->step++;
        }
        break;
    case 2:
        id = Field2DObject_GetParam(obj->gridObj, 4);
        facing = Field2DObject_GetParam(obj->gridObj, 6);
        obj->work.wander.direction = WiFiPlazaObj_RandomPick(ov49_0226965C, 4);
        if (WiFiPlazaObj_IsTagInDirection(manager, obj, obj->work.wander.direction, id + 4) == TRUE) {
            if (facing == obj->work.wander.direction) {
                WiFiPlazaObj_QueueCommand(manager, obj, 2, obj->work.wander.direction);
                obj->step = 4;
            } else {
                WiFiPlazaObj_QueueCommand(manager, obj, 1, obj->work.wander.direction);
                obj->step = 3;
            }
        } else {
            WiFiPlazaObj_QueueCommand(manager, obj, 1, obj->work.wander.direction);
            obj->step = 4;
        }
        break;
    case 3:
        if (Field2DObject_GetParam(obj->gridObj, 5) == 0) {
            WiFiPlazaObj_QueueCommand(manager, obj, 2, obj->work.wander.direction);
            obj->step++;
        }
        break;
    case 4:
        if (Field2DObject_GetParam(obj->gridObj, 5) == 0) {
            obj->step = 0;
        }
        break;
    }
}

// Waits 32 frames, then raises the avatar by 500 over 24 frames.
void WiFiPlazaObj_HandlerRiseOut(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    int done;
    VecFx32 position;

    switch (obj->step) {
    case 0:
        WiFiPlazaPlayerObject_SetMode(obj->avatar, 0);
        WiFiPlazaPlayerObject_StartEffect(obj->avatar, 0);
        WiFiPlazaObj_InitTween(&obj->work.lift.tween, 0, 0x1F4000, 0x18);
        obj->work.lift.frame = 0x20;
        obj->step++;
        // fallthrough
    case 1:
        obj->work.lift.frame--;
        if (obj->work.lift.frame == 0) {
            obj->step++;
            obj->work.lift.frame = 0;
            PlaySE(0x64E);
        }
        break;
    case 2:
        done = WiFiPlazaObj_UpdateTween(&obj->work.lift.tween, obj->work.lift.frame);
        obj->work.lift.frame++;
        WiFiPlazaPlayerObject_GetPosition(obj->avatar, &position);
        position.y = WiFiPlazaObj_GetTweenValue(&obj->work.lift.tween);
        WiFiPlazaPlayerObject_SetPosition(obj->avatar, &position);
        if (done == TRUE) {
            obj->step++;
            WiFiPlazaPlayerObject_StopEffect(obj->avatar);
            obj->finished = TRUE;
        }
        break;
    case 3:
        break;
    }
}

// Lowers the avatar from 500 to the ground over 24 frames, then waits 32 frames.
void WiFiPlazaObj_HandlerDropIn(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    int done;
    VecFx32 position;

    switch (obj->step) {
    case 0:
        WiFiPlazaPlayerObject_SetMode(obj->avatar, 0);
        WiFiPlazaPlayerObject_StartEffect(obj->avatar, 0);
        WiFiPlazaObj_InitTween(&obj->work.lift.tween, 0x1F4000, 0, 0x18);
        obj->work.lift.frame = 0;
        obj->step++;
        PlaySE(0x64E);
        // fallthrough
    case 1:
        done = WiFiPlazaObj_UpdateTween(&obj->work.lift.tween, obj->work.lift.frame);
        obj->work.lift.frame++;
        WiFiPlazaPlayerObject_GetPosition(obj->avatar, &position);
        position.y = WiFiPlazaObj_GetTweenValue(&obj->work.lift.tween);
        WiFiPlazaPlayerObject_SetPosition(obj->avatar, &position);
        if (done == TRUE) {
            obj->work.lift.frame = 0x20;
            obj->step++;
            return;
        }
        break;
    case 2:
        obj->work.lift.frame--;
        if (obj->work.lift.frame == 0) {
            obj->step++;
            WiFiPlazaPlayerObject_StopEffect(obj->avatar);
            WiFiPlazaPlayerObject_SetMode(obj->avatar, 1);
            obj->finished = TRUE;
        }
        break;
    case 3:
        break;
    }
}

void WiFiPlazaObj_HandlerSlideLeft(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    WiFiPlazaObj_SlideOneCell(obj, manager, 2);
}

void WiFiPlazaObj_HandlerSlideRight(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    WiFiPlazaObj_SlideOneCell(obj, manager, 3);
}

void WiFiPlazaObj_HandlerSlideUp(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    WiFiPlazaObj_SlideOneCell(obj, manager, 0);
}

void WiFiPlazaObj_HandlerSlideDown(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    WiFiPlazaObj_SlideOneCell(obj, manager, 1);
}

// Moves the avatar one cell (16 units) in `direction` over 4 frames, waits, then moves the grid object there.
void WiFiPlazaObj_SlideOneCell(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager, u32 direction) {
    VecFx32 startPos;
    VecFx32 position;
    Field2DPoint cellCopy;
    Field2DPoint nextCopy;
    Field2DPoint cell;
    Field2DPoint next;
    u32 done;
    u8 dir;

    switch (obj->step) {
    case 0:
        WiFiPlazaPlayerObject_SetMode(obj->avatar, 0);
        WiFiPlazaPlayerObject_StartEffect(obj->avatar, 0);
        WiFiPlazaPlayerObject_SetEffectStep(obj->avatar, 2);
        WiFiPlazaObj_InitTween(&obj->work.slide.tween, 0, 0x10000, 4);
        WiFiPlazaPlayerObject_GetPosition(obj->avatar, &startPos);
        switch (direction) {
        case 0:
        case 1:
            obj->work.slide.start = startPos.z;
            break;
        case 2:
        case 3:
            obj->work.slide.start = startPos.x;
            break;
        }
        obj->work.slide.frame = 0;
        obj->step++;
    case 1:
        done = WiFiPlazaObj_UpdateTween(&obj->work.slide.tween, obj->work.slide.frame);
        obj->work.slide.frame = obj->work.slide.frame + 1;
        WiFiPlazaPlayerObject_GetPosition(obj->avatar, &position);
        switch (direction) {
        case 0:
            position.z = obj->work.slide.start - WiFiPlazaObj_GetTweenValue(&obj->work.slide.tween);
            break;
        case 1:
            position.z = obj->work.slide.start + WiFiPlazaObj_GetTweenValue(&obj->work.slide.tween);
            break;
        case 2:
            position.x = obj->work.slide.start - WiFiPlazaObj_GetTweenValue(&obj->work.slide.tween);
            break;
        case 3:
            position.x = obj->work.slide.start + WiFiPlazaObj_GetTweenValue(&obj->work.slide.tween);
            break;
        }
        WiFiPlazaPlayerObject_SetPosition(obj->avatar, &position);
        if (done == 1) {
            dir = Field2D_GetOppositeDirection(direction);
            obj->work.slide.frame = 4;
            switch (dir) {
            case 3:
                obj->work.slide.frame += 2;
            case 0:
                obj->work.slide.frame += 2;
            case 2:
                obj->work.slide.frame += 2;
            case 1:
                break;
            }
            obj->step++;
        }
        break;
    case 2:
        obj->work.slide.frame--;
        if (obj->work.slide.frame == 0) {
            obj->step++;
            cell = Field2DObject_GetPos(obj->gridObj);
            cellCopy = cell;
            next = Field2D_StepInDirection(cellCopy, direction);
            nextCopy = next;
            WiFiPlazaObj_SetGridPosAndDir(obj, nextCopy, Field2D_GetOppositeDirection(direction));
            WiFiPlazaPlayerObject_StopEffect(obj->avatar);
            WiFiPlazaPlayerObject_SetMode(obj->avatar, 1);
            obj->finished = 1;
        }
        break;
    case 3:
        break;
    }
}

// Repeating hop: every 28 frames, 6-frame bounces (up 10, back 2) during the last 12 frames.
void WiFiPlazaObj_HandlerHop(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    VecFx32 position;
    s16 phase;
    s32 degrees;

    switch (obj->step) {
    case 0:
        WiFiPlazaPlayerObject_SetMode(obj->avatar, 0);
        WiFiPlazaPlayerObject_GetPosition(obj->avatar, &obj->work.hop.origin);
        obj->work.hop.frame = 0x1C;
        obj->step++;
        break;
    case 1:
        obj->work.hop.frame--;
        if (obj->work.hop.frame < 0) {
            obj->work.hop.frame = 0x1C;
        }
        position = obj->work.hop.origin;
        phase = obj->work.hop.frame - 0x10;
        if (phase > 0) {
            phase = phase % 6;
            degrees = phase * 0xB4 / 6;
            position.z += FX_Mul(GF_SinDegNoWrap(degrees), FX32_CONST(-2.0));
            position.y += FX_Mul(GF_SinDegNoWrap(degrees), FX32_CONST(10.0));
        }
        WiFiPlazaPlayerObject_SetPosition(obj->avatar, &position);
        WiFiPlazaPlayerObject_SetStandingSequence(obj->avatar, 1);
        break;
    }
}

void WiFiPlazaObj_HandlerResetHeight(WiFiPlazaObj *obj, WiFiPlazaObjManager *manager) {
    VecFx32 position;

    WiFiPlazaPlayerObject_SetMode(obj->avatar, 1);
    WiFiPlazaPlayerObject_GetPosition(obj->avatar, &position);
    position.y = 0;
    WiFiPlazaPlayerObject_SetPosition(obj->avatar, &position);
}

void WiFiPlazaObj_InitLight(WiFiPlazaObjLight *light, void *avatars, u32 value) {
    light->value = value;
    light->start = value;
    light->delta = 0;
    light->frame = 0;
    light->duration = 0;
    light->active = 0;
    WiFiPlazaPlayerObjects_SetModelAlpha(avatars, value);
}

void WiFiPlazaObj_StartLightFade(WiFiPlazaObjLight *light, u32 value) {
    light->active = 1;
    light->start = light->value;
    light->delta = value - light->value;
    light->frame = 0;
    light->duration = 60;
}
