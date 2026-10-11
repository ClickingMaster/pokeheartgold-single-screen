#include "application/main_menu/wonder_card_album.h"

#include "global.h"

#include "constants/save_arrays.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/main_menu/main_menu.h"
#include "application/main_menu/main_menu_internal.h"
#include "application/main_menu/mystery_gift_wireless.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0247.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "main.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "save.h"
#include "save_arrays.h"
#include "scanline_effect.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"

// Main menu > Mystery Gift > "Check card": browses the three saved Wonder Cards, flips a card between its
// front and back, deletes it, or shares it with other players over DS wireless.
struct WonderCardAlbum {
    enum HeapID heapId;                        // 0x0000
    u8 commOwner[0x8C];                        // 0x0004, start of the MysteryGiftComm owner work (ov74_02229CE0)
    UnkWonderCardSubstruct_104 shareHeader;    // 0x0090, sent with shareCard
    WonderCard shareCard;                      // 0x00E0, copy of the card being shared
    u8 unk_0438[0x25C4];                       // 0x0438
    BgConfig *bgConfig;                        // 0x29FC
    MessageFormat *msgFormat;                  // 0x2A00
    MsgData *msgData;                          // 0x2A04
    Window windows[19];                        // 0x2A08, card text boxes (ov74_0223C340)
    u8 unk_2B38[0x10];                         // 0x2B38
    u32 baseTiles[19];                         // 0x2B48
    u32 unk_2B94;                              // 0x2B94
    u32 boxBaseTile;                           // 0x2B98, of the text box WonderCardAlbum_PrintTextBox is printing
    const WonderCardTextBox *box;              // 0x2B9C, the text box WonderCardAlbum_PrintTextBox is printing
    MysteryGiftSave *mysteryGift;              // 0x2BA0
    SaveData *saveData;                        // 0x2BA4
    Options *options;                          // 0x2BA8
    int frame;                                 // 0x2BAC
    u32 unk_2BB0;                              // 0x2BB0
    WonderCard *cards[NUM_SAVED_WONDER_CARDS]; // 0x2BB4
    int cardIndex;                             // 0x2BC0
    int page;                                  // 0x2BC4
    struct ListMenu *listMenu;                 // 0x2BC8
    ListMenuItem *listMenuItems;               // 0x2BCC
    Window messageWindow;                      // 0x2BD0
    Window menuWindow;                         // 0x2BE0
    int menuBaseTile;                          // 0x2BF0
    int numReceivers;                          // 0x2BF4
    u8 unk_2BF8[0x10];                         // 0x2BF8
    PlayerProfile *receivers[5];               // 0x2C08, by net id (0 unused)
    u32 receiverOrder[5];                      // 0x2C1C, order the receivers connected in
    int receiversShown;                        // 0x2C30
    int timer;                                 // 0x2C34
    u32 unk_2C38;                              // 0x2C38
    u32 receiverSeq;                           // 0x2C3C
    int nextState;                             // 0x2C40
    SpriteList *spriteList;                    // 0x2C44
    u8 unk_2C48[0x17C];                        // 0x2C48
    Sprite *buttonSprites[2];                  // 0x2DC4, share / cancel
    Sprite *monIconSprites[3];                 // 0x2DCC
    u8 unk_2DD8[sizeof(WonderCard)];           // 0x2DD8
    WonderCardFlipEffect flipEffect;           // 0x3130
    void (*callback)(WonderCardAlbum *album);  // 0x3D48, run every frame while set
    int selection;                             // 0x3D4C, 0: share, 1: cancel
    WaitingIcon *waitingIcon;                  // 0x3D50
}; // size: 0x3D54

// Overlay helpers (not in a header yet).
u32 ov74_0223567C(void);
int ov74_022360B0(void);
void ov74_022360A0(SaveData *saveData);
void ov74_02235ED0(UnkWonderCardSubstruct_104 *header, void *buffer, enum HeapID heapID);
u32 ov74_0223615C(void);
void ov74_02236128(void);
void WonderCard_SetGraphicsBanks(void);
void WonderCard_InitBgLayers(BgConfig *bgConfig);

// Declared here rather than through font.h / text.h: the card screens pass the u32 font ids of their text box
// tables unconverted (those headers type them as the u8 FontID; matching).
extern void ResetAllTextPrinters(void);
extern void LoadFontPal0(enum GFPalLoadLocation location, enum GFPalSlotOffset palSlotOffset, enum HeapID heapID);
extern u8 GetFontAttribute(u8 fontId, int attr);
extern u32 FontID_String_GetCenterAlignmentX(u32 fontId, String *string, u32 letterSpacing, u32 windowWidth);
extern u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);

// Other modules (not in a header yet).
extern void RTC_ConvertDayToDate(RTCDate *date, s32 day);
extern void WirelessStatusIcon_Create(void);
extern void sub_020358B8(void *a0);
extern void sub_02037AC0(u8 a0);
extern int sub_02037B38(u8 a0);
extern BOOL sub_02037D78(void);
extern void sub_020398D4(u32 a0, u32 a1);
extern PlayerProfile *sub_02034818(int netId);

extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_MysteryGift;
extern const MysteryGiftMenuItem ov74_0223C2D0[10];
extern const MysteryGiftMenuItem ov74_0223C2E0[];
extern const MysteryGiftMenuItem ov74_0223C2F0[];
extern const ListMenuTemplate ov74_0223C320;
extern const WonderCardTextBox ov74_0223C340[19];
extern u32 ov74_0223D0BC;

void WonderCardAlbum_LoadShareSprites(WonderCardAlbum *album) {
    WonderCardAlbum_DeleteSprites(album);
    ov74_0223563C();
    ov74_02235690();
    ov74_02235728(NARC_a_1_1_3, 19, 16, 18, 17, 0);
}

#pragma force_active on

void WonderCardAlbum_DeleteSprites(WonderCardAlbum *album) {
    if (album->buttonSprites[0] != NULL) {
        Sprite_Delete(album->buttonSprites[0]);
    }
    if (album->buttonSprites[1] != NULL) {
        Sprite_Delete(album->buttonSprites[1]);
    }
    album->buttonSprites[0] = album->buttonSprites[1] = NULL;
    if (album->monIconSprites[0] != NULL) {
        Sprite_Delete(album->monIconSprites[0]);
    }
    if (album->monIconSprites[1] != NULL) {
        Sprite_Delete(album->monIconSprites[1]);
    }
    if (album->monIconSprites[2] != NULL) {
        Sprite_Delete(album->monIconSprites[2]);
    }
    album->monIconSprites[0] = album->monIconSprites[1] = album->monIconSprites[2] = NULL;
    ov74_022359BC();
}

#pragma force_active reset

void WonderCardAlbum_CreateButtonSprites(WonderCardAlbum *album) {
    album->selection = 0;
    album->buttonSprites[0] = ov74_02235930(0, album->buttonSprites[0], 72, 168, 1);
    Sprite_SetPriority(album->buttonSprites[0], 2);
    album->buttonSprites[1] = ov74_02235930(0, album->buttonSprites[1], 184, 168, 0);
    Sprite_SetPriority(album->buttonSprites[1], 2);
}

void WonderCardAlbum_EraseWindowIfInUse(Window *window, BOOL dontCopyToVram) {
    if (WindowIsInUse(window) == TRUE) {
        EraseFrameAndWindow1(window, dontCopyToVram);
    }
}

void WonderCardAlbum_ClearWindowIfInUse(Window *window, BOOL dontCopyToVram) {
    if (WindowIsInUse(window) == TRUE) {
        ClearFrameAndWindow2(window, dontCopyToVram);
    }
}

#pragma force_active on

void WonderCardAlbum_CreateListMenu(WonderCardAlbum *album, const MysteryGiftMenuItem *items, int count, Window *window, int itemsAbove) {
    int i;
    ListMenuTemplate template;

    if (album->listMenuItems != NULL) {
        ListMenuItems_Delete(album->listMenuItems);
    }
    if (album->listMenu != NULL) {
        DestroyListMenu(album->listMenu, NULL, NULL);
    }
    album->listMenuItems = ListMenuItems_New(count, HEAP_ID_85);
    album->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_85);
    i = 0;
    if (count > 0) {
        do {
            ListMenuItems_AppendFromMsgData(album->listMenuItems, album->msgData, items->msgId, (int)items->func);
            i++;
            items++;
        } while (i < count);
    }
    DestroyMsgData(album->msgData);
    template = ov74_0223C320;
    template.items = album->listMenuItems;
    template.totalItems = count;
    template.window = window;
    album->listMenu = ListMenuInit(&template, 0, itemsAbove, HEAP_ID_85);
}

#pragma force_active reset

void WonderCardAlbum_PrintMessage(Window *window, int msgId) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_85);
    MessageFormat *msgFormat = MessageFormat_New(HEAP_ID_85);
    String *string;

    FillWindowPixelBuffer(window, GetFontAttribute(1, 6));
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, HEAP_ID_85);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, 0x010200, NULL);
    CopyWindowToVram(window);
    String_Delete(string);
    DestroyMsgData(msgData);
    MessageFormat_Delete(msgFormat);
}

#pragma force_active on

void WonderCardAlbum_LoadShareScreenBg(BgConfig *bgConfig) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 15, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, 32, HEAP_ID_85);
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 20, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, 32, HEAP_ID_85);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 21, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xC00, TRUE, HEAP_ID_85);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 22, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_85);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 32, 24, 12);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_1);
}

#pragma force_active reset

int WonderCardAlbum_SelectShare(OverlayManager *manager) {
    WonderCardAlbum *album = OverlayManager_GetData(manager);

    WonderCardAlbum_DestroyListMenu(album);
    WonderCardAlbum_PrintMessage(&album->messageWindow, msg_0247_00047);
    WonderCardAlbum_PrintTextBox(album, &album->menuWindow, 10, album->menuBaseTile);
    return 14;
}

int WonderCardAlbum_SelectTrash(OverlayManager *manager) {
    WonderCardAlbum *album = OverlayManager_GetData(manager);

    WonderCardAlbum_DestroyListMenu(album);
    if (SaveMysteryGift_HasAnyGift(album->mysteryGift, album->cardIndex) == TRUE) {
        WonderCardAlbum_PrintMessage(&album->messageWindow, msg_0247_00063);
    } else {
        WonderCardAlbum_PrintMessage(&album->messageWindow, msg_0247_00062);
    }
    WonderCardAlbum_PrintTextBox(album, &album->menuWindow, 9, album->menuBaseTile);
    return 12;
}

#pragma force_active on

int WonderCardAlbum_ConfirmTrash(OverlayManager *manager) {
    WonderCardAlbum *album = OverlayManager_GetData(manager);

    WonderCardAlbum_DestroyListMenu(album);
    WonderCardAlbum_PrintMessage(&album->messageWindow, msg_0247_00064);
    album->waitingIcon = WaitingIcon_New(&album->messageWindow, 19);
    if (SaveMysteryGift_HasAnyGift(album->mysteryGift, album->cardIndex) == TRUE) {
        SaveMysteryGift_ReceiveGiftAndClearCardByIndex(album->mysteryGift, album->cardIndex);
    } else {
        SaveMysteryGift_DeleteWonderCardByIndex(album->mysteryGift, album->cardIndex);
    }
    SaveGameNormal(album->saveData);
    WaitingIcon_Delete(album->waitingIcon);
    if (!SaveMysteryGift_HasAnyCard(album->mysteryGift)) {
        return 27;
    }
    album->cardIndex = WonderCardAlbum_FindNextCard(album, album->cardIndex, 1);
    WonderCardAlbum_PrintMessage(&album->messageWindow, msg_0247_00037);
    WonderCardAlbum_PrintTextBox(album, &album->menuWindow, 8, album->menuBaseTile);
    WonderCardAlbum_DrawCard(album, 1, 0);
    return 5;
}

#pragma force_active reset

int WonderCardAlbum_ReturnToCardMenu(OverlayManager *manager) {
    WonderCardAlbum *album = OverlayManager_GetData(manager);

    WonderCardAlbum_DestroyListMenu(album);
    WonderCardAlbum_PrintMessage(&album->messageWindow, msg_0247_00037);
    WonderCardAlbum_PrintTextBox(album, &album->menuWindow, 8, album->menuBaseTile);
    return 5;
}

#pragma force_active on

void WonderCardAlbum_DestroyListMenu(WonderCardAlbum *album) {
    ListMenuItems_Delete(album->listMenuItems);
    album->listMenuItems = NULL;
    DestroyListMenu(album->listMenu, NULL, NULL);
    album->listMenu = NULL;
    WonderCardAlbum_EraseWindowIfInUse(&album->menuWindow, FALSE);
    ClearWindowTilemapAndCopyToVram(&album->menuWindow);
    RemoveWindow(&album->menuWindow);
}

#pragma force_active reset

BOOL WonderCardAlbum_PrintMessageOnly(void) {
    return TRUE;
}

BOOL WonderCardAlbum_PrintCardTitle(WonderCardAlbum *album, Window *window, u32 color) {
    String *string = String_New(37, album->heapId);

    CopyU16ArrayToStringN(string, album->cards[album->cardIndex]->unk104.name, 36);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, color, NULL);
    String_Delete(string);
    return TRUE;
}

#pragma force_active on

BOOL WonderCardAlbum_PrintGiftStatus(WonderCardAlbum *album, Window *window, u32 color) {
    int msgId;
    int index = album->cardIndex;
    String *string;

    if (!album->cards[index]->unk104.unk8E_3) {
        msgId = msg_0247_00040;
    } else if (album->mysteryGift == NULL || SaveMysteryGift_HasAnyGift(album->mysteryGift, index) == TRUE) {
        msgId = msg_0247_00038;
    } else {
        msgId = msg_0247_00039;
    }
    string = ReadMsgData_ExpandPlaceholders(album->msgFormat, album->msgData, msgId, album->heapId);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, color, NULL);
    String_Delete(string);
    return TRUE;
}

#pragma force_active reset

BOOL WonderCardAlbum_BufferReceiveDate(WonderCardAlbum *album) {
    RTCDate date;

    RTC_ConvertDayToDate(&date, album->cards[album->cardIndex]->receiveDate);
    BufferIntegerAsString(album->msgFormat, 0, date.year + 2000, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferMonthNameAbbr(album->msgFormat, 1, date.month);
    BufferIntegerAsString(album->msgFormat, 2, date.day, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    return TRUE;
}

BOOL WonderCardAlbum_PrintCardText(WonderCardAlbum *album, Window *window, u32 color) {
    String *string = String_New(251, HEAP_ID_85);

    CopyU16ArrayToStringN(string, album->cards[album->cardIndex]->text, 250);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, color, NULL);
    String_Delete(string);
    return FALSE;
}

BOOL WonderCardAlbum_PrintSharesLeft(WonderCardAlbum *album, Window *window, u32 color) {
    u8 shareMax = album->cards[album->cardIndex]->shareMax;

    if (shareMax == 0xFF) {
        String *string = ReadMsgData_ExpandPlaceholders(album->msgFormat, album->msgData, msg_0247_00052, HEAP_ID_85);
        AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, color, NULL);
        String_Delete(string);
        return FALSE;
    }
    if (shareMax != 0) {
        BufferIntegerAsString(album->msgFormat, 0, shareMax, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL WonderCardAlbum_ShowCardMenu(WonderCardAlbum *album, Window *window, u32 color) {
    MysteryGiftMenuItem items[4];
    const WonderCardTextBox *box;
    int count;
    int shrink;

    count = 0;
    items[count++] = ov74_0223C2D0[6];
    if (album->cards[album->cardIndex]->shareMax) {
        items[count++] = ov74_0223C2D0[7];
    }
    items[count] = ov74_0223C2D0[8];
    items[count + 1] = ov74_0223C2D0[9];
    box = album->box;
    ClearWindowTilemapAndCopyToVram(window);
    RemoveWindow(window);
    shrink = (4 - (count + 2)) * 2;
    AddWindowParameterized(album->bgConfig, window, GF_BG_LYR_MAIN_2, box->x, box->y + shrink, box->width, box->height - shrink, 15, album->boxBaseTile);
    WonderCardAlbum_CreateListMenu(album, items, count + 2, window, 0);
    return TRUE;
}

#pragma force_active reset

BOOL WonderCardAlbum_ShowTrashYesNo(WonderCardAlbum *album, Window *window, u32 color) {
    WonderCardAlbum_CreateListMenu(album, ov74_0223C2D0, 2, window, 1);
    return TRUE;
}

BOOL WonderCardAlbum_ShowShareYesNo(WonderCardAlbum *album, Window *window, u32 color) {
    WonderCardAlbum_CreateListMenu(album, ov74_0223C2F0, 2, window, 0);
    return TRUE;
}

BOOL WonderCardAlbum_ShowWirelessYesNo(WonderCardAlbum *album, Window *window, u32 color) {
    WonderCardAlbum_PrintMessage(&album->messageWindow, msg_0247_00002);
    WonderCardAlbum_CreateListMenu(album, ov74_0223C2E0, 2, window, 0);
    return TRUE;
}

BOOL ov74_0222D7D0(WonderCardAlbum *album) {
    BufferIntegerAsString(album->msgFormat, 0, 0, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    return TRUE;
}

#pragma force_active on

void WonderCardAlbum_StartFade(WonderCardAlbum *album, enum FadeType fadeType, int nextState, int *state) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, fadeType, fadeType, RGB_BLACK, 6, 1, HEAP_ID_85);
    if (state != NULL) {
        *state = 28;
    }
    album->nextState = nextState;
}

void WonderCardAlbum_DrawCard(WonderCardAlbum *album, int unused, int page) {
    const WonderCardTextBox *boxes = ov74_0223C340;
    u32 i;
    u32 baseTile;
    u32 color;
    u32 x;
    String *string;

    album->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, album->heapId);
    album->msgFormat = MessageFormat_New(album->heapId);
    album->page = page;
    WonderCardAlbum_LoadCardBg(album, page);
    for (i = 0; i < 19; i++) {
        if (page != boxes[i].page && boxes[i].page != 2 && album->windows[i].bgConfig != NULL) {
            ClearWindowTilemapAndCopyToVram(&album->windows[i]);
            RemoveWindow(&album->windows[i]);
        }
    }
    baseTile = 49;
    for (i = 0; i < 19; i++) {
        if (page == boxes[i].page) {
            if (album->windows[i].bgConfig == NULL) {
                album->baseTiles[i] = baseTile;
                AddWindowParameterized(album->bgConfig, &album->windows[i], GF_BG_LYR_MAIN_0, boxes[i].x, boxes[i].y, boxes[i].width, boxes[i].height, 15, baseTile);
            }
            FillWindowPixelBuffer(&album->windows[i], GetFontAttribute(boxes[i].fontId, (u8)boxes[i].fillColor));
            color = boxes[i].color;
            if (boxes[i].func(album, &album->windows[i], color) == TRUE && boxes[i].msgId != 0) {
                string = ReadMsgData_ExpandPlaceholders(album->msgFormat, album->msgData, boxes[i].msgId, album->heapId);
                x = boxes[i].textX;
                if (x == -1) {
                    x = FontID_String_GetCenterAlignmentX(boxes[i].fontId, string, 0, boxes[i].width * 8);
                }
                AddTextPrinterParameterizedWithColor(&album->windows[i], boxes[i].fontId, string, x, boxes[i].textY, 0xFF, color, NULL);
                String_Delete(string);
            }
            CopyWindowToVram(&album->windows[i]);
            baseTile += boxes[i].width * boxes[i].height;
        }
    }
    DestroyMsgData(album->msgData);
    MessageFormat_Delete(album->msgFormat);
    if (page == 0) {
        WonderCardAlbum_CreateMonIconSprites(album);
    }
}

int WonderCardAlbum_PrintTextBox(WonderCardAlbum *album, Window *window, int boxId, int baseTile) {
    const WonderCardTextBox *box = &ov74_0223C340[boxId];
    String *string;

    if (window->bgConfig == NULL) {
        AddWindowParameterized(album->bgConfig, window, GF_BG_LYR_MAIN_2, box->x, box->y, box->width, box->height, 15, baseTile);
    }
    FillWindowPixelBuffer(window, box->fillColor);
    if (box->msgId != 0) {
        album->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_85);
        album->msgFormat = MessageFormat_New(HEAP_ID_85);
    }
    album->boxBaseTile = baseTile;
    album->box = box;
    box->func(album, window, 0x010200);
    if (box->msgId != 0) {
        string = ReadMsgData_ExpandPlaceholders(album->msgFormat, album->msgData, box->msgId, HEAP_ID_85);
        AddTextPrinterParameterizedWithColor(window, box->fontId, string, 0, 0, 0xFF, box->color, NULL);
        String_Delete(string);
        DestroyMsgData(album->msgData);
        MessageFormat_Delete(album->msgFormat);
    }
    if (window == &album->messageWindow) {
        DrawFrameAndWindow2(window, FALSE, 19, 10);
    } else {
        DrawFrameAndWindow1(window, FALSE, 10, 14);
    }
    return baseTile + box->width * box->height;
}

#pragma force_active reset

int WonderCardAlbum_FindNextCard(WonderCardAlbum *album, int index, int step) {
    int start = index;

    do {
        index += step;
        if (index == NUM_SAVED_WONDER_CARDS) {
            index = 0;
        }
        if (index == -1) {
            index = NUM_SAVED_WONDER_CARDS - 1;
        }
        if (start == index) {
            break;
        }
    } while (SaveMysteryGift_CardTagIsValid(album->mysteryGift, index) == FALSE);
    return index;
}

void WonderCardAlbum_LoadScreenData(WonderCardAlbum *album, int fileId, int bgId, int offset) {
    NNSG2dScreenData *screenData;
    void *data = GfGfxLoader_LoadFromNarc(NARC_a_1_1_3, fileId, TRUE, album->heapId, TRUE);

    NNS_G2dGetUnpackedScreenData(data, &screenData);
    BG_LoadScreenTilemapData(album->bgConfig, bgId, screenData->rawData, offset);
    Heap_Free(data);
}

void WonderCardAlbum_LoadCardBg(WonderCardAlbum *album, int page) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x100, album->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 6, album->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x3000, TRUE, album->heapId);
    switch (page) {
    case 0:
        GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 4, album->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, album->heapId);
        break;
    case 1:
        GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 5, album->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, album->heapId);
        break;
    }
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, 0x20, album->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, album->bgConfig, GF_BG_LYR_MAIN_3, 0, 0x200, TRUE, album->heapId);
    WonderCardAlbum_LoadScreenData(album, 2, GF_BG_LYR_MAIN_3, 0x600);
    BgTilemapRectChangePalette(album->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 32, 24, 11);
    BgCommitTilemapBufferToVram(album->bgConfig, GF_BG_LYR_MAIN_3);
}

void WonderCardAlbum_HandleListMenuInput(OverlayManager *manager, int *state, MysteryGiftMenuFunc cancelFunc) {
    u32 input = ListMenu_ProcessInput(((WonderCardAlbum *)OverlayManager_GetData(manager))->listMenu);
    int nextState;

    switch (input) {
    case LIST_CANCEL:
        PlaySE(SEQ_SE_DP_SELECT);
        if (cancelFunc != NULL) {
            nextState = cancelFunc(manager);
            if (nextState != -1) {
                *state = nextState;
                return;
            }
        }
        break;
    case LIST_NOTHING_CHOSEN:
        break;
    default:
        PlaySE(SEQ_SE_DP_SELECT);
        if (input != 0) {
            // small values are states, the others menu functions
            if (input < 31) {
                *state = input;
                return;
            }
            ov74_0223D0BC = input;
            nextState = ((MysteryGiftMenuFunc)input)(manager);
            if (nextState != -1) {
                *state = nextState;
            }
        }
        break;
    }
}

#pragma force_active on

void WonderCardAlbum_CreateMonIconSprites(WonderCardAlbum *album) {
    int x;
    int i;
    int species;
    void *data;
    NNSG2dCharacterData *charData;

    if (album->monIconSprites[0] == NULL && album->monIconSprites[1] == NULL && album->monIconSprites[2] == NULL) {
        if (ov74_0223567C() == 1) {
            WonderCardAlbum_DeleteSprites(album);
        }
        ov74_0223563C();
        ov74_02235690();
        ov74_02235728(NARC_a_1_1_3, 30, 27, 29, 28, 0);
        GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, (enum GFPalSlotOffset)0x60, 0, album->heapId);
    }
    x = 178;
    for (i = 0; i < 3; i++, x += 25) {
        species = album->cards[album->cardIndex]->monIcon[i];
        if (species == SPECIES_NONE) {
            if (album->monIconSprites[i] != NULL) {
                Sprite_SetDrawFlag(album->monIconSprites[i], FALSE);
            }
        } else {
            album->monIconSprites[i] = ov74_02235930(0, album->monIconSprites[i], x, 16, i + 10);
            data = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(species, FALSE, 0), FALSE, &charData, album->heapId);
            DC_FlushRange(charData->pRawData, 0x200);
            GX_LoadOBJ(charData->pRawData, (100 + i * 16) * 32, 0x200);
            Sprite_SetPaletteOverride(album->monIconSprites[i], GetMonIconPaletteEx(species, 0, FALSE) + 3);
            Heap_Free(data);
        }
    }
}

#pragma force_active reset

BOOL WonderCardAlbum_Init(OverlayManager *manager, int *state) {
    WonderCardAlbum *album;

    Heap_Create(HEAP_ID_3, HEAP_ID_85, 0x30000);
    album = OverlayManager_CreateAndGetData(manager, sizeof(WonderCardAlbum), HEAP_ID_85);
    memset(album, 0, sizeof(WonderCardAlbum));
    album->bgConfig = BgConfig_Alloc(HEAP_ID_85);
    album->heapId = HEAP_ID_85;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    album->cardIndex = 2;
    album->receiverSeq = 1;
    ov74_022352A0(HEAP_ID_85);
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570);
    return TRUE;
}

void WonderCardAlbum_FlipEffectTask(SysTask *task, WonderCardFlipEffect *effect) {
    void *buffer;

    HBlankDma_Stop();
    buffer = DoubleBuffer_GetOther(effect->doubleBuffer);
    HBlankDma_Start(buffer, (void *)REG_BG0HOFS_ADDR, 8, 1);
}

#pragma force_active on

void WonderCardAlbum_StartFlipEffect(WonderCardAlbum *album, int mode, fx32 speed, fx32 accel) {
    WonderCardFlipEffect *effect = &album->flipEffect;

    effect->mode = mode;
    effect->speed = speed;
    effect->accel = accel;
    if (effect->doubleBuffer == NULL) {
        effect->doubleBuffer = DoubleBuffer_New(HEAP_ID_85, effect->buffers[0], effect->buffers[1]);
    }
    if (effect->task == NULL) {
        effect->task = SysTask_CreateOnVBlankQueue((SysTaskFunc)WonderCardAlbum_FlipEffectTask, effect, 1024);
    }
    effect->active = TRUE;
}

#pragma force_active reset

void WonderCardAlbum_StopFlipEffect(WonderCardAlbum *album) {
    WonderCardFlipEffect *effect = &album->flipEffect;

    if (effect->doubleBuffer != NULL) {
        DoubleBuffer_Delete(effect->doubleBuffer);
    }
    if (effect->task != NULL) {
        SysTask_Destroy(effect->task);
    }
    effect->doubleBuffer = NULL;
    effect->task = NULL;
    HBlankDma_Stop();
}

#pragma force_active on

BOOL WonderCardAlbum_UpdateFlipEffect(WonderCardAlbum *album) {
    WonderCardFlipEffect *effect = &album->flipEffect;
    fx32 pos = 84 << FX32_SHIFT;
    int i;
    int x;
    u16 *buffer;

    if (effect->active == FALSE) {
        return TRUE;
    }
    if (effect->mode == 1) {
        effect->speed += effect->accel;
        effect->accel += effect->accel;
        if (effect->speed / FX32_ONE > 1000) {
            effect->active = FALSE;
            effect->speed = 0;
            pos = 168;
            effect->accel = 0;
        }
    } else {
        effect->speed -= effect->accel;
        effect->accel /= 2;
        if (effect->accel < 1024) {
            effect->active = FALSE;
            effect->speed = FX32_ONE;
            effect->accel = 0;
        }
    }
    buffer = DoubleBuffer_GetCurrent(effect->doubleBuffer);
    for (i = 84; i < 168; i++) {
        x = pos / FX32_ONE;
        if (x < 0) {
            x = 0;
        }
        if (x > 168) {
            x = 168;
        }
        buffer[i * 4 + 3] = x - i;
        buffer[i * 4 + 1] = buffer[i * 4 + 3];
        buffer[(168 - i) * 4 + 3] = i - x;
        buffer[(168 - i) * 4 + 1] = buffer[(168 - i) * 4 + 3];
        pos += effect->speed;
    }
    DC_FlushRange(buffer, 0x600);
    DoubleBuffer_Swap(effect->doubleBuffer);
    return FALSE;
}

#pragma force_active reset

void WonderCardAlbum_WaitForSave(WonderCardAlbum *album) {
    int status = ov74_022360B0();

    if (status == 2 || status == 3) {
        PlaySE(SEQ_SE_DP_SAVE);
        album->callback = NULL;
    }
}

void WonderCardAlbum_CountShare(WonderCardAlbum *album) {
    GF_ASSERT(album->cardIndex < NUM_SAVED_WONDER_CARDS);
    SaveSubstruct_AssertCRC(SAVE_MYSTERY_GIFT);
    if (album->cards[album->cardIndex]->shareCount != 0xFF) {
        album->cards[album->cardIndex]->shareCount++;
    }
    if (album->cards[album->cardIndex]->shareMax != 0xFF) {
        album->cards[album->cardIndex]->shareMax--;
    }
    SaveSubstruct_UpdateCRC(SAVE_MYSTERY_GIFT);
    ov74_022360A0(album->saveData);
    album->callback = WonderCardAlbum_WaitForSave;
}

void WonderCardAlbum_HandleShareInput(WonderCardAlbum *album, BOOL canShare, int *state) {
    int prevSelection = album->selection;
    int action;

    if ((gSystem.newKeys & PAD_KEY_RIGHT) && prevSelection != 1) {
        album->selection = 1;
    }
    if ((gSystem.newKeys & PAD_KEY_LEFT) && album->selection != 0) {
        album->selection = 0;
    }
    if (prevSelection != album->selection) {
        Sprite_SetAnimCtrlSeq(album->buttonSprites[0], album->selection == 0);
        Sprite_SetAnimCtrlSeq(album->buttonSprites[1], album->selection != 0);
    }
    action = 0;
    if (gSystem.newKeys & PAD_BUTTON_B) {
        action = 2;
    } else if ((gSystem.newKeys & PAD_BUTTON_A) && canShare && album->selection == 0) {
        action = 1;
    } else if ((gSystem.newKeys & PAD_BUTTON_A) && album->selection == 1) {
        action = 2;
    } else if ((gSystem.newKeys & PAD_BUTTON_A) && !canShare) {
        action = 3;
    }
    if (action == 1) {
        PlaySE(SEQ_SE_DP_SELECT);
        sub_02037AC0(171);
        album->timer = 1;
        *state = 22;
        WonderCardAlbum_PrintTextBox(album, &album->messageWindow, 17, 640);
        album->waitingIcon = WaitingIcon_New(&album->messageWindow, 19);
    }
    if (action == 2) {
        PlaySE(SEQ_SE_DP_SELECT);
        WonderCardAlbum_StopSharing(album, state, 20);
    }
    if (action == 3) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

BOOL WonderCardAlbum_Main(OverlayManager *manager, int *state) {
    WonderCardAlbum *album = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        album->saveData = ((UnkStruct_02111868_sub *)OverlayManager_GetArgs(manager))->saveData;
        album->mysteryGift = Save_MysteryGift_Get(album->saveData);
        album->options = Save_PlayerData_GetOptionsAddr(album->saveData);
        album->frame = Options_GetFrame(album->options);
        album->cards[0] = SaveMysteryGift_CardGetByIdx(album->mysteryGift, 0);
        album->cards[1] = SaveMysteryGift_CardGetByIdx(album->mysteryGift, 1);
        album->cards[2] = SaveMysteryGift_CardGetByIdx(album->mysteryGift, 2);
        album->cardIndex = WonderCardAlbum_FindNextCard(album, album->cardIndex, 1);
        *state = 1;
        break;
    case 1:
        WonderCard_SetGraphicsBanks();
        WonderCard_InitBgLayers(album->bgConfig);
        *state = 2;
        break;
    case 2:
        ResetAllTextPrinters();
        WonderCardAlbum_LoadCardBg(album, 0);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_85);
        LoadUserFrameGfx1(album->bgConfig, GF_BG_LYR_MAIN_0, 1, 13, 0, HEAP_ID_85);
        LoadUserFrameGfx1(album->bgConfig, GF_BG_LYR_MAIN_0, 10, 14, 1, HEAP_ID_85);
        LoadUserFrameGfx2(album->bgConfig, GF_BG_LYR_MAIN_0, 19, 10, album->frame, HEAP_ID_85);
        WonderCardAlbum_DrawCard(album, 1, 0);
        WonderCardAlbum_StartFade(album, FADE_TYPE_BRIGHTNESS_IN, 3, state);
        break;
    case 3: {
        int index = album->cardIndex;
        if (gSystem.newKeys & PAD_KEY_UP) {
            index = WonderCardAlbum_FindNextCard(album, index, -1);
        } else if (gSystem.newKeys & PAD_KEY_DOWN) {
            index = WonderCardAlbum_FindNextCard(album, index, 1);
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            WonderCardAlbum_StartFade(album, FADE_TYPE_BRIGHTNESS_OUT, 29, state);
        } else if (gSystem.newKeys & PAD_BUTTON_A) {
            PlaySE(SEQ_SE_DP_SELECT);
            *state = 4;
        }
        if (album->cardIndex != index) {
            PlaySE(SEQ_SE_DP_CARD2);
            album->cardIndex = index;
            WonderCardAlbum_DrawCard(album, 1, 0);
        }
        break;
    }
    case 4:
        album->menuBaseTile = WonderCardAlbum_PrintTextBox(album, &album->messageWindow, 7, 640);
        WonderCardAlbum_PrintTextBox(album, &album->menuWindow, 8, album->menuBaseTile);
        *state = 5;
        break;
    case 5:
        WonderCardAlbum_HandleListMenuInput(manager, state, NULL);
        if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            *state = 11;
        }
        break;
    case 6:
        PlaySE(SEQ_SE_DP_CARD2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        WonderCardAlbum_ClearWindowIfInUse(&album->messageWindow, FALSE);
        WonderCardAlbum_EraseWindowIfInUse(&album->menuWindow, FALSE);
        WonderCardAlbum_StartFlipEffect(album, 1, FX32_ONE, 0x66);
        *state = 7;
        break;
    case 7:
        if (WonderCardAlbum_UpdateFlipEffect(album)) {
            WonderCardAlbum_DrawCard(album, 1, 1);
            WonderCardAlbum_StartFlipEffect(album, 0, 0x708000, 0x708000U >> 1);
            *state = 8;
        }
        break;
    case 8:
        WonderCardAlbum_UpdateFlipEffect(album);
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            PlaySE(SEQ_SE_DP_CARD2);
            WonderCardAlbum_StartFlipEffect(album, 1, FX32_ONE, 0x66);
            *state = 9;
        }
        break;
    case 9:
        if (WonderCardAlbum_UpdateFlipEffect(album)) {
            WonderCardAlbum_DrawCard(album, 1, 0);
            WonderCardAlbum_StartFlipEffect(album, 0, 0x708000, 0x708000U >> 1);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            *state = 10;
        }
        break;
    case 10:
        if (WonderCardAlbum_UpdateFlipEffect(album)) {
            DrawFrameAndWindow2(&album->messageWindow, FALSE, 19, 10);
            DrawFrameAndWindow1(&album->menuWindow, FALSE, 10, 14);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            WonderCardAlbum_StopFlipEffect(album);
            *state = 5;
        }
        break;
    case 11:
        WonderCardAlbum_DestroyListMenu(album);
        WonderCardAlbum_ClearWindowIfInUse(&album->messageWindow, FALSE);
        ClearWindowTilemapAndCopyToVram(&album->messageWindow);
        RemoveWindow(&album->messageWindow);
        *state = 3;
        break;
    case 12:
        WonderCardAlbum_HandleListMenuInput(manager, state, WonderCardAlbum_ReturnToCardMenu);
        break;
    case 14:
        WonderCardAlbum_HandleListMenuInput(manager, state, WonderCardAlbum_ReturnToCardMenu);
        break;
    case 15:
        WonderCardAlbum_ShowWirelessYesNo(album, &album->menuWindow, 0x010200);
        *state = 14;
        break;
    case 16:
        WonderCardAlbum_StartFade(album, FADE_TYPE_BRIGHTNESS_OUT, 17, state);
        break;
    case 17:
        WonderCardAlbum_DestroyListMenu(album);
        WonderCardAlbum_ClearWindowIfInUse(&album->messageWindow, FALSE);
        ClearWindowTilemapAndCopyToVram(&album->messageWindow);
        RemoveWindow(&album->messageWindow);
        WonderCardAlbum_DrawCard(album, 0, 3);
        WonderCardAlbum_LoadShareScreenBg(album->bgConfig);
        WonderCardAlbum_LoadShareSprites(album);
        WonderCardAlbum_CreateButtonSprites(album);
        *state = 18;
        break;
    case 18:
        WonderCardAlbum_StartSharing(album);
        WirelessStatusIcon_Create();
        WonderCardAlbum_StartFade(album, FADE_TYPE_BRIGHTNESS_IN, 19, state);
        break;
    case 19: {
        int numReceivers = 0;
        if (Comm_IsNetIdConnected(0)) {
            sub_020358B8(&album->shareHeader);
            numReceivers = WonderCardAlbum_UpdateReceiverList(album, &album->windows[16]);
            if (numReceivers == 0 && album->receiversShown != 0) {
                FillWindowPixelBuffer(&album->windows[16], 0);
                CopyWindowToVram(&album->windows[16]);
                album->receiversShown = 0;
            }
            WonderCardAlbum_PrintReceiverCount(album, &album->windows[15], numReceivers);
            album->receiversShown = numReceivers;
        }
        WonderCardAlbum_HandleShareInput(album, numReceivers, state);
        break;
    }
    case 20:
        WonderCardAlbum_StartFade(album, FADE_TYPE_BRIGHTNESS_OUT, 21, state);
        break;
    case 21:
        WonderCardAlbum_DeleteSprites(album);
        WonderCardAlbum_ClearWindowIfInUse(&album->messageWindow, FALSE);
        WonderCardAlbum_EraseWindowIfInUse(&album->menuWindow, FALSE);
        WonderCardAlbum_DrawCard(album, 1, 0);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_85);
        WonderCardAlbum_StartFade(album, FADE_TYPE_BRIGHTNESS_IN, 4, state);
        break;
    case 23:
        if (!sub_02037D78()) {
            *state = album->nextState;
        }
        break;
    case 22:
        album->timer--;
        if (album->timer == 0) {
            ov74_02235ED0(&album->shareHeader, album->unk_2DD8, album->heapId);
            ov74_02229D0C(album->unk_2DD8, (void *)sizeof(WonderCard));
            WonderCardAlbum_CountShare(album);
            *state = 24;
        }
        break;
    case 24:
        if (ov74_0223615C() == 4) {
            sub_02037AC0(147);
            sub_020398D4(1, 1);
            *state = 25;
        }
        break;
    case 25:
        if (WonderCardAlbum_CountReceivers() == 0 || sub_02037B38(147) == TRUE) {
            sub_020398D4(0, 0);
            ov74_02236128();
            WonderCardAlbum_PrintTextBox(album, &album->messageWindow, 18, 640);
            WaitingIcon_Delete(album->waitingIcon);
            WonderCardAlbum_StopSharing(album, state, 26);
        }
        break;
    case 26:
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            *state = 20;
        }
        break;
    case 13:
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            WonderCardAlbum_ClearWindowIfInUse(&album->messageWindow, FALSE);
            ClearWindowTilemapAndCopyToVram(&album->messageWindow);
            RemoveWindow(&album->messageWindow);
            *state = 3;
        }
        break;
    case 27:
        WonderCardAlbum_StartFade(album, FADE_TYPE_BRIGHTNESS_OUT, 29, state);
        break;
    case 28:
        if (IsPaletteFadeFinished()) {
            *state = album->nextState;
        }
        break;
    case 29:
        WonderCardAlbum_DeleteSprites(album);
        WonderCardAlbum_StopFlipEffect(album);
        return TRUE;
    case 30:
        break;
    }
    if (album->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(album->spriteList);
    }
    ov74_022358BC();
    if (album->callback != NULL) {
        album->callback(album);
    }
    return FALSE;
}

int WonderCardAlbum_StartSharing(WonderCardAlbum *album) {
    WonderCard *card = album->cards[album->cardIndex];

    memcpy(&album->shareCard, card, sizeof(WonderCard));
    memcpy(&album->shareHeader, &card->unk104, sizeof(UnkWonderCardSubstruct_104));
    album->shareCard.shareMax = 0;
    album->shareHeader.unk8E_4 = 0;
    album->shareHeader.unk8E_5 = 1;
    ov74_02229CE0(album->commOwner, album->saveData, (void *)15);
    return 0;
}

// Returns 1 + the index of the smallest of values[0..3] and replaces it with a value larger than all others.
int WonderCardAlbum_TakeEarliest(s32 *values) {
    s32 *minimum = &values[0];
    s32 minimumValue = values[0];
    int result = 0;

    if (values[1] < minimumValue) {
        minimumValue = values[1];
        result = 1;
        minimum = &values[1];
    }
    if (values[2] < minimumValue) {
        minimum = values;
        minimumValue = values[2];
        result = 2;
        minimum += 2;
    }
    if (values[3] < minimumValue) {
        minimum = values;
        result = 3;
        minimum += 3;
    }
    *minimum = 0x3FFF0001;
    return result + 1;
}

int WonderCardAlbum_CountReceivers(void) {
    int i;
    int count = 0;

    for (i = 1; i < 5; i++) {
        if (sub_02034818(i) != NULL) {
            count++;
        }
    }
    return count;
}

#pragma force_active on

// Updates the list of connected receivers; reprints it in connection order when it changed. Returns their count.
int WonderCardAlbum_UpdateReceiverList(WonderCardAlbum *album, Window *window) {
    int count = 0;
    int changes = 0;
    int i;
    u32 order[4];
    int netIds[4];
    MessageFormat *msgFormat;
    String *string;
    MsgData *msgData;
    PlayerProfile *profile;
    int y;

    for (i = 1; i < 5; i++) {
        profile = sub_02034818(i);
        if (profile == NULL) {
            if (album->receivers[i] != NULL) {
                changes++;
            }
            album->receivers[i] = NULL;
            album->receiverOrder[i] = 0x3FFF0001;
        } else {
            if (album->receivers[i] != profile) {
                changes++;
                album->receivers[i] = profile;
                album->receiverOrder[i] = album->receiverSeq++;
                count++;
            } else {
                count++;
            }
        }
    }
    if (!changes) {
        return count;
    }
    order[0] = album->receiverOrder[1];
    order[1] = album->receiverOrder[2];
    order[2] = album->receiverOrder[3];
    order[3] = album->receiverOrder[4];
    netIds[0] = WonderCardAlbum_TakeEarliest((s32 *)order);
    netIds[1] = WonderCardAlbum_TakeEarliest((s32 *)order);
    netIds[2] = WonderCardAlbum_TakeEarliest((s32 *)order);
    netIds[3] = WonderCardAlbum_TakeEarliest((s32 *)order);
    msgFormat = MessageFormat_New(HEAP_ID_85);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_85);
    y = 0;
    FillWindowPixelBuffer(window, 0);
    for (i = 0; i < count; i++) {
        profile = sub_02034818(netIds[i]);
        if (profile != NULL) {
            BufferPlayersName(msgFormat, 0, profile);
            string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0247_00054, HEAP_ID_85);
            if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
                AddTextPrinterParameterizedWithColor(window, 0, string, 0, y, 0xFF, 0x050600, NULL);
            } else {
                AddTextPrinterParameterizedWithColor(window, 0, string, 0, y, 0xFF, 0x030400, NULL);
            }
            String_Delete(string);
            BufferIntegerAsString(msgFormat, 0, (u16)PlayerProfile_GetTrainerID(profile), 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
            string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0247_00055, HEAP_ID_85);
            AddTextPrinterParameterizedWithColor(window, 0, string, 80, y, 0xFF, 0x0E0F00, NULL);
            String_Delete(string);
            y += 24;
        }
    }
    if (count) {
        CopyWindowToVram(window);
    }
    DestroyMsgData(msgData);
    MessageFormat_Delete(msgFormat);
    return count;
}

void WonderCardAlbum_PrintReceiverCount(WonderCardAlbum *album, Window *window, int count) {
    String *string;

    album->numReceivers = count;
    album->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_85);
    album->msgFormat = MessageFormat_New(HEAP_ID_85);
    FillWindowPixelBuffer(window, 0);
    BufferIntegerAsString(album->msgFormat, 0, album->numReceivers, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(album->msgFormat, album->msgData, msg_0247_00057, HEAP_ID_85);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, 0x010200, NULL);
    CopyWindowToVram(window);
    String_Delete(string);
    DestroyMsgData(album->msgData);
    MessageFormat_Delete(album->msgFormat);
}

#pragma force_active reset

void WonderCardAlbum_StopSharing(WonderCardAlbum *album, int *state, int nextState) {
    MysteryGiftComm_Free();
    WirelessStatusIcon_Destroy();
    album->nextState = nextState;
    *state = 23;
}

#pragma force_active on

BOOL WonderCardAlbum_Exit(OverlayManager *manager) {
    WonderCardAlbum *album = OverlayManager_GetData(manager);
    u32 i;

    for (i = 0; i < 19; i++) {
        if (album->windows[i].bgConfig != NULL) {
            ClearWindowTilemapAndCopyToVram(&album->windows[i]);
            RemoveWindow(&album->windows[i]);
        }
    }
    if (album->messageWindow.bgConfig != NULL) {
        ClearWindowTilemapAndCopyToVram(&album->messageWindow);
        RemoveWindow(&album->messageWindow);
    }
    if (album->menuWindow.bgConfig != NULL) {
        ClearWindowTilemapAndCopyToVram(&album->menuWindow);
        RemoveWindow(&album->menuWindow);
    }
    FreeBgTilemapBuffer(album->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(album->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(album->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(album->bgConfig, GF_BG_LYR_MAIN_3);
    Heap_Free(album->bgConfig);
    RegisterMainOverlay(FS_OVERLAY_ID(OVY_74), &gApp_MainMenu_SelectOption_MysteryGift);
    Heap_Destroy(HEAP_ID_89);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_85);
    return TRUE;
}

// Draws a Wonder Card on bgConfig with a temporary album work (used by the Mystery Gift app).
void WonderCardAlbum_DrawCardOnBg(BgConfig *bgConfig, WonderCard *card, enum HeapID heapID) {
    WonderCardAlbum *album;

    WonderCard_InitBgLayers(bgConfig);
    album = Heap_AllocAtEnd(heapID, sizeof(WonderCardAlbum));
    memset(album, 0, sizeof(WonderCardAlbum));
    album->bgConfig = bgConfig;
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, album->heapId = heapID);
    album->cards[0] = card;
    album->cardIndex = 0;
    WonderCardAlbum_DrawCard(album, 1, 0);
    Heap_Free(album);
}

#pragma force_active reset
