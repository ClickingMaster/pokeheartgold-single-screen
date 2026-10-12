#include "wifi_plaza_room.h"

#include <string.h>

#include "global.h"

#include "comm_state.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "render_text.h"
#include "sound_playback.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "unk_0200FA24.h"
#include "swap_3d_buffers.h"
#include "wifi_plaza_event.h"
#include "wireless_status_icon.h"

// unk_02037C94.c (no header declares it)
BOOL CommState_HasPlazaLobbyFailed(void);

// rodata of this overlay (asm/wifi_plaza_map_rodata.s)
extern const u8 sWiFiPlazaListArrowY[2];
extern const u32 sWiFiPlazaRoomMsgBanks[4];
extern const GraphicsModes sWiFiPlazaRoomGraphicsModes;
extern const u32 sWiFiPlazaRoomBgIds[4];
extern const ObjCharTransferTemplate sWiFiPlazaRoomCharTransfer;
extern const GXRgb sWiFiPlazaRoomEdgeColors[8];
extern const ListMenuTemplate sWiFiPlazaYesNoTemplate;
extern const GraphicsBanks sWiFiPlazaRoomBanks;
extern const BgTemplate sWiFiPlazaRoomBgTemplates[4];

BOOL WiFiPlazaObj_UpdateLight(WiFiPlazaObjLight *light, void *avatars) {
    if (light->active == 0) {
        return TRUE;
    }
    light->frame++;
    if (light->frame >= light->duration) {
        light->active = 0;
    }
    light->value = light->frame * light->delta / light->duration;
    light->value += light->start;
    ov45_02230700(avatars, light->value);
    return FALSE;
}

BOOL WiFiPlazaRoom_Init(OverlayManager *manager, int *state) {
    WiFiPlazaRoomWork *room;
    WiFiPlazaRoomArgs *args = OverlayManager_GetArgs(manager);

    Heap_Create(HEAP_ID_3, (enum HeapID)0x77, 0x3A000);
    Heap_Create(HEAP_ID_3, (enum HeapID)0x78, 0x3D000);
    room = OverlayManager_CreateAndGetData(manager, sizeof(WiFiPlazaRoomWork), (enum HeapID)0x77);
    memset(room, 0, sizeof(WiFiPlazaRoomWork));
    room->startParams = args->startParams;
    room->lobby = args->lobby;
    room->ownPlayerIndex = ov45_0222A53C(room->lobby);
    if (ov45_0222A53C(room->lobby) == -1) {
        GF_AssertFail();
    }
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    room->profile = Save_PlayerData_GetProfile(args->save);
    WiFiPlazaRoom_InitGraphics(&room->graphics, args->save, (enum HeapID)0x77);
    room->map = WiFiPlazaMap_Create((enum HeapID)0x77);
    room->objManager = WiFiPlazaObj_CreateManager(0x18, PlayerProfile_GetTrainerGender(room->profile), room->map, (enum HeapID)0x77, 0x78);
    room->models = WiFiPlazaModels_Create(args->group, args->variant, room->map, (enum HeapID)0x77, (enum HeapID)0x78);
    room->camera = WiFiPlazaCamera_Create((enum HeapID)0x77);
    WiFiPlazaMsgWindow_Init(&room->msgWindow, &room->graphics, args->save, (enum HeapID)0x77);
    WiFiPlazaSignWindow_Init(&room->signWindow, &room->graphics, args->save, (enum HeapID)0x77);
    WiFiPlazaListMenu_Init(&room->listMenu, &room->graphics, (enum HeapID)0x77);
    WiFiPlazaErrorWindow_Init(&room->errorWindow, &room->graphics, (enum HeapID)0x77);
    WiFiPlazaTextWindow_Init(&room->textWindow, &room->graphics, (enum HeapID)0x77);
    WiFiPlazaRoomMsg_Init(&room->msg, (enum HeapID)0x77);
    WiFiPlazaYesNoMenu_Init(&room->yesNoMenu, &room->msg, (enum HeapID)0x77);
    WiFiPlazaInfoPanel_Init(&room->infoPanel, room->startParams, &room->graphics, room->profile, (enum HeapID)0x77);
    args = (WiFiPlazaRoomArgs *)WiFiPlazaWork_GetMyPlayer(room->lobby);
    if (((BOOL(*)(struct WiFiPlazaPlayer *))ov45_0222AADC)((struct WiFiPlazaPlayer *)args) == TRUE) {
        WiFiPlazaInfoPanel_ShowOwnCard(&room->infoPanel, &room->graphics, &room->msg, (enum HeapID)0x77, (struct WiFiPlazaPlayer *)args);
    }
    room->playerStates = WiFiPlazaPlayerState_CreateManager(room, (enum HeapID)0x77);
    WiFiPlazaPlayerState_InitOwn(room);
    WiFiPlazaPlayerState_InitOthers(room);
    room->instruments = WiFiPlazaInstrument_Create(room->lobby, room->map, room->camera, room->objManager, (enum HeapID)0x77, (enum HeapID)0x78);
    room->event = WiFiPlazaEvent_Create((enum HeapID)0x77, room);
    Main_SetVBlankIntrCB(WiFiPlazaRoom_VBlank, room);
    HBlankInterruptDisable();
    return TRUE;
}

BOOL WiFiPlazaRoom_Main(OverlayManager *manager, int *state) {
    WiFiPlazaRoomWork *room = OverlayManager_GetData(manager);
    u32 result;

    OverlayManager_GetArgs(manager);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, (enum HeapID)0x77);
        room->fading = TRUE;
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            room->fading = FALSE;
            (*state)++;
        }
        break;
    case 2:
        if (room->mode == 1) {
            if (ov45_0222D844() == TRUE || ov45_0222A1FC(room->lobby) != 0) {
                room->error = 1;
            }
        }
        if (room->ignorePlazaClosing == FALSE && ov45_0222A33C(room->lobby) == TRUE) {
            room->plazaClosing = TRUE;
            WiFiPlazaRoom_SetResult(room, 8);
        }
        if (room->finished == TRUE || room->error == 1 || room->plazaClosing == TRUE) {
            if (room->error != 0) {
                *state = 3;
            } else if (room->plazaClosing == TRUE) {
                *state = 5;
            } else {
                *state = 7;
            }
        }
        break;
    case 3:
        WiFiPlazaMsgWindow_Close(&room->msgWindow);
        WiFiPlazaListMenu_Close(&room->listMenu, NULL, NULL);
        WiFiPlazaTextWindow_Close(&room->textWindow);
        WiFiPlazaYesNoMenu_Free(&room->yesNoMenu);
        if (CommState_HasWifiFailed() != FALSE) {
            WiFiPlazaErrorWindow_PrintCommError(&room->errorWindow, &room->msg);
        } else if (CommState_HasPlazaLobbyFailed() != 0) {
            WiFiPlazaErrorWindow_PrintLobbyError(&room->errorWindow, &room->msg, ov45_0222E7CC());
        } else {
            WiFiPlazaErrorWindow_PrintDisconnected(&room->errorWindow, &room->msg, ov45_0222A1FC(room->lobby));
        }
        *state = 4;
        break;
    case 4:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            *state = 7;
        }
        break;
    case 5:
        WiFiPlazaMsgWindow_Close(&room->msgWindow);
        WiFiPlazaListMenu_Close(&room->listMenu, NULL, NULL);
        WiFiPlazaTextWindow_Close(&room->textWindow);
        WiFiPlazaYesNoMenu_Free(&room->yesNoMenu);
        WiFiPlazaMsgWindow_Print(&room->msgWindow, WiFiPlazaRoomMsg_GetString(&room->msg, 1, 0x46));
        *state = 6;
        room->closingTimer = 60;
        break;
    case 6:
        if (WiFiPlazaMsgWindow_IsPrintFinished(&room->msgWindow) != FALSE) {
            room->closingTimer--;
            if (room->closingTimer <= 0) {
                *state = 7;
            }
        }
        break;
    case 7:
        result = WiFiPlazaInfoPanel_GetState(&room->infoPanel);
        if (result == 5) {
            BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, (enum HeapID)0x77);
            room->fading = TRUE;
            (*state)++;
        } else if (result == 0) {
            BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, (enum HeapID)0x77);
            room->fading = TRUE;
            (*state)++;
        }
        break;
    case 8:
        if (IsPaletteFadeFinished() == TRUE) {
            room->fading = FALSE;
            return TRUE;
        }
        break;
    }
    WiFiPlazaRoom_Update(room);
    WiFiPlazaRoom_Draw(room);
    return FALSE;
}

BOOL WiFiPlazaRoom_Exit(OverlayManager *manager, int *state) {
    WiFiPlazaRoomWork *room = OverlayManager_GetData(manager);
    WiFiPlazaRoomArgs *args = OverlayManager_GetArgs(manager);

    args->result = room->result;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    WiFiPlazaEvent_Delete(room->event);
    WiFiPlazaInstrument_Delete(room->instruments);
    WiFiPlazaPlayerState_DeleteManager(room->playerStates);
    WiFiPlazaInfoPanel_Free(&room->infoPanel, &room->signWindow, &room->graphics);
    WiFiPlazaRoomMsg_Free(&room->msg);
    WiFiPlazaMsgWindow_Free(&room->msgWindow);
    WiFiPlazaSignWindow_Free(&room->signWindow);
    WiFiPlazaListMenu_Free(&room->listMenu, &room->graphics);
    WiFiPlazaErrorWindow_Free(&room->errorWindow);
    WiFiPlazaYesNoMenu_Free(&room->yesNoMenu);
    WiFiPlazaTextWindow_Free(&room->textWindow);
    WiFiPlazaCamera_Delete(room->camera);
    WiFiPlazaModels_Delete(room->models);
    WiFiPlazaObj_DeleteManager(room->objManager);
    WiFiPlazaMap_Delete(room->map);
    WiFiPlazaRoom_FreeGraphics(&room->graphics);
    Heap_Free(room);
    Heap_Destroy((enum HeapID)0x77);
    Heap_Destroy((enum HeapID)0x78);
    StopAllSE(0);
    StopCry(0);
    return TRUE;
}

WiFiPlazaWork *WiFiPlazaRoom_GetLobby(WiFiPlazaRoomWork *room) {
    return room->lobby;
}

void *WiFiPlazaRoom_GetStartParams(WiFiPlazaRoomWork *room) {
    return room->startParams;
}

WiFiPlazaObjManager *WiFiPlazaRoom_GetObjManager(WiFiPlazaRoomWork *room) {
    return room->objManager;
}

WiFiPlazaModels *WiFiPlazaRoom_GetModels(WiFiPlazaRoomWork *room) {
    return room->models;
}

WiFiPlazaMap *WiFiPlazaRoom_GetMap(WiFiPlazaRoomWork *room) {
    return room->map;
}

WiFiPlazaCamera *WiFiPlazaRoom_GetCamera(WiFiPlazaRoomWork *room) {
    return room->camera;
}

WiFiPlazaPlayerStateManager *WiFiPlazaRoom_GetPlayerStates(WiFiPlazaRoomWork *room) {
    return room->playerStates;
}

void WiFiPlazaRoom_SetMode(WiFiPlazaRoomWork *room, u32 mode) {
    room->mode = mode;
}

u8 WiFiPlazaRoom_GetOwnPlayerIndex(WiFiPlazaRoomWork *room) {
    return room->ownPlayerIndex;
}

u8 WiFiPlazaRoom_IsFading(WiFiPlazaRoomWork *room) {
    return room->fading;
}

void WiFiPlazaRoom_SetFinished(WiFiPlazaRoomWork *room, u8 finished) {
    room->finished = finished;
}

void WiFiPlazaRoom_SetResult(WiFiPlazaRoomWork *room, u8 result) {
    room->result = result;
}

void WiFiPlazaRoom_SetActionTile(WiFiPlazaRoomWork *room, u8 value) {
    room->actionTile = value;
}

u8 WiFiPlazaRoom_GetActionTile(WiFiPlazaRoomWork *room) {
    return room->actionTile;
}

void WiFiPlazaRoom_IgnorePlazaClosing(WiFiPlazaRoomWork *room) {
    room->ignorePlazaClosing = TRUE;
}

void WiFiPlazaRoom_SetPlayerBusy(WiFiPlazaRoomWork *room, u8 index, u8 value) {
    if (index >= 20) {
        GF_AssertFail();
    }
    room->playerBusy[index] = value;
}

u8 WiFiPlazaRoom_IsPlayerBusy(WiFiPlazaRoomWork *room, u32 index) {
    return room->playerBusy[index];
}

#pragma force_active on

void WiFiPlazaRoom_SetCardBlocked(WiFiPlazaRoomWork *room, u8 index, u8 value) {
    GF_ASSERT(index < 20);
    room->cardBlocked[index] = value;
}

#pragma force_active reset

u8 WiFiPlazaRoom_IsCardBlocked(WiFiPlazaRoomWork *room, u32 index) {
    return room->cardBlocked[index];
}

void WiFiPlazaRoom_PrintMessage(WiFiPlazaRoomWork *room, String *message) {
    WiFiPlazaMsgWindow_Print(&room->msgWindow, message);
}

void WiFiPlazaRoom_PrintMessageInstant(WiFiPlazaRoomWork *room, String *message) {
    WiFiPlazaMsgWindow_PrintInstant(&room->msgWindow, message);
}

BOOL WiFiPlazaRoom_IsMessageFinished(WiFiPlazaRoomWork *room) {
    return WiFiPlazaMsgWindow_IsPrintFinished(&room->msgWindow);
}

void WiFiPlazaRoom_ShowWaitingIcon(WiFiPlazaRoomWork *room) {
    WiFiPlazaMsgWindow_ShowWaitingIcon(&room->msgWindow);
}

void WiFiPlazaRoom_HideWaitingIcon(WiFiPlazaRoomWork *room) {
    WiFiPlazaMsgWindow_HideWaitingIcon(&room->msgWindow);
}

BOOL WiFiPlazaRoom_IsWaitingIconShown(WiFiPlazaRoomWork *room) {
    return WiFiPlazaMsgWindow_IsWaitingIconShown(&room->msgWindow);
}

void WiFiPlazaRoom_CloseMessage(WiFiPlazaRoomWork *room) {
    WiFiPlazaMsgWindow_Close(&room->msgWindow);
}

void WiFiPlazaRoom_PrintSign(WiFiPlazaRoomWork *room, String *message) {
    WiFiPlazaSignWindow_Print(&room->signWindow, message);
}

void WiFiPlazaRoom_CreateListItems(WiFiPlazaRoomWork *room, u32 count) {
    WiFiPlazaListMenu_CreateItems(&room->listMenu, count, (enum HeapID)0x78, 0);
}

void WiFiPlazaRoom_CreateListItemsWithValue(WiFiPlazaRoomWork *room, u32 count, int value) {
    WiFiPlazaListMenu_CreateItems(&room->listMenu, count, (enum HeapID)0x78, value);
}

void WiFiPlazaRoom_DeleteListItems(WiFiPlazaRoomWork *room) {
    WiFiPlazaListMenu_DeleteItems(&room->listMenu);
}

void WiFiPlazaRoom_AddListItem(WiFiPlazaRoomWork *room, String *text, int value) {
    WiFiPlazaListMenu_AddItem(&room->listMenu, text, value);
}

ListMenuItem *WiFiPlazaRoom_GetListItems(WiFiPlazaRoomWork *room) {
    return WiFiPlazaListMenu_GetItems(&room->listMenu);
}

BOOL WiFiPlazaRoom_HasListItemValue(WiFiPlazaRoomWork *room, int value) {
    return WiFiPlazaListMenu_HasItemValue(&room->listMenu, value);
}

void WiFiPlazaRoom_OpenList(WiFiPlazaRoomWork *room, const ListMenuTemplate *template, u16 cursorPos, u16 itemsAbove) {
    WiFiPlazaListMenu_Open(&room->listMenu, template, &room->graphics, cursorPos, itemsAbove, (enum HeapID)0x77, 16, 3, 15);
}

void WiFiPlazaRoom_OpenListAt(WiFiPlazaRoomWork *room, const ListMenuTemplate *template, u16 cursorPos, u16 itemsAbove, u8 x, u8 y, u8 width) {
    WiFiPlazaListMenu_Open(&room->listMenu, template, &room->graphics, cursorPos, itemsAbove, (enum HeapID)0x77, x, y, width);
}

u32 WiFiPlazaRoom_ProcessList(WiFiPlazaRoomWork *room) {
    return WiFiPlazaListMenu_ProcessInput(&room->listMenu);
}

void WiFiPlazaRoom_CloseList(WiFiPlazaRoomWork *room, u16 *cursorPos, u16 *itemsAbove) {
    WiFiPlazaListMenu_Close(&room->listMenu, cursorPos, itemsAbove);
}

void WiFiPlazaRoom_SetListArrows(WiFiPlazaRoomWork *room, BOOL show) {
    WiFiPlazaListMenu_SetShowArrows(&room->listMenu, show);
}

void WiFiPlazaRoom_OpenTextWindow(WiFiPlazaRoomWork *room, u8 x, u8 y, u8 width, u8 height) {
    WiFiPlazaTextWindow_Open(&room->textWindow, &room->graphics, (enum HeapID)0x77, x, y, width, height);
}

void WiFiPlazaRoom_CloseTextWindow(WiFiPlazaRoomWork *room) {
    WiFiPlazaTextWindow_Close(&room->textWindow);
}

void WiFiPlazaRoom_PrintTextWindow(WiFiPlazaRoomWork *room, String *string, u32 x, u32 y) {
    WiFiPlazaTextWindow_Print(&room->textWindow, string, x, y);
}

void WiFiPlazaRoom_FillTextWindowRect(WiFiPlazaRoomWork *room, u16 x, u16 y, u16 width, u16 height) {
    WiFiPlazaTextWindow_FillRect(&room->textWindow, x, y, width, height);
}

void WiFiPlazaRoom_OpenYesNoMenu(WiFiPlazaRoomWork *room) {
    WiFiPlazaListMenu_Open(&room->listMenu, &room->yesNoMenu.template, &room->graphics, 0, 0, (enum HeapID)0x77, 25, 13, 6);
}

void WiFiPlazaRoom_OpenYesNoMenuDefaultNo(WiFiPlazaRoomWork *room) {
    WiFiPlazaListMenu_Open(&room->listMenu, &room->yesNoMenu.template, &room->graphics, 0, 1, (enum HeapID)0x77, 25, 13, 6);
}

u32 WiFiPlazaRoom_GetYesNoResult(WiFiPlazaRoomWork *room) {
    s32 input = WiFiPlazaListMenu_ProcessInput(&room->listMenu);
    if (input == 0) {
        goto yes;
    }
    if (input == 1) {
        goto no;
    }
    if (input != LIST_CANCEL) {
        goto open;
    }
    PlaySE(SEQ_SE_DP_SELECT);
no:
    return 1;
yes:
    return 0;
open:
    return 2;
}

void WiFiPlazaRoom_CloseYesNoMenu(WiFiPlazaRoomWork *room) {
    WiFiPlazaListMenu_Close(&room->listMenu, NULL, NULL);
}

String *WiFiPlazaRoom_GetMsgString(WiFiPlazaRoomWork *room, int bank, int msgId) {
    return WiFiPlazaRoomMsg_GetString(&room->msg, bank, msgId);
}

void WiFiPlazaRoom_BufferInt(WiFiPlazaRoomWork *room, int value, int digits, int field, int style) {
    WiFiPlazaRoomMsg_BufferInt(&room->msg, value, digits, field, style);
}

void WiFiPlazaRoom_BufferPlayerName(WiFiPlazaRoomWork *room, u32 playerIndex, u32 field) {
    PlayerProfile *profile = PlayerProfile_New((enum HeapID)0x77);
    struct WiFiPlazaPlayer *player;
    if (playerIndex == ov45_0222A53C(room->lobby)) {
        player = WiFiPlazaWork_GetMyPlayer(room->lobby);
    } else {
        player = WiFiPlazaWork_GetPlayer(room->lobby, playerIndex);
    }
    WiFiPlazaPlayer_CopyToProfile(player, profile, (enum HeapID)0x77);
    WiFiPlazaRoomMsg_BufferPlayerName(&room->msg, profile, field);
    Heap_Free(profile);
}

void WiFiPlazaRoom_BufferActivityName(WiFiPlazaRoomWork *room, u32 activity, u32 field) {
    WiFiPlazaRoomMsg_BufferActivityName(&room->msg, activity, field);
}

void WiFiPlazaRoom_BufferEventName(WiFiPlazaRoomWork *room, u32 event, u32 field) {
    WiFiPlazaRoomMsg_BufferEventName(&room->msg, event, field);
}

void WiFiPlazaRoom_BufferInstrumentName(WiFiPlazaRoomWork *room, u32 instrument, u32 field) {
    WiFiPlazaRoomMsg_BufferInstrumentName(&room->msg, instrument, field);
}

void WiFiPlazaRoom_BufferJPGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay) {
    BufferJPGreeting(room->msg.format, field, timeOfDay);
}

void WiFiPlazaRoom_BufferENGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay) {
    BufferENGreeting(room->msg.format, field, timeOfDay);
}

void WiFiPlazaRoom_BufferFRGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay) {
    BufferFRGreeting(room->msg.format, field, timeOfDay);
}

void WiFiPlazaRoom_BufferITGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay) {
    BufferITGreeting(room->msg.format, field, timeOfDay);
}

void WiFiPlazaRoom_BufferDEGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay) {
    BufferDEGreeting(room->msg.format, field, timeOfDay);
}

void WiFiPlazaRoom_BufferSPGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay) {
    BufferSPGreeting(room->msg.format, field, timeOfDay);
}

void WiFiPlazaRoom_BufferTypeName(WiFiPlazaRoomWork *room, u32 field, u32 index) {
    BufferTypeName(room->msg.format, field, ov45_0222D7C0(index));
}

void WiFiPlazaRoom_ShowPlayerCard(WiFiPlazaRoomWork *room, u32 playerIndex, u32 arg) {
    if (playerIndex != WiFiPlazaInfoPanel_GetCardPlayer(&room->infoPanel) || WiFiPlazaInfoPanel_IsPlayerCardShown(&room->infoPanel) != TRUE || WiFiPlazaInfoPanel_HasRequest(&room->infoPanel) != FALSE || arg != WiFiPlazaInfoPanel_CardShowsMessage(&room->infoPanel)) {
        WiFiPlazaInfoPanel_RequestPlayerCard(&room->infoPanel, playerIndex, arg);
    }
}

void WiFiPlazaRoom_SetPlayerCardIcon(WiFiPlazaRoomWork *room, u32 icon) {
    WiFiPlazaInfoPanel_SetCardIcon(&room->infoPanel, &room->graphics, icon, (enum HeapID)0x77);
}

void WiFiPlazaRoom_RefreshPlayerCardTypes(WiFiPlazaRoomWork *room) {
    if (WiFiPlazaInfoPanel_IsPlayerCardShown(&room->infoPanel) != FALSE) {
        WiFiPlazaPlayerCard_LoadTypeIcons(&room->infoPanel, &room->infoPanel.card, &room->graphics, (enum HeapID)0x77, WiFiPlazaWork_GetPlayer(room->lobby, WiFiPlazaInfoPanel_GetCardPlayer(&room->infoPanel)));
    }
}

void WiFiPlazaRoom_ShowOwnCard(WiFiPlazaRoomWork *room) {
    WiFiPlazaInfoPanel_RequestOwnCard(&room->infoPanel);
}

u32 WiFiPlazaRoom_GetPlayerCardPlayer(WiFiPlazaRoomWork *room) {
    return WiFiPlazaInfoPanel_GetCardPlayer(&room->infoPanel);
}

BOOL WiFiPlazaRoom_IsPlayerCardShown(WiFiPlazaRoomWork *room) {
    return WiFiPlazaInfoPanel_IsPlayerCardShown(&room->infoPanel);
}

BOOL WiFiPlazaRoom_IsPlayerCardIdle(WiFiPlazaRoomWork *room) {
    return WiFiPlazaInfoPanel_IsPlayerCardIdle(&room->infoPanel);
}

void WiFiPlazaRoom_ShowInstrumentPanel(WiFiPlazaRoomWork *room) {
    WiFiPlazaInfoPanel_RequestInstrument(&room->infoPanel);
}

BOOL ov49_0225A520(WiFiPlazaRoomWork *room, u32 index) {
    return ov49_02268968(room->event, index, 1);
}

void WiFiPlazaRoom_StartInfoPanel(WiFiPlazaRoomWork *room) {
    room->infoPanel.started = TRUE;
}

void WiFiPlazaRoom_SetPanelButtonDisabled(WiFiPlazaRoomWork *room, u16 disabled) {
    WiFiPlazaInfoPanel_SetButtonDisabled(&room->infoPanel, disabled);
}

void WiFiPlazaRoom_ResetPanelButton(WiFiPlazaRoomWork *room) {
    WiFiPlazaPanelButton_ResetTimer(&room->infoPanel.button);
}

BOOL WiFiPlazaRoom_TakePanelButtonPress(WiFiPlazaRoomWork *room) {
    BOOL pressed = room->panelButtonPressed;
    room->panelButtonPressed = FALSE;
    return pressed;
}

void WiFiPlazaRoom_StartInstrument(WiFiPlazaRoomWork *room, u32 playerIndex) {
    u32 instrument;
    if (playerIndex >= 20) {
        GF_AssertFail();
    }
    instrument = ov45_0222AAC8(WiFiPlazaWork_GetPlayer(room->lobby, playerIndex));
    WiFiPlazaInstrument_Start(room->instruments, playerIndex, instrument);
    if (playerIndex == ov45_0222A53C(room->lobby)) {
        ov45_0222AD70(room->lobby, instrument);
    }
}

BOOL WiFiPlazaRoom_IsInstrumentFinished(WiFiPlazaRoomWork *room, u32 playerIndex) {
    if (playerIndex >= 20) {
        GF_AssertFail();
    }
    return WiFiPlazaInstrument_IsFinished(room->instruments, playerIndex);
}

void WiFiPlazaRoom_VBlank(void *data) {
    WiFiPlazaRoomWork *room = data;
    WiFiPlazaModels_VBlank(room->models);
    ov49_02258BE0(room->objManager);
    WiFiPlazaRoom_GraphicsVBlank(&room->graphics);
}

void WiFiPlazaRoom_InitGraphics(WiFiPlazaRoomGraphics *graphics, SaveData *save, enum HeapID heapID) {
    s32 i;
    s32 j;
    u8 frame;

    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GF_CreateVramTransferManager(32, heapID);
    GfGfx_SetBanks(&sWiFiPlazaRoomBanks);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    SetBothScreensModesAndDisable(&sWiFiPlazaRoomGraphicsModes);
    graphics->bgConfig = BgConfig_Alloc(heapID);

    for (i = 0; i < 4; i++) {
        InitBgFromTemplate(graphics->bgConfig, (u8)sWiFiPlazaRoomBgIds[i], &sWiFiPlazaRoomBgTemplates[i], GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange((u8)sWiFiPlazaRoomBgIds[i], 32, 0, heapID);
        BgClearTilemapBufferAndCommit(graphics->bgConfig, (u8)sWiFiPlazaRoomBgIds[i]);
    }

    frame = (u8)Options_GetFrame(Save_PlayerData_GetOptionsAddr(save));
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_5_OFFSET, heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_4_OFFSET, heapID);
    LoadUserFrameGfx1(graphics->bgConfig, GF_BG_LYR_MAIN_1, 0x55, 3, 0, heapID);
    LoadUserFrameGfx2(graphics->bgConfig, GF_BG_LYR_MAIN_1, 1, 1, frame, heapID);
    LoadMapSignpostFrameAndGraphic(graphics->bgConfig, GF_BG_LYR_MAIN_1, 0x1F, 2, 3, 0, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_2_0_7, 0x5A, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, 0x20, heapID);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, heapID);
    ObjCharTransfer_InitEx((ObjCharTransferTemplate *)&sWiFiPlazaRoomCharTransfer, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_64K);
    ObjPlttTransfer_Init(24, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    WirelessStatusIcon_Create();
    graphics->spriteList = G2dRenderer_Init(24, &graphics->renderer, heapID);
    G2dRenderer_SetSubSurfaceCoords(&graphics->renderer, 0, FX32_CONST(256));
    for (j = 0; j < 4; j++) {
        graphics->resManagers[j] = Create2DGfxResObjMan(24, (GfGfxResType)j, heapID);
    }
    graphics->cellTransfer = CreateCellTransferStateManager(24, heapID);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    graphics->vramManager = GF_3DVramMan_Create(heapID, GF_3D_TEXALLOC_LNK, 2, GF_3D_PLTTALLOC_LNK, 4, WiFiPlazaRoom_Setup3D);
}

void WiFiPlazaRoom_DrawSprites(WiFiPlazaRoomGraphics *graphics) {
    SpriteList_RenderAndAnimateSprites(graphics->spriteList);
    thunk_UpdateCellTransferStateManager();
}

void WiFiPlazaRoom_FreeGraphics(WiFiPlazaRoomGraphics *graphics) {
    int i;
    int j;
    const u32 *bgIds;
    u32 offset;

    GF_DestroyVramTransferManager();
    bgIds = sWiFiPlazaRoomBgIds;
    for (i = 0; i < 4; i++, bgIds++) {
        FreeBgTilemapBuffer(graphics->bgConfig, (u8)*bgIds);
    }
    Heap_Free(graphics->bgConfig);
    DestroyCellTransferStateManager(graphics->cellTransfer);
    // cellTransfer = NULL, then resManagers[0-3] (the offset is kept in a register)
    offset = offsetof(WiFiPlazaRoomGraphics, cellTransfer);
    j = 0;
    *(void **)((u8 *)graphics + offset) = NULL;
    bgIds = (const u32 *)graphics;
    offset -= sizeof(graphics->resManagers);
    do {
        Destroy2DGfxResObjMan(*(GF_2DGfxResMan **)((u8 *)bgIds + offset));
        j++;
        bgIds++;
    } while (j < 4);
    SpriteList_Delete(graphics->spriteList);
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
    GF_3DVramMan_Delete(graphics->vramManager);
}

void WiFiPlazaRoom_GraphicsVBlank(WiFiPlazaRoomGraphics *graphics) {
    DoScheduledBgGpuUpdates(graphics->bgConfig);
    OamManager_ApplyAndResetBuffers();
    GF_RunVramTransferTasks();
}

void WiFiPlazaRoom_Setup3D(void) {
    VecFx16 vec;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sWiFiPlazaRoomEdgeColors);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(26, 26, 26), 0, 0x7FFF, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    NNS_G3dGlbLightVector(GX_LIGHTID_0, -2043, -3548, 110);

    vec.x = 0;
    vec.y = 0;
    vec.z = FX16_ONE;
    VEC_Fx16Normalize(&vec, &vec);
    NNS_G3dGlbLightVector(GX_LIGHTID_1, vec.x, vec.y, vec.z);
    vec.x = 0;
    vec.y = 0;
    vec.z = FX16_ONE;
    VEC_Fx16Normalize(&vec, &vec);
    NNS_G3dGlbLightVector(GX_LIGHTID_2, vec.x, vec.y, vec.z);
    NNS_G3dGlbLightColor(GX_LIGHTID_2, GX_RGB(31, 31, 31));
    vec.x = 0;
    vec.y = 0;
    vec.z = FX16_ONE;
    VEC_Fx16Normalize(&vec, &vec);
    NNS_G3dGlbLightVector(GX_LIGHTID_3, vec.x, vec.y, vec.z);
    NNS_G3dGlbLightColor(GX_LIGHTID_3, GX_RGB(31, 31, 31));
}

void WiFiPlazaRoom_Update(WiFiPlazaRoomWork *room) {
    u32 error;
    ov49_02258B44(room->objManager);
    error = room->error;
    if (error == 0 && room->plazaClosing == FALSE) {
        WiFiPlazaPlayerState_Update(room->playerStates);
    }
    WiFiPlazaModels_Update(room->models);
    error = room->error;
    if (error == 0 && room->plazaClosing == FALSE) {
        WiFiPlazaObj_UpdateManager(room->objManager);
        WiFiPlazaPlayerState_LateUpdate(room->playerStates);
        WiFiPlazaEvent_Update(room->event);
    }
    WiFiPlazaRoom_UpdateMatchSigns(room);
    if (room->fading == FALSE) {
        u32 paused;
        error = room->error;
        if (error == 1 || room->plazaClosing == TRUE || room->finished == TRUE) {
            paused = TRUE;
        } else {
            paused = FALSE;
        }
        WiFiPlazaInfoPanel_Update(&room->infoPanel, room, paused, (enum HeapID)0x77);
    }
    WiFiPlazaInstrument_Update(room->instruments);
}

void WiFiPlazaRoom_Draw(WiFiPlazaRoomWork *room) {
    Thunk_G3X_Reset();
    WiFiPlazaCamera_Update(room->camera);
    WiFiPlazaModels_Draw(room->models);
    ov49_02258BD4(room->objManager);
    WiFiPlazaInstrument_Draw(room->instruments);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    WiFiPlazaRoom_DrawSprites(&room->graphics);
}

// The signs of the three Pokemon match ups (ov45_0222F274: match state, ov45_0222F294: closed for entry)
void WiFiPlazaRoom_UpdateMatchSigns(WiFiPlazaRoomWork *room) {
    int i;
    u32 state;
    u32 recruiting;
    u32 stop = ov45_0222A394(room->lobby);
    for (i = 0; i < 3; i++) {
        state = ov45_0222F274(i);
        if (state == TRUE) {
            if (ov45_0222F294(i) == FALSE) {
                recruiting = TRUE;
            } else {
                recruiting = FALSE;
            }
        } else {
            recruiting = FALSE;
        }
        WiFiPlazaModels_UpdateMatchSign(room->models, i, ov45_0222F314(i), state, recruiting, stop);
    }
}

void WiFiPlazaMsgWindow_Init(WiFiPlazaMsgWindow *window, WiFiPlazaRoomGraphics *graphics, SaveData *save, enum HeapID heapID) {
    AddWindowParameterized(graphics->bgConfig, &window->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 4, 0x5E);
    FillWindowPixelBuffer(&window->window, 15);
    window->string = String_New(0x180, heapID);
    window->textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(save));
}

void WiFiPlazaMsgWindow_Free(WiFiPlazaMsgWindow *window) {
    if (TextPrinterCheckActive((u8)window->printerId)) {
        RemoveTextPrinter((u8)window->printerId);
    }
    WiFiPlazaMsgWindow_DeleteWaitingIcon(window);
    String_Delete(window->string);
    RemoveWindow(&window->window);
}

void WiFiPlazaMsgWindow_Print(WiFiPlazaMsgWindow *window, String *string) {
    if (TextPrinterCheckActive((u8)window->printerId)) {
        RemoveTextPrinter((u8)window->printerId);
    }
    FillWindowPixelBuffer(&window->window, 15);
    String_Copy(window->string, string);
    window->printerId = AddTextPrinterParameterizedWithColor(&window->window, 1, window->string, 0, 0, window->textSpeed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    DrawFrameAndWindow2(&window->window, TRUE, 1, 1);
}

void WiFiPlazaMsgWindow_PrintInstant(WiFiPlazaMsgWindow *window, String *string) {
    if (TextPrinterCheckActive((u8)window->printerId)) {
        RemoveTextPrinter((u8)window->printerId);
    }
    FillWindowPixelBuffer(&window->window, 15);
    String_Copy(window->string, string);
    AddTextPrinterParameterizedWithColor(&window->window, 1, window->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    DrawFrameAndWindow2(&window->window, TRUE, 1, 1);
    ScheduleWindowCopyToVram(&window->window);
}

void WiFiPlazaMsgWindow_ShowWaitingIcon(WiFiPlazaMsgWindow *window) {
    if (window->waitingIcon != NULL) {
        GF_AssertFail();
    }
    window->waitingIcon = WaitingIcon_New(&window->window, 1);
}

void WiFiPlazaMsgWindow_HideWaitingIcon(WiFiPlazaMsgWindow *window) {
    WaitingIcon *icon = window->waitingIcon;
    if (icon != NULL) {
        WaitingIcon_Delete(icon);
        window->waitingIcon = NULL;
    }
}

void WiFiPlazaMsgWindow_DeleteWaitingIcon(WiFiPlazaMsgWindow *window) {
    WaitingIcon *icon = window->waitingIcon;
    if (icon != NULL) {
        // render_window.h declares it without its argument
        ((void (*)(WaitingIcon *))WaitingIcon_DeleteWithoutRestore)(icon);
        window->waitingIcon = NULL;
    }
}

BOOL WiFiPlazaMsgWindow_IsWaitingIconShown(WiFiPlazaMsgWindow *window) {
    return window->waitingIcon != NULL;
}

BOOL WiFiPlazaMsgWindow_IsPrintFinished(WiFiPlazaMsgWindow *window) {
    if (TextPrinterCheckActive((u8)window->printerId) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaMsgWindow_Close(WiFiPlazaMsgWindow *window) {
    if (TextPrinterCheckActive((u8)window->printerId)) {
        RemoveTextPrinter((u8)window->printerId);
    }
    WiFiPlazaMsgWindow_DeleteWaitingIcon(window);
    ClearFrameAndWindow2(&window->window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&window->window);
}

void WiFiPlazaSignWindow_Init(WiFiPlazaMsgWindow *window, WiFiPlazaRoomGraphics *graphics, SaveData *save, enum HeapID heapID) {
    WiFiPlazaMsgWindow_Init(window, graphics, save, heapID);
    SetWindowPaletteNum(&window->window, 2);
}

void WiFiPlazaSignWindow_Free(WiFiPlazaMsgWindow *window) {
    WiFiPlazaMsgWindow_Free(window);
}

void WiFiPlazaSignWindow_Print(WiFiPlazaMsgWindow *window, String *string) {
    if (TextPrinterCheckActive((u8)window->printerId)) {
        RemoveTextPrinter((u8)window->printerId);
    }
    FillWindowPixelBuffer(&window->window, 15);
    String_Copy(window->string, string);
    window->printerId = AddTextPrinterParameterized(&window->window, 1, window->string, 0, 0, window->textSpeed, NULL);
    DrawFrameAndWindow3(&window->window, TRUE, 0x1F, 2, 3);
}

#pragma force_active on

void WiFiPlazaListMenu_Init(WiFiPlazaListMenu *menu, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template;
    int i;
    NARC *narc = NARC_New(NARC_a_0_6_0, heapID);

    menu->arrowResources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_CHAR], narc, 4, FALSE, 5000, NNS_G2D_VRAM_TYPE_2DMAIN, heapID);
    menu->arrowResources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_PLTT], narc, 10, FALSE, 5000, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapID);
    menu->arrowResources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_CELL], narc, 5, FALSE, 5000, GF_GFX_RES_TYPE_CELL, heapID);
    menu->arrowResources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_ANIM], narc, 6, FALSE, 5000, GF_GFX_RES_TYPE_ANIM, heapID);
    NARC_Delete(narc);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(menu->arrowResources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(menu->arrowResources[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&header, 5000, 5000, 5000, 5000, -1, -1, FALSE, 0, graphics->resManagers[GF_GFX_RES_TYPE_CHAR], graphics->resManagers[GF_GFX_RES_TYPE_PLTT], graphics->resManagers[GF_GFX_RES_TYPE_CELL], graphics->resManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = graphics->spriteList;
    template.header = &header;
    template.priority = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = heapID;
    template.position.x = FX32_CONST(192);
    for (i = 0; i < 2; i++) {
        template.position.y = sWiFiPlazaListArrowY[i] << FX32_SHIFT;
        menu->arrows[i] = Sprite_Create(&template);
        Sprite_SetAnimCtrlSeq(menu->arrows[i], i);
        Sprite_SetAnimActiveFlag(menu->arrows[i], TRUE);
        Sprite_SetDrawFlag(menu->arrows[i], FALSE);
    }
    menu->showArrows = FALSE;
}

#pragma force_active reset

void WiFiPlazaListMenu_Free(WiFiPlazaListMenu *menu, WiFiPlazaRoomGraphics *graphics) {
    int i;
    if (menu->items) {
        WiFiPlazaListMenu_DeleteItems(menu);
    }
    if (menu->listMenu) {
        WiFiPlazaListMenu_Close(menu, NULL, NULL);
    }
    for (i = 0; i < 2; i++) {
        Sprite_Delete(menu->arrows[i]);
        menu->arrows[i] = NULL;
    }
    SpriteTransfer_DeleteCharTransferTask(menu->arrowResources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(menu->arrowResources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(graphics->resManagers[i], menu->arrowResources[i]);
    }
}

void WiFiPlazaListMenu_CreateItems(WiFiPlazaListMenu *menu, u32 count, enum HeapID heapID, int value) {
    u32 i;
    u32 offset;
    GF_ASSERT(menu->items == NULL);
    menu->items = ListMenuItems_New(count, heapID);
    i = 0;
    menu->itemCount = count;
    if (i < count) {
        offset = i;
        do {
            ((int *)((u8 *)menu->items + offset))[1] = value;
            i++;
            offset += sizeof(ListMenuItem);
        } while (i < count);
    }
}

void WiFiPlazaListMenu_DeleteItems(WiFiPlazaListMenu *menu) {
    ListMenuItem *items = menu->items;
    if (items != NULL) {
        ListMenuItems_Delete(items);
        menu->items = NULL;
        menu->itemCount = 0;
    }
}

#pragma force_active on

void WiFiPlazaListMenu_AddItem(WiFiPlazaListMenu *menu, String *text, int value) {
    ListMenuItems_AddItem(menu->items, text, value);
}

#pragma force_active reset

ListMenuItem *WiFiPlazaListMenu_GetItems(WiFiPlazaListMenu *menu) {
    return menu->items;
}

BOOL WiFiPlazaListMenu_HasItemValue(WiFiPlazaListMenu *menu, int value) {
    int i;
    ListMenuItem *item;
    i = 0;
    if (i < menu->itemCount) {
        item = menu->items;
        do {
            if (value == item->value) {
                return TRUE;
            }
            i++;
            item++;
        } while (i < *(volatile u16 *)&menu->itemCount);
    }
    return FALSE;
}

void WiFiPlazaListMenu_Open(WiFiPlazaListMenu *menu, const ListMenuTemplate *template, WiFiPlazaRoomGraphics *graphics, u16 cursorPos, u16 itemsAbove, enum HeapID heapID, u8 x, u8 y, u8 width) {
    if (menu->listMenu != NULL) {
        GF_AssertFail();
    }
    if (template->maxShowed * 2 >= 18) {
        GF_AssertFail();
    }
    menu->template = *template;
    menu->template.window = &menu->window;
    menu->totalItems = template->totalItems;
    menu->template.moveCursorFunc = WiFiPlazaListMenu_OnCursorMove;
    AddWindowParameterized(graphics->bgConfig, &menu->window, GF_BG_LYR_MAIN_1, x, y, width, template->maxShowed * 2, 5, 0xCA);
    FillWindowPixelBuffer(&menu->window, 15);
    DrawFrameAndWindow1(&menu->window, TRUE, 0x55, 3);
    menu->listMenu = ListMenuInit(&menu->template, cursorPos, itemsAbove, (enum HeapID)(u8)heapID);
    ScheduleWindowCopyToVram(&menu->window);
}

s32 WiFiPlazaListMenu_ProcessInput(WiFiPlazaListMenu *menu) {
    s32 input;
    if (menu->listMenu == NULL) {
        return LIST_CANCEL;
    }
    input = ListMenu_ProcessInput(menu->listMenu);
    switch (input) {
    case LIST_CANCEL:
    case LIST_NOTHING_CHOSEN:
        WiFiPlazaListMenu_UpdateArrows(menu);
        break;
    default:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
    return input;
}

void WiFiPlazaListMenu_Close(WiFiPlazaListMenu *menu, u16 *cursorPos, u16 *itemsAbove) {
    int i;
    if (menu->listMenu != NULL) {
        DestroyListMenu(menu->listMenu, cursorPos, itemsAbove);
        menu->listMenu = NULL;
        EraseFrameAndWindow1(&menu->window, TRUE);
        ClearWindowTilemapAndScheduleTransfer(&menu->window);
        RemoveWindow(&menu->window);
        menu->showArrows = FALSE;
        for (i = 0; i < 2; i++) {
            Sprite_SetDrawFlag(menu->arrows[i], FALSE);
        }
    }
}

void WiFiPlazaListMenu_OnCursorMove(ListMenu *listMenu, s32 index, u8 onInit) {
    if (onInit == FALSE) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

void WiFiPlazaListMenu_SetShowArrows(WiFiPlazaListMenu *menu, BOOL show) {
    menu->showArrows = show;
}

#pragma force_active on

void WiFiPlazaListMenu_UpdateArrows(WiFiPlazaListMenu *menu) {
    u16 scroll;
    if (menu->showArrows == FALSE) {
        Sprite_SetDrawFlag(menu->arrows[0], FALSE);
        Sprite_SetDrawFlag(menu->arrows[1], FALSE);
        return;
    }
    ListMenuGetScrollAndRow(menu->listMenu, &scroll, NULL);
    if (scroll == 0) {
        Sprite_SetDrawFlag(menu->arrows[0], FALSE);
    } else {
        Sprite_SetDrawFlag(menu->arrows[0], TRUE);
    }
    if (scroll >= menu->totalItems - 7) {
        Sprite_SetDrawFlag(menu->arrows[1], FALSE);
    } else {
        Sprite_SetDrawFlag(menu->arrows[1], TRUE);
    }
}

#pragma force_active reset

void WiFiPlazaTextWindow_Init(Window *window, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID) {
}

void WiFiPlazaTextWindow_Free(Window *window) {
    WiFiPlazaTextWindow_Close(window);
}

void WiFiPlazaTextWindow_Open(Window *window, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID, u8 x, u8 y, u8 width, u8 height) {
    AddWindowParameterized(graphics->bgConfig, window, GF_BG_LYR_MAIN_1, x, y, width, height, 5, 0xCA);
    DrawFrameAndWindow1(window, TRUE, 0x55, 3);
    FillWindowPixelBuffer(window, 15);
    ScheduleWindowCopyToVram(window);
}

void WiFiPlazaTextWindow_Close(Window *window) {
    if (WindowIsInUse(window) == TRUE) {
        EraseFrameAndWindow1(window, TRUE);
        ClearWindowTilemapAndScheduleTransfer(window);
        RemoveWindow(window);
    }
}

void WiFiPlazaTextWindow_Print(Window *window, String *string, u32 x, u32 y) {
    AddTextPrinterParameterizedWithColor(window, 0, string, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    ScheduleWindowCopyToVram(window);
}

void WiFiPlazaTextWindow_FillRect(Window *window, u16 x, u16 y, u16 width, u16 height) {
    FillWindowPixelRect(window, 15, x, y, width, height);
}

void WiFiPlazaYesNoMenu_Init(WiFiPlazaYesNoMenu *menu, WiFiPlazaRoomMsg *msg, enum HeapID heapID) {
    String *text;
    if (menu->items != NULL) {
        GF_AssertFail();
    }
    menu->items = ListMenuItems_New(2, heapID);
    text = WiFiPlazaRoomMsg_GetString(msg, 1, 0x43);
    ListMenuItems_AddItem(menu->items, text, 0);
    text = WiFiPlazaRoomMsg_GetString(msg, 1, 0x42);
    ListMenuItems_AddItem(menu->items, text, 1);
    menu->template = sWiFiPlazaYesNoTemplate;
    menu->template.totalItems = 2;
    menu->template.items = menu->items;
}

void WiFiPlazaYesNoMenu_Free(WiFiPlazaYesNoMenu *menu) {
    ListMenuItem *items = menu->items;
    if (items != NULL) {
        ListMenuItems_Delete(items);
        menu->items = NULL;
    }
}

void WiFiPlazaErrorWindow_Init(Window *window, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID) {
    AddWindowParameterized(graphics->bgConfig, window, GF_BG_LYR_MAIN_1, 4, 4, 23, 16, 5, 0x5E);
    FillWindowPixelBuffer(window, 15);
}

void WiFiPlazaErrorWindow_Free(Window *window) {
    RemoveWindow(window);
}

void WiFiPlazaErrorWindow_Print(Window *window, String *string) {
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    DrawFrameAndWindow1(window, TRUE, 0x55, 3);
    ScheduleWindowCopyToVram(window);
}

void WiFiPlazaErrorWindow_PrintCommError(Window *window, WiFiPlazaRoomMsg *msg) {
    u32 *error = CommState_GetWifiError();
    u32 msgId = ov45_0222D7CC(error[0], error[1]);
    WiFiPlazaRoomMsg_BufferInt(msg, error[0], 5, 0, 2);
    WiFiPlazaErrorWindow_Print(window, WiFiPlazaRoomMsg_GetString(msg, 2, msgId));
}

void WiFiPlazaErrorWindow_PrintLobbyError(Window *window, WiFiPlazaRoomMsg *msg, u32 error) {
    u32 code = ov45_0222E7FC(error);
    WiFiPlazaRoomMsg_BufferInt(msg, code, 5, 0, 2);
    WiFiPlazaErrorWindow_Print(window, WiFiPlazaRoomMsg_GetString(msg, 2, 0x20));
}

void WiFiPlazaErrorWindow_PrintDisconnected(Window *window, WiFiPlazaRoomMsg *msg, u32 unused) {
    WiFiPlazaErrorWindow_Print(window, WiFiPlazaRoomMsg_GetString(msg, 2, 0xE));
}

void WiFiPlazaRoomMsg_Init(WiFiPlazaRoomMsg *msg, enum HeapID heapID) {
    int i;
    msg->format = MessageFormat_New_Custom(8, 64, heapID);
    for (i = 0; i < 4; i++) {
        msg->msgData[i] = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, sWiFiPlazaRoomMsgBanks[i], heapID);
    }
    msg->formatted = String_New(0x1E0, heapID);
    msg->raw = String_New(0x1E0, heapID);
}

void WiFiPlazaRoomMsg_Free(WiFiPlazaRoomMsg *msg) {
    int i;
    MessageFormat_Delete(msg->format);
    for (i = 0; i < 4; i++) {
        DestroyMsgData(msg->msgData[i]);
    }
    String_Delete(msg->formatted);
    String_Delete(msg->raw);
}

String *WiFiPlazaRoomMsg_GetString(WiFiPlazaRoomMsg *msg, int bank, int msgId) {
    ReadMsgDataIntoString(msg->msgData[bank], msgId, msg->raw);
    StringExpandPlaceholders(msg->format, msg->formatted, msg->raw);
    return msg->formatted;
}

void WiFiPlazaRoomMsg_BufferInt(WiFiPlazaRoomMsg *msg, int value, int digits, int field, int style) {
    BufferIntegerAsString(msg->format, field, value, digits, (PrintingMode)style, TRUE);
}

void WiFiPlazaRoomMsg_BufferPlayerName(WiFiPlazaRoomMsg *msg, PlayerProfile *profile, u32 field) {
    BufferPlayersName(msg->format, field, profile);
}

void WiFiPlazaRoomMsg_BufferActivityName(WiFiPlazaRoomMsg *msg, u32 activity, u32 field) {
    BufferWiFiPlazaActivityName(msg->format, field, activity);
}

void WiFiPlazaRoomMsg_BufferEventName(WiFiPlazaRoomMsg *msg, u32 event, u32 field) {
    BufferWiFiPlazaEventName(msg->format, field, event);
}

void WiFiPlazaRoomMsg_BufferInstrumentName(WiFiPlazaRoomMsg *msg, u32 instrument, u32 field) {
    BufferWiFiPlazaInstrumentName(msg->format, field, instrument);
}

void WiFiPlazaRoomMsg_BufferCountryName(WiFiPlazaRoomMsg *msg, u32 country, u32 field) {
    BufferCountryName(msg->format, field, country);
}

void WiFiPlazaRoomMsg_BufferCityName(WiFiPlazaRoomMsg *msg, u32 country, u32 city, u32 field) {
    BufferCityName(msg->format, field, country, city);
}

void WiFiPlazaRoomMsg_BufferECWord(WiFiPlazaRoomMsg *msg, u32 field, u16 word) {
    BufferECWord(msg->format, field, word);
}

void WiFiPlazaRoomMsg_ResetBuffers(WiFiPlazaRoomMsg *msg) {
    MessageFormat_ResetBuffers(msg->format);
}

void WiFiPlazaRoom_SetPanelButtonPressed(WiFiPlazaRoomWork *room) {
    room->panelButtonPressed = TRUE;
}
