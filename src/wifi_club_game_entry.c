
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0778.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_00_arm.h"
#include "overlay_00_thumb.h"
#include "overlay_42.h"
#include "overlay_44.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sprite.h"
#include "text.h"
#include "sound_playback.h"
#include "sprite_resource.h"
#include "oam_manager.h"
#include "communication_player_state.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "unk_020971F8.h"
#include "vram_transfer_manager.h"

const u8 sWifiClubGameEntryAvatarSpinFrames[4] = { 0x08, 0x00, 0x04, 0x0C };

const WindowTemplate sWifiClubGameEntryYesNoWindowTemplate = {
    .bgId = 2,
    .left = 25,
    .top = 13,
    .width = 5,
    .height = 4,
    .palette = 1,
    .baseTile = 57,
};

const WifiClubGameEntryAvatarFunc sWifiClubGameEntryAvatarFuncs[4] = { WifiClubGameEntry_AvatarNone, WifiClubGameEntry_AvatarStartSpin, WifiClubGameEntry_AvatarUpdateSpin, WifiClubGameEntry_AvatarIdle };

const ObjCharTransferTemplate sWifiClubGameEntryObjCharTransferTemplate = {
    4,
    0x20000,
    0x4000,
    HEAP_ID_DEFAULT
};
const GraphicsModes sWifiClubGameEntryGraphicsModes = {
    GX_DISPMODE_GRAPHICS,
    GX_BGMODE_0,
    GX_BGMODE_0,
    GX_BG0_AS_2D
};

const BgTemplate sWifiClubGameEntryBgTemplateMain0 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

const BgTemplate sWifiClubGameEntryBgTemplateMain2 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xd800,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_23,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

const BgTemplate sWifiClubGameEntryBgTemplateMain1 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xd000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

const GraphicsBanks sWifiClubGameEntryGraphicsBanks = {
    GX_VRAM_BG_128_A, GX_VRAM_BGEXTPLTT_NONE, GX_VRAM_SUB_BG_128_C, GX_VRAM_SUB_BGEXTPLTT_NONE, GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I, GX_VRAM_SUB_OBJEXTPLTT_NONE, GX_VRAM_TEX_NONE, GX_VRAM_TEXPLTT_NONE
};

const UnkStruct_ov42_02122667 sWifiClubGameEntryAvatarTemplates[4] = {
    { 208, 48, 0, 0, 1, 0 },
    { 232, 72, 1, 0, 2, 0 },
    { 208, 96, 2, 0, 0, 0 },
    { 184, 72, 3, 0, 3, 0 },
};

const WifiClubGameEntryStateFunc sWifiClubGameEntryMemberStates[30] = {
    WifiClubGameEntry_Member_Start,
    WifiClubGameEntry_Member_WaitLeaderInfo,
    Wifi_PromptAwaitingResponse,
    WifiClubGameEntry_Member_WaitJoinAnswer,
    WifiClubGameEntry_Member_PrintApproved,
    WifiClubGameEntry_Member_WaitApproved,
    WifiClubGameEntry_Member_PrintRefused,
    WifiClubGameEntry_Member_EndRefused,
    Wifi_PromptAwaitingMembers,
    WifiClubGameEntry_Member_WaitAwaitingMessage,
    WifiClubGameEntry_Member_WaitStart,
    WifiClubGameEntry_Member_SendStatus,
    WifiClubGameEntry_Member_CheckStatuses,
    WifiClubGameEntry_Member_StartFinalSync,
    WifiClubGameEntry_Member_WaitFinalSync,
    WifiClubGameEntry_Member_OnDropped,
    Wifi_PromptMemberDrop,
    WifiClubGameEntry_Member_EndAfterDropped,
    Wifi_PromptToggleVoiceChat_,
    WifiClubGameEntry_Member_ShowVoiceChatYesNo,
    WifiClubGameEntry_Member_HandleVoiceChatYesNo,
    WifiClubGameEntry_Member_PrintLeaderBusy,
    WifiClubGameEntry_Member_ShowApplyAgainYesNo,
    WifiClubGameEntry_Member_HandleApplyAgainYesNo,
    WifiClubGameEntry_Member_PrintContacting,
    WifiClubGameEntry_Member_ApplyAgain,
    WifiClubGameEntry_Member_WaitReconnected,
    WifiClubGameEntry_Member_RebuildScreen,
    WifiClubGameEntry_Member_WaitFadeIn,
    WifiClubGameEntry_Member_Cancel,
};

const WifiClubGameEntryStateFunc sWifiClubGameEntryLeaderStates[33] = {
    WifiClubGameEntry_Leader_Start,
    Wifi_PromptReadyMessage,
    WifiClubGameEntry_Leader_HandleInput,
    Wifi_PromptUserJoinRequest,
    WifiClubGameEntry_Leader_ShowAcceptYesNo,
    WifiClubGameEntry_Leader_HandleAcceptYesNo,
    Wifi_PromptInsufficientMembers,
    WifiClubGameEntry_Leader_WaitNotEnoughMembers,
    Wifi_PromptConfirmMembers,
    WifiClubGameEntry_Leader_ShowConfirmYesNo,
    WifiClubGameEntry_Leader_HandleConfirmYesNo,
    WifiClubGameEntry_Leader_StartCommunicating,
    WifiClubGameEntry_Leader_WaitMembersConnected,
    WifiClubGameEntry_Leader_SendStartCommand,
    WifiClubGameEntry_Leader_WaitStartSync,
    WifiClubGameEntry_Leader_SendStatus,
    WifiClubGameEntry_Leader_CheckStatuses,
    WifiClubGameEntry_Leader_StartFinalSync,
    WifiClubGameEntry_Leader_WaitFinalSync,
    Wifi_PromptStopFindingMembers,
    WifiClubGameEntry_Leader_ShowStopSeekingYesNo,
    WifiClubGameEntry_Leader_HandleStopSeekingYesNo,
    Wifi_PromptDropAsLeader,
    WifiClubGameEntry_Leader_ShowDisbandYesNo,
    WifiClubGameEntry_Leader_HandleDisbandYesNo,
    Wifi_PromptCancelInvites,
    WifiClubGameEntry_Leader_Cancel,
    WifiClubGameEntry_Leader_OnMemberDropped,
    Wifi_PromptMemberDropped,
    WifiClubGameEntry_Leader_EndAfterMemberDropped,
    Wifi_PromptToggleVoiceChat,
    WifiClubGameEntry_Leader_ShowVoiceChatYesNo,
    WifiClubGameEntry_Leader_HandleVoiceChatYesNo,
};

const u8 sWifiClubGameEntryBgPaletteFrames[20] = { 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5, 0, 0 };

u8 sWifiClubGameEntryTitleMsgIds[4] = { 126, 127, 127, 127 };

FS_EXTERN_OVERLAY(OVY_42);

void WifiClubGameEntry_Init(OverlayManager *overlayMananger) {
    WifiClubGameEntryArgs *temp_r5 = OverlayManager_GetArgs(overlayMananger);
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_42), OVY_LOAD_ASYNC);
    GF_ASSERT(CommState_IsWifi() == 1);
    Heap_Create(HEAP_ID_3, HEAP_ID_103, 0x18000);
    Heap_Create(HEAP_ID_3, HEAP_ID_104, 0xA000);
    WifiClubGameEntry *temp_r0 = OverlayManager_CreateAndGetData(overlayMananger, sizeof(WifiClubGameEntry), HEAP_ID_103);
    MI_CpuFill8(temp_r0, 0, sizeof(WifiClubGameEntry));
    MI_CpuFill8(temp_r0->unk1C, 1, 4);
    MI_CpuFill8(temp_r0->unk20, 1, 4);
    GF_CreateVramTransferManager(16, HEAP_ID_103);
    UnkStruct_ov44_0223197C *temp_r0_2 = CommState_GetWifiClubData();
    temp_r0->unk0 = &temp_r0_2->unk0;
    temp_r0_2->unk0.unk21 = temp_r0_2->unk0.unk22;
    temp_r0->unk4 = WifiClubGameEntry_IsLocalLeader(temp_r0);
    temp_r0->unk6 = 255;
    WifiClubComm_RegisterCommands(temp_r0);
    WifiClubGameEntry_SetDisconnectCallback(temp_r0);
    WifiClubGameEntry_InitScreen(temp_r0, temp_r5, HEAP_ID_103);
    Main_SetVBlankIntrCB((void *)&WifiClubGameEntry_VBlankCB, temp_r0);
    WirelessStatusIcon_Create();
    temp_r0->unk27 = 1;
}

s32 WifiClubGameEntry_Main(OverlayManager *overlayManager, u32 *arg1) {
    WifiClubGameEntryArgs *temp_r7;
    WifiClubGameEntry *temp_r4;
    s32 var_r6;

    temp_r4 = OverlayManager_GetData(overlayManager);
    temp_r7 = OverlayManager_GetArgs(overlayManager);
    switch (arg1[0]) {
    case 0:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_103);
        arg1[0]++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            arg1[0]++;
        }
        break;
    case 2:
        if (temp_r4->unk27 != 0) {
            WifiClubGameEntry_CheckConnectionLost(temp_r4, temp_r7, HEAP_ID_103);
        }
        if (temp_r4->unk4 != 0) {
            if (CommPlayerInfo_Exists() == 1) {
                WifiClubGameEntry_SendVoiceChatFlags(temp_r4);
                CommPlayerInfo_HostRedistribute();
                WifiClubGameEntry_RefuseLateApplicants(temp_r4);
            }
            var_r6 = sWifiClubGameEntryLeaderStates[temp_r4->unk5](temp_r4, temp_r7, HEAP_ID_103);
        } else {
            var_r6 = sWifiClubGameEntryMemberStates[temp_r4->unk5](temp_r4, temp_r7, HEAP_ID_103);
            if (CommPlayerInfo_Exists() == 1) {
                WifiClubGameEntry_AcceptAnnouncedMembers(temp_r4, HEAP_ID_103);
            }
        }
        if (WifiClubGameEntry_CheckMemberListChanged(temp_r4) != 0) {
            WifiClubGameEntry_DrawMemberList(temp_r4, temp_r7, HEAP_ID_103);
        }
        WifiClubGameEntry_UpdateAvatars(temp_r4);
        if (var_r6 == 1) {
            temp_r7->unk3 = temp_r4->unk7;
            temp_r7->unk4 = temp_r4->unk11;
            arg1[0]++;
        }
        break;
    case 3:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_103);
        arg1[0]++;
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            return 1;
        }
    }
    WifiClubGameEntry_AnimateBgPalette(&temp_r4->unk30);
    SpriteList_RenderAndAnimateSprites(temp_r4->unk30.spriteList);
    return 0;
}

s32 WifiClubGameEntry_Exit(OverlayManager *arg0) {
    WifiClubGameEntry *temp_r4;

    temp_r4 = OverlayManager_GetData(arg0);
    OverlayManager_GetArgs(arg0);
    Main_SetVBlankIntrCB(0, 0);
    WifiClubGameEntry_ClearDisconnectCallback(temp_r4);
    WifiClubGameEntry_DeleteAvatars(temp_r4);
    WifiClubGameEntry_StopVoiceChat(temp_r4);
    WifiClubGameEntry_FreeScreen(temp_r4, HEAP_ID_103);
    GF_DestroyVramTransferManager();
    OverlayManager_FreeData(arg0);
    Heap_Destroy(HEAP_ID_104);
    Heap_Destroy(HEAP_ID_103);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
    return 1;
}

void WifiClubGameEntry_OnJoinAnswer(WifiClubGameEntry *arg0, String *arg1) {
    if (arg0->unk4 == 0) {

        if (arg1->maxsize == Comm_GetLocalNetId()) {
            switch (arg1->magic) {
            case 1:
                arg0->unk13 = 1;
                return;
            case 0:
                arg0->unk13 = 2;
                return;

            case 2:
                arg0->unk13 = 3;
                return;
            }
        } else {
            switch (arg1->magic) {
            case 0:
                break;
            case 1:
                arg0->unk28[arg1->maxsize] = 1;
                return;
            case 2:
                break;
            default:
                return;
            }
            if (CommPlayerInfo_Exists() == 1) {
                CommPlayerInfo_ClearPlayer(arg1->maxsize);
            }
        }
    }
}

void WifiClubGameEntry_OnStartCommand(WifiClubGameEntry *arg0) {
    if (arg0->unk2C == 0) {
        arg0->unk2C = 1;
        LinkSync_StartSync(13);
    }
}

void WifiClubGameEntry_OnVoiceChatFlags(WifiClubGameEntry *arg0, String *arg1) {
    if (arg0->unk4 != 1) {
        memcpy(arg0->unk20, arg1, 4);
    }
}

void WifiClubGameEntry_VBlankCB(WifiClubGameEntry *arg0) {
    DoScheduledBgGpuUpdates(arg0->unk30.bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
}

s32 WifiClubGameEntry_IsLocalLeader(WifiClubGameEntry *arg0) {
    if (Comm_GetLocalNetId() == 0) {
        return 1;
    }
    return 0;
}

void WifiClubGameEntry_InitScreen(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_SetGfxBanks();
    InitBgAndScrnData(&arg0->unk30, heapID);
    InitWindowList(&arg0->unk30, arg1, heapID);
    InitStringAndMsgData(&arg0->unk30, arg1, heapID);
    WifiClubGameEntry_PrintTitle(&arg0->unk30, arg1, heapID);
    WifiClubGameEntry_DrawMemberList(arg0, arg1, heapID);
    InitSpritesAnd2DGfxResObjs(&arg0->unk30, heapID);
}

void WifiClubGameEntry_FreeScreen(WifiClubGameEntry *arg0, enum HeapID heapID) {
    FreeSpritesAnd2DGfxResObjs(&arg0->unk30);
    FreeStringAndMsgData(&arg0->unk30);
    FreeWindowList(&arg0->unk30, heapID);
    FreeBgAndScreenData(&arg0->unk30);
}

void WifiClubGameEntry_SetGfxBanks(void) {
    GraphicsBanks graphicsBanks = sWifiClubGameEntryGraphicsBanks;
    GfGfx_SetBanks(&graphicsBanks);
}

void InitBgAndScrnData(WifiClubGameEntryScreen *arg0, enum HeapID heapID) {
    arg0->bgConfig = BgConfig_Alloc(heapID);
    GraphicsModes graphicModes = sWifiClubGameEntryGraphicsModes;
    SetBothScreensModesAndDisable(&graphicModes);

    BgTemplate bgTemplate1 = sWifiClubGameEntryBgTemplateMain0;
    InitBgFromTemplate(arg0->bgConfig, GF_BG_LYR_MAIN_0, &bgTemplate1, 0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, heapID);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate bgTemplate2 = sWifiClubGameEntryBgTemplateMain1;
    InitBgFromTemplate(arg0->bgConfig, GF_BG_LYR_MAIN_1, &bgTemplate2, 0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, heapID);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate bgTemplate3 = sWifiClubGameEntryBgTemplateMain2;
    InitBgFromTemplate(arg0->bgConfig, GF_BG_LYR_MAIN_2, &bgTemplate3, 0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, heapID);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, GF_BG_LYR_MAIN_2);

    GfGfx_EngineATogglePlanes(8, 0);
    GfGfx_EngineATogglePlanes(16, 1);
    GfGfx_EngineBTogglePlanes(1, 0);
    GfGfx_EngineBTogglePlanes(2, 0);
    GfGfx_EngineBTogglePlanes(4, 0);
    GfGfx_EngineBTogglePlanes(8, 0);
    GfGfx_EngineBTogglePlanes(16, 0);

    NARC *narc = NARC_New(NARC_a_0_8_8, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 14, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_5_OFFSET, 32, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 13, arg0->bgConfig, GF_BG_LYR_MAIN_2, 0, 1536, 0, heapID);
    arg0->scrnDataRaw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 15, 0, &arg0->scrnData, heapID);
    arg0->plttDataRaw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 45, &arg0->plttData, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 46, arg0->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 47, arg0->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, heapID);
    WifiClubGameEntry_LoadBgPaletteFrame(arg0, 0);
    arg0->unk1F4 = 3;
    arg0->unk1F6 = 1;
    NARC_Delete(narc);
}

void FreeBgAndScreenData(WifiClubGameEntryScreen *arg0) {
    Heap_Free(arg0->scrnDataRaw);
    Heap_Free(arg0->plttDataRaw);
    FreeBgTilemapBuffer(arg0->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(arg0->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(arg0->bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(arg0->bgConfig);
}

void InitWindowList(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, heapID);

    Options *options = Save_PlayerData_GetOptionsAddr(arg1->unk8);
    LoadUserFrameGfx2(arg0->bgConfig, GF_BG_LYR_MAIN_1, 1, 2, Options_GetFrame(options), heapID);
    LoadUserFrameGfx1(arg0->bgConfig, GF_BG_LYR_MAIN_1, 31, 3, 0, heapID);
    LoadUserFrameGfx1(arg0->bgConfig, GF_BG_LYR_MAIN_2, 48, 4, 0, heapID);
    AddWindowParameterized(arg0->bgConfig, &arg0->windowList[0], GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 1, 40);
    FillWindowPixelBuffer(&arg0->windowList[0], 15);
    DrawFrameAndWindow2(&arg0->windowList[0], 1, 1, 2);
    AddWindowParameterized(arg0->bgConfig, &arg0->windowList[1], GF_BG_LYR_MAIN_1, 1, 1, 27, 2, 1, 148);
    FillWindowPixelBuffer(&arg0->windowList[1], 15);
    DrawFrameAndWindow1(&arg0->windowList[1], 1, 31, 3);
    AddWindowParameterized(arg0->bgConfig, &arg0->windowList[2], GF_BG_LYR_MAIN_1, 2, 16, 28, 2, 1, 202);
    FillWindowPixelBuffer(&arg0->windowList[2], 0);
    AddWindowParameterized(arg0->bgConfig, &arg0->windowList[3], GF_BG_LYR_MAIN_1, 1, 5, 20, 8, 1, 258);
    FillWindowPixelBuffer(&arg0->windowList[3], 15);
    DrawFrameAndWindow1(&arg0->windowList[3], 1, 31, 3);
    AddWindowParameterized(arg0->bgConfig, &arg0->windowList[4], GF_BG_LYR_MAIN_1, 23, 5, 7, 5, 1, 418);
    FillWindowPixelBuffer(&arg0->windowList[4], 15);
}

void FreeWindowList(WifiClubGameEntryScreen *arg0, enum HeapID heapID) {
    WifiClubGameEntry_ClearMessage(arg0);
    ClearFrameAndWindow2(&arg0->windowList[0], 0);
    RemoveWindow(&arg0->windowList[0]);
    EraseFrameAndWindow1(&arg0->windowList[1], 0);
    RemoveWindow(&arg0->windowList[1]);
    RemoveWindow(&arg0->windowList[2]);
    EraseFrameAndWindow1(&arg0->windowList[3], 0);
    RemoveWindow(&arg0->windowList[3]);
    EraseFrameAndWindow1(&arg0->windowList[4], 0);
    RemoveWindow(&arg0->windowList[4]);

    if (arg0->listMenu2D != NULL) {
        Clear2dMenuWindowAndDelete(arg0->listMenu2D, heapID);
    }
}

void InitStringAndMsgData(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    arg0->msgFmt = MessageFormat_New(heapID);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, heapID);
    arg0->string1 = String_New(256, heapID);
    arg0->string2 = String_New(256, heapID);
    arg0->printerId = 255;

    Options *options = Save_PlayerData_GetOptionsAddr(arg1->unk8);
    arg0->textFrameDelay = Options_GetTextFrameDelay(options);
}

void FreeStringAndMsgData(WifiClubGameEntryScreen *arg0) {
    String_Delete(arg0->string2);
    String_Delete(arg0->string1);
    DestroyMsgData(arg0->msgData);
    MessageFormat_Delete(arg0->msgFmt);
}

void WifiClubGameEntry_PrintTitle(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    switch (arg1->unk0) {
    case 0:
        break;
    case 1:
        BufferWiFiPlazaActivityName(arg0->msgFmt, 0, 0);
        break;
    case 2:
        BufferWiFiPlazaActivityName(arg0->msgFmt, 0, 1);
        break;
    case 3:
        BufferWiFiPlazaActivityName(arg0->msgFmt, 0, 2);
        break;
    }

    // Seeking Poffin cooks! OR // Seeking {STRVAR_3 0, 0, 0} players!
    ReadMsgDataIntoString(arg0->msgData, sWifiClubGameEntryTitleMsgIds[arg1->unk0], arg0->string2);
    StringExpandPlaceholders(arg0->msgFmt, arg0->string1, arg0->string2);
    AddTextPrinterParameterizedWithColor(&arg0->windowList[1], 0, arg0->string1, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), 0);
    ScheduleWindowCopyToVram(&arg0->windowList[1]);
    ScheduleWindowCopyToVram(&arg0->windowList[3]);

    // Voice Chat ON/OFF: X Button
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00128, arg0->string1);
    AddTextPrinterParameterizedWithColor(&arg0->windowList[2], 0, arg0->string1, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ScheduleWindowCopyToVram(&arg0->windowList[2]);
    ScheduleWindowCopyToVram(&arg0->windowList[0]);
}

void InitSpritesAnd2DGfxResObjs(WifiClubGameEntryScreen *arg0, enum HeapID heapID) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 31, 0, 126, 0, 31, heapID);
    ObjCharTransferTemplate objCharTransferTemplate = sWifiClubGameEntryObjCharTransferTemplate;
    objCharTransferTemplate.heapID = heapID;
    ObjCharTransfer_InitEx(&objCharTransferTemplate, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(4, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    arg0->spriteList = G2dRenderer_Init(4, &arg0->g2dRenderer, heapID);

    for (s32 i = 0; i < 4; i++) {
        arg0->gf2DGfxResManList[i] = Create2DGfxResObjMan(4, (GfGfxResType)i, heapID);
    }

    arg0->unk154 = ov42_02228010(4, heapID);
    arg0->unk158 = ov42_02228EDC(arg0->spriteList, 0, 4, 1, heapID);
}

void FreeSpritesAnd2DGfxResObjs(WifiClubGameEntryScreen *arg0) {
    ov42_02228F94(arg0->unk158);
    ov42_02228050(arg0->unk154);
    SpriteList_Delete(arg0->spriteList);

    for (s32 i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(arg0->gf2DGfxResManList[i]);
    }

    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void Wifi_LoadAndPrintTextToWindowWrapper(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 strno, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindow(arg0, arg1, strno, 0, heapID);
}

void Wifi_LoadAndPrintTextToWindow(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 strno, s32 arg3, enum HeapID heapID) {
    if ((arg0->printerId != 255) && (TextPrinterCheckActive(arg0->printerId) != 0)) {
        RemoveTextPrinter(arg0->printerId);
        arg0->printerId = 255;
    }
    FillWindowPixelBuffer(&arg0->windowList[0], 15);
    String *string = String_New(256, heapID);
    ReadMsgDataIntoString(arg0->msgData, strno, string);
    StringExpandPlaceholders(arg0->msgFmt, arg0->string1, string);
    arg0->printerId = AddTextPrinterParameterizedWithColor(&arg0->windowList[0], 1, arg0->string1, 0, 0, arg0->textFrameDelay, MAKE_TEXT_COLOR(1, 2, 15), 0);
    ScheduleWindowCopyToVram(&arg0->windowList[0]);
    String_Delete(string);
    Options *options = Save_PlayerData_GetOptionsAddr(arg1->unk8);
    LoadUserFrameGfx2(arg0->bgConfig, GF_BG_LYR_MAIN_1, 1, 2, Options_GetFrame(options), heapID);
    arg0->unk15 = arg3;
}

s32 WifiClubGameEntry_IsMessageFinished(WifiClubGameEntryScreen *arg0) {
    if (arg0->printerId == 255) {
        if (arg0->unk15 != 0) {
            arg0->unk15--;
            return 0;
        }
    } else if ((TextPrinterCheckActive(arg0->printerId) == 0) && (arg0->printerId != 255)) {
        arg0->printerId = 255;
    }
    if ((arg0->printerId == 255) && (arg0->unk15 == 0)) {
        return 1;
    }
    return 0;
}

void WifiClubGameEntry_ClearMessage(WifiClubGameEntryScreen *arg0) {
    if (arg0->printerId != 255) {
        if (TextPrinterCheckActive(arg0->printerId) != 0) {
            RemoveTextPrinter(arg0->printerId);
        }
        arg0->printerId = 255;
    }
    FillWindowPixelBuffer(&arg0->windowList[0], 15);
    ScheduleWindowCopyToVram(&arg0->windowList[0]);
    arg0->unk15 = 0;
}

// {STRVAR_1 3, 0, 0}\nID {STRVAR_1 54, 1, 0}
void WifiClubGameEntry_ShowApplicantInfo(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 arg2, enum HeapID heapID) {
    String *string1 = String_New(256, heapID);
    String *string2 = String_New(256, heapID);

    WifiClubGameEntry_BufferPlayerName(arg0, arg1, arg2, 0, heapID);
    WifiClubGameEntry_BufferTrainerId(arg0, arg1, arg2, 1, heapID);

    ReadMsgDataIntoString(arg0->msgData, msg_0778_00131, string1);
    StringExpandPlaceholders(arg0->msgFmt, string2, string1);
    FillWindowPixelBuffer(&arg0->windowList[4], 15);
    AddTextPrinterParameterizedWithColor(&arg0->windowList[4], 0, string2, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), 0);
    DrawFrameAndWindow1(&arg0->windowList[4], 1, 31, 3);
    ScheduleWindowCopyToVram(&arg0->windowList[4]);
    String_Delete(string1);
    String_Delete(string2);
}

void WifiClubGameEntry_HideApplicantInfo(WifiClubGameEntryScreen *arg0) {
    EraseFrameAndWindow1(&arg0->windowList[4], 1);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windowList[4]);
}

void WifiClubGameEntry_DrawMemberList(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntryScreen *temp_r6;

    if (CommPlayerInfo_Exists() != 0) {
        temp_r6 = &arg0->unk30;
        for (s32 i = 0; i < 4; i++) {
            if ((i == 0) && (CommPlayerInfo_IsReceivedOrAccepted(i) == 1)) {
                WifiClubGameEntry_DrawMember(arg0, temp_r6, arg1, i, heapID);
            } else if (CommPlayerInfo_IsAccepted(i) == 1) {
                WifiClubGameEntry_DrawMember(arg0, temp_r6, arg1, i, heapID);
            } else {
                WifiClubGameEntry_ClearMember(temp_r6, arg1, i, heapID);
            }
        }
    }
}

void WifiClubGameEntry_DrawMember(WifiClubGameEntry *arg0, WifiClubGameEntryScreen *arg1, WifiClubGameEntryArgs *arg2, s32 arg3, enum HeapID heapID) {
    String *string1 = String_New(256, heapID);
    String *string2 = String_New(256, heapID);
    WifiClubGameEntry_BufferPlayerName(arg1, arg2, arg3, 0, heapID);

    // {STRVAR_1 3, 0, 0}
    ReadMsgDataIntoString(arg1->msgData, msg_0778_00129, string1);
    StringExpandPlaceholders(arg1->msgFmt, string2, string1);

    u32 temp_r6 = arg3 * 16;
    FillWindowPixelRect(&arg1->windowList[3], 15, 0, temp_r6, 160, 16);
    AddTextPrinterParameterizedWithColor(&arg1->windowList[3], 0, string2, 0, temp_r6, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), 0);
    WifiClubGameEntry_BufferTrainerId(arg1, arg2, arg3, 0, heapID);

    // ID {STRVAR_1 54, 0, 0}
    ReadMsgDataIntoString(arg1->msgData, msg_0778_00130, string1);
    StringExpandPlaceholders(arg1->msgFmt, string2, string1);
    AddTextPrinterParameterizedWithColor(&arg1->windowList[3], 0, string2, 72, temp_r6, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), 0);
    ScheduleWindowCopyToVram(&arg1->windowList[3]);
    s32 var_r2;
    if (arg3 == Comm_GetLocalNetId()) {
        if (arg0->unk0->unk21 != 0) {
            var_r2 = 2;
        } else {
            var_r2 = 1;
        }
    } else if (arg0->unk20[arg3] != 0) {
        var_r2 = 2;
    } else {
        var_r2 = 1;
    }

    NNSG2dScreenData *screenData = arg1->scrnData;
    u32 temp_r4 = (arg3 << 1) + 5;
    CopyToBgTilemapRect(arg1->bgConfig, GF_BG_LYR_MAIN_2, 18, temp_r4, 2, 2, &screenData->rawData[0], var_r2 * 2, 0, screenData->screenWidth / 8, screenData->screenHeight / 8);
    BgTilemapRectChangePalette(arg1->bgConfig, GF_BG_LYR_MAIN_2, 18, temp_r4, 2, 2, 5);
    ScheduleBgTilemapBufferTransfer(arg1->bgConfig, GF_BG_LYR_MAIN_2);
    String_Delete(string1);
    String_Delete(string2);
}

void WifiClubGameEntry_ClearMember(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 arg2, enum HeapID heapID) {
    FillWindowPixelRect(&arg0->windowList[3], 15, 0, arg2 * 16, 160, 16);

    NNSG2dScreenData *screenData = arg0->scrnData;
    CopyToBgTilemapRect(arg0->bgConfig, GF_BG_LYR_MAIN_2, 18, arg2 * 2 + 5, 2, 2, &screenData->rawData[0], 0, 0, screenData->screenWidth / 8, screenData->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, GF_BG_LYR_MAIN_2);
    ScheduleWindowCopyToVram(&arg0->windowList[3]);
}

void WifiClubGameEntry_BufferPlayerName(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 arg2, s32 arg3, enum HeapID heapID) {
    BufferPlayersName(arg0->msgFmt, arg3, CommPlayerInfo_GetProfile(arg2));
}

void WifiClubGameEntry_BufferTrainerId(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 arg2, s32 arg3, enum HeapID heapID) {
    BufferIntegerAsString(arg0->msgFmt, arg3, PlayerProfile_GetTrainerID_VisibleHalf(CommPlayerInfo_GetProfile(arg2)), 5, PRINTING_MODE_LEADING_ZEROS, 1);
}

void WifiClubGameEntry_AnimateBgPalette(WifiClubGameEntryScreen *arg0) {
    if (arg0->unk1F4 <= 0) {
        WifiClubGameEntry_LoadBgPaletteFrame(arg0, arg0->unk1F6);
        arg0->unk1F4 = 3;
        arg0->unk1F6 = (arg0->unk1F6 + 1) % 18;
        return;
    }
    arg0->unk1F4--;
}

void WifiClubGameEntry_LoadBgPaletteFrame(WifiClubGameEntryScreen *arg0, s32 arg1) {
    GF_ASSERT(GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, 0, (sWifiClubGameEntryBgPaletteFrames[arg1] << 5) + arg0->plttData->pRawData, 32));
}

u8 WifiClubGameEntry_ToggleVoiceChat(WifiClubGameEntry *arg0, enum HeapID heapID) {
    arg0->unk0->unk22 = 1 - arg0->unk0->unk22;
    arg0->unk0->unk21 = arg0->unk0->unk22;
    ov00_021E70B8(arg0->unk0->unk21);
    ov00_021E6D60(arg0->unk0, 36);
    if (arg0->unk0->unk21 == 1) {
        WifiClubGameEntry_EnableVoiceChat(arg0);
    } else {
        WifiClubGameEntry_DisableVoiceChat(arg0);
    }
    return arg0->unk0->unk22;
}

void WifiClubGameEntry_CheckConnectionLost(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r7;
    s32 temp_r6;
    s32 var_r5;

    if (arg0->unk4 != 0) {
        if ((u8)(arg0->unk5 + 229) <= 2) {
            return;
        }
        if ((CommState_GetWifiMatchStatus() <= 2) && (WifiClubGameEntry_CountMembers(arg0) <= 1) && (WifiClubGameEntry_IsAnyoneConnected(arg0) == 0)) {
            return;
        }
    } else if ((arg0->unk5 == 15) || (arg0->unk5 == 16) || (arg0->unk5 == 17) || ((u8)(arg0->unk5 + 250) <= 1)) {
        return;
    }

    temp_r6 = WifiClubGameEntry_HasTimedOut(arg0);
    temp_r7 = WifiClubGameEntry_CheckMemberDisconnected(arg0);
    if (WifiClubGameEntry_CountMembers(arg0) > Comm_CountConnected()) {
        var_r5 = 1;
    } else {
        var_r5 = 0;
    }
    if ((CommState_GetWifiMatchStatus() >= 3) || (CommState_HasWifiFailed() != 0) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0) || (WifiClubGameEntry_IsAnyoneConnected(arg0) == 0) || (var_r5 == 1) || (temp_r6 == 1) || (temp_r7 == 1)) {
        if (arg0->unk4 != 0) {
            arg0->unk5 = 27;
            return;
        }
        arg0->unk5 = 15;
    }
}

void WifiClubGameEntry_CheckMatchFailed(WifiClubGameEntry *arg0) {
    if ((CommState_GetWifiMatchStatus() > 3) || (CommState_WasWifiDisconnected() != 0) || (CommState_HasWifiFailed() != 0)) {
        arg0->unk5 = 15;
    }
}

s32 WifiClubGameEntry_IsAnyoneConnected(WifiClubGameEntry *arg0) {
    if (Comm_CountConnected() > 0) {
        return 1;
    }
    return 0;
}

s32 WifiClubGameEntry_StartMatch(u32 arg0, u32 arg1) {
    s32 var_r5 = CommState_StartWifiMatch(arg0);
    if (var_r5 == 1) {
        switch (arg1) {
        case 0:
            sub_02039AF8();
            break;
        case 1:
        case 2:
        case 3:
            CommState_SetClubWifiLinkMode();
        }
    }
    return var_r5;
}

void WifiClubGameEntry_SetPlayingStatus(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1) {
    switch (arg1->unk0) {
    case 0:
        arg0->unk0->unk1B = 18;
        break;
    case 1:
        arg0->unk0->unk1B = 22;
        break;
    case 2:
        arg0->unk0->unk1B = 24;
        break;
    case 3:
        arg0->unk0->unk1B = 26;
        break;
    }
    ov00_021E6D60(arg0->unk0, 36);
}

s32 WifiClubGameEntry_StartVoiceChat(WifiClubGameEntry *arg0) {
    if (arg0->unk11 == 1) {
        return 0;
    }
    if (WifiClubGameEntry_IsAnyoneConnected(arg0) == 1) {
        WifiConnection_StartVoiceChat(104);
        arg0->unk11 = 1;
        return 1;
    }
    return 0;
}

s32 WifiClubGameEntry_StopVoiceChat(WifiClubGameEntry *arg0) {
    if (arg0->unk11 == 0) {
        return 0;
    }
    WifiConnection_StopVoiceChat();
    arg0->unk11 = 0;
    return 1;
}

void WifiClubGameEntry_EnableVoiceChat(WifiClubGameEntry *arg0) {
    if (WifiClubGameEntry_StartVoiceChat(arg0) != 0) {
        GF_GetCurrentPlayingBGM();
        sub_02005448();
    }
}

void WifiClubGameEntry_DisableVoiceChat(WifiClubGameEntry *arg0) {
    if (WifiClubGameEntry_StopVoiceChat(arg0) != 0) {
        GF_SndHandleSetInitialVolume(0, 120);
    }
}

s32 WifiClubGameEntry_CountMembers(WifiClubGameEntry *arg0) {
    return CommPlayerInfo_CountAccepted();
}

s32 WifiClubGameEntry_AcceptEarlierMembers(WifiClubGameEntry *arg0, enum HeapID heapID) {
    s32 var_r7 = 1;
    for (s32 i = Comm_GetLocalNetId() - 1; i >= 0; i--) {
        if (CommPlayerInfo_IsReceived(i) == 1) {
            WifiClubGameEntry_AcceptMember(arg0, i, heapID);
        } else if (CommPlayerInfo_IsAccepted(i) == 0) {
            var_r7 = 0;
        }
    }
    return var_r7;
}

void WifiClubGameEntry_RefusePendingApplicants(WifiClubGameEntry *arg0) {
    WifiClubGameEntryJoinAnswer var_r5;
    var_r5.unk2 = 0;
    for (s32 i = 1; i < 4; i++) {
        if (CommPlayerInfo_IsReceived(i) == 1) {
            var_r5.unk0 = i;
            GF_ASSERT(Comm_SendCommand(22, &var_r5, 8) == TRUE);
            CommPlayerInfo_ClearPlayer(i);
        }
    }
}

void WifiClubGameEntry_ReleaseAllMembers(WifiClubGameEntry *arg0) {
    WifiClubGameEntryJoinAnswer var_r5;
    var_r5.unk2 = 0;
    for (s32 i = 1; i < 4; i++) {
        if (CommPlayerInfo_IsReceivedOrAccepted(i) == 1) {
            var_r5.unk0 = i;
            GF_ASSERT(Comm_SendCommand(22, &var_r5, 8) == TRUE);
            CommPlayerInfo_ClearPlayer(i);
        }
    }
}

void WifiClubGameEntry_SetDisconnectCallback(WifiClubGameEntry *arg0) {
    WifiConnection_SetDisconnectCallback(WifiClubGameEntry_OnDisconnect, arg0);
}

void WifiClubGameEntry_ClearDisconnectCallback(WifiClubGameEntry *arg0) {
    WifiConnection_SetDisconnectCallback(0, 0);
}

s32 WifiClubGameEntry_CheckMemberDisconnected(WifiClubGameEntry *arg0) {
    s32 var_r7 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (arg0->unk14[i] == 1) {
            if (CommPlayerInfo_IsAccepted(arg0->unk18[i]) == 1) {
                var_r7 = 1;
            }
            arg0->unk14[i] = 0;
            arg0->unk18[i] = 0;
        }
    }
    return var_r7;
}

void WifiClubGameEntry_OnDisconnect(u8 arg0, WifiClubGameEntry *arg1, s32 arg2) {
    arg1->unk14[arg0] = 1;
    arg1->unk18[arg0] = arg0;
}

s32 WifiClubGameEntry_CheckMemberListChanged(WifiClubGameEntry *arg0) {
    s32 i;
    s32 ret = 0;
    for (i = 0; i < 4; i++) {
        if (arg0->unk1C[i] != arg0->unk20[i]) {
            ret = 1;
        }
        arg0->unk1C[i] = arg0->unk20[i];
    }

    if (arg0->unk24 != 0) {
        arg0->unk24 = 0;
        ret = 1;
    }

    return ret;
}

void WifiClubGameEntry_RequestMemberListRedraw(WifiClubGameEntry *arg0) {
    arg0->unk24 = 1;
}

void WifiClubGameEntry_AcceptMember(WifiClubGameEntry *arg0, s32 arg1, enum HeapID heapID) {
    if (CommPlayerInfo_IsAccepted(arg1) == 0) {
        CommPlayerInfo_Accept(arg1);
        arg0->unk25 = arg1;
        WifiClubGameEntry_AddMemberAvatar(arg0, arg1, heapID);
        WifiClubGameEntry_RequestMemberListRedraw(arg0);
    }
}

void WifiClubGameEntry_AddMemberAvatar(WifiClubGameEntry *arg0, s32 arg1, enum HeapID heapID) {
    UnkStruct_ov42_02122667 sp4;

    if (arg0->unk30.unk15C[arg1].unk0 == 0) {
        sp4 = sWifiClubGameEntryAvatarTemplates[arg1];

        if (arg1 != Comm_GetLocalNetId()) {
            PlayerProfile *playerProfile = CommPlayerInfo_GetProfile(arg1);
            GF_ASSERT(playerProfile);
            sp4.unkA = PlayerProfile_GetAvatar(playerProfile);
        } else if (arg0->unk0->unk1C[2] == 0) {
            sp4.unkA = 0;
        } else {
            sp4.unkA = 97;
        }
        arg0->unk30.unk15C[arg1].unk0 = ov42_022280B8(arg0->unk30.unk154, &sp4);
        if (ov42_02229010(arg0->unk30.unk158, sp4.unkA) == 0) {
            ov42_02228FE0(arg0->unk30.unk158, sp4.unkA, 2, heapID);
        }

        arg0->unk30.unk15C[arg1].unk4 = ov42_0222903C(arg0->unk30.unk158, arg0->unk30.unk15C[arg1].unk0, 0, heapID);
        ov42_02229200(arg0->unk30.unk15C[arg1].unk4, 0);
        arg0->unk30.unk15C[arg1].unk8++;
    }
}

void WifiClubGameEntry_UpdateAvatars(WifiClubGameEntry *arg0) {
    for (s32 i = 0; i < 4; i++) {
        WifiClubGameEntryAvatarFunc ov44_func = sWifiClubGameEntryAvatarFuncs[arg0->unk30.unk15C[i].unk8];
        ov44_func(arg0, &arg0->unk30.unk15C[i]);
    }
    ov42_0222807C(arg0->unk30.unk154);
    ov42_022290DC(arg0->unk30.unk158);
}

void WifiClubGameEntry_DeleteAvatars(WifiClubGameEntry *arg0) {
    ov42_0222901C(arg0->unk30.unk158);
    memset(&arg0->unk30.unk15C, 0, 48);
}

void WifiClubGameEntry_SendLeaderBusy(WifiClubGameEntry *arg0, s32 arg1) {
    WifiClubGameEntryJoinAnswer sp;
    sp.unk0 = arg1;
    sp.unk2 = 2;
    GF_ASSERT(Comm_SendCommand(22, &sp, 8) == 1);
}

void WifiClubGameEntry_RefuseLateApplicants(WifiClubGameEntry *arg0) {
    for (s32 i = 0; i < 4; i++) {
        if (((arg0->unk25 + 1) < i) && (CommPlayerInfo_IsReceived(i) == 1)) {
            WifiClubGameEntry_SendLeaderBusy(arg0, i);
            CommPlayerInfo_ClearPlayer(i);
        }
    }
}

void WifiClubGameEntry_AcceptAnnouncedMembers(WifiClubGameEntry *arg0, enum HeapID heapID) {
    for (s32 i = 0; i < 4; i++) {
        if ((arg0->unk28[i] == 1) && (CommPlayerInfo_IsReceivedOrAccepted(i) == 1)) {
            WifiClubGameEntry_AcceptMember(arg0, i, heapID);
            arg0->unk28[i] = 0;
            arg0->unk12 = 1;
        }
    }
}

void WifiClubGameEntry_ShowWaitingIcon(WifiClubGameEntry *arg0) {
    GF_ASSERT(!arg0->unk30.waitingIcon);
    arg0->unk30.waitingIcon = WaitingIcon_New(&arg0->unk30.windowList[0], 1);
}

void WifiClubGameEntry_HideWaitingIcon(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (arg0->unk30.waitingIcon != 0) {
        WaitingIcon_Delete(arg0->unk30.waitingIcon);
        arg0->unk30.waitingIcon = 0;
        Options *options = Save_PlayerData_GetOptionsAddr(arg1->unk8);
        LoadUserFrameGfx2(arg0->unk30.bgConfig, GF_BG_LYR_MAIN_1, 1, 2, Options_GetFrame(options), heapID);
    }
}

void WifiClubGameEntry_AvatarNone(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1) {
}

void WifiClubGameEntry_AvatarStartSpin(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1) {
    ov42_0222919C(arg1->unk4, 0);
    ov42_022291A0(arg1->unk4);
    ov42_02229200(arg1->unk4, 1);
    arg1->unk8 = 2;
    s32 idx = ov42_02228188(arg1->unk0, 6);
    arg1->unkA = sWifiClubGameEntryAvatarSpinFrames[idx] + 16;
}

void WifiClubGameEntry_AvatarUpdateSpin(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1) {
    ov42_022291AC(arg1->unk4);
    arg1->unkA = (arg1->unkA - 1);
    if (arg1->unkA <= 0) {
        ov42_022291B8(arg1->unk4);
        ov42_0222919C(arg1->unk4, 1);
        arg1->unk8 = 3;
    }
}

void WifiClubGameEntry_AvatarIdle(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1) {
}

void WifiClubGameEntry_SendVoiceChatFlags(WifiClubGameEntry *arg0) {
    int sp0 = 0;
    if (CommPlayerInfo_Exists() != 0) {
        int sp_count = CommPlayerInfo_CountAccepted();
        if ((CommPlayerInfo_HasNewEntries() != 0) && (Comm_ParentQueueHasCommand(5) == 0)) {
            sp0 = 1;
        }
        int var_r4 = 0;
        int var_r0;
        if (sp_count > 0) {
            do {
                if (var_r4 == 0) {
                    var_r0 = arg0->unk0->unk21;
                } else {
                    int temp_r7 = CommPlayerInfo_GetFriendIndex(var_r4);
                    GF_ASSERT(temp_r7 != 32);
                    var_r0 = arg0->unk0[(temp_r7 + 1)].unk21;
                }
                if (var_r0 != arg0->unk20[var_r4]) {
                    sp0 = 1;
                }
                arg0->unk20[var_r4] = var_r0;
                var_r4 += 1;
            } while (var_r4 < sp_count);
        }
        if (sp0 == 1) {
            u8 sp4[4];
            memcpy(&sp4, arg0->unk20, 4);
            Comm_SendCommand(24, &sp4, 4);
        }
    }
}

int WifiClubGameEntry_IsStartSynced(WifiClubGameEntry *arg0) {
    if (LinkSync_IsSynced(13) != 0) {
        return 1;
    }
    return 0;
}

void WifiClubGameEntry_StartTimeout(WifiClubGameEntry *arg0) {
    arg0->unk10 = 1;
    arg0->unkC = 1800;
}

void WifiClubGameEntry_StopTimeout(WifiClubGameEntry *arg0) {
    arg0->unk10 = 0;
}

s32 WifiClubGameEntry_HasTimedOut(WifiClubGameEntry *arg0) {
    if (arg0->unk10 == 0) {
        return 0;
    }
    if (arg0->unkC > 0) {
        arg0->unkC--;
        return 0;
    }
    return 1;
}

s32 WifiClubGameEntry_Leader_Start(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (arg0->unk0->unk21 == 1) {
        WifiClubGameEntry_EnableVoiceChat(arg0);
    }
    ov00_021E7220(0);
    CommState_SetErrorWatch(0, 1);
    WifiClubGameEntry_AddMemberAvatar(arg0, Comm_GetLocalNetId(), heapID);
    arg0->unk5 = 1;
    return 0;
}

// When the group is ready:\nA Button: Proceed   B Button: Cancel
s32 Wifi_PromptReadyMessage(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00107, heapID);
    arg0->unk5 = 2;
    return 0;
}

s32 WifiClubGameEntry_Leader_HandleInput(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    if (WifiClubGameEntry_IsAnyoneConnected(arg0) == 1) {
        temp_r0 = CommPlayerInfo_FindReceived();
        if ((temp_r0 != 255) && (arg0->unk6 != 0)) {
            if (CommPlayerInfo_GetFriendIndex(temp_r0) == 32) {
                WifiClubGameEntryJoinAnswer subroutine_arg0;
                subroutine_arg0.unk0 = temp_r0;
                subroutine_arg0.unk2 = 0;
                CommPlayerInfo_ClearPlayer(temp_r0);
                GF_ASSERT(Comm_SendCommand(22, &subroutine_arg0, 8) == TRUE);
            } else {
                arg0->unk6 = temp_r0;
            }
        }
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        PlaySE(SEQ_SE_DP_DECIDE);
        if (WifiClubGameEntry_IsAnyoneConnected(arg0) == 0) {
            arg0->unk5 = 6;
        } else if (arg1->unk1 <= WifiClubGameEntry_CountMembers(arg0)) {
            arg0->unk5 = 8;
        } else {
            arg0->unk5 = 6;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_DECIDE);
        arg0->unk5 = 22;
    } else if (arg0->unk6 != 255) {
        PlaySE(SEQ_SE_DP_DECIDE);
        arg0->unk5 = 3;
    } else if (gSystem.newKeys & PAD_BUTTON_X) {
        arg0->unk5 = 30;
        PlaySE(SEQ_SE_DP_DECIDE);
    }
    return 0;
}

// Someone wishes to join.\nAccept this Trainer as a member?
s32 Wifi_PromptUserJoinRequest(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_ShowApplicantInfo(&arg0->unk30, arg1, arg0->unk6, heapID);
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00108, heapID);
    arg0->unk5 = 4;
    return 0;
}

s32 WifiClubGameEntry_Leader_ShowAcceptYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk30.listMenu2D = Std_CreateYesNoMenu(arg0->unk30.bgConfig, &sWifiClubGameEntryYesNoWindowTemplate, 48, 4, heapID);
    arg0->unk5 = 5;
    return 0;
}

s32 WifiClubGameEntry_Leader_HandleAcceptYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk30.listMenu2D, heapID);
    if (temp_r0 != -1) {
        WifiClubGameEntryJoinAnswer subroutine_arg0;
        subroutine_arg0.unk0 = (s16)arg0->unk6;
        if (temp_r0 == 0) {
            subroutine_arg0.unk2 = 1;
            WifiClubGameEntry_AcceptMember(arg0, arg0->unk6, heapID);
            if (arg0->unk0->unk21 == 1) {
                WifiClubGameEntry_StopVoiceChat(arg0);
                WifiClubGameEntry_EnableVoiceChat(arg0);
            }
            if (arg1->unk2 <= WifiClubGameEntry_CountMembers(arg0)) {
                arg0->unk5 = 8;
            } else {
                arg0->unk5 = 1;
            }
        } else {
            subroutine_arg0.unk2 = 0;
            CommPlayerInfo_ClearPlayer(arg0->unk6);
            arg0->unk5 = 1;
        }
        GF_ASSERT(Comm_SendCommand(22, &subroutine_arg0, 8) == TRUE);
        arg0->unk6 = 255;
        WifiClubGameEntry_HideApplicantInfo(&arg0->unk30);
        arg0->unk30.listMenu2D = NULL;
    }
    return 0;
}

// There aren’t enough members.\r
s32 Wifi_PromptInsufficientMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00119, heapID);
    arg0->unk5 = 7;
    return 0;
}

s32 WifiClubGameEntry_Leader_WaitNotEnoughMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk5 = 19;
    return 0;
}

// Are these members OK?
s32 Wifi_PromptConfirmMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00111, heapID);
    arg0->unk5 = 9;
    return 0;
}

s32 WifiClubGameEntry_Leader_ShowConfirmYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk30.listMenu2D = Std_CreateYesNoMenu(arg0->unk30.bgConfig, &sWifiClubGameEntryYesNoWindowTemplate, 48, 4, heapID);
    arg0->unk5 = 10;
    return 0;
}

s32 WifiClubGameEntry_Leader_HandleConfirmYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk30.listMenu2D, heapID);
    if (temp_r0 != -1) {
        if (temp_r0 == 0) {
            arg0->unk5 = 11;
        } else if (arg1->unk2 <= WifiClubGameEntry_CountMembers(arg0)) {
            arg0->unk5 = 19;
        } else {
            arg0->unk5 = 1;
        }
        arg0->unk30.listMenu2D = NULL;
    }
    return 0;
}

// Communicating. Please stand by...
s32 WifiClubGameEntry_Leader_StartCommunicating(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    arg0->unk8 = 30;
    WifiClubGameEntry_RefusePendingApplicants(arg0);
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00102, heapID);
    WifiClubGameEntry_ShowWaitingIcon(arg0);
    WifiClubGameEntry_StartTimeout(arg0);
    arg0->unk5 = 12;
    ov00_021E7130();
    return 0;
}

s32 WifiClubGameEntry_Leader_WaitMembersConnected(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (Comm_CountConnected() != CommPlayerInfo_CountAccepted()) {
        WifiClubGameEntry_RefusePendingApplicants(arg0);
        return 0;
    }
    if (ov00_021E70F4() == 0) {
        return 0;
    }
    ov00_021E7220(1);
    arg0->unk5 = 13;
    return 0;
}

s32 WifiClubGameEntry_Leader_SendStartCommand(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (arg0->unk8 == 0) {
        if (Comm_SendCommand(23, NULL, 0) != 0) {
            arg0->unk5 = 14;
        }
    } else {
        arg0->unk8--;
    }
    return 0;
}

s32 WifiClubGameEntry_Leader_WaitStartSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (LinkSync_IsSynced(13) != 0) {
        WifiClubGameEntry_SetPlayingStatus(arg0, arg1);
        LinkSync_ClearPackets();
        LinkSync_StartSync(14);
        arg0->unk5 = 15;
    }
    return 0;
}

s32 WifiClubGameEntry_Leader_SendStatus(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (LinkSync_IsSynced(14) != 0) {
        s16 subroutine_arg0 = (s16)arg0->unk0->unk1B;
        if (LinkSync_SendPacket(Comm_GetLocalNetId(), &subroutine_arg0) == 1) {
            arg0->unk5 = 16;
        }
    }
    return 0;
}

s32 WifiClubGameEntry_Leader_CheckStatuses(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 i = 0;
    s32 temp_r7 = WifiClubGameEntry_CountMembers(arg0);
    s32 sp0 = Comm_GetLocalNetId();
    s32 var_r5 = 0;

    for (i = 0; i < temp_r7; i++) {
        if (sp0 != i) {
            u16 *temp_r0 = LinkSync_GetPacket(i);
            if (temp_r0 != NULL) {
                if (temp_r0[0] == arg0->unk0->unk1B) {
                    var_r5++;
                } else {
                    arg0->unk5 = 27;
                }
            }
        } else {
            var_r5++;
        }
    }
    if (var_r5 == temp_r7) {
        arg0->unk5 = 17;
    }
    return 0;
}

s32 WifiClubGameEntry_Leader_StartFinalSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    CommState_SetErrorWatch(1, 1);
    LinkSync_StartSync(18);
    arg0->unk5 = 18;
    return 0;
}

s32 WifiClubGameEntry_Leader_WaitFinalSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (LinkSync_IsSynced(18) != 0) {
        arg0->unk7 = 1;
        WifiClubGameEntry_HideWaitingIcon(arg0, arg1, heapID);
        return 1;
    }
    return 0;
}

// Stop looking for members?
s32 Wifi_PromptStopFindingMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00120, heapID);
    arg0->unk5 = 20;
    return 0;
}

s32 WifiClubGameEntry_Leader_ShowStopSeekingYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk30.listMenu2D = CreateYesNoMenu(arg0->unk30.bgConfig, &sWifiClubGameEntryYesNoWindowTemplate, 48U, 4, 1, heapID);
    arg0->unk5 = 21;
    return 0;
}

s32 WifiClubGameEntry_Leader_HandleStopSeekingYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk30.listMenu2D, heapID);
    if (temp_r0 != -1) {
        if (temp_r0 == 0) {
            arg0->unk5 = 22;
        } else if (WifiClubGameEntry_IsAnyoneConnected(arg0) != 0) {
            if (arg1->unk2 <= WifiClubGameEntry_CountMembers(arg0)) {
                arg0->unk5 = 8;
            } else {
                arg0->unk5 = 1;
            }
        } else {
            arg0->unk5 = 1;
        }
        arg0->unk30.listMenu2D = NULL;
    }
    return 0;
}

// If the leader drops out, the group will\nbe disbanded. Is that OK?
s32 Wifi_PromptDropAsLeader(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00121, heapID);
    arg0->unk5 = 23;
    return 0;
}

s32 WifiClubGameEntry_Leader_ShowDisbandYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk30.listMenu2D = CreateYesNoMenu(arg0->unk30.bgConfig, &sWifiClubGameEntryYesNoWindowTemplate, 48, 4, 1, heapID);
    arg0->unk5 = 24;
    return 0;
}

s32 WifiClubGameEntry_Leader_HandleDisbandYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk30.listMenu2D, heapID);
    if (temp_r0 != -1) {
        if (temp_r0 == 0) {
            arg0->unk5 = 25;
        } else if (WifiClubGameEntry_IsAnyoneConnected(arg0) != 0) {
            if (arg1->unk2 <= WifiClubGameEntry_CountMembers(arg0)) {
                arg0->unk5 = 8;
            } else {
                arg0->unk5 = 1;
            }
        } else {
            arg0->unk5 = 1;
        }
        arg0->unk30.listMenu2D = NULL;
    }
    return 0;
}

// The member invitations will be canceled.
s32 Wifi_PromptCancelInvites(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00122, heapID);
    arg0->unk8 = 30;
    arg0->unk5 = 26;
    return 0;
}

s32 WifiClubGameEntry_Leader_Cancel(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    temp_r0 = arg0->unk8 - 1;
    arg0->unk8 = temp_r0;
    if (temp_r0 > 0) {
        return 0;
    }
    arg0->unk7 = 0;
    CommState_LeaveWifiMatch();
    WifiClubGameEntry_ReleaseAllMembers(arg0);
    CommState_SetWifiClubLinkMode();
    CommState_SetErrorWatch(0, 1);
    return 1;
}

s32 WifiClubGameEntry_Leader_OnMemberDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (arg0->unk30.listMenu2D != NULL) {
        Clear2dMenuWindowAndDelete(arg0->unk30.listMenu2D, heapID);
        arg0->unk30.listMenu2D = NULL;
    }
    WifiClubGameEntry_HideApplicantInfo(&arg0->unk30);
    WifiClubGameEntry_HideWaitingIcon(arg0, arg1, heapID);
    arg0->unk5 = 28;
    return 0;
}

// A member had to drop out.\nPlease register from the start again.\r
s32 Wifi_PromptMemberDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00118, heapID);
    arg0->unk5 = 29;
    WifiClubGameEntry_ReleaseAllMembers(arg0);
    WifiClubGameEntry_SetPlayingStatus(arg0, arg1);
    if ((u32)(CommState_GetWifiMatchStatus() - 4) <= 1U) {
        ov00_021EDB1C();
    }
    return 0;
}

s32 WifiClubGameEntry_Leader_EndAfterMemberDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if ((u32)(CommState_GetWifiMatchStatus() - 4) <= 1) {
        ov00_021EDB1C();
    }
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    CommState_LeaveWifiMatch();
    CommState_SetWifiClubLinkMode();
    WifiClubGameEntry_ReleaseAllMembers(arg0);
    arg0->unk7 = 0;
    CommState_SetErrorWatch(0, 1);
    return 1;
}

s32 Wifi_PromptToggleVoiceChat(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 strno;

    if (arg0->unk0->unk21 == 1) {
        // Is it OK to turn off the Voice Chat?
        strno = msg_0778_00125;
    } else {
        // Is it OK to turn on the Voice Chat?
        strno = msg_0778_00124;
    }
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, strno, heapID);
    arg0->unk5 = 31;
    return 0;
}

s32 WifiClubGameEntry_Leader_ShowVoiceChatYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk30.listMenu2D = Std_CreateYesNoMenu(arg0->unk30.bgConfig, &sWifiClubGameEntryYesNoWindowTemplate, 48, 4, heapID);
    arg0->unk5 = 32;
    return 0;
}

s32 WifiClubGameEntry_Leader_HandleVoiceChatYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk30.listMenu2D, heapID);
    if (temp_r0 != -1) {
        if (temp_r0 == 0) {
            WifiClubGameEntry_ToggleVoiceChat(arg0, heapID);
            WifiClubGameEntry_RequestMemberListRedraw(arg0);
        }
        arg0->unk5 = 1;
        arg0->unk30.listMenu2D = NULL;
    }
    return 0;
}

s32 WifiClubGameEntry_Member_Start(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    CommPlayerInfo_SendLocal();
    CommState_SetErrorWatch(0, 1);
    WifiClubGameEntry_StartTimeout(arg0);
    arg0->unk5 = 1;
    return 0;
}

s32 WifiClubGameEntry_Member_WaitLeaderInfo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    if (Comm_IsNetIdConnected(0) == 0) {
        WifiClubGameEntry_StopTimeout(arg0);
        arg0->unk5 = 15;
    } else if (CommPlayerInfo_IsAccepted(0) != 0) {
        WifiClubGameEntry_StopTimeout(arg0);
        arg0->unk5 = 2;
    }
    return 0;
}

// Awaiting {STRVAR_1 3, 0, 0}’s response...
s32 Wifi_PromptAwaitingResponse(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    WifiClubGameEntry_BufferPlayerName(&arg0->unk30, arg1, 0, 0, heapID);
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00114, heapID);
    WifiClubGameEntry_ShowWaitingIcon(arg0);
    arg0->unk5 = 3;
    return 0;
}

s32 WifiClubGameEntry_Member_WaitJoinAnswer(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    u8 temp_r0;

    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    if (arg0->unk13 != 0) {
        WifiClubGameEntry_HideWaitingIcon(arg0, arg1, heapID);
        temp_r0 = arg0->unk13;
        switch (temp_r0) {
        case 1:
            arg0->unk5 = 4;
            break;
        case 2:
            arg0->unk5 = 6;
            arg0->unk2D = 116;
            break;
        case 3:
            arg0->unk5 = 6;
            arg0->unk2D = 132;
            break;
        }
    }
    return 0;
}

// {STRVAR_1 3, 0, 0} approved your\nmember-registration request.
s32 WifiClubGameEntry_Member_PrintApproved(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_BufferPlayerName(&arg0->unk30, arg1, 0, 0, heapID);

    Wifi_LoadAndPrintTextToWindow(&arg0->unk30, arg1, msg_0778_00115, 30, heapID);
    arg0->unk5 = 5;
    WifiClubGameEntry_AcceptMember(arg0, Comm_GetLocalNetId(), HEAP_ID_103);
    return 0;
}

s32 WifiClubGameEntry_Member_WaitApproved(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk5 = 8;
    return 0;
}

s32 WifiClubGameEntry_Member_PrintRefused(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_BufferPlayerName(&arg0->unk30, arg1, 0, 0, heapID);
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, arg0->unk2D, heapID);
    arg0->unk5 = 7;
    CommState_LeaveWifiMatch();
    CommState_SetWifiClubLinkMode();
    return 0;
}

s32 WifiClubGameEntry_Member_EndRefused(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk7 = 0;
    CommState_SetErrorWatch(0, 1);
    return 1;
}

// Awaiting other members.
s32 Wifi_PromptAwaitingMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00123, heapID);
    WifiClubGameEntry_ShowWaitingIcon(arg0);
    arg0->unk5 = 9;
    if (arg0->unk0->unk21 == 1) {
        WifiClubGameEntry_EnableVoiceChat(arg0);
    }
    return 0;
}

s32 WifiClubGameEntry_Member_WaitAwaitingMessage(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk5 = 10;
    return 0;
}

// Communicating. Please stand by...
s32 WifiClubGameEntry_Member_WaitStart(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    if (arg0->unk12 != 0) {
        arg0->unk12 = 0U;
        if (arg0->unk0->unk21 == 1) {
            WifiClubGameEntry_StopVoiceChat(arg0);
            WifiClubGameEntry_EnableVoiceChat(arg0);
        }
        WifiClubGameEntry_RequestMemberListRedraw(arg0);
    }
    if (WifiClubGameEntry_IsStartSynced(arg0) == 1) {
        LinkSync_ClearPackets();
        LinkSync_StartSync(14);
        WifiClubGameEntry_HideWaitingIcon(arg0, arg1, heapID);
        Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00102, heapID);
        WifiClubGameEntry_StartTimeout(arg0);
        arg0->unk8 = 300;
        WifiClubGameEntry_ShowWaitingIcon(arg0);
        arg0->unk5 = 11;
    } else if (gSystem.newKeys & PAD_BUTTON_X) {
        WifiClubGameEntry_HideWaitingIcon(arg0, arg1, heapID);
        arg0->unk5 = 18;
    }
    return 0;
}

s32 WifiClubGameEntry_Member_SendStatus(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    if (LinkSync_IsSynced(14) != 0) {
        s16 subroutine_arg0 = (s16)arg0->unk0->unk1B;
        if (WifiClubGameEntry_CountMembers(arg0) != Comm_CountConnected()) {
            temp_r0 = arg0->unk8 - 1;
            arg0->unk8 = temp_r0;
            if (temp_r0 <= 0) {
                arg0->unk5 = 15;
            }
            return 0;
        }
        if (LinkSync_SendPacket(Comm_GetLocalNetId(), &subroutine_arg0) == 1) {
            arg0->unk5 = 12;
        }
    }
    return 0;
}

s32 WifiClubGameEntry_Member_CheckStatuses(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 i = 0;
    s32 temp_r7 = WifiClubGameEntry_CountMembers(arg0);
    s32 sp0 = Comm_GetLocalNetId();
    s32 var_r5 = 0;

    for (i = 0; i < temp_r7; i++) {
        if (sp0 != i) {
            u16 *temp_r0 = LinkSync_GetPacket(i);
            if (temp_r0 != NULL) {
                if (temp_r0[0] == arg0->unk0->unk1B) {
                    var_r5++;
                } else {
                    arg0->unk5 = 0xF;
                }
            }
        } else {
            var_r5++;
        }
    }

    if (var_r5 == temp_r7) {
        arg0->unk5 = 0xD;
    }
    return 0;
}

s32 WifiClubGameEntry_Member_StartFinalSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    CommState_SetErrorWatch(1, 1);
    LinkSync_StartSync(18);
    arg0->unk5 = 14;
    return 0;
}

s32 WifiClubGameEntry_Member_WaitFinalSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (LinkSync_IsSynced(18) != 0) {
        WifiClubGameEntry_HideWaitingIcon(arg0, arg1, heapID);
        arg0->unk7 = 1;
        return 1;
    }
    return 0;
}

s32 WifiClubGameEntry_Member_OnDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (arg0->unk30.listMenu2D != NULL) {
        Clear2dMenuWindowAndDelete(arg0->unk30.listMenu2D, heapID);
        arg0->unk30.listMenu2D = NULL;
    }
    WifiClubGameEntry_HideApplicantInfo(&arg0->unk30);
    WifiClubGameEntry_HideWaitingIcon(arg0, arg1, heapID);
    arg0->unk5 = 16;
    return 0;
}

// A member had to drop out.\nPlease register from the start again.\r
s32 Wifi_PromptMemberDrop(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00118, heapID);
    arg0->unk5 = 17;
    return 0;
}

s32 WifiClubGameEntry_Member_EndAfterDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    CommState_LeaveWifiMatch();
    CommState_SetWifiClubLinkMode();
    arg0->unk7 = 0;
    CommState_SetErrorWatch(0, 1);
    return 1;
}

s32 Wifi_PromptToggleVoiceChat_(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 strno;

    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    if (arg0->unk0->unk21 == 1) {
        // Is it OK to turn off the Voice Chat?
        strno = msg_0778_00125;
    } else {
        // Is it OK to turn on the Voice Chat?
        strno = msg_0778_00124;
    }
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, strno, heapID);
    arg0->unk5 = 19;
    return 0;
}

s32 WifiClubGameEntry_Member_ShowVoiceChatYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    if (WifiClubGameEntry_IsStartSynced(arg0) == 1) {
        arg0->unk5 = 10;
        WifiClubGameEntry_ClearMessage(&arg0->unk30);
        return 0;
    }
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk30.listMenu2D = Std_CreateYesNoMenu(arg0->unk30.bgConfig, &sWifiClubGameEntryYesNoWindowTemplate, 48, 4, heapID);
    arg0->unk5 = 20;
    return 0;
}

s32 WifiClubGameEntry_Member_HandleVoiceChatYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;

    WifiClubGameEntry_AcceptEarlierMembers(arg0, heapID);
    if (WifiClubGameEntry_IsStartSynced(arg0) == 1) {
        arg0->unk5 = 10;
        WifiClubGameEntry_ClearMessage(&arg0->unk30);
        Clear2dMenuWindowAndDelete(arg0->unk30.listMenu2D, heapID);
        arg0->unk30.listMenu2D = 0;
        return 0;
    }
    temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk30.listMenu2D, heapID);
    if (temp_r0 != -1) {
        if (temp_r0 == 0) {
            WifiClubGameEntry_ToggleVoiceChat(arg0, heapID);
            WifiClubGameEntry_RequestMemberListRedraw(arg0);
        }
        arg0->unk5 = 8;
        arg0->unk30.listMenu2D = 0;
    }
    return 0;
}

// The leader appears busy.\nPlease apply again.\r
s32 WifiClubGameEntry_Member_PrintLeaderBusy(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    arg0->unk26 = CommPlayerInfo_GetFriendIndex(0);
    GF_ASSERT(arg0->unk26 != 32);
    WifiClubGameEntry_BufferPlayerName(&arg0->unk30, arg1, 0, 0, heapID);
    CommState_LeaveWifiMatch();
    arg0->unk27 = 0;
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00132, heapID);
    arg0->unk5 = 22;
    return 0;
}

s32 WifiClubGameEntry_Member_ShowApplyAgainYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    arg0->unk30.listMenu2D = Std_CreateYesNoMenu(arg0->unk30.bgConfig, &sWifiClubGameEntryYesNoWindowTemplate, 48, 4, heapID);
    arg0->unk5 = 23;
    return 0;
}

s32 WifiClubGameEntry_Member_HandleApplyAgainYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    s32 temp_r0;
    s8 var_r0;

    temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk30.listMenu2D, heapID);
    if (temp_r0 != -1) {
        if (temp_r0 == 0) {
            var_r0 = 24;
        } else {
            var_r0 = 29;
        }
        arg0->unk5 = var_r0;
        arg0->unk30.listMenu2D = 0;
    }
    return 0;
}

// Communicating with {STRVAR_1 3, 0, 0}...
s32 WifiClubGameEntry_Member_PrintContacting(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    Wifi_LoadAndPrintTextToWindowWrapper(&arg0->unk30, arg1, msg_0778_00017, heapID);
    WifiClubGameEntry_ShowWaitingIcon(arg0);
    arg0->unk8 = 90;
    arg0->unk5 = 25;
    return 0;
}

s32 WifiClubGameEntry_Member_ApplyAgain(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (arg0->unk8 > 0) {
        arg0->unk8 = arg0->unk8 - 1;
        return 0;
    }
    if (WifiClubGameEntry_IsMessageFinished(&arg0->unk30) == 0) {
        return 0;
    }
    if (WifiClubGameEntry_StartMatch(arg0->unk26, arg1->unk0) != 0) {
        arg0->unk5 = 26;
    }
    return 0;
}

s32 WifiClubGameEntry_Member_WaitReconnected(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    WifiClubGameEntry_CheckMatchFailed(arg0);
    if (CommState_GetWifiMatchStatus() == 1 && Comm_IsNetIdConnected(0) == 1) {
        sub_020378E4(0);
        WifiClubGameEntry_HideWaitingIcon(arg0, arg1, heapID);
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, heapID);
        arg0->unk5 = 27;
    }
    return 0;
}

s32 WifiClubGameEntry_Member_RebuildScreen(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    u8 *temp_r0;

    if (IsPaletteFadeFinished()) {
        CommPlayerInfo_Init(arg1->unk8, 0);
        arg0->unk27 = 1;
        WifiClubGameEntry_DeleteAvatars(arg0);
        WifiClubGameEntry_FreeScreen(arg0, heapID);
        arg0 = arg0;
        memset(&arg0->unk30, 0, 504);
        WifiClubGameEntry_SetDisconnectCallback(arg0);
        WifiClubGameEntry_InitScreen(arg0, arg1, heapID);
        WirelessStatusIcon_Create();
        arg0->unk12 = 0;
        arg0->unk13 = 0;
        temp_r0 = arg0->unk14;
        arg0->unk14[0] = 0;
        temp_r0[1] = 0;
        temp_r0[2] = 0;
        temp_r0[3] = 0;
        temp_r0 = arg0->unk18;
        arg0->unk18[0] = 0;
        temp_r0[1] = 0;
        temp_r0[2] = 0;
        temp_r0[3] = 0;
        MI_CpuFill8(&arg0->unk1C, 1, 4);
        MI_CpuFill8(&arg0->unk20, 1, 4);
        temp_r0 = arg0->unk28;
        arg0->unk24 = 0;
        arg0->unk26 = 0;
        arg0->unk28[0] = 0;
        temp_r0[1] = 0;
        temp_r0[2] = 0;
        temp_r0[3] = 0;
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, heapID);
        arg0->unk5 = 28;
    }
    return 0;
}

s32 WifiClubGameEntry_Member_WaitFadeIn(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    if (IsPaletteFadeFinished() != 0) {
        arg0->unk5 = 0;
    }
    return 0;
}

s32 WifiClubGameEntry_Member_Cancel(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID) {
    CommState_LeaveWifiMatch();
    CommState_SetWifiClubLinkMode();
    arg0->unk7 = 0;
    CommState_SetErrorWatch(0, 1);
    return 1;
}
