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
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
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
    arg0->unk4 = arg1;
    u32 temp_r7 = (arg5 - 1) / 8;
    arg0->unk10 = arg3;
    WifiClubRoomGfx_Init(&arg0->unk14, arg0->unk4, arg2);
    WifiClubRoom_InitBgs(arg0->unk10, arg1);

    UnkStruct_ov44_02232914 sp4 = ov44_02232E90(temp_r7);
    arg0->unk6E4 = ov42_02227EE0(sp4.unk0, sp4.unk2, arg1);
    ov42_02227F48(arg0->unk6E4, ov44_02232E9C(temp_r7));
    arg0->unk6E8 = ov42_02228010(0x30, arg1);
    WifiClubRoom_CreateDrawers(arg0, arg4, arg1);
    ov42_02229394(&arg0->unk6F0);
    WifiClubRoom_CreateMapBg(arg0, arg1, arg2, temp_r7);
    WifiClubRoom_LoadBgGfx(arg0->unk10, arg1, arg2);
    WifiClubRoomLights_Init(arg0, &arg0->unk6D0, arg2);
    arg0->unk6F8 = ov42_02229A40(0x80, arg1);
    arg0->unk0 = 1;
}

void WifiClubRoom_Free(WifiClubRoom *arg0) {
    ov42_02229A78(arg0->unk6F8);
    WifiClubRoom_DeleteMapBg(arg0);
    WifiClubRoomLights_Free(arg0, &arg0->unk6D0);
    WifiClubRoom_DeleteAllPersons(arg0);
    WifiClubRoom_DeleteDrawers(arg0);
    ov42_02228050(arg0->unk6E8);
    ov42_02227F28(arg0->unk6E4);
    WifiClubRoomGfx_Free(&arg0->unk14);
    WifiClubRoom_FreeBgs(arg0->unk10);
    memset(arg0, 0, sizeof(WifiClubRoom));
}

s32 WifiClubRoom_IsInitialized(WifiClubRoom *arg0) {
    return arg0->unk0;
}

s32 WifiClubRoom_Update(WifiClubRoom *arg0) {
    ov42_0222807C(arg0->unk6E8);
    WifiClubRoom_UpdatePersons(arg0);
    WifiClubRoom_ProcessMoveCommands(arg0);
    ov42_02229358(&arg0->unk6F0, arg0->unk6CC->unk0);
    ov42_02229420(arg0->unk6F4, &arg0->unk6F0);
    ov42_022290DC(arg0->unk6EC);
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
    if (arg0->unk0 != 0) {
        SpriteList_RenderAndAnimateSprites(arg0->unk14.unk0);
    }
}

u8 WifiClubRoom_GetTalkPartnerId(WifiClubRoom *arg0) {
    GF_ASSERT(arg0->unk6CC != NULL);
    return arg0->unk6CC->unkA;
}

s32 WifiClubRoom_GetTileUnderPlayer(WifiClubRoom *arg0) {
    GF_ASSERT(arg0->unk6CC != NULL);
    UnkStruct_ov44_02232914 temp_r0 = ov42_022282DC(arg0->unk6CC->unk0);
    return ov42_02227FDC(arg0->unk6E4, temp_r0.unk0 / 16, temp_r0.unk2 / 16);
}

s32 WifiClubRoom_GetTileBelowPlayer(WifiClubRoom *arg0) {
    GF_ASSERT(arg0->unk6CC != NULL);
    UnkStruct_ov44_02232914 sp0 = ov42_022282DC(arg0->unk6CC->unk0);
    return ov42_02227FDC(arg0->unk6E4, sp0.unk0 / 16, (sp0.unk2 / 16) + 1);
}

void WifiClubRoom_ShowFriendMarker(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    UnkStruct_ov44_02232914 sp0 = ov42_022282DC(arg1->unk0);
    WifiClubRoom_ShowFriendMarkerAt(arg0, sp0, ov42_0222920C(arg1->unk4) - 2);
}

void WifiClubRoom_HideFriendMarker(WifiClubRoom *arg0) {
    WifiClubRoom_HideFriendMarkerSprite(arg0);
}

void WifiClubRoom_SetInputLocked(WifiClubRoom *arg0, s32 arg1) {
    arg0->unkC = arg1;
}

s32 WifiClubRoom_IsInputLocked(WifiClubRoom *arg0) {
    return arg0->unkC;
}

WifiClubRoomPerson *WifiClubRoom_AddPlayer(WifiClubRoom *arg0, s16 arg1) {
    s16 spC;
    WifiClubRoomPerson *temp_r0 = WifiClubRoom_AllocPerson(arg0);
    arg0->unk6CC = temp_r0;
    WifiClubRoom_FindEntrancePos(arg0, &spC);
    WifiClubRoom_CreatePersonObject(arg0, temp_r0, &spC, 0, arg1, 0, 0);
    WifiClubRoom_SetPersonModePlayer(arg0, temp_r0);
    WifiClubRoom_CreatePersonDrawer(arg0, temp_r0, 1);
    ov42_02229358(&arg0->unk6F0, arg0->unk6CC->unk0);
    ov42_02229420(arg0->unk6F4, &arg0->unk6F0);
    return temp_r0;
}

WifiClubRoomPerson *WifiClubRoom_AddFriend(WifiClubRoom *arg0, s16 arg1, s16 arg2) {
    s16 spC[2];
    WifiClubRoomPerson *temp_r4 = WifiClubRoom_AllocPerson(arg0);
    WifiClubRoom_FindFriendSpawnPos(arg0, &spC[0], arg2 - 1);
    if (arg0->unk6CC != NULL) {
        UnkStruct_ov44_02232914 sp0 = ov42_022282DC(arg0->unk6CC->unk0);
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
    ov42_02228188(arg1->unk0, 6);
    arg1->unk10 = WifiClubRoomPerson_UpdateWarpOut;
    arg1->unk18 = WifiClubRoomPerson_AnimateWarp;
    arg1->unkC = 2;
    arg1->unk8 = 0;
    ov42_0222919C(arg1->unk4, 0);
    ov42_022291A0(arg1->unk4);
    WifiClubRoom_QueuePersonCommand(arg0, 4, arg1);
}

void WifiClubRoomPerson_Delete(WifiClubRoomPerson *arg0) {
    ov42_022290C4(arg0->unk4);
    ov42_02228100(arg0->unk0);
    memset(arg0, 0, sizeof(WifiClubRoomPerson));
}

u8 WifiClubRoomPerson_GetId(WifiClubRoomPerson *arg0) {
    return ov42_02228188(arg0->unk0, 4);
}

s32 WifiClubRoomPerson_GetOppositeDirection(WifiClubRoomPerson *arg0) {
    return ov42_022282A4(ov42_02228188(arg0->unk0, 6));
}

void WifiClubRoom_StartTalk(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s32 arg2) {
    arg1->unk14 = arg1->unk10;
    arg1->unk10 = WifiClubRoomPerson_UpdateTalking;
    arg1->unkE = arg1->unkC;
    arg1->unkC = 5;
    WifiClubRoom_QueueMoveCommand(arg0, 0, arg2, ov42_02228188(arg1->unk0, 4));
    ov42_0222919C(arg1->unk4, 0);
    ov42_02229218(arg1->unk4, arg2);

    UnkStruct_ov44_02232914 temp_r3 = { ov42_02228188(arg1->unk0, 0), ov42_02228188(arg1->unk0, 1) };
    ov42_022291D8(arg1->unk4, temp_r3);
}

void WifiClubRoom_EndTalk(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    if (arg1 != NULL) {
        if (arg1->unk14 != 0) {
            arg1->unk10 = arg1->unk14;
            arg1->unk14 = 0;
            arg1->unkC = arg1->unkE;
            ov42_0222919C(arg1->unk4, 1);
        }
    }
}

void WifiClubRoom_SetPersonBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u8 arg2) {
    if (WifiClubRoomPerson_IsNotWarping(arg1) == 0) {
        arg1->unkB = arg2;
        return;
    }
    WifiClubRoom_SetPersonActivityMode(arg0, arg1, arg2);
}

void WifiClubRoom_StartCounterLight(WifiClubRoom *arg0) {
    if (WifiClubRoom_IsPlayerFacingCounter(arg0) == 1) {
        s8 temp_r4 = (s8)WifiClubRoom_GetTileBelowPlayer(arg0) - 3;
        GF_ASSERT(temp_r4 >= 0 && temp_r4 < 4);
        WifiClubRoomLights_Start(&arg0->unk6D0, temp_r4);
        return;
    }
    GF_AssertFail();
}

void WifiClubRoom_ContinueLightSequence(WifiClubRoom *arg0) {
    WifiClubRoomLights_Continue(&arg0->unk6D0);
}

void WifiClubRoom_UpdateLights(WifiClubRoom *arg0) {
    WifiClubRoomLights_Update(arg0, &arg0->unk6D0);
}

void WifiClubRoom_DeleteAllPersons(WifiClubRoom *arg0) {
    for (s32 i = 0; i < 48; i++) {
        if (arg0->unk18C[i].unk0 != 0) {
            WifiClubRoomPerson_Delete(&arg0->unk18C[i]);
        }
    }
}

s32 WifiClubRoom_IsPlayerIdle(WifiClubRoom *arg0) {
    if (ov42_02228188(arg0->unk6CC->unk0, 5) == 0) {
        return 1;
    }
    return 0;
}

s32 WifiClubRoom_IsPlayerFacingCounter(WifiClubRoom *arg0) {
    UnkStruct_ov44_02232914 sp0 = ov42_022282DC(arg0->unk6CC->unk0);
    u32 temp_r4 = ov42_02228188(arg0->unk6CC->unk0, 6);
    sp0 = ov42_02228270(sp0, temp_r4);
    s32 temp_r0_3 = ov42_02227FDC(arg0->unk6E4, sp0.unk0 / 16, sp0.unk2 / 16);
    if ((temp_r4 == 0) && (temp_r0_3 == 2)) {
        return 1;
    }
    return 0;
}

void WifiClubRoom_ProcessMoveCommands(WifiClubRoom *arg0) {
    UnkStruct_ov42_02228CDC sp8;
    UnkStruct_ov42_02228EB0 sp0;
    if (ov42_02229AC8(arg0->unk6F8, &sp8) == 1) {
        do {
            if (ov42_02228C80(arg0->unk6E4, arg0->unk6E8, &sp8, &sp0) == 1) {
                ov42_02228068(arg0->unk6E8, &sp0);
            }
        } while (ov42_02229AC8(arg0->unk6F8, &sp8) == 1);
    }
}

void WifiClubRoom_UpdatePersons(WifiClubRoom *arg0) {
    for (s32 i = 0; i < 48; i++) {
        if ((arg0->unk18C[i].unk0 != 0) && (arg0->unk18C[i].unk10(arg0, &arg0->unk18C[i]) == 1)) {
            WifiClubRoomPerson_Delete(&arg0->unk18C[i]);
        }
    }
}

void WifiClubRoom_AnimatePersons(WifiClubRoom *arg0) {
    for (s32 i = 0; i < 48; i++) {
        if (arg0->unk18C[i].unk0 != 0) {
            arg0->unk18C[i].unk18(arg0, &arg0->unk18C[i]);
        }
    }
}

WifiClubRoomPerson *WifiClubRoom_GetPersonInDirection(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u32 arg2) {
    UnkStruct_ov42_02228110 *temp_r0 = ov42_022283BC(arg1->unk0, arg0->unk6E8, arg2);
    if (temp_r0 != NULL) {
        for (s32 i = 0; i < 48; i++) {
            UnkStruct_ov42_02228110 *temp_r3 = arg0->unk18C[i].unk0;
            if (temp_r3 == temp_r0) {
                return &arg0->unk18C[i];
            }
        }
    }
    return NULL;
}

void WifiClubRoom_CreateDrawers(WifiClubRoom *arg0, s32 arg1, enum HeapID arg2) {
    arg0->unk6EC = ov42_02228F24(arg0->unk14.unk0, NULL, 48, arg1, 0, 1, arg2);
    ov42_02229028(arg0->unk6EC, 2, arg2);
}

void WifiClubRoom_DeleteDrawers(WifiClubRoom *arg0) {
    ov42_02228F94(arg0->unk6EC);
}

void WifiClubRoom_CreateMapBg(WifiClubRoom *arg0, enum HeapID arg1, NARC *arg2, s32 arg3) {
    UnkTemplate_ov42_022293B8 sp0 = {
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
    arg0->unk6F4 = ov42_022293B8(&arg0->unk14.unk4, arg0->unk10, &sp0, arg1);
}

void WifiClubRoom_DeleteMapBg(WifiClubRoom *arg0) {
    ov42_0222940C(arg0->unk6F4);
}

void WifiClubRoomGfx_Init(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc) {
    arg0->unk0 = G2dRenderer_Init(96, &arg0->unk4, heapID);
    G2dRenderer_SetSubSurfaceCoords(&arg0->unk4, 0, 0x320000);
    for (s32 i = 0; i < 4; i++) {
        arg0->unk12C[i] = Create2DGfxResObjMan(16, (GfGfxResType)i, heapID);
    }
    WifiClubRoomGfx_LoadResources(arg0, heapID, narc);
    WifiClubRoomGfx_CreateSprites(arg0, heapID);
}

void WifiClubRoomGfx_Free(WifiClubRoomGfx *arg0) {
    WifiClubRoomGfx_DeleteSprites(arg0);
    WifiClubRoomGfx_FreeResources(arg0);
    for (s32 i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(arg0->unk12C[i]);
    }
    SpriteList_Delete(arg0->unk0);
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
    arg0->unk13C[0] = AddCharResObjFromOpenNarc(arg0->unk12C[0], narc, NARC_a_0_3_8, FALSE, 50, NNS_G2D_VRAM_TYPE_2DMAIN, heapID);
    arg0->unk13C[1] = AddPlttResObjFromOpenNarc(arg0->unk12C[1], narc, NARC_fielddata_encountdata_g_enc_data, FALSE, 50, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapID);
    arg0->unk13C[2] = AddCellOrAnimResObjFromOpenNarc(arg0->unk12C[2], narc, NARC_a_0_3_9, FALSE, 50, GF_GFX_RES_TYPE_CELL, heapID);
    arg0->unk13C[3] = AddCellOrAnimResObjFromOpenNarc(arg0->unk12C[3], narc, NARC_a_0_4_0, FALSE, 50, GF_GFX_RES_TYPE_ANIM, heapID);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(arg0->unk13C[0]) == TRUE);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(arg0->unk13C[1]) == TRUE);
    GF2DGfxResObj_FreeData(arg0->unk13C[0]);
    GF2DGfxResObj_FreeData(arg0->unk13C[1]);
    CreateSpriteResourcesHeader(&arg0->unk14C, 50, 50, 50, 50, -1, -1, 0, 2, arg0->unk12C[0], arg0->unk12C[1], arg0->unk12C[2], arg0->unk12C[3], 0, 0);
}

void WifiClubRoomGfx_FreeSpriteResources(WifiClubRoomGfx *arg0) {
    SpriteTransfer_DeleteCharTransferTask(arg0->unk13C[0]);
    SpriteTransfer_DeletePlttTransferTask(arg0->unk13C[1]);
    for (s32 i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(arg0->unk12C[i], arg0->unk13C[i]);
    }
}

void WifiClubRoomGfx_CreateMarkerSprites(WifiClubRoomGfx *arg0, enum HeapID heapID) {
    SpriteTemplate spriteTemplate = { };
    spriteTemplate.spriteList = arg0->unk0;
    spriteTemplate.header = &arg0->unk14C;

    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;

    spriteTemplate.scale.x = 4096;
    spriteTemplate.scale.y = 4096;
    spriteTemplate.heapID = heapID;

    spriteTemplate.drawPriority = 0;

    arg0->unk170 = Sprite_CreateAffine(&spriteTemplate);
    arg0->unk174 = Sprite_CreateAffine(&spriteTemplate);
    Sprite_SetDrawFlag(arg0->unk170, 0);
    Sprite_SetDrawFlag(arg0->unk174, 0);
    Sprite_SetAnimActiveFlag(arg0->unk170, 1);
    Sprite_SetAnimCtrlSeq(arg0->unk174, 1);
}

void WifiClubRoomGfx_DeleteMarkerSprites(WifiClubRoomGfx *arg0) {
    Sprite_Delete(arg0->unk170);
    Sprite_Delete(arg0->unk174);
}

void WifiClubRoom_ShowExitArrow(WifiClubRoom *arg0) {
    VecFx32 vec;
    s16 sp0[2];

    WifiClubRoom_FindEntrancePos(arg0, sp0);
    sp0[0] = sp0[0] + 8;
    sp0[1] = sp0[1] + 32;
    vec.x = sp0[0] << 12;
    vec.y = sp0[1] << 12;
    Sprite_SetMatrix(arg0->unk14.unk170, &vec);
    Sprite_SetDrawFlag(arg0->unk14.unk170, 1);
}

void WifiClubRoom_HideExitArrow(WifiClubRoom *arg0) {
    Sprite_SetDrawFlag(arg0->unk14.unk170, 0);
}

void WifiClubRoom_ShowFriendMarkerAt(WifiClubRoom *arg0, UnkStruct_ov44_02232914 arg1, s32 arg2) {
    VecFx32 vec;
    vec.x = (s16)(arg1.unk0 + 8) * FX32_ONE;
    vec.y = arg1.unk2 * FX32_ONE;
    Sprite_SetMatrix(arg0->unk14.unk174, &vec);
    Sprite_SetDrawPriority(arg0->unk14.unk174, arg2);
    Sprite_SetDrawFlag(arg0->unk14.unk174, 1);
}

void WifiClubRoom_HideFriendMarkerSprite(WifiClubRoom *arg0) {
    Sprite_SetDrawFlag(arg0->unk14.unk174, 0);
}

s32 WifiClubRoom_IsPlayerAtEntrance(WifiClubRoom *arg0) {
    if (arg0->unk6CC == 0) {
        return 0;
    }
    UnkStruct_ov44_02232914 sp0 = ov42_022282DC(arg0->unk6CC->unk0);
    u32 temp_r4 = ov42_02228188(arg0->unk6CC->unk0, 6);
    if (ov42_02227FDC(arg0->unk6E4, sp0.unk0 / 16, sp0.unk2 / 16) == 0xF && temp_r4 == 1) {
        return 1;
    }
    return 0;
}

void WifiClubRoom_FindFriendSpawnPos(WifiClubRoom *arg0, s16 *arg1, u32 arg2) {
    u32 r7 = arg2 / 8;
    u32 r4 = arg2 % 8;
    u16 r6 = ov42_02227F40(arg0->unk6E4);
    u16 sp4 = ov42_02227F44(arg0->unk6E4);

    for (u32 i = 0; i < sp4; i++) {
        for (u32 j = 0; j < r6; j++) {
            s32 temp_r0 = ov42_02227FDC(arg0->unk6E4, j, i);
            if (temp_r0 == r4 + 7) {
                temp_r0 = ov42_02227FDC(arg0->unk6E4, j, i + 1);
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
    u16 temp_r5 = ov42_02227F40(arg0->unk6E4);
    u16 temp_r0 = ov42_02227F44(arg0->unk6E4);

    for (u32 i = 0; i < temp_r0; i++) {
        for (u32 j = 0; j < temp_r5; j++) {
            if (ov42_02227FDC(arg0->unk6E4, j, i) == 15) {
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
        if (arg0->unk18C[i].unk0 == 0) {
            return &arg0->unk18C[i];
        }
    }
    GF_AssertFail();
    return NULL;
}

void WifiClubRoom_CreatePersonDrawer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s32 arg2) {
    arg1->unk4 = ov42_0222903C(arg0->unk6EC, arg1->unk0, arg2, arg0->unk4);
    if (arg2 == 0) {
        ov42_0222919C(arg1->unk4, 0);
        ov42_022291A0(arg1->unk4);
        ov42_02229200(arg1->unk4, 0);
    }
}

void WifiClubRoom_CreatePersonObject(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s16 *arg2, s16 arg3, u16 arg4, u8 arg5, s32 arg6) {
    UnkStruct_ov42_02122667 sp;
    sp.unk0 = arg2[0];
    sp.unk2 = arg2[1];
    sp.unk4 = arg3;
    sp.unk6 = arg6;
    sp.unk8 = arg5;
    sp.unkA = arg4;

    arg1->unk0 = ov42_022280B8(arg0->unk6E8, &sp);
}

s32 WifiClubRoomPerson_CanTalk(WifiClubRoomPerson *arg0) {
    switch (arg0->unkC) {
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
    arg1->unk10 = WifiClubRoomPerson_UpdatePlayer;
    arg1->unk18 = WifiClubRoomPerson_AnimateNone;
    arg1->unkC = 0;
    WifiClubRoom_QueuePersonCommand(arg0, 0, arg1);
}

void WifiClubRoom_SetPersonModeWarpIn(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->unk10 = WifiClubRoomPerson_UpdateWarpIn;
    arg1->unk18 = WifiClubRoomPerson_AnimateWarp;
    arg1->unk8 = 45;
    arg1->unkC = 1;
}

void WifiClubRoom_SetPersonModeWander(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->unk10 = WifiClubRoomPerson_UpdateWander;
    arg1->unk18 = WifiClubRoomPerson_AnimateNone;
    arg1->unk8 = (1023 & MTRandom()) + 256;
    arg1->unkC = 3;
    WifiClubRoom_QueuePersonCommand(arg0, 0, arg1);
    ov42_0222919C(arg1->unk4, 1);
}

void WifiClubRoom_SetPersonModeBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->unk10 = WifiClubRoomPerson_UpdateBusy;
    arg1->unk18 = WifiClubRoomPerson_AnimateHop;
    arg1->unk8 = 28;
    arg1->unkC = 4;
    WifiClubRoom_QueuePersonCommand(arg0, 4, arg1);
    ov42_0222919C(arg1->unk4, 0);
}

void WifiClubRoomPerson_AnimateNone(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
}

void WifiClubRoomPerson_AnimateWarp(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    ov42_022291AC(arg1->unk4);
    if (ov42_022291F4(arg1->unk4) == 0) {
        ov42_02229200(arg1->unk4, 1);
    }
    UnkStruct_ov44_02232914 sp0 = { ov42_02228188(arg1->unk0, 0), ov42_02228188(arg1->unk0, 1) };
    UnkStruct_ov44_02232914 sp4 = sp0;

    s64 temp_ret = (s64)GF_SinDegNoWrap(arg1->unk8) * (signed)0xFFF40000;
    temp_ret += 2048;

    s32 r1 = (temp_ret >> 12);
    sp0.unk2 = (s16)sp0.unk2 + (r1 >> 12);

    ov42_022291D8(arg1->unk4, sp0);
    ov42_02229258(arg1->unk4, sp4);
}

void WifiClubRoomPerson_AnimateHop(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    UnkStruct_ov44_02232914 sp0 = { ov42_02228188(arg1->unk0, 0), ov42_02228188(arg1->unk0, 1) };
    UnkStruct_ov44_02232914 sp4 = sp0;

    s16 temp_r0 = arg1->unk8 - 16;
    if (temp_r0 > 0) {
        s32 temp_r0_2 = (s16)(temp_r0 % 6) * 180;
        s32 temp_r0_3 = GF_SinDegNoWrap(temp_r0_2 / 6);

        s64 r6 = ((s64)temp_r0_3 << 14) + 2048;
        sp0.unk2 = (s16)sp0.unk2 - ((s32)(r6 >> 12) >> 12);
    }
    ov42_022291D8(arg1->unk4, sp0);
    ov42_02229258(arg1->unk4, sp4);

    ov42_02229218(arg1->unk4, 1);
}

s32 WifiClubRoomPerson_UpdatePlayer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    u32 temp_r4 = ov42_02228188(arg1->unk0, 6);
    u32 temp_r7 = ov42_02228188(arg1->unk0, 4);
    if (ov42_02228188(arg1->unk0, 5) != 0) {
        return 0;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        WifiClubRoomPerson *temp_r0 = WifiClubRoom_GetPersonInDirection(arg0, arg1, temp_r4);
        if (temp_r0 != NULL) {
            if (WifiClubRoomPerson_CanTalk(temp_r0) == 1) {
                arg1->unkA = ov42_02228188(temp_r0->unk0, 4);
            } else {
                arg1->unkA = 0;
            }
        } else {
            arg1->unkA = 0;
        }
    }
    if (arg0->unkC == 1) {
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
    arg1->unk8--;
    if (arg1->unk8 <= 0) {
        ov42_022291B8(arg1->unk4);
        ov42_0222919C(arg1->unk4, 1);
        u32 temp_r6 = ov42_02228188(arg0->unk6CC->unk0, 4);
        u32 temp_r0 = ov42_02228188(arg1->unk0, 4);
        if (temp_r0 == temp_r6) {
            WifiClubRoom_SetPersonModePlayer(arg0, arg1);
        } else {
            WifiClubRoom_SetPersonActivityMode(arg0, arg1, arg1->unkB);
        }
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateWarpOut(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->unk8++;
    if (arg1->unk8 > 45) {
        return 1;
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateWander(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->unk8--;
    if (arg1->unk8 <= 0) {
        arg1->unk8 = (1023 & MTRandom()) + 256;
        u32 temp_r4 = MTRandom() & 3;
        WifiClubRoom_QueueMoveCommand(arg0, 0, temp_r4, ov42_02228188(arg1->unk0, 4));
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    arg1->unk8--;
    if (arg1->unk8 < 0) {
        arg1->unk8 = 28;
    }
    return 0;
}

s32 WifiClubRoomPerson_UpdateTalking(WifiClubRoom *arg0, WifiClubRoomPerson *arg1) {
    return 0;
}

void WifiClubRoom_QueueMoveCommand(WifiClubRoom *arg0, s16 arg1, u32 arg2, u32 arg3) {
    UnkStruct_ov42_02228CDC temp;
    temp.unk0 = arg1;
    temp.unk2 = arg2;
    temp.unk3 = arg3;

    ov42_02229A8C(arg0->unk6F8, &temp);
}

void WifiClubRoom_QueuePersonCommand(WifiClubRoom *arg0, s16 arg1, WifiClubRoomPerson *arg2) {
    u32 temp_r7 = ov42_02228188(arg2->unk0, 6);
    WifiClubRoom_QueueMoveCommand(arg0, arg1, temp_r7, ov42_02228188(arg2->unk0, 4));
}

s32 WifiClubRoomPerson_IsNotWarping(WifiClubRoomPerson *arg0) {
    if (arg0->unk18 != WifiClubRoomPerson_AnimateWarp) {
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
    arg1->unkB = arg2;
}

void WifiClubRoomLights_Init(WifiClubRoom *arg0, WifiClubRoomLights *arg1, NARC *arg2) {
    memset(arg1, 0, sizeof(WifiClubRoomLights));
    arg1->unk0 = GfGfxLoader_GetPlttDataFromOpenNarc(arg2, 32, &arg1->unk4, arg0->unk4);
    arg1->unk8 = 15;
    arg1->unk9 = 0;
    arg1->unkA = sWifiClubRoomLightBlinkFrames[arg1->unk9];
}

void WifiClubRoomLights_Free(WifiClubRoom *arg0, WifiClubRoomLights *arg1) {
    Heap_Free(arg1->unk0);
    memset(arg1, 0, sizeof(WifiClubRoomLights));
}

void WifiClubRoomLights_Update(WifiClubRoom *arg0, WifiClubRoomLights *arg1) {
    WifiClubRoomLights_UpdateBlink(arg1);
    WifiClubRoomLights_UpdateSequence(arg1);
}

void WifiClubRoomLights_UpdateBlink(WifiClubRoomLights *arg0) {
    s32 var_r6;

    arg0->unkA--;
    if (arg0->unkA <= 0) {
        arg0->unk9 = (arg0->unk9 + 1) % 2;
        arg0->unkA = sWifiClubRoomLightBlinkFrames[arg0->unk9];
        if (arg0->unk9 == 1) {
            var_r6 = 2;
        } else {
            var_r6 = 1;
        }
        for (s32 i = 0; i < 4; i++) {
            if (arg0->unk8 & (1 << i)) {
                GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, ((i + 1) * 2) + 224, WifiClubRoomLights_GetColor(arg0, var_r6), 2);
            }
        }
    }
}

void WifiClubRoomLights_UpdateSequence(WifiClubRoomLights *arg0) {
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r2 = 0;
    if (sWifiClubRoomLightSequence[arg0->unkD].unk_0 == 1) {
        arg0->unkE--;
        if (arg0->unkE <= 0) {
            r2 = 1;
        }
    } else if (arg0->unk10 != 0) {
        arg0->unk10 = 0;
        r2 = 1;
    }
    if (r2 != 0) {
        arg0->unkD++;
        if (arg0->unkD >= 8) {
            WifiClubRoomLights_EndSequence(arg0);
            return;
        }
        arg0->unkE = sWifiClubRoomLightSequence[arg0->unkD].unk_1;
        r0 = sWifiClubRoomLightSequence[arg0->unkD].unk_2;
        r1 = sWifiClubRoomLightSequence[arg0->unkD].unk_3;
    }
    if (r0 != 0) {
        GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, ((arg0->unkC + 1) * 2) + 224, WifiClubRoomLights_GetColor(arg0, r1), 2);
    }
}

void *WifiClubRoomLights_GetColor(WifiClubRoomLights *arg0, s32 arg1) {
    return arg0->unk4->pRawData + (arg1 + 1) * 2;
}

void WifiClubRoomLights_EndSequence(WifiClubRoomLights *arg0) {
    arg0->unk8 = (arg0->unk8 | (1 << arg0->unkC));
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->unkE = 0;
    arg0->unk10 = 0;
}

void WifiClubRoomLights_Trigger(WifiClubRoomLights *arg0) {
    arg0->unk10 = 1;
}

void WifiClubRoomLights_Start(WifiClubRoomLights *arg0, u8 arg1) {
    if (arg0->unkD != 0) {
        WifiClubRoomLights_EndSequence(arg0);
    }
    arg0->unk8 = arg0->unk8 ^ (1 << arg1);
    arg0->unkC = arg1;
    WifiClubRoomLights_Trigger(arg0);
}

void WifiClubRoomLights_Continue(WifiClubRoomLights *arg0) {
    if (arg0->unkD != 0) {
        WifiClubRoomLights_Trigger(arg0);
    }
}
