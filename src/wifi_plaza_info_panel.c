#include "global.h"

#include "assert.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "pokemon.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "text.h"
#include "type_icon_resources.h"
#include "unk_0200FA24.h"
#include "wifi_plaza_event.h"
#include "wifi_plaza_room.h"

// The touch screen of the plaza room: the own trainer card (with a button that plays the own Tap Toy), the
// card of another player (name, ID, country, easy chat message or Pokemon, Tap Toy icon, avatar) and the Tap Toy
// panel. Page switches fade the sub screen out and in (WiFiPlazaInfoPanel_Update).

// type_icon_resources.c (no header declares it)
int TypeIcon_GetPlttFile(void);

// rodata of this overlay (asm/wifi_plaza_map_rodata.s)
extern const u8 ov49_022696EC[];
extern const s8 ov49_022696F0[];
extern const TouchscreenHitbox sWiFiPlazaPanelButtonHitbox[];
extern u16 ov49_022696F8[];
extern const u16 ov49_022696FC[];
extern const u16 ov49_02269704[];
extern const WindowTemplate sWiFiPlazaPanelButtonWindowTemplate;
extern u8 ov49_02269764[];
extern const WiFiPlazaCardMonResources ov49_02269774[];
extern const u8 ov49_0226978C[][0x10];
extern const WindowTemplate sWiFiPlazaCardWindowTemplates[];
extern const WiFiPlazaCardAvatar sWiFiPlazaCardAvatars[];
extern const VecFx32 sWiFiPlazaCardMonPositions[];
extern u8 ov49_022699AC[];

void WiFiPlazaInfoPanel_Init(WiFiPlazaInfoPanel *panel, u16 *startParams, WiFiPlazaRoomGraphics *graphics, PlayerProfile *profile, enum HeapID heapID) {
    u32 gender = PlayerProfile_GetTrainerGender(profile);

    panel->narc = NARC_New(NARC_a_2_0_7, heapID);
    WiFiPlazaInfoPanel_LoadGraphics(panel, graphics, panel->narc, gender, heapID);
    WiFiPlazaPlayerCard_Init(&panel->card, graphics, panel->narc, heapID);
    WiFiPlazaPanelButton_Init(&panel->button, graphics, panel->narc, heapID);
    panel->request = 0;
    panel->page = 0;
    if (startParams[3] == 0) {
        panel->state = 0;
        // called without the mask colour argument
        ((void (*)(u8))BG_SetMaskColor)(GF_BG_LYR_SUB_0);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    } else {
        panel->state = 5;
    }
}

void WiFiPlazaInfoPanel_Free(WiFiPlazaInfoPanel *panel, WiFiPlazaMsgWindow *signWindow, WiFiPlazaRoomGraphics *graphics) {
    WiFiPlazaPanelButton_Free(&panel->button);
    WiFiPlazaPlayerCard_Free(&panel->card, signWindow, graphics);
    WiFiPlazaInfoPanel_FreeGraphics(panel, graphics);
    NARC_Delete(panel->narc);
}

typedef struct WiFiPlazaInstrumentToken {
    u32 type;
    u32 index;
} WiFiPlazaInstrumentToken;

#pragma force_active on
#pragma opt_propagation off

void WiFiPlazaInfoPanel_Update(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomWork *room, BOOL paused, enum HeapID heapID) {
    WiFiPlazaInstrumentToken started, own;
    WiFiPlazaWork *lobby;
    int offset;
    WiFiPlazaRoomGraphics *graphics;
    int special, hasMessage, isOwn, message, token;
    struct WiFiPlazaPlayer *myPlayer;
    int playerIndex;
    struct WiFiPlazaPlayer *player;

    graphics = &room->graphics;
    lobby = room->lobby;
    offset = offsetof(WiFiPlazaRoomWork, msg); // + 0x3C: signWindow
    switch (panel->state) {
    case 0:
        if (panel->started == TRUE) {
            panel->state++;
        }
        break;
    case 1:
        BeginNormalPaletteFade(4, 0, 0, RGB_WHITE, 4, 1, heapID);
        panel->state++;
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            WiFiPlazaInfoPanel_ShowOwnCard(panel, graphics, &room->msg, heapID, WiFiPlazaWork_GetMyPlayer(lobby));
            panel->page = 4;
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            panel->state++;
        }
        break;
    case 3:
        BeginNormalPaletteFade(4, 1, 1, RGB_WHITE, 6, 1, heapID);
        panel->state++;
        break;
    case 4:
        if (IsPaletteFadeFinished() == TRUE) {
            panel->state++;
        }
        break;
    case 5:
        if (paused != TRUE) {
            switch (panel->page) {
            case 0:
            case 4:
                break;
            case 1:
                if (WiFiPlazaPanelButton_Update(&panel->button, graphics, panel->narc, heapID) == TRUE) {
                    WiFiPlazaRoom_SetPanelButtonPressed(room);
                }
                break;
            case 3:
                if (panel->instrumentPlaying) {
                    if (WiFiPlazaEvent_GetMoverState(room->event, panel->instrumentIndex, panel->instrumentType) == 0) {
                        panel->instrumentPlaying = FALSE;
                        WiFiPlazaPanelButton_ResetTimer(&panel->button);
                    }
                }
                if (WiFiPlazaPanelButton_Update(&panel->button, graphics, panel->narc, heapID) == TRUE) {
                    ov45_0222ADD8(lobby, WiFiPlazaWork_GetOwnSlot(lobby));
                    ov45_0222AE54(lobby);
                    ov45_0222AE08(ov45_0222ADA8(lobby, WiFiPlazaWork_GetOwnSlot(lobby)), &started.index, &started.type);
                    panel->instrumentPlaying = TRUE;
                    panel->instrumentIndex = started.index;
                    panel->instrumentType = started.type;
                }
                break;
            case 2:
                WiFiPlazaPlayerCard_Update(&panel->card, (WiFiPlazaMsgWindow *)((u8 *)room + (offset + 0x3C)));
                offset += 0x3C;
                break;
            }
            if (panel->request) {
                panel->state++;
            }
        }
        break;
    case 6:
        BeginNormalPaletteFade(4, 1, 0, 0, 3, 1, heapID);
        panel->state++;
        break;
    case 7:
        if (IsPaletteFadeFinished() != TRUE) {
            break;
        }
        switch (panel->page) {
        case 0:
            break;
        case 1:
        case 3:
        case 4:
            WiFiPlazaInfoPanel_HideButtonPage(panel, graphics);
            break;
        case 2:
            WiFiPlazaInfoPanel_HidePlayerCard(panel, &room->signWindow, graphics);
            break;
        }
        panel->state++;
        break;
    case 8:
        switch (panel->request) {
        case 0:
            break;
        case 1:
        case 4:
            WiFiPlazaInfoPanel_ShowOwnCard(panel, graphics, &room->msg, heapID, WiFiPlazaWork_GetMyPlayer(lobby));
            break;
        case 2:
            myPlayer = WiFiPlazaWork_GetMyPlayer(lobby);
            playerIndex = panel->requestPlayer;
            if (playerIndex == WiFiPlazaWork_GetOwnSlot(lobby)) {
                player = WiFiPlazaWork_GetMyPlayer(lobby);
                isOwn = TRUE;
            } else {
                player = WiFiPlazaWork_GetPlayer(lobby, playerIndex);
                isOwn = FALSE;
            }
            special = WiFiPlazaWork_IsSpecialVisitor(lobby, panel->requestPlayer);
            if (panel->requestArg == TRUE) {
                hasMessage = WiFiPlazaWork_HasWordSet(lobby, panel->requestPlayer);
                message = (int)WiFiPlazaWork_GetWordSet(lobby, panel->requestPlayer);
                if (message == 0) {
                    hasMessage = FALSE;
                }
            } else {
                hasMessage = FALSE;
                message = 0;
            }
            WiFiPlazaInfoPanel_ShowPlayerCard(panel, &room->signWindow, graphics, (WiFiPlazaRoomMsg *)((u8 *)room + offset), isOwn, heapID, player, myPlayer, special, hasMessage, (u16 *)message, TRUE);
            break;
        case 3:
            token = ov45_0222ADA8(lobby, WiFiPlazaWork_GetOwnSlot(lobby));
            if (token == -1) {
                GF_ASSERT(FALSE);
                token = 0;
            }
            ov45_0222AE08(token, &own.index, &own.type);
            WiFiPlazaInfoPanel_ShowInstrument(panel, graphics, heapID, own.type);
            break;
        }
        panel->request = 0;
        panel->state++;
        break;
    case 9:
        BeginNormalPaletteFade(4, 1, 1, 0, 3, 1, heapID);
        if (panel->page == 2) {
            WiFiPlazaPlayerCard_Update(&panel->card, &room->signWindow);
        }
        panel->state++;
        break;
    case 10:
        if (IsPaletteFadeFinished() == TRUE) {
            panel->state = 5;
        }
        break;
    }
}

#pragma opt_propagation reset
#pragma force_active reset

u8 WiFiPlazaInfoPanel_GetState(WiFiPlazaInfoPanel *panel) {
    return panel->state;
}

void WiFiPlazaInfoPanel_RequestPlayerCard(WiFiPlazaInfoPanel *panel, u32 playerIndex, u32 showMessage) {
    panel->request = 2;
    panel->requestPlayer = playerIndex;
    panel->requestArg = showMessage;
}

#pragma force_active on

void WiFiPlazaInfoPanel_SetCardIcon(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, u32 icon, enum HeapID heapID) {
    GF_ASSERT(icon < 27);
    GF_ASSERT(panel->page == 2);
    if (panel->page == 2) {
        WiFiPlazaPlayerCard_ChangeIcon(&panel->card, graphics, panel->narc, heapID, icon);
    }
}

#pragma force_active reset

void WiFiPlazaInfoPanel_RequestOwnCard(WiFiPlazaInfoPanel *panel) {
    if (panel->page != 1) {
        panel->request = 1;
    }
}

void WiFiPlazaInfoPanel_RequestInstrument(WiFiPlazaInfoPanel *panel) {
    if (panel->page != 3) {
        panel->request = 3;
    }
}

u8 WiFiPlazaInfoPanel_GetCardPlayer(WiFiPlazaInfoPanel *panel) {
    return panel->requestPlayer;
}

BOOL WiFiPlazaInfoPanel_IsPlayerCardShown(WiFiPlazaInfoPanel *panel) {
    if (panel->request == 2) {
        return TRUE;
    }
    if (panel->page == 2) {
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaInfoPanel_IsPlayerCardIdle(WiFiPlazaInfoPanel *panel) {
    return panel->request == 0 && panel->page == 2;
}

BOOL WiFiPlazaInfoPanel_CardShowsMessage(WiFiPlazaInfoPanel *panel) {
    return WiFiPlazaPlayerCard_ShowsMessage(&panel->card);
}

BOOL WiFiPlazaInfoPanel_HasRequest(WiFiPlazaInfoPanel *panel) {
    return panel->request != 0;
}

#pragma force_active on

void WiFiPlazaInfoPanel_ShowPlayerCard(WiFiPlazaInfoPanel *panel, WiFiPlazaMsgWindow *signWindow, WiFiPlazaRoomGraphics *graphics, WiFiPlazaRoomMsg *msg, BOOL isOwn, enum HeapID heapID, struct WiFiPlazaPlayer *player, struct WiFiPlazaPlayer *myPlayer, int special, BOOL hasMessage, u16 *message, BOOL showIcon) {
    if (panel->page == 2) {
        WiFiPlazaInfoPanel_HidePlayerCard(panel, signWindow, graphics);
    }
    WiFiPlazaPlayerCard_Show(panel, signWindow, &panel->card, graphics, msg, isOwn, panel->narc, heapID, player, myPlayer, special, hasMessage, message, showIcon);
    panel->page = 2;
}

#pragma force_active reset

void WiFiPlazaInfoPanel_HidePlayerCard(WiFiPlazaInfoPanel *panel, WiFiPlazaMsgWindow *signWindow, WiFiPlazaRoomGraphics *graphics) {
    WiFiPlazaPlayerCard_Clear(&panel->card, signWindow, graphics);
    panel->page = 0;
}

void WiFiPlazaInfoPanel_ShowOwnCard(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, WiFiPlazaRoomMsg *msg, enum HeapID heapID, struct WiFiPlazaPlayer *player) {
    if (panel->page == 1 || panel->page == 3 || panel->page == 4) {
        WiFiPlazaInfoPanel_HideButtonPage(panel, graphics);
    }
    WiFiPlazaPanelButton_ShowOwnCard(&panel->button, graphics, msg, player, panel->narc, heapID);
    panel->page = 1;
}

void WiFiPlazaInfoPanel_ShowInstrument(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID, u32 type) {
    if (panel->page == 3) {
        WiFiPlazaInfoPanel_HideButtonPage(panel, graphics);
    }
    WiFiPlazaPanelButton_ShowInstrument(&panel->button, graphics, type, panel->narc, heapID);
    panel->page = 3;
}

void WiFiPlazaInfoPanel_HideButtonPage(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics) {
    WiFiPlazaPanelButton_Clear(&panel->button, graphics);
    panel->page = 0;
}

void WiFiPlazaInfoPanel_SetButtonDisabled(WiFiPlazaInfoPanel *panel, u16 disabled) {
    panel->button.disabled = disabled;
}

SysTask *WiFiPlazaInfoPanel_StartPaletteLoad(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, u32 member, enum HeapID heapID) {
    panel->paletteMember = member;
    panel->paletteHeapID = heapID;
    return SysTask_CreateOnVWaitQueue(WiFiPlazaInfoPanel_LoadPaletteTask, panel, 0);
}

void WiFiPlazaInfoPanel_LoadPaletteTask(SysTask *task, void *data) {
    WiFiPlazaInfoPanel *panel = data;
    NarcId narcId;
    int memberNo;

    GfGfxLoader_GXLoadPalFromOpenNarc(panel->narc, panel->paletteMember, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, (enum HeapID)panel->paletteHeapID);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_5_OFFSET, (enum HeapID)panel->paletteHeapID);
    narcId = TypeIcon_GetNarc();
    memberNo = TypeIcon_GetPlttFile();
    GfGfxLoader_GXLoadPal(narcId, memberNo, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_11_OFFSET, 0x60, (enum HeapID)panel->paletteHeapID);
    SysTask_Destroy(task);
}

void WiFiPlazaInfoPanel_LoadGraphics(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, NARC *narc, u32 gender, enum HeapID heapID) {
    if (gender == 0) {
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0x56, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, heapID);
    } else {
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0x57, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, heapID);
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x36, graphics->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_5_OFFSET, heapID);
}

void WiFiPlazaInfoPanel_FreeGraphics(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics) {
}

void WiFiPlazaInfoPanel_LoadScreen(WiFiPlazaRoomGraphics *graphics, NARC *narc, int member, int bgId, int tileOffset, enum HeapID heapID) {
    NNSG2dScreenData *screen;
    void *allocation;
    int i;
    int count;
    u16 *rawData;

    allocation = GfGfxLoader_GetScrnDataFromOpenNarc(narc, member, FALSE, &screen, heapID);
    count = screen->szByte >> 1;
    rawData = (u16 *)screen->rawData;
    for (i = 0; i < count; i++) {
        rawData[i] += tileOffset;
    }
    LoadRectToBgTilemapRect(graphics->bgConfig, bgId, screen->rawData, 0, 0, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(graphics->bgConfig, bgId);
    Heap_Free(allocation);
}

void WiFiPlazaPlayerCard_Init(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID) {
    int i;
    const WindowTemplate *template = sWiFiPlazaCardWindowTemplates;
    Window *window;

    i = 0;
    window = card->windows;
    do {
        AddWindow(graphics->bgConfig, window, template);
        i++;
        template ++;
        window++;
    } while (i < 5);
}

void WiFiPlazaPlayerCard_Free(WiFiPlazaPlayerCard *card, WiFiPlazaMsgWindow *signWindow, WiFiPlazaRoomGraphics *graphics) {
    int i;

    if (card->iconSprite != NULL) {
        WiFiPlazaPlayerCard_Clear(card, signWindow, graphics);
    }
    for (i = 0; i < 5; i++) {
        RemoveWindow(&card->windows[i]);
    }
}

void WiFiPlazaPlayerCard_Update(WiFiPlazaPlayerCard *card, WiFiPlazaMsgWindow *signWindow) {
}

#pragma force_active on

void WiFiPlazaPlayerCard_Show(WiFiPlazaInfoPanel *panel, WiFiPlazaMsgWindow *signWindow, WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, WiFiPlazaRoomMsg *msg, BOOL isOwn, NARC *narc, enum HeapID heapID, struct WiFiPlazaPlayer *player, struct WiFiPlazaPlayer *myPlayer, int special, BOOL hasMessage, u16 *message, BOOL showIcon) {
    int color, avatar;
    PlayerProfile *profile;

    WiFiPlazaRoomMsg_ResetBuffers(msg);
    if (!WiFiPlazaPlayer_GetGender(player)) {
        color = MAKE_TEXT_COLOR(7, 8, 0);
        if (!special) {
            WiFiPlazaInfoPanel_StartPaletteLoad(panel, graphics, 86, heapID);
        } else {
            WiFiPlazaInfoPanel_StartPaletteLoad(panel, graphics, 88, heapID);
        }
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
        if (!special) {
            WiFiPlazaInfoPanel_StartPaletteLoad(panel, graphics, 87, heapID);
        } else {
            WiFiPlazaInfoPanel_StartPaletteLoad(panel, graphics, 88, heapID);
        }
    }
    BgFillTilemapBufferAndSchedule(graphics->bgConfig, GF_BG_LYR_SUB_0, 0);
    BgFillTilemapBufferAndSchedule(graphics->bgConfig, GF_BG_LYR_SUB_1, 0);
    BgFillTilemapBufferAndSchedule(graphics->bgConfig, GF_BG_LYR_SUB_2, 0);
    WiFiPlazaPlayerCard_ClearWindows(card);
    WiFiPlazaInfoPanel_LoadScreen(graphics, narc, 92, GF_BG_LYR_SUB_0, 0, heapID);
    profile = PlayerProfile_New(heapID);
    WiFiPlazaPlayer_CopyToProfile(player, profile, heapID);
    WiFiPlazaPlayerCard_Print(card, msg, 0, 44, 0, 0, color);
    WiFiPlazaRoomMsg_BufferInt(msg, PlayerProfile_GetTrainerID_VisibleHalf(profile), 5, 0, 2);
    WiFiPlazaPlayerCard_PrintRightAligned(card, msg, 0, 49, 122, 0, MAKE_TEXT_COLOR(1, 2, 0));
    WiFiPlazaPlayerCard_Print(card, msg, 0, 45, 0, 16, color);
    WiFiPlazaPlayerCard_BufferName(card, msg, profile);
    WiFiPlazaPlayerCard_PrintRightAligned(card, msg, 0, 50, 122, 16, MAKE_TEXT_COLOR(1, 2, 0));
    WiFiPlazaPlayerCard_CopyWindow(card, 0);
    WiFiPlazaPlayerCard_Print(card, msg, 1, 46, 0, 0, color);
    if (!WiFiPlazaPlayer_GetCountry(player)) {
        WiFiPlazaPlayerCard_Print(card, msg, 1, 55, 32, 16, MAKE_TEXT_COLOR(1, 2, 0));
    } else {
        WiFiPlazaPlayerCard_BufferLocation(card, msg, player);
        WiFiPlazaPlayerCard_Print(card, msg, 1, 51, 32, 16, MAKE_TEXT_COLOR(1, 2, 0));
    }
    WiFiPlazaPlayerCard_CopyWindow(card, 1);
    if (hasMessage) {
        card->showsMessage = TRUE;
        WiFiPlazaPlayerCard_BufferName(card, msg, profile);
        WiFiPlazaPlayerCard_Print(card, msg, 4, 56, 8, 0, color);
        WiFiPlazaPlayerCard_BufferMessage(card, msg, message);
        WiFiPlazaPlayerCard_Print(card, msg, 4, 59, 6, 24, MAKE_TEXT_COLOR(1, 2, 0));
        WiFiPlazaPlayerCard_Print(card, msg, 4, 60, 90, 24, MAKE_TEXT_COLOR(1, 2, 0));
        WiFiPlazaPlayerCard_Print(card, msg, 4, 61, 6, 44, MAKE_TEXT_COLOR(1, 2, 0));
        WiFiPlazaPlayerCard_Print(card, msg, 4, 62, 90, 44, MAKE_TEXT_COLOR(1, 2, 0));
        WiFiPlazaPlayerCard_CopyWindow(card, 4);
    } else {
        card->showsMessage = FALSE;
        WiFiPlazaPlayerCard_Print(card, msg, 4, 48, 8, 0, color);
        WiFiPlazaPlayerCard_CopyWindow(card, 4);
        WiFiPlazaPlayerCard_LoadMons(card, graphics, narc, heapID, player, myPlayer);
    }
    WiFiPlazaPlayerCard_LoadIcon(card, graphics, narc, heapID, ov45_0222AAC8(player), showIcon);
    if (isOwn == TRUE) {
        if (WiFiPlazaPlayer_GetGender(player) == 1) {
            avatar = 97;
        } else {
            avatar = 0;
        }
    } else {
        avatar = WiFiPlazaPlayer_GetAvatar(player);
    }
    WiFiPlazaPlayerCard_LoadAvatar(card, graphics, narc, heapID, avatar);
    WiFiPlazaPlayerCard_LoadTypeIcons(panel, card, graphics, heapID, player);
    Heap_Free(profile);
}

#pragma force_active reset

void WiFiPlazaPlayerCard_LoadTypeIcons(WiFiPlazaInfoPanel *unused0, WiFiPlazaPlayerCard *unused1, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID, struct WiFiPlazaPlayer *player) {
    s32 i = 0;
    u16 *tileBase = ov49_022696F8;
    const u8(*tiles)[0x10] = ov49_0226978C;
    u32 y = 2;

    for (; i < 2; i++) {
        if ((u16)ov45_0222AAEC(player, i) != 0) {
            u16 type = ((u32(*)(void))ov45_0222D7C0)(); // called without its argument: the type is still in r0
            NarcId narcId = TypeIcon_GetNarc();
            u32 member = TypeIcon_GetCharFile(type);
            void *buf = GfGfxLoader_LoadFromNarc(narcId, member, TRUE, heapID, TRUE);
            NNSG2dCharacterData *charData;

            NNS_G2dGetUnpackedCharacterData(buf, &charData);
            BG_LoadCharTilesData(graphics->bgConfig, GF_BG_LYR_SUB_2, charData->pRawData, 0x100, *tileBase);
            Heap_Free(buf);
            CopyToBgTilemapRect(graphics->bgConfig, GF_BG_LYR_SUB_2, 26, y, 4, 2, *tiles, 0, 0, 4, 2);
            BgTilemapRectChangePalette(graphics->bgConfig, GF_BG_LYR_SUB_2, 26, y, 4, 2, (u8)(TypeIcon_GetPalette(type) + 11));
            ScheduleBgTilemapBufferTransfer(graphics->bgConfig, GF_BG_LYR_SUB_2);
        }
        tiles++;
        tileBase++;
        y += 2;
    }
}

void WiFiPlazaPlayerCard_Clear(WiFiPlazaPlayerCard *card, WiFiPlazaMsgWindow *unused, WiFiPlazaRoomGraphics *graphics) {
    if (card->showsMessage == TRUE) {
        card->showsMessage = FALSE;
    } else {
        WiFiPlazaPlayerCard_FreeMons(card, graphics);
    }
    WiFiPlazaPlayerCard_FreeIcon(card, graphics);
    WiFiPlazaPlayerCard_FreeAvatar(card, graphics);
    BgClearTilemapBufferAndCommit(graphics->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(graphics->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(graphics->bgConfig, GF_BG_LYR_SUB_2);
}

void WiFiPlazaPlayerCard_ChangeIcon(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID, u32 icon) {
    WiFiPlazaPlayerCard_FreeIcon(card, graphics);
    WiFiPlazaPlayerCard_LoadIcon(card, graphics, narc, heapID, icon, TRUE);
}

BOOL WiFiPlazaPlayerCard_ShowsMessage(WiFiPlazaPlayerCard *card) {
    return card->showsMessage;
}

// Tap Toy icon: 27 icons, 3 animations per graphics set
void WiFiPlazaPlayerCard_LoadIcon(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID, u32 icon, BOOL draw) {
    SpriteResourcesHeader header;
    u32 set, anim;

    GF_ASSERT(icon < 27);
    GF_ASSERT(card->iconSprite == NULL);
    set = icon / 3;
    anim = icon % 3;

    card->iconResources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_CHAR], narc, set * 3 + 0x3D, FALSE, 100, NNS_G2D_VRAM_TYPE_2DSUB, heapID);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(card->iconResources[GF_GFX_RES_TYPE_CHAR]));
    GF2DGfxResObj_FreeData(card->iconResources[GF_GFX_RES_TYPE_CHAR]);
    card->iconResources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_PLTT], narc, 0x59, FALSE, 100, NNS_G2D_VRAM_TYPE_2DSUB, 3, heapID);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(card->iconResources[GF_GFX_RES_TYPE_PLTT]));
    GF2DGfxResObj_FreeData(card->iconResources[GF_GFX_RES_TYPE_PLTT]);
    card->iconResources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_CELL], narc, set * 3 + 0x3C, FALSE, 100, GF_GFX_RES_TYPE_CELL, heapID);
    card->iconResources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_ANIM], narc, set * 3 + 0x3B, FALSE, 100, GF_GFX_RES_TYPE_ANIM, heapID);

    SimpleSpriteTemplate template = { 0 };
    CreateSpriteResourcesHeader(&header, 100, 100, 100, 100, -1, -1, FALSE, 0, graphics->resManagers[GF_GFX_RES_TYPE_CHAR], graphics->resManagers[GF_GFX_RES_TYPE_PLTT], graphics->resManagers[GF_GFX_RES_TYPE_CELL], graphics->resManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = graphics->spriteList;
    template.header = &header;
    template.priority = 16;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.position.x = FX32_CONST(208);
    template.position.y = FX32_CONST(408);
    template.heapID = heapID;
    card->iconSprite = Sprite_Create(&template);
    Sprite_SetAnimCtrlSeq(card->iconSprite, anim);
    Sprite_SetDrawFlag(card->iconSprite, draw);
}

#pragma force_active on

void WiFiPlazaPlayerCard_FreeIcon(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics) {
    int i;

    Sprite_Delete(card->iconSprite);
    card->iconSprite = NULL;
    SpriteTransfer_DeleteCharTransferTask(card->iconResources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(card->iconResources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(graphics->resManagers[i], card->iconResources[i]);
    }
}

#pragma force_active reset

void WiFiPlazaPlayerCard_LoadAvatar(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID, u32 avatar) {
    TrainerPicFileIds files;
    SpriteResourcesHeader header;
    u16 trainerClass, x;
    u16 y;
    u32 i;

    GF_ASSERT(card->avatarSprite == NULL);
    for (i = 0; i < 18; i++) {
        if (avatar == sWiFiPlazaCardAvatars[i].avatar) {
            trainerClass = sWiFiPlazaCardAvatars[i].trainerClass;
            x = sWiFiPlazaCardAvatars[i].x;
            y = sWiFiPlazaCardAvatars[i].y;
        }
    }
    GetTrainerPicFileIdsNoLink(trainerClass, 2, &files);

    card->avatarResources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(graphics->resManagers[GF_GFX_RES_TYPE_CHAR], files.narcId, files.ncgr_id, FALSE, 101, NNS_G2D_VRAM_TYPE_2DSUB, heapID);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(card->avatarResources[GF_GFX_RES_TYPE_CHAR]));
    card->avatarResources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(graphics->resManagers[GF_GFX_RES_TYPE_PLTT], files.narcId, files.nclr_id, FALSE, 101, NNS_G2D_VRAM_TYPE_2DSUB, 1, heapID);
    WiFiPlazaInfoPanel_TintPalette(GF2DGfxResObj_GetPlttDataPtr(card->avatarResources[GF_GFX_RES_TYPE_PLTT]));
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(card->avatarResources[GF_GFX_RES_TYPE_PLTT]));
    GF2DGfxResObj_FreeData(card->avatarResources[GF_GFX_RES_TYPE_PLTT]);
    card->avatarResources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(graphics->resManagers[GF_GFX_RES_TYPE_CELL], files.narcId, files.ncer_id, FALSE, 101, GF_GFX_RES_TYPE_CELL, heapID);
    card->avatarResources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(graphics->resManagers[GF_GFX_RES_TYPE_ANIM], files.narcId, files.nanr_id, FALSE, 101, GF_GFX_RES_TYPE_ANIM, heapID);

    SimpleSpriteTemplate template = { 0 };
    CreateSpriteResourcesHeader(&header, 101, 101, 101, 101, -1, -1, TRUE, 3, graphics->resManagers[GF_GFX_RES_TYPE_CHAR], graphics->resManagers[GF_GFX_RES_TYPE_PLTT], graphics->resManagers[GF_GFX_RES_TYPE_CELL], graphics->resManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = graphics->spriteList;
    template.header = &header;
    template.priority = 32;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = heapID;
    template.position.x = FX32_CONST(x);
    template.position.y = FX32_CONST(y) + FX32_CONST(256);
    card->avatarSprite = Sprite_Create(&template);
}

#pragma force_active on

void WiFiPlazaPlayerCard_FreeAvatar(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics) {
    int i;

    ObjCharTransfer_DeleteTaskCopyByProxyPtr(Sprite_GetImageProxy(card->avatarSprite));
    Sprite_Delete(card->avatarSprite);
    card->avatarSprite = NULL;
    SpriteTransfer_DeleteCharTransferTask(card->avatarResources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(card->avatarResources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(graphics->resManagers[i], card->avatarResources[i]);
    }
}

#pragma force_active reset

// Tints the avatar palette: grey level of each colour, mapped to a blue-green shade.
#pragma force_active on
#pragma opt_propagation off

void WiFiPlazaInfoPanel_TintPalette(NNSG2dPaletteData *pltt) {
    int i;
    int n = pltt->szByte / 2;
    u16 *p = pltt->pRawData;
    int r, g, b, gray;
    i = 0;
    if (n > 0) {
        do {
            r = *p & 0x1F;
            g = (*p >> 5) & 0x1F;
            b = (*p >> 10) & 0x1F;
            gray = (r * 76 + g * 151 + b * 29) >> 8;
            *p = (((gray * 128) >> 8) << 10) | (((gray * 200) >> 8) << 5) | ((gray * 256) >> 8);
            p++;
        } while (++i < n);
    }
}

#pragma opt_propagation reset
#pragma force_active reset

void WiFiPlazaPlayerCard_ClearWindows(WiFiPlazaPlayerCard *card) {
    int i;
    Window *window = card->windows;
    for (i = 0; i < 5; i++, window++) {
        FillWindowPixelBuffer(window, 0);
    }
}

void WiFiPlazaPlayerCard_Print(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, int window, int msgId, u8 x, u8 y, u32 color) {
    String *string = WiFiPlazaRoomMsg_GetString(msg, 1, msgId);
    AddTextPrinterParameterizedWithColor(&card->windows[window], 0, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void WiFiPlazaPlayerCard_PrintRightAligned(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, int window, int msgId, u8 width, u8 y, u32 color) {
    String *string = WiFiPlazaRoomMsg_GetString(msg, 1, msgId);
    int x = width - FontID_String_GetWidth(0, string, 0);
    if (x < 0) {
        x = 0;
    }
    AddTextPrinterParameterizedWithColor(&card->windows[window], 0, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void WiFiPlazaPlayerCard_CopyWindow(WiFiPlazaPlayerCard *card, int window) {
    ScheduleWindowCopyToVram(&card->windows[window]);
}

void WiFiPlazaPlayerCard_BufferName(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, PlayerProfile *profile) {
    WiFiPlazaRoomMsg_BufferPlayerName(msg, profile, 0);
}

void WiFiPlazaPlayerCard_BufferLocation(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, struct WiFiPlazaPlayer *player) {
    int country = WiFiPlazaPlayer_GetCountry(player);
    int region = WiFiPlazaPlayer_GetRegion(player);
    WiFiPlazaRoomMsg_BufferCountryName(msg, country, 0);
    WiFiPlazaRoomMsg_BufferCityName(msg, country, region, 1);
}

void WiFiPlazaPlayerCard_BufferMessage(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, u16 *words) {
    int i;
    for (i = 0; i < 4; i++) {
        WiFiPlazaRoomMsg_BufferECWord(msg, i, words[i]);
    }
}

// The player's Pokemon (12 slots, species 24 = empty) with shared palettes / cells / animations per kind.
#pragma force_active on

void WiFiPlazaPlayerCard_LoadMons(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *panelNarc, enum HeapID heapID, struct WiFiPlazaPlayer *player, struct WiFiPlazaPlayer *myPlayer) {
    NARC *narc = NARC_New((NarcId)215, heapID);
    int gender;
    const VecFx32 *pos;
    int version = WiFiPlazaPlayer_GetLobbyId(myPlayer);
    const WiFiPlazaCardMonResources *res;
    const u16 *cell, *anim;
    void *use;
    int i;
    WiFiPlazaPlayerCard *cursor;
    SimpleSpriteTemplate template;
    SpriteResourcesHeader header;

    gender = WiFiPlazaPlayer_GetGender(myPlayer);
    res = ov49_02269774;
    i = 0;
    cell = ov49_02269704;
    cursor = card;
    anim = ov49_022696FC;
    for (; i < 4; i++) {
        if (i == 2) {
            if (gender == 1) {
                cursor->monPltts[0] = NULL;
                cursor->monCells[0] = NULL;
                cursor->monAnims[0] = NULL;
                goto next;
            }
        } else if (i == 3) {
            if (gender == 0) {
                cursor->monPltts[0] = NULL;
                cursor->monCells[0] = NULL;
                cursor->monAnims[0] = NULL;
                goto next;
            }
        }
        use = res->narc == 215 ? narc : panelNarc;
        cursor->monPltts[0] = AddPlttResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_PLTT], use, res->member, FALSE, 150 + i, NNS_G2D_VRAM_TYPE_2DSUB, res->count, heapID);
        if (!SpriteTransfer_CreatePlttTransferTask(cursor->monPltts[0])) {
            GF_AssertFail();
        }
        GF2DGfxResObj_FreeData(cursor->monPltts[0]);
        cursor->monCells[0] = AddCellOrAnimResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_CELL], use, *cell, FALSE, 150 + i, GF_GFX_RES_TYPE_CELL, heapID);
        cursor->monAnims[0] = AddCellOrAnimResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_ANIM], use, *anim, FALSE, 150 + i, GF_GFX_RES_TYPE_ANIM, heapID);
next:
        res++;
        cursor = (WiFiPlazaPlayerCard *)((u8 *)cursor + 4);
        cell++;
        anim++;
    }
    pos = sWiFiPlazaCardMonPositions;
    for (i = 0; i < 12; i++) {
        int species = WiFiPlazaPlayer_GetHistoryId(player, i);
        int form = WiFiPlazaPlayer_GetHistoryValue(player, i);
        if (species == 24) {
            card->monChars[0] = NULL;
            card->monSprites[0] = NULL;
        } else {
            const WiFiPlazaCardMonInfo *info = WiFiPlazaPlayerCard_GetMonInfo(species, form, version, gender);
            card->monChars[0] = AddCharResObjFromOpenNarc(graphics->resManagers[GF_GFX_RES_TYPE_CHAR], info->source == 1 ? panelNarc : narc, info->member, FALSE, 150 + i, NNS_G2D_VRAM_TYPE_2DSUB, heapID);
            if (!SpriteTransfer_CreateCharTransferTask_AllocAtEnd(card->monChars[0])) {
                GF_AssertFail();
            }
            GF2DGfxResObj_FreeData(card->monChars[0]);
            CreateSpriteResourcesHeader(&header, 150 + i, 150 + info->source, 150 + info->source, 150 + info->source, -1, -1, FALSE, 0, graphics->resManagers[GF_GFX_RES_TYPE_CHAR], graphics->resManagers[GF_GFX_RES_TYPE_PLTT], graphics->resManagers[GF_GFX_RES_TYPE_CELL], graphics->resManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
            template.spriteList = graphics->spriteList;
            template.header = &header;
            template.priority = 0;
            template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
            template.heapID = heapID;
            template.position = *pos;
            template.position.x += (int)(info->x > 0 ? 0.5f + (float)(info->x << FX32_SHIFT) : (float)(info->x << FX32_SHIFT) - 0.5f);
            template.position.y += (int)(info->y > 0 ? 0.5f + (float)(info->y << FX32_SHIFT) : (float)(info->y << FX32_SHIFT) - 0.5f);
            card->monSprites[0] = Sprite_Create(&template);
            Sprite_SetPalOffsetRespectVramOffset(card->monSprites[0], info->palette);
        }
        pos++;
        card = (WiFiPlazaPlayerCard *)((u8 *)card + 4);
    }
    NARC_Delete(narc);
}

#pragma force_active reset

void WiFiPlazaPlayerCard_FreeMons(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics) {
    int i = 0;
    u8 *cursor = (u8 *)card;
    for (; i < 12; i++, cursor += 4) {
        WiFiPlazaPlayerCard *slot = (WiFiPlazaPlayerCard *)cursor;
        if (slot->monChars[0]) {
            Sprite_Delete(slot->monSprites[0]);
            slot->monSprites[0] = NULL;
            SpriteTransfer_DeleteCharTransferTask(slot->monChars[0]);
            DestroySingle2DGfxResObj(graphics->resManagers[GF_GFX_RES_TYPE_CHAR], slot->monChars[0]);
            slot->monChars[0] = NULL;
        }
    }
    cursor = (u8 *)card;
    for (i = 0; i < 4; i++, cursor += 4) {
        WiFiPlazaPlayerCard *slot = (WiFiPlazaPlayerCard *)cursor;
        if (slot->monPltts[0]) {
            SpriteTransfer_DeletePlttTransferTask(slot->monPltts[0]);
            DestroySingle2DGfxResObj(graphics->resManagers[GF_GFX_RES_TYPE_PLTT], slot->monPltts[0]);
            DestroySingle2DGfxResObj(graphics->resManagers[GF_GFX_RES_TYPE_CELL], slot->monCells[0]);
            DestroySingle2DGfxResObj(graphics->resManagers[GF_GFX_RES_TYPE_ANIM], slot->monAnims[0]);
            slot->monPltts[0] = NULL;
        }
    }
}

const WiFiPlazaCardMonInfo *WiFiPlazaPlayerCard_GetMonInfo(int species, int form, int version, int gender) {
    if (form == version) {
        return (const WiFiPlazaCardMonInfo *)(ov49_02269764 + gender * 8);
    }
    return (const WiFiPlazaCardMonInfo *)(ov49_022699AC + species * 8);
}

void WiFiPlazaPanelButton_Init(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID) {
    s32 i;

    AddWindow(graphics->bgConfig, &button->window, &sWiFiPlazaPanelButtonWindowTemplate);
    for (i = 0; i < 3; i++) {
        button->screenAllocs[i] = GfGfxLoader_GetScrnDataFromOpenNarc(narc, i + 0x38, FALSE, &button->screens[i], heapID);
    }
    button->touch = TouchHitboxController_Create(sWiFiPlazaPanelButtonHitbox, 1, WiFiPlazaPanelButton_OnTouch, button, heapID);
    button->disabled = TRUE;
}

void WiFiPlazaPanelButton_Free(WiFiPlazaPanelButton *button) {
    int i;
    u8 *cursor;
    TouchHitboxController_Destroy(button->touch);
    RemoveWindow(&button->window);
    i = 0;
    cursor = (u8 *)button;
    for (; i < 3; i++, cursor += 4) {
        Heap_Free(((WiFiPlazaPanelButton *)cursor)->screenAllocs[0]);
    }
    button->active = FALSE;
}

BOOL WiFiPlazaPanelButton_Update(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID) {
    BOOL pressed = FALSE;
    if (button->disabled == FALSE) {
        TouchHitboxController_IsTriggered(button->touch);
    } else {
        button->selected = 1;
    }
    if (button->active == TRUE && button->timer == 0) {
        button->active = FALSE;
        WiFiPlazaPanelButton_Draw(button, graphics, 0, narc, heapID);
    }
    if (button->selected != button->previous) {
        button->previous = button->selected;
        if (button->selected == 2) {
            WiFiPlazaPanelButton_Draw(button, graphics, 1, narc, heapID);
        } else if (button->active == FALSE) {
            WiFiPlazaPanelButton_Draw(button, graphics, 0, narc, heapID);
        } else {
            WiFiPlazaPanelButton_Draw(button, graphics, 2, narc, heapID);
        }
        if (button->active == FALSE && button->selected == 2) {
            button->active = TRUE;
            pressed = TRUE;
            button->timer = 1;
        }
    }
    return pressed;
}

void WiFiPlazaPanelButton_ShowOwnCard(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, WiFiPlazaRoomMsg *msg, struct WiFiPlazaPlayer *player, NARC *narc, enum HeapID heapID) {
    String *text;
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x37, graphics->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    button->instrumentMode = FALSE;
    button->graphic = ov45_0222AAC8(player);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, button->graphic, graphics->bgConfig, GF_BG_LYR_SUB_2, 0x140, 0, FALSE, heapID);
    WiFiPlazaInfoPanel_LoadScreen(graphics, narc, button->graphic + 0x1B, GF_BG_LYR_SUB_2, 0x140, heapID);
    text = WiFiPlazaRoomMsg_GetString(msg, 1, 0x3F);
    FillWindowPixelBuffer(&button->window, 0);
    AddTextPrinterParameterizedWithColor(&button->window, 0, text, 0, 4, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    if (button->active == FALSE) {
        WiFiPlazaPanelButton_Draw(button, graphics, 0, narc, heapID);
    } else {
        WiFiPlazaPanelButton_Draw(button, graphics, 2, narc, heapID);
    }
}

void WiFiPlazaPanelButton_ShowInstrument(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, u32 type, NARC *narc, enum HeapID heapID) {
    if (type >= 3) {
        GF_AssertFail();
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x37, graphics->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    button->instrumentMode = TRUE;
    button->graphic = type;
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, button->graphic + 0x5D, graphics->bgConfig, GF_BG_LYR_SUB_2, 0x140, 0, FALSE, heapID);
    if (button->active == FALSE) {
        WiFiPlazaPanelButton_Draw(button, graphics, 0, narc, heapID);
    } else {
        WiFiPlazaPanelButton_Draw(button, graphics, 2, narc, heapID);
    }
}

void WiFiPlazaPanelButton_Clear(WiFiPlazaPanelButton *unused, WiFiPlazaRoomGraphics *graphics) {
    BgClearTilemapBufferAndCommit(graphics->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(graphics->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(graphics->bgConfig, GF_BG_LYR_SUB_2);
    BgSetPosTextAndCommit(graphics->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 0);
}

// index 0: up, 1: pressed, 2: playing
#pragma force_active on

void WiFiPlazaPanelButton_Draw(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, int index, NARC *narc, enum HeapID heapID) {
    CopyToBgTilemapRect(graphics->bgConfig, GF_BG_LYR_SUB_1, 0, 3, 32, 21, button->screens[index]->rawData, 0, 3, 32, 32);
    ScheduleBgTilemapBufferTransfer(graphics->bgConfig, GF_BG_LYR_SUB_1);
    if (button->instrumentMode == TRUE) {
        WiFiPlazaInfoPanel_LoadScreen(graphics, narc, ov49_022696EC[index] + button->graphic * 2 + 96, GF_BG_LYR_SUB_2, 320, heapID);
    }
    ScheduleSetBgPosText(graphics->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, ov49_022696F0[index]);
}

#pragma force_active reset

void WiFiPlazaPanelButton_OnTouch(u32 index, u32 event, void *arg) {
    WiFiPlazaPanelButton *button = arg;
    button->selected = event;
    if (button->selected == 0) {
        button->selected = 2;
    } else if (button->selected == 3) {
        button->selected = 1;
    }
}

void WiFiPlazaPanelButton_ResetTimer(WiFiPlazaPanelButton *button) {
    button->timer = 0;
}

u32 WiFiPlazaRoom_GetTextSpeed(WiFiPlazaRoomWork *room) {
    return room->msgWindow.textSpeed;
}
