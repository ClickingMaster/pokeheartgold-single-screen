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

YesNoPrompt *WifiClub_CreateYesNoPromptIgnoreTouch(BgConfig *arg0, const WindowTemplate *arg1, u32 arg2) {
    YesNoPrompt *yesNoPrompt = WifiClub_CreateYesNoPrompt(arg0, arg1, arg2);
    YesNoPrompt_SetIgnoreTouch(yesNoPrompt, 1);
    return yesNoPrompt;
}

YesNoPrompt *WifiClub_CreateYesNoPrompt(BgConfig *arg0, const WindowTemplate *arg1, u32 arg2) {
    YesNoPrompt *yesNoPrompt = YesNoPrompt_Create(HEAP_ID_53);
    YesNoPromptTemplate yesNoPromptTemplate;
    yesNoPromptTemplate.bgConfig = arg0;
    yesNoPromptTemplate.bgId = arg1->bgId;
    yesNoPromptTemplate.tileStart = arg1->baseTile;
    yesNoPromptTemplate.plttSlot = arg1->palette;
    yesNoPromptTemplate.x = arg1->left;
    yesNoPromptTemplate.y = arg1->top;
    yesNoPromptTemplate.ignoreTouchFlag = 0;
    yesNoPromptTemplate.initialCursorPos = arg2;
    yesNoPromptTemplate.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(yesNoPrompt, &yesNoPromptTemplate);
    return yesNoPrompt;
}

void WifiClubRoom_Init(WifiClubRoom *arg0, enum HeapID arg1, NARC *arg2, BgConfig *arg3, s32 arg4, u32 arg5) {
    memset(arg0, 0, sizeof(WifiClubRoom));
    arg0->heapID = arg1;
    u32 temp_r7 = (arg5 - 1) / 8;
    arg0->bgConfig = arg3;
    WifiClubRoomGfx_Init(&arg0->gfx, arg0->heapID, arg2);
    WifiClubRoom_InitBgs(arg0->bgConfig, arg1);

    Field2DPoint sp4 = ov44_02232E90(temp_r7);
    arg0->map = Field2DMap_New(sp4.unk0, sp4.unk2, arg1);
    Field2DMap_Load(arg0->map, ov44_02232E9C(temp_r7));
    arg0->objects = Field2DObjectManager_New(0x30, arg1);
    WifiClubRoom_CreateDrawers(arg0, arg4, arg1);
    Field2DCamera_Init(&arg0->cameraPos);
    WifiClubRoom_CreateMapBg(arg0, arg1, arg2, temp_r7);
    WifiClubRoom_LoadBgGfx(arg0->bgConfig, arg1, arg2);
    WifiClubRoomLights_Init(arg0, &arg0->lights, arg2);
    arg0->commandQueue = Field2DCommandQueue_New(0x80, arg1);
    arg0->initialized = 1;
}

void WifiClubRoom_Free(WifiClubRoom *arg0) {
    Field2DCommandQueue_Free(arg0->commandQueue);
    WifiClubRoom_DeleteMapBg(arg0);
    WifiClubRoomLights_Free(arg0, &arg0->lights);
    WifiClubRoom_DeleteAllPersons(arg0);
    WifiClubRoom_DeleteDrawers(arg0);
    Field2DObjectManager_Free(arg0->objects);
    Field2DMap_Free(arg0->map);
    WifiClubRoomGfx_Free(&arg0->gfx);
    WifiClubRoom_FreeBgs(arg0->bgConfig);
    memset(arg0, 0, sizeof(WifiClubRoom));
}

s32 WifiClubRoom_IsInitialized(WifiClubRoom *arg0) {
    return arg0->initialized;
}

s32 WifiClubRoom_Update(WifiClubRoom *arg0) {
    Field2DObjectManager_Update(arg0->objects);
    WifiClubRoom_UpdatePersons(arg0);
    WifiClubRoom_ProcessMoveCommands(arg0);
    Field2DCamera_CenterOnObject(&arg0->cameraPos, arg0->player->object);
    Field2DBgLayer_SetCamera(arg0->mapBg, &arg0->cameraPos);
    Field2DObjectSpriteManager_Update(arg0->drawers);
    WifiClubRoom_AnimatePersons(arg0);
    WifiClubRoom_HideExitArrow(arg0);
    if (WifiClubRoom_IsPlayerIdle(arg0) == 1) {
        if (WifiClubRoom_IsPlayerAtEntrance(arg0) == 1) {
            WifiClubRoom_ShowExitArrow(arg0);
            if (gSystem.heldKeys & PAD_KEY_DOWN) {
                return 1;
            }
        }
        if (gSystem.newKeys & PAD_BUTTON_A) {
            if (WifiClubRoom_GetTalkPartnerId(arg0) != 0) {
                return 2;
            }
            if (WifiClubRoom_IsPlayerFacingCounter(arg0) == 1) {
                return 3;
            }
        }
    }
    return 0;
}

void WifiClubRoom_Draw(WifiClubRoom *arg0) {
    if (arg0->initialized != 0) {
        SpriteList_RenderAndAnimateSprites(arg0->gfx.spriteList);
    }
}

u8 WifiClubRoom_GetTalkPartnerId(WifiClubRoom *arg0) {
    GF_ASSERT(arg0->player != NULL);
    return arg0->player->talkPartnerId;
}

s32 WifiClubRoom_GetTileUnderPlayer(WifiClubRoom *arg0) {
    GF_ASSERT(arg0->player != NULL);
    Field2DPoint temp_r0 = Field2DObject_GetPos(arg0->player->object);
    return Field2DMap_GetTag(arg0->map, temp_r0.unk0 / 16, temp_r0.unk2 / 16);
}

s32 WifiClubRoom_GetTileBelowPlayer(WifiClubRoom *arg0) {
    GF_ASSERT(arg0->player != NULL);
    Field2DPoint sp0 = Field2DObject_GetPos(arg0->player->object);
    return Field2DMap_GetTag(arg0->map, sp0.unk0 / 16, (sp0.unk2 / 16) + 1);
}

void WifiClubRoom_ShowFriendMarker(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    Field2DPoint sp0 = Field2DObject_GetPos(arg1->object);
    WifiClubRoom_ShowFriendMarkerAt(arg0, sp0, Field2DObjectSprite_GetDrawPriority(arg1->drawer) - 2);
}

void WifiClubRoom_HideFriendMarker(WifiClubRoom *arg0) {
    WifiClubRoom_HideFriendMarkerSprite(arg0);
}

void WifiClubRoom_SetInputLocked(WifiClubRoom *arg0, s32 arg1) {
    arg0->inputLocked = arg1;
}

s32 WifiClubRoom_IsInputLocked(WifiClubRoom *arg0) {
    return arg0->inputLocked;
}

WifiClubRoomPerson *WifiClubRoom_AddPlayer(WifiClubRoom *arg0, s16 arg1) {
    s16 spC;
    WifiClubRoomPerson *temp_r0 = WifiClubRoom_AllocPerson(arg0);
    arg0->player = temp_r0;
    WifiClubRoom_FindEntrancePos(arg0, &spC);
    WifiClubRoom_CreatePersonObject(arg0, temp_r0, &spC, 0, arg1, 0, 0);
    WifiClubRoom_SetPersonModePlayer(arg0, temp_r0);
    WifiClubRoom_CreatePersonDrawer(arg0, temp_r0, 1);
    Field2DCamera_CenterOnObject(&arg0->cameraPos, arg0->player->object);
    Field2DBgLayer_SetCamera(arg0->mapBg, &arg0->cameraPos);
    return temp_r0;
}

WifiClubRoomPerson *WifiClubRoom_AddFriend(WifiClubRoom *arg0, s16 arg1, s16 arg2) {
    s16 spC[2];
    WifiClubRoomPerson *temp_r4 = WifiClubRoom_AllocPerson(arg0);
    WifiClubRoom_FindFriendSpawnPos(arg0, &spC[0], arg2 - 1);
    if (arg0->player != NULL) {
        Field2DPoint sp0 = Field2DObject_GetPos(arg0->player->object);
        if ((sp0.unk0 == spC[0]) && (sp0.unk2 == spC[1])) {
            return NULL;
        }
    }
    WifiClubRoom_CreatePersonObject(arg0, temp_r4, &spC[0], arg2, arg1, 1, 4);
    WifiClubRoom_SetPersonModeWarpIn(arg0, temp_r4);
    WifiClubRoom_CreatePersonDrawer(arg0, temp_r4, 0);
    return temp_r4;
}

void WifiClubRoom_StartPersonLeave(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    Field2DObject_GetParam(arg1->object, 6);
    arg1->update = WifiClubRoomPerson_UpdateWarpOut;
    arg1->animate = WifiClubRoomPerson_AnimateWarp;
    arg1->mode = 2;
    arg1->timer = 0;
    Field2DObjectSprite_SetAutoUpdate(arg1->drawer, 0);
    Field2DObjectSprite_StartLoopAnim(arg1->drawer);
    WifiClubRoom_QueuePersonCommand(arg0, 4, arg1);
}

void WifiClubRoomPerson_Delete(WifiClubRoomPerson *arg0) {
    Field2DObjectSprite_Delete(arg0->drawer);
    Field2DObject_Delete(arg0->object);
    memset(arg0, 0, sizeof(WifiClubRoomPerson));
}

u8 WifiClubRoomPerson_GetId(WifiClubRoomPerson *arg0) {
    return Field2DObject_GetParam(arg0->object, 4);
}

s32 WifiClubRoomPerson_GetOppositeDirection(WifiClubRoomPerson *arg0) {
    return Field2D_GetOppositeDirection(Field2DObject_GetParam(arg0->object, 6));
}

void WifiClubRoom_StartTalk(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s32 arg2) {
    arg1->savedUpdate = arg1->update;
    arg1->update = WifiClubRoomPerson_UpdateTalking;
    arg1->savedMode = arg1->mode;
    arg1->mode = 5;
    WifiClubRoom_QueueMoveCommand(arg0, 0, arg2, Field2DObject_GetParam(arg1->object, 4));
    Field2DObjectSprite_SetAutoUpdate(arg1->drawer, 0);
    Field2DObjectSprite_FaceDirection(arg1->drawer, arg2);

    Field2DPoint temp_r3 = { Field2DObject_GetParam(arg1->object, 0), Field2DObject_GetParam(arg1->object, 1) };
    Field2DObjectSprite_SetPosition(arg1->drawer, temp_r3);
}

void WifiClubRoom_EndTalk(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    if (arg1 != NULL) {
        if (arg1->savedUpdate != 0) {
            arg1->update = arg1->savedUpdate;
            arg1->savedUpdate = 0;
            arg1->mode = arg1->savedMode;
            Field2DObjectSprite_SetAutoUpdate(arg1->drawer, 1);
        }
    }
}

void WifiClubRoom_SetPersonBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u8 arg2) {
    if (WifiClubRoomPerson_IsNotWarping(arg1) == 0) {
        arg1->activityMode = arg2;
        return;
    }
    WifiClubRoom_SetPersonActivityMode(arg0, arg1, arg2);
}

void WifiClubRoom_StartCounterLight(WifiClubRoom *arg0) {
    if (WifiClubRoom_IsPlayerFacingCounter(arg0) == 1) {
        s8 temp_r4 = (s8)WifiClubRoom_GetTileBelowPlayer(arg0) - 3;
        GF_ASSERT(temp_r4 >= 0 && temp_r4 < 4);
        WifiClubRoomLights_Start(&arg0->lights, temp_r4);
        return;
    }
    GF_AssertFail();
}

void WifiClubRoom_ContinueLightSequence(WifiClubRoom *arg0) {
    WifiClubRoomLights_Continue(&arg0->lights);
}

void WifiClubRoom_UpdateLights(WifiClubRoom *arg0) {
    WifiClubRoomLights_Update(arg0, &arg0->lights);
}

void WifiClubRoom_DeleteAllPersons(WifiClubRoom *arg0) {
    for (s32 i = 0; i < 48; i++) {
        if (arg0->persons[i].object != 0) {
            WifiClubRoomPerson_Delete(&arg0->persons[i]);
        }
    }
}

s32 WifiClubRoom_IsPlayerIdle(WifiClubRoom *arg0) {
    if (Field2DObject_GetParam(arg0->player->object, 5) == 0) {
        return 1;
    }
    return 0;
}

s32 WifiClubRoom_IsPlayerFacingCounter(WifiClubRoom *arg0) {
    Field2DPoint sp0 = Field2DObject_GetPos(arg0->player->object);
    u32 temp_r4 = Field2DObject_GetParam(arg0->player->object, 6);
    sp0 = Field2D_StepInDirection(sp0, temp_r4);
    s32 temp_r0_3 = Field2DMap_GetTag(arg0->map, sp0.unk0 / 16, sp0.unk2 / 16);
    if ((temp_r4 == 0) && (temp_r0_3 == 2)) {
        return 1;
    }
    return 0;
}

void WifiClubRoom_ProcessMoveCommands(WifiClubRoom *arg0) {
    Field2DCommand sp8;
    Field2DMove sp0;
    if (Field2DCommandQueue_Pop(arg0->commandQueue, &sp8) == 1) {
        do {
            if (Field2DObjectManager_TryCommand(arg0->map, arg0->objects, &sp8, &sp0) == 1) {
                Field2DObjectManager_StartMove(arg0->objects, &sp0);
            }
        } while (Field2DCommandQueue_Pop(arg0->commandQueue, &sp8) == 1);
    }
}

void WifiClubRoom_UpdatePersons(WifiClubRoom *arg0) {
    for (s32 i = 0; i < 48; i++) {
        if ((arg0->persons[i].object != 0) && (arg0->persons[i].update(arg0, &arg0->persons[i]) == 1)) {
            WifiClubRoomPerson_Delete(&arg0->persons[i]);
        }
    }
}

void WifiClubRoom_AnimatePersons(WifiClubRoom *arg0) {
    for (s32 i = 0; i < 48; i++) {
        if (arg0->persons[i].object != 0) {
            arg0->persons[i].animate(arg0, &arg0->persons[i]);
        }
    }
}

WifiClubRoomPerson *WifiClubRoom_GetPersonInDirection(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u32 arg2) {
    Field2DObject *temp_r0 = Field2DObject_FindObjectInDirection(arg1->object, arg0->objects, arg2);
    if (temp_r0 != NULL) {
        for (s32 i = 0; i < 48; i++) {
            Field2DObject *temp_r3 = arg0->persons[i].object;
            if (temp_r3 == temp_r0) {
                return &arg0->persons[i];
            }
        }
    }
    return NULL;
}

void WifiClubRoom_CreateDrawers(WifiClubRoom *arg0, s32 arg1, enum HeapID arg2) {
    arg0->drawers = Field2DObjectSpriteManager_NewWithShadow(arg0->gfx.spriteList, NULL, 48, arg1, 0, 1, arg2);
    Field2DObjectSpriteManager_LoadAvatarChars(arg0->drawers, 2, arg2);
}

void WifiClubRoom_DeleteDrawers(WifiClubRoom *arg0) {
    Field2DObjectSpriteManager_Free(arg0->drawers);
}

void WifiClubRoom_CreateMapBg(WifiClubRoom *arg0, enum HeapID arg1, NARC *arg2, s32 arg3) {
    Field2DBgLayerTemplate sp0 = {
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

    sp0.fileId += arg3;
    arg0->mapBg = Field2DBgLayer_New(&arg0->gfx.g2dRenderer, arg0->bgConfig, &sp0, arg1);
}

void WifiClubRoom_DeleteMapBg(WifiClubRoom *arg0) {
    Field2DBgLayer_Free(arg0->mapBg);
}

void WifiClubRoomGfx_Init(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc) {
    arg0->spriteList = G2dRenderer_Init(96, &arg0->g2dRenderer, heapID);
    G2dRenderer_SetSubSurfaceCoords(&arg0->g2dRenderer, 0, 0x320000);
    for (s32 i = 0; i < 4; i++) {
        arg0->resManagers[i] = Create2DGfxResObjMan(16, (GfGfxResType)i, heapID);
    }
    WifiClubRoomGfx_LoadResources(arg0, heapID, narc);
    WifiClubRoomGfx_CreateSprites(arg0, heapID);
}

void WifiClubRoomGfx_Free(WifiClubRoomGfx *arg0) {
    WifiClubRoomGfx_DeleteSprites(arg0);
    WifiClubRoomGfx_FreeResources(arg0);
    for (s32 i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(arg0->resManagers[i]);
    }
    SpriteList_Delete(arg0->spriteList);
}

void WifiClubRoomGfx_LoadResources(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc) {
    WifiClubRoomGfx_LoadSpriteResources(arg0, heapID, narc);
}

void WifiClubRoomGfx_FreeResources(WifiClubRoomGfx *arg0) {
    WifiClubRoomGfx_FreeSpriteResources(arg0);
}

void WifiClubRoomGfx_CreateSprites(WifiClubRoomGfx *arg1, enum HeapID heapID) {
    WifiClubRoomGfx_CreateMarkerSprites(arg1, heapID);
}

void WifiClubRoomGfx_DeleteSprites(WifiClubRoomGfx *arg0) {
    WifiClubRoomGfx_DeleteMarkerSprites(arg0);
}

void WifiClubRoom_InitBgs(BgConfig *arg0, enum HeapID heapID) {
}

void WifiClubRoom_FreeBgs(BgConfig *arg0) {
}

void WifiClubRoom_LoadBgGfx(BgConfig *bgConfig, enum HeapID heapID, NARC *narc) {
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 31, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 256, heapID);
    BG_SetMaskColor(0, 0);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 30, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, heapID);
}

void WifiClubRoomGfx_LoadSpriteResources(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc) {
    arg0->resources[0] = AddCharResObjFromOpenNarc(arg0->resManagers[0], narc, NARC_a_0_3_8, FALSE, 50, NNS_G2D_VRAM_TYPE_2DMAIN, heapID);
    arg0->resources[1] = AddPlttResObjFromOpenNarc(arg0->resManagers[1], narc, NARC_fielddata_encountdata_g_enc_data, FALSE, 50, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapID);
    arg0->resources[2] = AddCellOrAnimResObjFromOpenNarc(arg0->resManagers[2], narc, NARC_a_0_3_9, FALSE, 50, GF_GFX_RES_TYPE_CELL, heapID);
    arg0->resources[3] = AddCellOrAnimResObjFromOpenNarc(arg0->resManagers[3], narc, NARC_a_0_4_0, FALSE, 50, GF_GFX_RES_TYPE_ANIM, heapID);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(arg0->resources[0]) == TRUE);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(arg0->resources[1]) == TRUE);
    GF2DGfxResObj_FreeData(arg0->resources[0]);
    GF2DGfxResObj_FreeData(arg0->resources[1]);
    CreateSpriteResourcesHeader(&arg0->resourcesHeader, 50, 50, 50, 50, -1, -1, 0, 2, arg0->resManagers[0], arg0->resManagers[1], arg0->resManagers[2], arg0->resManagers[3], 0, 0);
}

void WifiClubRoomGfx_FreeSpriteResources(WifiClubRoomGfx *arg0) {
    SpriteTransfer_DeleteCharTransferTask(arg0->resources[0]);
    SpriteTransfer_DeletePlttTransferTask(arg0->resources[1]);
    for (s32 i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(arg0->resManagers[i], arg0->resources[i]);
    }
}

void WifiClubRoomGfx_CreateMarkerSprites(WifiClubRoomGfx *arg0, enum HeapID heapID) {
    SpriteTemplate spriteTemplate = { };
    spriteTemplate.spriteList = arg0->spriteList;
    spriteTemplate.header = &arg0->resourcesHeader;

    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;

    spriteTemplate.scale.x = 4096;
    spriteTemplate.scale.y = 4096;
    spriteTemplate.heapID = heapID;

    spriteTemplate.drawPriority = 0;

    arg0->exitArrowSprite = Sprite_CreateAffine(&spriteTemplate);
    arg0->friendMarkerSprite = Sprite_CreateAffine(&spriteTemplate);
    Sprite_SetDrawFlag(arg0->exitArrowSprite, 0);
    Sprite_SetDrawFlag(arg0->friendMarkerSprite, 0);
    Sprite_SetAnimActiveFlag(arg0->exitArrowSprite, 1);
    Sprite_SetAnimCtrlSeq(arg0->friendMarkerSprite, 1);
}

void WifiClubRoomGfx_DeleteMarkerSprites(WifiClubRoomGfx *arg0) {
    Sprite_Delete(arg0->exitArrowSprite);
    Sprite_Delete(arg0->friendMarkerSprite);
}

void WifiClubRoom_ShowExitArrow(WifiClubRoom *arg0) {
    VecFx32 vec;
    s16 sp0[2];

    WifiClubRoom_FindEntrancePos(arg0, sp0);
    sp0[0] = sp0[0] + 8;
    sp0[1] = sp0[1] + 32;
    vec.x = sp0[0] << 12;
    vec.y = sp0[1] << 12;
    Sprite_SetMatrix(arg0->gfx.exitArrowSprite, &vec);
    Sprite_SetDrawFlag(arg0->gfx.exitArrowSprite, 1);
}

void WifiClubRoom_HideExitArrow(WifiClubRoom *arg0) {
    Sprite_SetDrawFlag(arg0->gfx.exitArrowSprite, 0);
}

void WifiClubRoom_ShowFriendMarkerAt(WifiClubRoom *arg0, Field2DPoint arg1, s32 arg2) {
    VecFx32 vec;
    vec.x = (s16)(arg1.unk0 + 8) * FX32_ONE;
    vec.y = arg1.unk2 * FX32_ONE;
    Sprite_SetMatrix(arg0->gfx.friendMarkerSprite, &vec);
    Sprite_SetDrawPriority(arg0->gfx.friendMarkerSprite, arg2);
    Sprite_SetDrawFlag(arg0->gfx.friendMarkerSprite, 1);
}

void WifiClubRoom_HideFriendMarkerSprite(WifiClubRoom *arg0) {
    Sprite_SetDrawFlag(arg0->gfx.friendMarkerSprite, 0);
}

s32 WifiClubRoom_IsPlayerAtEntrance(WifiClubRoom *arg0) {
    if (arg0->player == 0) {
        return 0;
    }
    Field2DPoint sp0 = Field2DObject_GetPos(arg0->player->object);
    u32 temp_r4 = Field2DObject_GetParam(arg0->player->object, 6);
    if (Field2DMap_GetTag(arg0->map, sp0.unk0 / 16, sp0.unk2 / 16) == 0xF && temp_r4 == 1) {
        return 1;
    }
    return 0;
}

void WifiClubRoom_FindFriendSpawnPos(WifiClubRoom *arg0, s16 *arg1, u32 arg2) {
    u32 r7 = arg2 / 8;
    u32 r4 = arg2 % 8;
    u16 r6 = Field2DMap_GetWidth(arg0->map);
    u16 sp4 = Field2DMap_GetHeight(arg0->map);

    for (u32 i = 0; i < sp4; i++) {
        for (u32 j = 0; j < r6; j++) {
            s32 temp_r0 = Field2DMap_GetTag(arg0->map, j, i);
            if (temp_r0 == r4 + 7) {
                temp_r0 = Field2DMap_GetTag(arg0->map, j, i + 1);
                if (temp_r0 == r7 + 3) {
                    arg1[0] = j << 4;
                    arg1[1] = i << 4;
                    return;
                }
            }
        }
    }
    GF_AssertFail();
}

void WifiClubRoom_FindEntrancePos(WifiClubRoom *arg0, s16 *arg1) {
    u16 temp_r5 = Field2DMap_GetWidth(arg0->map);
    u16 temp_r0 = Field2DMap_GetHeight(arg0->map);

    for (u32 i = 0; i < temp_r0; i++) {
        for (u32 j = 0; j < temp_r5; j++) {
            if (Field2DMap_GetTag(arg0->map, j, i) == 15) {
                arg1[0] = j << 4;
                arg1[1] = i << 4;
                return;
            }
        }
    }
    GF_AssertFail();
}

WifiClubRoomPerson *WifiClubRoom_AllocPerson(WifiClubRoom *arg0) {
    for (s32 i = 0; i < 48; i++) {
        if (arg0->persons[i].object == 0) {
            return &arg0->persons[i];
        }
    }
    GF_AssertFail();
    return NULL;
}

void WifiClubRoom_CreatePersonDrawer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s32 arg2) {
    arg1->drawer = Field2DObjectSpriteManager_CreateSprite(arg0->drawers, arg1->object, arg2, arg0->heapID);
    if (arg2 == 0) {
        Field2DObjectSprite_SetAutoUpdate(arg1->drawer, 0);
        Field2DObjectSprite_StartLoopAnim(arg1->drawer);
        Field2DObjectSprite_SetVisible(arg1->drawer, 0);
    }
}

void WifiClubRoom_CreatePersonObject(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s16 *arg2, s16 arg3, u16 arg4, u8 arg5, s32 arg6) {
    Field2DObjectTemplate sp;
    sp.unk0 = arg2[0];
    sp.unk2 = arg2[1];
    sp.unk4 = arg3;
    sp.unk6 = arg6;
    sp.unk8 = arg5;
    sp.unkA = arg4;

    arg1->object = Field2DObjectManager_AddObject(arg0->objects, &sp);
}

s32 WifiClubRoomPerson_CanTalk(WifiClubRoomPerson *arg0) {
    switch (arg0->mode) {
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

void WifiClubRoom_SetPersonModePlayer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->update = WifiClubRoomPerson_UpdatePlayer;
    arg1->animate = WifiClubRoomPerson_AnimateNone;
    arg1->mode = 0;
    WifiClubRoom_QueuePersonCommand(arg0, 0, arg1);
}

void WifiClubRoom_SetPersonModeWarpIn(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->update = WifiClubRoomPerson_UpdateWarpIn;
    arg1->animate = WifiClubRoomPerson_AnimateWarp;
    arg1->timer = 45;
    arg1->mode = 1;
}

void WifiClubRoom_SetPersonModeWander(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->update = WifiClubRoomPerson_UpdateWander;
    arg1->animate = WifiClubRoomPerson_AnimateNone;
    arg1->timer = (1023 & MTRandom()) + 256;
    arg1->mode = 3;
    WifiClubRoom_QueuePersonCommand(arg0, 0, arg1);
    Field2DObjectSprite_SetAutoUpdate(arg1->drawer, 1);
}

void WifiClubRoom_SetPersonModeBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->update = WifiClubRoomPerson_UpdateBusy;
    arg1->animate = WifiClubRoomPerson_AnimateHop;
    arg1->timer = 28;
    arg1->mode = 4;
    WifiClubRoom_QueuePersonCommand(arg0, 4, arg1);
    Field2DObjectSprite_SetAutoUpdate(arg1->drawer, 0);
}

void WifiClubRoomPerson_AnimateNone(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
}

void WifiClubRoomPerson_AnimateWarp(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    Field2DObjectSprite_UpdateAnim(arg1->drawer);
    if (Field2DObjectSprite_IsVisible(arg1->drawer) == 0) {
        Field2DObjectSprite_SetVisible(arg1->drawer, 1);
    }
    Field2DPoint sp0 = { Field2DObject_GetParam(arg1->object, 0), Field2DObject_GetParam(arg1->object, 1) };
    Field2DPoint sp4 = sp0;

    s64 temp_ret = (s64)GF_SinDegNoWrap(arg1->timer) * (signed)0xFFF40000;
    temp_ret += 2048;

    s32 r1 = (temp_ret >> 12);
    sp0.unk2 = (s16)sp0.unk2 + (r1 >> 12);

    Field2DObjectSprite_SetPosition(arg1->drawer, sp0);
    Field2DObjectSprite_SetShadowPosition(arg1->drawer, sp4);
}

void WifiClubRoomPerson_AnimateHop(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    Field2DPoint sp0 = { Field2DObject_GetParam(arg1->object, 0), Field2DObject_GetParam(arg1->object, 1) };
    Field2DPoint sp4 = sp0;

    s16 temp_r0 = arg1->timer - 16;
    if (temp_r0 > 0) {
        s32 temp_r0_2 = (s16)(temp_r0 % 6) * 180;
        s32 temp_r0_3 = GF_SinDegNoWrap(temp_r0_2 / 6);

        s64 r6 = ((s64)temp_r0_3 << 14) + 2048;
        sp0.unk2 = (s16)sp0.unk2 - ((s32)(r6 >> 12) >> 12);
    }
    Field2DObjectSprite_SetPosition(arg1->drawer, sp0);
    Field2DObjectSprite_SetShadowPosition(arg1->drawer, sp4);

    Field2DObjectSprite_FaceDirection(arg1->drawer, 1);
}

s32 WifiClubRoomPerson_UpdatePlayer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    u32 temp_r4 = Field2DObject_GetParam(arg1->object, 6);
    u32 temp_r7 = Field2DObject_GetParam(arg1->object, 4);
    if (Field2DObject_GetParam(arg1->object, 5) != 0) {
        return 0;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        WifiClubRoomPerson *temp_r0 = WifiClubRoom_GetPersonInDirection(arg0, arg1, temp_r4);
        if (temp_r0 != NULL) {
            if (WifiClubRoomPerson_CanTalk(temp_r0) == 1) {
                arg1->talkPartnerId = Field2DObject_GetParam(temp_r0->object, 4);
            } else {
                arg1->talkPartnerId = 0;
            }
        } else {
            arg1->talkPartnerId = 0;
        }
    }
    if (arg0->inputLocked == 1) {
        return 0;
    }
    s16 var_r1 = 2;
    if (gSystem.heldKeys & PAD_BUTTON_B) {
        var_r1 = 3;
    }
    if (gSystem.heldKeys & PAD_KEY_UP) {
        if (temp_r4 == 0) {
            WifiClubRoom_QueueMoveCommand(arg0, var_r1, temp_r4, temp_r7);
        } else {
            WifiClubRoom_QueueMoveCommand(arg0, 1, 0, temp_r7);
        }
    } else if (gSystem.heldKeys & PAD_KEY_DOWN) {
        if (temp_r4 == 1) {
            WifiClubRoom_QueueMoveCommand(arg0, var_r1, temp_r4, temp_r7);
        } else {
            WifiClubRoom_QueueMoveCommand(arg0, 1, 1, temp_r7);
        }
    } else if (gSystem.heldKeys & PAD_KEY_LEFT) {
        if (temp_r4 == 2) {
            WifiClubRoom_QueueMoveCommand(arg0, var_r1, temp_r4, temp_r7);
        } else {
            WifiClubRoom_QueueMoveCommand(arg0, 1, 2, temp_r7);
        }
    } else if (gSystem.heldKeys & PAD_KEY_RIGHT) {
        if (temp_r4 == 3) {
            WifiClubRoom_QueueMoveCommand(arg0, var_r1, temp_r4, temp_r7);
        } else {
            WifiClubRoom_QueueMoveCommand(arg0, 1, 3, temp_r7);
        }
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateWarpIn(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->timer--;
    if (arg1->timer <= 0) {
        Field2DObjectSprite_SyncAnim(arg1->drawer);
        Field2DObjectSprite_SetAutoUpdate(arg1->drawer, 1);
        u32 temp_r6 = Field2DObject_GetParam(arg0->player->object, 4);
        u32 temp_r0 = Field2DObject_GetParam(arg1->object, 4);
        if (temp_r0 == temp_r6) {
            WifiClubRoom_SetPersonModePlayer(arg0, arg1);
        } else {
            WifiClubRoom_SetPersonActivityMode(arg0, arg1, arg1->activityMode);
        }
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateWarpOut(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->timer++;
    if (arg1->timer > 45) {
        return 1;
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateWander(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->timer--;
    if (arg1->timer <= 0) {
        arg1->timer = (1023 & MTRandom()) + 256;
        u32 temp_r4 = MTRandom() & 3;
        WifiClubRoom_QueueMoveCommand(arg0, 0, temp_r4, Field2DObject_GetParam(arg1->object, 4));
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->timer--;
    if (arg1->timer < 0) {
        arg1->timer = 28;
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateTalking(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    return 0;
}

void WifiClubRoom_QueueMoveCommand(WifiClubRoom *arg0, s16 arg1, u32 arg2, u32 arg3) {
    Field2DCommand temp;
    temp.unk0 = arg1;
    temp.unk2 = arg2;
    temp.unk3 = arg3;

    Field2DCommandQueue_Push(arg0->commandQueue, &temp);
}

void WifiClubRoom_QueuePersonCommand(WifiClubRoom *arg0, s16 arg1, WifiClubRoomPerson *arg2) {
    u32 temp_r7 = Field2DObject_GetParam(arg2->object, 6);
    WifiClubRoom_QueueMoveCommand(arg0, arg1, temp_r7, Field2DObject_GetParam(arg2->object, 4));
}

s32 WifiClubRoomPerson_IsNotWarping(WifiClubRoomPerson *arg0) {
    if (arg0->animate != WifiClubRoomPerson_AnimateWarp) {
        return 1;
    }
    return 0;
}

void WifiClubRoom_SetPersonActivityMode(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u8 arg2) {
    switch (arg2) {
    case 0:
        WifiClubRoom_SetPersonModeWander(arg0, arg1);
        break;
    case 1:
        WifiClubRoom_SetPersonModeBusy(arg0, arg1);
        break;
    default:
        GF_AssertFail();
        break;
    }
    arg1->activityMode = arg2;
}

void WifiClubRoomLights_Init(WifiClubRoom *arg0, WifiClubRoomLights *arg1, NARC *arg2) {
    memset(arg1, 0, sizeof(WifiClubRoomLights));
    arg1->plttDataRaw = GfGfxLoader_GetPlttDataFromOpenNarc(arg2, 32, &arg1->plttData, arg0->heapID);
    arg1->blinkMask = 15;
    arg1->blinkPhase = 0;
    arg1->blinkTimer = sWifiClubRoomLightBlinkFrames[arg1->blinkPhase];
}

void WifiClubRoomLights_Free(WifiClubRoom *arg0, WifiClubRoomLights *arg1) {
    Heap_Free(arg1->plttDataRaw);
    memset(arg1, 0, sizeof(WifiClubRoomLights));
}

void WifiClubRoomLights_Update(WifiClubRoom *arg0, WifiClubRoomLights *arg1) {
    WifiClubRoomLights_UpdateBlink(arg1);
    WifiClubRoomLights_UpdateSequence(arg1);
}

void WifiClubRoomLights_UpdateBlink(WifiClubRoomLights *arg0) {
    s32 var_r6;

    arg0->blinkTimer--;
    if (arg0->blinkTimer <= 0) {
        arg0->blinkPhase = (arg0->blinkPhase + 1) % 2;
        arg0->blinkTimer = sWifiClubRoomLightBlinkFrames[arg0->blinkPhase];
        if (arg0->blinkPhase == 1) {
            var_r6 = 2;
        } else {
            var_r6 = 1;
        }
        for (s32 i = 0; i < 4; i++) {
            if (arg0->blinkMask & (1 << i)) {
                GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, ((i + 1) * 2) + 224, WifiClubRoomLights_GetColor(arg0, var_r6), 2);
            }
        }
    }
}

void WifiClubRoomLights_UpdateSequence(WifiClubRoomLights *arg0) {
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r2 = 0;
    if (sWifiClubRoomLightSequence[arg0->step].timed == 1) {
        arg0->stepTimer--;
        if (arg0->stepTimer <= 0) {
            r2 = 1;
        }
    } else if (arg0->triggered != 0) {
        arg0->triggered = 0;
        r2 = 1;
    }
    if (r2 != 0) {
        arg0->step++;
        if (arg0->step >= 8) {
            WifiClubRoomLights_EndSequence(arg0);
            return;
        }
        arg0->stepTimer = sWifiClubRoomLightSequence[arg0->step].frames;
        r0 = sWifiClubRoomLightSequence[arg0->step].setColor;
        r1 = sWifiClubRoomLightSequence[arg0->step].color;
    }
    if (r0 != 0) {
        GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, ((arg0->activeLight + 1) * 2) + 224, WifiClubRoomLights_GetColor(arg0, r1), 2);
    }
}

void *WifiClubRoomLights_GetColor(WifiClubRoomLights *arg0, s32 arg1) {
    return arg0->plttData->pRawData + (arg1 + 1) * 2;
}

void WifiClubRoomLights_EndSequence(WifiClubRoomLights *arg0) {
    arg0->blinkMask = (arg0->blinkMask | (1 << arg0->activeLight));
    arg0->activeLight = 0;
    arg0->step = 0;
    arg0->stepTimer = 0;
    arg0->triggered = 0;
}

void WifiClubRoomLights_Trigger(WifiClubRoomLights *arg0) {
    arg0->triggered = 1;
}

void WifiClubRoomLights_Start(WifiClubRoomLights *arg0, u8 arg1) {
    if (arg0->step != 0) {
        WifiClubRoomLights_EndSequence(arg0);
    }
    arg0->blinkMask = arg0->blinkMask ^ (1 << arg1);
    arg0->activeLight = arg1;
    WifiClubRoomLights_Trigger(arg0);
}

void WifiClubRoomLights_Continue(WifiClubRoomLights *arg0) {
    if (arg0->step != 0) {
        WifiClubRoomLights_Trigger(arg0);
    }
}
