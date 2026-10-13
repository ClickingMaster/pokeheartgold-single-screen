#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "math_util.h"
#include "overlay_42.h"
#include "overlay_44_02232E9C.h"
#include "sprite_transfer.h"
#include "sprite_resource.h"
#include "vram_transfer_manager.h"

typedef struct WifiClubRoomLightStep {
    u8 timed;
    u8 frames;
    u8 setColor;
    u8 color;
} WifiClubRoomLightStep;

const WifiClubRoomLightStep sWifiClubRoomLightSequence[8] = {
    { 0, 0, 0, 1 },
    { 1, 2, 1, 0 },
    { 1, 4, 1, 1 },
    { 1, 2, 1, 0 },
    { 0, 0, 0, 1 },
    { 1, 2, 1, 1 },
    { 1, 4, 1, 0 },
    { 1, 3, 1, 1 },
};

const u16 sWifiClubRoomLightBlinkFrames[2] = { 3, 2 };

YesNoPrompt *WifiClub_CreateYesNoPromptIgnoreTouch(BgConfig *bgConfig, const WindowTemplate *windowTemplate, u32 initialCursorPos) {
    YesNoPrompt *yesNoPrompt = WifiClub_CreateYesNoPrompt(bgConfig, windowTemplate, initialCursorPos);
    YesNoPrompt_SetIgnoreTouch(yesNoPrompt, 1);
    return yesNoPrompt;
}

YesNoPrompt *WifiClub_CreateYesNoPrompt(BgConfig *bgConfig, const WindowTemplate *windowTemplate, u32 initialCursorPos) {
    YesNoPrompt *yesNoPrompt = YesNoPrompt_Create(HEAP_ID_53);
    YesNoPromptTemplate yesNoPromptTemplate;
    yesNoPromptTemplate.bgConfig = bgConfig;
    yesNoPromptTemplate.bgId = windowTemplate->bgId;
    yesNoPromptTemplate.tileStart = windowTemplate->baseTile;
    yesNoPromptTemplate.plttSlot = windowTemplate->palette;
    yesNoPromptTemplate.x = windowTemplate->left;
    yesNoPromptTemplate.y = windowTemplate->top;
    yesNoPromptTemplate.ignoreTouchFlag = 0;
    yesNoPromptTemplate.initialCursorPos = initialCursorPos;
    yesNoPromptTemplate.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(yesNoPrompt, &yesNoPromptTemplate);
    return yesNoPrompt;
}

void WifiClubRoom_Init(WifiClubRoom *room, enum HeapID heapID, NARC *narc, BgConfig *bgConfig, s32 playerSpriteId, u32 friendCount) {
    memset(room, 0, sizeof(WifiClubRoom));
    room->heapID = heapID;
    u32 mapIndex = (friendCount - 1) / 8;
    room->bgConfig = bgConfig;
    WifiClubRoomGfx_Init(&room->gfx, room->heapID, narc);
    WifiClubRoom_InitBgs(room->bgConfig, heapID);

    Field2DPoint mapSize = WifiClubRoom_GetMapSize(mapIndex);
    room->map = Field2DMap_New(mapSize.unk0, mapSize.unk2, heapID);
    Field2DMap_Load(room->map, WifiClubRoom_GetMapCells(mapIndex));
    room->objects = Field2DObjectManager_New(0x30, heapID);
    WifiClubRoom_CreateDrawers(room, playerSpriteId, heapID);
    Field2DCamera_Init(&room->cameraPos);
    WifiClubRoom_CreateMapBg(room, heapID, narc, mapIndex);
    WifiClubRoom_LoadBgGfx(room->bgConfig, heapID, narc);
    WifiClubRoomLights_Init(room, &room->lights, narc);
    room->commandQueue = Field2DCommandQueue_New(0x80, heapID);
    room->initialized = 1;
}

void WifiClubRoom_Free(WifiClubRoom *room) {
    Field2DCommandQueue_Free(room->commandQueue);
    WifiClubRoom_DeleteMapBg(room);
    WifiClubRoomLights_Free(room, &room->lights);
    WifiClubRoom_DeleteAllPersons(room);
    WifiClubRoom_DeleteDrawers(room);
    Field2DObjectManager_Free(room->objects);
    Field2DMap_Free(room->map);
    WifiClubRoomGfx_Free(&room->gfx);
    WifiClubRoom_FreeBgs(room->bgConfig);
    memset(room, 0, sizeof(WifiClubRoom));
}

s32 WifiClubRoom_IsInitialized(WifiClubRoom *room) {
    return room->initialized;
}

s32 WifiClubRoom_Update(WifiClubRoom *room) {
    Field2DObjectManager_Update(room->objects);
    WifiClubRoom_UpdatePersons(room);
    WifiClubRoom_ProcessMoveCommands(room);
    Field2DCamera_CenterOnObject(&room->cameraPos, room->player->object);
    Field2DBgLayer_SetCamera(room->mapBg, &room->cameraPos);
    Field2DObjectSpriteManager_Update(room->drawers);
    WifiClubRoom_AnimatePersons(room);
    WifiClubRoom_HideExitArrow(room);
    if (WifiClubRoom_IsPlayerIdle(room) == 1) {
        if (WifiClubRoom_IsPlayerAtEntrance(room) == 1) {
            WifiClubRoom_ShowExitArrow(room);
            if (gSystem.heldKeys & PAD_KEY_DOWN) {
                return 1;
            }
        }
        if (gSystem.newKeys & PAD_BUTTON_A) {
            if (WifiClubRoom_GetTalkPartnerId(room) != 0) {
                return 2;
            }
            if (WifiClubRoom_IsPlayerFacingCounter(room) == 1) {
                return 3;
            }
        }
    }
    return 0;
}

void WifiClubRoom_Draw(WifiClubRoom *room) {
    if (room->initialized != 0) {
        SpriteList_RenderAndAnimateSprites(room->gfx.spriteList);
    }
}

u8 WifiClubRoom_GetTalkPartnerId(WifiClubRoom *room) {
    GF_ASSERT(room->player != NULL);
    return room->player->talkPartnerId;
}

s32 WifiClubRoom_GetTileUnderPlayer(WifiClubRoom *room) {
    GF_ASSERT(room->player != NULL);
    Field2DPoint pos = Field2DObject_GetPos(room->player->object);
    return Field2DMap_GetTag(room->map, pos.unk0 / 16, pos.unk2 / 16);
}

s32 WifiClubRoom_GetTileBelowPlayer(WifiClubRoom *room) {
    GF_ASSERT(room->player != NULL);
    Field2DPoint pos = Field2DObject_GetPos(room->player->object);
    return Field2DMap_GetTag(room->map, pos.unk0 / 16, (pos.unk2 / 16) + 1);
}

void WifiClubRoom_ShowFriendMarker(WifiClubRoom *room, WifiClubRoomPerson *person) {
    Field2DPoint pos = Field2DObject_GetPos(person->object);
    WifiClubRoom_ShowFriendMarkerAt(room, pos, Field2DObjectSprite_GetDrawPriority(person->drawer) - 2);
}

void WifiClubRoom_HideFriendMarker(WifiClubRoom *room) {
    WifiClubRoom_HideFriendMarkerSprite(room);
}

void WifiClubRoom_SetInputLocked(WifiClubRoom *room, s32 locked) {
    room->inputLocked = locked;
}

s32 WifiClubRoom_IsInputLocked(WifiClubRoom *room) {
    return room->inputLocked;
}

WifiClubRoomPerson *WifiClubRoom_AddPlayer(WifiClubRoom *room, s16 spriteId) {
    s16 pos;
    WifiClubRoomPerson *person = WifiClubRoom_AllocPerson(room);
    room->player = person;
    WifiClubRoom_FindEntrancePos(room, &pos);
    WifiClubRoom_CreatePersonObject(room, person, &pos, 0, spriteId, 0, 0);
    WifiClubRoom_SetPersonModePlayer(room, person);
    WifiClubRoom_CreatePersonDrawer(room, person, 1);
    Field2DCamera_CenterOnObject(&room->cameraPos, room->player->object);
    Field2DBgLayer_SetCamera(room->mapBg, &room->cameraPos);
    return person;
}

WifiClubRoomPerson *WifiClubRoom_AddFriend(WifiClubRoom *room, s16 spriteId, s16 id) {
    s16 pos[2];
    WifiClubRoomPerson *person = WifiClubRoom_AllocPerson(room);
    WifiClubRoom_FindFriendSpawnPos(room, &pos[0], id - 1);
    if (room->player != NULL) {
        Field2DPoint playerPos = Field2DObject_GetPos(room->player->object);
        if ((playerPos.unk0 == pos[0]) && (playerPos.unk2 == pos[1])) {
            return NULL;
        }
    }
    WifiClubRoom_CreatePersonObject(room, person, &pos[0], id, spriteId, 1, 4);
    WifiClubRoom_SetPersonModeWarpIn(room, person);
    WifiClubRoom_CreatePersonDrawer(room, person, 0);
    return person;
}

void WifiClubRoom_StartPersonLeave(WifiClubRoom *room, WifiClubRoomPerson *person) {
    Field2DObject_GetParam(person->object, 6);
    person->update = WifiClubRoomPerson_UpdateWarpOut;
    person->animate = WifiClubRoomPerson_AnimateWarp;
    person->mode = 2;
    person->timer = 0;
    Field2DObjectSprite_SetAutoUpdate(person->drawer, 0);
    Field2DObjectSprite_StartLoopAnim(person->drawer);
    WifiClubRoom_QueuePersonCommand(room, 4, person);
}

void WifiClubRoomPerson_Delete(WifiClubRoomPerson *person) {
    Field2DObjectSprite_Delete(person->drawer);
    Field2DObject_Delete(person->object);
    memset(person, 0, sizeof(WifiClubRoomPerson));
}

u8 WifiClubRoomPerson_GetId(WifiClubRoomPerson *person) {
    return Field2DObject_GetParam(person->object, 4);
}

s32 WifiClubRoomPerson_GetOppositeDirection(WifiClubRoomPerson *person) {
    return Field2D_GetOppositeDirection(Field2DObject_GetParam(person->object, 6));
}

void WifiClubRoom_StartTalk(WifiClubRoom *room, WifiClubRoomPerson *person, s32 direction) {
    person->savedUpdate = person->update;
    person->update = WifiClubRoomPerson_UpdateTalking;
    person->savedMode = person->mode;
    person->mode = 5;
    WifiClubRoom_QueueMoveCommand(room, 0, direction, Field2DObject_GetParam(person->object, 4));
    Field2DObjectSprite_SetAutoUpdate(person->drawer, 0);
    Field2DObjectSprite_FaceDirection(person->drawer, direction);

    Field2DPoint pos = { Field2DObject_GetParam(person->object, 0), Field2DObject_GetParam(person->object, 1) };
    Field2DObjectSprite_SetPosition(person->drawer, pos);
}

void WifiClubRoom_EndTalk(WifiClubRoom *room, WifiClubRoomPerson *person) {
    if (person != NULL) {
        if (person->savedUpdate != 0) {
            person->update = person->savedUpdate;
            person->savedUpdate = 0;
            person->mode = person->savedMode;
            Field2DObjectSprite_SetAutoUpdate(person->drawer, 1);
        }
    }
}

void WifiClubRoom_SetPersonBusy(WifiClubRoom *room, WifiClubRoomPerson *person, u8 activityMode) {
    if (WifiClubRoomPerson_IsNotWarping(person) == 0) {
        person->activityMode = activityMode;
        return;
    }
    WifiClubRoom_SetPersonActivityMode(room, person, activityMode);
}

void WifiClubRoom_StartCounterLight(WifiClubRoom *room) {
    if (WifiClubRoom_IsPlayerFacingCounter(room) == 1) {
        s8 light = (s8)WifiClubRoom_GetTileBelowPlayer(room) - 3;
        GF_ASSERT(light >= 0 && light < 4);
        WifiClubRoomLights_Start(&room->lights, light);
        return;
    }
    GF_AssertFail();
}

void WifiClubRoom_ContinueLightSequence(WifiClubRoom *room) {
    WifiClubRoomLights_Continue(&room->lights);
}

void WifiClubRoom_UpdateLights(WifiClubRoom *room) {
    WifiClubRoomLights_Update(room, &room->lights);
}

void WifiClubRoom_DeleteAllPersons(WifiClubRoom *room) {
    for (s32 i = 0; i < 48; i++) {
        if (room->persons[i].object != 0) {
            WifiClubRoomPerson_Delete(&room->persons[i]);
        }
    }
}

s32 WifiClubRoom_IsPlayerIdle(WifiClubRoom *room) {
    if (Field2DObject_GetParam(room->player->object, 5) == 0) {
        return 1;
    }
    return 0;
}

s32 WifiClubRoom_IsPlayerFacingCounter(WifiClubRoom *room) {
    Field2DPoint pos = Field2DObject_GetPos(room->player->object);
    u32 direction = Field2DObject_GetParam(room->player->object, 6);
    pos = Field2D_StepInDirection(pos, direction);
    s32 tag = Field2DMap_GetTag(room->map, pos.unk0 / 16, pos.unk2 / 16);
    if ((direction == 0) && (tag == 2)) {
        return 1;
    }
    return 0;
}

void WifiClubRoom_ProcessMoveCommands(WifiClubRoom *room) {
    Field2DCommand command;
    Field2DMove move;
    if (Field2DCommandQueue_Pop(room->commandQueue, &command) == 1) {
        do {
            if (Field2DObjectManager_TryCommand(room->map, room->objects, &command, &move) == 1) {
                Field2DObjectManager_StartMove(room->objects, &move);
            }
        } while (Field2DCommandQueue_Pop(room->commandQueue, &command) == 1);
    }
}

void WifiClubRoom_UpdatePersons(WifiClubRoom *room) {
    for (s32 i = 0; i < 48; i++) {
        if ((room->persons[i].object != 0) && (room->persons[i].update(room, &room->persons[i]) == 1)) {
            WifiClubRoomPerson_Delete(&room->persons[i]);
        }
    }
}

void WifiClubRoom_AnimatePersons(WifiClubRoom *room) {
    for (s32 i = 0; i < 48; i++) {
        if (room->persons[i].object != 0) {
            room->persons[i].animate(room, &room->persons[i]);
        }
    }
}

WifiClubRoomPerson *WifiClubRoom_GetPersonInDirection(WifiClubRoom *room, WifiClubRoomPerson *person, u32 direction) {
    Field2DObject *object = Field2DObject_FindObjectInDirection(person->object, room->objects, direction);
    if (object != NULL) {
        for (s32 i = 0; i < 48; i++) {
            Field2DObject *personObject = room->persons[i].object;
            if (personObject == object) {
                return &room->persons[i];
            }
        }
    }
    return NULL;
}

void WifiClubRoom_CreateDrawers(WifiClubRoom *room, s32 spriteId, enum HeapID heapID) {
    room->drawers = Field2DObjectSpriteManager_NewWithShadow(room->gfx.spriteList, NULL, 48, spriteId, 0, 1, heapID);
    Field2DObjectSpriteManager_LoadAvatarChars(room->drawers, 2, heapID);
}

void WifiClubRoom_DeleteDrawers(WifiClubRoom *room) {
    Field2DObjectSpriteManager_Free(room->drawers);
}

void WifiClubRoom_CreateMapBg(WifiClubRoom *room, enum HeapID heapID, NARC *narc, s32 mapIndex) {
    Field2DBgLayerTemplate bgLayerTemplate = {
        .unk_0 = 0,
        .unk_1 = 0,
        .colorMode = GX_BG_COLORMODE_16,
        .screenBase = GX_BG_SCRBASE_0xe000,
        .charBase = GX_BG_CHARBASE_0x00000,
        .bgExtPltt = GX_BG_EXTPLTT_01,
        .priority = 3,
        .mosaic = 0,
        .narcId = NARC_a_0_8_8,
        .fileId = 33,
        .compressed = FALSE,
    };

    bgLayerTemplate.fileId += mapIndex;
    room->mapBg = Field2DBgLayer_New(&room->gfx.g2dRenderer, room->bgConfig, &bgLayerTemplate, heapID);
}

void WifiClubRoom_DeleteMapBg(WifiClubRoom *room) {
    Field2DBgLayer_Free(room->mapBg);
}

void WifiClubRoomGfx_Init(WifiClubRoomGfx *gfx, enum HeapID heapID, NARC *narc) {
    gfx->spriteList = G2dRenderer_Init(96, &gfx->g2dRenderer, heapID);
    G2dRenderer_SetSubSurfaceCoords(&gfx->g2dRenderer, 0, 0x320000);
    for (s32 i = 0; i < 4; i++) {
        gfx->resManagers[i] = Create2DGfxResObjMan(16, (GfGfxResType)i, heapID);
    }
    WifiClubRoomGfx_LoadResources(gfx, heapID, narc);
    WifiClubRoomGfx_CreateSprites(gfx, heapID);
}

void WifiClubRoomGfx_Free(WifiClubRoomGfx *gfx) {
    WifiClubRoomGfx_DeleteSprites(gfx);
    WifiClubRoomGfx_FreeResources(gfx);
    for (s32 i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resManagers[i]);
    }
    SpriteList_Delete(gfx->spriteList);
}

void WifiClubRoomGfx_LoadResources(WifiClubRoomGfx *gfx, enum HeapID heapID, NARC *narc) {
    WifiClubRoomGfx_LoadSpriteResources(gfx, heapID, narc);
}

void WifiClubRoomGfx_FreeResources(WifiClubRoomGfx *gfx) {
    WifiClubRoomGfx_FreeSpriteResources(gfx);
}

void WifiClubRoomGfx_CreateSprites(WifiClubRoomGfx *gfx, enum HeapID heapID) {
    WifiClubRoomGfx_CreateMarkerSprites(gfx, heapID);
}

void WifiClubRoomGfx_DeleteSprites(WifiClubRoomGfx *gfx) {
    WifiClubRoomGfx_DeleteMarkerSprites(gfx);
}

void WifiClubRoom_InitBgs(BgConfig *bgConfig, enum HeapID heapID) {
}

void WifiClubRoom_FreeBgs(BgConfig *bgConfig) {
}

void WifiClubRoom_LoadBgGfx(BgConfig *bgConfig, enum HeapID heapID, NARC *narc) {
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 31, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 256, heapID);
    BG_SetMaskColor(0, 0);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 30, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, heapID);
}

void WifiClubRoomGfx_LoadSpriteResources(WifiClubRoomGfx *gfx, enum HeapID heapID, NARC *narc) {
    gfx->resources[0] = AddCharResObjFromOpenNarc(gfx->resManagers[0], narc, NARC_a_0_3_8, FALSE, 50, NNS_G2D_VRAM_TYPE_2DMAIN, heapID);
    gfx->resources[1] = AddPlttResObjFromOpenNarc(gfx->resManagers[1], narc, NARC_fielddata_encountdata_g_enc_data, FALSE, 50, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapID);
    gfx->resources[2] = AddCellOrAnimResObjFromOpenNarc(gfx->resManagers[2], narc, NARC_a_0_3_9, FALSE, 50, GF_GFX_RES_TYPE_CELL, heapID);
    gfx->resources[3] = AddCellOrAnimResObjFromOpenNarc(gfx->resManagers[3], narc, NARC_a_0_4_0, FALSE, 50, GF_GFX_RES_TYPE_ANIM, heapID);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(gfx->resources[0]) == TRUE);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(gfx->resources[1]) == TRUE);
    GF2DGfxResObj_FreeData(gfx->resources[0]);
    GF2DGfxResObj_FreeData(gfx->resources[1]);
    CreateSpriteResourcesHeader(&gfx->resourcesHeader, 50, 50, 50, 50, -1, -1, 0, 2, gfx->resManagers[0], gfx->resManagers[1], gfx->resManagers[2], gfx->resManagers[3], 0, 0);
}

void WifiClubRoomGfx_FreeSpriteResources(WifiClubRoomGfx *gfx) {
    SpriteTransfer_DeleteCharTransferTask(gfx->resources[0]);
    SpriteTransfer_DeletePlttTransferTask(gfx->resources[1]);
    for (s32 i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(gfx->resManagers[i], gfx->resources[i]);
    }
}

void WifiClubRoomGfx_CreateMarkerSprites(WifiClubRoomGfx *gfx, enum HeapID heapID) {
    SpriteTemplate spriteTemplate = { };
    spriteTemplate.spriteList = gfx->spriteList;
    spriteTemplate.header = &gfx->resourcesHeader;

    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;

    spriteTemplate.scale.x = 4096;
    spriteTemplate.scale.y = 4096;
    spriteTemplate.heapID = heapID;

    spriteTemplate.drawPriority = 0;

    gfx->exitArrowSprite = Sprite_CreateAffine(&spriteTemplate);
    gfx->friendMarkerSprite = Sprite_CreateAffine(&spriteTemplate);
    Sprite_SetDrawFlag(gfx->exitArrowSprite, 0);
    Sprite_SetDrawFlag(gfx->friendMarkerSprite, 0);
    Sprite_SetAnimActiveFlag(gfx->exitArrowSprite, 1);
    Sprite_SetAnimCtrlSeq(gfx->friendMarkerSprite, 1);
}

void WifiClubRoomGfx_DeleteMarkerSprites(WifiClubRoomGfx *gfx) {
    Sprite_Delete(gfx->exitArrowSprite);
    Sprite_Delete(gfx->friendMarkerSprite);
}

void WifiClubRoom_ShowExitArrow(WifiClubRoom *room) {
    VecFx32 vec;
    s16 pos[2];

    WifiClubRoom_FindEntrancePos(room, pos);
    pos[0] = pos[0] + 8;
    pos[1] = pos[1] + 32;
    vec.x = pos[0] << 12;
    vec.y = pos[1] << 12;
    Sprite_SetMatrix(room->gfx.exitArrowSprite, &vec);
    Sprite_SetDrawFlag(room->gfx.exitArrowSprite, 1);
}

void WifiClubRoom_HideExitArrow(WifiClubRoom *room) {
    Sprite_SetDrawFlag(room->gfx.exitArrowSprite, 0);
}

void WifiClubRoom_ShowFriendMarkerAt(WifiClubRoom *room, Field2DPoint pos, s32 drawPriority) {
    VecFx32 vec;
    vec.x = (s16)(pos.unk0 + 8) * FX32_ONE;
    vec.y = pos.unk2 * FX32_ONE;
    Sprite_SetMatrix(room->gfx.friendMarkerSprite, &vec);
    Sprite_SetDrawPriority(room->gfx.friendMarkerSprite, drawPriority);
    Sprite_SetDrawFlag(room->gfx.friendMarkerSprite, 1);
}

void WifiClubRoom_HideFriendMarkerSprite(WifiClubRoom *room) {
    Sprite_SetDrawFlag(room->gfx.friendMarkerSprite, 0);
}

s32 WifiClubRoom_IsPlayerAtEntrance(WifiClubRoom *room) {
    if (room->player == 0) {
        return 0;
    }
    Field2DPoint pos = Field2DObject_GetPos(room->player->object);
    u32 direction = Field2DObject_GetParam(room->player->object, 6);
    if (Field2DMap_GetTag(room->map, pos.unk0 / 16, pos.unk2 / 16) == 0xF && direction == 1) {
        return 1;
    }
    return 0;
}

void WifiClubRoom_FindFriendSpawnPos(WifiClubRoom *room, s16 *pos, u32 spawnIndex) {
    u32 counterIndex = spawnIndex / 8;
    u32 seatIndex = spawnIndex % 8;
    u16 width = Field2DMap_GetWidth(room->map);
    u16 height = Field2DMap_GetHeight(room->map);

    for (u32 i = 0; i < height; i++) {
        for (u32 j = 0; j < width; j++) {
            s32 tag = Field2DMap_GetTag(room->map, j, i);
            if (tag == seatIndex + 7) {
                tag = Field2DMap_GetTag(room->map, j, i + 1);
                if (tag == counterIndex + 3) {
                    pos[0] = j << 4;
                    pos[1] = i << 4;
                    return;
                }
            }
        }
    }
    GF_AssertFail();
}

void WifiClubRoom_FindEntrancePos(WifiClubRoom *room, s16 *pos) {
    u16 width = Field2DMap_GetWidth(room->map);
    u16 height = Field2DMap_GetHeight(room->map);

    for (u32 i = 0; i < height; i++) {
        for (u32 j = 0; j < width; j++) {
            if (Field2DMap_GetTag(room->map, j, i) == 15) {
                pos[0] = j << 4;
                pos[1] = i << 4;
                return;
            }
        }
    }
    GF_AssertFail();
}

WifiClubRoomPerson *WifiClubRoom_AllocPerson(WifiClubRoom *room) {
    for (s32 i = 0; i < 48; i++) {
        if (room->persons[i].object == 0) {
            return &room->persons[i];
        }
    }
    GF_AssertFail();
    return NULL;
}

void WifiClubRoom_CreatePersonDrawer(WifiClubRoom *room, WifiClubRoomPerson *person, s32 priorityMode) {
    person->drawer = Field2DObjectSpriteManager_CreateSprite(room->drawers, person->object, priorityMode, room->heapID);
    if (priorityMode == 0) {
        Field2DObjectSprite_SetAutoUpdate(person->drawer, 0);
        Field2DObjectSprite_StartLoopAnim(person->drawer);
        Field2DObjectSprite_SetVisible(person->drawer, 0);
    }
}

void WifiClubRoom_CreatePersonObject(WifiClubRoom *room, WifiClubRoomPerson *person, s16 *pos, s16 id, u16 spriteId, u8 direction, s32 action) {
    Field2DObjectTemplate template;
    template.unk0 = pos[0];
    template.unk2 = pos[1];
    template.unk4 = id;
    template.unk6 = action;
    template.unk8 = direction;
    template.unkA = spriteId;

    person->object = Field2DObjectManager_AddObject(room->objects, &template);
}

s32 WifiClubRoomPerson_CanTalk(WifiClubRoomPerson *person) {
    switch (person->mode) {
    case 0:
    case 3:
    case 4:
        return 1;
    case 1:
    case 2:
    case 5:
        return 0;
    default:
        return 0;
    }
}

void WifiClubRoom_SetPersonModePlayer(WifiClubRoom *room, WifiClubRoomPerson *person) {
    person->update = WifiClubRoomPerson_UpdatePlayer;
    person->animate = WifiClubRoomPerson_AnimateNone;
    person->mode = 0;
    WifiClubRoom_QueuePersonCommand(room, 0, person);
}

void WifiClubRoom_SetPersonModeWarpIn(WifiClubRoom *room, WifiClubRoomPerson *person) {
    person->update = WifiClubRoomPerson_UpdateWarpIn;
    person->animate = WifiClubRoomPerson_AnimateWarp;
    person->timer = 45;
    person->mode = 1;
}

void WifiClubRoom_SetPersonModeWander(WifiClubRoom *room, WifiClubRoomPerson *person) {
    person->update = WifiClubRoomPerson_UpdateWander;
    person->animate = WifiClubRoomPerson_AnimateNone;
    person->timer = (1023 & MTRandom()) + 256;
    person->mode = 3;
    WifiClubRoom_QueuePersonCommand(room, 0, person);
    Field2DObjectSprite_SetAutoUpdate(person->drawer, 1);
}

void WifiClubRoom_SetPersonModeBusy(WifiClubRoom *room, WifiClubRoomPerson *person) {
    person->update = WifiClubRoomPerson_UpdateBusy;
    person->animate = WifiClubRoomPerson_AnimateHop;
    person->timer = 28;
    person->mode = 4;
    WifiClubRoom_QueuePersonCommand(room, 4, person);
    Field2DObjectSprite_SetAutoUpdate(person->drawer, 0);
}

void WifiClubRoomPerson_AnimateNone(WifiClubRoom *room, WifiClubRoomPerson *person) {
}

void WifiClubRoomPerson_AnimateWarp(WifiClubRoom *room, WifiClubRoomPerson *person) {
    Field2DObjectSprite_UpdateAnim(person->drawer);
    if (Field2DObjectSprite_IsVisible(person->drawer) == 0) {
        Field2DObjectSprite_SetVisible(person->drawer, 1);
    }
    Field2DPoint pos = { Field2DObject_GetParam(person->object, 0), Field2DObject_GetParam(person->object, 1) };
    Field2DPoint shadowPos = pos;

    s64 offset = (s64)GF_SinDegNoWrap(person->timer) * (signed)0xFFF40000;
    offset += 2048;

    s32 r1 = (offset >> 12);
    pos.unk2 = (s16)pos.unk2 + (r1 >> 12);

    Field2DObjectSprite_SetPosition(person->drawer, pos);
    Field2DObjectSprite_SetShadowPosition(person->drawer, shadowPos);
}

void WifiClubRoomPerson_AnimateHop(WifiClubRoom *room, WifiClubRoomPerson *person) {
    Field2DPoint pos = { Field2DObject_GetParam(person->object, 0), Field2DObject_GetParam(person->object, 1) };
    Field2DPoint shadowPos = pos;

    s16 hopTimer = person->timer - 16;
    if (hopTimer > 0) {
        s32 temp_r0_2 = (s16)(hopTimer % 6) * 180;
        s32 sine = GF_SinDegNoWrap(temp_r0_2 / 6);

        s64 r6 = ((s64)sine << 14) + 2048;
        pos.unk2 = (s16)pos.unk2 - ((s32)(r6 >> 12) >> 12);
    }
    Field2DObjectSprite_SetPosition(person->drawer, pos);
    Field2DObjectSprite_SetShadowPosition(person->drawer, shadowPos);

    Field2DObjectSprite_FaceDirection(person->drawer, 1);
}

s32 WifiClubRoomPerson_UpdatePlayer(WifiClubRoom *room, WifiClubRoomPerson *person) {
    u32 direction = Field2DObject_GetParam(person->object, 6);
    u32 id = Field2DObject_GetParam(person->object, 4);
    if (Field2DObject_GetParam(person->object, 5) != 0) {
        return 0;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        WifiClubRoomPerson *partner = WifiClubRoom_GetPersonInDirection(room, person, direction);
        if (partner != NULL) {
            if (WifiClubRoomPerson_CanTalk(partner) == 1) {
                person->talkPartnerId = Field2DObject_GetParam(partner->object, 4);
            } else {
                person->talkPartnerId = 0;
            }
        } else {
            person->talkPartnerId = 0;
        }
    }
    if (room->inputLocked == 1) {
        return 0;
    }
    s16 moveAction = 2;
    if (gSystem.heldKeys & PAD_BUTTON_B) {
        moveAction = 3;
    }
    if (gSystem.heldKeys & PAD_KEY_UP) {
        if (direction == 0) {
            WifiClubRoom_QueueMoveCommand(room, moveAction, direction, id);
        } else {
            WifiClubRoom_QueueMoveCommand(room, 1, 0, id);
        }
    } else if (gSystem.heldKeys & PAD_KEY_DOWN) {
        if (direction == 1) {
            WifiClubRoom_QueueMoveCommand(room, moveAction, direction, id);
        } else {
            WifiClubRoom_QueueMoveCommand(room, 1, 1, id);
        }
    } else if (gSystem.heldKeys & PAD_KEY_LEFT) {
        if (direction == 2) {
            WifiClubRoom_QueueMoveCommand(room, moveAction, direction, id);
        } else {
            WifiClubRoom_QueueMoveCommand(room, 1, 2, id);
        }
    } else if (gSystem.heldKeys & PAD_KEY_RIGHT) {
        if (direction == 3) {
            WifiClubRoom_QueueMoveCommand(room, moveAction, direction, id);
        } else {
            WifiClubRoom_QueueMoveCommand(room, 1, 3, id);
        }
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateWarpIn(WifiClubRoom *room, WifiClubRoomPerson *person) {
    person->timer--;
    if (person->timer <= 0) {
        Field2DObjectSprite_SyncAnim(person->drawer);
        Field2DObjectSprite_SetAutoUpdate(person->drawer, 1);
        u32 playerId = Field2DObject_GetParam(room->player->object, 4);
        u32 id = Field2DObject_GetParam(person->object, 4);
        if (id == playerId) {
            WifiClubRoom_SetPersonModePlayer(room, person);
        } else {
            WifiClubRoom_SetPersonActivityMode(room, person, person->activityMode);
        }
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateWarpOut(WifiClubRoom *room, WifiClubRoomPerson *person) {
    person->timer++;
    if (person->timer > 45) {
        return 1;
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateWander(WifiClubRoom *room, WifiClubRoomPerson *person) {
    person->timer--;
    if (person->timer <= 0) {
        person->timer = (1023 & MTRandom()) + 256;
        u32 direction = MTRandom() & 3;
        WifiClubRoom_QueueMoveCommand(room, 0, direction, Field2DObject_GetParam(person->object, 4));
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateBusy(WifiClubRoom *room, WifiClubRoomPerson *person) {
    person->timer--;
    if (person->timer < 0) {
        person->timer = 28;
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateTalking(WifiClubRoom *room, WifiClubRoomPerson *person) {
    return 0;
}

void WifiClubRoom_QueueMoveCommand(WifiClubRoom *room, s16 action, u32 direction, u32 objectId) {
    Field2DCommand command;
    command.unk0 = action;
    command.unk2 = direction;
    command.unk3 = objectId;

    Field2DCommandQueue_Push(room->commandQueue, &command);
}

void WifiClubRoom_QueuePersonCommand(WifiClubRoom *room, s16 action, WifiClubRoomPerson *person) {
    u32 direction = Field2DObject_GetParam(person->object, 6);
    WifiClubRoom_QueueMoveCommand(room, action, direction, Field2DObject_GetParam(person->object, 4));
}

s32 WifiClubRoomPerson_IsNotWarping(WifiClubRoomPerson *person) {
    if (person->animate != WifiClubRoomPerson_AnimateWarp) {
        return 1;
    }
    return 0;
}

void WifiClubRoom_SetPersonActivityMode(WifiClubRoom *room, WifiClubRoomPerson *person, u8 activityMode) {
    switch (activityMode) {
    case 0:
        WifiClubRoom_SetPersonModeWander(room, person);
        break;
    case 1:
        WifiClubRoom_SetPersonModeBusy(room, person);
        break;
    default:
        GF_AssertFail();
        break;
    }
    person->activityMode = activityMode;
}

void WifiClubRoomLights_Init(WifiClubRoom *room, WifiClubRoomLights *lights, NARC *narc) {
    memset(lights, 0, sizeof(WifiClubRoomLights));
    lights->plttDataRaw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 32, &lights->plttData, room->heapID);
    lights->blinkMask = 15;
    lights->blinkPhase = 0;
    lights->blinkTimer = sWifiClubRoomLightBlinkFrames[lights->blinkPhase];
}

void WifiClubRoomLights_Free(WifiClubRoom *room, WifiClubRoomLights *lights) {
    Heap_Free(lights->plttDataRaw);
    memset(lights, 0, sizeof(WifiClubRoomLights));
}

void WifiClubRoomLights_Update(WifiClubRoom *room, WifiClubRoomLights *lights) {
    WifiClubRoomLights_UpdateBlink(lights);
    WifiClubRoomLights_UpdateSequence(lights);
}

void WifiClubRoomLights_UpdateBlink(WifiClubRoomLights *lights) {
    s32 colorIndex;

    lights->blinkTimer--;
    if (lights->blinkTimer <= 0) {
        lights->blinkPhase = (lights->blinkPhase + 1) % 2;
        lights->blinkTimer = sWifiClubRoomLightBlinkFrames[lights->blinkPhase];
        if (lights->blinkPhase == 1) {
            colorIndex = 2;
        } else {
            colorIndex = 1;
        }
        for (s32 i = 0; i < 4; i++) {
            if (lights->blinkMask & (1 << i)) {
                GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, ((i + 1) * 2) + 224, WifiClubRoomLights_GetColor(lights, colorIndex), 2);
            }
        }
    }
}

void WifiClubRoomLights_UpdateSequence(WifiClubRoomLights *lights) {
    u32 setColor = 0;
    u32 colorIndex = 0;
    u32 advance = 0;
    if (sWifiClubRoomLightSequence[lights->step].timed == 1) {
        lights->stepTimer--;
        if (lights->stepTimer <= 0) {
            advance = 1;
        }
    } else if (lights->triggered != 0) {
        lights->triggered = 0;
        advance = 1;
    }
    if (advance != 0) {
        lights->step++;
        if (lights->step >= 8) {
            WifiClubRoomLights_EndSequence(lights);
            return;
        }
        lights->stepTimer = sWifiClubRoomLightSequence[lights->step].frames;
        setColor = sWifiClubRoomLightSequence[lights->step].setColor;
        colorIndex = sWifiClubRoomLightSequence[lights->step].color;
    }
    if (setColor != 0) {
        GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, ((lights->activeLight + 1) * 2) + 224, WifiClubRoomLights_GetColor(lights, colorIndex), 2);
    }
}

void *WifiClubRoomLights_GetColor(WifiClubRoomLights *lights, s32 colorIndex) {
    return lights->plttData->pRawData + (colorIndex + 1) * 2;
}

void WifiClubRoomLights_EndSequence(WifiClubRoomLights *lights) {
    lights->blinkMask = (lights->blinkMask | (1 << lights->activeLight));
    lights->activeLight = 0;
    lights->step = 0;
    lights->stepTimer = 0;
    lights->triggered = 0;
}

void WifiClubRoomLights_Trigger(WifiClubRoomLights *lights) {
    lights->triggered = 1;
}

void WifiClubRoomLights_Start(WifiClubRoomLights *lights, u8 light) {
    if (lights->step != 0) {
        WifiClubRoomLights_EndSequence(lights);
    }
    lights->blinkMask = lights->blinkMask ^ (1 << light);
    lights->activeLight = light;
    WifiClubRoomLights_Trigger(lights);
}

void WifiClubRoomLights_Continue(WifiClubRoomLights *lights) {
    if (lights->step != 0) {
        WifiClubRoomLights_Trigger(lights);
    }
}
