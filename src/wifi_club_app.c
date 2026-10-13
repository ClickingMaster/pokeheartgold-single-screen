#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0778.h"

#include "bg_window.h"
#include "dwcaccount.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "obj_pltt_transfer.h"
#include "overlay_00_thumb.h"
#include "overlay_44_02232E9C.h"
#include "party.h"
#include "poke_overlay.h"
#include "pokedex.h"
#include "pokemon.h"
#include "render_text.h"
#include "render_window.h"
#include "save_frontier.h"
#include "save_wifi_history.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_system.h"
#include "sprite_transfer.h"
#include "sys_flags.h"
#include "system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "sound_playback.h"
#include "sprite_resource.h"
#include "oam_manager.h"
#include "font_sprite.h"
#include "touchpad.h"
#include "friend_roster.h"
#include "frontier_save_access.h"
#include "save_wii_message.h"
#include "communication_player_state.h"
#include "comm_wireless.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "wifi_club.h"
#include "network_overlay_loader.h"
#include "unk_020971F8.h"
#include "vram_transfer_manager.h"

static const u8 sProfileButtonPressAnims[4] = { 0, 2, 4, 0 };

static const u8 sRosterSlotTextY[4] = { 8, 7, 5, 7 };

static const u8 sRosterSelectAnimDurations[4] = { 0, 1, 2, 1 };

static const u8 sProfileButtonIdleAnims[4] = { 1, 3, 5, 0 };

static const s8 sCloseTextYOffsets[5] = { 248, 247, 246, 246, 247 };

static const WindowTemplate sYesNoPromptWindowTemplate = {
    .bgId = 2,
    .left = 25,
    .top = 10,
    .width = 6,
    .height = 8,
    .palette = 14,
    .baseTile = 81,
};

static const WindowTemplate sConnectPromptWindowTemplate = {
    .bgId = 2,
    .left = 25,
    .top = 10,
    .width = 6,
    .height = 8,
    .palette = 14,
    .baseTile = 81,
};

static const WindowTemplate sUserInfoPromptWindowTemplate = {
    .bgId = 1,
    .left = 25,
    .top = 16,
    .width = 6,
    .height = 8,
    .palette = 14,
    .baseTile = 393,
};

static const WindowTemplate sYesNoMenuWindowTemplate = {
    .bgId = 2,
    .left = 25,
    .top = 13,
    .width = 6,
    .height = 4,
    .palette = 13,
    .baseTile = 81,
};

static const TouchscreenHitbox sProfileButtonHitboxes[3] = {
    { .rect = { .top = 160, .bottom = 192, .left = 8, .right = 64 } },
    { .rect = { .top = 160, .bottom = 192, .left = 80, .right = 176 } },
    { .rect = { .top = 160, .bottom = 192, .left = 192, .right = 248 } },
};

static const WifiClubMenuItem sApplyMenuItems[2] = {
    { .strno = 39, .value = 1          },
    { .strno = 41, .value = 0xFFFFFFFE },
};

static const GraphicsModes sWifiClubGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const ObjCharTransferTemplate sWifiClubObjCharTransferTemplate = {
    .maxTasks = 20,
    .sizeMain = 0x20000,
    .sizeSub = 0x4000,
    .heapID = HEAP_ID_53,
};

static const u32 sFrontierStreakFlagStats[6] = { 100, 102, 104, 108, 106, 110 };

static const BgTemplate sWifiClubBgTemplate_Main0 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_512x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_512x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

static const BgTemplate sWifiClubBgTemplate_Main1 = {
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

static const BgTemplate sWifiClubBgTemplate_Main3 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x18000,
    .bgExtPltt = GX_BG_EXTPLTT_23,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

static const BgTemplate sWifiClubBgTemplate_Sub0 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

static const BgTemplate sWifiClubBgTemplate_Sub1 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

static const BgTemplate sWifiClubBgTemplate_Sub2 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

static const BgTemplate sWifiClubBgTemplate_Main2 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xd800,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_23,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

static const BgTemplate sWifiClubBgTemplate_Sub3 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x18000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = 0,
};

static const TouchscreenHitbox sRosterSlotHitboxes[8] = {
    { .rect = { .top = 0, .bottom = 47, .left = 0, .right = 119 } },
    { .rect = { .top = 48, .bottom = 95, .left = 0, .right = 119 } },
    { .rect = { .top = 96, .bottom = 143, .left = 0, .right = 119 } },
    { .rect = { .top = 144, .bottom = 191, .left = 0, .right = 119 } },
    { .rect = { .top = 0, .bottom = 47, .left = 128, .right = 255 } },
    { .rect = { .top = 48, .bottom = 95, .left = 128, .right = 255 } },
    { .rect = { .top = 96, .bottom = 143, .left = 128, .right = 255 } },
    { .rect = { .top = 144, .bottom = 191, .left = 128, .right = 255 } },
};

static const ListMenuTemplate sBattleSubmenuTemplate = {
    .items = NULL,
    .moveCursorFunc = WifiClub_ListMenuCursorCallback,
    .itemPrintFunc = NULL,
    .window = NULL,
    .totalItems = 4,
    .maxShowed = 4,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 1,
    .fillValue = 15,
    .cursorShadowPal = 2,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = 0,
    .cursorKind = 0,
    .data = NULL,
};

static const ListMenuTemplate sPlazaGameSubmenuTemplate = {
    .items = NULL,
    .moveCursorFunc = WifiClub_ListMenuCursorCallback,
    .itemPrintFunc = NULL,
    .window = NULL,
    .totalItems = 4,
    .maxShowed = 4,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 1,
    .fillValue = 15,
    .cursorShadowPal = 2,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = 0,
    .cursorKind = 0,
    .data = NULL,
};

static const ListMenuTemplate sApplyMenuTemplate = {
    .items = NULL,
    .moveCursorFunc = WifiClub_ListMenuCursorCallback,
    .itemPrintFunc = NULL,
    .window = NULL,
    .totalItems = 2,
    .maxShowed = 2,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 1,
    .fillValue = 15,
    .cursorShadowPal = 2,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = 0,
    .cursorKind = 0,
    .data = NULL,
};

static const GraphicsBanks sWifiClubGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_128_B,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

WifiClubMenuItem WifiClubSeekItem_PlazaGames = { .strno = msg_0778_00034, .value = 29 };
WifiClubMenuItem WifiClubSeekItem_Cancel = { .strno = msg_0778_00041, .value = LIST_CANCEL };
WifiClubMenuItem WifiClubSeekItem_Frontier = { .strno = msg_0778_00032, .value = 21 };
WifiClubMenuItem WifiClubSeekItem_Cook = { .strno = msg_0778_00030, .value = 19 };

WifiClubProfilePageFunc WifiClubProfilePageFuncs[7] = {
    WifiClub_DrawProfileSummary,
    WifiClub_DrawProfilePlazaGames,
    WifiClub_DrawProfileBattleTower,
    WifiClub_DrawProfileBattleFactory,
    WifiClub_DrawProfileBattleCastle,
    WifiClub_DrawProfileBattleHall,
    WifiClub_DrawProfileBattleArcade,
};

ListMenuTemplate WifiClubSeekMenuTemplate = {
    .items = NULL,
    .moveCursorFunc = WifiClub_ListMenuCursorCallback,
    .itemPrintFunc = NULL,
    .window = NULL,
    .totalItems = 3,
    .maxShowed = 3,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 1,
    .fillValue = 0xF,
    .cursorShadowPal = 2,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = 0,
    .cursorKind = 0,
    .data = NULL
};

WifiClubMenuItem WifiClubPlazaGameItems[4] = {
    { .strno = msg_0778_00071, .value = 23          },
    { .strno = msg_0778_00071, .value = 25          },
    { .strno = msg_0778_00071, .value = 27          },
    { .strno = msg_0778_00041, .value = LIST_CANCEL }
};

WifiClubMenuItem WifiClubSingleBattleItems[4] = {
    { .strno = msg_0778_00095, .value = 11          },
    { .strno = msg_0778_00096, .value = 9           },
    { .strno = msg_0778_00097, .value = 10          },
    { .strno = msg_0778_00041, .value = LIST_CANCEL }
};

WifiClubMenuItem WifiClubDoubleBattleItems[4] = {
    { .strno = msg_0778_00098, .value = 14          },
    { .strno = msg_0778_00099, .value = 12          },
    { .strno = msg_0778_00100, .value = 13          },
    { .strno = msg_0778_00041, .value = LIST_CANCEL }
};

WifiClubMenuItem WifiClubSeekMenuItems[10] = {
    { .strno = msg_0778_00093, .value = 0  },
    { .strno = msg_0778_00094, .value = 1  },
    { .strno = msg_0778_00028, .value = 15 },
    { .strno = 0,              .value = 0  },
    { .strno = 0,              .value = 0  },
    { .strno = 0,              .value = 0  },
    { .strno = 0,              .value = 0  },
    { .strno = 0,              .value = 0  },
    { .strno = 0,              .value = 0  },
    { .strno = 0,              .value = 0  }
};

WifiClubStateFunc WifiClubStateFuncs[72] = {
    WifiClub_State_Start,
    WifiClub_State_CreateConnectPrompt,
    WifiClub_State_ConnectPrompt,
    WifiClub_State_AskUpdateUserInfo,
    WifiClub_State_UpdateUserInfoPrompt,
    WifiClub_State_ConfirmUpdateUserInfoPrompt,
    WifiClub_State_SaveUserInfoPrompt,
    WifiClub_State_WaitErrorButton,
    WifiClub_State_ErrorAcknowledged,
    WifiClub_State_GoToReconnectPrompt,
    WifiClub_State_ErrorThenAskReconnect,
    WifiClub_State_CreateReconnectPrompt,
    WifiClub_State_ReconnectPrompt,
    WifiClub_State_Reconnect,
    WifiClub_State_StartConnecting,
    WifiClub_State_Connecting,
    WifiClub_State_ShowUserInfoSaved,
    WifiClub_State_UserInfoSavedWaitButton,
    WifiClub_State_EnterRoom,
    WifiClub_State_Room,
    WifiClub_State_VoiceChatToggleMessage,
    WifiClub_State_WaitVoiceChatPartner,
    WifiClub_State_StartVoiceChat,
    WifiClub_State_VoiceChatStartMessage,
    WifiClub_State_VoiceChat,
    WifiClub_State_CreateEndVoiceChatPrompt,
    WifiClub_State_EndVoiceChatPrompt,
    WifiClub_State_MessageThenLeaveMatch,
    WifiClub_State_MessageThenReenterRoom,
    WifiClub_State_MessageThenReturnToRoom,
    WifiClub_State_Dummy30,
    WifiClub_State_Dummy31,
    WifiClub_State_Dummy32,
    WifiClub_State_ReloadRoom,
    WifiClub_State_Exit,
    WifiClub_State_ErrorThenDisconnect,
    WifiClub_State_OpenSeekMenu,
    WifiClub_State_SeekMenu,
    WifiClub_State_SeekSubmenu,
    WifiClub_State_AskCancelSeeking,
    WifiClub_State_CreateCancelSeekingPrompt,
    WifiClub_State_CancelSeekingPrompt,
    WifiClub_State_TalkToPlayer,
    WifiClub_State_OpenApplyMenu,
    WifiClub_State_ApplyMenu,
    WifiClub_State_WaitMatchAccepted,
    WifiClub_State_Dummy46,
    WifiClub_State_Dummy47,
    WifiClub_State_Contacted,
    WifiClub_State_SyncStart,
    WifiClub_State_SendStatus,
    WifiClub_State_ReceiveStatus,
    WifiClub_State_SendPlayerInfo,
    WifiClub_State_StartActivity,
    WifiClub_State_ToMessageWait,
    WifiClub_State_MessageWait,
    WifiClub_State_CloseMessage,
    WifiClub_State_CreateDisconnectPrompt,
    WifiClub_State_DisconnectPrompt,
    WifiClub_State_Disconnecting,
    WifiClub_State_Disconnected,
    WifiClub_State_CreateRematchPrompt,
    WifiClub_State_RematchPrompt,
    WifiClub_State_CreateVoiceChatModePrompt,
    WifiClub_State_VoiceChatModePrompt,
    WifiClub_State_WaitThenReenterRoom,
    WifiClub_State_Dummy66,
    WifiClub_State_Dummy67,
    WifiClub_State_StartRecruiting,
    WifiClub_State_PrepareSave,
    WifiClub_State_MatchTimeout,
    WifiClub_State_WriteSave,
};

static s32 sCurrentState[8];

FS_EXTERN_OVERLAY(OVY_42);

s32 WifiClub_HasBattleFrontier(WifiClubApp *arg0) {
    return Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(arg0->saveData), 2, 27);
}

s32 WifiClub_IsPoffinCookingAvailable(WifiClubApp *arg0) {
    return 0;
}

s32 WifiClub_ArePlazaGamesAvailable(WifiClubApp *arg0) {
    return 1;
}

u32 WifiClub_GetPlayerStatus(WifiClubApp *arg0, WifiClubPlayerStatus *arg1) {
    u32 var_r4 = arg1->status;
    if ((var_r4 - 20 <= 1) && (WifiClub_HasBattleFrontier(arg0) == 0)) {
        var_r4 = 29;
    }
    if ((var_r4 - 22 <= 5) && (WifiClub_ArePlazaGamesAvailable(arg0) == 0)) {
        var_r4 = 29;
    }
    if ((var_r4 - 18 <= 1) && (WifiClub_IsPoffinCookingAvailable(arg0) == 0)) {
        var_r4 = 29;
    }
    return var_r4;
}

s32 WifiClub_IsGroupActivityStatus(s32 arg0) {
    switch (arg0 - 18) {
    case 0:
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return 1;
    default:
        return 0;
    }
}

u32 WifiClub_GetGroupActivityChoice(u32 arg0) {
    switch (arg0 - 18) {
    case 0:
    case 1:
        return 11;
    case 4:
    case 5:
        return 13;
    case 8:
    case 9:
        return 15;
    case 6:
    case 7:
        return 14;
    default:
        GF_AssertFail();
        return 0;
    }
}

s32 WifiClub_IsSeekingBattleStatus(u32 arg0) {
    switch (arg0 - 9) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 1;
    default:
        return 0;
    }
}

s32 WifiClub_IsSeekingStatus(u32 arg0) {
    if (WifiClub_IsSeekingBattleStatus(arg0) != 0) {
        return 1;
    }
    switch (arg0 - 15) {
    case 0:
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
        return 1;
    default:
        return 0;
    }
    return 0;
}

s32 WifiClub_IsInBattleStatus(u32 arg0) {
    switch (arg0) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        return 1;
    default:
        return 0;
    }
}

s32 WifiClub_IsBusyStatus(u16 arg0) {
    if (WifiClub_IsInBattleStatus(arg0) != 0) {
        return 1;
    }
    switch (arg0) {
    case 1:
    case 8:
    case 18:
    case 20:
    case 22:
    case 24:
    case 26:
        return 1;
    default:
        return 0;
    }
}

s32 WifiClub_StatusNeedsTwoMons(u32 arg0) {
    u32 temp_r0;

    temp_r0 = arg0 - 12;
    switch (temp_r0) {
    case 0:
    case 1:
    case 2:
    case 3:
        return 1;
    default:
        return 0;
    }
}

u32 WifiClub_GetCounterpartStatus(u32 arg0) {
    u32 var_r1;

    var_r1 = 29;
    switch (arg0) {
    case 19:
        var_r1 = 18;
        break;
    case 21:
        var_r1 = 20;
        break;
    case 23:
        var_r1 = 22;
        break;
    case 27:
        var_r1 = 26;
        break;
    case 25:
        var_r1 = 24;
        break;
    case 15:
        var_r1 = 8;
        break;
    case 9:
        var_r1 = 2;
        break;
    case 10:
        var_r1 = 3;
        break;
    case 11:
        var_r1 = 4;
        break;
    case 12:
        var_r1 = 5;
        break;
    case 13:
        var_r1 = 6;
        break;
    case 14:
        var_r1 = 7;
        break;
    case 16:
        var_r1 = 1;
        break;
    case 18:
        var_r1 = 19;
        break;
    case 20:
        var_r1 = 21;
        break;
    case 22:
        var_r1 = 23;
        break;
    case 26:
        var_r1 = 27;
        break;
    case 24:
        var_r1 = 25;
        break;
    case 8:
        var_r1 = 15;
        break;
    case 2:
        var_r1 = 9;
        break;
    case 3:
        var_r1 = 10;
        break;
    case 4:
        var_r1 = 11;
        break;
    case 5:
        var_r1 = 12;
        break;
    case 6:
        var_r1 = 13;
        break;
    case 7:
        var_r1 = 14;
        break;
    case 1:
        var_r1 = 16;
        break;
    }
    return var_r1;
}

s32 WifiClub_CountBattleReadyMons(WifiClubApp *arg0) {
    Party *party = SaveArray_Party_Get(arg0->saveData);
    int partyCount = Party_GetCount(party);
    int i;
    int var_r5 = 0;

    for (i = 0; i < partyCount; i++) {
        Pokemon *pokemon = Party_GetMonByIndex(party, i);
        if ((GetMonData(pokemon, MON_DATA_HP, NULL) != 0) && (GetMonData(pokemon, MON_DATA_IS_EGG, NULL) == 0)) {
            var_r5 += 1;
        }
    }
    return var_r5;
}

void WifiClub_SetLinkModeForStatus(u32 arg0) {
    if ((arg0 - 18) <= 1) {
        sub_02039AF8();
        return;
    }
    if (WifiClub_IsGroupActivityStatus(arg0) != 0) {
        CommState_SetClubWifiLinkMode();
        return;
    }
    if (arg0 == 16) {
        CommState_SetWifiClubLinkMode();
        return;
    }
    if ((arg0 - 20) <= 1) {
        sub_020378E4(0);
        CommState_SetFrontierWifiLinkMode();
        return;
    }
    sub_020378E4(1);
    CommState_SetSingleBattleWifiLinkMode();
}

void WifiClub_SetupGraphics(WifiClubApp *arg0) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetKeyRepeatTimers(4, 8);
    NARC *temp_r0 = NARC_New(NARC_a_0_8_8, HEAP_ID_53);
    arg0->bgConfig = BgConfig_Alloc(HEAP_ID_53);
    arg0->msgFormat = MessageFormat_New(HEAP_ID_53);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_53);
    arg0->connectionMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_53);
    WifiClub_SetVramBanks();
    WifiClub_InitBgs(arg0->bgConfig);
    WifiClub_LoadBgGraphics(arg0, temp_r0);
    MI_CpuFill8(&arg0->paletteCycle, 0, sizeof(WifiClubPaletteCycle));

    NNSG2dPaletteData *sp2C;
    void *temp_r4 = GfGfxLoader_GetPlttDataFromOpenNarc(temp_r0, 5, &sp2C, HEAP_ID_53);
    MIi_CpuCopy16(sp2C->pRawData, *arg0->paletteCycle.basePalettes, 128);
    MIi_CpuCopy16(sp2C->pRawData, *arg0->paletteCycle.blendedPalettes, 128);
    Heap_Free(temp_r4);
    int i;
    int sp18 = 0;
    for (i = 0; i < 3; i++) {
        int j = 0;
        int sp10 = 0;
        int a = i + 1;
        while (TRUE) {
            GF_ASSERT(sp18 < 21);
            for (int k = 1; k < 16; k++) {
                BlendPalette(&arg0->paletteCycle.basePalettes[i][k], &arg0->paletteCycle.blendedPalettes[sp18][k], 1, j >> 8, arg0->paletteCycle.basePalettes[a][k]);
            }
            sp18++;
            if (sp10 == 1) {
                break;
            }
            j += 768;
            if (j >= 4096) {
                j = 4096;
                sp10 = 1;
            }
        }
    }
    DC_FlushRange(arg0->paletteCycle.blendedPalettes, 672);
    arg0->paletteCycle.active = 1;
    arg0->paletteCycle.targetScreens = 0;
    arg0->paletteCycle.task = SysTask_CreateOnVBlankQueue((SysTaskFunc)WifiClub_PaletteCycleTask, &arg0->paletteCycle, 20);
    Main_SetVBlankIntrCB((GFIntrCB)WifiClub_VBlankCB, arg0);
    WifiClub_InitObjTransfer();
    WifiClub_InitSpriteSystem(arg0, temp_r0);
    WifiClub_EnableObjPlane(arg0);
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    NARC_Delete(temp_r0);
}

void WifiClub_PaletteCycleTask(s32 arg0, WifiClubPaletteCycle *arg1) {
    if (arg1->active != 0) {
        arg1->frameToggle ^= 1;
        if ((1 & arg1->frameToggle) == 0) {
            if (arg1->targetScreens <= 1) {
                GX_LoadBGPltt(arg1->blendedPalettes[arg1->blendIndex], 0, 32);
            }
            u32 temp_r0 = arg1->targetScreens;
            if ((temp_r0 == 0) || (temp_r0 == 2)) {
                GXS_LoadBGPltt(arg1->blendedPalettes[arg1->blendIndex], 0, 32);
            }
            if (arg1->reverse == 0) {
                arg1->blendIndex++;
                if (arg1->blendIndex >= 21) {
                    arg1->blendIndex = 19;
                    arg1->reverse ^= 1;
                }
            } else {
                arg1->blendIndex--;
                if (arg1->blendIndex < 0) {
                    arg1->blendIndex = 1;
                    arg1->reverse ^= 1;
                }
            }
        }
    }
}

s32 WifiClub_Init(OverlayManager *overlayMananger, s32 *arg1) {
    switch (*arg1) {
    case 0:
        UnkStruct_ov44_0222A758_args *overlayArgs = OverlayManager_GetArgs(overlayMananger);
        if (CommState_IsWifi() != 0) {
            switch (overlayArgs->choice) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 12:
                Heap_Create(HEAP_ID_3, HEAP_ID_128, 0x10000);
                break;
            }
        } else {
            Heap_Create(HEAP_ID_3, HEAP_ID_128, 0x10000);
        }
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_42), OVY_LOAD_ASYNC);
        if (overlayArgs->choice == 1) {
            Heap_Create(HEAP_ID_3, HEAP_ID_53, 0x70000);
        } else {
            Heap_Create(HEAP_ID_3, HEAP_ID_53, 0xA0000);
        }
        WifiClubApp *data = OverlayManager_CreateAndGetData(overlayMananger, 4288, HEAP_ID_53);
        MI_CpuFill8(data, 0, 4288);
        GF_CreateVramTransferManager(32, HEAP_ID_53);
        data->textPrinterId = 8;
        data->saveData = overlayArgs->saveData;
        data->friendRoster = Save_FriendRoster_Get(data->saveData);
        data->startMode = overlayArgs->choice;
        data->exitChoice = 8;
        data->contactFriend = -1;
        GfGfx_EngineATogglePlanes(15, 0);
        GfGfx_EngineBTogglePlanes(15, 0);
        WifiClub_AllocStrings(data);
        WifiClub_SetupGraphics(data);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_53);
        if (CommState_IsWifi() != 0) {
            WirelessStatusIcon_Create();
        } else {
            LoadDwcOverlay();
            Comm_InitDwc(HEAP_ID_53);
        }
        GF_ASSERT(GF_TouchpadStartSampling(4) == 1);
        *arg1 += 1;
        break;
    case 1:
        OverlayManager_GetData(overlayMananger);
        *arg1 = 0;
        return 1;
    }
    return 0;
}

s32 WifiClub_Main(OverlayManager *overlayManager, s32 *arg1) {
    WifiClubApp *data = OverlayManager_GetData(overlayManager);
    int r1 = arg1[0];
    switch (r1) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *arg1 = 1;
        }
        break;
    case 1:
        int temp_r2 = data->state;
        WifiClubStateFunc temp_r6 = WifiClubStateFuncs[temp_r2];
        if (temp_r6 != NULL) {
            *sCurrentState = temp_r2;
            *arg1 = temp_r6(data, r1);
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            return 1;
        }
    }
    SpriteList *temp_r0 = data->spriteList;
    if (temp_r0 != NULL) {
        SpriteList_RenderAndAnimateSprites(temp_r0);
    }
    if (WifiClubRoom_IsInitialized(&data->room) == 1) {
        WifiClubRoom_Draw(&data->room);
    }
    return 0;
}

void WifiClub_TeardownGraphics(WifiClubApp *arg0) {
    Main_SetVBlankIntrCB(NULL, 0);
    if (arg0->paletteCycle.task) {
        SysTask_Destroy(arg0->paletteCycle.task);
        arg0->paletteCycle.task = NULL;
        arg0->paletteCycle.active = 0;
    }
    if (WifiClubRoom_IsInitialized(&arg0->room) == 1) {
        WifiClubRoom_Free(&arg0->room);
    }
    if (WifiClub_IsRosterScreenActive(arg0) == 1) {
        WifiClub_FreeRosterScreen(arg0);
    }
    WifiClub_FreeStatusIcons(&arg0->statusIcons);
    FontSystem_Delete(arg0->fontSystem);
    for (int i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(arg0->gfxResMan[i]);
    }
    SpriteList_Delete(arg0->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    WifiClub_RemoveWindows(arg0);
    WifiClub_FreeBgConfig(arg0->bgConfig);
    DestroyMsgData(arg0->connectionMsgData);
    DestroyMsgData(arg0->msgData);
    MessageFormat_Delete(arg0->msgFormat);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
}

s32 WifiClub_Exit(OverlayManager *overlayManager) {
    WifiClubApp *data = OverlayManager_GetData(overlayManager);
    WifiClub_TeardownGraphics(data);
    UnkStruct_ov44_0222A758_args *args = OverlayManager_GetArgs(overlayManager);
    args->choice = data->exitChoice;
    if ((data->exitChoice == 8) || (data->exitChoice == 10)) {
        UnloadDwcOverlay();
        TextFlags_SetCanTouchSpeedUpPrint(0);
    } else {
        args->partnerFriendIndex = WifiConnection_GetTargetFriendIndex();
    }
    WifiClub_FreeStrings(data);
    OverlayManager_FreeData(overlayManager);
    GF_TouchpadStopSampling();
    GF_DestroyVramTransferManager();
    Heap_Destroy(HEAP_ID_53);
    switch (data->exitChoice) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
        Heap_Destroy(HEAP_ID_128);
        break;
    }
    if (WifiClub_IsRoomBgmPlaying() == 1) {
        GF_SndHandleSetInitialVolume(0, 120);
    }
    return 1;
}

void WifiClub_VBlankCB(WifiClubApp *arg0) {
    DoScheduledBgGpuUpdates(arg0->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    if (arg0->resetBrightness == 1) {
        SetMasterBrightnessNeutral(PM_LCD_TOP);
        SetMasterBrightnessNeutral(PM_LCD_BOTTOM);
        arg0->resetBrightness = 0;
    }
}

void WifiClub_SetVramBanks(void) {
    GraphicsBanks graphicsBanks = sWifiClubGraphicsBanks;
    GfGfx_SetBanks(&graphicsBanks);
}

void WifiClub_InitBgs(BgConfig *arg0) {
    GraphicsModes graphicsModes = sWifiClubGraphicsModes;
    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgTemplate0 = sWifiClubBgTemplate_Main0;
    InitBgFromTemplate(arg0, 0, &bgTemplate0, 0);
    BG_ClearCharDataRange(0, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(arg0, 0);

    BgTemplate bgTemplate1 = sWifiClubBgTemplate_Main1;
    InitBgFromTemplate(arg0, 1, &bgTemplate1, 0);
    BG_ClearCharDataRange(1, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(arg0, 1);

    BgTemplate bgTemplate2 = sWifiClubBgTemplate_Main2;
    InitBgFromTemplate(arg0, 2, &bgTemplate2, 0);
    BG_ClearCharDataRange(2, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(arg0, 2);

    BgTemplate bgTemplate3 = sWifiClubBgTemplate_Main3;
    InitBgFromTemplate(arg0, 3, &bgTemplate3, 0);
    BG_ClearCharDataRange(3, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(arg0, 3);

    BgTemplate bgTemplate4 = sWifiClubBgTemplate_Sub0;
    InitBgFromTemplate(arg0, 4, &bgTemplate4, 0);
    BgClearTilemapBufferAndCommit(arg0, 4);

    BgTemplate bgTemplate5 = sWifiClubBgTemplate_Sub1;
    InitBgFromTemplate(arg0, 5, &bgTemplate5, 0);
    BgClearTilemapBufferAndCommit(arg0, 5);

    BgTemplate bgTemplate6 = sWifiClubBgTemplate_Sub2;
    InitBgFromTemplate(arg0, 6, &bgTemplate6, 0);
    BgClearTilemapBufferAndCommit(arg0, 6);

    BgTemplate bgTemplate7 = sWifiClubBgTemplate_Sub3;
    InitBgFromTemplate(arg0, 7, &bgTemplate7, 0);
    BgClearTilemapBufferAndCommit(arg0, 7);
    BG_ClearCharDataRange(0, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(1, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(2, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(3, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(5, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(6, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(7, 32, 0, HEAP_ID_53);
    SetBgPriority(0, 3);
    SetBgPriority(1, 1);
    SetBgPriority(3, 0);
    SetBgPriority(2, 0);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
}

void WifiClub_AddPlayerObject(WifiClubApp *arg0, WifiClubRoomPerson *arg1) {
    for (int i = 0; i < 33; i++) {
        if (arg0->playerObjects[i] == NULL) {
            arg0->playerObjects[i] = arg1;
            return;
        }
    }
    GF_AssertFail();
    return;
}

void WifiClub_RemovePlayerObject(WifiClubApp *arg0, WifiClubRoomPerson *arg1) {
    for (int i = 0; i < 33; i++) {
        if (arg0->playerObjects[i] == arg1) {
            arg0->playerObjects[i] = NULL;
            return;
        }
    }
    GF_AssertFail();
    return;
}

WifiClubRoomPerson *WifiClub_FindPlayerObject(WifiClubApp *arg0, u32 arg1) {
    for (s32 i = 0; i < 33; i++) {
        if ((arg0->playerObjects[i] != NULL) && (arg1 == WifiClubRoomPerson_GetId(arg0->playerObjects[i]))) {
            return arg0->playerObjects[i];
        }
    }
    return NULL;
}

void WifiClub_LoadStatusIcons(BgConfig *arg0, WifiClubStatusIcons *arg1, NARC *arg2, enum HeapID arg3) {
    GfGfxLoader_GXLoadPalFromOpenNarc(arg2, 14, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_8_OFFSET, 64, arg3);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg2, 14, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, 64, arg3);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg2, 13, arg0, GF_BG_LYR_MAIN_1, 0, 0, 0, arg3);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg2, 13, arg0, GF_BG_LYR_SUB_2, 0, 0, 0, arg3);
    if (arg1->charsRaw == 0) {
        arg1->charsRaw = GfGfxLoader_GetCharDataFromOpenNarc(arg2, 13, 0, &arg1->chars, arg3);
    }
    if (arg1->screenRaw == NULL) {
        arg1->screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(arg2, 15, 0, &arg1->screen, arg3);
    }
}

void WifiClub_FreeStatusIcons(WifiClubStatusIcons *arg0) {
    if (arg0->screenRaw) {
        Heap_Free(arg0->screenRaw);
        arg0->screenRaw = NULL;
    }
    if (arg0->charsRaw) {
        Heap_Free(arg0->charsRaw);
        arg0->charsRaw = NULL;
    }
}

void WifiClub_DrawIconToBg(BgConfig *arg0, WifiClubStatusIcons *arg1, s32 arg2, u8 arg3, u8 arg4, s32 arg5, s32 arg6) {
    CopyToBgTilemapRect(arg0, arg2, arg3, arg4, 2, 2, arg1->screen->rawData, arg5 * 2, 0, arg1->screen->screenWidth / 8, arg1->screen->screenHeight / 8);
    s32 r0 = WifiClub_GetIconPaletteBase(arg2);
    BgTilemapRectChangePalette(arg0, arg2, arg3, arg4, 2, 2, r0 + arg6);
    ScheduleBgTilemapBufferTransfer(arg0, arg2);
}

void WifiClub_DrawStatusIconToBg(BgConfig *arg0, WifiClubStatusIcons *arg1, s32 arg2, u8 arg3, u8 arg4, u32 arg5) {
    s32 var_r4;
    s32 var_r5;

    var_r4 = 0;
    switch (arg5) {
    case 1:
        var_r5 = 3;
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        var_r4 = 1;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        var_r5 = 6;
        break;
    case 15:
        var_r4 = 1;
    case 8:
        var_r5 = 7;
        break;
    case 23:
    case 25:
    case 27:
        var_r4 = 1;
    case 22:
    case 24:
    case 26:
        var_r5 = 10;
        break;
    case 18:
    case 19:
        var_r5 = 5;
        break;
    case 21:
        var_r4 = 1;
    case 20:
        var_r5 = 11;
        break;
    case 16:
        var_r5 = 4;
        break;
    case 0:
        var_r5 = 8;
        break;
    case 28:
    case 29:
        var_r5 = 5;
        break;
    default:
        var_r5 = 5;
        break;
    }
    WifiClub_DrawIconToBg(arg0, arg1, arg2, arg3, arg4, var_r5, var_r4);
}

void WifiClub_DrawIconToWindow(Window *arg0, WifiClubStatusIcons *arg1, u8 arg2, u16 arg3, s32 arg4, s32 arg5) {
    BlitBitmapRectToWindow(arg0, arg1->chars->pRawData, arg4 * 16, 0, arg1->chars->W * 8, arg1->chars->H * 8, arg2, arg3, 16, 16);
    s32 r0 = WifiClub_GetIconPaletteBase(arg0->bgId);
    SetWindowPaletteNum(arg0, r0 + arg5);
}

void WifiClub_DrawStatusIconToWindow(Window *arg0, WifiClubStatusIcons *arg1, u8 arg2, u16 arg3, u32 arg4) {
    u8 var_r5;
    u8 var_r4 = 0;
    switch (arg4) {
    case 1:
        var_r5 = 3;
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        var_r4 = 1;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        var_r5 = 6;
        break;
    case 15:
        var_r4 = 1;
    case 8:
        var_r5 = 7;
        break;
    case 23:
    case 25:
    case 27:
        var_r4 = 1;
    case 22:
    case 24:
    case 26:
        var_r5 = 10;
        break;
    case 18:
    case 19:
        var_r5 = 5;
        break;
    case 21:
        var_r4 = 1;
    case 20:
        var_r5 = 11;
        break;
    case 16:
        var_r5 = 4;
        break;
    case 0:
        var_r5 = 8;
        break;
    case 28:
    case 29:
        var_r5 = 5;
        break;
    default:
        var_r5 = 5;
        break;
    }
    WifiClub_DrawIconToWindow(arg0, arg1, arg2, arg3, var_r5, var_r4);
}

s32 WifiClub_GetIconPaletteBase(u32 arg0) {
    if (arg0 < 4) {
        return 8;
    }
    return 13;
}

u32 WifiClub_FadeOutToEnterRoom(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
    WifiClub_PlayRoomBgm();
    return 18;
}

void WifiClub_AllocStrings(WifiClubApp *arg0) {
    arg0->msgExpanded = String_New(380, HEAP_ID_53);
    arg0->unk17C = String_New(380, HEAP_ID_53);
    arg0->msgRaw = String_New(380, HEAP_ID_53);
    arg0->tmpString = String_New(180, HEAP_ID_53);
    arg0->state = 0;
}

void WifiClub_FreeStrings(WifiClubApp *arg0) {
    String_Delete(arg0->tmpString);
    String_Delete(arg0->msgExpanded);
    String_Delete(arg0->unk17C);
    String_Delete(arg0->msgRaw);
}

void WifiClub_FreeBgConfig(BgConfig *arg0) {
    FreeBgTilemapBuffer(arg0, 7);
    FreeBgTilemapBuffer(arg0, 6);
    FreeBgTilemapBuffer(arg0, 5);
    FreeBgTilemapBuffer(arg0, 4);
    FreeBgTilemapBuffer(arg0, 3);
    FreeBgTilemapBuffer(arg0, 2);
    FreeBgTilemapBuffer(arg0, 1);
    FreeBgTilemapBuffer(arg0, 0);
    Heap_Free(arg0);
}

void WifiClub_LoadBgGraphics(WifiClubApp *arg0, NARC *arg1) {
    BgConfig *bgConfig = arg0->bgConfig;
    GfGfx_EngineATogglePlanes(1, 1);
    GfGfx_EngineATogglePlanes(2, 1);
    GfGfx_EngineATogglePlanes(4, 1);
    GfGfx_EngineATogglePlanes(8, 1);
    GfGfx_EngineBTogglePlanes(1, 1);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_53);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_53);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_53);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_53);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 2, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, HEAP_ID_53);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 6, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, HEAP_ID_53);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 11, bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, HEAP_ID_53);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 12, bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, HEAP_ID_53);
    Options *options = Save_PlayerData_GetOptionsAddr(arg0->saveData);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_2, 482, 10, Options_GetFrame(options), HEAP_ID_53);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_2, 473, 11, 0, HEAP_ID_53);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_1, 473, 11, 0, HEAP_ID_53);
}

void WifiClub_InitObjTransfer(void) {
    ObjCharTransferTemplate objCharTransferTemplate = sWifiClubObjCharTransferTemplate;
    ObjCharTransfer_InitEx(&objCharTransferTemplate, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(20, HEAP_ID_53);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
}

void WifiClub_InitSpriteSystem(WifiClubApp *arg0, NARC *arg1) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_53);
    arg0->spriteList = G2dRenderer_Init(8, &arg0->g2dRenderer, HEAP_ID_53);
    G2dRenderer_SetSubSurfaceCoords(&arg0->g2dRenderer, 0, 0x100000);
    for (int i = 0; i < 4; i++) {
        arg0->gfxResMan[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, HEAP_ID_53);
    }
    arg0->fontSystem = FontSystem_NewInit(1, HEAP_ID_53);
}

void WifiClub_EnableObjPlane(WifiClubApp *arg0) {
    GfGfx_EngineATogglePlanes(16, 1);
}

void WifiClub_PrintTitleBar(WifiClubApp *arg0) {
    if (WindowIsInUse(&arg0->titleWindow) != 0) {
        EraseFrameAndWindow1(&arg0->titleWindow, 0);
        RemoveWindow(&arg0->titleWindow);
    }
    AddWindowParameterized(arg0->bgConfig, &arg0->titleWindow, 3, 5, 1, 22, 2, 13, 458);
    FillWindowPixelBuffer(&arg0->titleWindow, 0);

    // Nintendo Wi-Fi Connection
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00021, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->titleWindow, 1, arg0->tmpString, FontID_String_GetCenterAlignmentX(0, arg0->tmpString, 0, 176), 0, 255, 0xF0E00, NULL);
    ScheduleWindowCopyToVram(&arg0->titleWindow);
}

void WifiClub_RemoveWindows(WifiClubApp *arg0) {
    WifiClub_CloseMessage(arg0);
    if (WindowIsInUse(&arg0->titleWindow) != 0) {
        RemoveWindow(&arg0->titleWindow);
    }
    if (WindowIsInUse(&arg0->msgWindow) != 0) {
        RemoveWindow(&arg0->msgWindow);
    }
    if (WindowIsInUse(&arg0->unk330) != 0) {
        RemoveWindow(&arg0->unk330);
    }
    if (WindowIsInUse(&arg0->statusWindow) != 0) {
        RemoveWindow(&arg0->statusWindow);
    }
    if (WindowIsInUse(&arg0->unk310) != 0) {
        RemoveWindow(&arg0->unk310);
    }
    if (arg0->unk150 != NULL) {
        DestroyListMenu(arg0->unk150, NULL, NULL);
    }
    if (arg0->unk14C != NULL) {
        ListMenuItems_Delete(arg0->unk14C);
        arg0->unk14C = NULL;
    }
    if (WindowIsInUse(&arg0->noticeWindow) != 0) {
        RemoveWindow(&arg0->noticeWindow);
    }
}

s32 WifiClub_State_Start(WifiClubApp *arg0, s32 arg1) {
    if (CommState_IsWifi() != 0) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        arg0->clubData = CommState_GetWifiClubData();
        u32 temp_r0 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
        if ((temp_r0 == 8) || (temp_r0 == 18) || (temp_r0 == 20) || (temp_r0 == 22) || (temp_r0 == 23) || (temp_r0 == 24) || (temp_r0 == 25) || (temp_r0 == 26) || (temp_r0 == 27) || (temp_r0 == 19)) {
            WifiClub_SetStatusAndSend(arg0, 16);
            CommPlayerInfo_Free();
            CommState_EndWifiMatch();
            WifiClubStatusData *temp_r0_2 = CommState_GetWifiClubData();
            arg0->clubData = temp_r0_2;
            temp_r0_2->own.voiceChat = temp_r0_2->own.voiceChatSetting;
            arg0->timer = 20;
            arg0->state = 65;
        } else {
            WifiClub_PrintMessage(arg0, 77, 0);
            CommState_SetErrorWatch(1, 1);
            LinkSync_StartSync(16);
            arg0->state = 61;
        }
    } else {
        Sound_SetSceneAndPlayBGM(11, SEQ_GS_WIFI_ACCESS, 1);
        WifiClub_PrintTitleBar(arg0);
        FriendRoster_GetUserData(arg0->friendRoster);
        if (DWC_CheckHasProfile() == 0) {
            SetBgPriority(0, 3);
            SetBgPriority(1, 0);
            SetBgPriority(3, 0);
            SetBgPriority(2, 1);
            WifiClub_ShowConnectionNotice(arg0, 18);
            arg0->yesNoPrompt = WifiClub_CreateYesNoPrompt(arg0->bgConfig, &sUserInfoPromptWindowTemplate, 0);
            arg0->state = 6;
        } else {
            FriendRoster_GetUserData(arg0->friendRoster);
            if (DWC_CheckValidConsole() == 0) {
                WifiClub_ShowConnectionNotice(arg0, 20);
                arg0->state = 3;
            } else {
                WifiClub_PrintMessage(arg0, 17, 1);
                arg0->state = 1;
            }
        }
    }
    return arg1;
}

s32 WifiClub_State_CreateConnectPrompt(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        arg0->yesNoPrompt = WifiClub_CreateYesNoPrompt(arg0->bgConfig, &sConnectPromptWindowTemplate, 0);
        arg0->state = 2;
    }
    return arg1;
}

s32 WifiClub_State_ConnectPrompt(WifiClubApp *arg0, s32 arg1) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        arg0->clubData = CommState_StartWifiClub(arg0->saveData, 1188);
        WifiClub_PrintMessage(arg0, 23, 1);
        GF_ASSERT(arg0->waitingIcon == NULL);
        arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
        arg0->state = 14;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        arg0->state = 34;
    } else {
        return arg1;
    }
    return arg1;
}

s32 WifiClub_State_AskUpdateUserInfo(WifiClubApp *arg0, s32 arg1) {
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        WifiClub_ShowConnectionNotice(arg0, 21);
        SetBgPriority(3, 0);
        SetBgPriority(2, 1);
        SetBgPriority(1, 0);
        SetBgPriority(0, 3);
        arg0->yesNoPrompt = WifiClub_CreateYesNoPrompt(arg0->bgConfig, &sUserInfoPromptWindowTemplate, 1);
        arg0->state = 4;
    }
    return arg1;
}

s32 WifiClub_State_UpdateUserInfoPrompt(WifiClubApp *arg0, s32 arg1) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        ClearFrameAndWindow2(&arg0->noticeWindow, 0);
        RemoveWindow(&arg0->noticeWindow);
        WifiClub_ShowConnectionNotice(arg0, 22);
        arg0->yesNoPrompt = WifiClub_CreateYesNoPrompt(arg0->bgConfig, &sUserInfoPromptWindowTemplate, 1);
        arg0->state = 5;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        ClearFrameAndWindow2(&arg0->noticeWindow, 0);
        RemoveWindow(&arg0->noticeWindow);
        arg0->state = 34;
    } else {
        return arg1;
    }
    return arg1;
}

s32 WifiClub_State_ConfirmUpdateUserInfoPrompt(WifiClubApp *arg0, s32 arg1) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        SetBgPriority(3, 0);
        SetBgPriority(2, 0);
        SetBgPriority(1, 1);
        SetBgPriority(0, 3);
        ClearFrameAndWindow2(&arg0->noticeWindow, 0);
        RemoveWindow(&arg0->noticeWindow);
        Save_FriendRoster_Init(arg0->friendRoster);
        void *saveArray = SaveArray_Get(arg0->saveData, 31);
        Save_WiiMessage_Init(saveArray);
        FrontierSave_ClearRows(Save_Frontier_GetStatic(arg0->saveData));
        arg0->state = 14;
        arg0->clubData = CommState_StartWifiClub(arg0->saveData, 1188);
        WifiClub_PrintMessage(arg0, 23, 1);
        GF_ASSERT(arg0->waitingIcon == NULL);
        arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
        arg0->userInfoSaved = 1;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        SetBgPriority(3, 0);
        SetBgPriority(2, 0);
        SetBgPriority(1, 1);
        SetBgPriority(0, 3);
        ClearFrameAndWindow2(&arg0->noticeWindow, 0);
        RemoveWindow(&arg0->noticeWindow);
        WifiClub_ShowConnectionNotice(arg0, 20);
        arg0->state = 3;
    } else {
        return arg1;
    }
    return arg1;
}

s32 WifiClub_State_SaveUserInfoPrompt(WifiClubApp *arg0, s32 arg1) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        SetBgPriority(0, 3);
        SetBgPriority(1, 1);
        SetBgPriority(3, 0);
        SetBgPriority(2, 0);
        ClearFrameAndWindow2(&arg0->noticeWindow, 0);
        RemoveWindow(&arg0->noticeWindow);
        arg0->state = 14;
        arg0->clubData = CommState_StartWifiClub(arg0->saveData, 1188);
        WifiClub_PrintMessage(arg0, 23, 1);
        GF_ASSERT(arg0->waitingIcon == NULL);
        arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
        arg0->userInfoSaved = 1;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        SetBgPriority(0, 3);
        SetBgPriority(1, 1);
        SetBgPriority(3, 0);
        SetBgPriority(2, 0);
        ClearFrameAndWindow2(&arg0->noticeWindow, 0);
        RemoveWindow(&arg0->noticeWindow);
        arg0->state = 34;
    } else {
        return arg1;
    }
    return arg1;
}

s32 WifiClub_State_StartConnecting(WifiClubApp *arg0, s32 arg1) {
    SetBgPriority(0, 3);
    SetBgPriority(1, 1);
    SetBgPriority(3, 0);
    SetBgPriority(2, 0);
    arg0->state = 15;
    return arg1;
}

void WifiClub_InitOwnStatus(WifiClubApp *arg0, u32 arg1) {
    PlayerProfile *playerProfile = Save_PlayerData_GetProfile(arg0->saveData);
    Pokedex *pokedex = Save_Pokedex_Get(arg0->saveData);
    Party *party = SaveArray_Party_Get(arg0->saveData);
    SaveWiFiHistory *saveWiFiHistory = Save_WiFiHistory_Get(arg0->saveData);
    s32 partyCount = Party_GetCount(party);

    for (s32 i = 0; i < partyCount; i++) {
        Pokemon *pokemon = Party_GetMonByIndex(party, i);
        arg0->clubData->own.species[i] = GetMonData(pokemon, MON_DATA_SPECIES, 0);
        arg0->clubData->own.heldItems[i] = GetMonData(pokemon, MON_DATA_HELD_ITEM, 0);
    }
    arg0->clubData->own.gameInfo[0] = PlayerProfile_GetVersion(playerProfile);
    arg0->clubData->own.gameInfo[1] = PlayerProfile_GetLanguage(playerProfile);
    arg0->clubData->own.gameInfo[2] = Pokedex_GetNatDexFlag(pokedex);
    arg0->clubData->own.status = 29;
    WifiClub_SetStatus(arg0, arg1);
    arg0->clubData->own.trainerInfo[0] = 0;
    arg0->clubData->own.trainerInfo[1] = PlayerProfile_GetAvatar(playerProfile);
    arg0->clubData->own.trainerInfo[2] = PlayerProfile_GetTrainerGender(playerProfile);
    arg0->clubData->own.trainerInfo[3] = WifiHistory_GetPlayerCountry(saveWiFiHistory);
    arg0->clubData->own.trainerInfo[4] = WiFiHistory_GetPlayerRegion(saveWiFiHistory);
    arg0->clubData->own.voiceChat = 1;
    arg0->clubData->own.voiceChatSetting = 1;
    WifiConnection_SetOwnStatusData(arg0->clubData, 36);
    WifiConnection_SetFriendStatusBuffer(arg0->clubData->friends, 36);
}

s32 WifiClub_CollectOnlineFriends(WifiClubApp *arg0) {
    s32 counter = 0;
    for (s32 i = 0; i < 32; i++) {
        arg0->onlineFriends[i] = 0;
        if ((arg0->friendCount > i) && (WifiClub_GetFriendOnlineState(i) != 0)) {
            WifiClubPlayerStatus *temp_r0 = WifiClub_GetFriendStatusData(arg0, i);
            u32 temp_r0_2 = WifiClub_GetPlayerStatus(arg0, temp_r0);
            if ((temp_r0_2 != 0) && (temp_r0_2 != 28) && (temp_r0->trainerInfo[1] != 0)) {
                arg0->onlineFriends[counter] = i + 1;
                counter += 1;
            } else if ((temp_r0_2 == 28) && (WifiClub_GetRosterSlotState(arg0, i + 1) != 0)) {
                WifiClub_ClearRosterSlotState(arg0, i + 1);
            }
        }
    }
    return counter;
}

s32 WifiClub_UpdateFriendStatuses(WifiClubApp *arg0) {
    s32 counter = 0;
    s32 sp8;
    for (s32 i = 0; i < 32; i++) {
        WifiClubPlayerStatus *temp_r0 = WifiClub_GetFriendStatusData(arg0, i);
        s32 temp_r0_2 = WifiClub_GetPlayerStatus(arg0, temp_r0);
        if ((temp_r0_2 != arg0->friendStatuses[i]) || (temp_r0->voiceChat != arg0->friendVoiceChat[i])) {
            WifiClubRoomPerson *temp_r0_3 = WifiClub_FindPlayerObject(arg0, i + 1);
            if (temp_r0_3 != NULL) {
                sp8 = WifiClub_IsSeekingStatus(arg0->friendStatuses[i]);
                s32 temp_r0_4 = WifiClub_IsSeekingStatus(temp_r0_2);
                if ((sp8 == 1) && (temp_r0_4 == 0)) {
                    WifiClubRoom_SetPersonBusy(&arg0->room, temp_r0_3, 0);
                } else if ((sp8 == 0) && (temp_r0_4 == 1)) {
                    WifiClubRoom_SetPersonBusy(&arg0->room, temp_r0_3, 1);
                }
                arg0->friendStatuses[i] = temp_r0_2;
                arg0->friendVoiceChat[i] = temp_r0->voiceChat;
                FriendRoster_SetEntryField(arg0->friendRoster, i, 7, temp_r0->trainerInfo[1]);
                FriendRoster_SetEntryField(arg0->friendRoster, i, 8, temp_r0->trainerInfo[2]);
                counter += 1;
            }
        }
    }
    return counter;
}

void WifiClub_HandleWifiError(WifiClubApp *arg0) {
    u32 *temp_r0 = CommState_GetWifiError();
    s32 temp_r6 = temp_r0[0];
    s32 var_r5 = WifiConnection_ClassifyErrorCode(temp_r6, temp_r0[1]);
    s32 var_r7;
    if ((var_r5 == 11) || (temp_r6 == 25)) {
        if (temp_r0[1] != 2) {
            var_r7 = 14;
        } else {
            var_r7 = 11;
        }
        var_r5 = 11;
    } else if (temp_r6 == 26) {
        var_r7 = 13;
        var_r5 = 12;
    } else if (var_r5 >= 0) {
        var_r7 = var_r5;
    } else {
        var_r7 = 11;
    }
    WifiClub_CloseMessage(arg0);
    BufferIntegerAsString(arg0->msgFormat, 0, temp_r6, 5, PRINTING_MODE_LEADING_ZEROS, 1);
    arg0->timer = 30;
    WifiClub_ShowConnectionNotice(arg0, var_r7);
    switch (var_r5) {
    case 1:
    case 4:
    case 5:
    case 11:
        arg0->state = 10;
        return;
    case 6:
    case 7:
    case 8:
    case 9:
        arg0->state = 10;
        return;
    case 10:
        arg0->state = 10;
        return;
    }
    arg0->state = 35;
}

s32 WifiClub_State_WaitErrorButton(WifiClubApp *arg0, s32 arg1) {
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        if (WindowIsInUse(&arg0->noticeWindow) != 0) {
            ClearFrameAndWindow2(&arg0->noticeWindow, 0);
            RemoveWindow(&arg0->noticeWindow);
        }
        arg0->state = 8;
    }
    return arg1;
}

s32 WifiClub_State_ErrorAcknowledged(WifiClubApp *arg0, s32 arg1) {
    arg0->state = 9;
    return arg1;
}

s32 WifiClub_State_GoToReconnectPrompt(WifiClubApp *arg0, s32 arg1) {
    arg0->state = 11;
    return arg1;
}

s32 WifiClub_State_ErrorThenAskReconnect(WifiClubApp *arg0, s32 arg1) {
    s32 temp_r1;

    temp_r1 = arg0->timer;
    if (temp_r1 > 0) {
        arg0->timer = temp_r1 - 1;
        return arg1;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        if (WindowIsInUse(&arg0->noticeWindow) != 0) {
            ClearFrameAndWindow2(&arg0->noticeWindow, 0);
            RemoveWindow(&arg0->noticeWindow);
        }
        WifiClub_PrintMessage(arg0, 88, 0);
        arg0->state = 11;
    }
    return arg1;
}

s32 WifiClub_State_CreateReconnectPrompt(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->yesNoMenu = Std_CreateYesNoMenu(arg0->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            arg0->yesNoPrompt = WifiClub_CreateYesNoPrompt(arg0->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        arg0->state = 12;
    }
    return arg1;
}

s32 WifiClub_State_ReconnectPrompt(WifiClubApp *arg0, s32 arg1) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->yesNoMenu, HEAP_ID_53);
        if (temp_r0 == -1) {
            return arg1;
        }
        if (temp_r0 == 0) {
            CommState_EndWifiSession();
            WifiClub_SetStatusAndSend(arg0, 16);
            arg0->state = 13;
        } else {
            WifiClub_SetStatusAndSend(arg0, 16);
            CommState_EndWifiSession();
            CommPlayerInfo_Free();
            arg0->exitChoice = 8;
            arg0->state = 34;
        }
    } else {
        enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
        if (response == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            CommState_EndWifiSession();
            WifiClub_SetStatusAndSend(arg0, 16);
            arg0->state = 13;
        } else if (response == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            WifiClub_SetStatusAndSend(arg0, 16);
            CommState_EndWifiSession();
            CommPlayerInfo_Free();
            arg0->exitChoice = 8;
            arg0->state = 34;
        } else {
            return arg1;
        }
    }
    WifiClub_CloseMessage(arg0);
    return arg1;
}

s32 WifiClub_State_Reconnect(WifiClubApp *arg0, s32 arg1) {
    if (CommWireless_IsStartedUp() != 0) {

    } else {
        if (WifiClubRoom_IsInitialized(&arg0->room) == 1) {
            SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
            SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
            WifiClubRoom_Free(&arg0->room);
            for (s32 i = 0; i < 33; i++) {
                arg0->playerObjects[i] = 0;
            }
            thunk_ClearMainOAM(HEAP_ID_53);
            thunk_ClearSubOAM(HEAP_ID_53);
        }
        if (WifiClub_IsRosterScreenActive(arg0) == 1) {
            WifiClub_FreeRosterScreen(arg0);
        }
        if (WindowIsInUse(&arg0->titleWindow) != 0) {
            EraseFrameAndWindow1(&arg0->titleWindow, 0);
            RemoveWindow(&arg0->titleWindow);
        }
        if (WindowIsInUse(&arg0->statusWindow) != 0) {
            RemoveWindow(&arg0->statusWindow);
        }
        BgClearTilemapBufferAndCommit(arg0->bgConfig, 3);
        NARC *temp_r0 = NARC_New(NARC_a_0_8_8, HEAP_ID_53);
        WifiClub_LoadBgGraphics(arg0, temp_r0);
        NARC_Delete(temp_r0);
        arg0->paletteCycle.active = 1;
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        BgSetPosTextAndCommit(arg0->bgConfig, 0, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(arg0->bgConfig, 0, BG_POS_OP_SET_Y, 0);
        arg0->resetBrightness = 1;
        if (arg0->unk14C != NULL) {
            RemoveWindow(&arg0->unk310);
            DestroyListMenu(arg0->unk150, NULL, NULL);
            arg0->unk150 = NULL;
            ListMenuItems_Delete(arg0->unk14C);
            arg0->unk14C = NULL;
        }
        MI_CpuFill8(arg0->onlineFriends, 0, 32);
        MI_CpuFill8(arg0->prevOnlineFriends, 0, 32);
        MI_CpuFill8(arg0->friendStatuses, 0, 128);
        MI_CpuFill8(arg0->friendVoiceChat, 0, 128);
        WifiClub_PrintTitleBar(arg0);
        arg0->clubData = CommState_StartWifiClub(arg0->saveData, 1188);
        WifiClub_PrintMessage(arg0, 23, 1);
        GF_ASSERT(arg0->waitingIcon == NULL);
        arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
        arg0->state = 14;
    }
    return arg1;
}

s32 WifiClub_State_Connecting(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    if (WifiConnection_IsFriendDataChanged() != 0) {
        arg0->state = 69;
        WifiClub_PrintMessage(arg0, 30, 1);
        GF_ASSERT(arg0->waitingIcon == NULL);
        arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
    }
    if (CommState_IsWifiJoiningLobby() != 0) {
        if (arg0->userInfoSaved != 0) {
            arg0->state = 16;
        } else {
            WifiClub_InitOwnStatus(arg0, 16);
            WifiClub_CollectOnlineFriends(arg0);
            arg0->state = WifiClub_FadeOutToEnterRoom();
        }
    } else if ((CommState_HasWifiFailed() != 0) || (CommState_GetWifiMatchStatus() == 3)) {
        WifiClub_HandleWifiError(arg0);
    }
    return arg1;
}

s32 WifiClub_State_PrepareSave(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    if (WifiConnection_IsFriendDataChanged() != 0) {
        Save_PrepareForAsyncWrite(arg0->saveData, 0);
        arg0->state = 71;
        return arg1;
    }
    if (CommState_IsWifiJoiningLobby() != 0) {
        if (arg0->userInfoSaved != 0) {
            arg0->state = 16;
        } else {
            WifiClub_InitOwnStatus(arg0, 16);
            WifiClub_CollectOnlineFriends(arg0);
            arg0->state = WifiClub_FadeOutToEnterRoom();
        }
    } else if ((CommState_HasWifiFailed() != 0) || (CommState_GetWifiMatchStatus() == 3)) {
        WifiClub_HandleWifiError(arg0);
    }
    return arg1;
}

s32 WifiClub_State_WriteSave(WifiClubApp *arg0, s32 arg1) {
    if (WifiConnection_IsFriendDataChanged() != 0) {
        s32 temp_r0 = Save_WriteFileAsync(arg0->saveData);
        if ((temp_r0 != 0) && (temp_r0 != 1)) {
            WifiConnection_ClearFriendDataChanged();
            Save_ClearStatusFlags(arg0->saveData);
        } else {
            return arg1;
        }
    }
    if (CommState_IsWifiJoiningLobby() != 0) {
        if (arg0->userInfoSaved != 0) {
            arg0->state = 16;
        } else {
            WifiClub_InitOwnStatus(arg0, 16);
            WifiClub_CollectOnlineFriends(arg0);
            arg0->state = WifiClub_FadeOutToEnterRoom();
        }
    } else if ((CommState_HasWifiFailed() != 0) || (CommState_GetWifiMatchStatus() == 3)) {
        WifiClub_HandleWifiError(arg0);
    }
    return arg1;
}

s32 WifiClub_State_WaitThenReenterRoom(WifiClubApp *arg0, s32 arg1) {
    if (arg0->timer > 0) {
        arg0->timer--;
        return arg1;
    }
    if (CommState_IsWifiInLobby() != 0) {
        arg0->state = WifiClub_FadeOutToEnterRoom();
    } else if ((CommState_HasWifiFailed() != 0) || (CommState_GetWifiMatchStatus() == 3)) {
        WifiClub_HandleWifiError(arg0);
    } else if ((CommState_GetWifiMatchStatus() >= 4) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0)) {
        CommState_LeaveWifiMatch();
        arg0->state = WifiClub_FadeOutToEnterRoom();
    }
    return arg1;
}

s32 WifiClub_State_ShowUserInfoSaved(WifiClubApp *arg0, s32 arg1) {
    WifiClub_CloseMessage(arg0);
    WifiClub_ShowConnectionNotice(arg0, 19);
    arg0->state = 17;
    arg0->userInfoSaved = 0;
    return arg1;
}

s32 WifiClub_State_UserInfoSavedWaitButton(WifiClubApp *arg0, s32 arg1) {
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        if (arg0->startMode == 1) {
            arg0->exitChoice = 10;
            arg0->state = 34;
            CommState_EndWifiSession();
        } else if (FriendRoster_CountFriends(arg0->friendRoster) == 0) {
            WifiClub_PrintMessage(arg0, 26, 1);
            arg0->state = 59;
            arg0->timer = 1;
        } else {
            WifiClub_InitOwnStatus(arg0, 16);
            WifiClub_CollectOnlineFriends(arg0);
            arg0->state = WifiClub_FadeOutToEnterRoom();
            if (WindowIsInUse(&arg0->noticeWindow) != 0) {
                ClearFrameAndWindow2(&arg0->noticeWindow, 0);
                RemoveWindow(&arg0->noticeWindow);
            }
        }
    }
    return arg1;
}

void WifiClub_PrintOwnStatus(WifiClubApp *arg0) {
    PlayerProfile *playerProfile = Save_PlayerData_GetProfile(arg0->saveData);
    String *strPlayerName = PlayerProfile_GetPlayerName_NewString(playerProfile, HEAP_ID_53);
    if (WindowIsInUse(&arg0->statusWindow) == 0) {
        String_Delete(strPlayerName);
        return;
    }
    u32 color;
    FillWindowPixelBuffer(&arg0->statusWindow, 0);
    if (PlayerProfile_GetTrainerGender(playerProfile) == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    AddTextPrinterParameterizedWithColor(&arg0->statusWindow, 0, strPlayerName, 32, 0, 255, color, NULL);
    String_Delete(strPlayerName);
    s32 temp_r0_2 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
    s32 temp_r6 = WifiClub_GetStatusLabel(temp_r0_2, &color);
    if (temp_r0_2 == 1) {
        if (WifiConnection_GetTargetFriendIndex() == -1) {
            ReadMsgDataIntoString(arg0->msgData, temp_r6, arg0->msgRaw);
        } else {
            s32 temp_r0_3 = FriendRoster_GetEntryField(arg0->friendRoster, WifiConnection_GetTargetFriendIndex(), 8);
            if (temp_r0_3 == 0) {
                color = MAKE_TEXT_COLOR(5, 6, 0);
            } else if (temp_r0_3 == 1) {
                color = MAKE_TEXT_COLOR(3, 4, 0);
            }
            CopyU16ArrayToString(arg0->msgRaw, FriendRoster_GetPlayerName(arg0->friendRoster, WifiConnection_GetTargetFriendIndex()));
        }
    } else {
        ReadMsgDataIntoString(arg0->msgData, temp_r6, arg0->msgRaw);
    }
    AddTextPrinterParameterizedWithColor(&arg0->statusWindow, 0, arg0->msgRaw, 102, 0, 255, color, NULL);
    WifiClub_DrawStatusIconToBg(arg0->bgConfig, &arg0->statusIcons, 1, 2, 1, temp_r0_2);
    s32 var_r0_2;
    if (arg0->clubData->own.voiceChat != 0) {
        var_r0_2 = 8;
    } else {
        var_r0_2 = 1;
    }
    WifiClub_DrawIconToBg(arg0->bgConfig, &arg0->statusIcons, 1, 26, 1, var_r0_2, 0);
    ScheduleWindowCopyToVram(&arg0->statusWindow);
}

void WifiClub_ListMenuCursorCallback(struct ListMenu *list, s32 index, u8 onInit) {
    if (onInit == 0) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

void WifiClub_CreateStatusBarWindows(WifiClubApp *arg0) {
    if (WindowIsInUse(&arg0->statusWindow) != 0) {
        RemoveWindow(&arg0->statusWindow);
    }
    if (WindowIsInUse(&arg0->titleWindow) != 0) {
        EraseFrameAndWindow1(&arg0->titleWindow, 0);
        RemoveWindow(&arg0->titleWindow);
    }
    AddWindowParameterized(arg0->bgConfig, &arg0->statusWindow, 3, 1, 1, 28, 2, 13, 402);
    AddWindowParameterized(arg0->bgConfig, &arg0->titleWindow, 1, 1, 1, 28, 2, 13, 417);
    FillWindowPixelBuffer(&arg0->titleWindow, 15);
    ScheduleWindowCopyToVram(&arg0->titleWindow);
    DrawFrameAndWindow1(&arg0->titleWindow, 0, 473, 11);
}

s32 WifiClub_State_EnterRoom(WifiClubApp *arg0, s32 arg1) {
    if (IsPaletteFadeFinished() == 0) {
        return arg1;
    }
    if ((CommState_HasWifiFailed() == 0) && (CommState_IsWifiInLobby() == 0)) {
        return arg1;
    }
    WifiConnection_SetConnectFilterCallback(WifiClub_IsMatchingActivity);
    arg0->paletteCycle.active = 0;
    NARC *temp_r0 = NARC_New(NARC_a_0_8_8, HEAP_ID_53);
    WifiClub_LoadStatusIcons(arg0->bgConfig, &arg0->statusIcons, temp_r0, HEAP_ID_53);
    WifiClub_RestoreVoiceChatSetting(arg0);
    WifiClub_CloseMessage(arg0);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 3);
    if (arg0->unk14C != NULL) {
        RemoveWindow(&arg0->unk310);
        DestroyListMenu(arg0->unk150, NULL, NULL);
        arg0->unk150 = NULL;
        ListMenuItems_Delete(arg0->unk14C);
        arg0->unk14C = NULL;
    }
    if (WifiClubRoom_IsInitialized(&arg0->room) == 0) {
        s16 var_r4;
        if (arg0->clubData->own.trainerInfo[2] == 0) {
            var_r4 = 0;
        } else {
            var_r4 = 97;
        }
        arg0->friendCount = FriendRoster_GetUsedSlotCount(arg0->friendRoster);
        WifiClubRoom_Init(&arg0->room, HEAP_ID_53, temp_r0, arg0->bgConfig, var_r4, arg0->friendCount);
        WifiClub_AddPlayerObject(arg0, WifiClubRoom_AddPlayer(&arg0->room, var_r4));
    }
    if (WifiClub_IsRosterScreenActive(arg0) == 0) {
        WifiClub_InitRosterScreen(arg0, temp_r0, HEAP_ID_53);
    }
    WifiClub_RedrawRoster(arg0, HEAP_ID_53);
    WifiClub_CloseMessage(arg0);
    WifiClub_CollectOnlineFriends(arg0);
    WifiClub_CreateStatusBarWindows(arg0);
    WifiClub_SetStatusAndSend(arg0, 16);
    CommState_SetWifiClubLinkMode();
    arg0->contactFriend = -1;
    NARC_Delete(temp_r0);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_53);
    arg0->state = 19;
    CommState_SetErrorWatch(0, 0);
    return arg1;
}

s32 WifiClub_IsMatchConnected(WifiClubApp *arg0) {
    if (CommState_GetWifiMatchStatus() == 1) {
        return 1;
    }
    return 0;
}

s32 WifiClub_CheckIncomingContact(WifiClubApp *arg0) {
    if (WifiConnection_GetNewClientIndex() != -1) {
        WifiConnection_SetVoiceChatEnabled(arg0->clubData->own.voiceChat);
        return 1;
    }
    return 0;
}

s32 WifiClub_SyncRoomPlayers(WifiClubApp *arg0) {
    int sp8 = WifiClub_CollectOnlineFriends(arg0);
    int sp4 = 0;
    int sp0 = 0;
    if (memcmp(arg0->onlineFriends, arg0->prevOnlineFriends, 32) != 0) {
        for (int i = 0; i < 32; i++) {
            int j;
            int var_r2 = 0;
            for (j = 0; j < 32; j++) {
                if (arg0->onlineFriends[j] == arg0->prevOnlineFriends[i]) {
                    var_r2 = 1;
                    break;
                }
            }
            if ((var_r2 == 0) && (arg0->prevOnlineFriends[i] != 0)) {
                WifiClubRoomPerson *temp_r0 = WifiClub_FindPlayerObject(arg0, arg0->prevOnlineFriends[i]);
                if (temp_r0 != NULL) {
                    WifiClubRoom_StartPersonLeave(&arg0->room, temp_r0);
                    WifiClub_RemovePlayerObject(arg0, temp_r0);
                    WifiClub_ClearRosterSlotState(arg0, arg0->prevOnlineFriends[i]);
                    WifiClub_CancelRosterSelectionOf(arg0, arg0->prevOnlineFriends[i], HEAP_ID_53);
                    sp0 = 1;
                }
            }
            var_r2 = 0;
            for (j = 0; j < 32; j++) {
                if (arg0->onlineFriends[i] == arg0->prevOnlineFriends[j]) {
                    var_r2 = 1;
                    break;
                }
            }
            if (var_r2 == 0 && arg0->onlineFriends[i] != 0) {
                WifiClubPlayerStatus *temp = WifiClub_GetFriendStatusData(arg0, arg0->onlineFriends[i] - 1);
                WifiClubRoomPerson *temp_r0_2 = WifiClubRoom_AddFriend(&arg0->room, temp->trainerInfo[1], arg0->onlineFriends[i]);
                if (temp_r0_2 != NULL) {
                    WifiClub_SetRosterSlotState(arg0, arg0->onlineFriends[i], 2);
                    WifiClub_AddPlayerObject(arg0, temp_r0_2);
                    sp4 = 1;
                } else {
                    WifiClub_SetRosterSlotState(arg0, arg0->onlineFriends[i], 1);
                    arg0->onlineFriends[i] = 0;
                }
            }
        }
        memcpy(arg0->prevOnlineFriends, arg0->onlineFriends, 32);
        WifiClub_RequestRosterRedraw(arg0);
        if (sp4 == 1) {
            PlaySE(SEQ_SE_DP_TELE2);
        } else if (sp0 == 1) {
            PlaySE(SEQ_SE_DP_TELE2);
        }
    }
    return sp8;
}

void WifiClub_RefreshFriendStatuses(WifiClubApp *arg0, enum HeapID heapID) {
    if (WifiClub_UpdateFriendStatuses(arg0) > 0) {
        WifiClub_RedrawRoster(arg0, heapID);
    }
}

void WifiClub_UpdateRoomPlayers(WifiClubApp *arg0, enum HeapID heapID) {
    WifiClub_SyncRoomPlayers(arg0);
    WifiClub_RefreshFriendStatuses(arg0, heapID);
}

s32 WifiClub_State_Room(WifiClubApp *arg0, s32 arg1) {
    u32 var_r7 = 0;
    if (IsPaletteFadeFinished() == 0) {
        return arg1;
    }
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    if (CommState_GetWifiMatchStatus() == 4) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 18, 0);
        arg0->state = 29;
        WifiClub_SetStatusAndSend(arg0, 16);
        CommState_LeaveWifiMatch();
        return arg1;
    }
    WifiClub_UpdateRoomPlayers(arg0, HEAP_ID_53);
    WifiClub_RedrawRosterIfNeeded(arg0);
    WifiClubRoom_ContinueLightSequence(&arg0->room);
    if (WifiClubRoom_IsInputLocked(&arg0->room) == 0) {
        if ((WifiConnection_GetNewClientIndex() == -1) && (gSystem.newKeys & PAD_BUTTON_X)) {
            if (WifiClub_ToggleVoiceChatSetting(arg0) != 0) {
                WifiClub_PrintMessage(arg0, 90, 0);
            } else {
                WifiClub_PrintMessage(arg0, 91, 0);
            }
            PlaySE(SEQ_SE_DP_DECIDE);
            WifiClub_PrintOwnStatus(arg0);
            arg0->state = 20;
            WifiClub_CancelRosterSelection(arg0, HEAP_ID_53);
            return arg1;
        }
        u32 temp_r0 = WifiClub_UpdateRosterScreen(arg0, HEAP_ID_53);
        WifiClubRoomPerson *temp_r1 = WifiClub_FindPlayerObject(arg0, temp_r0);
        if (temp_r0 != 0) {
            if (temp_r1 != NULL) {
                WifiClubRoom_ShowFriendMarker(&arg0->room, temp_r1);
            } else {
                WifiClubRoom_HideFriendMarker(&arg0->room);
            }
        } else {
            WifiClubRoom_HideFriendMarker(&arg0->room);
        }
    }
    if ((WifiClub_IsMatchConnected(arg0) != 0) && (arg0->contactFriend != -1)) {
        PlaySE(SEQ_SE_DP_UG_020);
        arg0->state = 48;
        return arg1;
    }
    if ((arg0->contactFriend == -1) && (WifiConnection_GetNewClientIndex() != -1)) {
        arg0->contactFriend = WifiConnection_GetNewClientIndex();
        WifiClub_BufferFriendName(arg0, arg0->contactFriend);
        WifiClub_PrintMessage(arg0, 76, 0);
        WifiConnection_SetVoiceChatEnabled(arg0->clubData->own.voiceChat);
        arg0->unk344 = 0;
    } else if ((arg0->contactFriend != -1) && (WifiConnection_GetNewClientIndex() == -1)) {
        WifiConnection_SetVoiceChatEnabled(0);
        WifiClub_BufferFriendName(arg0, arg0->contactFriend);
        WifiClub_PrintMessage(arg0, 18, 0);
        CommState_LeaveWifiMatch();
        arg0->state = 27;
        return arg1;
    }
    if ((WifiConnection_IsVoiceChatEnabled() == 1) && (WifiConnection_GetNewClientIndex() == -1)) {
        WifiConnection_SetVoiceChatEnabled(0);
    }
    u32 r0 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
    if (arg0->contactFriend == -1) {
        if ((gSystem.newKeys & PAD_BUTTON_B) && (WifiClub_IsSeekingStatus(r0) != 0)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            arg0->state = 39;
            WifiClub_CancelRosterSelection(arg0, HEAP_ID_53);
            return arg1;
        }
        var_r7 = WifiClubRoom_Update(&arg0->room);
        WifiClubRoom_UpdateLights(&arg0->room);
    }
    switch (var_r7) {
    case 0:
        return arg1;
    case 1:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (WifiClub_IsSeekingStatus(r0) != 0) {
            arg0->state = 39;
        } else {
            arg0->exitChoice = 8;
            arg0->state = 57;
            WifiClub_PrintMessage(arg0, 25, 1);
        }
        WifiClub_CancelRosterSelection(arg0, HEAP_ID_53);
        return arg1;
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (WifiClub_IsSeekingStatus(r0) != 0) {
            WifiClubRoom_StartCounterLight(&arg0->room);
            arg0->state = 39;
        } else if (r0 == 16 && CommState_IsWifiInLobby() == 1) {
            WifiClubRoom_StartCounterLight(&arg0->room);
            arg0->state = 36;
        }
        WifiClub_CancelRosterSelection(arg0, HEAP_ID_53);
        return arg1;
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (WifiClub_IsSeekingStatus(r0) != 0) {
            arg0->state = 39;
        } else {
            arg0->state = 42;
        }
        WifiClub_CancelRosterSelection(arg0, HEAP_ID_53);
        return arg1;
    }
    GF_AssertFail();
    return arg1;
}

s32 WifiClub_State_VoiceChatToggleMessage(WifiClubApp *arg0, s32 arg1) {
    if (WifiClub_CheckIncomingContact(arg0) != 0) {
        arg0->state = 19;
        WifiClub_StopSeeking(arg0);
    } else if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        arg0->state = 19;
        WifiClub_StopSeeking(arg0);
        WifiClub_CloseMessage(arg0);
    }
    return arg1;
}

s32 WifiClub_State_WaitVoiceChatPartner(WifiClubApp *arg0, s32 arg1) {
    WifiClubPlayerStatus *temp_r0 = WifiClub_GetFriendStatusData(arg0, arg0->partnerId - 1);
    u8 temp_r4 = temp_r0->voiceChat;
    u32 temp_r6 = WifiClub_GetPlayerStatus(arg0, temp_r0);
    arg0->matchTimer -= 1;
    if (arg0->matchTimer < 0) {
        arg0->state = 70;
    } else if (CommState_GetWifiMatchStatus() == 5) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 15, 0);
        arg0->state = 27;
    } else if (CommState_GetWifiMatchStatus() == 3) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 18, 0);
        arg0->state = 27;
    } else if ((CommState_GetWifiMatchStatus() == 4) || (CommState_WasWifiDisconnected() != 0)) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 19, 0);
        arg0->state = 27;
    } else if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
    } else if (temp_r4 == 0) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 27;
    } else if (temp_r6 != 16) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 27;
    } else if (CommState_GetWifiMatchStatus() == 1) {
        WifiClub_CloseMessage(arg0);
        WifiClub_SetStatusAndSend(arg0, 1);
        arg0->state = 24;
        FriendRoster_UpdateDate(arg0->friendRoster, arg0->partnerId - 1);
    }
    return arg1;
}

s32 WifiClub_State_StartVoiceChat(WifiClubApp *arg0, s32 arg1) {
    WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
    WifiClub_PrintMessage(arg0, 14, 0);
    WifiClub_SetStatusAndSend(arg0, 1);
    arg0->state = 23;
    if (WifiConnection_GetTargetFriendIndex() != -1) {
        FriendRoster_UpdateDate(arg0->friendRoster, WifiConnection_GetTargetFriendIndex());
    }
    return arg1;
}

s32 WifiClub_State_VoiceChatStartMessage(WifiClubApp *arg0, s32 arg1) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    if (WifiConnection_GetTargetFriendIndex() < 0) {
        WifiClub_PrintMessage(arg0, 19, 0);
        arg0->state = 27;
        return arg1;
    }
    u32 temp_r0 = WifiClub_GetPlayerStatus(arg0, WifiClub_GetFriendStatusData(arg0, WifiConnection_GetTargetFriendIndex()));
    if ((temp_r0 != 16) && (temp_r0 != 1)) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 27;
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
            WifiClub_CloseMessage(arg0);
            arg0->state = 24;
        } else {
            WifiClub_State_VoiceChat(arg0, arg1);
        }
    }
    return arg1;
}

s32 WifiClub_State_VoiceChat(WifiClubApp *arg0, s32 arg1) {
    if (WifiConnection_GetTargetFriendIndex() < 0) {
        WifiClub_PrintMessage(arg0, 19, 0);
        arg0->state = 27;
        return arg1;
    }
    int temp_r4 = WifiClub_GetPlayerStatus(arg0, WifiClub_GetFriendStatusData(arg0, WifiConnection_GetTargetFriendIndex()));
    WifiClub_UpdateRoomPlayers(arg0, HEAP_ID_53);
    WifiClub_RedrawRosterIfNeeded(arg0);
    if (CommState_GetWifiMatchStatus() == 5) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 15, 0);
        arg0->state = 27;
    } else if (CommState_GetWifiMatchStatus() == 3) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 18, 0);
        arg0->state = 27;
    } else if ((CommState_GetWifiMatchStatus() == 4) || (CommState_WasWifiDisconnected() != 0)) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 19, 0);
        arg0->state = 27;
    } else if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
    } else if ((temp_r4 != 16) && (temp_r4 != 1)) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 27;
    } else if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        WifiClub_PrintMessage(arg0, 20, 0);
        arg0->state = 25;
    } else if (WifiConnection_GetVoiceActivity() != 0) {
        WifiClub_DrawIconToBg(arg0->bgConfig, &arg0->statusIcons, 1, 2, 1, 2, 0);
    } else {
        WifiClub_DrawIconToBg(arg0->bgConfig, &arg0->statusIcons, 1, 2, 1, 3, 0);
    }
    return arg1;
}

s32 WifiClub_State_CreateEndVoiceChatPrompt(WifiClubApp *arg0, s32 arg1) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        if (WifiConnection_GetTargetFriendIndex() != -1) {
            FriendRoster_UpdateDate(arg0->friendRoster, WifiConnection_GetTargetFriendIndex());
        }
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->yesNoMenu = Std_CreateYesNoMenu(arg0->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            arg0->yesNoPrompt = WifiClub_CreateYesNoPromptIgnoreTouch(arg0->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        arg0->state = 26;
    }
    return arg1;
}

s32 WifiClub_State_EndVoiceChatPrompt(WifiClubApp *arg0, s32 arg1) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->yesNoMenu, HEAP_ID_53);
        if (temp_r0 == -1) {
            if ((CommState_GetWifiMatchStatus() >= 4) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0)) {
                WifiClub_CloseMessage(arg0);
                WifiClub_SetStatusAndSend(arg0, 16);
                Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
                arg0->contactFriend = -1;
                arg0->timer = 20;
                arg0->state = 65;
            } else if (CommState_HasWifiFailed() != 0) {
                WifiClub_HandleWifiError(arg0);
            }
            return arg1;
        }
        if (temp_r0 == 0) {
            if (CommState_HasWifiFailed() == 0) {
                WifiClub_SetStatusAndSend(arg0, 16);
                CommState_SendWifiDisconnect();
                arg0->contactFriend = -1;
                arg0->timer = 20;
                arg0->state = 65;
            } else {
                WifiClub_HandleWifiError(arg0);
            }
        } else {
            arg0->state = 24;
        }
    } else {
        enum YesNoResponse yesNoReponse = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
        if (yesNoReponse == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            if (CommState_HasWifiFailed() == 0) {
                WifiClub_SetStatusAndSend(arg0, 16);
                CommState_SendWifiDisconnect();
                arg0->contactFriend = -1;
                arg0->timer = 20;
                arg0->state = 65;
            } else {
                WifiClub_HandleWifiError(arg0);
            }
        } else if (yesNoReponse == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            arg0->state = 24;
        } else {
            if ((CommState_GetWifiMatchStatus() >= 4) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0)) {
                WifiClub_CloseMessage(arg0);
                WifiClub_SetStatusAndSend(arg0, 16);
                YesNoPrompt_Destroy(arg0->yesNoPrompt);
                arg0->contactFriend = -1;
                arg0->timer = 20;
                arg0->state = 65;
            } else if (CommState_HasWifiFailed() != 0) {
                WifiClub_HandleWifiError(arg0);
            }
            return arg1;
        }
    }
    WifiClub_CloseMessage(arg0);
    return arg1;
}

s32 WifiClub_State_MessageThenLeaveMatch(WifiClubApp *arg0, s32 arg1) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    if (WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own) != 16) {
        WifiClub_SetStatusAndSend(arg0, 16);
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        arg0->timer = 30;
        return arg1;
    }
    arg0->timer -= 1;
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (arg0->timer == 0) || (System_GetTouchNew() == 1)) {
        WifiClub_CloseMessage(arg0);
        CommState_LeaveWifiMatch();
        CommState_SetWifiClubLinkMode();
        if (WifiClubRoom_IsInitialized(&arg0->room) == 0) {
            arg0->timer = 20;
            arg0->state = 65;
        } else {
            WifiClub_RestoreVoiceChatSetting(arg0);
            WifiClub_PrintOwnStatus(arg0);
            arg0->contactFriend = -1;
            arg0->state = 19;
            WifiClub_StopSeeking(arg0);
        }
    }
    return arg1;
}

s32 WifiClub_State_MessageThenReenterRoom(WifiClubApp *arg0, s32 arg1) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        WifiClub_CloseMessage(arg0);
        WifiClub_SetStatusAndSend(arg0, 16);
        CommState_LeaveWifiMatch();
        arg0->timer = 20;
        arg0->state = 65;
    }
    return arg1;
}

s32 WifiClub_State_MessageThenReturnToRoom(WifiClubApp *arg0, s32 arg1) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        arg0->timer = 30;
        return arg1;
    }
    arg0->timer -= 1;
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (arg0->timer == 0) || (System_GetTouchNew() == 1)) {
        WifiClub_CloseMessage(arg0);
        WifiClub_SetStatusAndSend(arg0, 16);
        CommState_SetWifiClubLinkMode();
        if (WifiClubRoom_IsInitialized(&arg0->room) == 0) {
            arg0->state = WifiClub_FadeOutToEnterRoom();
        } else {
            WifiClub_RestoreVoiceChatSetting(arg0);
            WifiClub_PrintOwnStatus(arg0);
            arg0->contactFriend = -1;
            arg0->state = 19;
            WifiClub_StopSeeking(arg0);
        }
    }
    return arg1;
}

s32 WifiClub_State_Dummy30(WifiClubApp *arg0, s32 arg1) {
    return arg1;
}

s32 WifiClub_State_Dummy31(WifiClubApp *arg0, s32 arg1) {
    return arg1;
}

s32 WifiClub_State_Dummy32(WifiClubApp *arg0, s32 arg1) {
    return arg1;
}

s32 WifiClub_State_ErrorThenDisconnect(WifiClubApp *arg0, s32 arg1) {
    if (arg0->timer > 0) {
        arg0->timer--;
        return arg1;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        WifiClub_PrintMessage(arg0, 26, 1);
        arg0->state = 59;
        arg0->timer = 1;
    }
    return arg1;
}

s32 WifiClub_State_AskCancelSeeking(WifiClubApp *arg0, s32 arg1) {
    WifiClubRoom_UpdateLights(&arg0->room);
    WifiClub_PrintMessage(arg0, 10, 0);
    arg0->state = 40;
    return arg1;
}

s32 WifiClub_State_CreateCancelSeekingPrompt(WifiClubApp *arg0, s32 arg1) {
    WifiClubRoom_UpdateLights(&arg0->room);
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->yesNoMenu = Std_CreateYesNoMenu(arg0->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            arg0->yesNoPrompt = WifiClub_CreateYesNoPromptIgnoreTouch(arg0->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        arg0->state = 41;
    }
    return arg1;
}

s32 WifiClub_State_CancelSeekingPrompt(WifiClubApp *arg0, s32 arg1) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 var_r5 = Handle2dMenuInput_DeleteOnFinish(arg0->yesNoMenu, HEAP_ID_53);
        WifiClubRoom_UpdateLights(&arg0->room);
        if ((arg0->contactFriend == -1) && (WifiConnection_GetNewClientIndex() != -1)) {
            WifiConnection_SetVoiceChatEnabled(arg0->clubData->own.voiceChat);
            if (var_r5 == -1) {
                Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
            }
            var_r5 = 1;
        }
        if (var_r5 == -1) {
            return arg1;
        } else if (var_r5 == 0) {
            WifiClub_SetStatusAndSend(arg0, 16);
            CommState_SetWifiClubLinkMode();
            WifiClub_StopSeeking(arg0);
        } else {
            WifiClub_StopSeeking(arg0);
            WifiClub_StartSeeking(arg0, 1);
        }
    } else {
        enum YesNoResponse yesNoResponse = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
        WifiClubRoom_UpdateLights(&arg0->room);
        if ((arg0->contactFriend == -1) && (WifiConnection_GetNewClientIndex() != -1)) {
            WifiConnection_SetVoiceChatEnabled(arg0->clubData->own.voiceChat);
            yesNoResponse = YESNORESPONSE_NO;
        }
        if (yesNoResponse == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            WifiClub_SetStatusAndSend(arg0, 16);
            CommState_SetWifiClubLinkMode();
            WifiClub_StopSeeking(arg0);
        } else if (yesNoResponse == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            WifiClub_StopSeeking(arg0);
            WifiClub_StartSeeking(arg0, 1);
        } else {
            return arg1;
        }
    }
    arg0->state = 19;
    return arg1;
}

s32 WifiClub_State_OpenSeekMenu(WifiClubApp *arg0, s32 arg1) {
    s32 listLen = 3;
    ListMenuTemplate listMenuTemplate = WifiClubSeekMenuTemplate;
    WifiClubMenuItem *listMenuItems = WifiClubSeekMenuItems;

    if (WifiClub_IsPoffinCookingAvailable(arg0) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;

        s32 index = listLen - 1;
        listMenuItems[index] = WifiClubSeekItem_Cook;
    }
    if (WifiClub_HasBattleFrontier(arg0) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;

        s32 index = listLen - 1;
        listMenuItems[index] = WifiClubSeekItem_Frontier;
    }
    if (WifiClub_ArePlazaGamesAvailable(arg0) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;

        s32 index = listLen - 1;
        listMenuItems[index] = WifiClubSeekItem_PlazaGames;
    }
    listMenuTemplate.totalItems++;
    listMenuTemplate.maxShowed++;
    listMenuItems[listLen] = WifiClubSeekItem_Cancel;
    arg0->menuItems = ListMenuItems_New(listLen + 1, HEAP_ID_53);

    for (s32 i = 0; i < listLen + 1; i++) {
        ListMenuItems_AppendFromMsgData(arg0->menuItems, arg0->msgData, listMenuItems[i].strno, listMenuItems[i].value);
    }
    if (WindowIsInUse(&arg0->menuWindow) != 0) {
        EraseFrameAndWindow1(&arg0->menuWindow, 0);
        RemoveWindow(&arg0->menuWindow);
    }
    s32 height = (listLen + 1) * 2;
    AddWindowParameterized(arg0->bgConfig, &arg0->menuWindow, 2, 16, 3, 15, height, 13, 59 - height);
    DrawFrameAndWindow1(&arg0->menuWindow, 1, 473, 11);
    listMenuTemplate.items = arg0->menuItems;
    listMenuTemplate.window = &arg0->menuWindow;
    arg0->listMenu = ListMenuInit(&listMenuTemplate, 0, arg0->seekMenuCursor, HEAP_ID_53);
    ScheduleWindowCopyToVram(&arg0->menuWindow);
    WifiClub_PrintMessage(arg0, 8, 0);
    WifiClubRoom_UpdateLights(&arg0->room);
    arg0->state = 37;
    return arg1;
}

s32 WifiClub_State_SeekMenu(WifiClubApp *arg0, s32 arg1) {
    s32 sp0 = WifiClub_CountBattleReadyMons(arg0);
    s32 temp_r6 = WifiClub_GetCookingError(arg0);
    WifiClubRoom_UpdateLights(&arg0->room);
    if (CommState_HasWifiFailed() != 0) {
        EraseFrameAndWindow1(&arg0->menuWindow, 0);
        RemoveWindow(&arg0->menuWindow);
        DestroyListMenu(arg0->listMenu, NULL, &arg0->seekMenuCursor);
        ListMenuItems_Delete(arg0->menuItems);
        WifiClub_CloseMessage(arg0);
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        if (WifiClub_CheckIncomingContact(arg0) != 0) {
            arg0->state = 19;
            EraseFrameAndWindow1(&arg0->menuWindow, 0);
            RemoveWindow(&arg0->menuWindow);
            DestroyListMenu(arg0->listMenu, NULL, &arg0->seekMenuCursor);
            ListMenuItems_Delete(arg0->menuItems);
            WifiClub_CloseMessage(arg0);
        }
        return arg1;
    }
    s32 var_r4 = ListMenu_ProcessInput(arg0->listMenu);
    if (WifiClub_CheckIncomingContact(arg0) != 0) {
        var_r4 = -2;
    }
    switch (var_r4) {
    case -1:
        return arg1;
    case -2:
        arg0->state = 19;
        WifiClub_CloseMessage(arg0);
        PlaySE(SEQ_SE_DP_DECIDE);
        break;
    default:
        PlaySE(SEQ_SE_DP_DECIDE);
        if ((var_r4 == 19) && (temp_r6 != 0)) {
            if (temp_r6 == 1) {
                WifiClub_PrintMessage(arg0, 104, 0);
            } else if (temp_r6 == 3) {
                WifiClub_PrintMessage(arg0, 106, 0);
            } else {
                WifiClub_PrintMessage(arg0, 105, 0);
            }
            arg0->state = 29;
        } else if ((var_r4 == 15) && (sp0 < 2)) {
            WifiClub_PrintMessage(arg0, 89, 0);
            arg0->state = 29;
        } else if ((var_r4 == 1) && (sp0 < 2)) {
            WifiClub_PrintMessage(arg0, 103, 0);
            arg0->state = 29;
        } else if ((var_r4 == 0) || (var_r4 == 1) || (var_r4 == 29)) {
            EraseFrameAndWindow1(&arg0->menuWindow, 0);
            RemoveWindow(&arg0->menuWindow);
            DestroyListMenu(arg0->listMenu, NULL, &arg0->seekMenuCursor);
            ListMenuItems_Delete(arg0->menuItems);
            WifiClub_OpenSeekSubmenu(arg0, var_r4);
            arg0->state = 38;
            return arg1;
        } else {
            WifiClub_SetStatusAndSend(arg0, var_r4);
            arg0->state = 19;
            WifiClub_CloseMessage(arg0);
        }
        s32 var_r6 = 1;
        if ((WifiClub_IsGroupActivityStatus(var_r4) != 0) && (arg0->state == 19)) {
            arg0->state = 68;
            WifiClub_StartMatch(arg0, -1, var_r4);
            WifiClub_SetLinkModeForStatus(var_r4);
            WifiClub_SetStatusAndSend(arg0, var_r4);
            arg0->exitChoice = WifiClub_GetGroupActivityChoice(var_r4);
            var_r6 = 0;
        } else {
            WifiClub_StartMatch(arg0, -1, var_r4);
            WifiClub_SetLinkModeForStatus(var_r4);
        }
        if (arg0->state == 19) {
            WifiClub_StartSeeking(arg0, var_r6);
        }
    }

    EraseFrameAndWindow1(&arg0->menuWindow, 0);
    RemoveWindow(&arg0->menuWindow);
    DestroyListMenu(arg0->listMenu, NULL, &arg0->seekMenuCursor);
    ListMenuItems_Delete(arg0->menuItems);
    return arg1;
}

s32 WifiClub_State_StartRecruiting(WifiClubApp *arg0, s32 arg1) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    WifiClubRoom_UpdateLights(&arg0->room);
    if (WifiClub_CheckIncomingContact(arg0) != 0) {
        arg0->state = 19;
    }
    if (CommState_IsWifiInLobby() != 0) {
        CommState_SetErrorWatch(0, 1);
        sub_020378E4(0);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
        arg0->isRematch = 0;
        CommPlayerInfo_Init(arg0->saveData, 0);
        CommPlayerInfo_SendLocal();
        CommPlayerInfo_Accept(Comm_GetLocalNetId());
        arg1 = 2;
    }
    return arg1;
}

s32 WifiClub_OpenSeekSubmenu(WifiClubApp *arg0, s32 arg1) {
    ListMenuTemplate listMenuTemplate;
    WifiClubMenuItem *listMenuItems;
    s32 listLen;
    switch (arg1) {
    case 0:
        arg0->unk37C = 1;
        listMenuItems = WifiClubSingleBattleItems;
        listLen = NELEMS(WifiClubSingleBattleItems);
        listMenuTemplate = sBattleSubmenuTemplate;
        break;
    case 1:
        arg0->unk37C = 0;
        listMenuItems = WifiClubDoubleBattleItems;
        listLen = NELEMS(WifiClubDoubleBattleItems);
        listMenuTemplate = sBattleSubmenuTemplate;
        break;
    case 29:
        listMenuItems = WifiClubPlazaGameItems;
        listLen = NELEMS(WifiClubPlazaGameItems);
        listMenuTemplate = sPlazaGameSubmenuTemplate;
        arg0->unk37C = 2;
        break;
    }
    arg0->menuItems = ListMenuItems_New(listLen, HEAP_ID_53);
    for (s32 i = 0; i < listLen; i++) {
        if (listMenuItems[i].strno != msg_0778_00071) {
            ListMenuItems_AppendFromMsgData(arg0->menuItems, arg0->msgData, listMenuItems[i].strno, listMenuItems[i].value);
        } else {
            BufferWiFiPlazaActivityName(arg0->msgFormat, 0, i);
            ReadMsgDataIntoString(arg0->msgData, listMenuItems[i].strno, arg0->msgRaw);
            StringExpandPlaceholders(arg0->msgFormat, arg0->tmpString, arg0->msgRaw);
            ListMenuItems_AddItem(arg0->menuItems, arg0->tmpString, listMenuItems[i].value);
        }
    }
    if (WindowIsInUse(&arg0->menuWindow) != 0) {
        EraseFrameAndWindow1(&arg0->menuWindow, 0);
        RemoveWindow(&arg0->menuWindow);
    }
    u32 temp_r0 = listLen * 2;
    AddWindowParameterized(arg0->bgConfig, &arg0->menuWindow, 2, 16, 9, 15, temp_r0, 13, 59 - temp_r0);
    DrawFrameAndWindow1(&arg0->menuWindow, 1, 473, 11);
    listMenuTemplate.items = arg0->menuItems;
    listMenuTemplate.window = &arg0->menuWindow;
    arg0->listMenu = ListMenuInit(&listMenuTemplate, 0, (&arg0->unk376)[arg0->unk37C], HEAP_ID_53);
    ScheduleWindowCopyToVram(&arg0->menuWindow);
    return 1;
}

s32 WifiClub_State_SeekSubmenu(WifiClubApp *arg0, s32 arg1) {
    WifiClubRoom_UpdateLights(&arg0->room);
    if (CommState_HasWifiFailed() != 0) {
        EraseFrameAndWindow1(&arg0->menuWindow, 0);
        RemoveWindow(&arg0->menuWindow);
        DestroyListMenu(arg0->listMenu, 0, &arg0->unk376 + arg0->unk37C);
        ListMenuItems_Delete(arg0->menuItems);
        WifiClub_CloseMessage(arg0);
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    if (WifiClub_CheckIncomingContact(arg0) != 0) {
        arg0->state = 19;
        EraseFrameAndWindow1(&arg0->menuWindow, 0);
        RemoveWindow(&arg0->menuWindow);
        DestroyListMenu(arg0->listMenu, 0, &arg0->unk376 + arg0->unk37C);
        ListMenuItems_Delete(arg0->menuItems);
        WifiClub_CloseMessage(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    s32 temp_r0 = ListMenu_ProcessInput(arg0->listMenu);
    switch (temp_r0) {
    case -1:
        return arg1;
    case -2:
        PlaySE(SEQ_SE_DP_DECIDE);
        arg0->state = 36;
        break;
    default:
        PlaySE(SEQ_SE_DP_DECIDE);
        WifiClub_SetStatusAndSend(arg0, temp_r0);
        arg0->state = 19;
        WifiClub_CloseMessage(arg0);
    }
    if (arg0->state == 19) {
        s32 var_r7 = 1;
        if (WifiClub_IsGroupActivityStatus(temp_r0) != 0) {
            arg0->state = 68;
            WifiClub_StartMatch(arg0, -1, temp_r0);
            WifiClub_SetLinkModeForStatus(temp_r0);
            WifiClub_SetStatusAndSend(arg0, temp_r0);
            arg0->exitChoice = WifiClub_GetGroupActivityChoice(temp_r0);
            var_r7 = 0;
        } else {
            WifiClub_StartMatch(arg0, -1, temp_r0);
            CommState_SetSingleBattleWifiLinkMode();
        }
        WifiClub_StartSeeking(arg0, var_r7);
    }
    EraseFrameAndWindow1(&arg0->menuWindow, 0);
    RemoveWindow(&arg0->menuWindow);
    DestroyListMenu(arg0->listMenu, 0, &arg0->unk376 + arg0->unk37C);
    ListMenuItems_Delete(arg0->menuItems);
    return arg1;
}

s32 WifiClub_State_TalkToPlayer(WifiClubApp *arg0, s32 arg1) {
    u8 temp_r5 = WifiClubRoom_GetTalkPartnerId(&arg0->room);
    WifiClubRoomPerson *temp_r0 = WifiClub_FindPlayerObject(arg0, 0);
    GF_ASSERT(temp_r0 != 0);
    WifiClubRoomPerson *temp_r0_2 = WifiClub_FindPlayerObject(arg0, temp_r5);
    if (temp_r0_2 == 0) {
        WifiClub_BufferFriendName(arg0, temp_r5 - 1);
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 29;
        return arg1;
    }
    WifiClubRoom_StartTalk(&arg0->room, temp_r0_2, WifiClubRoomPerson_GetOppositeDirection(temp_r0));
    u32 temp_r7 = temp_r5 - 1;
    u16 temp_r5_2 = WifiClub_GetPlayerStatus(arg0, WifiClub_GetFriendStatusData(arg0, temp_r7));
    arg0->partnerStatus = temp_r5_2;
    WifiClub_BufferFriendName(arg0, temp_r7);
    s32 var_r1 = 0;

    if (temp_r5_2 == 15) {
        var_r1 = 3;
    } else if (temp_r5_2 == 19) {
        var_r1 = 4;
    } else if (temp_r5_2 == 18) {
        var_r1 = 83;
    } else if (temp_r5_2 == 21) {
        var_r1 = 5;
    } else if (temp_r5_2 == 20) {
        var_r1 = 84;
    } else if (temp_r5_2 == 23) {
        BufferWiFiPlazaActivityName(arg0->msgFormat, 1, 0);
        var_r1 = 6;
    } else if (temp_r5_2 == 22) {
        var_r1 = 85;
    } else if (temp_r5_2 == 25) {
        BufferWiFiPlazaActivityName(arg0->msgFormat, 1, 1);
        var_r1 = 6;
    } else if (temp_r5_2 == 24) {
        var_r1 = 85;
    } else if (temp_r5_2 == 27) {
        BufferWiFiPlazaActivityName(arg0->msgFormat, 1, 2);
        var_r1 = 6;
    } else if (temp_r5_2 == 26) {
        var_r1 = 85;
    } else {
        if (WifiClub_IsSeekingBattleStatus(temp_r5_2) != 0) {
            var_r1 = 2;
        } else if (temp_r5_2 == 8) {
            var_r1 = 82;
        } else if (WifiClub_IsInBattleStatus(temp_r5_2) != 0) {
            var_r1 = 81;
        } else if (temp_r5_2 == 1) {
            var_r1 = 86;
        } else {
            var_r1 = 7;
        }
    }
    WifiClub_PrintMessage(arg0, var_r1, 0);
    arg0->state = 43;
    return arg1;
}

s32 WifiClub_State_OpenApplyMenu(WifiClubApp *arg0, s32 arg1) {
    s32 i;
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    s32 temp_r0 = WifiClubRoom_GetTalkPartnerId(&arg0->room);
    WifiClubRoomPerson *temp_r6 = WifiClub_FindPlayerObject(arg0, temp_r0);
    if (WifiClub_CheckIncomingContact(arg0) != 0) {
        WifiClubRoom_EndTalk(&arg0->room, temp_r6);
        WifiClub_CloseMessage(arg0);
        arg0->state = 19;
        return arg1;
    }
    if (temp_r6 == 0) {
        WifiClubRoom_EndTalk(&arg0->room, temp_r6);
        DestroyListMenu(arg0->listMenu, 0, 0);
        WifiClub_BufferFriendName(arg0, temp_r0 - 1);
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 29;
        return arg1;
    }
    if (arg0->partnerStatus != WifiClub_GetPlayerStatus(arg0, WifiClub_GetFriendStatusData(arg0, temp_r0 - 1))) {
        WifiClubRoom_EndTalk(&arg0->room, temp_r6);
        WifiClub_BufferFriendName(arg0, temp_r0 - 1);
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 29;
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    s32 sp1C = 2;
    ListMenuTemplate listMenuTemplate = sApplyMenuTemplate;
    const WifiClubMenuItem *listMenuItems = sApplyMenuItems;
    WifiClubPlayerStatus *r6 = WifiClub_GetFriendStatusData(arg0, WifiClubRoom_GetTalkPartnerId(&arg0->room) - 1);
    u16 temp_r4 = WifiClub_GetPlayerStatus(arg0, r6);
    u8 r1 = r6->voiceChat;
    arg0->partnerVoiceChat = r1;
    u16 val = r1 + arg0->clubData->own.voiceChat;
    arg0->menuItems = ListMenuItems_New(2, HEAP_ID_53);

    for (i = 0; i < NELEMS(sApplyMenuItems); i++) {
        if (i == 0) {
            if ((WifiClub_IsBusyStatus(temp_r4) != 0) || (temp_r4 == 0) || (temp_r4 == 28) || (temp_r4 >= 29)) {
                listMenuTemplate.maxShowed--;
                listMenuTemplate.totalItems--;
                sp1C -= 1;
            } else if (temp_r4 == 16) {
                if (val == 2) {
                    ListMenuItems_AppendFromMsgData(arg0->menuItems, arg0->msgData, msg_0778_00036, listMenuItems[i].value);
                } else {
                    listMenuTemplate.maxShowed--;
                    listMenuTemplate.totalItems--;
                    sp1C -= 1;
                }
            } else {
                ListMenuItems_AppendFromMsgData(arg0->menuItems, arg0->msgData, listMenuItems[i].strno, listMenuItems[i].value);
            }
        } else {
            ListMenuItems_AppendFromMsgData(arg0->menuItems, arg0->msgData, listMenuItems[i].strno, listMenuItems[i].value);
        }
    }
    AddWindowParameterized(arg0->bgConfig, &arg0->menuWindow, 2, 16, ((3 - sp1C) * 2) + 11, 15, sp1C * 2, 13, 59);
    DrawFrameAndWindow1(&arg0->menuWindow, 1, 473, 11);
    listMenuTemplate.items = arg0->menuItems; // itemPrintFunc
    listMenuTemplate.window = &arg0->menuWindow;
    arg0->listMenu = ListMenuInit(&listMenuTemplate, 0, 0, HEAP_ID_53);
    ScheduleWindowCopyToVram(&arg0->menuWindow);
    arg0->state = 44;
    return arg1;
}

s32 WifiClub_IsPartyValid(WifiClubPlayerStatus *arg0) {
    for (s32 i = 0; i < 6; i++) {
        if (arg0->species[i] == 495) {
            return 0;
        }
        if (arg0->species[i] > 495) {
            return 0;
        }
        if (arg0->heldItems[i] > 536) {
            return 0;
        }
    }
    return 1;
}

s32 WifiClub_GetCookingError(WifiClubApp *arg0) {
    return 3;
}

void WifiClub_PlayRoomBgm(void) {
    WifiConnection_SetVoiceChatEnabled(0);
    if (WifiClub_IsRoomBgmPlaying() == FALSE) {
        IsNighttime();
        Sound_SetScene(0);
        Sound_SetSceneAndPlayBGM(11, SEQ_GS_POKESEN, 1);
        return;
    }
    u16 bgm = GF_GetCurrentPlayingBGM();
    GF_GetCurrentPlayingBGM();
    GF_SetVolumeBySeqNo(bgm, GF_GetVolumeBySeqNo());
}

BOOL WifiClub_IsRoomBgmPlaying(void) {
    u16 bgm = GF_GetCurrentPlayingBGM();
    if ((bgm != SEQ_GS_POKESEN) && (bgm != SEQ_GS_POKESEN)) {
        return FALSE;
    }
    return TRUE;
}

s32 WifiClub_State_ApplyMenu(WifiClubApp *arg0, s32 arg1) {
    s32 var_r6 = 0;
    s32 var_r7 = ListMenu_ProcessInput(arg0->listMenu);
    if (WifiClub_CheckIncomingContact(arg0) != 0) {
        var_r7 = -2;
    }
    s32 temp_r0 = WifiClubRoom_GetTalkPartnerId(&arg0->room);
    WifiClubRoomPerson *sp14 = WifiClub_FindPlayerObject(arg0, temp_r0);
    if (CommState_HasWifiFailed() != 0) {
        EraseFrameAndWindow1(&arg0->menuWindow, 0);
        RemoveWindow(&arg0->menuWindow);
        DestroyListMenu(arg0->listMenu, 0, NULL);
        ListMenuItems_Delete(arg0->menuItems);
        if (sp14 != 0) {
            WifiClubRoom_EndTalk(&arg0->room, sp14);
        }
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    switch (var_r7) {
    case -1:
        if (sp14 == 0) {
            DestroyListMenu(arg0->listMenu, 0, NULL);
            WifiClub_BufferFriendName(arg0, temp_r0 - 1);
            WifiClub_PrintMessage(arg0, 16, 0);
            arg0->state = 29;
            var_r6 = 1;
            break;
        } else {
            WifiClubPlayerStatus *temp_r0_2 = WifiClub_GetFriendStatusData(arg0, temp_r0 - 1);
            u32 r0 = WifiClub_GetPlayerStatus(arg0, temp_r0_2);
            u8 r6 = temp_r0_2->voiceChat;
            if ((arg0->partnerStatus != r0) || ((WifiClub_IsGroupActivityStatus(r0) == 0) && (arg0->partnerVoiceChat != r6))) {
                WifiClub_BufferFriendName(arg0, temp_r0 - 1);
                WifiClub_PrintMessage(arg0, 16, 0);
                arg0->state = 29;
                var_r6 = 1;
                break;
            }
            return arg1;
        }
    case -2:
        PlaySE(SEQ_SE_DP_DECIDE);
        arg0->state = 19;
        break;
    default:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (var_r7 == 1) {
            arg0->state = 19;
            arg0->partnerId = temp_r0;
            if (temp_r0 == 0) {
                break;
            } else {
                s32 sp10 = WifiClub_CountBattleReadyMons(arg0);
                s32 spC = WifiClub_GetCookingError(arg0);
                WifiClubPlayerStatus *sp8 = WifiClub_GetFriendStatusData(arg0, temp_r0 - 1);
                u32 temp_r7 = WifiClub_GetPlayerStatus(arg0, sp8);
                s32 sp4 = sp8->voiceChat;
                if ((arg0->partnerStatus != temp_r7) || ((WifiClub_IsGroupActivityStatus(temp_r7) == 0) && (arg0->partnerVoiceChat != sp4))) {
                    WifiClub_BufferFriendName(arg0, temp_r0 - 1);
                    WifiClub_PrintMessage(arg0, 16, 0);
                    arg0->state = 29;
                    var_r6 = 1;
                } else if (WifiClub_IsPartyValid(sp8) == 0) {
                    WifiClub_BufferFriendName(arg0, temp_r0 - 1);
                    WifiClub_PrintMessage(arg0, 16, 0);
                    arg0->state = 29;
                    var_r6 = 1;
                } else if ((WifiClub_StatusNeedsTwoMons(temp_r7) != 0) && (sp10 < 2)) {
                    if (temp_r7 == 15) {
                        WifiClub_PrintMessage(arg0, 89, 0);
                    } else {
                        WifiClub_PrintMessage(arg0, 103, 0);
                    }
                    arg0->state = 29;
                    var_r6 = 1;
                } else if ((temp_r7 == 19) && (spC != 0)) {
                    if (spC == 1) {
                        WifiClub_PrintMessage(arg0, 104, 0);
                    } else if (spC == 3) {
                        WifiClub_PrintMessage(arg0, 106, 0);
                    } else {
                        WifiClub_PrintMessage(arg0, 105, 0);
                    }
                    arg0->state = 29;
                    var_r6 = 1;
                } else if ((WifiClub_IsGroupActivityStatus(temp_r7) == 0) && (sp4 != arg0->clubData->own.voiceChat)) {
                    if (sp4 != 0) {
                        WifiClub_PrintMessage(arg0, 134, 0);
                    } else {
                        WifiClub_PrintMessage(arg0, 135, 0);
                    }
                    WifiClub_SetStatusAndSend(arg0, WifiClub_GetCounterpartStatus(temp_r7));
                    arg0->state = 63;
                    var_r6 = 1;
                } else {
                    u32 temp_r0_3 = WifiClub_GetCounterpartStatus(temp_r7);
                    if (temp_r0_3 != 29) {
                        if (WifiClub_GetFriendOnlineState(temp_r0 - 1) == 6) {
                            if (WifiClub_StartMatch(arg0, temp_r0 - 1, temp_r0_3) != 0) {
                                arg0->matchTimer = 1800;
                                WifiClub_SetLinkModeForStatus(temp_r0_3);
                                WifiClub_SetStatusAndSend(arg0, temp_r0_3);
                                WifiClub_BufferFriendName(arg0, temp_r0 - 1);
                                WifiClub_PrintMessage(arg0, 17, 0);
                                GF_ASSERT(arg0->waitingIcon == NULL);
                                arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
                                if (temp_r0_3 != 1) {
                                    arg0->state = 45;

                                } else {
                                    arg0->matchTimer = 1800;
                                    arg0->state = 21;
                                }
                                var_r6 = 1;
                            } else {
                                WifiClub_BufferFriendName(arg0, temp_r0 - 1);
                                WifiClub_PrintMessage(arg0, 16, 0);
                                arg0->state = 29;
                                var_r6 = 1;
                            }
                        } else {
                            WifiClub_BufferFriendName(arg0, temp_r0 - 1);
                            WifiClub_PrintMessage(arg0, 16, 0);
                            arg0->state = 29;
                            var_r6 = 1;
                        }
                    }
                }
            }
            break;
        }
        if (var_r7 == 2) {
            arg0->state = 54;
            break;
        }
    }
    if (var_r6 == 0) {
        WifiClub_CloseMessage(arg0);
    }
    EraseFrameAndWindow1(&arg0->menuWindow, 0);
    RemoveWindow(&arg0->menuWindow);
    DestroyListMenu(arg0->listMenu, 0, NULL);
    ListMenuItems_Delete(arg0->menuItems);
    if (sp14 != 0) {
        WifiClubRoom_EndTalk(&arg0->room, sp14);
    }
    return arg1;
}

s32 WifiClub_State_WaitMatchAccepted(WifiClubApp *arg0, s32 arg1) {
    arg0->matchTimer -= 1;
    if (arg0->matchTimer < 0) {
        arg0->state = 70;
    } else if (CommState_GetWifiMatchStatus() == 3) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 18, 0);
        arg0->state = 27;
    } else if (CommState_GetWifiMatchStatus() == 5) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 15, 0);
        arg0->state = 27;
    } else if ((CommState_GetWifiMatchStatus() == 4) || (CommState_WasWifiDisconnected() != 0)) {
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 18, 0);
        arg0->state = 27;
    } else if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
    } else if (CommState_GetWifiMatchStatus() == 1) {
        u32 temp_r6 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
        if (WifiClub_IsGroupActivityStatus(temp_r6) == 0) {
            arg0->state = 49;
            WifiClub_CloseMessage(arg0);
            CommPlayerInfo_Init(arg0->saveData, 0);
            arg0->timer = 30;
        } else if (Comm_IsNetIdConnected(0) == 1) {
            WifiClub_CloseMessage(arg0);
            CommPlayerInfo_Init(arg0->saveData, 0);
            CommState_SetErrorWatch(0, 1);
            WifiClub_SetLinkModeForStatus(temp_r6);
            arg0->exitChoice = WifiClub_GetGroupActivityChoice(temp_r6);
            sub_020378E4(0);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
            arg0->isRematch = 0;
            arg1 = 2;
        } else if (Comm_IsWifiConnected() == 1) {
            WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
            WifiClub_PrintMessage(arg0, 18, 0);
            arg0->state = 27;
        }
    }
    return arg1;
}

s32 WifiClub_State_Dummy66(WifiClubApp *arg0, s32 arg1) {
    return arg1;
}

s32 WifiClub_State_Dummy67(WifiClubApp *arg0, s32 arg1) {
    return arg1;
}

s32 WifiClub_State_MatchTimeout(WifiClubApp *arg0, s32 arg1) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
        return arg1;
    }
    WifiClub_RestoreVoiceChatSetting(arg0);
    WifiClub_SetStatusAndSend(arg0, 16);
    CommState_LeaveWifiMatch();
    arg0->contactFriend = -1;
    arg0->state = 19;
    WifiClub_StopSeeking(arg0);
    WifiClub_CloseMessage(arg0);
    return arg1;
}

s32 WifiClub_State_Dummy46(WifiClubApp *arg0, s32 arg1) {
    return arg1;
}

s32 WifiClub_State_Dummy47(WifiClubApp *arg0, s32 arg1) {
    return arg1;
}

s32 WifiClub_State_Contacted(WifiClubApp *arg0, s32 arg1) {
    WifiClubPlayerStatus *temp_r7 = WifiClub_GetFriendStatusData(arg0, WifiConnection_GetTargetFriendIndex());
    u32 temp_r4 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
    u32 temp_r0 = WifiClub_GetPlayerStatus(arg0, temp_r7);

    s32 r1 = arg0->clubData->own.voiceChat;
    if (temp_r4 == 12 && temp_r0 == 5) {
        arg0->state = 49;
    } else if (temp_r4 == 13 && temp_r0 == 6) {
        arg0->state = 49;
    } else if (temp_r4 == 14 && temp_r0 == 7) {
        arg0->state = 49;
    } else if (temp_r4 == 9 && temp_r0 == 2) {
        arg0->state = 49;
    } else if (temp_r4 == 10 && temp_r0 == 3) {
        arg0->state = 49;
    } else if (temp_r4 == 11 && temp_r0 == 4) {
        arg0->state = 49;
    } else if (temp_r4 == 15 && temp_r0 == 8) {
        arg0->state = 49;
    } else if (temp_r4 == 19 && temp_r0 == 18) {
        arg0->state = 49;
    } else if (temp_r4 == 21 && temp_r0 == 20) {
        arg0->state = 49;
    } else if (temp_r4 == 23 && temp_r0 == 22) {
        arg0->state = 49;
    } else if (temp_r4 == 25 && temp_r0 == 24) {
        arg0->state = 49;
    } else if (temp_r4 == 27 && temp_r0 == 26) {
        arg0->state = 49;
    } else if (temp_r4 == 16 && temp_r0 == 1 && r1 > 0) {
        arg0->state = 22;
        return arg1;
    } else {
        if (CommState_HasWifiFailed() != 0) {
            WifiClub_HandleWifiError(arg0);
            return arg1;
        }
        WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(arg0, 18, 0);
        arg0->state = 29;
        WifiClub_SetStatusAndSend(arg0, 16);
        CommState_LeaveWifiMatch();
        return arg1;
    }
    WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
    WifiClub_PrintMessage(arg0, 11, 0);
    CommPlayerInfo_Init(arg0->saveData, 0);
    arg0->state = 49;
    arg0->timer = 30;
    return arg1;
}

s32 WifiClub_CheckMatchLost(WifiClubApp *arg0) {
    if (CommState_GetWifiMatchStatus() >= 3) {
        WifiClub_BufferFriendName(arg0, arg0->contactFriend);
        WifiClub_PrintMessage(arg0, 18, 0);
        arg0->state = 27;
    } else if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(arg0);
    } else {
        WifiClubPlayerStatus *temp_r6 = WifiClub_GetFriendStatusData(arg0, WifiConnection_GetTargetFriendIndex());
        u32 temp_r4 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
        u32 temp_r0 = WifiClub_GetPlayerStatus(arg0, temp_r6);
        if ((temp_r4 != WifiClub_GetCounterpartStatus(temp_r0)) && (temp_r4 != temp_r0)) {
            WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
            WifiClub_PrintMessage(arg0, 16, 0);
            arg0->state = 27;

        } else {
            return 0;
        }
    }
    arg0->isRematch = 0;
    return 1;
}

s32 WifiClub_State_SyncStart(WifiClubApp *arg0, s32 arg1) {
    if ((CommState_GetWifiMatchStatus() >= 4) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0)) {
        if (arg0->isRematch != 0) {
            WifiClub_PrintMessage(arg0, 101, 0);
        } else {
            WifiClub_BufferFriendName(arg0, arg0->contactFriend);
            WifiClub_PrintMessage(arg0, 18, 0);
        }
        arg0->isRematch = 0;
        arg0->state = 27;
    } else if (WifiClub_CheckMatchLost(arg0) == 0) {
        u32 temp_r1 = arg0->timer;
        if (temp_r1 == 0) {
            if (LinkSync_IsSynced(13) != 0) {
                LinkSync_ClearPackets();
                LinkSync_StartSync(14);
                arg0->isRematch = 0;
                arg0->state = 50;
            }
        } else {
            arg0->timer = temp_r1 - 1;
            if (arg0->timer == 0) {
                LinkSync_StartSync(13);
            }
        }
    }
    return arg1;
}

s32 WifiClub_State_SendStatus(WifiClubApp *arg0, s32 arg1) {
    if ((WifiClub_CheckMatchLost(arg0) == 0) && (LinkSync_IsSynced(14) != 0)) {
        s16 r1 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
        if (LinkSync_SendPacket(Comm_GetLocalNetId(), &r1) != 0) {
            arg0->state = 51;
        }
    }
    return arg1;
}

s32 WifiClub_State_ReceiveStatus(WifiClubApp *arg0, s32 arg1) {
    s32 var_r0;
    if (Comm_GetLocalNetId() == 0) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    u16 *temp_r4 = LinkSync_GetPacket(var_r0);
    if ((WifiClub_CheckMatchLost(arg0) == 0) && (temp_r4 != NULL)) {
        u16 temp_r6 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
        u16 r0 = WifiClub_GetCounterpartStatus(temp_r6);
        if ((temp_r4[0] == r0) || (temp_r4[0] == temp_r6)) {
            LinkSync_StartSync(15);
            arg0->state = 52;
        } else {
            WifiClub_BufferFriendName(arg0, WifiConnection_GetTargetFriendIndex());
            WifiClub_PrintMessage(arg0, 16, 0);
            arg0->state = 27;
        }
    }
    return arg1;
}

s32 WifiClub_State_SendPlayerInfo(WifiClubApp *arg0, s32 arg1) {
    if ((WifiClub_CheckMatchLost(arg0) == 0) && (LinkSync_IsSynced(15) != 0)) {
        CommPlayerInfo_SendLocal();
        CommState_SetErrorWatch(1, 1);
        LinkSync_StartSync(18);
        arg0->state = 53;
    }
    return arg1;
}

s32 WifiClub_IsMessageFinished(WifiClubApp *arg0) {
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        return 1;
    }
    if (WindowIsInUse(&arg0->msgWindow) == 0) {
        return 1;
    }
    return 0;
}

s32 WifiClub_State_StartActivity(WifiClubApp *arg0, s32 arg1) {
    ListMenuTemplate spC; // Unknown struct of size 0x20
    if ((WifiClub_CheckMatchLost(arg0) == 0) && (LinkSync_IsSynced(18) != 0) && (WifiClub_IsMessageFinished(arg0) == 1)) {
        Comm_RegisterLinkedFriends(arg0->saveData, &spC, HEAP_ID_53);
        WifiClub_CloseMessage(arg0);
        u32 temp_r0 = WifiClub_GetPlayerStatus(arg0, WifiClub_GetFriendStatusData(arg0, WifiConnection_GetTargetFriendIndex()));
        u32 var_r1 = temp_r0;
        if ((var_r1 == 15) || (var_r1 == 8)) {
            var_r1 = 8;
            arg0->exitChoice = 7;
        } else if (temp_r0 - 18 <= 1) {
            var_r1 = 18;
            arg0->exitChoice = 11;
        } else if (var_r1 - 20 <= 1) {
            var_r1 = 20;
            arg0->exitChoice = 12;
        } else if (var_r1 - 22 <= 1) {
            var_r1 = 22;
            arg0->exitChoice = 13;
        } else if (var_r1 - 24 <= 1) {
            var_r1 = 24;
            arg0->exitChoice = 14;
        } else if (var_r1 - 26 <= 1) {
            var_r1 = 26;
            arg0->exitChoice = 15;
        } else if (var_r1 == 9 || var_r1 == 2) {
            var_r1 = 2;
            arg0->exitChoice = 1;
        } else if (var_r1 == 10 || var_r1 == 3) {
            var_r1 = 3;
            arg0->exitChoice = 2;
        } else if (var_r1 == 11 || var_r1 == 4) {
            var_r1 = 4;
            arg0->exitChoice = 3;
        } else if (var_r1 == 12 || var_r1 == 5) {
            var_r1 = 5;
            arg0->exitChoice = 4;
        } else if (var_r1 == 13 || var_r1 == 6) {
            var_r1 = 6;
            arg0->exitChoice = 5;
        } else if (var_r1 == 14 || var_r1 == 7) {
            var_r1 = 7;
            arg0->exitChoice = 6;
        }
        WifiClub_SetStatusAndSend(arg0, var_r1);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
        arg1 = 2;
    }
    return arg1;
}

s32 WifiClub_State_ReloadRoom(WifiClubApp *arg0, s32 arg1) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 3);
    arg0->unk364 = 0;
    arg0->state = WifiClub_FadeOutToEnterRoom();
    return arg1;
}

s32 WifiClub_State_ToMessageWait(WifiClubApp *arg0, s32 arg1) {
    arg0->state = 55;
    return arg1;
}

s32 WifiClub_State_MessageWait(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    if (WifiClub_IsMatchConnected(arg0) != 0) {
        arg0->state = 56;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        arg0->state = 56;
    }
    return arg1;
}

s32 WifiClub_State_CloseMessage(WifiClubApp *arg0, s32 arg1) {
    WifiClub_CloseMessage(arg0);
    arg0->state = 19;
    return arg1;
}

s32 WifiClub_State_CreateDisconnectPrompt(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->yesNoMenu = Std_CreateYesNoMenu(arg0->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            arg0->yesNoPrompt = WifiClub_CreateYesNoPromptIgnoreTouch(arg0->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        arg0->state = 58;
    }
    return arg1;
}

s32 WifiClub_State_DisconnectPrompt(WifiClubApp *arg0, s32 arg1) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 var_r5 = Handle2dMenuInput_DeleteOnFinish(arg0->yesNoMenu, HEAP_ID_53);
        if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
            return arg1;
        }
        if (WifiClub_CheckIncomingContact(arg0) != 0) {
            if (var_r5 == -1) {
                Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
            }
            var_r5 = -2;
        }
        if (var_r5 == -1) {
            return arg1;
        }
        if (var_r5 == 0) {
            WifiClub_PrintMessage(arg0, 26, 1);
            arg0->state = 59;
            arg0->timer = 1;
        } else {
            WifiClub_CloseMessage(arg0);
            arg0->state = 19;
            WifiClub_StopSeeking(arg0);
        }
    } else {
        YesNoResponse yesNoResponse = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
        if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
            return arg1;
        }
        if (WifiClub_CheckIncomingContact(arg0) != 0) {
            yesNoResponse = YESNORESPONSE_NO;
        }
        if (yesNoResponse == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            WifiClub_PrintMessage(arg0, 26, 1);
            arg0->state = 59;
            arg0->timer = 1;
        } else if (yesNoResponse == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            WifiClub_CloseMessage(arg0);
            arg0->state = 19;
            WifiClub_StopSeeking(arg0);
        } else {
            return arg1;
        }
    }
    return arg1;
}

s32 WifiClub_State_Disconnecting(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    if (arg0->timer == 1) {
        arg0->timer = 0;
        CommState_EndWifiSession();
    }
    if (CommState_Exists() == 0) {
        FriendRoster_Compact(arg0->friendRoster);
        WifiClub_PrintMessage(arg0, 27, 1);
        arg0->state = 60;
        arg0->timer = 30;
    }
    return arg1;
}

s32 WifiClub_State_Disconnected(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->exitChoice = 8;
        arg0->state = 34;
        WifiClub_CloseMessage(arg0);
    }
    return arg1;
}

s32 WifiClub_State_CreateRematchPrompt(WifiClubApp *arg0, s32 arg1) {
    if (LinkSync_IsSynced(16) == 0) {
        return arg1;
    }
    CommState_SetErrorWatch(0, 1);
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->yesNoMenu = Std_CreateYesNoMenu(arg0->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            arg0->yesNoPrompt = WifiClub_CreateYesNoPrompt(arg0->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        arg0->state = 62;
    }
    return arg1;
}

s32 WifiClub_State_RematchPrompt(WifiClubApp *arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->textPrinterId) != 0) {
        return arg1;
    }
    if ((CommState_IsWifiJoiningLobby() != 0) || (CommState_WasWifiDisconnected() != 0) || (CommState_GetWifiMatchStatus() >= 3)) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
        }
        WifiClub_PrintMessage(arg0, 101, 0);
        arg0->state = 28;
    } else {
        if (CommState_HasWifiFailed() != 0) {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
            } else {
                YesNoPrompt_Destroy(arg0->yesNoPrompt);
            }
            WifiClub_HandleWifiError(arg0);
        } else {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                s32 temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->yesNoMenu, HEAP_ID_53);
                if (temp_r0 == -1) {
                    return arg1;
                } else if (temp_r0 == 0) {
                    WifiClub_PrintMessage(arg0, 138, 0);
                    GF_ASSERT(arg0->waitingIcon == 0);
                    arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
                    arg0->isRematch = 1;
                    arg0->state = 49;
                    arg0->timer = 30;
                } else {
                    WifiClub_CloseMessage(arg0);
                    CommPlayerInfo_Free();
                    CommState_SendWifiDisconnect();
                    arg0->timer = 20;
                    arg0->state = 65;
                }
            } else {
                s32 temp_r0_2 = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
                if (temp_r0_2 == 1) {
                    YesNoPrompt_Destroy(arg0->yesNoPrompt);
                    WifiClub_PrintMessage(arg0, 138, 0);
                    GF_ASSERT(arg0->waitingIcon == 0);
                    arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
                    arg0->isRematch = 1;
                    arg0->state = 49;
                    arg0->timer = 30;
                } else if (temp_r0_2 == 2) {
                    YesNoPrompt_Destroy(arg0->yesNoPrompt);
                    WifiClub_CloseMessage(arg0);
                    CommPlayerInfo_Free();
                    CommState_SendWifiDisconnect();
                    arg0->timer = 20;
                    arg0->state = 65;
                } else {
                    return arg1;
                }
            }
            if (arg0->isRematch == 0) {
                arg0->clubData = CommState_GetWifiClubData();
                arg0->clubData->own.voiceChat = arg0->clubData->own.voiceChatSetting;
            }
        }
    }
    return arg1;
}

s32 WifiClub_State_CreateVoiceChatModePrompt(WifiClubApp *arg0, s32 arg1) {
    u32 temp_r0 = WifiClubRoom_GetTalkPartnerId(&arg0->room);
    if (WifiClub_FindPlayerObject(arg0, temp_r0) == 0) {
        WifiClub_BufferFriendName(arg0, temp_r0 - 1);
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 29;
        return arg1;
    }
    WifiClubPlayerStatus *temp_r0_2 = WifiClub_GetFriendStatusData(arg0, temp_r0 - 1);
    u32 r0 = WifiClub_GetPlayerStatus(arg0, temp_r0_2);
    u8 r7 = temp_r0_2->voiceChat;
    if ((arg0->partnerStatus != r0) || (arg0->partnerVoiceChat != r7)) {
        WifiClub_BufferFriendName(arg0, temp_r0 - 1);
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 29;
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->yesNoMenu = Std_CreateYesNoMenu(arg0->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            arg0->yesNoPrompt = WifiClub_CreateYesNoPromptIgnoreTouch(arg0->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        arg0->state = 64;
    }
    return arg1;
}

s32 WifiClub_State_VoiceChatModePrompt(WifiClubApp *arg0, s32 arg1) {
    s32 temp_r0 = WifiClubRoom_GetTalkPartnerId(&arg0->room);
    if (WifiClub_FindPlayerObject(arg0, temp_r0) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
        }
        WifiClub_BufferFriendName(arg0, temp_r0 - 1);
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 29;
        return arg1;
    }
    WifiClubPlayerStatus *temp_r0_2 = WifiClub_GetFriendStatusData(arg0, temp_r0 - 1);
    u32 r0 = WifiClub_GetPlayerStatus(arg0, temp_r0_2);
    u8 r7 = temp_r0_2->voiceChat;
    if ((arg0->partnerStatus != r0) || (arg0->partnerVoiceChat != r7)) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
        }
        WifiClub_BufferFriendName(arg0, temp_r0 - 1);
        WifiClub_PrintMessage(arg0, 16, 0);
        arg0->state = 29;
        return arg1;
    }
    if (CommState_HasWifiFailed() != 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
        }
        WifiClub_HandleWifiError(arg0);
    } else {
        if (CommState_GetWifiMatchStatus() >= 3) {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                Clear2dMenuWindowAndDelete(arg0->yesNoMenu, HEAP_ID_53);
            } else {
                YesNoPrompt_Destroy(arg0->yesNoPrompt);
            }
            WifiClub_PrintMessage(arg0, 18, 0);
            CommState_LeaveWifiMatch();
            arg0->state = 29;
            return arg1;
        } else if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            s32 menuInputResult = Handle2dMenuInput_DeleteOnFinish(arg0->yesNoMenu, HEAP_ID_53);
            if (menuInputResult == -1) {
                return arg1;
            }
            if (menuInputResult == 0) {
                u32 temp_r0 = WifiClub_GetCounterpartStatus(arg0->partnerStatus);
                if (temp_r0 != 29) {
                    if (WifiClub_GetFriendOnlineState(arg0->partnerId - 1) == 6) {
                        WifiClub_ToggleVoiceChat(arg0);
                        if (WifiClub_StartMatch(arg0, arg0->partnerId - 1, temp_r0) != 0) {
                            arg0->matchTimer = 1800;
                            WifiClub_SetLinkModeForStatus(temp_r0);
                            sub_020378E4(0);
                            WifiClub_SetStatusAndSend(arg0, temp_r0);
                            WifiClub_BufferFriendName(arg0, arg0->partnerId - 1);
                            WifiClub_PrintMessage(arg0, 17, 0);
                            GF_ASSERT(arg0->waitingIcon == 0);
                            arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
                            if (temp_r0 != 1) {
                                arg0->state = 45;
                                return arg1;
                            }
                            arg0->matchTimer = 1800;
                            arg0->state = 21;
                            return arg1;
                        }
                    }
                }
            }
            WifiClub_CloseMessage(arg0);
            WifiClub_SetStatusAndSend(arg0, 16);
            WifiClub_RestoreVoiceChatSetting(arg0);
            WifiClub_PrintOwnStatus(arg0);
            arg0->state = 19;
            return arg1;
        }
        YesNoResponse yesNoResponse = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
        if (yesNoResponse == 1) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            u32 temp_r0 = WifiClub_GetCounterpartStatus(arg0->partnerStatus);
            if (temp_r0 != 29) {
                if (WifiClub_GetFriendOnlineState(arg0->partnerId - 1) == 6) {
                    WifiClub_ToggleVoiceChat(arg0);
                    if (WifiClub_StartMatch(arg0, arg0->partnerId - 1, temp_r0) != 0) {
                        arg0->matchTimer = 1800;
                        WifiClub_SetLinkModeForStatus(temp_r0);
                        sub_020378E4(0);
                        WifiClub_SetStatusAndSend(arg0, temp_r0);
                        WifiClub_BufferFriendName(arg0, arg0->partnerId - 1);
                        WifiClub_PrintMessage(arg0, 17, 0);
                        GF_ASSERT(arg0->waitingIcon == 0);
                        arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 482);
                        if (temp_r0 != 1) {
                            arg0->state = 45;
                            return arg1;
                        }
                        arg0->matchTimer = 1800;
                        arg0->state = 21;
                        return arg1;
                    }
                }
            }
        } else if (yesNoResponse == 2) {
            YesNoPrompt_Destroy(arg0->yesNoPrompt);
            WifiClub_CloseMessage(arg0);
            WifiClub_SetStatusAndSend(arg0, 16);
            WifiClub_RestoreVoiceChatSetting(arg0);
            WifiClub_PrintOwnStatus(arg0);
            arg0->state = 19;
            return arg1;
        } else {
            return arg1;
        }
    }
    return arg1;
}

s32 WifiClub_State_Exit(WifiClubApp *arg0, s32 arg1) {
    if (CommState_Exists() == 0) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
        arg1 = 2;
    }
    return arg1;
}

void WifiClub_PrintMessage(WifiClubApp *arg0, s32 arg1, s32 arg2) {
    u32 textSpeed_int;
    if (arg2 != 0) {
        textSpeed_int = 1;
    } else {
        Options *options = Save_PlayerData_GetOptionsAddr(arg0->saveData);
        textSpeed_int = Options_GetTextFrameDelay(options);
    }
    u8 textSpeed = textSpeed_int;
    WifiClub_DeleteWaitingIcon(arg0);
    if (WindowIsInUse(&arg0->noticeWindow) != 0) {
        ClearFrameAndWindow2(&arg0->noticeWindow, 0);
        RemoveWindow(&arg0->noticeWindow);
    }
    if (WindowIsInUse(&arg0->msgWindow) != 0) {
        ClearFrameAndWindow2(&arg0->msgWindow, 0);
        RemoveWindow(&arg0->msgWindow);
    }
    s32 temp_r0 = arg0->textPrinterId;
    if ((temp_r0 != 8) && (TextPrinterCheckActive(temp_r0) != 0)) {
        RemoveTextPrinter(arg0->textPrinterId);
        arg0->textPrinterId = 8;
    }
    AddWindowParameterized(arg0->bgConfig, &arg0->msgWindow, 2, 2, 19, 27, 4, 12, 301);
    if (arg2 != 0) {
        ReadMsgDataIntoString(arg0->connectionMsgData, arg1, arg0->msgRaw);
    } else {
        ReadMsgDataIntoString(arg0->msgData, arg1, arg0->msgRaw);
    }
    StringExpandPlaceholders(arg0->msgFormat, arg0->msgExpanded, arg0->msgRaw);
    FillWindowPixelBuffer(&arg0->msgWindow, 15);
    DrawFrameAndWindow2(&arg0->msgWindow, 1, 482, 10);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    arg0->textPrinterId = AddTextPrinterParameterized(&arg0->msgWindow, 1, arg0->msgExpanded, 0, 0, textSpeed, 0);
    ScheduleWindowCopyToVram(&arg0->msgWindow);
}

void WifiClub_ShowConnectionNotice(WifiClubApp *arg0, s32 arg1) {
    WifiClub_DeleteWaitingIcon(arg0);
    if (WindowIsInUse(&arg0->noticeWindow) != 0) {
        ClearFrameAndWindow2(&arg0->noticeWindow, 0);
        RemoveWindow(&arg0->noticeWindow);
    }
    if (WindowIsInUse(&arg0->msgWindow) != 0) {
        ClearFrameAndWindow2(&arg0->msgWindow, 0);
        RemoveWindow(&arg0->msgWindow);
    }
    s32 temp_r0 = arg0->textPrinterId;
    if ((temp_r0 != 8) && (TextPrinterCheckActive(temp_r0) != 0)) {
        RemoveTextPrinter(arg0->textPrinterId);
        arg0->textPrinterId = 8;
    }
    AddWindowParameterized(arg0->bgConfig, &arg0->noticeWindow, 2, 4, 4, 23, 16, 12, 105);
    ReadMsgDataIntoString(arg0->connectionMsgData, arg1, arg0->msgRaw);
    StringExpandPlaceholders(arg0->msgFormat, arg0->msgExpanded, arg0->msgRaw);
    FillWindowPixelBuffer(&arg0->noticeWindow, 15);
    DrawFrameAndWindow1(&arg0->noticeWindow, 1, 473, 11);
    arg0->textPrinterId = AddTextPrinterParameterized(&arg0->noticeWindow, 1, arg0->msgExpanded, 0, 0, 255, 0);
    ScheduleWindowCopyToVram(&arg0->noticeWindow);
}

void WifiClub_BufferFriendName(WifiClubApp *arg0, s32 arg1) {
    if (arg1 != -1) {
        PlayerProfile *playerProfile = PlayerProfile_New(HEAP_ID_53);
        Save_Profile_PlayerName_Set(playerProfile, FriendRoster_GetPlayerName(arg0->friendRoster, arg1));
        BufferPlayersName(arg0->msgFormat, 0, playerProfile);
        Heap_Free(playerProfile);
    }
}

void WifiClub_CloseMessage(WifiClubApp *arg0) {
    s32 temp_r0;

    WifiClub_DeleteWaitingIcon(arg0);
    temp_r0 = arg0->textPrinterId;
    if ((temp_r0 != 8) && (TextPrinterCheckActive(temp_r0) != 0)) {
        RemoveTextPrinter(arg0->textPrinterId);
        arg0->textPrinterId = 8;
    }
    if (WindowIsInUse(&arg0->msgWindow) != 0) {
        ClearFrameAndWindow2(&arg0->msgWindow, 0);
        RemoveWindow(&arg0->msgWindow);
    }
}

void WifiClub_SetStatusAndSend(WifiClubApp *arg0, u32 arg1) {
    if (arg0->clubData != NULL) {
        WifiClub_SetStatus(arg0, arg1);
        WifiConnection_SetOwnStatusData(arg0->clubData, 36);
    }
}

void WifiClub_SetStatus(WifiClubApp *arg0, u32 arg1) {
    if (arg0->clubData != NULL) {
        u32 r0 = WifiClub_GetPlayerStatus(arg0, &arg0->clubData->own);
        if (r0 != arg1) {
            arg0->clubData->own.status = arg1;
            if ((WifiClub_IsInBattleStatus(arg1) == 0) && (arg1 != 8) && (arg1 != 20) && (arg1 != 22) && (arg1 != 24) && (arg1 != 26) && (arg1 != 18)) {
                if (arg1 == 1) {
                    GF_SndStartFadeOutBGM(0, 30);
                } else if (arg1 == 16) {
                    WifiConnection_SetVoiceChatEnabled(0);
                    if (WifiClub_IsRoomBgmPlaying() == 1) {
                        GF_SndStartFadeInBGM(120, 30, 1);
                    }
                }
            }
        }
        WifiClub_PrintOwnStatus(arg0);
    }
}

u8 WifiClub_ToggleVoiceChat(WifiClubApp *arg0) {
    arg0->clubData->own.voiceChat = 1 - arg0->clubData->own.voiceChat;
    WifiConnection_SetOwnStatusData(arg0->clubData, 36);
    return arg0->clubData->own.voiceChat;
}

u8 WifiClub_ToggleVoiceChatSetting(WifiClubApp *arg0) {
    arg0->clubData->own.voiceChatSetting = 1 - arg0->clubData->own.voiceChatSetting;
    arg0->clubData->own.voiceChat = arg0->clubData->own.voiceChatSetting;
    WifiConnection_SetOwnStatusData(arg0->clubData, 36);
    return arg0->clubData->own.voiceChatSetting;
}

u8 WifiClub_RestoreVoiceChatSetting(WifiClubApp *arg0) {
    arg0->clubData->own.voiceChat = arg0->clubData->own.voiceChatSetting;
    WifiConnection_SetOwnStatusData(arg0->clubData, 36);
    return arg0->clubData->own.voiceChatSetting;
}

void WifiClub_DeleteWaitingIcon(WifiClubApp *arg0) {
    if (arg0->waitingIcon != 0) {
        WaitingIcon_DeleteWithoutRestore();
        arg0->waitingIcon = 0;
        if (WindowIsInUse(&arg0->msgWindow) != 0) {
            ClearFrameAndWindow2(&arg0->msgWindow, 0);
            RemoveWindow(&arg0->msgWindow);
        }
    }
}

void WifiClub_CancelRosterSelection(WifiClubApp *arg0, enum HeapID heapID) {
    WifiClub_DeselectRosterFriend(arg0);
    WifiClubRoom_HideFriendMarker(&arg0->room);
    WifiClub_RedrawRoster(arg0, heapID);
}

void WifiClub_CancelRosterSelectionOf(WifiClubApp *arg0, s32 arg1, enum HeapID heapID) {
    if (arg1 == WifiClub_GetSelectedRosterFriend(arg0)) {
        WifiClub_CancelRosterSelection(arg0, heapID);
    }
}

s32 WifiClub_IsRosterScreenActive(WifiClubApp *arg0) {
    if (arg0->roster.slotHitboxes != 0) {
        return 1;
    }
    return 0;
}

void WifiClub_InitRosterScreen(WifiClubApp *arg0, NARC *arg1, enum HeapID arg2) {
    memset(&arg0->roster, 0, sizeof(WifiClubRoster));
    GfGfx_EngineBTogglePlanes(1, 1);
    GfGfx_EngineBTogglePlanes(2, 1);
    GfGfx_EngineBTogglePlanes(4, 1);
    GfGfx_EngineBTogglePlanes(8, 0);
    arg0->roster.slotHitboxes = TouchHitboxController_Create(sRosterSlotHitboxes, 8, (TouchHitboxControllerCallback)WifiClub_RosterHitboxCallback, arg0, arg2);
    arg0->roster.msgFormat = MessageFormat_New(arg2);
    WifiClub_LoadRosterGraphics(arg0, arg1, arg2);
    arg0->roster.redrawAll = 1;
    WifiClub_SetRosterPagePalette(arg0);
    WifiClub_DrawRosterPage(arg0);
}

void WifiClub_FreeRosterScreen(WifiClubApp *arg0) {
    if (WifiClub_IsRosterScreenActive(arg0) != 0) {
        WifiClub_FreeRosterGraphics(arg0);
        TouchHitboxController_Destroy(arg0->roster.slotHitboxes);
        arg0->roster.slotHitboxes = NULL;
        MessageFormat_Delete(arg0->roster.msgFormat);
        arg0->roster.msgFormat = NULL;
        GfGfx_EngineBTogglePlanes(1, 1);
        GfGfx_EngineBTogglePlanes(2, 0);
        GfGfx_EngineBTogglePlanes(4, 0);
        GfGfx_EngineBTogglePlanes(8, 0);
    }
}

u8 WifiClub_UpdateRosterScreen(WifiClubApp *arg0, enum HeapID heapID) {
    u32 var_r5 = WifiClubRoom_GetTileUnderPlayer(&arg0->room);
    u32 temp_r0 = WifiClubRoom_GetTileBelowPlayer(&arg0->room);
    if ((temp_r0 >= 3) && (temp_r0 <= 6)) {
        var_r5 = temp_r0;
    }
    if ((var_r5 >= 3) && (var_r5 <= 6)) {
        if (arg0->roster.page != var_r5 - 3) {
            arg0->roster.page = var_r5 - 3;
            PlaySE(SEQ_SE_DP_SELECT5);
            WifiClub_SetRosterPagePalette(arg0);
            arg0->roster.redrawSlots = 1;
            arg0->roster.redrawAll = 1;
        }
    }
    if (arg0->roster.profileState == 1) {
        arg0->roster.profileState = 2;
        arg0->roster.profilePage = 0;
        WifiClub_DrawProfilePage(arg0, heapID);
        WifiClub_ShowProfileButtons(arg0);
    }
    if (arg0->roster.profileState == 0) {
        TouchHitboxController_IsTriggered(arg0->roster.slotHitboxes);
        WifiClub_AnimateRosterSelection(arg0);
        if (arg0->roster.redrawSlots == 1) {
            WifiClub_DrawRosterPage(arg0);
            arg0->roster.redrawSlots = 0;
        }
    }
    if ((u8)(arg0->roster.profileState + 254) <= 1) {
        u32 temp_r0_2 = WifiClub_GetPressedProfileButton(arg0);
        if (WifiClub_ShouldCloseProfile(arg0, temp_r0_2) == 1) {
            arg0->roster.prevSelectedFriend = arg0->roster.selectedFriend;
            arg0->roster.selectedFriend = 0;
            arg0->roster.selectAnimStep = 0;
            arg0->roster.redrawSlots = 1;
            arg0->roster.redrawAll = 1;
            arg0->roster.profileState = 0;
            WifiClub_HideProfileButtons(arg0);
            PlaySE(SEQ_SE_DP_DECIDE);
        } else if (temp_r0_2 != 0) {
            PlaySE(SEQ_SE_DP_DECIDE);
            WifiClub_ChangeProfilePage(arg0, temp_r0_2);
            WifiClub_DrawProfilePage(arg0, heapID);
        }
    }
    return arg0->roster.selectedFriend;
}

void WifiClub_RedrawRosterIfNeeded(WifiClubApp *arg0) {
    if ((arg0->roster.profileState == 0) && (arg0->roster.redrawSlots == 1)) {
        WifiClub_DrawRosterPage(arg0);
        arg0->roster.redrawSlots = 0;
    }
}

s32 WifiClub_ShouldCloseProfile(WifiClubApp *arg0, s32 arg1) {
    if (arg0->roster.profileState == 3) {
        return 0;
    }
    if ((gSystem.heldKeys & (PAD_KEY_DOWN | PAD_KEY_UP | PAD_KEY_LEFT | PAD_KEY_RIGHT)) || (gSystem.newKeys & (PAD_BUTTON_X | PAD_BUTTON_A | PAD_BUTTON_B)) || (arg1 == 2)) {
        return 1;
    }
    return 0;
}

u8 WifiClub_GetSelectedRosterFriend(WifiClubApp *arg0) {
    return arg0->roster.selectedFriend;
}

void WifiClub_DeselectRosterFriend(WifiClubApp *arg0) {
    if (WifiClub_IsRosterScreenActive(arg0) == 1) {
        arg0->roster.selectedFriend = 0;
        arg0->roster.selectAnimStep = 0;
        arg0->roster.profileState = 0;
        arg0->roster.redrawSlots = 1;
        arg0->roster.redrawAll = 1;
        WifiClub_HideProfileButtons(arg0);
    }
}

void WifiClub_ChangeProfilePage(WifiClubApp *arg0, s32 arg1) {
    u32 var_r1;
    if (WifiClub_HasBattleFrontier(arg0) == 1) {
        var_r1 = 7;
    } else {
        var_r1 = 2;
    }
    if (arg1 == 3) {
        arg0->roster.profilePage = (arg0->roster.profilePage + 1) % var_r1;
        return;
    }
    s8 *ptr = &arg0->roster.profilePage;
    ptr[0]--;
    if (arg0->roster.profilePage < 0) {
        ptr[0] += var_r1;
    }
}

void WifiClub_RosterHitboxCallback(s32 arg0, enum HeapID arg1, WifiClubApp *arg2) {
    s32 temp_r0 = arg2->roster.page * 8 + arg0;
    if ((arg2->roster.slotStates[temp_r0] == 2) && (arg2->roster.selectedFriend == 0) && (arg1 == 0)) {
        arg2->roster.selectedFriend = temp_r0 + 1;
        PlaySE(SEQ_SE_DP_DECIDE);
    }
}

void WifiClub_LoadRosterGraphics(WifiClubApp *arg0, NARC *arg1, enum HeapID arg2) {
    s32 i;
    s32 j;
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 4);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 17, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 128, arg2);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 20, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_4_OFFSET, 160, arg2);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 16, arg0->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, arg2);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 19, arg0->bgConfig, GF_BG_LYR_SUB_2, 48, 0, 0, arg2);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 18, arg0->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, arg2);
    arg0->roster.slotScreenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(arg1, 21, 0, &arg0->roster.slotScreen, arg2);
    WifiClub_OffsetScreenTiles(arg0->roster.slotScreen, 48);

    for (i = 0; i < 7; i++) {
        arg0->roster.pageScreensRaw[i] = GfGfxLoader_GetScrnDataFromOpenNarc(arg1, i + 22, 0, &arg0->roster.pageScreens[i], arg2);
        WifiClub_OffsetScreenTiles(arg0->roster.pageScreens[i], 48);
    }

    arg0->roster.tabCoverRaw = GfGfxLoader_GetScrnDataFromOpenNarc(arg1, 29, 0, &arg0->roster.tabCoverScreen, arg2);
    WifiClub_OffsetScreenTiles(arg0->roster.tabCoverScreen, 48);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, arg2);

    s32 sp20;
    s32 r5;
    for (i = 0; i < 8; i++) {
        InitWindow(&arg0->roster.nameWindows[i]);
        r5 = i / 4;
        sp20 = i % 4;

        AddWindowParameterized(arg0->bgConfig, &arg0->roster.nameWindows[i], 5, r5 * 16 + 4, sp20 * 6 + 1, 9, 3, 15, 1 + (i * 27));
        FillWindowPixelBuffer(&arg0->roster.nameWindows[i], 0);
        ScheduleWindowCopyToVram(&arg0->roster.nameWindows[i]);

        for (j = 0; j < 2; j++) {
            InitWindow(&arg0->roster.iconWindows[i][j]);
            AddWindowParameterized(arg0->bgConfig, &arg0->roster.iconWindows[i][j], 5, r5 * 16 + 1 + j * 12, sp20 * 6 + 1, 2, 3, 13, 6 * (j + i * 2) + 218);
            FillWindowPixelBuffer(&arg0->roster.iconWindows[i][j], 0);
            ScheduleWindowCopyToVram(&arg0->roster.iconWindows[i][j]);
        }
    }
    AddWindowParameterized(arg0->bgConfig, &arg0->roster.profileWindow, 7, 1, 1, 30, 21, 15, 1);
    FillWindowPixelBuffer(&arg0->roster.profileWindow, 0);
    ScheduleWindowCopyToVram(&arg0->roster.profileWindow);
    WifiClub_CreateProfileButtons(arg0, arg1, arg2);
}

void WifiClub_FreeRosterGraphics(WifiClubApp *arg0) {
    s32 i;
    WifiClub_DeleteProfileButtons(arg0);
    for (i = 0; i < 8; i++) {
        RemoveWindow(&arg0->roster.nameWindows[i]);
        for (s32 j = 0; j < 2; j++) {
            RemoveWindow(&arg0->roster.iconWindows[i][j]);
        }
    }
    RemoveWindow(&arg0->roster.profileWindow);
    Heap_Free(arg0->roster.slotScreenRaw);
    for (i = 0; i < 7; i++) {
        Heap_Free(arg0->roster.pageScreensRaw[i]);
    }
    Heap_Free(arg0->roster.tabCoverRaw);
}

void WifiClub_OffsetScreenTiles(NNSG2dScreenData *arg0, s32 arg1) {
    s32 i;
    s32 j;
    u16 *ip = (u16 *)&arg0->rawData[0];
    s32 width = arg0->screenWidth / 8;
    s32 height = arg0->screenHeight / 8;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            ip[i * width + j] += arg1;
        }
    }
}

void WifiClub_SetRosterSlotState(WifiClubApp *arg0, s32 arg1, u32 arg2) {
    GF_ASSERT(arg1 != 0);
    GF_ASSERT(arg2 != 0);
    GF_ASSERT(arg2 < 3);
    arg0->roster.slotStates[arg1 - 1] = arg2;
    arg0->roster.redrawSlots = 1;
}

void WifiClub_ClearRosterSlotState(WifiClubApp *arg0, s32 arg1) {
    GF_ASSERT(arg1 != 0);
    arg0->roster.slotStates[arg1 - 1] = 0;
    arg0->roster.redrawSlots = 1;
}

u8 WifiClub_GetRosterSlotState(WifiClubApp *arg0, s32 arg1) {
    GF_ASSERT(arg1 != 0);
    return arg0->roster.slotStates[arg1 - 1];
}

void WifiClub_RedrawRoster(WifiClubApp *arg0, enum HeapID heapID) {
    if (arg0->roster.profileState != 0) {
        WifiClub_DrawProfilePage(arg0, heapID);
        return;
    }
    arg0->roster.redrawAll = 1;
    WifiClub_DrawRosterPage(arg0);
}

void WifiClub_RequestRosterRedraw(WifiClubApp *arg0) {
    arg0->roster.redrawAll = 1;
    arg0->roster.redrawSlots = 1;
}

void WifiClub_SetRosterPagePalette(WifiClubApp *arg0) {
    BgTilemapRectChangePalette(arg0->bgConfig, 4, 0, 0, 32, 24, arg0->roster.page);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 4);
}

void WifiClub_DrawRosterPage(WifiClubApp *arg0) {
    s32 sp1C;
    s32 sp18;
    if (arg0->roster.redrawAll == 1) {
        arg0->roster.redrawAll = 0;
        sp18 = 1;
        FillBgTilemapRect(arg0->bgConfig, 6, 0, 0, 0, 32, 24, 0);
    } else {
        sp18 = 0;
    }
    for (s32 i = 0; i < 8; i++) {
        s32 temp_r4 = i + (arg0->roster.page * 8) + 1;
        if (arg0->roster.slotStates[temp_r4 - 1] != 0) {
            if (FriendRoster_GetEntryField(arg0->friendRoster, temp_r4 - 1, 8) == 0) {
                sp1C = 1;
            } else {
                sp1C = 0;
            }
        } else {
            sp1C = 2;
        }
        s32 r2 = i / 4;
        s32 temp_ip = i % 4;
        u32 var_r5;
        if (temp_r4 == arg0->roster.selectedFriend) {
            var_r5 = arg0->roster.selectAnimStep;
        } else {
            var_r5 = 0;
        }
        s32 var_r1;
        if ((temp_r4 == arg0->roster.selectedFriend) || (temp_r4 == arg0->roster.prevSelectedFriend)) {
            var_r1 = 1;
        } else {
            var_r1 = 0;
        }
        if (sp18 == 1 || var_r1 == 1) {
            WifiClub_DrawRosterSlotFrame(arg0, r2 << 4, 6 * temp_ip, sp1C, var_r5);
            if (sp1C != 2) {
                WifiClub_PrintRosterSlotName(arg0, &arg0->roster.nameWindows[i], temp_r4, var_r5, i);
                WifiClub_DrawRosterSlotIcons(arg0, &arg0->roster.iconWindows[i][0], temp_r4, var_r5, i);
            } else {
                FillWindowPixelBuffer(&arg0->roster.nameWindows[i], 0);
                ScheduleWindowCopyToVram(&arg0->roster.nameWindows[i]);
                for (s32 j = 0; j < 2; j++) {
                    FillWindowPixelBuffer(&arg0->roster.iconWindows[i][j], 0);
                    ScheduleWindowCopyToVram(&arg0->roster.iconWindows[i][j]);
                }
            }
        }
    }
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6);
    GfGfx_EngineBTogglePlanes(2, 1);
    GfGfx_EngineBTogglePlanes(8, 0);
}

void WifiClub_DrawProfilePage(WifiClubApp *arg0, enum HeapID heapID) {
    s32 var_r4;
    if (FriendRoster_GetEntryField(arg0->friendRoster, arg0->roster.selectedFriend - 1, 8) == 1) {
        var_r4 = 7;
    } else {
        var_r4 = 8;
    }
    NNSG2dScreenData *temp_r3 = arg0->roster.pageScreens[arg0->roster.profilePage];
    BG_LoadScreenTilemapData(arg0->bgConfig, 6, &temp_r3->rawData[0], temp_r3->szByte);
    BgTilemapRectChangePalette(arg0->bgConfig, 6, 0, 0, 32, 24, var_r4);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 7);
    if (WifiClub_HasBattleFrontier(arg0) == 0) {
        WifiClub_CoverFrontierPageTabs(arg0);
    }
    FillWindowPixelBuffer(&arg0->roster.profileWindow, 0);
    WifiClubProfilePageFuncs[arg0->roster.profilePage](arg0, heapID);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6);
    ScheduleWindowCopyToVram(&arg0->roster.profileWindow);
    GfGfx_EngineBTogglePlanes(2, 0);
    GfGfx_EngineBTogglePlanes(8, 1);
}

void WifiClub_DrawProfileSummary(WifiClubApp *arg0, enum HeapID heapID) {
    u32 color;
    s32 temp_r4 = arg0->roster.selectedFriend - 1;
    if (FriendRoster_GetEntryField(arg0->friendRoster, temp_r4, 8) == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    WifiClub_BufferRosterFriendName(arg0, temp_r4);
    // {STRVAR_1 3, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00042, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 1, arg0->tmpString, 32, 8, 255, color, 0);
    WifiClubPlayerStatus *sp10 = WifiClub_GetFriendStatusData(arg0, temp_r4);
    u32 temp_r0 = WifiClub_GetPlayerStatus(arg0, sp10);
    ReadMsgDataIntoString(arg0->msgData, WifiClub_GetStatusLabel(temp_r0, &color), arg0->msgRaw);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->msgRaw, 104, 8, 255, color, 0);
    PlayerProfile *playerProfile = PlayerProfile_New(HEAP_ID_53);
    Save_Profile_PlayerName_Set(playerProfile, FriendRoster_GetGroupName(arg0->friendRoster, temp_r4));
    BufferPlayersName(arg0->roster.msgFormat, 0, playerProfile);
    Heap_Free(playerProfile);
    // GROUP
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00043, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 32, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // {STRVAR_1 3, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00150, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);

    u32 width = 232 - FontID_String_GetWidth(0, arg0->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, 32, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // BATTLE RECORD
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00044, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 56, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(arg0->roster.msgFormat, 0, FriendRoster_GetEntryField(arg0->friendRoster, temp_r4, 1), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    // W: {STRVAR_1 53, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00045, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 120, 56, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(arg0->roster.msgFormat, 0, FriendRoster_GetEntryField(arg0->friendRoster, temp_r4, 2), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    // L: {STRVAR_1 53, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00046, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);

    width = 232 - FontID_String_GetWidth(0, arg0->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, 56, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // POKéMON TRADES
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00047, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 80, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(arg0->roster.msgFormat, 0, FriendRoster_GetEntryField(arg0->friendRoster, temp_r4, 3), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    // {STRVAR_1 53, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00048, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);

    width = 232 - FontID_String_GetWidth(0, arg0->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, 80, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    if (WifiClub_IsPoffinCookingAvailable(arg0) == 1) {
        // POFFIN SESSIONS
        ReadMsgDataIntoString(arg0->msgData, msg_0778_00049, arg0->tmpString);
        AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 104, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
        BufferIntegerAsString(arg0->roster.msgFormat, 0, FriendRoster_GetEntryField(arg0->friendRoster, temp_r4, 9), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        // {STRVAR_1 53, 0, 0}
        ReadMsgDataIntoString(arg0->msgData, msg_0778_00050, arg0->msgRaw);
        StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);

        width = 232 - FontID_String_GetWidth(0, arg0->tmpString, 0);
        AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, 104, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    }
    // DATE LAST PLAYED
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00051, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 128, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    s32 temp_r0_2 = FriendRoster_GetEntryField(arg0->friendRoster, temp_r4, 6);
    if (temp_r0_2 != 0) {
        BufferIntegerAsString(arg0->roster.msgFormat, 2, temp_r0_2, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferIntegerAsString(arg0->roster.msgFormat, 0, FriendRoster_GetEntryField(arg0->friendRoster, temp_r4, 4), 4, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferMonthNameAbbr(arg0->roster.msgFormat, 1, FriendRoster_GetEntryField(arg0->friendRoster, temp_r4, 5));
        // {STRVAR_34 16, 1, 0} {STRVAR_1 51, 2, 0}, {STRVAR_1 53, 0, 0}
        ReadMsgDataIntoString(arg0->msgData, msg_0778_00074, arg0->msgRaw);
        StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);

        width = 232 - FontID_String_GetWidth(0, arg0->tmpString, 0);
        AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, 128, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    }
    WifiClub_DrawStatusIconToBg(arg0->bgConfig, &arg0->statusIcons, 6, 2, 2, temp_r0);

    s32 var_r1;
    if (sp10->voiceChat != 0) {
        var_r1 = 8;
    } else {
        var_r1 = 1;
    }
    WifiClub_DrawIconToBg(arg0->bgConfig, &arg0->statusIcons, 6, 28, 2, var_r1, 0);
}

void WifiClub_DrawProfileBattleTower(WifiClubApp *arg0, enum HeapID heapID) {
    s32 temp_r4 = arg0->roster.selectedFriend - 1;
    // BATTLE TOWER
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00052, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    WifiClub_GetFrontierStreakLabel(arg0, arg0->tmpString, 0, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00065, 113, temp_r4, 120, 24);
    // RECORD
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00059, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00065, 112, temp_r4, 120, 48);
}

void WifiClub_DrawProfileBattleFactory(WifiClubApp *arg0, enum HeapID heapID) {
    s32 temp_r4 = arg0->roster.selectedFriend - 1;
    // BATTLE FACTORY
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00053, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    // LEVEL 50
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00060, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // RENTAL/TRADE
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00062, arg0->tmpString);

    u32 width = 232 - FontID_String_GetWidth(0, arg0->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_GetFrontierStreakLabel(arg0, arg0->tmpString, 1, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00065, 115, temp_r4, 104, 48);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00066, 117, temp_r4, 212, 48);
    // RECORD
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00059, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 64, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00065, 114, temp_r4, 104, 64);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00066, 116, temp_r4, 212, 64);
    // OPEN LEVEL
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00061, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 88, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // RENTAL/TRADE
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00062, arg0->tmpString);

    width = 232 - FontID_String_GetWidth(0, arg0->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, 88, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_GetFrontierStreakLabel(arg0, arg0->tmpString, 2, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 112, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00065, 119, temp_r4, 104, 112);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00066, 121, temp_r4, 212, 112);
    // RECORD
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00059, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 128, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00065, 118, temp_r4, 104, 128);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00066, 120, temp_r4, 212, 128);
}

void WifiClub_DrawProfileBattleCastle(WifiClubApp *arg0, enum HeapID heapID) {
    s32 temp_r4 = arg0->roster.selectedFriend - 1;
    // BATTLE CASTLE
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00054, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    // CASTLE POINTS
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00063, arg0->tmpString);

    u32 width = 180 - (FontID_String_GetWidth(0, arg0->tmpString, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_GetFrontierStreakLabel(arg0, arg0->tmpString, 3, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00068, 135, temp_r4, 104, 48);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00067, 136, temp_r4, 212, 48);
    // RECORD
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00059, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 80, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00068, 134, temp_r4, 104, 80);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00067, 138, temp_r4, 212, 80);
}

void WifiClub_DrawProfileBattleHall(WifiClubApp *arg0, enum HeapID arg1) {
    FrontierSave *frontierSave = Save_Frontier_GetStatic(arg0->saveData);
    s32 temp_r4 = arg0->roster.selectedFriend - 1;
    // BATTLE HALL
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00055, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    String *temp_r0 = GetSpeciesName(FrontierSave_GetStat(frontierSave, 124, temp_r4), arg1);
    u32 width = 33 - (FontID_String_GetWidth(0, temp_r0, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, temp_r0, width, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    String_Delete(temp_r0);
    WifiClub_GetFrontierStreakLabel(arg0, arg0->tmpString, 4, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00065, 123, temp_r4, 108, 48);
    // RECORD
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00059, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 80, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00065, 122, temp_r4, 108, 80);
}

void WifiClub_DrawProfileBattleArcade(WifiClubApp *arg0, enum HeapID heapID) {
    s32 temp_r4 = arg0->roster.selectedFriend - 1;
    // BATTLE ARCADE
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00056, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    WifiClub_GetFrontierStreakLabel(arg0, arg0->tmpString, 5, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00069, 143, temp_r4, 112, 24);
    // RECORD
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00059, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(arg0, msg_0778_00069, 142, temp_r4, 112, 48);
}

void WifiClub_DrawProfilePlazaGames(WifiClubApp *arg0, enum HeapID heapID) {
    FriendRoster *temp_r6 = Save_FriendRoster_Get(arg0->saveData);
    s32 temp_r4 = arg0->roster.selectedFriend - 1;
    // PLAZA GAMES PLAYED
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00070, arg0->tmpString);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    BufferWiFiPlazaActivityName(arg0->roster.msgFormat, 0, 0);
    // {STRVAR_3 0, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00071, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // {STRVAR_1 53, 0, 0}
    WifiClub_PrintCenteredNumber(arg0, msg_0778_00072, FriendRoster_GetEntryField(temp_r6, temp_r4, 10), 164, 24);
    BufferWiFiPlazaActivityName(arg0->roster.msgFormat, 0, 1);
    // {STRVAR_3 0, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00071, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // {STRVAR_1 53, 0, 0}
    WifiClub_PrintCenteredNumber(arg0, msg_0778_00072, FriendRoster_GetEntryField(temp_r6, temp_r4, 11), 164, 48);
    BufferWiFiPlazaActivityName(arg0->roster.msgFormat, 0, 2);
    // {STRVAR_3 0, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00071, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, 8, 72, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // {STRVAR_1 53, 0, 0}
    WifiClub_PrintCenteredNumber(arg0, msg_0778_00072, FriendRoster_GetEntryField(temp_r6, temp_r4, 12), 164, 72);
}

void WifiClub_CoverFrontierPageTabs(WifiClubApp *arg0) {
    for (s32 i = 0; i < 5; i++) {
        CopyToBgTilemapRect(arg0->bgConfig, 6, i + 26, 0, 1, 1, &arg0->roster.tabCoverScreen->rawData[0], 0, 0, arg0->roster.tabCoverScreen->screenWidth / 8, arg0->roster.tabCoverScreen->screenHeight / 8);
    }
    BgTilemapRectChangePalette(arg0->bgConfig, 6, 26, 0, 5, 1, 6);
}

void WifiClub_PrintFrontierStat(WifiClubApp *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    WifiClub_PrintCenteredNumber(arg0, arg1, FrontierSave_GetStat(Save_Frontier_GetStatic(arg0->saveData), arg2, arg3), arg4, arg5);
}

void WifiClub_GetFrontierStreakLabel(WifiClubApp *arg0, String *arg1, u32 arg2, s32 arg3) {

    GF_ASSERT(arg2 < 6);
    s32 var_r1;
    if (FrontierSave_GetStat(Save_Frontier_GetStatic(arg0->saveData), sFrontierStreakFlagStats[arg2], arg3) == 0) {
        // PREV
        var_r1 = msg_0778_00057;
    } else {
        // CURRENT
        var_r1 = msg_0778_00058;
    }
    ReadMsgDataIntoString(arg0->msgData, var_r1, arg1);
}

void WifiClub_PrintCenteredNumber(WifiClubApp *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    BufferIntegerAsString(arg0->roster.msgFormat, 0, arg2, 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ReadMsgDataIntoString(arg0->msgData, arg1, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);
    u32 width = arg3 - (FontID_String_GetWidth(0, arg0->tmpString, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&arg0->roster.profileWindow, 0, arg0->tmpString, width, arg4, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
}

void WifiClub_DrawRosterSlotFrame(WifiClubApp *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    GF_ASSERT(arg4 < 4);
    if (arg4 == 3) {
        arg4 = 1;
    }
    if (arg4 < 2) {
        CopyToBgTilemapRect(arg0->bgConfig, 6, arg1, arg2, 16, 6, &arg0->roster.slotScreen->rawData, (u16)(arg4 << 4), (u16)(arg3 * 6), 32, 18);
    } else {
        s64 offset = 18 << 4;
        CopyToBgTilemapRect(arg0->bgConfig, 6, arg1, arg2, 16, 6, &arg0->roster.slotScreen->rawData + offset, 0, (u16)(arg3 * 6), 16, 18);
    }
    BgTilemapRectChangePalette(arg0->bgConfig, 6, arg1, arg2, 16, 6, arg3 + 4);
}

s32 WifiClub_GetStatusLabel(u32 arg0, u32 *color) {
    *color = MAKE_TEXT_COLOR(1, 2, 0);
    s32 ret = 0;
    switch (arg0) {
    case 1:
        ret = msg_0778_00036;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        ret = msg_0778_00027;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 9:
        ret = msg_0778_00096;
        break;
    case 10:
        ret = msg_0778_00097;
        break;
    case 11:
        ret = msg_0778_00095;
        break;
    case 12:
        ret = msg_0778_00099;
        break;
    case 13:
        ret = msg_0778_00100;
        break;
    case 14:
        ret = msg_0778_00098;
        break;
    case 8:
        ret = msg_0778_00029;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 15:
        ret = msg_0778_00028;
        break;
    case 18:
    case 19:
        ret = msg_0778_00092;
        break;
    case 20:
        ret = msg_0778_00033;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 21:
        ret = msg_0778_00032;
        break;
    case 22:
    case 24:
    case 26:
        ret = msg_0778_00035;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 23:
    case 25:
    case 27:
        ret = msg_0778_00034;
        break;
    case 16:
        ret = msg_0778_00079;
        break;
    default:
        ret = msg_0778_00092;
    }
    return ret;
}

void WifiClub_PrintRosterSlotName(WifiClubApp *arg0, Window *arg1, s32 arg2, s32 arg3, s32 arg4) {
    FillWindowPixelBuffer(arg1, 0);
    u8 temp_r7 = sRosterSlotTextY[arg3];
    s32 sp14 = arg2 - 1;
    s32 color;
    if (FriendRoster_GetEntryField(arg0->friendRoster, sp14, 8) == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    WifiClub_BufferRosterFriendName(arg0, sp14);
    // {STRVAR_1 3, 0, 0}
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00042, arg0->msgRaw);
    StringExpandPlaceholders(arg0->roster.msgFormat, arg0->tmpString, arg0->msgRaw);
    AddTextPrinterParameterizedWithColor(arg1, 1, arg0->tmpString, 0, temp_r7, 255, color, 0);
    ScheduleWindowCopyToVram(arg1);
}

void WifiClub_DrawRosterSlotIcons(WifiClubApp *arg0, Window *arg1, s32 arg2, u32 arg3, u32 arg4) {
    s32 var_r0;
    WifiClubPlayerStatus *spC = WifiClub_GetFriendStatusData(arg0, arg2 - 1);
    u32 sp8 = WifiClub_GetPlayerStatus(arg0, spC);
    u32 temp_r7 = sRosterSlotTextY[arg3];
    for (s32 i = 0; i < 2; i++) {
        FillWindowPixelBuffer(&arg1[i], 0);
        if (i == 0) {
            WifiClub_DrawStatusIconToWindow(&arg1[i], &arg0->statusIcons, 0, temp_r7, sp8);
        } else {
            if (spC->voiceChat != 0) {
                var_r0 = 8;
            } else {
                var_r0 = 1;
            }
            WifiClub_DrawIconToWindow(&arg1[i], &arg0->statusIcons, 0, temp_r7, var_r0, 0);
        }
        ScheduleWindowCopyToVram(&arg1[i]);
    }
}

void WifiClub_AnimateRosterSelection(WifiClubApp *arg0) {
    if (arg0->roster.selectedFriend != 0) {
        arg0->roster.selectAnimTimer++;
        if (arg0->roster.selectAnimTimer >= sRosterSelectAnimDurations[arg0->roster.selectAnimStep]) {
            arg0->roster.selectAnimTimer = 0;
            arg0->roster.selectAnimStep++;
            arg0->roster.redrawSlots = 1;
            if (arg0->roster.selectAnimStep >= 4) {
                arg0->roster.selectAnimTimer = 0;
                arg0->roster.selectAnimStep = 0;
                arg0->roster.profileState = 1;
            }
        }
    }
}

void WifiClub_CreateProfileButtons(WifiClubApp *arg0, NARC *arg1, enum HeapID arg2) {
    SpriteTemplate spriteTemplateList[3] = {
        {
         .spriteList = NULL,
         .header = NULL,
         .position = { .x = FX32_CONST(40), .y = FX32_CONST(428), .z = 0 },
         .scale = { .x = FX32_ONE, .y = FX32_ONE, .z = FX32_ONE },
         .rotation = 0,
         .drawPriority = 128,
         .whichScreen = NNS_G2D_VRAM_TYPE_2DSUB,
         .heapID = HEAP_ID_DEFAULT,
         },
        {
         .spriteList = NULL,
         .header = NULL,
         .position = { .x = FX32_CONST(128), .y = FX32_CONST(428), .z = 0 },
         .scale = { .x = FX32_ONE, .y = FX32_ONE, .z = FX32_ONE },
         .rotation = 0,
         .drawPriority = 128,
         .whichScreen = NNS_G2D_VRAM_TYPE_2DSUB,
         .heapID = HEAP_ID_DEFAULT,
         },
        {
         .spriteList = NULL,
         .header = NULL,
         .position = { .x = FX32_CONST(224), .y = FX32_CONST(428), .z = 0 },
         .scale = { .x = FX32_ONE, .y = FX32_ONE, .z = FX32_ONE },
         .rotation = 0,
         .drawPriority = 128,
         .whichScreen = NNS_G2D_VRAM_TYPE_2DSUB,
         .heapID = HEAP_ID_DEFAULT,
         },
    };
    GfGfx_EngineBTogglePlanes(16, 0);
    arg0->roster.buttonResources[0] = AddCharResObjFromOpenNarc(arg0->gfxResMan[0], arg1, NARC_a_0_4_3, FALSE, 30, NNS_G2D_VRAM_TYPE_2DSUB, arg2);
    arg0->roster.buttonResources[1] = AddPlttResObjFromOpenNarc(arg0->gfxResMan[1], arg1, NARC_a_0_4_4, FALSE, 30, NNS_G2D_VRAM_TYPE_2DSUB, 8, arg2);
    arg0->roster.buttonResources[2] = AddCellOrAnimResObjFromOpenNarc(arg0->gfxResMan[2], arg1, NARC_fielddata_mapmatrix_map_matrix, FALSE, 30, GF_GFX_RES_TYPE_CELL, arg2);
    arg0->roster.buttonResources[3] = AddCellOrAnimResObjFromOpenNarc(arg0->gfxResMan[3], arg1, NARC_a_0_4_2, FALSE, 30, GF_GFX_RES_TYPE_ANIM, arg2);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(arg0->roster.buttonResources[0]) != 0);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(arg0->roster.buttonResources[1]) != 0);
    GF2DGfxResObj_FreeData(arg0->roster.buttonResources[0]);
    GF2DGfxResObj_FreeData(arg0->roster.buttonResources[1]);

    SpriteResourcesHeader spriteResourcesHeader;
    CreateSpriteResourcesHeader(&spriteResourcesHeader, 30, 30, 30, 30, -1, -1, 0, 0, arg0->gfxResMan[0], arg0->gfxResMan[1], arg0->gfxResMan[2], arg0->gfxResMan[3], 0, 0);
    for (s32 i = 0; i < 3; i++) {
        spriteTemplateList[i].spriteList = arg0->spriteList;
        spriteTemplateList[i].header = &spriteResourcesHeader;
        spriteTemplateList[i].heapID = arg2;
        arg0->roster.buttonSprites[i] = Sprite_CreateAffine(&spriteTemplateList[i]);
        Sprite_SetAnimCtrlSeq(arg0->roster.buttonSprites[i], sProfileButtonIdleAnims[i]);
    }
    FontID_Alloc(2, arg2);
    Window window;
    InitWindow(&window);
    AddTextWindowTopLeftCorner(arg0->bgConfig, &window, 8, 2, 0, 0);
    String *string = String_New(64, arg2);
    // CLOSE
    ReadMsgDataIntoString(arg0->msgData, msg_0778_00073, string);
    AddTextPrinterParameterizedWithColor(&window, 2, string, FontID_String_GetCenterAlignmentX(2, string, 0, 64), 0, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    String_Delete(string);
    TextOBJLayout *temp_r0_4 = TextOBJLayout_New(&window, arg2);
    GF_ASSERT(ObjCharTransfer_ReserveCharSpace(TextOBJLayout_GetCharSize(temp_r0_4, NNS_G2D_VRAM_TYPE_2DSUB), TRUE, NNS_G2D_VRAM_TYPE_2DSUB, &arg0->roster.closeTextCharSpace) != 0);

    TextOBJTemplate textObjTemplate;
    textObjTemplate.fontSystem = arg0->fontSystem;
    textObjTemplate.window = &window;
    textObjTemplate.spriteList = arg0->spriteList;
    textObjTemplate.plttResourceProxy = SpriteTransfer_GetPaletteProxy(arg0->roster.buttonResources[1], NULL);
    textObjTemplate.sprite = arg0->roster.buttonSprites[1];
    textObjTemplate.offset = arg0->roster.closeTextCharSpace.offset;
    textObjTemplate.x = -32;
    textObjTemplate.y = -8;
    textObjTemplate.unk_20 = 0;
    textObjTemplate.unk_24 = 0;
    textObjTemplate.vram = 2;
    textObjTemplate.heapID = arg2;
    arg0->roster.closeText = TextOBJ_Create(&textObjTemplate, temp_r0_4);
    TextOBJLayout_Delete(temp_r0_4);
    RemoveWindow(&window);
    FontID_Release(2);
    arg0->roster.buttonHitboxes = TouchHitboxController_Create(sProfileButtonHitboxes, 3, (TouchHitboxControllerCallback)WifiClub_ProfileButtonHitboxCallback, arg0, arg2);
}

void WifiClub_DeleteProfileButtons(WifiClubApp *arg0) {
    TouchHitboxController_Destroy(arg0->roster.buttonHitboxes);
    TextOBJ_Destroy(arg0->roster.closeText);
    ObjCharTransfer_FreeCharSpace(&arg0->roster.closeTextCharSpace);

    for (s32 i = 0; i < 3; i++) {
        Sprite_Delete(arg0->roster.buttonSprites[i]);
    }
    SpriteTransfer_DeleteCharTransferTask(arg0->roster.buttonResources[0]);
    SpriteTransfer_DeletePlttTransferTask(arg0->roster.buttonResources[1]);

    for (s32 i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(arg0->gfxResMan[i], arg0->roster.buttonResources[i]);
    }
}

void WifiClub_ShowProfileButtons(WifiClubApp *arg0) {
    arg0->roster.buttonsShown = 1;
    GfGfx_EngineBTogglePlanes(16, 1);
    arg0->roster.touchedButton = 0;
    for (s32 i = 0; i < 3; i++) {
        WifiClub_ResetProfileButton(&arg0->roster, i);
    }
}

void WifiClub_HideProfileButtons(WifiClubApp *arg0) {
    if (arg0->roster.buttonsShown == 2) {
        Sprite_SetDrawFlag(arg0->roster.buttonSprites[1], 1);
        TextOBJ_SetSpritesDrawFlag(arg0->roster.closeText, 1);
    }
    arg0->roster.buttonsShown = 0;
    GfGfx_EngineBTogglePlanes(16, 0);
}

s32 WifiClub_GetPressedProfileButton(WifiClubApp *arg0) {
    if (arg0->roster.buttonsShown == 0) {
        return 0;
    }
    arg0->roster.touchedButton = 3;
    arg0->roster.touchState = 255;
    TouchHitboxController_IsTriggered(arg0->roster.buttonHitboxes);

    for (s32 i = 0; i < 3; i++) {
        if ((arg0->roster.buttonsShown != 2 || i != 1) && WifiClub_UpdateProfileButton(&arg0->roster, i, arg0->roster.touchedButton, arg0->roster.touchState) == 1) {
            return i + 1;
        }
    }
    return 0;
}

void WifiClub_ProfileButtonHitboxCallback(u32 arg0, u32 arg1, WifiClubApp *arg2) {
    arg2->roster.touchedButton = arg0;
    arg2->roster.touchState = arg1;
}

void WifiClub_ResetProfileButton(WifiClubRoster *arg0, s32 arg1) {
    Sprite_SetAnimCtrlSeq(arg0->buttonSprites[arg1], sProfileButtonIdleAnims[arg1]);
    Sprite_SetAnimationFrame(arg0->buttonSprites[arg1], 4);
    if (arg1 == 1) {
        TextOBJ_SetPosition(arg0->closeText, -32, -8);
    }
}

s32 WifiClub_UpdateProfileButton(WifiClubRoster *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp8 = 0;
    if (arg2 == arg1) {
        u32 temp_r0 = Sprite_GetAnimationFrame(arg0->buttonSprites[arg1]);
        if (arg3 == 0 || arg3 == 2) {
            if (arg3 == 0) {
                Sprite_SetAnimCtrlSeq(arg0->buttonSprites[arg1], sProfileButtonPressAnims[arg1]);
            }
            if (temp_r0 < 3) {
                Sprite_UpdateAnim(arg0->buttonSprites[arg1], 2 * FX32_ONE);
                u16 temp_r4_2 = Sprite_GetAnimationFrame(arg0->buttonSprites[arg1]);
                if (arg1 == 1) {
                    TextOBJ_SetPosition(arg0->closeText, -32, sCloseTextYOffsets[temp_r4_2]);
                }
                if (temp_r4_2 >= 3) {
                    sp8 = 1;
                }
            }
        } else if (arg3 == 1 && temp_r0 < 3) {
            sp8 = 1;
        }
    } else {
        u16 temp_r7_2 = Sprite_GetAnimationNumber(arg0->buttonSprites[arg1]);
        u16 spC = Sprite_GetAnimationFrame(arg0->buttonSprites[arg1]);
        if (temp_r7_2 == sProfileButtonPressAnims[arg1]) {
            Sprite_SetAnimCtrlSeq(arg0->buttonSprites[arg1], sProfileButtonIdleAnims[arg1]);
            Sprite_SetAnimationFrame(arg0->buttonSprites[arg1], 4 - spC);
        }
        Sprite_UpdateAnim(arg0->buttonSprites[arg1], 2 * FX32_ONE);
    }
    return sp8;
}

void WifiClub_BufferRosterFriendName(WifiClubApp *arg0, s32 arg1) {
    if (arg1 != -1) {
        PlayerProfile *playerProfile = PlayerProfile_New(HEAP_ID_53);
        Save_Profile_PlayerName_Set(playerProfile, FriendRoster_GetPlayerName(arg0->friendRoster, arg1));
        BufferPlayersName(arg0->roster.msgFormat, 0, playerProfile);
        Heap_Free(playerProfile);
    }
}

WifiClubPlayerStatus *WifiClub_GetFriendStatusData(WifiClubApp *arg0, u32 arg1) {
    if (arg1 >= 32) {
        return &arg0->hiddenFriendStatus;
    }
    return &arg0->clubData->friends[arg1];
}

s32 WifiClub_GetFriendOnlineState(s32 arg0) {
    return WifiConnection_GetFriendStatus(arg0);
}

s32 WifiClub_StartMatch(WifiClubApp *arg0, s32 arg1, s32 arg2) {
    LinkSync_Init(HEAP_ID_15);
    WifiConnection_SetVoiceChatEnabled(arg0->clubData->own.voiceChat);
    if (WifiClub_IsGroupActivityStatus(arg2) == 1) {
        WifiConnection_SetExtraHeapEnabled(1, 128);
    } else {
        WifiConnection_SetExtraHeapEnabled(0, 128);
    }
    WifiClubComm_RegisterCommandsNoContext();
    return CommState_StartWifiMatch(arg1);
}

void WifiClub_StartSeeking(WifiClubApp *arg0, s32 arg1) {
    if (arg0->isSeeking == 0) {
        arg0->isSeeking = 1;
        WifiClubRoom_SetInputLocked(&arg0->room, 1);
        if (arg1 == 1) {
            WifiClub_PrintMessage(arg0, 9, 0);
        }
    }
}

void WifiClub_StopSeeking(WifiClubApp *arg0) {
    if (arg0->isSeeking == 1) {
        arg0->isSeeking = 0;
        WifiClub_CloseMessage(arg0);
        WifiClubRoom_SetInputLocked(&arg0->room, 0);
    }
}
