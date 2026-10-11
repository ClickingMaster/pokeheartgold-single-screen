#include "application/main_menu/lock_capsule_card.h"

#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/main_menu/main_menu.h"
#include "application/main_menu/main_menu_internal.h"
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

// Main menu > Mystery Gift > the Lock Capsule card (the special Wonder Card, RECEIVED_WONDER_CARD_IDX): shows the card and flips it
// between its front and back. Same layout as the Wonder Card album without the sharing part. Also holds the
// screen setup shared by both card screens and the Mystery Gift app, and the check whether a gift can be received.
struct LockCapsuleCardView {
    enum HeapID heapId;                          // 0x0000
    u8 unk_0004[0x29F8];                         // 0x0004
    BgConfig *bgConfig;                          // 0x29FC
    MessageFormat *msgFormat;                    // 0x2A00
    MsgData *msgData;                            // 0x2A04
    Window windows[11];                          // 0x2A08, card text boxes (ov74_0223C700)
    u8 unk_2AB8[0x90];                           // 0x2AB8
    u32 baseTiles[11];                           // 0x2B48
    u8 unk_2B74[0x24];                           // 0x2B74
    u32 boxBaseTile;                             // 0x2B98, of the text box LockCapsuleCard_PrintTextBox is printing
    const WonderCardTextBox *box;                // 0x2B9C, the text box LockCapsuleCard_PrintTextBox is printing
    MysteryGiftSave *mysteryGift;                // 0x2BA0
    SaveData *saveData;                          // 0x2BA4
    Options *options;                            // 0x2BA8
    int frame;                                   // 0x2BAC
    u32 unk_2BB0;                                // 0x2BB0
    WonderCard *card;                            // 0x2BB4
    int page;                                    // 0x2BB8
    struct ListMenu *listMenu;                   // 0x2BBC
    ListMenuItem *listMenuItems;                 // 0x2BC0
    Window messageWindow;                        // 0x2BC4
    Window menuWindow;                           // 0x2BD4
    int menuBaseTile;                            // 0x2BE4
    u8 unk_2BE8[0xC];                            // 0x2BE8
    u32 unk_2BF4;                                // 0x2BF4
    int nextState;                               // 0x2BF8
    SpriteList *spriteList;                      // 0x2BFC
    u8 unk_2C00[0x17C];                          // 0x2C00
    Sprite *buttonSprites[2];                    // 0x2D7C
    Sprite *monIconSprites[3];                   // 0x2D84
    u8 unk_2D90[sizeof(WonderCard)];             // 0x2D90
    WonderCardFlipEffect flipEffect;             // 0x30E8
    void (*callback)(LockCapsuleCardView *view); // 0x3D00, run every frame while set
    int selection;                               // 0x3D04
    WaitingIcon *waitingIcon;                    // 0x3D08
}; // size: 0x3D0C

// Overlay helpers (not in a header yet).
u32 ov74_0223567C(void);

// Declared here rather than through font.h / text.h: the card screens pass the u32 font ids of their text box
// tables unconverted (those headers type them as the u8 FontID; matching).
extern void ResetAllTextPrinters(void);
extern void LoadFontPal0(enum GFPalLoadLocation location, enum GFPalSlotOffset palSlotOffset, enum HeapID heapID);
extern u8 GetFontAttribute(u8 fontId, int attr);
extern u32 FontID_String_GetCenterAlignmentX(u32 fontId, String *string, u32 letterSpacing, u32 windowWidth);
extern u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);

// Other modules (not in a header yet).
extern void RTC_ConvertDayToDate(RTCDate *date, s32 day);
extern BOOL sub_02037D78(void);
extern void OS_ResetSystem(u32 parameter);

extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_MysteryGift;
extern const MysteryGiftMenuItem ov74_0223C6D0[2];
extern const ListMenuTemplate ov74_0223C6E0;
extern const WonderCardTextBox ov74_0223C700[11];
extern u32 ov74_0223D0C0;
extern const GraphicsModes _0223B430;
extern const BgTemplate _0223B440;
extern const BgTemplate _0223B45C;
extern const BgTemplate _0223B478;
extern const BgTemplate _0223B494;
extern const GraphicsBanks _0223B4B0;

#pragma force_active on

void LockCapsuleCard_DeleteSprites(LockCapsuleCardView *view) {
    if (view->buttonSprites[0] != NULL) {
        Sprite_Delete(view->buttonSprites[0]);
    }
    if (view->buttonSprites[1] != NULL) {
        Sprite_Delete(view->buttonSprites[1]);
    }
    view->buttonSprites[0] = view->buttonSprites[1] = NULL;
    if (view->monIconSprites[0] != NULL) {
        Sprite_Delete(view->monIconSprites[0]);
    }
    if (view->monIconSprites[1] != NULL) {
        Sprite_Delete(view->monIconSprites[1]);
    }
    if (view->monIconSprites[2] != NULL) {
        Sprite_Delete(view->monIconSprites[2]);
    }
    view->monIconSprites[0] = view->monIconSprites[1] = view->monIconSprites[2] = NULL;
    ov74_022359BC();
}

#pragma force_active reset

void LockCapsuleCard_EraseWindowIfInUse(Window *window, BOOL dontCopyToVram) {
    if (WindowIsInUse(window) == TRUE) {
        EraseFrameAndWindow1(window, dontCopyToVram);
    }
}

void LockCapsuleCard_ClearWindowIfInUse(Window *window, BOOL dontCopyToVram) {
    if (WindowIsInUse(window) == TRUE) {
        ClearFrameAndWindow2(window, dontCopyToVram);
    }
}

#pragma force_active on

void LockCapsuleCard_CreateListMenu(LockCapsuleCardView *view, const MysteryGiftMenuItem *items, int count, Window *window, int itemsAbove) {
    int i;
    ListMenuTemplate template;

    if (view->listMenuItems != NULL) {
        ListMenuItems_Delete(view->listMenuItems);
    }
    if (view->listMenu != NULL) {
        DestroyListMenu(view->listMenu, NULL, NULL);
    }
    view->listMenuItems = ListMenuItems_New(count, HEAP_ID_85);
    view->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_85);
    for (i = 0; i < count; i++) {
        ListMenuItems_AppendFromMsgData(view->listMenuItems, view->msgData, items[i].msgId, (int)items[i].func);
    }
    DestroyMsgData(view->msgData);
    template = ov74_0223C6E0;
    template.items = view->listMenuItems;
    template.totalItems = count;
    template.window = window;
    view->listMenu = ListMenuInit(&template, 0, itemsAbove, HEAP_ID_85);
}

void LockCapsuleCard_DestroyListMenu(LockCapsuleCardView *view) {
    ListMenuItems_Delete(view->listMenuItems);
    view->listMenuItems = NULL;
    DestroyListMenu(view->listMenu, NULL, NULL);
    view->listMenu = NULL;
    LockCapsuleCard_EraseWindowIfInUse(&view->menuWindow, FALSE);
    ClearWindowTilemapAndCopyToVram(&view->menuWindow);
    RemoveWindow(&view->menuWindow);
}

#pragma force_active reset

BOOL LockCapsuleCard_PrintMessageOnly(void) {
    return TRUE;
}

BOOL LockCapsuleCard_PrintCardTitle(LockCapsuleCardView *view, Window *window, u32 color) {
    String *string = String_New(37, view->heapId);

    CopyU16ArrayToStringN(string, view->card->unk104.name, 36);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, color, NULL);
    String_Delete(string);
    return TRUE;
}

BOOL LockCapsuleCard_PrintStatus(LockCapsuleCardView *view, Window *window, u32 color) {
    int msgId = view->card->shareCount == 0 ? msg_0247_00081 : msg_0247_00082;
    String *string = ReadMsgData_ExpandPlaceholders(view->msgFormat, view->msgData, msgId, view->heapId);

    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, color, NULL);
    String_Delete(string);
    return TRUE;
}

#pragma force_active on

BOOL LockCapsuleCard_BufferReceiveDate(LockCapsuleCardView *view) {
    RTCDate date;

    RTC_ConvertDayToDate(&date, view->card->receiveDate);
    BufferIntegerAsString(view->msgFormat, 0, date.year + 2000, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(view->msgFormat, 1, date.month, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(view->msgFormat, 2, date.day, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    return TRUE;
}

#pragma force_active reset

BOOL LockCapsuleCard_PrintCardText(LockCapsuleCardView *view, Window *window, u32 color) {
    String *string = String_New(251, HEAP_ID_85);

    CopyU16ArrayToStringN(string, view->card->text, 250);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xFF, color, NULL);
    String_Delete(string);
    return FALSE;
}

BOOL LockCapsuleCard_PrintNothing(void) {
    return FALSE;
}

#pragma force_active on

BOOL LockCapsuleCard_ShowCardMenu(LockCapsuleCardView *view, Window *window, u32 color) {
    MysteryGiftMenuItem items[2];
    const WonderCardTextBox *box;

    items[0] = ov74_0223C6D0[0];
    items[1] = ov74_0223C6D0[1];
    box = view->box;

    ClearWindowTilemapAndCopyToVram(window);
    RemoveWindow(window);
    AddWindowParameterized(view->bgConfig, window, GF_BG_LYR_MAIN_2, box->x, box->y, box->width, box->height, 15, view->boxBaseTile);
    LockCapsuleCard_CreateListMenu(view, items, 2, window, 0);
    return TRUE;
}

#pragma force_active reset

void LockCapsuleCard_StartFade(LockCapsuleCardView *view, enum FadeType fadeType, int nextState, int *state) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, fadeType, fadeType, RGB_BLACK, 6, 1, HEAP_ID_85);
    if (state != NULL) {
        *state = 28;
    }
    view->nextState = nextState;
}

#pragma force_active on

void LockCapsuleCard_DrawCard(LockCapsuleCardView *view, int unused, int page) {
    const WonderCardTextBox *boxes = ov74_0223C700;
    u32 i;
    u32 baseTile;
    u32 color;
    String *string;

    view->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, view->heapId);
    view->msgFormat = MessageFormat_New(view->heapId);
    view->page = page;
    LockCapsuleCard_LoadCardBg(view, page);
    for (i = 0; i < 11; i++) {
        if (page != boxes[i].page && boxes[i].page != 2 && view->windows[i].bgConfig != NULL) {
            ClearWindowTilemapAndCopyToVram(&view->windows[i]);
            RemoveWindow(&view->windows[i]);
        }
    }
    baseTile = 49;
    for (i = 0; i < 11; i++) {
        if (page == boxes[i].page) {
            if (view->windows[i].bgConfig == NULL) {
                view->baseTiles[i] = baseTile;
                AddWindowParameterized(view->bgConfig, &view->windows[i], GF_BG_LYR_MAIN_0, boxes[i].x, boxes[i].y, boxes[i].width, boxes[i].height, 15, baseTile);
            }
            FillWindowPixelBuffer(&view->windows[i], GetFontAttribute(boxes[i].fontId, (u8)boxes[i].fillColor));
            color = boxes[i].color;
            if (boxes[i].func(view, &view->windows[i], color) == TRUE && boxes[i].msgId != 0) {
                string = ReadMsgData_ExpandPlaceholders(view->msgFormat, view->msgData, boxes[i].msgId, view->heapId);
                AddTextPrinterParameterizedWithColor(&view->windows[i], boxes[i].fontId, string, boxes[i].textX, boxes[i].textY, 0xFF, color, NULL);
                String_Delete(string);
            }
            CopyWindowToVram(&view->windows[i]);
            baseTile += boxes[i].width * boxes[i].height;
        }
    }
    DestroyMsgData(view->msgData);
    MessageFormat_Delete(view->msgFormat);
    if (page == 0) {
        LockCapsuleCard_CreateMonIconSprites(view);
    }
}

int LockCapsuleCard_PrintTextBox(LockCapsuleCardView *view, Window *window, int boxId, int baseTile) {
    const WonderCardTextBox *box = &ov74_0223C700[boxId];

    if (window->bgConfig == NULL) {
        AddWindowParameterized(view->bgConfig, window, GF_BG_LYR_MAIN_2, box->x, box->y, box->width, box->height, 15, baseTile);
    }
    FillWindowPixelBuffer(window, box->fillColor);
    if (box->msgId != 0) {
        view->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_85);
        view->msgFormat = MessageFormat_New(HEAP_ID_85);
    }
    view->boxBaseTile = baseTile;
    view->box = box;
    box->func(view, window, 0x010200);
    if (box->msgId != 0) {
        String *string = ReadMsgData_ExpandPlaceholders(view->msgFormat, view->msgData, box->msgId, HEAP_ID_85);
        AddTextPrinterParameterizedWithColor(window, box->fontId, string, 0, 0, 0xFF, box->color, NULL);
        String_Delete(string);
        DestroyMsgData(view->msgData);
        MessageFormat_Delete(view->msgFormat);
    }
    if (window == &view->messageWindow) {
        DrawFrameAndWindow2(window, FALSE, 19, 10);
    } else {
        DrawFrameAndWindow1(window, FALSE, 10, 14);
    }
    return baseTile + box->width * box->height;
}

#pragma force_active reset

void LockCapsuleCard_LoadScreenData(LockCapsuleCardView *view, int fileId, int bgId, int offset) {
    NNSG2dScreenData *screenData;
    void *data = GfGfxLoader_LoadFromNarc(NARC_a_1_1_3, fileId, TRUE, view->heapId, TRUE);

    NNS_G2dGetUnpackedScreenData(data, &screenData);
    BG_LoadScreenTilemapData(view->bgConfig, bgId, screenData->rawData, offset);
    Heap_Free(data);
}

#pragma force_active on

void LockCapsuleCard_LoadCardBg(LockCapsuleCardView *view, int page) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x100, view->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 10, view->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x3000, TRUE, view->heapId);
    switch (page) {
    case 0:
        GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 8, view->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, view->heapId);
        break;
    case 1:
        GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 9, view->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, view->heapId);
        break;
    }
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, 0x20, view->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, view->bgConfig, GF_BG_LYR_MAIN_3, 0, 0x200, TRUE, view->heapId);
    LockCapsuleCard_LoadScreenData(view, 2, GF_BG_LYR_MAIN_3, 0x600);
    BgTilemapRectChangePalette(view->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 32, 24, 11);
    BgCommitTilemapBufferToVram(view->bgConfig, GF_BG_LYR_MAIN_3);
}

void LockCapsuleCard_HandleListMenuInput(OverlayManager *manager, int *state, MysteryGiftMenuFunc cancelFunc) {
    LockCapsuleCardView *view = OverlayManager_GetData(manager);
    u32 input = ListMenu_ProcessInput(view->listMenu);
    u32 nextState;

    switch (input) {
    case LIST_CANCEL:
        PlaySE(SEQ_SE_DP_SELECT);
        if (cancelFunc != NULL) {
            nextState = cancelFunc(manager);
            if (nextState != -1) {
                *state = nextState;
            }
        }
        break;
    case LIST_NOTHING_CHOSEN:
        break;
    default:
        PlaySE(SEQ_SE_DP_SELECT);
        if (input == 0) {
            break;
        }
        // small values are states, the others menu functions
        if (input < 31) {
            *state = input;
            break;
        }
        ov74_0223D0C0 = input;
        nextState = ((MysteryGiftMenuFunc)input)(manager);
        if (nextState != -1) {
            *state = nextState;
        }
        break;
    }
}

void LockCapsuleCard_CreateMonIconSprites(LockCapsuleCardView *view) {
    int x;
    int i;
    int species;
    void *data;
    NNSG2dCharacterData *charData;

    if (view->monIconSprites[0] == NULL && view->monIconSprites[1] == NULL && view->monIconSprites[2] == NULL) {
        if (ov74_0223567C() == 1) {
            LockCapsuleCard_DeleteSprites(view);
        }
        ov74_0223563C();
        ov74_02235690();
        ov74_02235728(NARC_a_1_1_3, 30, 27, 29, 28, 0);
        GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, (enum GFPalSlotOffset)0x60, 0, view->heapId);
    }
    x = 178;
    for (i = 0; i < 3; i++, x += 25) {
        species = view->card->monIcon[i];
        if (species == SPECIES_NONE) {
            if (view->monIconSprites[i] != NULL) {
                Sprite_SetDrawFlag(view->monIconSprites[i], FALSE);
            }
        } else {
            view->monIconSprites[i] = ov74_02235930(0, view->monIconSprites[i], x, 16, i + 10);
            data = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(species, FALSE, 0), FALSE, &charData, view->heapId);
            DC_FlushRange(charData->pRawData, 0x200);
            GX_LoadOBJ(charData->pRawData, (100 + i * 16) * 32, 0x200);
            Sprite_SetPaletteOverride(view->monIconSprites[i], GetMonIconPaletteEx(species, 0, FALSE) + 3);
            Heap_Free(data);
        }
    }
}

#pragma force_active reset

BOOL LockCapsuleCard_Init(OverlayManager *manager, int *state) {
    LockCapsuleCardView *view;

    Heap_Create(HEAP_ID_3, HEAP_ID_85, 0x30000);
    view = OverlayManager_CreateAndGetData(manager, sizeof(LockCapsuleCardView), HEAP_ID_85);
    memset(view, 0, sizeof(LockCapsuleCardView));
    view->bgConfig = BgConfig_Alloc(HEAP_ID_85);
    view->heapId = HEAP_ID_85;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    view->unk_2BF4 = 1;
    ov74_022352A0(HEAP_ID_85);
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570);
    return TRUE;
}

void LockCapsuleCard_FlipEffectTask(SysTask *task, WonderCardFlipEffect *effect) {
    void *buffer;

    HBlankDma_Stop();
    buffer = DoubleBuffer_GetOther(effect->doubleBuffer);
    HBlankDma_Start(buffer, (void *)REG_BG0HOFS_ADDR, 8, 1);
}

#pragma force_active on

void LockCapsuleCard_StartFlipEffect(LockCapsuleCardView *view, int mode, fx32 speed, fx32 accel) {
    WonderCardFlipEffect *effect = &view->flipEffect;

    effect->mode = mode;
    effect->speed = speed;
    effect->accel = accel;
    if (effect->doubleBuffer == NULL) {
        effect->doubleBuffer = DoubleBuffer_New(HEAP_ID_85, effect->buffers[0], effect->buffers[1]);
    }
    if (effect->task == NULL) {
        effect->task = SysTask_CreateOnVBlankQueue((SysTaskFunc)LockCapsuleCard_FlipEffectTask, effect, 1024);
    }
    effect->active = TRUE;
}

#pragma force_active reset

void LockCapsuleCard_StopFlipEffect(LockCapsuleCardView *view) {
    WonderCardFlipEffect *effect = &view->flipEffect;

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

BOOL LockCapsuleCard_UpdateFlipEffect(LockCapsuleCardView *view) {
    WonderCardFlipEffect *effect = &view->flipEffect;
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

#pragma push
#pragma opt_common_subs off

BOOL LockCapsuleCard_Main(OverlayManager *manager, int *state) {
    LockCapsuleCardView *view = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        view->saveData = ((UnkStruct_02111868_sub *)OverlayManager_GetArgs(manager))->saveData;
        view->mysteryGift = Save_MysteryGift_Get(view->saveData);
        view->options = Save_PlayerData_GetOptionsAddr(view->saveData);
        view->frame = Options_GetFrame(view->options);
        view->card = SaveMysteryGift_CardGetByIdx(view->mysteryGift, RECEIVED_WONDER_CARD_IDX);
        *state = 1;
        break;
    case 1:
        WonderCard_SetGraphicsBanks();
        WonderCard_InitBgLayers(view->bgConfig);
        *state = 2;
        break;
    case 2:
        ResetAllTextPrinters();
        LockCapsuleCard_LoadCardBg(view, 0);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_85);
        LoadUserFrameGfx1(view->bgConfig, GF_BG_LYR_MAIN_0, 1, 13, 0, HEAP_ID_85);
        LoadUserFrameGfx1(view->bgConfig, GF_BG_LYR_MAIN_0, 10, 14, 1, HEAP_ID_85);
        LoadUserFrameGfx2(view->bgConfig, GF_BG_LYR_MAIN_0, 19, 10, view->frame, HEAP_ID_85);
        LockCapsuleCard_DrawCard(view, 1, 0);
        LockCapsuleCard_StartFade(view, FADE_TYPE_BRIGHTNESS_IN, 3, state);
        break;
    case 3:
        if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            LockCapsuleCard_StartFade(view, FADE_TYPE_BRIGHTNESS_OUT, 29, state);
        } else if (gSystem.newKeys & PAD_BUTTON_A) {
            PlaySE(SEQ_SE_DP_SELECT);
            *state = 4;
        }
        break;
    case 4:
        view->menuBaseTile = LockCapsuleCard_PrintTextBox(view, &view->messageWindow, 7, 640);
        LockCapsuleCard_PrintTextBox(view, &view->menuWindow, 8, view->menuBaseTile);
        *state = 5;
        break;
    case 5:
        LockCapsuleCard_HandleListMenuInput(manager, state, NULL);
        if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            *state = 11;
        }
        break;
    case 6:
        PlaySE(SEQ_SE_DP_CARD2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        LockCapsuleCard_ClearWindowIfInUse(&view->messageWindow, FALSE);
        LockCapsuleCard_EraseWindowIfInUse(&view->menuWindow, FALSE);
        LockCapsuleCard_StartFlipEffect(view, 1, FX32_ONE, 0x66);
        *state = 7;
        break;
    case 7:
        if (LockCapsuleCard_UpdateFlipEffect(view)) {
            LockCapsuleCard_DrawCard(view, 1, 1);
            LockCapsuleCard_StartFlipEffect(view, 0, 0x708000, 0x384000);
            *state = 8;
        }
        break;
    case 8:
        LockCapsuleCard_UpdateFlipEffect(view);
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            PlaySE(SEQ_SE_DP_CARD2);
            LockCapsuleCard_StartFlipEffect(view, 1, FX32_ONE, 0x66);
            *state = 9;
        }
        break;
    case 9:
        if (LockCapsuleCard_UpdateFlipEffect(view)) {
            LockCapsuleCard_DrawCard(view, 1, 0);
            LockCapsuleCard_StartFlipEffect(view, 0, 0x708000, 0x384000);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            *state = 10;
        }
        break;
    case 10:
        if (LockCapsuleCard_UpdateFlipEffect(view)) {
            DrawFrameAndWindow2(&view->messageWindow, FALSE, 19, 10);
            DrawFrameAndWindow1(&view->menuWindow, FALSE, 10, 14);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            LockCapsuleCard_StopFlipEffect(view);
            *state = 5;
        }
        break;
    case 11:
        LockCapsuleCard_DestroyListMenu(view);
        LockCapsuleCard_ClearWindowIfInUse(&view->messageWindow, FALSE);
        ClearWindowTilemapAndCopyToVram(&view->messageWindow);
        RemoveWindow(&view->messageWindow);
        *state = 3;
        break;
    case 20:
        LockCapsuleCard_StartFade(view, FADE_TYPE_BRIGHTNESS_OUT, 21, state);
        break;
    case 21:
        LockCapsuleCard_DeleteSprites(view);
        LockCapsuleCard_ClearWindowIfInUse(&view->messageWindow, FALSE);
        LockCapsuleCard_EraseWindowIfInUse(&view->menuWindow, FALSE);
        LockCapsuleCard_DrawCard(view, 1, 0);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_85);
        LockCapsuleCard_StartFade(view, FADE_TYPE_BRIGHTNESS_IN, 4, state);
        break;
    case 23:
        if (!sub_02037D78()) {
            *state = view->nextState;
        }
        break;
    case 13:
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            LockCapsuleCard_ClearWindowIfInUse(&view->messageWindow, FALSE);
            ClearWindowTilemapAndCopyToVram(&view->messageWindow);
            RemoveWindow(&view->messageWindow);
            *state = 3;
        }
        break;
    case 27:
        LockCapsuleCard_StartFade(view, FADE_TYPE_BRIGHTNESS_OUT, 29, state);
        break;
    case 28:
        if (IsPaletteFadeFinished()) {
            *state = view->nextState;
        }
        break;
    case 29:
        LockCapsuleCard_DeleteSprites(view);
        LockCapsuleCard_StopFlipEffect(view);
        return TRUE;
    case 30:
        break;
    }
    if (view->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(view->spriteList);
    }
    ov74_022358BC();
    if (view->callback != NULL) {
        view->callback(view);
    }
    return FALSE;
}

#pragma pop
#pragma force_active reset

BOOL LockCapsuleCard_Exit(OverlayManager *manager, int *state) {
    LockCapsuleCardView *view = OverlayManager_GetData(manager);
    u32 i;

    for (i = 0; i < 11; i++) {
        if (view->windows[i].bgConfig != NULL) {
            ClearWindowTilemapAndCopyToVram(&view->windows[i]);
            RemoveWindow(&view->windows[i]);
        }
    }
    if (view->messageWindow.bgConfig != NULL) {
        ClearWindowTilemapAndCopyToVram(&view->messageWindow);
        RemoveWindow(&view->messageWindow);
    }
    if (view->menuWindow.bgConfig != NULL) {
        ClearWindowTilemapAndCopyToVram(&view->menuWindow);
        RemoveWindow(&view->menuWindow);
    }
    FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_MAIN_3);
    Heap_Free(view->bgConfig);
    RegisterMainOverlay(FS_OVERLAY_ID(OVY_74), &gApp_MainMenu_SelectOption_MysteryGift);
    Heap_Destroy(HEAP_ID_89);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_85);
    return TRUE;
}

#pragma force_active on

// Draws the Lock Capsule card on bgConfig with a temporary work (used by the Mystery Gift app).
void LockCapsuleCard_DrawCardOnBg(BgConfig *bgConfig, WonderCard *card, enum HeapID heapID) {
    LockCapsuleCardView *view;

    WonderCard_InitBgLayers(bgConfig);
    view = Heap_AllocAtEnd(heapID, sizeof(LockCapsuleCardView));
    memset(view, 0, sizeof(LockCapsuleCardView));
    view->bgConfig = bgConfig;
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, view->heapId = heapID);
    view->card = card;
    LockCapsuleCard_DrawCard(view, 1, 0);
    Heap_Free(view);
}

#pragma force_active reset

void WonderCard_SetGraphicsBanks(void) {
    GraphicsBanks banks = _0223B4B0;
    GfGfx_SetBanks(&banks);
}

void WonderCard_InitBgLayers(BgConfig *bgConfig) {
    {
        GraphicsModes modes = _0223B430;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        BgTemplate template = _0223B45C;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    }
    {
        BgTemplate template = _0223B494;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    }
    {
        BgTemplate template = _0223B478;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    }
    {
        BgTemplate template = _0223B440;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    }
}

#pragma force_active on

// 0: the gift can be received; 1: not for this game, 2: already received (unique gift), 3: no free gift slot,
// 4: no free Wonder Card slot, 5: delivered by another route. A header with all bits set wipes the Mystery Gift
// save data and resets the system.
int MysteryGift_CheckCanReceive(SaveData *saveData, UnkWonderCardSubstruct_104 *header) {
    MysteryGiftSave *mg = Save_MysteryGift_Get(saveData);
    u32 versions = header->version;

    if (versions == 0xFFFFFFFF && header->id == 0xFFFF) {
        Save_MysteryGift_Init(mg);
        Save_NowWriteFile_AfterMGInit(saveData, 0);
        OS_ResetSystem(0);
    }
    if (header->id >= 100 && header->id <= 152) {
        versions |= 0x1180;
    }
    if (versions == 0) {
        versions = 0xFFFFFFFF;
    }
    if (!(versions & (1 << GAME_VERSION))) {
        return 1;
    }
    if (header->unique == TRUE && SaveMysteryGift_ReceivedFlagTest(mg, header->id) == TRUE) {
        return 2;
    }
    if (header->unk8E_2 == TRUE && SaveMysteryGift_CardFindAvailable(mg) == FALSE) {
        return 4;
    }
    if (SaveMysteryGift_FindAvailable(mg) == FALSE) {
        return 3;
    }
    if (header->unk8E_5 == TRUE) {
        return 5;
    }
    return 0;
}

#pragma force_active reset
