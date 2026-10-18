#include "mailbox_app.h"

#include "global.h"

#include "application/mailbox_app_internal.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0232.h"

#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "mail.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "sprite_system.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"

static const TouchscreenListMenuTemplate sMailboxMailMenuTemplate = {
    .wrapAround = 1,
    .centered = 1,
    .xOffset = 0,
    .bgId = 0,
    .plttOffset = 12,
    .unk4 = 0,
    .unk5 = 0,
    .baseTile = 0x2FD,
    .charOffset = 0x2E2,
    .unkA = 169,
};

static const GraphicsModes sMailboxAppGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const BgTemplate sMailboxBgTemplate4 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x18000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sMailboxBgTemplate0 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x18000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sMailboxBgTemplate5 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sMailboxBgTemplate6 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sMailboxBgTemplate2 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sMailboxBgTemplate3 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sMailboxBgTemplate1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sMailboxBgTemplate7 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const GraphicsBanks sMailboxAppGraphicsBanks = {
    .bg = GX_VRAM_BG_128_B,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_0_A,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

static u32 MailboxApp_GetMailMenuItemCount(MailboxApp *app);

BOOL MailboxApp_Init(OverlayManager *man, int *state) {
    MailboxApp *app;

    Heap_Create(HEAP_ID_3, HEAP_ID_156, 0x10000);
    app = OverlayManager_CreateAndGetData(man, sizeof(MailboxApp), HEAP_ID_156);
    MI_CpuFill8(app, 0, sizeof(MailboxApp));
    app->args = OverlayManager_GetArgs(man);
    app->options = Save_PlayerData_GetOptionsAddr(app->args->saveData);
    app->mailbox = Save_Mailbox_Get(app->args->saveData);
    app->nextState = MAILBOX_STATE_FADE_IN;
    return TRUE;
}

BOOL MailboxApp_Main(OverlayManager *man, int *state) {
    MailboxApp *app = OverlayManager_GetData(man);
    if (MailboxApp_RunState(app, state) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL MailboxApp_Exit(OverlayManager *man, int *state) {
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_156);
    return TRUE;
}

void MailboxApp_VBlankCB(void *cbArg) {
    MailboxApp *app = cbArg;
    vu32 *dtcm = (vu32 *)0x027E0000;

    DoScheduledBgGpuUpdates(app->data->bgConfig);
    SpriteSystem_TransferOam();
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

void MailboxApp_SetGfxBanks(void) {
    GfGfx_SetBanks(&sMailboxAppGraphicsBanks);
}

void MailboxApp_InitBgLayers(MailboxApp *app) {
    GraphicsModes modes;
    BgTemplate bg4, bg5, bg6, bg7, bg0, bg1, bg2, bg3;

    app->data->bgConfig = BgConfig_Alloc(HEAP_ID_157);
    modes = sMailboxAppGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    bg4 = sMailboxBgTemplate4;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_SUB_0, &bg4, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_157);
    bg5 = sMailboxBgTemplate5;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_SUB_1, &bg5, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_1);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_157);
    bg6 = sMailboxBgTemplate6;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_SUB_2, &bg6, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_2);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_157);
    bg7 = sMailboxBgTemplate7;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_SUB_3, &bg7, GF_BG_TYPE_TEXT);
    bg0 = sMailboxBgTemplate0;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_MAIN_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_157);
    bg1 = sMailboxBgTemplate1;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_157);
    bg2 = sMailboxBgTemplate2;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_MAIN_2);
    bg3 = sMailboxBgTemplate3;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
}

void MailboxApp_FreeBgLayers(MailboxApp *app) {
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_SUB_0);
    Heap_Free(app->data->bgConfig);
}

void MailboxApp_LoadBgGraphics(MailboxApp *app) {
    NARC *narc = NARC_New(NARC_a_2_5_0, HEAP_ID_157);
    NNSG2dCharacterData *charData;
    void *charFile;
    u8 *rawData;

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->data->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, app->data->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_157);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, app->data->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, app->data->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, app->data->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_157);
    charFile = GfGfxLoader_GetCharDataFromOpenNarc(narc, 1, TRUE, &charData, HEAP_ID_157);
    rawData = charData->pRawData;
    MI_CpuCopy32(rawData + 0xE0, app->data->nameplateTileLeft, 0x20);
    MI_CpuCopy32(rawData + 0x160, app->data->nameplateTileRight, 0x20);
    Heap_Free(charFile);
    NARC_Delete(narc);
}

void MailboxApp_LoadFrameAndFontGfx(MailboxApp *app) {
    MailboxAppData *data = app->data;
    u8 frame = Options_GetFrame(app->options);

    LoadUserFrameGfx2(data->bgConfig, GF_BG_LYR_MAIN_0, 0x3E2, 13, frame, HEAP_ID_157);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(7 << 6), HEAP_ID_157);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(0x1E << 4), HEAP_ID_157);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(0x1E << 4), HEAP_ID_157);
}

void MailboxApp_CreateNameplateCells(MailboxApp *app) {
    u32 i;
    u16 *firstTilemap;

    app->data->cellManager = BgCellManager_New(app->data->bgConfig, 2, 11, HEAP_ID_157);
    for (i = 0; i < MAILBOX_MAIL_PER_PAGE; i++) {
        BgCellManager_InitCell(app->data->cellManager, i, 2, 14, 4);
    }
    BgCellManager_InitCell(app->data->cellManager, MAILBOX_CELL_SELECTED, 1, 14, 4);
    BgCellManager_LoadCellTilemap(app->data->cellManager, 0, NARC_a_2_5_0, 3, TRUE);
    firstTilemap = BgCellManager_GetCellTilemap(app->data->cellManager, 0);
    for (i = 1; i <= MAILBOX_MAIL_PER_PAGE; i++) {
        MI_CpuCopy8(firstTilemap, BgCellManager_GetCellTilemap(app->data->cellManager, i), 0x70);
    }
    for (i = 0; i < MAILBOX_MAIL_PER_PAGE; i++) {
        BgCellManager_DrawCellAt(app->data->cellManager, i, (s8)((i & 1) * 16 + 1), (s8)((i >> 1) * 4));
    }
}

void MailboxApp_DeleteNameplateCells(MailboxApp *app) {
    BgCellManager_Delete(app->data->cellManager);
}

void MailboxAppData_CreateStrings(MailboxAppData *data) {
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0232_bin, HEAP_ID_157);
    data->msgFormat = MessageFormat_New(HEAP_ID_157);
    data->string = String_New(0x400, HEAP_ID_157);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
}

void MailboxAppData_DeleteStrings(MailboxAppData *data) {
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    String_Delete(data->string);
    MessageFormat_Delete(data->msgFormat);
    DestroyMsgData(data->msgData);
}

void MailboxApp_CreateYesNoPrompt(MailboxApp *app) {
    app->data->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_157);
}

void MailboxApp_DestroyYesNoPrompt(MailboxApp *app) {
    YesNoPrompt_Destroy(app->data->yesNoPrompt);
}

void MailboxApp_ShowYesNoPrompt(MailboxApp *app) {
    YesNoPromptTemplate template;

    template.bgConfig = app->data->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = 0x3A6;
    template.plttSlot = 10;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = FALSE;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(app->data->yesNoPrompt, &template);
}

// READ / ERASE / GIVE / CANCEL; only READ and CANCEL when the party is empty.
void MailboxApp_CreateMailMenuItems(MailboxApp *app) {
    u32 numItems;
    u16 i;

    app->data->listMenuSpawner = TouchscreenListMenuSpawner_Create(HEAP_ID_157, NULL);
    numItems = MailboxApp_GetMailMenuItemCount(app);
    app->data->listMenuItems = ListMenuItems_New(numItems, HEAP_ID_157);
    i = 0;
    do {
        if (numItems != 2 || (i != 2 && i != 1)) {
            ListMenuItems_AppendFromMsgData(app->data->listMenuItems, app->data->msgData, msg_0232_00005 + i, i);
        }
        i++;
    } while (i < 4);
}

void MailboxApp_DeleteMailMenuItems(MailboxApp *app) {
    ListMenuItems_Delete(app->data->listMenuItems);
    TouchscreenListMenuSpawner_Destroy(app->data->listMenuSpawner);
}

void MailboxApp_OpenMailMenu(MailboxApp *app) {
    TouchscreenListMenuHeader header;
    TouchscreenListMenuTemplate template = sMailboxMailMenuTemplate;

    header.template = template;
    header.listMenuItems = app->data->listMenuItems;
    header.bgConfig = app->data->bgConfig;
    header.numWindows = MailboxApp_GetMailMenuItemCount(app);
    if (header.numWindows == 4) {
        app->data->listMenu = TouchscreenListMenu_Create(app->data->listMenuSpawner, &header, 0, 18, 5, 11, 0);
    } else {
        app->data->listMenu = TouchscreenListMenu_Create(app->data->listMenuSpawner, &header, 0, 18, 11, 11, 0);
    }
}

void MailboxApp_CloseMailMenu(MailboxApp *app) {
    TouchscreenListMenu_Destroy(app->data->listMenu);
}

void MailboxApp_LoadMail(MailboxApp *app) {
    u32 i;

    app->data->mailCount = 0;
    for (i = 0; i < MAILBOX_MSG_COUNT; i++) {
        app->data->mailOrder[i] = MAILBOX_NO_MAIL;
        app->data->mail[i] = Mailbox_AllocAndFetchMailI(app->mailbox->msgs, 0, i, HEAP_ID_157);
        if (Mail_TypeIsValid(app->data->mail[i]) == TRUE) {
            app->data->mailOrder[app->data->mailCount] = i;
            app->data->mailCount++;
        }
    }
    if (app->data->mailCount == 0) {
        app->data->lastPage = 0;
    } else {
        app->data->lastPage = (app->data->mailCount - 1) / MAILBOX_MAIL_PER_PAGE;
    }
}

void MailboxApp_FreeMail(MailboxApp *app) {
    u32 i;

    for (i = 0; i < MAILBOX_MSG_COUNT; i++) {
        Heap_Free(app->data->mail[i]);
    }
}

void MailboxApp_DrawPage(MailboxApp *app) {
    u32 first = (u16)(app->page * MAILBOX_MAIL_PER_PAGE);
    u16 cell = 0;
    u8 *mailOrder = app->data->mailOrder + first;
    u16 windowId = first;
    u16 i;

    for (i = 0; i < MAILBOX_MAIL_PER_PAGE; i++) {
        u8 mailIdx = mailOrder[i];
        if (mailIdx == MAILBOX_NO_MAIL) {
            BgCellManager_EraseCell(app->data->cellManager, cell);
        } else {
            MailboxApp_DrawNameplate(app, cell, windowId, mailIdx);
            BgCellManager_DrawCell(app->data->cellManager, cell);
        }
        windowId++;
        cell++;
    }
}

void MailboxApp_DrawSelectedNameplate(MailboxApp *app) {
    MailboxApp_DrawNameplate(app, MAILBOX_CELL_SELECTED, 20, app->selectedMail);
    BgCellManager_DrawCellAt(app->data->cellManager, MAILBOX_CELL_SELECTED, 4, 7);
}

BOOL MailboxApp_RunPaletteFlash(MailboxApp *app) {
    MailboxAppPaletteFlash *flash = &app->data->flash;

    switch (flash->state) {
    case 0:
        if (!flash->isBg) {
            ManagedSprite_SetPaletteOverride(app->data->sprites[flash->target], flash->firstPalette);
        } else {
            BgTilemapRectChangePalette(app->data->bgConfig, flash->target, flash->x, flash->y, flash->width, flash->height, flash->firstPalette);
            ScheduleBgTilemapBufferTransfer(app->data->bgConfig, flash->target);
        }
        flash->state++;
        break;
    case 1:
        flash->timer++;
        if (flash->timer == 4) {
            if (!flash->isBg) {
                ManagedSprite_SetPaletteOverride(app->data->sprites[flash->target], flash->secondPalette);
            } else {
                BgTilemapRectChangePalette(app->data->bgConfig, flash->target, flash->x, flash->y, flash->width, flash->height, flash->secondPalette);
                ScheduleBgTilemapBufferTransfer(app->data->bgConfig, flash->target);
            }
            flash->timer = 0;
            flash->state++;
        }
        break;
    case 2:
        flash->timer++;
        if (flash->timer == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

void MailboxApp_LoadStationeryGfx(MailboxApp *app) {
    NARC *narc = NARC_New(NARC_a_0_7_9, HEAP_ID_157);
    u32 mailType = Mail_GetType(app->data->mail[app->selectedMail]);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, mailType + 12, app->data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, mailType, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, mailType + 24, app->data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_157);
    NARC_Delete(narc);
}

static u32 MailboxApp_GetMailMenuItemCount(MailboxApp *app) {
    if (Party_GetCount(SaveArray_Party_Get(app->args->saveData)) > 0) {
        return 4;
    }
    return 2;
}
