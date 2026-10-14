#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0778.h"
#include "msgdata/msg/msg_0800.h"

#include "bg_window.h"
#include "dwcaccount.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "obj_pltt_transfer.h"
#include "overlay_00_thumb.h"
#include "wifi_club_app.h"
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
#include "wifi_club_comm.h"
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

s32 WifiClub_HasBattleFrontier(WifiClubApp *app) {
    return Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(app->saveData), 2, 27);
}

s32 WifiClub_IsPoffinCookingAvailable(WifiClubApp *app) {
    return 0;
}

s32 WifiClub_ArePlazaGamesAvailable(WifiClubApp *app) {
    return 1;
}

u32 WifiClub_GetPlayerStatus(WifiClubApp *app, WifiClubPlayerStatus *playerStatus) {
    u32 var_r4 = playerStatus->status;
    if ((var_r4 - 20 <= 1) && (WifiClub_HasBattleFrontier(app) == 0)) {
        var_r4 = 29;
    }
    if ((var_r4 - 22 <= 5) && (WifiClub_ArePlazaGamesAvailable(app) == 0)) {
        var_r4 = 29;
    }
    if ((var_r4 - 18 <= 1) && (WifiClub_IsPoffinCookingAvailable(app) == 0)) {
        var_r4 = 29;
    }
    return var_r4;
}

s32 WifiClub_IsGroupActivityStatus(s32 status) {
    switch (status - 18) {
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

u32 WifiClub_GetGroupActivityChoice(u32 status) {
    switch (status - 18) {
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

s32 WifiClub_IsSeekingBattleStatus(u32 status) {
    switch (status - 9) {
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

s32 WifiClub_IsSeekingStatus(u32 status) {
    if (WifiClub_IsSeekingBattleStatus(status) != 0) {
        return 1;
    }
    switch (status - 15) {
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

s32 WifiClub_IsInBattleStatus(u32 status) {
    switch (status) {
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

s32 WifiClub_IsBusyStatus(u16 status) {
    if (WifiClub_IsInBattleStatus(status) != 0) {
        return 1;
    }
    switch (status) {
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

s32 WifiClub_StatusNeedsTwoMons(u32 status) {
    u32 statusOffset;

    statusOffset = status - 12;
    switch (statusOffset) {
    case 0:
    case 1:
    case 2:
    case 3:
        return 1;
    default:
        return 0;
    }
}

u32 WifiClub_GetCounterpartStatus(u32 status) {
    u32 var_r1;

    var_r1 = 29;
    switch (status) {
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

s32 WifiClub_CountBattleReadyMons(WifiClubApp *app) {
    Party *party = SaveArray_Party_Get(app->saveData);
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

void WifiClub_SetLinkModeForStatus(u32 status) {
    if ((status - 18) <= 1) {
        CommState_SetWifiPeerLinkMode29();
        return;
    }
    if (WifiClub_IsGroupActivityStatus(status) != 0) {
        CommState_SetClubWifiLinkMode();
        return;
    }
    if (status == 16) {
        CommState_SetWifiClubLinkMode();
        return;
    }
    if ((status - 20) <= 1) {
        Comm_SetWifiContinuousSend(0);
        CommState_SetFrontierWifiLinkMode();
        return;
    }
    Comm_SetWifiContinuousSend(1);
    CommState_SetSingleBattleWifiLinkMode();
}

void WifiClub_SetupGraphics(WifiClubApp *app) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetKeyRepeatTimers(4, 8);
    NARC *narc = NARC_New(NARC_a_0_8_8, HEAP_ID_53);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_53);
    app->msgFormat = MessageFormat_New(HEAP_ID_53);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_53);
    app->connectionMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_53);
    WifiClub_SetVramBanks();
    WifiClub_InitBgs(app->bgConfig);
    WifiClub_LoadBgGraphics(app, narc);
    MI_CpuFill8(&app->paletteCycle, 0, sizeof(WifiClubPaletteCycle));

    NNSG2dPaletteData *plttData;
    void *plttFile = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, HEAP_ID_53);
    MIi_CpuCopy16(plttData->pRawData, *app->paletteCycle.basePalettes, 128);
    MIi_CpuCopy16(plttData->pRawData, *app->paletteCycle.blendedPalettes, 128);
    Heap_Free(plttFile);
    int i;
    int blendIndex = 0;
    for (i = 0; i < 3; i++) {
        int j = 0;
        int done = 0;
        int a = i + 1;
        while (TRUE) {
            GF_ASSERT(blendIndex < 21);
            for (int k = 1; k < 16; k++) {
                BlendPalette(&app->paletteCycle.basePalettes[i][k], &app->paletteCycle.blendedPalettes[blendIndex][k], 1, j >> 8, app->paletteCycle.basePalettes[a][k]);
            }
            blendIndex++;
            if (done == 1) {
                break;
            }
            j += 768;
            if (j >= 4096) {
                j = 4096;
                done = 1;
            }
        }
    }
    DC_FlushRange(app->paletteCycle.blendedPalettes, 672);
    app->paletteCycle.active = 1;
    app->paletteCycle.targetScreens = 0;
    app->paletteCycle.task = SysTask_CreateOnVBlankQueue((SysTaskFunc)WifiClub_PaletteCycleTask, &app->paletteCycle, 20);
    Main_SetVBlankIntrCB((GFIntrCB)WifiClub_VBlankCB, app);
    WifiClub_InitObjTransfer();
    WifiClub_InitSpriteSystem(app, narc);
    WifiClub_EnableObjPlane(app);
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    NARC_Delete(narc);
}

void WifiClub_PaletteCycleTask(s32 task, WifiClubPaletteCycle *paletteCycle) {
    if (paletteCycle->active != 0) {
        paletteCycle->frameToggle ^= 1;
        if ((1 & paletteCycle->frameToggle) == 0) {
            if (paletteCycle->targetScreens <= 1) {
                GX_LoadBGPltt(paletteCycle->blendedPalettes[paletteCycle->blendIndex], 0, 32);
            }
            u32 targetScreens = paletteCycle->targetScreens;
            if ((targetScreens == 0) || (targetScreens == 2)) {
                GXS_LoadBGPltt(paletteCycle->blendedPalettes[paletteCycle->blendIndex], 0, 32);
            }
            if (paletteCycle->reverse == 0) {
                paletteCycle->blendIndex++;
                if (paletteCycle->blendIndex >= 21) {
                    paletteCycle->blendIndex = 19;
                    paletteCycle->reverse ^= 1;
                }
            } else {
                paletteCycle->blendIndex--;
                if (paletteCycle->blendIndex < 0) {
                    paletteCycle->blendIndex = 1;
                    paletteCycle->reverse ^= 1;
                }
            }
        }
    }
}

s32 WifiClub_Init(OverlayManager *overlayMananger, s32 *state) {
    switch (*state) {
    case 0:
        WifiClubArgs *overlayArgs = OverlayManager_GetArgs(overlayMananger);
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
        WifiClubApp *data = OverlayManager_CreateAndGetData(overlayMananger, sizeof(WifiClubApp), HEAP_ID_53);
        MI_CpuFill8(data, 0, sizeof(WifiClubApp));
        GF_CreateVramTransferManager(32, HEAP_ID_53);
        data->textPrinterId = 8;
        data->saveData = overlayArgs->saveData;
        data->friendRoster = Save_FriendRoster_Get(data->saveData);
        data->startMode = overlayArgs->choice;
        data->exitChoice = 8;
        data->contactFriend = -1;
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
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
        *state += 1;
        break;
    case 1:
        OverlayManager_GetData(overlayMananger);
        *state = 0;
        return 1;
    }
    return 0;
}

s32 WifiClub_Main(OverlayManager *overlayManager, s32 *state) {
    WifiClubApp *data = OverlayManager_GetData(overlayManager);
    int r1 = state[0];
    switch (r1) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        break;
    case 1:
        int curState = data->state;
        WifiClubStateFunc stateFunc = WifiClubStateFuncs[curState];
        if (stateFunc != NULL) {
            *sCurrentState = curState;
            *state = stateFunc(data, r1);
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            return 1;
        }
    }
    SpriteList *spriteList = data->spriteList;
    if (spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(spriteList);
    }
    if (WifiClubRoom_IsInitialized(&data->room) == 1) {
        WifiClubRoom_Draw(&data->room);
    }
    return 0;
}

void WifiClub_TeardownGraphics(WifiClubApp *app) {
    Main_SetVBlankIntrCB(NULL, 0);
    if (app->paletteCycle.task) {
        SysTask_Destroy(app->paletteCycle.task);
        app->paletteCycle.task = NULL;
        app->paletteCycle.active = 0;
    }
    if (WifiClubRoom_IsInitialized(&app->room) == 1) {
        WifiClubRoom_Free(&app->room);
    }
    if (WifiClub_IsRosterScreenActive(app) == 1) {
        WifiClub_FreeRosterScreen(app);
    }
    WifiClub_FreeStatusIcons(&app->statusIcons);
    FontSystem_Delete(app->fontSystem);
    for (int i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(app->gfxResMan[i]);
    }
    SpriteList_Delete(app->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    WifiClub_RemoveWindows(app);
    WifiClub_FreeBgConfig(app->bgConfig);
    DestroyMsgData(app->connectionMsgData);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
}

s32 WifiClub_Exit(OverlayManager *overlayManager) {
    WifiClubApp *data = OverlayManager_GetData(overlayManager);
    WifiClub_TeardownGraphics(data);
    WifiClubArgs *args = OverlayManager_GetArgs(overlayManager);
    args->choice = data->exitChoice;
    if ((data->exitChoice == 8) || (data->exitChoice == 10)) {
        UnloadDwcOverlay();
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
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
        GF_SndHandleSetInitialVolume(SND_HANDLE_FIELD, 120);
    }
    return 1;
}

void WifiClub_VBlankCB(WifiClubApp *app) {
    DoScheduledBgGpuUpdates(app->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    if (app->resetBrightness == 1) {
        SetMasterBrightnessNeutral(PM_LCD_TOP);
        SetMasterBrightnessNeutral(PM_LCD_BOTTOM);
        app->resetBrightness = 0;
    }
}

void WifiClub_SetVramBanks(void) {
    GraphicsBanks graphicsBanks = sWifiClubGraphicsBanks;
    GfGfx_SetBanks(&graphicsBanks);
}

void WifiClub_InitBgs(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = sWifiClubGraphicsModes;
    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgTemplate0 = sWifiClubBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bgTemplate0, 0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate bgTemplate1 = sWifiClubBgTemplate_Main1;
    InitBgFromTemplate(bgConfig, 1, &bgTemplate1, 0);
    BG_ClearCharDataRange(1, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(bgConfig, 1);

    BgTemplate bgTemplate2 = sWifiClubBgTemplate_Main2;
    InitBgFromTemplate(bgConfig, 2, &bgTemplate2, 0);
    BG_ClearCharDataRange(2, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(bgConfig, 2);

    BgTemplate bgTemplate3 = sWifiClubBgTemplate_Main3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bgTemplate3, 0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate bgTemplate4 = sWifiClubBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, 4, &bgTemplate4, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 4);

    BgTemplate bgTemplate5 = sWifiClubBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bgTemplate5, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    BgTemplate bgTemplate6 = sWifiClubBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bgTemplate6, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);

    BgTemplate bgTemplate7 = sWifiClubBgTemplate_Sub3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &bgTemplate7, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(1, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(2, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 32, 0, HEAP_ID_53);
    SetBgPriority(GF_BG_LYR_MAIN_0, 3);
    SetBgPriority(1, 1);
    SetBgPriority(GF_BG_LYR_MAIN_3, 0);
    SetBgPriority(2, 0);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
}

void WifiClub_AddPlayerObject(WifiClubApp *app, WifiClubRoomPerson *person) {
    for (int i = 0; i < 33; i++) {
        if (app->playerObjects[i] == NULL) {
            app->playerObjects[i] = person;
            return;
        }
    }
    GF_AssertFail();
    return;
}

void WifiClub_RemovePlayerObject(WifiClubApp *app, WifiClubRoomPerson *person) {
    for (int i = 0; i < 33; i++) {
        if (app->playerObjects[i] == person) {
            app->playerObjects[i] = NULL;
            return;
        }
    }
    GF_AssertFail();
    return;
}

WifiClubRoomPerson *WifiClub_FindPlayerObject(WifiClubApp *app, u32 personId) {
    for (s32 i = 0; i < 33; i++) {
        if ((app->playerObjects[i] != NULL) && (personId == WifiClubRoomPerson_GetId(app->playerObjects[i]))) {
            return app->playerObjects[i];
        }
    }
    return NULL;
}

void WifiClub_LoadStatusIcons(BgConfig *bgConfig, WifiClubStatusIcons *statusIcons, NARC *narc, enum HeapID heapID) {
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 14, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_8_OFFSET, 64, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 14, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, 64, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 13, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 13, bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, heapID);
    if (statusIcons->charsRaw == 0) {
        statusIcons->charsRaw = GfGfxLoader_GetCharDataFromOpenNarc(narc, 13, FALSE, &statusIcons->chars, heapID);
    }
    if (statusIcons->screenRaw == NULL) {
        statusIcons->screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 15, FALSE, &statusIcons->screen, heapID);
    }
}

void WifiClub_FreeStatusIcons(WifiClubStatusIcons *statusIcons) {
    if (statusIcons->screenRaw) {
        Heap_Free(statusIcons->screenRaw);
        statusIcons->screenRaw = NULL;
    }
    if (statusIcons->charsRaw) {
        Heap_Free(statusIcons->charsRaw);
        statusIcons->charsRaw = NULL;
    }
}

void WifiClub_DrawIconToBg(BgConfig *bgConfig, WifiClubStatusIcons *statusIcons, s32 bgId, u8 x, u8 y, s32 iconIndex, s32 paletteOffset) {
    CopyToBgTilemapRect(bgConfig, bgId, x, y, 2, 2, statusIcons->screen->rawData, iconIndex * 2, 0, statusIcons->screen->screenWidth / 8, statusIcons->screen->screenHeight / 8);
    s32 r0 = WifiClub_GetIconPaletteBase(bgId);
    BgTilemapRectChangePalette(bgConfig, bgId, x, y, 2, 2, r0 + paletteOffset);
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}

void WifiClub_DrawStatusIconToBg(BgConfig *bgConfig, WifiClubStatusIcons *statusIcons, s32 bgId, u8 x, u8 y, u32 status) {
    s32 var_r4;
    s32 var_r5;

    var_r4 = 0;
    switch (status) {
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
    WifiClub_DrawIconToBg(bgConfig, statusIcons, bgId, x, y, var_r5, var_r4);
}

void WifiClub_DrawIconToWindow(Window *window, WifiClubStatusIcons *statusIcons, u8 x, u16 y, s32 iconIndex, s32 paletteOffset) {
    BlitBitmapRectToWindow(window, statusIcons->chars->pRawData, iconIndex * 16, 0, statusIcons->chars->W * 8, statusIcons->chars->H * 8, x, y, 16, 16);
    s32 r0 = WifiClub_GetIconPaletteBase(window->bgId);
    SetWindowPaletteNum(window, r0 + paletteOffset);
}

void WifiClub_DrawStatusIconToWindow(Window *window, WifiClubStatusIcons *statusIcons, u8 x, u16 y, u32 status) {
    u8 var_r5;
    u8 var_r4 = 0;
    switch (status) {
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
    WifiClub_DrawIconToWindow(window, statusIcons, x, y, var_r5, var_r4);
}

s32 WifiClub_GetIconPaletteBase(u32 bgId) {
    if (bgId < 4) {
        return 8;
    }
    return 13;
}

u32 WifiClub_FadeOutToEnterRoom(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
    WifiClub_PlayRoomBgm();
    return 18;
}

void WifiClub_AllocStrings(WifiClubApp *app) {
    app->msgExpanded = String_New(380, HEAP_ID_53);
    app->unk17C = String_New(380, HEAP_ID_53);
    app->msgRaw = String_New(380, HEAP_ID_53);
    app->tmpString = String_New(180, HEAP_ID_53);
    app->state = 0;
}

void WifiClub_FreeStrings(WifiClubApp *app) {
    String_Delete(app->tmpString);
    String_Delete(app->msgExpanded);
    String_Delete(app->unk17C);
    String_Delete(app->msgRaw);
}

void WifiClub_FreeBgConfig(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, 4);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, 2);
    FreeBgTilemapBuffer(bgConfig, 1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(bgConfig);
}

void WifiClub_LoadBgGraphics(WifiClubApp *app, NARC *narc) {
    BgConfig *bgConfig = app->bgConfig;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_53);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_53);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_53);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_53);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, HEAP_ID_53);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, HEAP_ID_53);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_53);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_53);
    Options *options = Save_PlayerData_GetOptionsAddr(app->saveData);
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

void WifiClub_InitSpriteSystem(WifiClubApp *app, NARC *narc) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_53);
    app->spriteList = G2dRenderer_Init(8, &app->g2dRenderer, HEAP_ID_53);
    G2dRenderer_SetSubSurfaceCoords(&app->g2dRenderer, 0, 0x100000);
    for (int i = 0; i < 4; i++) {
        app->gfxResMan[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, HEAP_ID_53);
    }
    app->fontSystem = FontSystem_NewInit(1, HEAP_ID_53);
}

void WifiClub_EnableObjPlane(WifiClubApp *app) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void WifiClub_PrintTitleBar(WifiClubApp *app) {
    if (WindowIsInUse(&app->titleWindow) != 0) {
        EraseFrameAndWindow1(&app->titleWindow, FALSE);
        RemoveWindow(&app->titleWindow);
    }
    AddWindowParameterized(app->bgConfig, &app->titleWindow, GF_BG_LYR_MAIN_3, 5, 1, 22, 2, 13, 458);
    FillWindowPixelBuffer(&app->titleWindow, 0);

    // Nintendo Wi-Fi Connection
    ReadMsgDataIntoString(app->msgData, msg_0778_00021, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->titleWindow, 1, app->tmpString, FontID_String_GetCenterAlignmentX(0, app->tmpString, 0, 176), 0, TEXT_SPEED_NOTRANSFER, 0xF0E00, NULL);
    ScheduleWindowCopyToVram(&app->titleWindow);
}

void WifiClub_RemoveWindows(WifiClubApp *app) {
    WifiClub_CloseMessage(app);
    if (WindowIsInUse(&app->titleWindow) != 0) {
        RemoveWindow(&app->titleWindow);
    }
    if (WindowIsInUse(&app->msgWindow) != 0) {
        RemoveWindow(&app->msgWindow);
    }
    if (WindowIsInUse(&app->unk330) != 0) {
        RemoveWindow(&app->unk330);
    }
    if (WindowIsInUse(&app->statusWindow) != 0) {
        RemoveWindow(&app->statusWindow);
    }
    if (WindowIsInUse(&app->unk310) != 0) {
        RemoveWindow(&app->unk310);
    }
    if (app->unk150 != NULL) {
        DestroyListMenu(app->unk150, NULL, NULL);
    }
    if (app->unk14C != NULL) {
        ListMenuItems_Delete(app->unk14C);
        app->unk14C = NULL;
    }
    if (WindowIsInUse(&app->noticeWindow) != 0) {
        RemoveWindow(&app->noticeWindow);
    }
}

s32 WifiClub_State_Start(WifiClubApp *app, s32 mainState) {
    if (CommState_IsWifi() != 0) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        app->clubData = CommState_GetWifiClubData();
        u32 ownStatus = WifiClub_GetPlayerStatus(app, &app->clubData->own);
        if ((ownStatus == 8) || (ownStatus == 18) || (ownStatus == 20) || (ownStatus == 22) || (ownStatus == 23) || (ownStatus == 24) || (ownStatus == 25) || (ownStatus == 26) || (ownStatus == 27) || (ownStatus == 19)) {
            WifiClub_SetStatusAndSend(app, 16);
            CommPlayerInfo_Free();
            CommState_EndWifiMatch();
            WifiClubStatusData *clubData = CommState_GetWifiClubData();
            app->clubData = clubData;
            clubData->own.voiceChat = clubData->own.voiceChatSetting;
            app->timer = 20;
            app->state = 65;
        } else {
            WifiClub_PrintMessage(app, msg_0778_00077, 0);
            CommState_SetErrorWatch(1, 1);
            LinkSync_StartSync(16);
            app->state = 61;
        }
    } else {
        Sound_SetSceneAndPlayBGM(11, SEQ_GS_WIFI_ACCESS, 1);
        WifiClub_PrintTitleBar(app);
        FriendRoster_GetUserData(app->friendRoster);
        if (DWC_CheckHasProfile() == 0) {
            SetBgPriority(GF_BG_LYR_MAIN_0, 3);
            SetBgPriority(1, 0);
            SetBgPriority(GF_BG_LYR_MAIN_3, 0);
            SetBgPriority(2, 1);
            WifiClub_ShowConnectionNotice(app, msg_0800_00018);
            app->yesNoPrompt = WifiClub_CreateYesNoPrompt(app->bgConfig, &sUserInfoPromptWindowTemplate, 0);
            app->state = 6;
        } else {
            FriendRoster_GetUserData(app->friendRoster);
            if (DWC_CheckValidConsole() == 0) {
                WifiClub_ShowConnectionNotice(app, msg_0800_00020);
                app->state = 3;
            } else {
                WifiClub_PrintMessage(app, msg_0800_00017, 1);
                app->state = 1;
            }
        }
    }
    return mainState;
}

s32 WifiClub_State_CreateConnectPrompt(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        app->yesNoPrompt = WifiClub_CreateYesNoPrompt(app->bgConfig, &sConnectPromptWindowTemplate, 0);
        app->state = 2;
    }
    return mainState;
}

s32 WifiClub_State_ConnectPrompt(WifiClubApp *app, s32 mainState) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(app->yesNoPrompt);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->clubData = CommState_StartWifiClub(app->saveData, 1188);
        WifiClub_PrintMessage(app, msg_0800_00023, 1);
        GF_ASSERT(app->waitingIcon == NULL);
        app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
        app->state = 14;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = 34;
    } else {
        return mainState;
    }
    return mainState;
}

s32 WifiClub_State_AskUpdateUserInfo(WifiClubApp *app, s32 mainState) {
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        WifiClub_ShowConnectionNotice(app, msg_0800_00021);
        SetBgPriority(GF_BG_LYR_MAIN_3, 0);
        SetBgPriority(2, 1);
        SetBgPriority(1, 0);
        SetBgPriority(GF_BG_LYR_MAIN_0, 3);
        app->yesNoPrompt = WifiClub_CreateYesNoPrompt(app->bgConfig, &sUserInfoPromptWindowTemplate, 1);
        app->state = 4;
    }
    return mainState;
}

s32 WifiClub_State_UpdateUserInfoPrompt(WifiClubApp *app, s32 mainState) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(app->yesNoPrompt);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        ClearFrameAndWindow2(&app->noticeWindow, FALSE);
        RemoveWindow(&app->noticeWindow);
        WifiClub_ShowConnectionNotice(app, msg_0800_00022);
        app->yesNoPrompt = WifiClub_CreateYesNoPrompt(app->bgConfig, &sUserInfoPromptWindowTemplate, 1);
        app->state = 5;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        ClearFrameAndWindow2(&app->noticeWindow, FALSE);
        RemoveWindow(&app->noticeWindow);
        app->state = 34;
    } else {
        return mainState;
    }
    return mainState;
}

s32 WifiClub_State_ConfirmUpdateUserInfoPrompt(WifiClubApp *app, s32 mainState) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(app->yesNoPrompt);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        SetBgPriority(GF_BG_LYR_MAIN_3, 0);
        SetBgPriority(2, 0);
        SetBgPriority(1, 1);
        SetBgPriority(GF_BG_LYR_MAIN_0, 3);
        ClearFrameAndWindow2(&app->noticeWindow, FALSE);
        RemoveWindow(&app->noticeWindow);
        Save_FriendRoster_Init(app->friendRoster);
        void *saveArray = SaveArray_Get(app->saveData, 31);
        Save_WiiMessage_Init(saveArray);
        FrontierSave_ClearRows(Save_Frontier_GetStatic(app->saveData));
        app->state = 14;
        app->clubData = CommState_StartWifiClub(app->saveData, 1188);
        WifiClub_PrintMessage(app, msg_0800_00023, 1);
        GF_ASSERT(app->waitingIcon == NULL);
        app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
        app->userInfoSaved = 1;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        SetBgPriority(GF_BG_LYR_MAIN_3, 0);
        SetBgPriority(2, 0);
        SetBgPriority(1, 1);
        SetBgPriority(GF_BG_LYR_MAIN_0, 3);
        ClearFrameAndWindow2(&app->noticeWindow, FALSE);
        RemoveWindow(&app->noticeWindow);
        WifiClub_ShowConnectionNotice(app, msg_0800_00020);
        app->state = 3;
    } else {
        return mainState;
    }
    return mainState;
}

s32 WifiClub_State_SaveUserInfoPrompt(WifiClubApp *app, s32 mainState) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(app->yesNoPrompt);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        SetBgPriority(GF_BG_LYR_MAIN_0, 3);
        SetBgPriority(1, 1);
        SetBgPriority(GF_BG_LYR_MAIN_3, 0);
        SetBgPriority(2, 0);
        ClearFrameAndWindow2(&app->noticeWindow, FALSE);
        RemoveWindow(&app->noticeWindow);
        app->state = 14;
        app->clubData = CommState_StartWifiClub(app->saveData, 1188);
        WifiClub_PrintMessage(app, msg_0800_00023, 1);
        GF_ASSERT(app->waitingIcon == NULL);
        app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
        app->userInfoSaved = 1;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        SetBgPriority(GF_BG_LYR_MAIN_0, 3);
        SetBgPriority(1, 1);
        SetBgPriority(GF_BG_LYR_MAIN_3, 0);
        SetBgPriority(2, 0);
        ClearFrameAndWindow2(&app->noticeWindow, FALSE);
        RemoveWindow(&app->noticeWindow);
        app->state = 34;
    } else {
        return mainState;
    }
    return mainState;
}

s32 WifiClub_State_StartConnecting(WifiClubApp *app, s32 mainState) {
    SetBgPriority(GF_BG_LYR_MAIN_0, 3);
    SetBgPriority(1, 1);
    SetBgPriority(GF_BG_LYR_MAIN_3, 0);
    SetBgPriority(2, 0);
    app->state = 15;
    return mainState;
}

void WifiClub_InitOwnStatus(WifiClubApp *app, u32 status) {
    PlayerProfile *playerProfile = Save_PlayerData_GetProfile(app->saveData);
    Pokedex *pokedex = Save_Pokedex_Get(app->saveData);
    Party *party = SaveArray_Party_Get(app->saveData);
    SaveWiFiHistory *saveWiFiHistory = Save_WiFiHistory_Get(app->saveData);
    s32 partyCount = Party_GetCount(party);

    for (s32 i = 0; i < partyCount; i++) {
        Pokemon *pokemon = Party_GetMonByIndex(party, i);
        app->clubData->own.species[i] = GetMonData(pokemon, MON_DATA_SPECIES, 0);
        app->clubData->own.heldItems[i] = GetMonData(pokemon, MON_DATA_HELD_ITEM, 0);
    }
    app->clubData->own.gameInfo[0] = PlayerProfile_GetVersion(playerProfile);
    app->clubData->own.gameInfo[1] = PlayerProfile_GetLanguage(playerProfile);
    app->clubData->own.gameInfo[2] = Pokedex_GetNatDexFlag(pokedex);
    app->clubData->own.status = 29;
    WifiClub_SetStatus(app, status);
    app->clubData->own.trainerInfo[0] = 0;
    app->clubData->own.trainerInfo[1] = PlayerProfile_GetAvatar(playerProfile);
    app->clubData->own.trainerInfo[2] = PlayerProfile_GetTrainerGender(playerProfile);
    app->clubData->own.trainerInfo[3] = WifiHistory_GetPlayerCountry(saveWiFiHistory);
    app->clubData->own.trainerInfo[4] = WiFiHistory_GetPlayerRegion(saveWiFiHistory);
    app->clubData->own.voiceChat = 1;
    app->clubData->own.voiceChatSetting = 1;
    WifiConnection_SetOwnStatusData(app->clubData, 36);
    WifiConnection_SetFriendStatusBuffer(app->clubData->friends, 36);
}

s32 WifiClub_CollectOnlineFriends(WifiClubApp *app) {
    s32 counter = 0;
    for (s32 i = 0; i < 32; i++) {
        app->onlineFriends[i] = 0;
        if ((app->friendCount > i) && (WifiClub_GetFriendOnlineState(i) != 0)) {
            WifiClubPlayerStatus *friendStatusData = WifiClub_GetFriendStatusData(app, i);
            u32 friendStatus = WifiClub_GetPlayerStatus(app, friendStatusData);
            if ((friendStatus != 0) && (friendStatus != 28) && (friendStatusData->trainerInfo[1] != 0)) {
                app->onlineFriends[counter] = i + 1;
                counter += 1;
            } else if ((friendStatus == 28) && (WifiClub_GetRosterSlotState(app, i + 1) != 0)) {
                WifiClub_ClearRosterSlotState(app, i + 1);
            }
        }
    }
    return counter;
}

s32 WifiClub_UpdateFriendStatuses(WifiClubApp *app) {
    s32 counter = 0;
    s32 wasSeeking;
    for (s32 i = 0; i < 32; i++) {
        WifiClubPlayerStatus *friendStatusData = WifiClub_GetFriendStatusData(app, i);
        s32 friendStatus = WifiClub_GetPlayerStatus(app, friendStatusData);
        if ((friendStatus != app->friendStatuses[i]) || (friendStatusData->voiceChat != app->friendVoiceChat[i])) {
            WifiClubRoomPerson *person = WifiClub_FindPlayerObject(app, i + 1);
            if (person != NULL) {
                wasSeeking = WifiClub_IsSeekingStatus(app->friendStatuses[i]);
                s32 isSeeking = WifiClub_IsSeekingStatus(friendStatus);
                if ((wasSeeking == 1) && (isSeeking == 0)) {
                    WifiClubRoom_SetPersonBusy(&app->room, person, 0);
                } else if ((wasSeeking == 0) && (isSeeking == 1)) {
                    WifiClubRoom_SetPersonBusy(&app->room, person, 1);
                }
                app->friendStatuses[i] = friendStatus;
                app->friendVoiceChat[i] = friendStatusData->voiceChat;
                FriendRoster_SetEntryField(app->friendRoster, i, 7, friendStatusData->trainerInfo[1]);
                FriendRoster_SetEntryField(app->friendRoster, i, 8, friendStatusData->trainerInfo[2]);
                counter += 1;
            }
        }
    }
    return counter;
}

void WifiClub_HandleWifiError(WifiClubApp *app) {
    u32 *wifiError = CommState_GetWifiError();
    s32 errorCode = wifiError[0];
    s32 var_r5 = WifiConnection_ClassifyErrorCode(errorCode, wifiError[1]);
    s32 var_r7;
    if ((var_r5 == 11) || (errorCode == 25)) {
        if (wifiError[1] != 2) {
            var_r7 = msg_0800_00014;
        } else {
            var_r7 = msg_0800_00011;
        }
        var_r5 = 11;
    } else if (errorCode == 26) {
        var_r7 = msg_0800_00013;
        var_r5 = 12;
    } else if (var_r5 >= 0) {
        var_r7 = var_r5;
    } else {
        var_r7 = msg_0800_00011;
    }
    WifiClub_CloseMessage(app);
    BufferIntegerAsString(app->msgFormat, 0, errorCode, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    app->timer = 30;
    WifiClub_ShowConnectionNotice(app, var_r7);
    switch (var_r5) {
    case 1:
    case 4:
    case 5:
    case 11:
        app->state = 10;
        return;
    case 6:
    case 7:
    case 8:
    case 9:
        app->state = 10;
        return;
    case 10:
        app->state = 10;
        return;
    }
    app->state = 35;
}

s32 WifiClub_State_WaitErrorButton(WifiClubApp *app, s32 mainState) {
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        if (WindowIsInUse(&app->noticeWindow) != 0) {
            ClearFrameAndWindow2(&app->noticeWindow, FALSE);
            RemoveWindow(&app->noticeWindow);
        }
        app->state = 8;
    }
    return mainState;
}

s32 WifiClub_State_ErrorAcknowledged(WifiClubApp *app, s32 mainState) {
    app->state = 9;
    return mainState;
}

s32 WifiClub_State_GoToReconnectPrompt(WifiClubApp *app, s32 mainState) {
    app->state = 11;
    return mainState;
}

s32 WifiClub_State_ErrorThenAskReconnect(WifiClubApp *app, s32 mainState) {
    s32 timer;

    timer = app->timer;
    if (timer > 0) {
        app->timer = timer - 1;
        return mainState;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        if (WindowIsInUse(&app->noticeWindow) != 0) {
            ClearFrameAndWindow2(&app->noticeWindow, FALSE);
            RemoveWindow(&app->noticeWindow);
        }
        WifiClub_PrintMessage(app, msg_0778_00088, 0);
        app->state = 11;
    }
    return mainState;
}

s32 WifiClub_State_CreateReconnectPrompt(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            app->yesNoPrompt = WifiClub_CreateYesNoPrompt(app->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        app->state = 12;
    }
    return mainState;
}

s32 WifiClub_State_ReconnectPrompt(WifiClubApp *app, s32 mainState) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 menuResult = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_53);
        if (menuResult == -1) {
            return mainState;
        }
        if (menuResult == 0) {
            CommState_EndWifiSession();
            WifiClub_SetStatusAndSend(app, 16);
            app->state = 13;
        } else {
            WifiClub_SetStatusAndSend(app, 16);
            CommState_EndWifiSession();
            CommPlayerInfo_Free();
            app->exitChoice = 8;
            app->state = 34;
        }
    } else {
        enum YesNoResponse response = YesNoPrompt_HandleInput(app->yesNoPrompt);
        if (response == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            CommState_EndWifiSession();
            WifiClub_SetStatusAndSend(app, 16);
            app->state = 13;
        } else if (response == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            WifiClub_SetStatusAndSend(app, 16);
            CommState_EndWifiSession();
            CommPlayerInfo_Free();
            app->exitChoice = 8;
            app->state = 34;
        } else {
            return mainState;
        }
    }
    WifiClub_CloseMessage(app);
    return mainState;
}

s32 WifiClub_State_Reconnect(WifiClubApp *app, s32 mainState) {
    if (CommWireless_IsStartedUp() != 0) {

    } else {
        if (WifiClubRoom_IsInitialized(&app->room) == 1) {
            SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
            SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
            WifiClubRoom_Free(&app->room);
            for (s32 i = 0; i < 33; i++) {
                app->playerObjects[i] = 0;
            }
            thunk_ClearMainOAM(HEAP_ID_53);
            thunk_ClearSubOAM(HEAP_ID_53);
        }
        if (WifiClub_IsRosterScreenActive(app) == 1) {
            WifiClub_FreeRosterScreen(app);
        }
        if (WindowIsInUse(&app->titleWindow) != 0) {
            EraseFrameAndWindow1(&app->titleWindow, FALSE);
            RemoveWindow(&app->titleWindow);
        }
        if (WindowIsInUse(&app->statusWindow) != 0) {
            RemoveWindow(&app->statusWindow);
        }
        BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
        NARC *narc = NARC_New(NARC_a_0_8_8, HEAP_ID_53);
        WifiClub_LoadBgGraphics(app, narc);
        NARC_Delete(narc);
        app->paletteCycle.active = 1;
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 0);
        app->resetBrightness = 1;
        if (app->unk14C != NULL) {
            RemoveWindow(&app->unk310);
            DestroyListMenu(app->unk150, NULL, NULL);
            app->unk150 = NULL;
            ListMenuItems_Delete(app->unk14C);
            app->unk14C = NULL;
        }
        MI_CpuFill8(app->onlineFriends, 0, 32);
        MI_CpuFill8(app->prevOnlineFriends, 0, 32);
        MI_CpuFill8(app->friendStatuses, 0, 128);
        MI_CpuFill8(app->friendVoiceChat, 0, 128);
        WifiClub_PrintTitleBar(app);
        app->clubData = CommState_StartWifiClub(app->saveData, 1188);
        WifiClub_PrintMessage(app, msg_0800_00023, 1);
        GF_ASSERT(app->waitingIcon == NULL);
        app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
        app->state = 14;
    }
    return mainState;
}

s32 WifiClub_State_Connecting(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    if (WifiConnection_IsFriendDataChanged() != 0) {
        app->state = 69;
        WifiClub_PrintMessage(app, msg_0800_00030, 1);
        GF_ASSERT(app->waitingIcon == NULL);
        app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
    }
    if (CommState_IsWifiJoiningLobby() != 0) {
        if (app->userInfoSaved != 0) {
            app->state = 16;
        } else {
            WifiClub_InitOwnStatus(app, 16);
            WifiClub_CollectOnlineFriends(app);
            app->state = WifiClub_FadeOutToEnterRoom();
        }
    } else if ((CommState_HasWifiFailed() != 0) || (CommState_GetWifiMatchStatus() == 3)) {
        WifiClub_HandleWifiError(app);
    }
    return mainState;
}

s32 WifiClub_State_PrepareSave(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    if (WifiConnection_IsFriendDataChanged() != 0) {
        Save_PrepareForAsyncWrite(app->saveData, 0);
        app->state = 71;
        return mainState;
    }
    if (CommState_IsWifiJoiningLobby() != 0) {
        if (app->userInfoSaved != 0) {
            app->state = 16;
        } else {
            WifiClub_InitOwnStatus(app, 16);
            WifiClub_CollectOnlineFriends(app);
            app->state = WifiClub_FadeOutToEnterRoom();
        }
    } else if ((CommState_HasWifiFailed() != 0) || (CommState_GetWifiMatchStatus() == 3)) {
        WifiClub_HandleWifiError(app);
    }
    return mainState;
}

s32 WifiClub_State_WriteSave(WifiClubApp *app, s32 mainState) {
    if (WifiConnection_IsFriendDataChanged() != 0) {
        s32 saveResult = Save_WriteFileAsync(app->saveData);
        if ((saveResult != 0) && (saveResult != 1)) {
            WifiConnection_ClearFriendDataChanged();
            Save_ClearStatusFlags(app->saveData);
        } else {
            return mainState;
        }
    }
    if (CommState_IsWifiJoiningLobby() != 0) {
        if (app->userInfoSaved != 0) {
            app->state = 16;
        } else {
            WifiClub_InitOwnStatus(app, 16);
            WifiClub_CollectOnlineFriends(app);
            app->state = WifiClub_FadeOutToEnterRoom();
        }
    } else if ((CommState_HasWifiFailed() != 0) || (CommState_GetWifiMatchStatus() == 3)) {
        WifiClub_HandleWifiError(app);
    }
    return mainState;
}

s32 WifiClub_State_WaitThenReenterRoom(WifiClubApp *app, s32 mainState) {
    if (app->timer > 0) {
        app->timer--;
        return mainState;
    }
    if (CommState_IsWifiInLobby() != 0) {
        app->state = WifiClub_FadeOutToEnterRoom();
    } else if ((CommState_HasWifiFailed() != 0) || (CommState_GetWifiMatchStatus() == 3)) {
        WifiClub_HandleWifiError(app);
    } else if ((CommState_GetWifiMatchStatus() >= 4) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0)) {
        CommState_LeaveWifiMatch();
        app->state = WifiClub_FadeOutToEnterRoom();
    }
    return mainState;
}

s32 WifiClub_State_ShowUserInfoSaved(WifiClubApp *app, s32 mainState) {
    WifiClub_CloseMessage(app);
    WifiClub_ShowConnectionNotice(app, msg_0800_00019);
    app->state = 17;
    app->userInfoSaved = 0;
    return mainState;
}

s32 WifiClub_State_UserInfoSavedWaitButton(WifiClubApp *app, s32 mainState) {
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        if (app->startMode == 1) {
            app->exitChoice = 10;
            app->state = 34;
            CommState_EndWifiSession();
        } else if (FriendRoster_CountFriends(app->friendRoster) == 0) {
            WifiClub_PrintMessage(app, msg_0800_00026, 1);
            app->state = 59;
            app->timer = 1;
        } else {
            WifiClub_InitOwnStatus(app, 16);
            WifiClub_CollectOnlineFriends(app);
            app->state = WifiClub_FadeOutToEnterRoom();
            if (WindowIsInUse(&app->noticeWindow) != 0) {
                ClearFrameAndWindow2(&app->noticeWindow, FALSE);
                RemoveWindow(&app->noticeWindow);
            }
        }
    }
    return mainState;
}

void WifiClub_PrintOwnStatus(WifiClubApp *app) {
    PlayerProfile *playerProfile = Save_PlayerData_GetProfile(app->saveData);
    String *strPlayerName = PlayerProfile_GetPlayerName_NewString(playerProfile, HEAP_ID_53);
    if (WindowIsInUse(&app->statusWindow) == 0) {
        String_Delete(strPlayerName);
        return;
    }
    u32 color;
    FillWindowPixelBuffer(&app->statusWindow, 0);
    if (PlayerProfile_GetTrainerGender(playerProfile) == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    AddTextPrinterParameterizedWithColor(&app->statusWindow, 0, strPlayerName, 32, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
    String_Delete(strPlayerName);
    s32 ownStatus = WifiClub_GetPlayerStatus(app, &app->clubData->own);
    s32 statusMsgId = WifiClub_GetStatusLabel(ownStatus, &color);
    if (ownStatus == 1) {
        if (WifiConnection_GetTargetFriendIndex() == -1) {
            ReadMsgDataIntoString(app->msgData, statusMsgId, app->msgRaw);
        } else {
            s32 temp_r0_3 = FriendRoster_GetEntryField(app->friendRoster, WifiConnection_GetTargetFriendIndex(), 8);
            if (temp_r0_3 == 0) {
                color = MAKE_TEXT_COLOR(5, 6, 0);
            } else if (temp_r0_3 == 1) {
                color = MAKE_TEXT_COLOR(3, 4, 0);
            }
            CopyU16ArrayToString(app->msgRaw, FriendRoster_GetPlayerName(app->friendRoster, WifiConnection_GetTargetFriendIndex()));
        }
    } else {
        ReadMsgDataIntoString(app->msgData, statusMsgId, app->msgRaw);
    }
    AddTextPrinterParameterizedWithColor(&app->statusWindow, 0, app->msgRaw, 102, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
    WifiClub_DrawStatusIconToBg(app->bgConfig, &app->statusIcons, 1, 2, 1, ownStatus);
    s32 var_r0_2;
    if (app->clubData->own.voiceChat != 0) {
        var_r0_2 = 8;
    } else {
        var_r0_2 = 1;
    }
    WifiClub_DrawIconToBg(app->bgConfig, &app->statusIcons, 1, 26, 1, var_r0_2, 0);
    ScheduleWindowCopyToVram(&app->statusWindow);
}

void WifiClub_ListMenuCursorCallback(struct ListMenu *list, s32 index, u8 onInit) {
    if (onInit == 0) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

void WifiClub_CreateStatusBarWindows(WifiClubApp *app) {
    if (WindowIsInUse(&app->statusWindow) != 0) {
        RemoveWindow(&app->statusWindow);
    }
    if (WindowIsInUse(&app->titleWindow) != 0) {
        EraseFrameAndWindow1(&app->titleWindow, FALSE);
        RemoveWindow(&app->titleWindow);
    }
    AddWindowParameterized(app->bgConfig, &app->statusWindow, GF_BG_LYR_MAIN_3, 1, 1, 28, 2, 13, 402);
    AddWindowParameterized(app->bgConfig, &app->titleWindow, 1, 1, 1, 28, 2, 13, 417);
    FillWindowPixelBuffer(&app->titleWindow, 15);
    ScheduleWindowCopyToVram(&app->titleWindow);
    DrawFrameAndWindow1(&app->titleWindow, FALSE, 473, 11);
}

s32 WifiClub_State_EnterRoom(WifiClubApp *app, s32 mainState) {
    if (IsPaletteFadeFinished() == 0) {
        return mainState;
    }
    if ((CommState_HasWifiFailed() == 0) && (CommState_IsWifiInLobby() == 0)) {
        return mainState;
    }
    WifiConnection_SetConnectFilterCallback(WifiClub_IsMatchingActivity);
    app->paletteCycle.active = 0;
    NARC *narc = NARC_New(NARC_a_0_8_8, HEAP_ID_53);
    WifiClub_LoadStatusIcons(app->bgConfig, &app->statusIcons, narc, HEAP_ID_53);
    WifiClub_RestoreVoiceChatSetting(app);
    WifiClub_CloseMessage(app);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    if (app->unk14C != NULL) {
        RemoveWindow(&app->unk310);
        DestroyListMenu(app->unk150, NULL, NULL);
        app->unk150 = NULL;
        ListMenuItems_Delete(app->unk14C);
        app->unk14C = NULL;
    }
    if (WifiClubRoom_IsInitialized(&app->room) == 0) {
        s16 var_r4;
        if (app->clubData->own.trainerInfo[2] == 0) {
            var_r4 = 0;
        } else {
            var_r4 = 97;
        }
        app->friendCount = FriendRoster_GetUsedSlotCount(app->friendRoster);
        WifiClubRoom_Init(&app->room, HEAP_ID_53, narc, app->bgConfig, var_r4, app->friendCount);
        WifiClub_AddPlayerObject(app, WifiClubRoom_AddPlayer(&app->room, var_r4));
    }
    if (WifiClub_IsRosterScreenActive(app) == 0) {
        WifiClub_InitRosterScreen(app, narc, HEAP_ID_53);
    }
    WifiClub_RedrawRoster(app, HEAP_ID_53);
    WifiClub_CloseMessage(app);
    WifiClub_CollectOnlineFriends(app);
    WifiClub_CreateStatusBarWindows(app);
    WifiClub_SetStatusAndSend(app, 16);
    CommState_SetWifiClubLinkMode();
    app->contactFriend = -1;
    NARC_Delete(narc);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_53);
    app->state = 19;
    CommState_SetErrorWatch(0, 0);
    return mainState;
}

s32 WifiClub_IsMatchConnected(WifiClubApp *app) {
    if (CommState_GetWifiMatchStatus() == 1) {
        return 1;
    }
    return 0;
}

s32 WifiClub_CheckIncomingContact(WifiClubApp *app) {
    if (WifiConnection_GetNewClientIndex() != -1) {
        WifiConnection_SetVoiceChatEnabled(app->clubData->own.voiceChat);
        return 1;
    }
    return 0;
}

s32 WifiClub_SyncRoomPlayers(WifiClubApp *app) {
    int onlineCount = WifiClub_CollectOnlineFriends(app);
    int friendAdded = 0;
    int friendLeft = 0;
    if (memcmp(app->onlineFriends, app->prevOnlineFriends, 32) != 0) {
        for (int i = 0; i < 32; i++) {
            int j;
            int var_r2 = 0;
            for (j = 0; j < 32; j++) {
                if (app->onlineFriends[j] == app->prevOnlineFriends[i]) {
                    var_r2 = 1;
                    break;
                }
            }
            if ((var_r2 == 0) && (app->prevOnlineFriends[i] != 0)) {
                WifiClubRoomPerson *leavingPerson = WifiClub_FindPlayerObject(app, app->prevOnlineFriends[i]);
                if (leavingPerson != NULL) {
                    WifiClubRoom_StartPersonLeave(&app->room, leavingPerson);
                    WifiClub_RemovePlayerObject(app, leavingPerson);
                    WifiClub_ClearRosterSlotState(app, app->prevOnlineFriends[i]);
                    WifiClub_CancelRosterSelectionOf(app, app->prevOnlineFriends[i], HEAP_ID_53);
                    friendLeft = 1;
                }
            }
            var_r2 = 0;
            for (j = 0; j < 32; j++) {
                if (app->onlineFriends[i] == app->prevOnlineFriends[j]) {
                    var_r2 = 1;
                    break;
                }
            }
            if (var_r2 == 0 && app->onlineFriends[i] != 0) {
                WifiClubPlayerStatus *temp = WifiClub_GetFriendStatusData(app, app->onlineFriends[i] - 1);
                WifiClubRoomPerson *newPerson = WifiClubRoom_AddFriend(&app->room, temp->trainerInfo[1], app->onlineFriends[i]);
                if (newPerson != NULL) {
                    WifiClub_SetRosterSlotState(app, app->onlineFriends[i], 2);
                    WifiClub_AddPlayerObject(app, newPerson);
                    friendAdded = 1;
                } else {
                    WifiClub_SetRosterSlotState(app, app->onlineFriends[i], 1);
                    app->onlineFriends[i] = 0;
                }
            }
        }
        memcpy(app->prevOnlineFriends, app->onlineFriends, 32);
        WifiClub_RequestRosterRedraw(app);
        if (friendAdded == 1) {
            PlaySE(SEQ_SE_DP_TELE2);
        } else if (friendLeft == 1) {
            PlaySE(SEQ_SE_DP_TELE2);
        }
    }
    return onlineCount;
}

void WifiClub_RefreshFriendStatuses(WifiClubApp *app, enum HeapID heapID) {
    if (WifiClub_UpdateFriendStatuses(app) > 0) {
        WifiClub_RedrawRoster(app, heapID);
    }
}

void WifiClub_UpdateRoomPlayers(WifiClubApp *app, enum HeapID heapID) {
    WifiClub_SyncRoomPlayers(app);
    WifiClub_RefreshFriendStatuses(app, heapID);
}

s32 WifiClub_State_Room(WifiClubApp *app, s32 mainState) {
    u32 var_r7 = 0;
    if (IsPaletteFadeFinished() == 0) {
        return mainState;
    }
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    if (CommState_GetWifiMatchStatus() == 4) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00018, 0);
        app->state = 29;
        WifiClub_SetStatusAndSend(app, 16);
        CommState_LeaveWifiMatch();
        return mainState;
    }
    WifiClub_UpdateRoomPlayers(app, HEAP_ID_53);
    WifiClub_RedrawRosterIfNeeded(app);
    WifiClubRoom_ContinueLightSequence(&app->room);
    if (WifiClubRoom_IsInputLocked(&app->room) == 0) {
        if ((WifiConnection_GetNewClientIndex() == -1) && (gSystem.newKeys & PAD_BUTTON_X)) {
            if (WifiClub_ToggleVoiceChatSetting(app) != 0) {
                WifiClub_PrintMessage(app, msg_0778_00090, 0);
            } else {
                WifiClub_PrintMessage(app, msg_0778_00091, 0);
            }
            PlaySE(SEQ_SE_DP_DECIDE);
            WifiClub_PrintOwnStatus(app);
            app->state = 20;
            WifiClub_CancelRosterSelection(app, HEAP_ID_53);
            return mainState;
        }
        u32 selectedFriend = WifiClub_UpdateRosterScreen(app, HEAP_ID_53);
        WifiClubRoomPerson *person = WifiClub_FindPlayerObject(app, selectedFriend);
        if (selectedFriend != 0) {
            if (person != NULL) {
                WifiClubRoom_ShowFriendMarker(&app->room, person);
            } else {
                WifiClubRoom_HideFriendMarker(&app->room);
            }
        } else {
            WifiClubRoom_HideFriendMarker(&app->room);
        }
    }
    if ((WifiClub_IsMatchConnected(app) != 0) && (app->contactFriend != -1)) {
        PlaySE(SEQ_SE_DP_UG_020);
        app->state = 48;
        return mainState;
    }
    if ((app->contactFriend == -1) && (WifiConnection_GetNewClientIndex() != -1)) {
        app->contactFriend = WifiConnection_GetNewClientIndex();
        WifiClub_BufferFriendName(app, app->contactFriend);
        WifiClub_PrintMessage(app, msg_0778_00076, 0);
        WifiConnection_SetVoiceChatEnabled(app->clubData->own.voiceChat);
        app->unk344 = 0;
    } else if ((app->contactFriend != -1) && (WifiConnection_GetNewClientIndex() == -1)) {
        WifiConnection_SetVoiceChatEnabled(0);
        WifiClub_BufferFriendName(app, app->contactFriend);
        WifiClub_PrintMessage(app, msg_0778_00018, 0);
        CommState_LeaveWifiMatch();
        app->state = 27;
        return mainState;
    }
    if ((WifiConnection_IsVoiceChatEnabled() == 1) && (WifiConnection_GetNewClientIndex() == -1)) {
        WifiConnection_SetVoiceChatEnabled(0);
    }
    u32 r0 = WifiClub_GetPlayerStatus(app, &app->clubData->own);
    if (app->contactFriend == -1) {
        if ((gSystem.newKeys & PAD_BUTTON_B) && (WifiClub_IsSeekingStatus(r0) != 0)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            app->state = 39;
            WifiClub_CancelRosterSelection(app, HEAP_ID_53);
            return mainState;
        }
        var_r7 = WifiClubRoom_Update(&app->room);
        WifiClubRoom_UpdateLights(&app->room);
    }
    switch (var_r7) {
    case 0:
        return mainState;
    case 1:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (WifiClub_IsSeekingStatus(r0) != 0) {
            app->state = 39;
        } else {
            app->exitChoice = 8;
            app->state = 57;
            WifiClub_PrintMessage(app, msg_0800_00025, 1);
        }
        WifiClub_CancelRosterSelection(app, HEAP_ID_53);
        return mainState;
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (WifiClub_IsSeekingStatus(r0) != 0) {
            WifiClubRoom_StartCounterLight(&app->room);
            app->state = 39;
        } else if (r0 == 16 && CommState_IsWifiInLobby() == 1) {
            WifiClubRoom_StartCounterLight(&app->room);
            app->state = 36;
        }
        WifiClub_CancelRosterSelection(app, HEAP_ID_53);
        return mainState;
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (WifiClub_IsSeekingStatus(r0) != 0) {
            app->state = 39;
        } else {
            app->state = 42;
        }
        WifiClub_CancelRosterSelection(app, HEAP_ID_53);
        return mainState;
    }
    GF_AssertFail();
    return mainState;
}

s32 WifiClub_State_VoiceChatToggleMessage(WifiClubApp *app, s32 mainState) {
    if (WifiClub_CheckIncomingContact(app) != 0) {
        app->state = 19;
        WifiClub_StopSeeking(app);
    } else if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        app->state = 19;
        WifiClub_StopSeeking(app);
        WifiClub_CloseMessage(app);
    }
    return mainState;
}

s32 WifiClub_State_WaitVoiceChatPartner(WifiClubApp *app, s32 mainState) {
    WifiClubPlayerStatus *partnerStatusData = WifiClub_GetFriendStatusData(app, app->partnerId - 1);
    u8 partnerVoiceChat = partnerStatusData->voiceChat;
    u32 partnerStatus = WifiClub_GetPlayerStatus(app, partnerStatusData);
    app->matchTimer -= 1;
    if (app->matchTimer < 0) {
        app->state = 70;
    } else if (CommState_GetWifiMatchStatus() == 5) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00015, 0);
        app->state = 27;
    } else if (CommState_GetWifiMatchStatus() == 3) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00018, 0);
        app->state = 27;
    } else if ((CommState_GetWifiMatchStatus() == 4) || (CommState_WasWifiDisconnected() != 0)) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00019, 0);
        app->state = 27;
    } else if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
    } else if (partnerVoiceChat == 0) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 27;
    } else if (partnerStatus != 16) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 27;
    } else if (CommState_GetWifiMatchStatus() == 1) {
        WifiClub_CloseMessage(app);
        WifiClub_SetStatusAndSend(app, 1);
        app->state = 24;
        FriendRoster_UpdateDate(app->friendRoster, app->partnerId - 1);
    }
    return mainState;
}

s32 WifiClub_State_StartVoiceChat(WifiClubApp *app, s32 mainState) {
    WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
    WifiClub_PrintMessage(app, msg_0778_00014, 0);
    WifiClub_SetStatusAndSend(app, 1);
    app->state = 23;
    if (WifiConnection_GetTargetFriendIndex() != -1) {
        FriendRoster_UpdateDate(app->friendRoster, WifiConnection_GetTargetFriendIndex());
    }
    return mainState;
}

s32 WifiClub_State_VoiceChatStartMessage(WifiClubApp *app, s32 mainState) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    if (WifiConnection_GetTargetFriendIndex() < 0) {
        WifiClub_PrintMessage(app, msg_0778_00019, 0);
        app->state = 27;
        return mainState;
    }
    u32 partnerStatus = WifiClub_GetPlayerStatus(app, WifiClub_GetFriendStatusData(app, WifiConnection_GetTargetFriendIndex()));
    if ((partnerStatus != 16) && (partnerStatus != 1)) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 27;
        return mainState;
    }
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
            WifiClub_CloseMessage(app);
            app->state = 24;
        } else {
            WifiClub_State_VoiceChat(app, mainState);
        }
    }
    return mainState;
}

s32 WifiClub_State_VoiceChat(WifiClubApp *app, s32 mainState) {
    if (WifiConnection_GetTargetFriendIndex() < 0) {
        WifiClub_PrintMessage(app, msg_0778_00019, 0);
        app->state = 27;
        return mainState;
    }
    int partnerStatus = WifiClub_GetPlayerStatus(app, WifiClub_GetFriendStatusData(app, WifiConnection_GetTargetFriendIndex()));
    WifiClub_UpdateRoomPlayers(app, HEAP_ID_53);
    WifiClub_RedrawRosterIfNeeded(app);
    if (CommState_GetWifiMatchStatus() == 5) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00015, 0);
        app->state = 27;
    } else if (CommState_GetWifiMatchStatus() == 3) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00018, 0);
        app->state = 27;
    } else if ((CommState_GetWifiMatchStatus() == 4) || (CommState_WasWifiDisconnected() != 0)) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00019, 0);
        app->state = 27;
    } else if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
    } else if ((partnerStatus != 16) && (partnerStatus != 1)) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 27;
    } else if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        WifiClub_PrintMessage(app, msg_0778_00020, 0);
        app->state = 25;
    } else if (WifiConnection_GetVoiceActivity() != 0) {
        WifiClub_DrawIconToBg(app->bgConfig, &app->statusIcons, 1, 2, 1, 2, 0);
    } else {
        WifiClub_DrawIconToBg(app->bgConfig, &app->statusIcons, 1, 2, 1, 3, 0);
    }
    return mainState;
}

s32 WifiClub_State_CreateEndVoiceChatPrompt(WifiClubApp *app, s32 mainState) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        if (WifiConnection_GetTargetFriendIndex() != -1) {
            FriendRoster_UpdateDate(app->friendRoster, WifiConnection_GetTargetFriendIndex());
        }
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            app->yesNoPrompt = WifiClub_CreateYesNoPromptIgnoreTouch(app->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        app->state = 26;
    }
    return mainState;
}

s32 WifiClub_State_EndVoiceChatPrompt(WifiClubApp *app, s32 mainState) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 menuResult = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_53);
        if (menuResult == -1) {
            if ((CommState_GetWifiMatchStatus() >= 4) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0)) {
                WifiClub_CloseMessage(app);
                WifiClub_SetStatusAndSend(app, 16);
                Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
                app->contactFriend = -1;
                app->timer = 20;
                app->state = 65;
            } else if (CommState_HasWifiFailed() != 0) {
                WifiClub_HandleWifiError(app);
            }
            return mainState;
        }
        if (menuResult == 0) {
            if (CommState_HasWifiFailed() == 0) {
                WifiClub_SetStatusAndSend(app, 16);
                CommState_SendWifiDisconnect();
                app->contactFriend = -1;
                app->timer = 20;
                app->state = 65;
            } else {
                WifiClub_HandleWifiError(app);
            }
        } else {
            app->state = 24;
        }
    } else {
        enum YesNoResponse yesNoReponse = YesNoPrompt_HandleInput(app->yesNoPrompt);
        if (yesNoReponse == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            if (CommState_HasWifiFailed() == 0) {
                WifiClub_SetStatusAndSend(app, 16);
                CommState_SendWifiDisconnect();
                app->contactFriend = -1;
                app->timer = 20;
                app->state = 65;
            } else {
                WifiClub_HandleWifiError(app);
            }
        } else if (yesNoReponse == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            app->state = 24;
        } else {
            if ((CommState_GetWifiMatchStatus() >= 4) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0)) {
                WifiClub_CloseMessage(app);
                WifiClub_SetStatusAndSend(app, 16);
                YesNoPrompt_Destroy(app->yesNoPrompt);
                app->contactFriend = -1;
                app->timer = 20;
                app->state = 65;
            } else if (CommState_HasWifiFailed() != 0) {
                WifiClub_HandleWifiError(app);
            }
            return mainState;
        }
    }
    WifiClub_CloseMessage(app);
    return mainState;
}

s32 WifiClub_State_MessageThenLeaveMatch(WifiClubApp *app, s32 mainState) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    if (WifiClub_GetPlayerStatus(app, &app->clubData->own) != 16) {
        WifiClub_SetStatusAndSend(app, 16);
    }
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        app->timer = 30;
        return mainState;
    }
    app->timer -= 1;
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (app->timer == 0) || (System_GetTouchNew() == 1)) {
        WifiClub_CloseMessage(app);
        CommState_LeaveWifiMatch();
        CommState_SetWifiClubLinkMode();
        if (WifiClubRoom_IsInitialized(&app->room) == 0) {
            app->timer = 20;
            app->state = 65;
        } else {
            WifiClub_RestoreVoiceChatSetting(app);
            WifiClub_PrintOwnStatus(app);
            app->contactFriend = -1;
            app->state = 19;
            WifiClub_StopSeeking(app);
        }
    }
    return mainState;
}

s32 WifiClub_State_MessageThenReenterRoom(WifiClubApp *app, s32 mainState) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        WifiClub_CloseMessage(app);
        WifiClub_SetStatusAndSend(app, 16);
        CommState_LeaveWifiMatch();
        app->timer = 20;
        app->state = 65;
    }
    return mainState;
}

s32 WifiClub_State_MessageThenReturnToRoom(WifiClubApp *app, s32 mainState) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        app->timer = 30;
        return mainState;
    }
    app->timer -= 1;
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (app->timer == 0) || (System_GetTouchNew() == 1)) {
        WifiClub_CloseMessage(app);
        WifiClub_SetStatusAndSend(app, 16);
        CommState_SetWifiClubLinkMode();
        if (WifiClubRoom_IsInitialized(&app->room) == 0) {
            app->state = WifiClub_FadeOutToEnterRoom();
        } else {
            WifiClub_RestoreVoiceChatSetting(app);
            WifiClub_PrintOwnStatus(app);
            app->contactFriend = -1;
            app->state = 19;
            WifiClub_StopSeeking(app);
        }
    }
    return mainState;
}

s32 WifiClub_State_Dummy30(WifiClubApp *app, s32 mainState) {
    return mainState;
}

s32 WifiClub_State_Dummy31(WifiClubApp *app, s32 mainState) {
    return mainState;
}

s32 WifiClub_State_Dummy32(WifiClubApp *app, s32 mainState) {
    return mainState;
}

s32 WifiClub_State_ErrorThenDisconnect(WifiClubApp *app, s32 mainState) {
    if (app->timer > 0) {
        app->timer--;
        return mainState;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        WifiClub_PrintMessage(app, msg_0800_00026, 1);
        app->state = 59;
        app->timer = 1;
    }
    return mainState;
}

s32 WifiClub_State_AskCancelSeeking(WifiClubApp *app, s32 mainState) {
    WifiClubRoom_UpdateLights(&app->room);
    WifiClub_PrintMessage(app, msg_0778_00010, 0);
    app->state = 40;
    return mainState;
}

s32 WifiClub_State_CreateCancelSeekingPrompt(WifiClubApp *app, s32 mainState) {
    WifiClubRoom_UpdateLights(&app->room);
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            app->yesNoPrompt = WifiClub_CreateYesNoPromptIgnoreTouch(app->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        app->state = 41;
    }
    return mainState;
}

s32 WifiClub_State_CancelSeekingPrompt(WifiClubApp *app, s32 mainState) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 var_r5 = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_53);
        WifiClubRoom_UpdateLights(&app->room);
        if ((app->contactFriend == -1) && (WifiConnection_GetNewClientIndex() != -1)) {
            WifiConnection_SetVoiceChatEnabled(app->clubData->own.voiceChat);
            if (var_r5 == -1) {
                Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
            }
            var_r5 = 1;
        }
        if (var_r5 == -1) {
            return mainState;
        } else if (var_r5 == 0) {
            WifiClub_SetStatusAndSend(app, 16);
            CommState_SetWifiClubLinkMode();
            WifiClub_StopSeeking(app);
        } else {
            WifiClub_StopSeeking(app);
            WifiClub_StartSeeking(app, 1);
        }
    } else {
        enum YesNoResponse yesNoResponse = YesNoPrompt_HandleInput(app->yesNoPrompt);
        WifiClubRoom_UpdateLights(&app->room);
        if ((app->contactFriend == -1) && (WifiConnection_GetNewClientIndex() != -1)) {
            WifiConnection_SetVoiceChatEnabled(app->clubData->own.voiceChat);
            yesNoResponse = YESNORESPONSE_NO;
        }
        if (yesNoResponse == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            WifiClub_SetStatusAndSend(app, 16);
            CommState_SetWifiClubLinkMode();
            WifiClub_StopSeeking(app);
        } else if (yesNoResponse == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            WifiClub_StopSeeking(app);
            WifiClub_StartSeeking(app, 1);
        } else {
            return mainState;
        }
    }
    app->state = 19;
    return mainState;
}

s32 WifiClub_State_OpenSeekMenu(WifiClubApp *app, s32 mainState) {
    s32 listLen = 3;
    ListMenuTemplate listMenuTemplate = WifiClubSeekMenuTemplate;
    WifiClubMenuItem *listMenuItems = WifiClubSeekMenuItems;

    if (WifiClub_IsPoffinCookingAvailable(app) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;

        s32 index = listLen - 1;
        listMenuItems[index] = WifiClubSeekItem_Cook;
    }
    if (WifiClub_HasBattleFrontier(app) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;

        s32 index = listLen - 1;
        listMenuItems[index] = WifiClubSeekItem_Frontier;
    }
    if (WifiClub_ArePlazaGamesAvailable(app) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;

        s32 index = listLen - 1;
        listMenuItems[index] = WifiClubSeekItem_PlazaGames;
    }
    listMenuTemplate.totalItems++;
    listMenuTemplate.maxShowed++;
    listMenuItems[listLen] = WifiClubSeekItem_Cancel;
    app->menuItems = ListMenuItems_New(listLen + 1, HEAP_ID_53);

    for (s32 i = 0; i < listLen + 1; i++) {
        ListMenuItems_AppendFromMsgData(app->menuItems, app->msgData, listMenuItems[i].strno, listMenuItems[i].value);
    }
    if (WindowIsInUse(&app->menuWindow) != 0) {
        EraseFrameAndWindow1(&app->menuWindow, FALSE);
        RemoveWindow(&app->menuWindow);
    }
    s32 height = (listLen + 1) * 2;
    AddWindowParameterized(app->bgConfig, &app->menuWindow, 2, 16, 3, 15, height, 13, 59 - height);
    DrawFrameAndWindow1(&app->menuWindow, TRUE, 473, 11);
    listMenuTemplate.items = app->menuItems;
    listMenuTemplate.window = &app->menuWindow;
    app->listMenu = ListMenuInit(&listMenuTemplate, 0, app->seekMenuCursor, HEAP_ID_53);
    ScheduleWindowCopyToVram(&app->menuWindow);
    WifiClub_PrintMessage(app, msg_0778_00008, 0);
    WifiClubRoom_UpdateLights(&app->room);
    app->state = 37;
    return mainState;
}

s32 WifiClub_State_SeekMenu(WifiClubApp *app, s32 mainState) {
    s32 battleReadyMons = WifiClub_CountBattleReadyMons(app);
    s32 cookingError = WifiClub_GetCookingError(app);
    WifiClubRoom_UpdateLights(&app->room);
    if (CommState_HasWifiFailed() != 0) {
        EraseFrameAndWindow1(&app->menuWindow, FALSE);
        RemoveWindow(&app->menuWindow);
        DestroyListMenu(app->listMenu, NULL, &app->seekMenuCursor);
        ListMenuItems_Delete(app->menuItems);
        WifiClub_CloseMessage(app);
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        if (WifiClub_CheckIncomingContact(app) != 0) {
            app->state = 19;
            EraseFrameAndWindow1(&app->menuWindow, FALSE);
            RemoveWindow(&app->menuWindow);
            DestroyListMenu(app->listMenu, NULL, &app->seekMenuCursor);
            ListMenuItems_Delete(app->menuItems);
            WifiClub_CloseMessage(app);
        }
        return mainState;
    }
    s32 var_r4 = ListMenu_ProcessInput(app->listMenu);
    if (WifiClub_CheckIncomingContact(app) != 0) {
        var_r4 = -2;
    }
    switch (var_r4) {
    case -1:
        return mainState;
    case -2:
        app->state = 19;
        WifiClub_CloseMessage(app);
        PlaySE(SEQ_SE_DP_DECIDE);
        break;
    default:
        PlaySE(SEQ_SE_DP_DECIDE);
        if ((var_r4 == 19) && (cookingError != 0)) {
            if (cookingError == 1) {
                WifiClub_PrintMessage(app, msg_0778_00104, 0);
            } else if (cookingError == 3) {
                WifiClub_PrintMessage(app, msg_0778_00106, 0);
            } else {
                WifiClub_PrintMessage(app, msg_0778_00105, 0);
            }
            app->state = 29;
        } else if ((var_r4 == 15) && (battleReadyMons < 2)) {
            WifiClub_PrintMessage(app, msg_0778_00089, 0);
            app->state = 29;
        } else if ((var_r4 == 1) && (battleReadyMons < 2)) {
            WifiClub_PrintMessage(app, msg_0778_00103, 0);
            app->state = 29;
        } else if ((var_r4 == 0) || (var_r4 == 1) || (var_r4 == 29)) {
            EraseFrameAndWindow1(&app->menuWindow, FALSE);
            RemoveWindow(&app->menuWindow);
            DestroyListMenu(app->listMenu, NULL, &app->seekMenuCursor);
            ListMenuItems_Delete(app->menuItems);
            WifiClub_OpenSeekSubmenu(app, var_r4);
            app->state = 38;
            return mainState;
        } else {
            WifiClub_SetStatusAndSend(app, var_r4);
            app->state = 19;
            WifiClub_CloseMessage(app);
        }
        s32 var_r6 = 1;
        if ((WifiClub_IsGroupActivityStatus(var_r4) != 0) && (app->state == 19)) {
            app->state = 68;
            WifiClub_StartMatch(app, -1, var_r4);
            WifiClub_SetLinkModeForStatus(var_r4);
            WifiClub_SetStatusAndSend(app, var_r4);
            app->exitChoice = WifiClub_GetGroupActivityChoice(var_r4);
            var_r6 = 0;
        } else {
            WifiClub_StartMatch(app, -1, var_r4);
            WifiClub_SetLinkModeForStatus(var_r4);
        }
        if (app->state == 19) {
            WifiClub_StartSeeking(app, var_r6);
        }
    }

    EraseFrameAndWindow1(&app->menuWindow, FALSE);
    RemoveWindow(&app->menuWindow);
    DestroyListMenu(app->listMenu, NULL, &app->seekMenuCursor);
    ListMenuItems_Delete(app->menuItems);
    return mainState;
}

s32 WifiClub_State_StartRecruiting(WifiClubApp *app, s32 mainState) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    WifiClubRoom_UpdateLights(&app->room);
    if (WifiClub_CheckIncomingContact(app) != 0) {
        app->state = 19;
    }
    if (CommState_IsWifiInLobby() != 0) {
        CommState_SetErrorWatch(0, 1);
        Comm_SetWifiContinuousSend(0);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
        app->isRematch = 0;
        CommPlayerInfo_Init(app->saveData, 0);
        CommPlayerInfo_SendLocal();
        CommPlayerInfo_Accept(Comm_GetLocalNetId());
        mainState = 2;
    }
    return mainState;
}

s32 WifiClub_OpenSeekSubmenu(WifiClubApp *app, s32 menuChoice) {
    ListMenuTemplate listMenuTemplate;
    WifiClubMenuItem *listMenuItems;
    s32 listLen;
    switch (menuChoice) {
    case 0:
        app->unk37C = 1;
        listMenuItems = WifiClubSingleBattleItems;
        listLen = NELEMS(WifiClubSingleBattleItems);
        listMenuTemplate = sBattleSubmenuTemplate;
        break;
    case 1:
        app->unk37C = 0;
        listMenuItems = WifiClubDoubleBattleItems;
        listLen = NELEMS(WifiClubDoubleBattleItems);
        listMenuTemplate = sBattleSubmenuTemplate;
        break;
    case 29:
        listMenuItems = WifiClubPlazaGameItems;
        listLen = NELEMS(WifiClubPlazaGameItems);
        listMenuTemplate = sPlazaGameSubmenuTemplate;
        app->unk37C = 2;
        break;
    }
    app->menuItems = ListMenuItems_New(listLen, HEAP_ID_53);
    for (s32 i = 0; i < listLen; i++) {
        if (listMenuItems[i].strno != msg_0778_00071) {
            ListMenuItems_AppendFromMsgData(app->menuItems, app->msgData, listMenuItems[i].strno, listMenuItems[i].value);
        } else {
            BufferWiFiPlazaActivityName(app->msgFormat, 0, i);
            ReadMsgDataIntoString(app->msgData, listMenuItems[i].strno, app->msgRaw);
            StringExpandPlaceholders(app->msgFormat, app->tmpString, app->msgRaw);
            ListMenuItems_AddItem(app->menuItems, app->tmpString, listMenuItems[i].value);
        }
    }
    if (WindowIsInUse(&app->menuWindow) != 0) {
        EraseFrameAndWindow1(&app->menuWindow, FALSE);
        RemoveWindow(&app->menuWindow);
    }
    u32 windowHeight = listLen * 2;
    AddWindowParameterized(app->bgConfig, &app->menuWindow, 2, 16, 9, 15, windowHeight, 13, 59 - windowHeight);
    DrawFrameAndWindow1(&app->menuWindow, TRUE, 473, 11);
    listMenuTemplate.items = app->menuItems;
    listMenuTemplate.window = &app->menuWindow;
    app->listMenu = ListMenuInit(&listMenuTemplate, 0, (&app->unk376)[app->unk37C], HEAP_ID_53);
    ScheduleWindowCopyToVram(&app->menuWindow);
    return 1;
}

s32 WifiClub_State_SeekSubmenu(WifiClubApp *app, s32 mainState) {
    WifiClubRoom_UpdateLights(&app->room);
    if (CommState_HasWifiFailed() != 0) {
        EraseFrameAndWindow1(&app->menuWindow, FALSE);
        RemoveWindow(&app->menuWindow);
        DestroyListMenu(app->listMenu, 0, &app->unk376 + app->unk37C);
        ListMenuItems_Delete(app->menuItems);
        WifiClub_CloseMessage(app);
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    if (WifiClub_CheckIncomingContact(app) != 0) {
        app->state = 19;
        EraseFrameAndWindow1(&app->menuWindow, FALSE);
        RemoveWindow(&app->menuWindow);
        DestroyListMenu(app->listMenu, 0, &app->unk376 + app->unk37C);
        ListMenuItems_Delete(app->menuItems);
        WifiClub_CloseMessage(app);
        return mainState;
    }
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    s32 selection = ListMenu_ProcessInput(app->listMenu);
    switch (selection) {
    case -1:
        return mainState;
    case -2:
        PlaySE(SEQ_SE_DP_DECIDE);
        app->state = 36;
        break;
    default:
        PlaySE(SEQ_SE_DP_DECIDE);
        WifiClub_SetStatusAndSend(app, selection);
        app->state = 19;
        WifiClub_CloseMessage(app);
    }
    if (app->state == 19) {
        s32 var_r7 = 1;
        if (WifiClub_IsGroupActivityStatus(selection) != 0) {
            app->state = 68;
            WifiClub_StartMatch(app, -1, selection);
            WifiClub_SetLinkModeForStatus(selection);
            WifiClub_SetStatusAndSend(app, selection);
            app->exitChoice = WifiClub_GetGroupActivityChoice(selection);
            var_r7 = 0;
        } else {
            WifiClub_StartMatch(app, -1, selection);
            CommState_SetSingleBattleWifiLinkMode();
        }
        WifiClub_StartSeeking(app, var_r7);
    }
    EraseFrameAndWindow1(&app->menuWindow, FALSE);
    RemoveWindow(&app->menuWindow);
    DestroyListMenu(app->listMenu, 0, &app->unk376 + app->unk37C);
    ListMenuItems_Delete(app->menuItems);
    return mainState;
}

s32 WifiClub_State_TalkToPlayer(WifiClubApp *app, s32 mainState) {
    u8 partnerId = WifiClubRoom_GetTalkPartnerId(&app->room);
    WifiClubRoomPerson *playerPerson = WifiClub_FindPlayerObject(app, 0);
    GF_ASSERT(playerPerson != 0);
    WifiClubRoomPerson *partnerPerson = WifiClub_FindPlayerObject(app, partnerId);
    if (partnerPerson == 0) {
        WifiClub_BufferFriendName(app, partnerId - 1);
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 29;
        return mainState;
    }
    WifiClubRoom_StartTalk(&app->room, partnerPerson, WifiClubRoomPerson_GetOppositeDirection(playerPerson));
    u32 friendIndex = partnerId - 1;
    u16 partnerStatus = WifiClub_GetPlayerStatus(app, WifiClub_GetFriendStatusData(app, friendIndex));
    app->partnerStatus = partnerStatus;
    WifiClub_BufferFriendName(app, friendIndex);
    s32 var_r1 = 0;

    if (partnerStatus == 15) {
        var_r1 = msg_0778_00003;
    } else if (partnerStatus == 19) {
        var_r1 = msg_0778_00004;
    } else if (partnerStatus == 18) {
        var_r1 = msg_0778_00083;
    } else if (partnerStatus == 21) {
        var_r1 = msg_0778_00005;
    } else if (partnerStatus == 20) {
        var_r1 = msg_0778_00084;
    } else if (partnerStatus == 23) {
        BufferWiFiPlazaActivityName(app->msgFormat, 1, 0);
        var_r1 = msg_0778_00006;
    } else if (partnerStatus == 22) {
        var_r1 = msg_0778_00085;
    } else if (partnerStatus == 25) {
        BufferWiFiPlazaActivityName(app->msgFormat, 1, 1);
        var_r1 = msg_0778_00006;
    } else if (partnerStatus == 24) {
        var_r1 = msg_0778_00085;
    } else if (partnerStatus == 27) {
        BufferWiFiPlazaActivityName(app->msgFormat, 1, 2);
        var_r1 = msg_0778_00006;
    } else if (partnerStatus == 26) {
        var_r1 = msg_0778_00085;
    } else {
        if (WifiClub_IsSeekingBattleStatus(partnerStatus) != 0) {
            var_r1 = msg_0778_00002;
        } else if (partnerStatus == 8) {
            var_r1 = msg_0778_00082;
        } else if (WifiClub_IsInBattleStatus(partnerStatus) != 0) {
            var_r1 = msg_0778_00081;
        } else if (partnerStatus == 1) {
            var_r1 = msg_0778_00086;
        } else {
            var_r1 = msg_0778_00007;
        }
    }
    WifiClub_PrintMessage(app, var_r1, 0);
    app->state = 43;
    return mainState;
}

s32 WifiClub_State_OpenApplyMenu(WifiClubApp *app, s32 mainState) {
    s32 i;
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    s32 partnerId = WifiClubRoom_GetTalkPartnerId(&app->room);
    WifiClubRoomPerson *partnerPerson = WifiClub_FindPlayerObject(app, partnerId);
    if (WifiClub_CheckIncomingContact(app) != 0) {
        WifiClubRoom_EndTalk(&app->room, partnerPerson);
        WifiClub_CloseMessage(app);
        app->state = 19;
        return mainState;
    }
    if (partnerPerson == 0) {
        WifiClubRoom_EndTalk(&app->room, partnerPerson);
        DestroyListMenu(app->listMenu, 0, 0);
        WifiClub_BufferFriendName(app, partnerId - 1);
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 29;
        return mainState;
    }
    if (app->partnerStatus != WifiClub_GetPlayerStatus(app, WifiClub_GetFriendStatusData(app, partnerId - 1))) {
        WifiClubRoom_EndTalk(&app->room, partnerPerson);
        WifiClub_BufferFriendName(app, partnerId - 1);
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 29;
        return mainState;
    }
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    s32 itemCount = 2;
    ListMenuTemplate listMenuTemplate = sApplyMenuTemplate;
    const WifiClubMenuItem *listMenuItems = sApplyMenuItems;
    WifiClubPlayerStatus *r6 = WifiClub_GetFriendStatusData(app, WifiClubRoom_GetTalkPartnerId(&app->room) - 1);
    u16 partnerStatus = WifiClub_GetPlayerStatus(app, r6);
    u8 r1 = r6->voiceChat;
    app->partnerVoiceChat = r1;
    u16 val = r1 + app->clubData->own.voiceChat;
    app->menuItems = ListMenuItems_New(2, HEAP_ID_53);

    for (i = 0; i < NELEMS(sApplyMenuItems); i++) {
        if (i == 0) {
            if ((WifiClub_IsBusyStatus(partnerStatus) != 0) || (partnerStatus == 0) || (partnerStatus == 28) || (partnerStatus >= 29)) {
                listMenuTemplate.maxShowed--;
                listMenuTemplate.totalItems--;
                itemCount -= 1;
            } else if (partnerStatus == 16) {
                if (val == 2) {
                    ListMenuItems_AppendFromMsgData(app->menuItems, app->msgData, msg_0778_00036, listMenuItems[i].value);
                } else {
                    listMenuTemplate.maxShowed--;
                    listMenuTemplate.totalItems--;
                    itemCount -= 1;
                }
            } else {
                ListMenuItems_AppendFromMsgData(app->menuItems, app->msgData, listMenuItems[i].strno, listMenuItems[i].value);
            }
        } else {
            ListMenuItems_AppendFromMsgData(app->menuItems, app->msgData, listMenuItems[i].strno, listMenuItems[i].value);
        }
    }
    AddWindowParameterized(app->bgConfig, &app->menuWindow, 2, 16, ((3 - itemCount) * 2) + 11, 15, itemCount * 2, 13, 59);
    DrawFrameAndWindow1(&app->menuWindow, TRUE, 473, 11);
    listMenuTemplate.items = app->menuItems; // itemPrintFunc
    listMenuTemplate.window = &app->menuWindow;
    app->listMenu = ListMenuInit(&listMenuTemplate, 0, 0, HEAP_ID_53);
    ScheduleWindowCopyToVram(&app->menuWindow);
    app->state = 44;
    return mainState;
}

s32 WifiClub_IsPartyValid(WifiClubPlayerStatus *playerStatus) {
    for (s32 i = 0; i < 6; i++) {
        if (playerStatus->species[i] == 495) {
            return 0;
        }
        if (playerStatus->species[i] > 495) {
            return 0;
        }
        if (playerStatus->heldItems[i] > 536) {
            return 0;
        }
    }
    return 1;
}

s32 WifiClub_GetCookingError(WifiClubApp *app) {
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

s32 WifiClub_State_ApplyMenu(WifiClubApp *app, s32 mainState) {
    s32 var_r6 = 0;
    s32 var_r7 = ListMenu_ProcessInput(app->listMenu);
    if (WifiClub_CheckIncomingContact(app) != 0) {
        var_r7 = -2;
    }
    s32 partnerId = WifiClubRoom_GetTalkPartnerId(&app->room);
    WifiClubRoomPerson *partnerPerson = WifiClub_FindPlayerObject(app, partnerId);
    if (CommState_HasWifiFailed() != 0) {
        EraseFrameAndWindow1(&app->menuWindow, FALSE);
        RemoveWindow(&app->menuWindow);
        DestroyListMenu(app->listMenu, 0, NULL);
        ListMenuItems_Delete(app->menuItems);
        if (partnerPerson != 0) {
            WifiClubRoom_EndTalk(&app->room, partnerPerson);
        }
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    switch (var_r7) {
    case -1:
        if (partnerPerson == 0) {
            DestroyListMenu(app->listMenu, 0, NULL);
            WifiClub_BufferFriendName(app, partnerId - 1);
            WifiClub_PrintMessage(app, msg_0778_00016, 0);
            app->state = 29;
            var_r6 = 1;
            break;
        } else {
            WifiClubPlayerStatus *partnerStatusData = WifiClub_GetFriendStatusData(app, partnerId - 1);
            u32 r0 = WifiClub_GetPlayerStatus(app, partnerStatusData);
            u8 r6 = partnerStatusData->voiceChat;
            if ((app->partnerStatus != r0) || ((WifiClub_IsGroupActivityStatus(r0) == 0) && (app->partnerVoiceChat != r6))) {
                WifiClub_BufferFriendName(app, partnerId - 1);
                WifiClub_PrintMessage(app, msg_0778_00016, 0);
                app->state = 29;
                var_r6 = 1;
                break;
            }
            return mainState;
        }
    case -2:
        PlaySE(SEQ_SE_DP_DECIDE);
        app->state = 19;
        break;
    default:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (var_r7 == 1) {
            app->state = 19;
            app->partnerId = partnerId;
            if (partnerId == 0) {
                break;
            } else {
                s32 battleReadyMons = WifiClub_CountBattleReadyMons(app);
                s32 cookingError = WifiClub_GetCookingError(app);
                WifiClubPlayerStatus *friendStatusData = WifiClub_GetFriendStatusData(app, partnerId - 1);
                u32 partnerStatus = WifiClub_GetPlayerStatus(app, friendStatusData);
                s32 partnerVoiceChat = friendStatusData->voiceChat;
                if ((app->partnerStatus != partnerStatus) || ((WifiClub_IsGroupActivityStatus(partnerStatus) == 0) && (app->partnerVoiceChat != partnerVoiceChat))) {
                    WifiClub_BufferFriendName(app, partnerId - 1);
                    WifiClub_PrintMessage(app, msg_0778_00016, 0);
                    app->state = 29;
                    var_r6 = 1;
                } else if (WifiClub_IsPartyValid(friendStatusData) == 0) {
                    WifiClub_BufferFriendName(app, partnerId - 1);
                    WifiClub_PrintMessage(app, msg_0778_00016, 0);
                    app->state = 29;
                    var_r6 = 1;
                } else if ((WifiClub_StatusNeedsTwoMons(partnerStatus) != 0) && (battleReadyMons < 2)) {
                    if (partnerStatus == 15) {
                        WifiClub_PrintMessage(app, msg_0778_00089, 0);
                    } else {
                        WifiClub_PrintMessage(app, msg_0778_00103, 0);
                    }
                    app->state = 29;
                    var_r6 = 1;
                } else if ((partnerStatus == 19) && (cookingError != 0)) {
                    if (cookingError == 1) {
                        WifiClub_PrintMessage(app, msg_0778_00104, 0);
                    } else if (cookingError == 3) {
                        WifiClub_PrintMessage(app, msg_0778_00106, 0);
                    } else {
                        WifiClub_PrintMessage(app, msg_0778_00105, 0);
                    }
                    app->state = 29;
                    var_r6 = 1;
                } else if ((WifiClub_IsGroupActivityStatus(partnerStatus) == 0) && (partnerVoiceChat != app->clubData->own.voiceChat)) {
                    if (partnerVoiceChat != 0) {
                        WifiClub_PrintMessage(app, msg_0778_00134, 0);
                    } else {
                        WifiClub_PrintMessage(app, msg_0778_00135, 0);
                    }
                    WifiClub_SetStatusAndSend(app, WifiClub_GetCounterpartStatus(partnerStatus));
                    app->state = 63;
                    var_r6 = 1;
                } else {
                    u32 counterpartStatus = WifiClub_GetCounterpartStatus(partnerStatus);
                    if (counterpartStatus != 29) {
                        if (WifiClub_GetFriendOnlineState(partnerId - 1) == 6) {
                            if (WifiClub_StartMatch(app, partnerId - 1, counterpartStatus) != 0) {
                                app->matchTimer = 1800;
                                WifiClub_SetLinkModeForStatus(counterpartStatus);
                                WifiClub_SetStatusAndSend(app, counterpartStatus);
                                WifiClub_BufferFriendName(app, partnerId - 1);
                                WifiClub_PrintMessage(app, msg_0778_00017, 0);
                                GF_ASSERT(app->waitingIcon == NULL);
                                app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
                                if (counterpartStatus != 1) {
                                    app->state = 45;

                                } else {
                                    app->matchTimer = 1800;
                                    app->state = 21;
                                }
                                var_r6 = 1;
                            } else {
                                WifiClub_BufferFriendName(app, partnerId - 1);
                                WifiClub_PrintMessage(app, msg_0778_00016, 0);
                                app->state = 29;
                                var_r6 = 1;
                            }
                        } else {
                            WifiClub_BufferFriendName(app, partnerId - 1);
                            WifiClub_PrintMessage(app, msg_0778_00016, 0);
                            app->state = 29;
                            var_r6 = 1;
                        }
                    }
                }
            }
            break;
        }
        if (var_r7 == 2) {
            app->state = 54;
            break;
        }
    }
    if (var_r6 == 0) {
        WifiClub_CloseMessage(app);
    }
    EraseFrameAndWindow1(&app->menuWindow, FALSE);
    RemoveWindow(&app->menuWindow);
    DestroyListMenu(app->listMenu, 0, NULL);
    ListMenuItems_Delete(app->menuItems);
    if (partnerPerson != 0) {
        WifiClubRoom_EndTalk(&app->room, partnerPerson);
    }
    return mainState;
}

s32 WifiClub_State_WaitMatchAccepted(WifiClubApp *app, s32 mainState) {
    app->matchTimer -= 1;
    if (app->matchTimer < 0) {
        app->state = 70;
    } else if (CommState_GetWifiMatchStatus() == 3) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00018, 0);
        app->state = 27;
    } else if (CommState_GetWifiMatchStatus() == 5) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00015, 0);
        app->state = 27;
    } else if ((CommState_GetWifiMatchStatus() == 4) || (CommState_WasWifiDisconnected() != 0)) {
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00018, 0);
        app->state = 27;
    } else if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
    } else if (CommState_GetWifiMatchStatus() == 1) {
        u32 ownStatus = WifiClub_GetPlayerStatus(app, &app->clubData->own);
        if (WifiClub_IsGroupActivityStatus(ownStatus) == 0) {
            app->state = 49;
            WifiClub_CloseMessage(app);
            CommPlayerInfo_Init(app->saveData, 0);
            app->timer = 30;
        } else if (Comm_IsNetIdConnected(0) == 1) {
            WifiClub_CloseMessage(app);
            CommPlayerInfo_Init(app->saveData, 0);
            CommState_SetErrorWatch(0, 1);
            WifiClub_SetLinkModeForStatus(ownStatus);
            app->exitChoice = WifiClub_GetGroupActivityChoice(ownStatus);
            Comm_SetWifiContinuousSend(0);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
            app->isRematch = 0;
            mainState = 2;
        } else if (Comm_IsWifiConnected() == 1) {
            WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
            WifiClub_PrintMessage(app, msg_0778_00018, 0);
            app->state = 27;
        }
    }
    return mainState;
}

s32 WifiClub_State_Dummy66(WifiClubApp *app, s32 mainState) {
    return mainState;
}

s32 WifiClub_State_Dummy67(WifiClubApp *app, s32 mainState) {
    return mainState;
}

s32 WifiClub_State_MatchTimeout(WifiClubApp *app, s32 mainState) {
    if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
        return mainState;
    }
    WifiClub_RestoreVoiceChatSetting(app);
    WifiClub_SetStatusAndSend(app, 16);
    CommState_LeaveWifiMatch();
    app->contactFriend = -1;
    app->state = 19;
    WifiClub_StopSeeking(app);
    WifiClub_CloseMessage(app);
    return mainState;
}

s32 WifiClub_State_Dummy46(WifiClubApp *app, s32 mainState) {
    return mainState;
}

s32 WifiClub_State_Dummy47(WifiClubApp *app, s32 mainState) {
    return mainState;
}

s32 WifiClub_State_Contacted(WifiClubApp *app, s32 mainState) {
    WifiClubPlayerStatus *partnerStatusData = WifiClub_GetFriendStatusData(app, WifiConnection_GetTargetFriendIndex());
    u32 ownStatus = WifiClub_GetPlayerStatus(app, &app->clubData->own);
    u32 partnerStatus = WifiClub_GetPlayerStatus(app, partnerStatusData);

    s32 r1 = app->clubData->own.voiceChat;
    if (ownStatus == 12 && partnerStatus == 5) {
        app->state = 49;
    } else if (ownStatus == 13 && partnerStatus == 6) {
        app->state = 49;
    } else if (ownStatus == 14 && partnerStatus == 7) {
        app->state = 49;
    } else if (ownStatus == 9 && partnerStatus == 2) {
        app->state = 49;
    } else if (ownStatus == 10 && partnerStatus == 3) {
        app->state = 49;
    } else if (ownStatus == 11 && partnerStatus == 4) {
        app->state = 49;
    } else if (ownStatus == 15 && partnerStatus == 8) {
        app->state = 49;
    } else if (ownStatus == 19 && partnerStatus == 18) {
        app->state = 49;
    } else if (ownStatus == 21 && partnerStatus == 20) {
        app->state = 49;
    } else if (ownStatus == 23 && partnerStatus == 22) {
        app->state = 49;
    } else if (ownStatus == 25 && partnerStatus == 24) {
        app->state = 49;
    } else if (ownStatus == 27 && partnerStatus == 26) {
        app->state = 49;
    } else if (ownStatus == 16 && partnerStatus == 1 && r1 > 0) {
        app->state = 22;
        return mainState;
    } else {
        if (CommState_HasWifiFailed() != 0) {
            WifiClub_HandleWifiError(app);
            return mainState;
        }
        WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
        WifiClub_PrintMessage(app, msg_0778_00018, 0);
        app->state = 29;
        WifiClub_SetStatusAndSend(app, 16);
        CommState_LeaveWifiMatch();
        return mainState;
    }
    WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
    WifiClub_PrintMessage(app, msg_0778_00011, 0);
    CommPlayerInfo_Init(app->saveData, 0);
    app->state = 49;
    app->timer = 30;
    return mainState;
}

s32 WifiClub_CheckMatchLost(WifiClubApp *app) {
    if (CommState_GetWifiMatchStatus() >= 3) {
        WifiClub_BufferFriendName(app, app->contactFriend);
        WifiClub_PrintMessage(app, msg_0778_00018, 0);
        app->state = 27;
    } else if (CommState_HasWifiFailed() != 0) {
        WifiClub_HandleWifiError(app);
    } else {
        WifiClubPlayerStatus *partnerStatusData = WifiClub_GetFriendStatusData(app, WifiConnection_GetTargetFriendIndex());
        u32 ownStatus = WifiClub_GetPlayerStatus(app, &app->clubData->own);
        u32 partnerStatus = WifiClub_GetPlayerStatus(app, partnerStatusData);
        if ((ownStatus != WifiClub_GetCounterpartStatus(partnerStatus)) && (ownStatus != partnerStatus)) {
            WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
            WifiClub_PrintMessage(app, msg_0778_00016, 0);
            app->state = 27;

        } else {
            return 0;
        }
    }
    app->isRematch = 0;
    return 1;
}

s32 WifiClub_State_SyncStart(WifiClubApp *app, s32 mainState) {
    if ((CommState_GetWifiMatchStatus() >= 4) || (CommState_WasWifiDisconnected() != 0) || (Comm_IsNetIdConnected(0) == 0)) {
        if (app->isRematch != 0) {
            WifiClub_PrintMessage(app, msg_0778_00101, 0);
        } else {
            WifiClub_BufferFriendName(app, app->contactFriend);
            WifiClub_PrintMessage(app, msg_0778_00018, 0);
        }
        app->isRematch = 0;
        app->state = 27;
    } else if (WifiClub_CheckMatchLost(app) == 0) {
        u32 timer = app->timer;
        if (timer == 0) {
            if (LinkSync_IsSynced(13) != 0) {
                LinkSync_ClearPackets();
                LinkSync_StartSync(14);
                app->isRematch = 0;
                app->state = 50;
            }
        } else {
            app->timer = timer - 1;
            if (app->timer == 0) {
                LinkSync_StartSync(13);
            }
        }
    }
    return mainState;
}

s32 WifiClub_State_SendStatus(WifiClubApp *app, s32 mainState) {
    if ((WifiClub_CheckMatchLost(app) == 0) && (LinkSync_IsSynced(14) != 0)) {
        s16 r1 = WifiClub_GetPlayerStatus(app, &app->clubData->own);
        if (LinkSync_SendPacket(Comm_GetLocalNetId(), &r1) != 0) {
            app->state = 51;
        }
    }
    return mainState;
}

s32 WifiClub_State_ReceiveStatus(WifiClubApp *app, s32 mainState) {
    s32 var_r0;
    if (Comm_GetLocalNetId() == 0) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    u16 *packet = LinkSync_GetPacket(var_r0);
    if ((WifiClub_CheckMatchLost(app) == 0) && (packet != NULL)) {
        u16 ownStatus = WifiClub_GetPlayerStatus(app, &app->clubData->own);
        u16 r0 = WifiClub_GetCounterpartStatus(ownStatus);
        if ((packet[0] == r0) || (packet[0] == ownStatus)) {
            LinkSync_StartSync(15);
            app->state = 52;
        } else {
            WifiClub_BufferFriendName(app, WifiConnection_GetTargetFriendIndex());
            WifiClub_PrintMessage(app, msg_0778_00016, 0);
            app->state = 27;
        }
    }
    return mainState;
}

s32 WifiClub_State_SendPlayerInfo(WifiClubApp *app, s32 mainState) {
    if ((WifiClub_CheckMatchLost(app) == 0) && (LinkSync_IsSynced(15) != 0)) {
        CommPlayerInfo_SendLocal();
        CommState_SetErrorWatch(1, 1);
        LinkSync_StartSync(18);
        app->state = 53;
    }
    return mainState;
}

s32 WifiClub_IsMessageFinished(WifiClubApp *app) {
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        return 1;
    }
    if (WindowIsInUse(&app->msgWindow) == 0) {
        return 1;
    }
    return 0;
}

s32 WifiClub_State_StartActivity(WifiClubApp *app, s32 mainState) {
    ListMenuTemplate spC; // Unknown struct of size 0x20
    if ((WifiClub_CheckMatchLost(app) == 0) && (LinkSync_IsSynced(18) != 0) && (WifiClub_IsMessageFinished(app) == 1)) {
        Comm_RegisterLinkedFriends(app->saveData, &spC, HEAP_ID_53);
        WifiClub_CloseMessage(app);
        u32 partnerStatus = WifiClub_GetPlayerStatus(app, WifiClub_GetFriendStatusData(app, WifiConnection_GetTargetFriendIndex()));
        u32 var_r1 = partnerStatus;
        if ((var_r1 == 15) || (var_r1 == 8)) {
            var_r1 = 8;
            app->exitChoice = 7;
        } else if (partnerStatus - 18 <= 1) {
            var_r1 = 18;
            app->exitChoice = 11;
        } else if (var_r1 - 20 <= 1) {
            var_r1 = 20;
            app->exitChoice = 12;
        } else if (var_r1 - 22 <= 1) {
            var_r1 = 22;
            app->exitChoice = 13;
        } else if (var_r1 - 24 <= 1) {
            var_r1 = 24;
            app->exitChoice = 14;
        } else if (var_r1 - 26 <= 1) {
            var_r1 = 26;
            app->exitChoice = 15;
        } else if (var_r1 == 9 || var_r1 == 2) {
            var_r1 = 2;
            app->exitChoice = 1;
        } else if (var_r1 == 10 || var_r1 == 3) {
            var_r1 = 3;
            app->exitChoice = 2;
        } else if (var_r1 == 11 || var_r1 == 4) {
            var_r1 = 4;
            app->exitChoice = 3;
        } else if (var_r1 == 12 || var_r1 == 5) {
            var_r1 = 5;
            app->exitChoice = 4;
        } else if (var_r1 == 13 || var_r1 == 6) {
            var_r1 = 6;
            app->exitChoice = 5;
        } else if (var_r1 == 14 || var_r1 == 7) {
            var_r1 = 7;
            app->exitChoice = 6;
        }
        WifiClub_SetStatusAndSend(app, var_r1);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
        mainState = 2;
    }
    return mainState;
}

s32 WifiClub_State_ReloadRoom(WifiClubApp *app, s32 mainState) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    app->unk364 = 0;
    app->state = WifiClub_FadeOutToEnterRoom();
    return mainState;
}

s32 WifiClub_State_ToMessageWait(WifiClubApp *app, s32 mainState) {
    app->state = 55;
    return mainState;
}

s32 WifiClub_State_MessageWait(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    if (WifiClub_IsMatchConnected(app) != 0) {
        app->state = 56;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) || (System_GetTouchNew() == 1)) {
        app->state = 56;
    }
    return mainState;
}

s32 WifiClub_State_CloseMessage(WifiClubApp *app, s32 mainState) {
    WifiClub_CloseMessage(app);
    app->state = 19;
    return mainState;
}

s32 WifiClub_State_CreateDisconnectPrompt(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            app->yesNoPrompt = WifiClub_CreateYesNoPromptIgnoreTouch(app->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        app->state = 58;
    }
    return mainState;
}

s32 WifiClub_State_DisconnectPrompt(WifiClubApp *app, s32 mainState) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 var_r5 = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_53);
        if (TextPrinterCheckActive(app->textPrinterId) != 0) {
            return mainState;
        }
        if (WifiClub_CheckIncomingContact(app) != 0) {
            if (var_r5 == -1) {
                Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
            }
            var_r5 = -2;
        }
        if (var_r5 == -1) {
            return mainState;
        }
        if (var_r5 == 0) {
            WifiClub_PrintMessage(app, msg_0800_00026, 1);
            app->state = 59;
            app->timer = 1;
        } else {
            WifiClub_CloseMessage(app);
            app->state = 19;
            WifiClub_StopSeeking(app);
        }
    } else {
        YesNoResponse yesNoResponse = YesNoPrompt_HandleInput(app->yesNoPrompt);
        if (TextPrinterCheckActive(app->textPrinterId) != 0) {
            return mainState;
        }
        if (WifiClub_CheckIncomingContact(app) != 0) {
            yesNoResponse = YESNORESPONSE_NO;
        }
        if (yesNoResponse == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            WifiClub_PrintMessage(app, msg_0800_00026, 1);
            app->state = 59;
            app->timer = 1;
        } else if (yesNoResponse == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            WifiClub_CloseMessage(app);
            app->state = 19;
            WifiClub_StopSeeking(app);
        } else {
            return mainState;
        }
    }
    return mainState;
}

s32 WifiClub_State_Disconnecting(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    if (app->timer == 1) {
        app->timer = 0;
        CommState_EndWifiSession();
    }
    if (CommState_Exists() == 0) {
        FriendRoster_Compact(app->friendRoster);
        WifiClub_PrintMessage(app, msg_0800_00027, 1);
        app->state = 60;
        app->timer = 30;
    }
    return mainState;
}

s32 WifiClub_State_Disconnected(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    app->timer--;
    if (app->timer == 0) {
        app->exitChoice = 8;
        app->state = 34;
        WifiClub_CloseMessage(app);
    }
    return mainState;
}

s32 WifiClub_State_CreateRematchPrompt(WifiClubApp *app, s32 mainState) {
    if (LinkSync_IsSynced(16) == 0) {
        return mainState;
    }
    CommState_SetErrorWatch(0, 1);
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            app->yesNoPrompt = WifiClub_CreateYesNoPrompt(app->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        app->state = 62;
    }
    return mainState;
}

s32 WifiClub_State_RematchPrompt(WifiClubApp *app, s32 mainState) {
    if (TextPrinterCheckActive(app->textPrinterId) != 0) {
        return mainState;
    }
    if ((CommState_IsWifiJoiningLobby() != 0) || (CommState_WasWifiDisconnected() != 0) || (CommState_GetWifiMatchStatus() >= 3)) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(app->yesNoPrompt);
        }
        WifiClub_PrintMessage(app, msg_0778_00101, 0);
        app->state = 28;
    } else {
        if (CommState_HasWifiFailed() != 0) {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
            } else {
                YesNoPrompt_Destroy(app->yesNoPrompt);
            }
            WifiClub_HandleWifiError(app);
        } else {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                s32 menuResult = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_53);
                if (menuResult == -1) {
                    return mainState;
                } else if (menuResult == 0) {
                    WifiClub_PrintMessage(app, msg_0778_00138, 0);
                    GF_ASSERT(app->waitingIcon == 0);
                    app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
                    app->isRematch = 1;
                    app->state = 49;
                    app->timer = 30;
                } else {
                    WifiClub_CloseMessage(app);
                    CommPlayerInfo_Free();
                    CommState_SendWifiDisconnect();
                    app->timer = 20;
                    app->state = 65;
                }
            } else {
                s32 promptResult = YesNoPrompt_HandleInput(app->yesNoPrompt);
                if (promptResult == 1) {
                    YesNoPrompt_Destroy(app->yesNoPrompt);
                    WifiClub_PrintMessage(app, msg_0778_00138, 0);
                    GF_ASSERT(app->waitingIcon == 0);
                    app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
                    app->isRematch = 1;
                    app->state = 49;
                    app->timer = 30;
                } else if (promptResult == 2) {
                    YesNoPrompt_Destroy(app->yesNoPrompt);
                    WifiClub_CloseMessage(app);
                    CommPlayerInfo_Free();
                    CommState_SendWifiDisconnect();
                    app->timer = 20;
                    app->state = 65;
                } else {
                    return mainState;
                }
            }
            if (app->isRematch == 0) {
                app->clubData = CommState_GetWifiClubData();
                app->clubData->own.voiceChat = app->clubData->own.voiceChatSetting;
            }
        }
    }
    return mainState;
}

s32 WifiClub_State_CreateVoiceChatModePrompt(WifiClubApp *app, s32 mainState) {
    u32 partnerId = WifiClubRoom_GetTalkPartnerId(&app->room);
    if (WifiClub_FindPlayerObject(app, partnerId) == 0) {
        WifiClub_BufferFriendName(app, partnerId - 1);
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 29;
        return mainState;
    }
    WifiClubPlayerStatus *partnerStatusData = WifiClub_GetFriendStatusData(app, partnerId - 1);
    u32 r0 = WifiClub_GetPlayerStatus(app, partnerStatusData);
    u8 r7 = partnerStatusData->voiceChat;
    if ((app->partnerStatus != r0) || (app->partnerVoiceChat != r7)) {
        WifiClub_BufferFriendName(app, partnerId - 1);
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 29;
        return mainState;
    }
    if (TextPrinterCheckActive(app->textPrinterId) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &sYesNoMenuWindowTemplate, 473, 11, HEAP_ID_53);
        } else {
            app->yesNoPrompt = WifiClub_CreateYesNoPromptIgnoreTouch(app->bgConfig, &sYesNoPromptWindowTemplate, 0);
        }
        app->state = 64;
    }
    return mainState;
}

s32 WifiClub_State_VoiceChatModePrompt(WifiClubApp *app, s32 mainState) {
    s32 temp_r0 = WifiClubRoom_GetTalkPartnerId(&app->room);
    if (WifiClub_FindPlayerObject(app, temp_r0) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(app->yesNoPrompt);
        }
        WifiClub_BufferFriendName(app, temp_r0 - 1);
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 29;
        return mainState;
    }
    WifiClubPlayerStatus *partnerStatusData = WifiClub_GetFriendStatusData(app, temp_r0 - 1);
    u32 r0 = WifiClub_GetPlayerStatus(app, partnerStatusData);
    u8 r7 = partnerStatusData->voiceChat;
    if ((app->partnerStatus != r0) || (app->partnerVoiceChat != r7)) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(app->yesNoPrompt);
        }
        WifiClub_BufferFriendName(app, temp_r0 - 1);
        WifiClub_PrintMessage(app, msg_0778_00016, 0);
        app->state = 29;
        return mainState;
    }
    if (CommState_HasWifiFailed() != 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(app->yesNoPrompt);
        }
        WifiClub_HandleWifiError(app);
    } else {
        if (CommState_GetWifiMatchStatus() >= 3) {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_53);
            } else {
                YesNoPrompt_Destroy(app->yesNoPrompt);
            }
            WifiClub_PrintMessage(app, msg_0778_00018, 0);
            CommState_LeaveWifiMatch();
            app->state = 29;
            return mainState;
        } else if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            s32 menuInputResult = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_53);
            if (menuInputResult == -1) {
                return mainState;
            }
            if (menuInputResult == 0) {
                u32 temp_r0 = WifiClub_GetCounterpartStatus(app->partnerStatus);
                if (temp_r0 != 29) {
                    if (WifiClub_GetFriendOnlineState(app->partnerId - 1) == 6) {
                        WifiClub_ToggleVoiceChat(app);
                        if (WifiClub_StartMatch(app, app->partnerId - 1, temp_r0) != 0) {
                            app->matchTimer = 1800;
                            WifiClub_SetLinkModeForStatus(temp_r0);
                            Comm_SetWifiContinuousSend(0);
                            WifiClub_SetStatusAndSend(app, temp_r0);
                            WifiClub_BufferFriendName(app, app->partnerId - 1);
                            WifiClub_PrintMessage(app, msg_0778_00017, 0);
                            GF_ASSERT(app->waitingIcon == 0);
                            app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
                            if (temp_r0 != 1) {
                                app->state = 45;
                                return mainState;
                            }
                            app->matchTimer = 1800;
                            app->state = 21;
                            return mainState;
                        }
                    }
                }
            }
            WifiClub_CloseMessage(app);
            WifiClub_SetStatusAndSend(app, 16);
            WifiClub_RestoreVoiceChatSetting(app);
            WifiClub_PrintOwnStatus(app);
            app->state = 19;
            return mainState;
        }
        YesNoResponse yesNoResponse = YesNoPrompt_HandleInput(app->yesNoPrompt);
        if (yesNoResponse == 1) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            u32 temp_r0 = WifiClub_GetCounterpartStatus(app->partnerStatus);
            if (temp_r0 != 29) {
                if (WifiClub_GetFriendOnlineState(app->partnerId - 1) == 6) {
                    WifiClub_ToggleVoiceChat(app);
                    if (WifiClub_StartMatch(app, app->partnerId - 1, temp_r0) != 0) {
                        app->matchTimer = 1800;
                        WifiClub_SetLinkModeForStatus(temp_r0);
                        Comm_SetWifiContinuousSend(0);
                        WifiClub_SetStatusAndSend(app, temp_r0);
                        WifiClub_BufferFriendName(app, app->partnerId - 1);
                        WifiClub_PrintMessage(app, msg_0778_00017, 0);
                        GF_ASSERT(app->waitingIcon == 0);
                        app->waitingIcon = WaitingIcon_New(&app->msgWindow, 482);
                        if (temp_r0 != 1) {
                            app->state = 45;
                            return mainState;
                        }
                        app->matchTimer = 1800;
                        app->state = 21;
                        return mainState;
                    }
                }
            }
        } else if (yesNoResponse == 2) {
            YesNoPrompt_Destroy(app->yesNoPrompt);
            WifiClub_CloseMessage(app);
            WifiClub_SetStatusAndSend(app, 16);
            WifiClub_RestoreVoiceChatSetting(app);
            WifiClub_PrintOwnStatus(app);
            app->state = 19;
            return mainState;
        } else {
            return mainState;
        }
    }
    return mainState;
}

s32 WifiClub_State_Exit(WifiClubApp *app, s32 mainState) {
    if (CommState_Exists() == 0) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_53);
        mainState = 2;
    }
    return mainState;
}

void WifiClub_PrintMessage(WifiClubApp *app, s32 msgId, s32 isConnectionMsg) {
    u32 textSpeed_int;
    if (isConnectionMsg != 0) {
        textSpeed_int = 1;
    } else {
        Options *options = Save_PlayerData_GetOptionsAddr(app->saveData);
        textSpeed_int = Options_GetTextFrameDelay(options);
    }
    u8 textSpeed = textSpeed_int;
    WifiClub_DeleteWaitingIcon(app);
    if (WindowIsInUse(&app->noticeWindow) != 0) {
        ClearFrameAndWindow2(&app->noticeWindow, FALSE);
        RemoveWindow(&app->noticeWindow);
    }
    if (WindowIsInUse(&app->msgWindow) != 0) {
        ClearFrameAndWindow2(&app->msgWindow, FALSE);
        RemoveWindow(&app->msgWindow);
    }
    s32 printerId = app->textPrinterId;
    if ((printerId != 8) && (TextPrinterCheckActive(printerId) != 0)) {
        RemoveTextPrinter(app->textPrinterId);
        app->textPrinterId = 8;
    }
    AddWindowParameterized(app->bgConfig, &app->msgWindow, 2, 2, 19, 27, 4, 12, 301);
    if (isConnectionMsg != 0) {
        ReadMsgDataIntoString(app->connectionMsgData, msgId, app->msgRaw);
    } else {
        ReadMsgDataIntoString(app->msgData, msgId, app->msgRaw);
    }
    StringExpandPlaceholders(app->msgFormat, app->msgExpanded, app->msgRaw);
    FillWindowPixelBuffer(&app->msgWindow, 15);
    DrawFrameAndWindow2(&app->msgWindow, TRUE, 482, 10);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    app->textPrinterId = AddTextPrinterParameterized(&app->msgWindow, 1, app->msgExpanded, 0, 0, textSpeed, 0);
    ScheduleWindowCopyToVram(&app->msgWindow);
}

void WifiClub_ShowConnectionNotice(WifiClubApp *app, s32 msgId) {
    WifiClub_DeleteWaitingIcon(app);
    if (WindowIsInUse(&app->noticeWindow) != 0) {
        ClearFrameAndWindow2(&app->noticeWindow, FALSE);
        RemoveWindow(&app->noticeWindow);
    }
    if (WindowIsInUse(&app->msgWindow) != 0) {
        ClearFrameAndWindow2(&app->msgWindow, FALSE);
        RemoveWindow(&app->msgWindow);
    }
    s32 printerId = app->textPrinterId;
    if ((printerId != 8) && (TextPrinterCheckActive(printerId) != 0)) {
        RemoveTextPrinter(app->textPrinterId);
        app->textPrinterId = 8;
    }
    AddWindowParameterized(app->bgConfig, &app->noticeWindow, 2, 4, 4, 23, 16, 12, 105);
    ReadMsgDataIntoString(app->connectionMsgData, msgId, app->msgRaw);
    StringExpandPlaceholders(app->msgFormat, app->msgExpanded, app->msgRaw);
    FillWindowPixelBuffer(&app->noticeWindow, 15);
    DrawFrameAndWindow1(&app->noticeWindow, TRUE, 473, 11);
    app->textPrinterId = AddTextPrinterParameterized(&app->noticeWindow, 1, app->msgExpanded, 0, 0, TEXT_SPEED_NOTRANSFER, 0);
    ScheduleWindowCopyToVram(&app->noticeWindow);
}

void WifiClub_BufferFriendName(WifiClubApp *app, s32 friendIndex) {
    if (friendIndex != -1) {
        PlayerProfile *playerProfile = PlayerProfile_New(HEAP_ID_53);
        Save_Profile_PlayerName_Set(playerProfile, FriendRoster_GetPlayerName(app->friendRoster, friendIndex));
        BufferPlayersName(app->msgFormat, 0, playerProfile);
        Heap_Free(playerProfile);
    }
}

void WifiClub_CloseMessage(WifiClubApp *app) {
    s32 printerId;

    WifiClub_DeleteWaitingIcon(app);
    printerId = app->textPrinterId;
    if ((printerId != 8) && (TextPrinterCheckActive(printerId) != 0)) {
        RemoveTextPrinter(app->textPrinterId);
        app->textPrinterId = 8;
    }
    if (WindowIsInUse(&app->msgWindow) != 0) {
        ClearFrameAndWindow2(&app->msgWindow, FALSE);
        RemoveWindow(&app->msgWindow);
    }
}

void WifiClub_SetStatusAndSend(WifiClubApp *app, u32 status) {
    if (app->clubData != NULL) {
        WifiClub_SetStatus(app, status);
        WifiConnection_SetOwnStatusData(app->clubData, 36);
    }
}

void WifiClub_SetStatus(WifiClubApp *app, u32 status) {
    if (app->clubData != NULL) {
        u32 r0 = WifiClub_GetPlayerStatus(app, &app->clubData->own);
        if (r0 != status) {
            app->clubData->own.status = status;
            if ((WifiClub_IsInBattleStatus(status) == 0) && (status != 8) && (status != 20) && (status != 22) && (status != 24) && (status != 26) && (status != 18)) {
                if (status == 1) {
                    GF_SndStartFadeOutBGM(0, 30);
                } else if (status == 16) {
                    WifiConnection_SetVoiceChatEnabled(0);
                    if (WifiClub_IsRoomBgmPlaying() == 1) {
                        GF_SndStartFadeInBGM(120, 30, 1);
                    }
                }
            }
        }
        WifiClub_PrintOwnStatus(app);
    }
}

u8 WifiClub_ToggleVoiceChat(WifiClubApp *app) {
    app->clubData->own.voiceChat = 1 - app->clubData->own.voiceChat;
    WifiConnection_SetOwnStatusData(app->clubData, 36);
    return app->clubData->own.voiceChat;
}

u8 WifiClub_ToggleVoiceChatSetting(WifiClubApp *app) {
    app->clubData->own.voiceChatSetting = 1 - app->clubData->own.voiceChatSetting;
    app->clubData->own.voiceChat = app->clubData->own.voiceChatSetting;
    WifiConnection_SetOwnStatusData(app->clubData, 36);
    return app->clubData->own.voiceChatSetting;
}

u8 WifiClub_RestoreVoiceChatSetting(WifiClubApp *app) {
    app->clubData->own.voiceChat = app->clubData->own.voiceChatSetting;
    WifiConnection_SetOwnStatusData(app->clubData, 36);
    return app->clubData->own.voiceChatSetting;
}

void WifiClub_DeleteWaitingIcon(WifiClubApp *app) {
    if (app->waitingIcon != 0) {
        WaitingIcon_DeleteWithoutRestore();
        app->waitingIcon = 0;
        if (WindowIsInUse(&app->msgWindow) != 0) {
            ClearFrameAndWindow2(&app->msgWindow, FALSE);
            RemoveWindow(&app->msgWindow);
        }
    }
}

void WifiClub_CancelRosterSelection(WifiClubApp *app, enum HeapID heapID) {
    WifiClub_DeselectRosterFriend(app);
    WifiClubRoom_HideFriendMarker(&app->room);
    WifiClub_RedrawRoster(app, heapID);
}

void WifiClub_CancelRosterSelectionOf(WifiClubApp *app, s32 friendId, enum HeapID heapID) {
    if (friendId == WifiClub_GetSelectedRosterFriend(app)) {
        WifiClub_CancelRosterSelection(app, heapID);
    }
}

s32 WifiClub_IsRosterScreenActive(WifiClubApp *app) {
    if (app->roster.slotHitboxes != 0) {
        return 1;
    }
    return 0;
}

void WifiClub_InitRosterScreen(WifiClubApp *app, NARC *narc, enum HeapID heapID) {
    memset(&app->roster, 0, sizeof(WifiClubRoster));
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    app->roster.slotHitboxes = TouchHitboxController_Create(sRosterSlotHitboxes, 8, (TouchHitboxControllerCallback)WifiClub_RosterHitboxCallback, app, heapID);
    app->roster.msgFormat = MessageFormat_New(heapID);
    WifiClub_LoadRosterGraphics(app, narc, heapID);
    app->roster.redrawAll = 1;
    WifiClub_SetRosterPagePalette(app);
    WifiClub_DrawRosterPage(app);
}

void WifiClub_FreeRosterScreen(WifiClubApp *app) {
    if (WifiClub_IsRosterScreenActive(app) != 0) {
        WifiClub_FreeRosterGraphics(app);
        TouchHitboxController_Destroy(app->roster.slotHitboxes);
        app->roster.slotHitboxes = NULL;
        MessageFormat_Delete(app->roster.msgFormat);
        app->roster.msgFormat = NULL;
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    }
}

u8 WifiClub_UpdateRosterScreen(WifiClubApp *app, enum HeapID heapID) {
    u32 var_r5 = WifiClubRoom_GetTileAtPlayer(&app->room);
    u32 tile = WifiClubRoom_GetTileBelowPlayer(&app->room);
    if ((tile >= 3) && (tile <= 6)) {
        var_r5 = tile;
    }
    if ((var_r5 >= 3) && (var_r5 <= 6)) {
        if (app->roster.page != var_r5 - 3) {
            app->roster.page = var_r5 - 3;
            PlaySE(SEQ_SE_DP_SELECT5);
            WifiClub_SetRosterPagePalette(app);
            app->roster.redrawSlots = 1;
            app->roster.redrawAll = 1;
        }
    }
    if (app->roster.profileState == 1) {
        app->roster.profileState = 2;
        app->roster.profilePage = 0;
        WifiClub_DrawProfilePage(app, heapID);
        WifiClub_ShowProfileButtons(app);
    }
    if (app->roster.profileState == 0) {
        TouchHitboxController_IsTriggered(app->roster.slotHitboxes);
        WifiClub_AnimateRosterSelection(app);
        if (app->roster.redrawSlots == 1) {
            WifiClub_DrawRosterPage(app);
            app->roster.redrawSlots = 0;
        }
    }
    if ((u8)(app->roster.profileState + 254) <= 1) {
        u32 pressedButton = WifiClub_GetPressedProfileButton(app);
        if (WifiClub_ShouldCloseProfile(app, pressedButton) == 1) {
            app->roster.prevSelectedFriend = app->roster.selectedFriend;
            app->roster.selectedFriend = 0;
            app->roster.selectAnimStep = 0;
            app->roster.redrawSlots = 1;
            app->roster.redrawAll = 1;
            app->roster.profileState = 0;
            WifiClub_HideProfileButtons(app);
            PlaySE(SEQ_SE_DP_DECIDE);
        } else if (pressedButton != 0) {
            PlaySE(SEQ_SE_DP_DECIDE);
            WifiClub_ChangeProfilePage(app, pressedButton);
            WifiClub_DrawProfilePage(app, heapID);
        }
    }
    return app->roster.selectedFriend;
}

void WifiClub_RedrawRosterIfNeeded(WifiClubApp *app) {
    if ((app->roster.profileState == 0) && (app->roster.redrawSlots == 1)) {
        WifiClub_DrawRosterPage(app);
        app->roster.redrawSlots = 0;
    }
}

s32 WifiClub_ShouldCloseProfile(WifiClubApp *app, s32 pressedButton) {
    if (app->roster.profileState == 3) {
        return 0;
    }
    if ((gSystem.heldKeys & (PAD_KEY_DOWN | PAD_KEY_UP | PAD_KEY_LEFT | PAD_KEY_RIGHT)) || (gSystem.newKeys & (PAD_BUTTON_X | PAD_BUTTON_A | PAD_BUTTON_B)) || (pressedButton == 2)) {
        return 1;
    }
    return 0;
}

u8 WifiClub_GetSelectedRosterFriend(WifiClubApp *app) {
    return app->roster.selectedFriend;
}

void WifiClub_DeselectRosterFriend(WifiClubApp *app) {
    if (WifiClub_IsRosterScreenActive(app) == 1) {
        app->roster.selectedFriend = 0;
        app->roster.selectAnimStep = 0;
        app->roster.profileState = 0;
        app->roster.redrawSlots = 1;
        app->roster.redrawAll = 1;
        WifiClub_HideProfileButtons(app);
    }
}

void WifiClub_ChangeProfilePage(WifiClubApp *app, s32 pressedButton) {
    u32 var_r1;
    if (WifiClub_HasBattleFrontier(app) == 1) {
        var_r1 = 7;
    } else {
        var_r1 = 2;
    }
    if (pressedButton == 3) {
        app->roster.profilePage = (app->roster.profilePage + 1) % var_r1;
        return;
    }
    s8 *ptr = &app->roster.profilePage;
    ptr[0]--;
    if (app->roster.profilePage < 0) {
        ptr[0] += var_r1;
    }
}

void WifiClub_RosterHitboxCallback(s32 index, enum HeapID event, WifiClubApp *app) {
    s32 rosterIndex = app->roster.page * 8 + index;
    if ((app->roster.slotStates[rosterIndex] == 2) && (app->roster.selectedFriend == 0) && (event == 0)) {
        app->roster.selectedFriend = rosterIndex + 1;
        PlaySE(SEQ_SE_DP_DECIDE);
    }
}

void WifiClub_LoadRosterGraphics(WifiClubApp *app, NARC *narc, enum HeapID heapID) {
    s32 i;
    s32 j;
    BgClearTilemapBufferAndCommit(app->bgConfig, 4);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 17, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 128, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 20, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_4_OFFSET, 160, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 16, app->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 19, app->bgConfig, GF_BG_LYR_SUB_2, 48, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 18, app->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    app->roster.slotScreenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 21, FALSE, &app->roster.slotScreen, heapID);
    WifiClub_OffsetScreenTiles(app->roster.slotScreen, 48);

    for (i = 0; i < 7; i++) {
        app->roster.pageScreensRaw[i] = GfGfxLoader_GetScrnDataFromOpenNarc(narc, i + 22, FALSE, &app->roster.pageScreens[i], heapID);
        WifiClub_OffsetScreenTiles(app->roster.pageScreens[i], 48);
    }

    app->roster.tabCoverRaw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 29, FALSE, &app->roster.tabCoverScreen, heapID);
    WifiClub_OffsetScreenTiles(app->roster.tabCoverScreen, 48);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, heapID);

    s32 row;
    s32 r5;
    for (i = 0; i < 8; i++) {
        InitWindow(&app->roster.nameWindows[i]);
        r5 = i / 4;
        row = i % 4;

        AddWindowParameterized(app->bgConfig, &app->roster.nameWindows[i], GF_BG_LYR_SUB_1, r5 * 16 + 4, row * 6 + 1, 9, 3, 15, 1 + (i * 27));
        FillWindowPixelBuffer(&app->roster.nameWindows[i], 0);
        ScheduleWindowCopyToVram(&app->roster.nameWindows[i]);

        for (j = 0; j < 2; j++) {
            InitWindow(&app->roster.iconWindows[i][j]);
            AddWindowParameterized(app->bgConfig, &app->roster.iconWindows[i][j], GF_BG_LYR_SUB_1, r5 * 16 + 1 + j * 12, row * 6 + 1, 2, 3, 13, 6 * (j + i * 2) + 218);
            FillWindowPixelBuffer(&app->roster.iconWindows[i][j], 0);
            ScheduleWindowCopyToVram(&app->roster.iconWindows[i][j]);
        }
    }
    AddWindowParameterized(app->bgConfig, &app->roster.profileWindow, GF_BG_LYR_SUB_3, 1, 1, 30, 21, 15, 1);
    FillWindowPixelBuffer(&app->roster.profileWindow, 0);
    ScheduleWindowCopyToVram(&app->roster.profileWindow);
    WifiClub_CreateProfileButtons(app, narc, heapID);
}

void WifiClub_FreeRosterGraphics(WifiClubApp *app) {
    s32 i;
    WifiClub_DeleteProfileButtons(app);
    for (i = 0; i < 8; i++) {
        RemoveWindow(&app->roster.nameWindows[i]);
        for (s32 j = 0; j < 2; j++) {
            RemoveWindow(&app->roster.iconWindows[i][j]);
        }
    }
    RemoveWindow(&app->roster.profileWindow);
    Heap_Free(app->roster.slotScreenRaw);
    for (i = 0; i < 7; i++) {
        Heap_Free(app->roster.pageScreensRaw[i]);
    }
    Heap_Free(app->roster.tabCoverRaw);
}

void WifiClub_OffsetScreenTiles(NNSG2dScreenData *screenData, s32 tileOffset) {
    s32 i;
    s32 j;
    u16 *ip = (u16 *)&screenData->rawData[0];
    s32 width = screenData->screenWidth / 8;
    s32 height = screenData->screenHeight / 8;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            ip[i * width + j] += tileOffset;
        }
    }
}

void WifiClub_SetRosterSlotState(WifiClubApp *app, s32 friendId, u32 slotState) {
    GF_ASSERT(friendId != 0);
    GF_ASSERT(slotState != 0);
    GF_ASSERT(slotState < 3);
    app->roster.slotStates[friendId - 1] = slotState;
    app->roster.redrawSlots = 1;
}

void WifiClub_ClearRosterSlotState(WifiClubApp *app, s32 friendId) {
    GF_ASSERT(friendId != 0);
    app->roster.slotStates[friendId - 1] = 0;
    app->roster.redrawSlots = 1;
}

u8 WifiClub_GetRosterSlotState(WifiClubApp *app, s32 friendId) {
    GF_ASSERT(friendId != 0);
    return app->roster.slotStates[friendId - 1];
}

void WifiClub_RedrawRoster(WifiClubApp *app, enum HeapID heapID) {
    if (app->roster.profileState != 0) {
        WifiClub_DrawProfilePage(app, heapID);
        return;
    }
    app->roster.redrawAll = 1;
    WifiClub_DrawRosterPage(app);
}

void WifiClub_RequestRosterRedraw(WifiClubApp *app) {
    app->roster.redrawAll = 1;
    app->roster.redrawSlots = 1;
}

void WifiClub_SetRosterPagePalette(WifiClubApp *app) {
    BgTilemapRectChangePalette(app->bgConfig, 4, 0, 0, 32, 24, app->roster.page);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 4);
}

void WifiClub_DrawRosterPage(WifiClubApp *app) {
    s32 frameStyle;
    s32 redrawAll;
    if (app->roster.redrawAll == 1) {
        app->roster.redrawAll = 0;
        redrawAll = 1;
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 0, 32, 24, 0);
    } else {
        redrawAll = 0;
    }
    for (s32 i = 0; i < 8; i++) {
        s32 friendId = i + (app->roster.page * 8) + 1;
        if (app->roster.slotStates[friendId - 1] != 0) {
            if (FriendRoster_GetEntryField(app->friendRoster, friendId - 1, 8) == 0) {
                frameStyle = 1;
            } else {
                frameStyle = 0;
            }
        } else {
            frameStyle = 2;
        }
        s32 r2 = i / 4;
        s32 temp_ip = i % 4;
        u32 var_r5;
        if (friendId == app->roster.selectedFriend) {
            var_r5 = app->roster.selectAnimStep;
        } else {
            var_r5 = 0;
        }
        s32 var_r1;
        if ((friendId == app->roster.selectedFriend) || (friendId == app->roster.prevSelectedFriend)) {
            var_r1 = 1;
        } else {
            var_r1 = 0;
        }
        if (redrawAll == 1 || var_r1 == 1) {
            WifiClub_DrawRosterSlotFrame(app, r2 << 4, 6 * temp_ip, frameStyle, var_r5);
            if (frameStyle != 2) {
                WifiClub_PrintRosterSlotName(app, &app->roster.nameWindows[i], friendId, var_r5, i);
                WifiClub_DrawRosterSlotIcons(app, &app->roster.iconWindows[i][0], friendId, var_r5, i);
            } else {
                FillWindowPixelBuffer(&app->roster.nameWindows[i], 0);
                ScheduleWindowCopyToVram(&app->roster.nameWindows[i]);
                for (s32 j = 0; j < 2; j++) {
                    FillWindowPixelBuffer(&app->roster.iconWindows[i][j], 0);
                    ScheduleWindowCopyToVram(&app->roster.iconWindows[i][j]);
                }
            }
        }
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_2);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void WifiClub_DrawProfilePage(WifiClubApp *app, enum HeapID heapID) {
    s32 var_r4;
    if (FriendRoster_GetEntryField(app->friendRoster, app->roster.selectedFriend - 1, 8) == 1) {
        var_r4 = 7;
    } else {
        var_r4 = 8;
    }
    NNSG2dScreenData *pageScreen = app->roster.pageScreens[app->roster.profilePage];
    BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_2, &pageScreen->rawData[0], pageScreen->szByte);
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 32, 24, var_r4);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_3);
    if (WifiClub_HasBattleFrontier(app) == 0) {
        WifiClub_CoverFrontierPageTabs(app);
    }
    FillWindowPixelBuffer(&app->roster.profileWindow, 0);
    WifiClubProfilePageFuncs[app->roster.profilePage](app, heapID);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_2);
    ScheduleWindowCopyToVram(&app->roster.profileWindow);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
}

void WifiClub_DrawProfileSummary(WifiClubApp *app, enum HeapID heapID) {
    u32 color;
    s32 friendIndex = app->roster.selectedFriend - 1;
    if (FriendRoster_GetEntryField(app->friendRoster, friendIndex, 8) == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    WifiClub_BufferRosterFriendName(app, friendIndex);
    // {STRVAR_1 3, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00042, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 1, app->tmpString, 32, 8, TEXT_SPEED_NOTRANSFER, color, 0);
    WifiClubPlayerStatus *friendStatusData = WifiClub_GetFriendStatusData(app, friendIndex);
    u32 friendStatus = WifiClub_GetPlayerStatus(app, friendStatusData);
    ReadMsgDataIntoString(app->msgData, WifiClub_GetStatusLabel(friendStatus, &color), app->msgRaw);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->msgRaw, 104, 8, TEXT_SPEED_NOTRANSFER, color, 0);
    PlayerProfile *playerProfile = PlayerProfile_New(HEAP_ID_53);
    Save_Profile_PlayerName_Set(playerProfile, FriendRoster_GetGroupName(app->friendRoster, friendIndex));
    BufferPlayersName(app->roster.msgFormat, 0, playerProfile);
    Heap_Free(playerProfile);
    // GROUP
    ReadMsgDataIntoString(app->msgData, msg_0778_00043, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 32, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // {STRVAR_1 3, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00150, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);

    u32 width = 232 - FontID_String_GetWidth(0, app->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, 32, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // BATTLE RECORD
    ReadMsgDataIntoString(app->msgData, msg_0778_00044, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 56, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(app->roster.msgFormat, 0, FriendRoster_GetEntryField(app->friendRoster, friendIndex, 1), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    // W: {STRVAR_1 53, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00045, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 120, 56, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(app->roster.msgFormat, 0, FriendRoster_GetEntryField(app->friendRoster, friendIndex, 2), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    // L: {STRVAR_1 53, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00046, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);

    width = 232 - FontID_String_GetWidth(0, app->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, 56, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // POKéMON TRADES
    ReadMsgDataIntoString(app->msgData, msg_0778_00047, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 80, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(app->roster.msgFormat, 0, FriendRoster_GetEntryField(app->friendRoster, friendIndex, 3), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    // {STRVAR_1 53, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00048, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);

    width = 232 - FontID_String_GetWidth(0, app->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, 80, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    if (WifiClub_IsPoffinCookingAvailable(app) == 1) {
        // POFFIN SESSIONS
        ReadMsgDataIntoString(app->msgData, msg_0778_00049, app->tmpString);
        AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 104, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
        BufferIntegerAsString(app->roster.msgFormat, 0, FriendRoster_GetEntryField(app->friendRoster, friendIndex, 9), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        // {STRVAR_1 53, 0, 0}
        ReadMsgDataIntoString(app->msgData, msg_0778_00050, app->msgRaw);
        StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);

        width = 232 - FontID_String_GetWidth(0, app->tmpString, 0);
        AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, 104, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    }
    // DATE LAST PLAYED
    ReadMsgDataIntoString(app->msgData, msg_0778_00051, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 128, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    s32 temp_r0_2 = FriendRoster_GetEntryField(app->friendRoster, friendIndex, 6);
    if (temp_r0_2 != 0) {
        BufferIntegerAsString(app->roster.msgFormat, 2, temp_r0_2, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferIntegerAsString(app->roster.msgFormat, 0, FriendRoster_GetEntryField(app->friendRoster, friendIndex, 4), 4, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferMonthNameAbbr(app->roster.msgFormat, 1, FriendRoster_GetEntryField(app->friendRoster, friendIndex, 5));
        // {STRVAR_34 16, 1, 0} {STRVAR_1 51, 2, 0}, {STRVAR_1 53, 0, 0}
        ReadMsgDataIntoString(app->msgData, msg_0778_00074, app->msgRaw);
        StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);

        width = 232 - FontID_String_GetWidth(0, app->tmpString, 0);
        AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, 128, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    }
    WifiClub_DrawStatusIconToBg(app->bgConfig, &app->statusIcons, 6, 2, 2, friendStatus);

    s32 var_r1;
    if (friendStatusData->voiceChat != 0) {
        var_r1 = 8;
    } else {
        var_r1 = 1;
    }
    WifiClub_DrawIconToBg(app->bgConfig, &app->statusIcons, 6, 28, 2, var_r1, 0);
}

void WifiClub_DrawProfileBattleTower(WifiClubApp *app, enum HeapID heapID) {
    s32 friendIndex = app->roster.selectedFriend - 1;
    // BATTLE TOWER
    ReadMsgDataIntoString(app->msgData, msg_0778_00052, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), 0);
    WifiClub_GetFrontierStreakLabel(app, app->tmpString, 0, friendIndex);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00065, 113, friendIndex, 120, 24);
    // RECORD
    ReadMsgDataIntoString(app->msgData, msg_0778_00059, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00065, 112, friendIndex, 120, 48);
}

void WifiClub_DrawProfileBattleFactory(WifiClubApp *app, enum HeapID heapID) {
    s32 friendIndex = app->roster.selectedFriend - 1;
    // BATTLE FACTORY
    ReadMsgDataIntoString(app->msgData, msg_0778_00053, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), 0);
    // LEVEL 50
    ReadMsgDataIntoString(app->msgData, msg_0778_00060, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // RENTAL/TRADE
    ReadMsgDataIntoString(app->msgData, msg_0778_00062, app->tmpString);

    u32 width = 232 - FontID_String_GetWidth(0, app->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_GetFrontierStreakLabel(app, app->tmpString, 1, friendIndex);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00065, 115, friendIndex, 104, 48);
    WifiClub_PrintFrontierStat(app, msg_0778_00066, 117, friendIndex, 212, 48);
    // RECORD
    ReadMsgDataIntoString(app->msgData, msg_0778_00059, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 64, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00065, 114, friendIndex, 104, 64);
    WifiClub_PrintFrontierStat(app, msg_0778_00066, 116, friendIndex, 212, 64);
    // OPEN LEVEL
    ReadMsgDataIntoString(app->msgData, msg_0778_00061, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 88, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // RENTAL/TRADE
    ReadMsgDataIntoString(app->msgData, msg_0778_00062, app->tmpString);

    width = 232 - FontID_String_GetWidth(0, app->tmpString, 0);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, 88, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_GetFrontierStreakLabel(app, app->tmpString, 2, friendIndex);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 112, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00065, 119, friendIndex, 104, 112);
    WifiClub_PrintFrontierStat(app, msg_0778_00066, 121, friendIndex, 212, 112);
    // RECORD
    ReadMsgDataIntoString(app->msgData, msg_0778_00059, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 128, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00065, 118, friendIndex, 104, 128);
    WifiClub_PrintFrontierStat(app, msg_0778_00066, 120, friendIndex, 212, 128);
}

void WifiClub_DrawProfileBattleCastle(WifiClubApp *app, enum HeapID heapID) {
    s32 friendIndex = app->roster.selectedFriend - 1;
    // BATTLE CASTLE
    ReadMsgDataIntoString(app->msgData, msg_0778_00054, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), 0);
    // CASTLE POINTS
    ReadMsgDataIntoString(app->msgData, msg_0778_00063, app->tmpString);

    u32 width = 180 - (FontID_String_GetWidth(0, app->tmpString, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_GetFrontierStreakLabel(app, app->tmpString, 3, friendIndex);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00068, 135, friendIndex, 104, 48);
    WifiClub_PrintFrontierStat(app, msg_0778_00067, 136, friendIndex, 212, 48);
    // RECORD
    ReadMsgDataIntoString(app->msgData, msg_0778_00059, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 80, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00068, 134, friendIndex, 104, 80);
    WifiClub_PrintFrontierStat(app, msg_0778_00067, 138, friendIndex, 212, 80);
}

void WifiClub_DrawProfileBattleHall(WifiClubApp *app, enum HeapID heapID) {
    FrontierSave *frontierSave = Save_Frontier_GetStatic(app->saveData);
    s32 friendIndex = app->roster.selectedFriend - 1;
    // BATTLE HALL
    ReadMsgDataIntoString(app->msgData, msg_0778_00055, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), 0);
    String *speciesName = GetSpeciesName(FrontierSave_GetStat(frontierSave, 124, friendIndex), heapID);
    u32 width = 33 - (FontID_String_GetWidth(0, speciesName, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, speciesName, width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    String_Delete(speciesName);
    WifiClub_GetFrontierStreakLabel(app, app->tmpString, 4, friendIndex);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00065, 123, friendIndex, 108, 48);
    // RECORD
    ReadMsgDataIntoString(app->msgData, msg_0778_00059, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 80, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00065, 122, friendIndex, 108, 80);
}

void WifiClub_DrawProfileBattleArcade(WifiClubApp *app, enum HeapID heapID) {
    s32 friendIndex = app->roster.selectedFriend - 1;
    // BATTLE ARCADE
    ReadMsgDataIntoString(app->msgData, msg_0778_00056, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), 0);
    WifiClub_GetFrontierStreakLabel(app, app->tmpString, 5, friendIndex);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00069, 143, friendIndex, 112, 24);
    // RECORD
    ReadMsgDataIntoString(app->msgData, msg_0778_00059, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    WifiClub_PrintFrontierStat(app, msg_0778_00069, 142, friendIndex, 112, 48);
}

void WifiClub_DrawProfilePlazaGames(WifiClubApp *app, enum HeapID heapID) {
    FriendRoster *friendRoster = Save_FriendRoster_Get(app->saveData);
    s32 friendIndex = app->roster.selectedFriend - 1;
    // PLAZA GAMES PLAYED
    ReadMsgDataIntoString(app->msgData, msg_0778_00070, app->tmpString);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), 0);
    BufferWiFiPlazaActivityName(app->roster.msgFormat, 0, 0);
    // {STRVAR_3 0, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00071, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // {STRVAR_1 53, 0, 0}
    WifiClub_PrintCenteredNumber(app, msg_0778_00072, FriendRoster_GetEntryField(friendRoster, friendIndex, 10), 164, 24);
    BufferWiFiPlazaActivityName(app->roster.msgFormat, 0, 1);
    // {STRVAR_3 0, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00071, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // {STRVAR_1 53, 0, 0}
    WifiClub_PrintCenteredNumber(app, msg_0778_00072, FriendRoster_GetEntryField(friendRoster, friendIndex, 11), 164, 48);
    BufferWiFiPlazaActivityName(app->roster.msgFormat, 0, 2);
    // {STRVAR_3 0, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00071, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, 8, 72, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    // {STRVAR_1 53, 0, 0}
    WifiClub_PrintCenteredNumber(app, msg_0778_00072, FriendRoster_GetEntryField(friendRoster, friendIndex, 12), 164, 72);
}

void WifiClub_CoverFrontierPageTabs(WifiClubApp *app) {
    for (s32 i = 0; i < 5; i++) {
        CopyToBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_2, i + 26, 0, 1, 1, &app->roster.tabCoverScreen->rawData[0], 0, 0, app->roster.tabCoverScreen->screenWidth / 8, app->roster.tabCoverScreen->screenHeight / 8);
    }
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_2, 26, 0, 5, 1, 6);
}

void WifiClub_PrintFrontierStat(WifiClubApp *app, s32 msgId, s32 stat, s32 friendIndex, s32 x, s32 y) {
    WifiClub_PrintCenteredNumber(app, msgId, FrontierSave_GetStat(Save_Frontier_GetStatic(app->saveData), stat, friendIndex), x, y);
}

void WifiClub_GetFrontierStreakLabel(WifiClubApp *app, String *dest, u32 facility, s32 friendIndex) {

    GF_ASSERT(facility < 6);
    s32 var_r1;
    if (FrontierSave_GetStat(Save_Frontier_GetStatic(app->saveData), sFrontierStreakFlagStats[facility], friendIndex) == 0) {
        // PREV
        var_r1 = msg_0778_00057;
    } else {
        // CURRENT
        var_r1 = msg_0778_00058;
    }
    ReadMsgDataIntoString(app->msgData, var_r1, dest);
}

void WifiClub_PrintCenteredNumber(WifiClubApp *app, s32 msgId, s32 value, s32 centerX, s32 y) {
    BufferIntegerAsString(app->roster.msgFormat, 0, value, 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ReadMsgDataIntoString(app->msgData, msgId, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);
    u32 width = centerX - (FontID_String_GetWidth(0, app->tmpString, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&app->roster.profileWindow, 0, app->tmpString, width, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
}

void WifiClub_DrawRosterSlotFrame(WifiClubApp *app, u8 x, u8 y, u8 frameStyle, u8 animStep) {
    GF_ASSERT(animStep < 4);
    if (animStep == 3) {
        animStep = 1;
    }
    if (animStep < 2) {
        CopyToBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_2, x, y, 16, 6, &app->roster.slotScreen->rawData, (u16)(animStep << 4), (u16)(frameStyle * 6), 32, 18);
    } else {
        s64 offset = 18 << 4;
        CopyToBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_2, x, y, 16, 6, &app->roster.slotScreen->rawData + offset, 0, (u16)(frameStyle * 6), 16, 18);
    }
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_2, x, y, 16, 6, frameStyle + 4);
}

s32 WifiClub_GetStatusLabel(u32 status, u32 *color) {
    *color = MAKE_TEXT_COLOR(1, 2, 0);
    s32 ret = 0;
    switch (status) {
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

void WifiClub_PrintRosterSlotName(WifiClubApp *app, Window *window, s32 friendId, s32 animStep, s32 slot) {
    FillWindowPixelBuffer(window, 0);
    u8 textY = sRosterSlotTextY[animStep];
    s32 friendIndex = friendId - 1;
    s32 color;
    if (FriendRoster_GetEntryField(app->friendRoster, friendIndex, 8) == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    WifiClub_BufferRosterFriendName(app, friendIndex);
    // {STRVAR_1 3, 0, 0}
    ReadMsgDataIntoString(app->msgData, msg_0778_00042, app->msgRaw);
    StringExpandPlaceholders(app->roster.msgFormat, app->tmpString, app->msgRaw);
    AddTextPrinterParameterizedWithColor(window, 1, app->tmpString, 0, textY, TEXT_SPEED_NOTRANSFER, color, 0);
    ScheduleWindowCopyToVram(window);
}

void WifiClub_DrawRosterSlotIcons(WifiClubApp *app, Window *windows, s32 friendId, u32 animStep, u32 slot) {
    s32 var_r0;
    WifiClubPlayerStatus *friendStatusData = WifiClub_GetFriendStatusData(app, friendId - 1);
    u32 friendStatus = WifiClub_GetPlayerStatus(app, friendStatusData);
    u32 textY = sRosterSlotTextY[animStep];
    for (s32 i = 0; i < 2; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
        if (i == 0) {
            WifiClub_DrawStatusIconToWindow(&windows[i], &app->statusIcons, 0, textY, friendStatus);
        } else {
            if (friendStatusData->voiceChat != 0) {
                var_r0 = 8;
            } else {
                var_r0 = 1;
            }
            WifiClub_DrawIconToWindow(&windows[i], &app->statusIcons, 0, textY, var_r0, 0);
        }
        ScheduleWindowCopyToVram(&windows[i]);
    }
}

void WifiClub_AnimateRosterSelection(WifiClubApp *app) {
    if (app->roster.selectedFriend != 0) {
        app->roster.selectAnimTimer++;
        if (app->roster.selectAnimTimer >= sRosterSelectAnimDurations[app->roster.selectAnimStep]) {
            app->roster.selectAnimTimer = 0;
            app->roster.selectAnimStep++;
            app->roster.redrawSlots = 1;
            if (app->roster.selectAnimStep >= 4) {
                app->roster.selectAnimTimer = 0;
                app->roster.selectAnimStep = 0;
                app->roster.profileState = 1;
            }
        }
    }
}

void WifiClub_CreateProfileButtons(WifiClubApp *app, NARC *narc, enum HeapID heapID) {
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
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    app->roster.buttonResources[0] = AddCharResObjFromOpenNarc(app->gfxResMan[0], narc, NARC_a_0_4_3, FALSE, 30, NNS_G2D_VRAM_TYPE_2DSUB, heapID);
    app->roster.buttonResources[1] = AddPlttResObjFromOpenNarc(app->gfxResMan[1], narc, NARC_a_0_4_4, FALSE, 30, NNS_G2D_VRAM_TYPE_2DSUB, 8, heapID);
    app->roster.buttonResources[2] = AddCellOrAnimResObjFromOpenNarc(app->gfxResMan[2], narc, NARC_fielddata_mapmatrix_map_matrix, FALSE, 30, GF_GFX_RES_TYPE_CELL, heapID);
    app->roster.buttonResources[3] = AddCellOrAnimResObjFromOpenNarc(app->gfxResMan[3], narc, NARC_a_0_4_2, FALSE, 30, GF_GFX_RES_TYPE_ANIM, heapID);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(app->roster.buttonResources[0]) != 0);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(app->roster.buttonResources[1]) != 0);
    GF2DGfxResObj_FreeData(app->roster.buttonResources[0]);
    GF2DGfxResObj_FreeData(app->roster.buttonResources[1]);

    SpriteResourcesHeader spriteResourcesHeader;
    CreateSpriteResourcesHeader(&spriteResourcesHeader, 30, 30, 30, 30, -1, -1, 0, 0, app->gfxResMan[0], app->gfxResMan[1], app->gfxResMan[2], app->gfxResMan[3], 0, 0);
    for (s32 i = 0; i < 3; i++) {
        spriteTemplateList[i].spriteList = app->spriteList;
        spriteTemplateList[i].header = &spriteResourcesHeader;
        spriteTemplateList[i].heapID = heapID;
        app->roster.buttonSprites[i] = Sprite_CreateAffine(&spriteTemplateList[i]);
        Sprite_SetAnimCtrlSeq(app->roster.buttonSprites[i], sProfileButtonIdleAnims[i]);
    }
    FontID_Alloc(2, heapID);
    Window window;
    InitWindow(&window);
    AddTextWindowTopLeftCorner(app->bgConfig, &window, 8, 2, 0, 0);
    String *string = String_New(64, heapID);
    // CLOSE
    ReadMsgDataIntoString(app->msgData, msg_0778_00073, string);
    AddTextPrinterParameterizedWithColor(&window, 2, string, FontID_String_GetCenterAlignmentX(2, string, 0, 64), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    String_Delete(string);
    TextOBJLayout *textLayout = TextOBJLayout_New(&window, heapID);
    GF_ASSERT(ObjCharTransfer_ReserveCharSpace(TextOBJLayout_GetCharSize(textLayout, NNS_G2D_VRAM_TYPE_2DSUB), TRUE, NNS_G2D_VRAM_TYPE_2DSUB, &app->roster.closeTextCharSpace) != 0);

    TextOBJTemplate textObjTemplate;
    textObjTemplate.fontSystem = app->fontSystem;
    textObjTemplate.window = &window;
    textObjTemplate.spriteList = app->spriteList;
    textObjTemplate.plttResourceProxy = SpriteTransfer_GetPaletteProxy(app->roster.buttonResources[1], NULL);
    textObjTemplate.sprite = app->roster.buttonSprites[1];
    textObjTemplate.offset = app->roster.closeTextCharSpace.offset;
    textObjTemplate.x = -32;
    textObjTemplate.y = -8;
    textObjTemplate.unk_20 = 0;
    textObjTemplate.unk_24 = 0;
    textObjTemplate.vram = 2;
    textObjTemplate.heapID = heapID;
    app->roster.closeText = TextOBJ_Create(&textObjTemplate, textLayout);
    TextOBJLayout_Delete(textLayout);
    RemoveWindow(&window);
    FontID_Release(2);
    app->roster.buttonHitboxes = TouchHitboxController_Create(sProfileButtonHitboxes, 3, (TouchHitboxControllerCallback)WifiClub_ProfileButtonHitboxCallback, app, heapID);
}

void WifiClub_DeleteProfileButtons(WifiClubApp *app) {
    TouchHitboxController_Destroy(app->roster.buttonHitboxes);
    TextOBJ_Destroy(app->roster.closeText);
    ObjCharTransfer_FreeCharSpace(&app->roster.closeTextCharSpace);

    for (s32 i = 0; i < 3; i++) {
        Sprite_Delete(app->roster.buttonSprites[i]);
    }
    SpriteTransfer_DeleteCharTransferTask(app->roster.buttonResources[0]);
    SpriteTransfer_DeletePlttTransferTask(app->roster.buttonResources[1]);

    for (s32 i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(app->gfxResMan[i], app->roster.buttonResources[i]);
    }
}

void WifiClub_ShowProfileButtons(WifiClubApp *app) {
    app->roster.buttonsShown = 1;
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    app->roster.touchedButton = 0;
    for (s32 i = 0; i < 3; i++) {
        WifiClub_ResetProfileButton(&app->roster, i);
    }
}

void WifiClub_HideProfileButtons(WifiClubApp *app) {
    if (app->roster.buttonsShown == 2) {
        Sprite_SetDrawFlag(app->roster.buttonSprites[1], TRUE);
        TextOBJ_SetSpritesDrawFlag(app->roster.closeText, TRUE);
    }
    app->roster.buttonsShown = 0;
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
}

s32 WifiClub_GetPressedProfileButton(WifiClubApp *app) {
    if (app->roster.buttonsShown == 0) {
        return 0;
    }
    app->roster.touchedButton = 3;
    app->roster.touchState = 255;
    TouchHitboxController_IsTriggered(app->roster.buttonHitboxes);

    for (s32 i = 0; i < 3; i++) {
        if ((app->roster.buttonsShown != 2 || i != 1) && WifiClub_UpdateProfileButton(&app->roster, i, app->roster.touchedButton, app->roster.touchState) == 1) {
            return i + 1;
        }
    }
    return 0;
}

void WifiClub_ProfileButtonHitboxCallback(u32 index, u32 event, WifiClubApp *app) {
    app->roster.touchedButton = index;
    app->roster.touchState = event;
}

void WifiClub_ResetProfileButton(WifiClubRoster *roster, s32 button) {
    Sprite_SetAnimCtrlSeq(roster->buttonSprites[button], sProfileButtonIdleAnims[button]);
    Sprite_SetAnimationFrame(roster->buttonSprites[button], 4);
    if (button == 1) {
        TextOBJ_SetPosition(roster->closeText, -32, -8);
    }
}

s32 WifiClub_UpdateProfileButton(WifiClubRoster *roster, s32 button, s32 touchedButton, s32 touchState) {
    s32 pressed = 0;
    if (touchedButton == button) {
        u32 animFrame = Sprite_GetAnimationFrame(roster->buttonSprites[button]);
        if (touchState == 0 || touchState == 2) {
            if (touchState == 0) {
                Sprite_SetAnimCtrlSeq(roster->buttonSprites[button], sProfileButtonPressAnims[button]);
            }
            if (animFrame < 3) {
                Sprite_UpdateAnim(roster->buttonSprites[button], 2 * FX32_ONE);
                u16 newFrame = Sprite_GetAnimationFrame(roster->buttonSprites[button]);
                if (button == 1) {
                    TextOBJ_SetPosition(roster->closeText, -32, sCloseTextYOffsets[newFrame]);
                }
                if (newFrame >= 3) {
                    pressed = 1;
                }
            }
        } else if (touchState == 1 && animFrame < 3) {
            pressed = 1;
        }
    } else {
        u16 animSeq = Sprite_GetAnimationNumber(roster->buttonSprites[button]);
        u16 curFrame = Sprite_GetAnimationFrame(roster->buttonSprites[button]);
        if (animSeq == sProfileButtonPressAnims[button]) {
            Sprite_SetAnimCtrlSeq(roster->buttonSprites[button], sProfileButtonIdleAnims[button]);
            Sprite_SetAnimationFrame(roster->buttonSprites[button], 4 - curFrame);
        }
        Sprite_UpdateAnim(roster->buttonSprites[button], 2 * FX32_ONE);
    }
    return pressed;
}

void WifiClub_BufferRosterFriendName(WifiClubApp *app, s32 friendIndex) {
    if (friendIndex != -1) {
        PlayerProfile *playerProfile = PlayerProfile_New(HEAP_ID_53);
        Save_Profile_PlayerName_Set(playerProfile, FriendRoster_GetPlayerName(app->friendRoster, friendIndex));
        BufferPlayersName(app->roster.msgFormat, 0, playerProfile);
        Heap_Free(playerProfile);
    }
}

WifiClubPlayerStatus *WifiClub_GetFriendStatusData(WifiClubApp *app, u32 friendIndex) {
    if (friendIndex >= 32) {
        return &app->hiddenFriendStatus;
    }
    return &app->clubData->friends[friendIndex];
}

s32 WifiClub_GetFriendOnlineState(s32 friendIndex) {
    return WifiConnection_GetFriendStatus(friendIndex);
}

s32 WifiClub_StartMatch(WifiClubApp *app, s32 friendIndex, s32 status) {
    LinkSync_Init(HEAP_ID_15);
    WifiConnection_SetVoiceChatEnabled(app->clubData->own.voiceChat);
    if (WifiClub_IsGroupActivityStatus(status) == 1) {
        WifiConnection_SetExtraHeapEnabled(1, 128);
    } else {
        WifiConnection_SetExtraHeapEnabled(0, 128);
    }
    WifiClubComm_RegisterCommandsNoContext();
    return CommState_StartWifiMatch(friendIndex);
}

void WifiClub_StartSeeking(WifiClubApp *app, s32 showMessage) {
    if (app->isSeeking == 0) {
        app->isSeeking = 1;
        WifiClubRoom_SetInputLocked(&app->room, 1);
        if (showMessage == 1) {
            WifiClub_PrintMessage(app, msg_0778_00009, 0);
        }
    }
}

void WifiClub_StopSeeking(WifiClubApp *app) {
    if (app->isSeeking == 1) {
        app->isSeeking = 0;
        WifiClub_CloseMessage(app);
        WifiClubRoom_SetInputLocked(&app->room, 0);
    }
}
