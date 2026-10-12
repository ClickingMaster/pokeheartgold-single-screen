#include "global.h"

#include "comm_player_manager_internal.h"
#include "communication_player_state.h"
#include "field_system.h"
#include "heap.h"
#include "map_events.h"
#include "map_events_internal.h"
#include "map_object.h"
#include "map_object_movement_utilities.h"
#include "player_avatar.h"
#include "player_data.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02054648.h"

// Communication core, network state and player movement functions, declared with the types their call sites were
// matched against (no header, or a header with other types).
int Comm_GetLocalNetId(void);
int Comm_IsNetIdConnected(int netId); // u16 in include/unk_02035900.h
BOOL sub_02037474(void);
void sub_020374E4(void);
int sub_0203774C(int value);
int sub_02037108(int command, const void *data, int size);
int sub_02037184(int command, void *data);
void sub_02037498(int mode);
int sub_020374AC(int netId);
u16 sub_020374C0(int netId);
void sub_020376E0(int command, void *data);
void sub_02037894(int netId, int index);
u8 sub_0203993C(void);
u32 sub_0205DF0C(PlayerAvatar *avatar, int keys, int heldKeys, u32 speed, int flag, int collision);
BOOL sub_0205DFC8(PlayerAvatar *avatar);
BOOL sub_0205DE38(PlayerAvatar *avatar);
void PlayerAvatar_SetMovement(PlayerAvatar *avatar, int movement);
int GetMovementForDirection(int direction, int movement);

void CommPlayerManager_RemoveAllAvatars(void) {
    int i;
    if (gCommPlayerManager != NULL) {
        for (i = 0; i < 8; i++) {
            CommPlayerManager_RemovePlayer((u8)i, 1, 1);
        }
    }
}

void sub_02056E60(void) {
    if (gCommPlayerManager != NULL) {
        gCommPlayerManager->sendAllPositions = 1;
        gCommPlayerManager->unk_EE = 0;
        CommPlayerManager_InitLocalPlayer();
        CommPlayerManager_SendLocalPosition(1);
        PlayerAvatar_GetXCoord(gCommPlayerManager->fieldSystem->playerAvatar);
        PlayerAvatar_GetZCoord(gCommPlayerManager->fieldSystem->playerAvatar);
    }
}

void CommPlayerManager_Delete(int deleteFromMap) {
    int i;
    if (gCommPlayerManager != NULL) {
        for (i = 0; i < 8; i++) {
            CommPlayerManager_RemovePlayer((u8)i, 0, deleteFromMap);
        }
        SysTask_Destroy(gCommPlayerManager->task);
        Heap_Free(gCommPlayerManager);
        gCommPlayerManager = NULL;
    }
}

BOOL CommPlayerManager_IsInitialized(void) {
    return gCommPlayerManager != NULL;
}

void CommPlayerManager_InitLocalPlayer(void) {
    gCommPlayerManager->avatars[Comm_GetLocalNetId()] = gCommPlayerManager->fieldSystem->playerAvatar;
    gCommPlayerManager->active[Comm_GetLocalNetId()] = 1;
    gCommPlayerManager->positions[Comm_GetLocalNetId()].x = PlayerAvatar_GetXCoord(gCommPlayerManager->fieldSystem->playerAvatar);
    gCommPlayerManager->positions[Comm_GetLocalNetId()].z = PlayerAvatar_GetZCoord(gCommPlayerManager->fieldSystem->playerAvatar);
    gCommPlayerManager->positions[Comm_GetLocalNetId()].facing = PlayerAvatar_GetFacingDirection(gCommPlayerManager->fieldSystem->playerAvatar);
    gCommPlayerManager->hostPositions[Comm_GetLocalNetId()].x = PlayerAvatar_GetXCoord(gCommPlayerManager->fieldSystem->playerAvatar);
    gCommPlayerManager->hostPositions[Comm_GetLocalNetId()].z = PlayerAvatar_GetZCoord(gCommPlayerManager->fieldSystem->playerAvatar);
    gCommPlayerManager->hostPositions[Comm_GetLocalNetId()].facing = PlayerAvatar_GetFacingDirection(gCommPlayerManager->fieldSystem->playerAvatar);
}

void CommPlayerManager_SendPosition(int forceSend, int x, int z) {
    u8 data[8];
    int facing = PlayerAvatar_GetFacingDirection(gCommPlayerManager->fieldSystem->playerAvatar);
    data[0] = x;
    data[1] = x >> 8;
    data[2] = z;
    data[3] = z >> 8;
    data[4] = facing;
    if (forceSend) {
        data[4] |= 0x80;
    }
    sub_020376E0(0x16, data);
    gCommPlayerManager->sendAllPositions = 1;
}

void CommPlayerManager_SendLocalPosition(int forceSend) {
    int x = PlayerAvatar_GetXCoord(gCommPlayerManager->fieldSystem->playerAvatar);
    int z = PlayerAvatar_GetZCoord(gCommPlayerManager->fieldSystem->playerAvatar);
    CommPlayerManager_SendPosition(forceSend, x, z);
}

void CommPlayerManager_SendHostPosition(int netId, const CommPlayerPosition *position) {
    u8 data[8];
    u16 x = position->x;
    u16 z = position->z;

    if (x >= 0xF000) {
        x = 0xEFFF;
    }
    if (z >= 0xF000) {
        z = 0xEFFF;
    }
    data[0] = netId & 0xF;
    data[1] = x;
    data[2] = ((x >> 8) & 1) + ((z >> 7) & 2);
    data[3] = z;
    if (position->bumped != 0) {
        data[2] |= 0x80;
    }
    data[0] += (position->facing % 4) << 4;
    data[0] += position->speed << 6;
    sub_02037108(0x17, data, 0);
}

void sub_020570C8(FieldSystem *unused) {
    int moveState = PlayerAvatar_GetMoveState(gCommPlayerManager->fieldSystem->playerAvatar);
    int playerMoveState = PlayerAvatar_GetPlayerMoveState(gCommPlayerManager->fieldSystem->playerAvatar);
    if (moveState == 0 && Comm_IsNetIdConnected(Comm_GetLocalNetId()) && gCommPlayerManager->localPositionSent == 0) {
        CommPlayerManager_SendLocalPosition(1);
        gCommPlayerManager->localPositionSent = 1;
    }
    if (moveState == 1 && playerMoveState == 1) {
        gCommPlayerManager->localPositionSent = 0;
    }
}

void CommPlayerManager_SendChangedHostPositions(FieldSystem *unused) {
    int netId;

    for (netId = 0; netId < 8; netId++) {
        CommPlayerManager *manager = gCommPlayerManager;

        if (manager->active[netId] != 0) {
            u8 changed = manager->positionChanged[netId];
            CommPlayerPosition *positions = manager->hostPositions;

            if (changed != 0 || manager->sendAllPositions != 0) {
                manager->positionChanged[netId] = 0;
                CommPlayerManager_SendHostPosition(netId, &positions[netId]);
            }
        }
    }
    gCommPlayerManager->sendAllPositions = 0;
}

u32 CommPlayerManager_sizeof(void) {
    return sizeof(CommPlayerManager);
}

#pragma force_active on

void CommPlayerManager_CreateAvatar(int netId) {
    PlayerProfile *profile;
    PlayerAvatar *avatar;
    LocalMapObject *object;
    int version;
    int game;

    if (gCommPlayerManager->avatars[netId] || gCommPlayerManager->unk_EE) {
        return;
    }
    profile = CommPlayerInfo_GetProfile(netId);
    if (!profile) {
        return;
    }
    if (netId != Comm_GetLocalNetId()) {
        object = MapObjectManager_GetFirstActiveObjectByID(gCommPlayerManager->fieldSystem->mapObjectManager, netId + 256);
        if (object) {
            MapObject_Remove(object);
        }
    }
    version = 2;
    game = PlayerProfile_GetVersion(profile);
    if (game == 0) {
        version = 1;
    } else if (game == 12) {
        version = 0;
    }
    avatar = PlayerAvatar_CreateWithParams(gCommPlayerManager->fieldSystem->mapObjectManager, gCommPlayerManager->positions[netId].x, gCommPlayerManager->positions[netId].z, gCommPlayerManager->positions[netId].facing, 0, PlayerProfile_GetTrainerGender(profile), version, 0);
    if (!avatar) {
        GF_AssertFail();
    }
    gCommPlayerManager->avatars[netId] = avatar;
    MapObject_SetID(PlayerAvatar_GetMapObject(avatar), netId + 256);
    gCommPlayerManager->active[netId] = 1;
}

#pragma force_active off

void CommPlayerManager_RemovePlayer(int netId, int keepActive, int deleteFromMap) {
    PlayerAvatar *avatar;
    u8 *active;
    if (gCommPlayerManager == NULL) {
        return;
    }
    avatar = gCommPlayerManager->avatars[netId];
    if (avatar != NULL) {
        if (gCommPlayerManager->fieldSystem->playerAvatar != avatar) {
            if (deleteFromMap != 0) {
                PlayerAvatar_DeleteFromMap(avatar);
            } else {
                PlayerAvatar_FreeToHeap(avatar);
            }
        }
        gCommPlayerManager->avatars[netId] = NULL;
    }
    active = gCommPlayerManager->active;
    if (active[netId] != 0 && keepActive == 0) {
        active[netId] = 0;
    }
    gCommPlayerManager->positionChanged[netId] = 1;
}

void sub_020572AC(FieldSystem *fieldSystem) {
    int mode = 2;
    if (gSystem.heldKeys & PAD_BUTTON_B) {
        mode = 1;
    } else if (fieldSystem != NULL && PlayerAvatar_CheckRunningShoesLock(FieldSystem_GetPlayerAvatar(fieldSystem))) {
        mode = 1;
    }
    sub_02037498(mode);
}

void CommPlayerManager_Task(SysTask *task, void *fieldSystem) {
    if (sub_02037474()) {
        sub_020572AC(fieldSystem);
        if (Comm_GetLocalNetId() == 0) {
            CommPlayerManager_SendChangedHostPositions(fieldSystem);
            sub_020570C8(fieldSystem);
        } else {
            sub_020570C8(fieldSystem);
        }
        CommPlayerManager_UpdateAvatars(fieldSystem);
    }
}

void CommPlayerManager_UpdateAvatars(FieldSystem *unused) {
    int i;
    u8 netId;
    for (i = 0; i < 8; i++) {
        if (Comm_IsNetIdConnected((u16)i)) {
            CommPlayerManager_MoveAvatar(i);
        } else if (gCommPlayerManager->active[i] != 0) {
            netId = i;
            CommPlayerManager_ReceiveRemovePlayer(0, 1, &netId, gCommPlayerManager->fieldSystem);
        }
    }
}

void sub_02057358(int value) {
    if (gCommPlayerManager != NULL) {
        if (gCommPlayerManager->unk_ED != value) {
            gCommPlayerManager->unk_ED = value;
        }
        sub_020376E0(0x3E, &gCommPlayerManager->unk_ED);
    }
}

void sub_02057384(int value) {
    if (gCommPlayerManager != NULL && gCommPlayerManager->unk_ED != value) {
        gCommPlayerManager->unk_ED = value;
        sub_020376E0(0x3E, &gCommPlayerManager->unk_ED);
    }
}

void sub_020573AC(int netId, int size, const u8 *data) {
    int valid;
    if (gCommPlayerManager == NULL) {
        return;
    }
    if (size != 1) {
        GF_AssertFail();
    }
    if (netId >= 8) {
        GF_AssertFail();
    }
    valid = 1;
    if (*data != 1 && *data != 0) {
        valid = 0;
    }
    if (!valid) {
        GF_AssertFail();
    }
    sub_02057C5C(netId, *data);
}

void sub_020573F0(FieldSystem *fieldSystem, int value) {
    PlayerAvatar *avatar;
    if (gCommPlayerManager == NULL) {
        return;
    }
    sub_02057384(value);
    gCommPlayerManager->unk_F1 = value;
    avatar = fieldSystem->playerAvatar;
    if (avatar != NULL) {
        gCommPlayerManager->avatars[Comm_GetLocalNetId()] = avatar;
    }
    if (sub_02037474() && Comm_GetLocalNetId() == 0) {
        CommPlayerManager_MoveHostPlayers(NULL, gCommPlayerManager);
    }
    if (sub_02037474() && Comm_GetLocalNetId() == 0) {
        sub_02057D74();
    }
}

int CommPlayerManager_KeysToDirection(int unused, int keys) {
    if (keys & PAD_KEY_LEFT) {
        return 2;
    }
    if (keys & PAD_KEY_RIGHT) {
        return 3;
    }
    if (keys & PAD_KEY_UP) {
        return 0;
    }
    if (keys & PAD_KEY_DOWN) {
        return 1;
    }
    return -1;
}

BOOL CommPlayerManager_IsObjectEventAt(int x, int z) {
    int count = Field_GetNumObjectEvents(gCommPlayerManager->fieldSystem);
    const ObjectEvent *events = Field_GetObjectEvents(gCommPlayerManager->fieldSystem);
    int i;
    for (i = 0; i < count; i++, events++) {
        if (x == events->x && z == events->z) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CommPlayerManager_IsPositionBlocked(int x, int z, int excludedNetId) {
    int netId;
    if (x != 0xFFFF && z != 0xFFFF) {
        for (netId = 0; netId < 8; netId++) {
            if (netId != excludedNetId && CommPlayerManager_GetHostX(netId) == x && CommPlayerManager_GetHostZ(netId) == z) {
                return TRUE;
            }
        }
        if (CommPlayerManager_IsObjectEventAt(x, z)) {
            return TRUE;
        }
    }
    return sub_020548C0(gCommPlayerManager->fieldSystem, x, z);
}

u32 CommPlayerManager_GetMoveDelay(int speed) {
    u32 delays[5] = { 2, 4, 8, 16, 2 };
    if (speed >= 5) {
        GF_AssertFail();
    }
    return delays[speed];
}

#pragma force_active on

void CommPlayerManager_MoveHostPlayers(SysTask *task, CommPlayerManager *unused) {
    int i;
    for (i = 0; i < 8; i++) {
        CommPlayerPosition *position;
        int direction;
        int x;
        int z;
        if (!gCommPlayerManager->active[i] || !gCommPlayerManager->unk_B4[i]) {
            continue;
        }
        position = &gCommPlayerManager->hostPositions[i];
        if (gCommPlayerManager->hostMoveTimer[i]) {
            --gCommPlayerManager->hostMoveTimer[i];
        }
        if (gCommPlayerManager->unk_BC[i]) {
            direction = 0;
        } else {
            direction = sub_020374C0(i);
        }
        position->speed = sub_020374AC(i);
        if (gCommPlayerManager->hostMoveTimer[i]) {
            continue;
        }
        if (position->bumped == 1) {
            gCommPlayerManager->positionChanged[i] = 1;
        }
        position->bumped = 0;
        if (gCommPlayerManager->unk_C4[i] == 2) {
            gCommPlayerManager->unk_C4[i] = 1;
            continue;
        }
        direction = CommPlayerManager_KeysToDirection(direction, direction);
        x = CommPlayerManager_GetHostFrontX(i);
        z = CommPlayerManager_GetHostFrontZ(i);
        if (x == 0xFFFF || z == 0xFFFF || direction == -1) {
            continue;
        }
        if (position->facing != direction) {
            position->facing = direction;
            gCommPlayerManager->hostMoveTimer[i] = 4;
            gCommPlayerManager->positionChanged[i] = 1;
            if (gCommPlayerManager->unk_C4[i] > 2) {
                --gCommPlayerManager->unk_C4[i];
            }
        } else if (!gCommPlayerManager->unk_C4[i]) {
            if (CommPlayerManager_IsPositionBlocked(x, z, i)) {
                position->bumped = 1;
                gCommPlayerManager->positionChanged[i] = 1;
                gCommPlayerManager->hostMoveTimer[i] = 4;
            } else {
                gCommPlayerManager->positionChanged[i] = 1;
                position->x = x;
                position->z = z;
                position->facing = direction;
                gCommPlayerManager->hostMoveTimer[i] = CommPlayerManager_GetMoveDelay(position->speed);
            }
        }
    }
}

void CommPlayerManager_ReceivePosition(int netId, int size, const u8 *data) {
    CommPlayerPosition *position;
    int i;
    if (gCommPlayerManager == NULL) {
        return;
    }
    position = &gCommPlayerManager->hostPositions[netId];
    if (data[4] & 0x80) {
        gCommPlayerManager->sendAllPositions = 1;
        return;
    }
    if (position->facing == -1) {
        for (i = 0; i < 8; i++) {
            gCommPlayerManager->positionChanged[i] = 1;
        }
    }
    position->x = 0;
    position->z = 0;
    position->x += data[0];
    position->x += (data[1] << 8) & 0xFF00;
    position->z += data[2];
    position->z += (data[3] << 8) & 0xFF00;
    position->facing = data[4] & 15;
    gCommPlayerManager->active[netId] = 1;
    gCommPlayerManager->positionChanged[netId] = 1;
    GF_ASSERT(sub_020548C0(gCommPlayerManager->fieldSystem, position->x, position->z) == 0);
}

#pragma force_active off

void CommPlayerManager_ReceiveRemovePlayer(int unused, int size, const u8 *data, FieldSystem *fieldSystem) {
    int netId = *data;
    if (gCommPlayerManager != NULL && netId != Comm_GetLocalNetId()) {
        CommPlayerManager_RemovePlayer(netId, 0, 0);
        CommPlayerInfo_ClearPlayer(netId);
    }
}

int CommPlayerManager_GetPositionSize(void) {
    return 5;
}

#pragma force_active on

void CommPlayerManager_ReceiveHostPosition(int unused, int size, const u8 *data) {
    CommPlayerPosition *position;
    int netId = data[0] & 0xF;
    if (gCommPlayerManager != NULL) {
        position = gCommPlayerManager->positions;
        position += netId;
        if (data[2] & 0x80) {
            position->bumped = 1;
        } else {
            position->bumped = 0;
        }
        position->x = 0;
        position->z = 0;
        position->x += data[1];
        position->x += (data[2] << 8) & 0x100;
        position->z += data[3];
        position->z += (data[2] << 7) & 0x100;
        position->facing = (data[0] >> 4) & 3;
        position->speed = (data[0] >> 6) & 3;
        CommPlayerManager_CreateAvatar((u8)netId);
    }
}

void CommPlayerManager_MoveAvatar(int netId) {
    CommPlayerPosition *position = &gCommPlayerManager->positions[netId];
    if (gCommPlayerManager->moveTimer[netId]) {
        gCommPlayerManager->moveTimer[netId]--;
        return;
    }
    if (position->x != 0xFFFF && position->z != 0xFFFF && !gCommPlayerManager->unk_EE && (netId != Comm_GetLocalNetId() || gCommPlayerManager->unk_F1)) {
        PlayerAvatar *avatar;
        int speed;
        int dx;
        int direction;
        u32 moveSpeed;
        int dz;
        u16 keys;
        int movement;

        avatar = gCommPlayerManager->avatars[netId];
        if (!avatar) {
            return;
        }
        dx = (s32)((u32)PlayerAvatar_GetXCoord(avatar) - position->x);
        dz = (s32)((u32)PlayerAvatar_GetZCoord(avatar) - position->z);
        direction = PlayerAvatar_GetFacingDirection(avatar);
        if (!dx && !dz) {
            keys = 0;
        } else if (abs(dx) > abs(dz)) {
            if (dx > 0) {
                keys = PAD_KEY_LEFT;
            } else {
                keys = PAD_KEY_RIGHT;
            }
        } else {
            if (dz > 0) {
                keys = PAD_KEY_UP;
            } else {
                keys = PAD_KEY_DOWN;
            }
        }
        (void)abs(dz);
        (void)abs(dx);
        speed = position->speed;
        movement = 255;
        switch (speed) {
        case 0:
            moveSpeed = 5;
            break;
        case 1:
            moveSpeed = 4;
            keys |= PAD_BUTTON_B;
            break;
        case 2:
            moveSpeed = 2;
            break;
        case 3:
            moveSpeed = 1;
            break;
        }
        if (gCommPlayerManager->localMoveDelay && netId == Comm_GetLocalNetId()) {
            gCommPlayerManager->localMoveDelay--;
        } else {
            int dirKeys = keys & ~PAD_BUTTON_B;
            if (!dirKeys && position->bumped) {
                speed = 3;
                switch (position->facing) {
                case 0:
                    movement = 28;
                    break;
                default:
                case 1:
                    movement = 29;
                    break;
                case 2:
                    movement = 30;
                    break;
                case 3:
                    movement = 31;
                    break;
                }
            } else if (!dirKeys && position->facing != direction) {
                movement = GetMovementForDirection(position->facing, 36);
            } else {
                movement = sub_0205DF0C(avatar, keys, keys, moveSpeed, 1, 0);
            }
        }
        if (sub_0205DFC8(avatar) || sub_0205DE38(avatar)) {
            if (movement != 255) {
                PlayerAvatar_SetMovement(avatar, movement);
                if (keys & ~PAD_BUTTON_B) {
                    if (!gCommPlayerManager->moveTimer[netId]) {
                        gCommPlayerManager->moveTimer[netId] = CommPlayerManager_GetMoveDelay(speed);
                    }
                    if (gCommPlayerManager->moveTimer[netId]) {
                        gCommPlayerManager->moveTimer[netId]--;
                    }
                }
            }
        }
    }
}

#pragma force_active off

int CommPlayerManager_GetHostPositionSize(void) {
    return 4;
}

BOOL CommPlayerManager_IsLocalPlayerConnected(void) {
    int netId;
    if (gCommPlayerManager == NULL) {
        return FALSE;
    }
    netId = Comm_GetLocalNetId();
    return Comm_IsNetIdConnected(netId) != 0;
}

int CommPlayerManager_GetPlayerX(int netId) {
    CommPlayerManager *manager;
    if (gCommPlayerManager == NULL) {
        return 0xFFFF;
    }
    if (netId == Comm_GetLocalNetId()) {
        return gCommPlayerManager->positions[netId].x;
    }
    if (CommPlayerManager_IsLocalPlayerConnected()) {
        manager = gCommPlayerManager;
        if (manager->active[netId] != 0) {
            goto found;
        }
    }
    return 0xFFFF;
found:
    return manager->positions[netId].x;
}

int CommPlayerManager_GetPlayerZ(int netId) {
    CommPlayerManager *manager;
    if (gCommPlayerManager == NULL) {
        return 0xFFFF;
    }
    if (netId == Comm_GetLocalNetId()) {
        return gCommPlayerManager->positions[netId].z;
    }
    if (CommPlayerManager_IsLocalPlayerConnected()) {
        manager = gCommPlayerManager;
        if (manager->active[netId] != 0) {
            goto found;
        }
    }
    return 0xFFFF;
found:
    return manager->positions[netId].z;
}

int CommPlayerManager_GetPlayerFrontX(int netId) {
    int x = CommPlayerManager_GetPlayerX(netId);
    if (x == 0xFFFF) {
        return 0xFFFF;
    }
    return gCommPlayerManager->positions[netId].x + GetDeltaXByFacingDirection(gCommPlayerManager->positions[netId].facing);
}

int CommPlayerManager_GetPlayerFrontZ(int netId) {
    int z = CommPlayerManager_GetPlayerZ(netId);
    if (z == 0xFFFF) {
        return 0xFFFF;
    }
    return gCommPlayerManager->positions[netId].z + GetDeltaYByFacingDirection(gCommPlayerManager->positions[netId].facing);
}

int CommPlayerManager_GetHostX(int netId) {
    if (gCommPlayerManager == NULL) {
        return 0xFFFF;
    }
    if (netId == Comm_GetLocalNetId()) {
        return gCommPlayerManager->hostPositions[netId].x;
    }
    if (!CommPlayerManager_IsLocalPlayerConnected() || gCommPlayerManager->active[netId] == 0) {
        return 0xFFFF;
    }
    return gCommPlayerManager->hostPositions[netId].x;
}

int CommPlayerManager_GetHostZ(int netId) {
    if (gCommPlayerManager == NULL) {
        return 0xFFFF;
    }
    if (netId == Comm_GetLocalNetId()) {
        return gCommPlayerManager->hostPositions[netId].z;
    }
    if (!CommPlayerManager_IsLocalPlayerConnected() || gCommPlayerManager->active[netId] == 0) {
        return 0xFFFF;
    }
    return gCommPlayerManager->hostPositions[netId].z;
}

int CommPlayerManager_GetHostFrontX(int netId) {
    if (CommPlayerManager_GetHostX(netId) == 0xFFFF) {
        return 0xFFFF;
    }
    return gCommPlayerManager->hostPositions[netId].x + GetDeltaXByFacingDirection(gCommPlayerManager->hostPositions[netId].facing);
}

int CommPlayerManager_GetHostFrontZ(int netId) {
    if (CommPlayerManager_GetHostZ(netId) == 0xFFFF) {
        return 0xFFFF;
    }
    return gCommPlayerManager->hostPositions[netId].z + GetDeltaYByFacingDirection(gCommPlayerManager->hostPositions[netId].facing);
}

// Every access reads the manager pointer again (volatile).
#define MANAGER_VOLATILE (*(CommPlayerManager *volatile *)&gCommPlayerManager)

void sub_02057C5C(int netId, int value) {
    u8 *values = MANAGER_VOLATILE->unk_B4;
    if (values[netId] != value) {
        values[netId] = value;
        MANAGER_VOLATILE->hostPositions[netId].bumped = 0;
        MANAGER_VOLATILE->positionChanged[netId] = 1;
        MANAGER_VOLATILE->hostPositions[netId].speed = 2;
    }
}

#undef MANAGER_VOLATILE

BOOL sub_02057C94(void) {
    u16 coords2[4];
    u16 coords4[8];
    int mapping[4];
    int count;
    int result;
    int n;
    int i;
    int j;
    const u16 *coords;

    coords2[0] = _020FC7A0[0];
    {
        const u16 *src = _020FC7A8;

        coords2[1] = _020FC7A0[1];
        coords2[2] = _020FC7A0[2];
        coords2[3] = _020FC7A0[3];
        u32 copyCount;
        u16 *dst;

        dst = coords4;
        copyCount = 8;

        do {
            *dst = *src;
            src++;
            dst++;
        } while (--copyCount != 0);
    }
    n = sub_0203774C(sub_0203993C());
    count = 0;
    result = FALSE;
    coords = n == 2 ? coords2 : coords4;
    for (i = 0; i < n; i++, coords += 2) {
        for (j = 0; j < n; j++) {
            if (coords[0] == CommPlayerManager_GetPlayerX(j) && coords[1] == CommPlayerManager_GetPlayerZ(j)) {
                count++;
                mapping[j] = i;
                if (j == Comm_GetLocalNetId()) {
                    result = TRUE;
                }
                break;
            }
        }
    }
    if (count == n) {
        for (i = 0; i < n; i++) {
            sub_02037894(mapping[i], i);
        }
    }
    return result;
}

void sub_02057D50(int netId, int size, const u8 *data) {
    int value = *data;
    if (value == 3) {
        gCommPlayerManager->unk_BC[netId] = 0;
    } else {
        gCommPlayerManager->unk_E6[netId] = value;
    }
}

void sub_02057D74(void) {
    int count;
    const u16 *table;
    u8 i;

    count = sub_0203774C(sub_0203993C());
    table = count == 2 ? _0210FAB4 : _0210FABC;
    i = 0;
    while (i < count) {
        if (gCommPlayerManager->unk_E6[i] != 0) {
            int j;
            const u16 *pair;
            j = 0;
            if (j < count) {
                pair = table;
                do {
                    if (CommPlayerManager_GetHostX(i) == pair[0] && CommPlayerManager_GetHostZ(i) == pair[1]) {
                        gCommPlayerManager->unk_BC[i] = 1;
                        sub_02037184(0x5F, &i);
                    }
                    j++;
                    pair += 2;
                } while (j < count);
            }
        }
        i++;
    }
}

void sub_02057E08(void) {
    int netId = Comm_GetLocalNetId();
    int movement;

    if (CommPlayerManager_GetPlayerX(netId) > 8) {
        CommPlayerManager_SetLocalFacing(2);
        movement = GetMovementForDirection(2, 0x24);
    } else {
        CommPlayerManager_SetLocalFacing(3);
        movement = GetMovementForDirection(3, 0x24);
    }
    PlayerAvatar_SetMovement(gCommPlayerManager->avatars[netId], movement);
    CommPlayerManager_SetLocalMoveDelay();
}

BOOL sub_02057E50(void) {
    int count;
    int i;
    CommPlayerPosition *position;

    count = sub_0203774C(sub_0203993C());
    for (i = 0; i < count; i++) {
        if (gCommPlayerManager->active[i] == 0 || gCommPlayerManager->positionChanged[i] == 1) {
            return FALSE;
        }
    }
    for (i = 0; i < count; i++) {
        int facing;
        position = &gCommPlayerManager->hostPositions[i];
        facing = CommPlayerManager_GetOppositeDirection(position->facing);
        position->x += GetDeltaXByFacingDirection(facing);
        position->z += GetDeltaYByFacingDirection(facing);
        position->speed = 2;
        gCommPlayerManager->positionChanged[i] = 1;
        sub_02057C5C(i, 1);
    }
    return TRUE;
}

int CommPlayerManager_GetOppositeDirection(int facing) {
    if (facing == 0) {
        return 1;
    }
    if (facing == 1) {
        return 0;
    }
    if (facing == 2) {
        return 3;
    }
    if (facing == 3) {
        return 2;
    }
    GF_AssertFail();
    return 2;
}

int CommPlayerManager_GetMoveTimer(int netId) {
    return gCommPlayerManager->moveTimer[netId];
}

void CommPlayerManager_SetLocalFacing(int facing) {
    PlayerAvatar_SetFacingDirection(gCommPlayerManager->fieldSystem->playerAvatar, facing);
    gCommPlayerManager->positions[Comm_GetLocalNetId()].facing = facing;
    CommPlayerManager_SendLocalPosition(0);
}

void sub_02057F58(void) {
    sub_0203E2F4();
    gCommPlayerManager->unk_ED = 0;
}

void sub_02057F70(void) {
    sub_0203E30C();
    sub_02057358(1);
}

void sub_02057F80(void) {
    if (gCommPlayerManager->unk_EF == 0) {
        sub_02057F70();
        gCommPlayerManager->unk_EF = 1;
    }
}

void CommPlayerManager_ForcePosition(void) {
    int i;
    if (gCommPlayerManager == NULL) {
        return;
    }
    for (i = 0; i < 8; i++) {
        LocalMapObject *object;
        u32 x;
        u32 z;
        u32 facing;

        if (gCommPlayerManager->avatars[i] != NULL && i != Comm_GetLocalNetId()) {
            object = PlayerAvatar_GetMapObject(gCommPlayerManager->avatars[i]);
            x = gCommPlayerManager->positions[i].x;
            z = gCommPlayerManager->positions[i].z;
            facing = gCommPlayerManager->positions[i].facing;
            MapObject_SetCurrentX(object, x);
            MapObject_SetCurrentZ(object, z);
            MapObject_SetPositionFromXYZAndDirection(object, x, 0, z, facing);
        }
    }
}

void CommPlayerManager_SetLocalMoveDelay(void) {
    gCommPlayerManager->localMoveDelay = 8;
}
