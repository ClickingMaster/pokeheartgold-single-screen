#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "msgdata/msg/msg_0775.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gts_internal.h"
#include "heap.h"
#include "item.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "char_tiles.h"
#include "wireless_status_icon.h"
#include "yes_no_prompt.h"

// The sub screen panel shared by the screens (they link here), then the summary of the deposited Pokemon
// (screen 2): the Pokemon, its trainer and the wanted Pokemon, with TAKE BACK and LEAVE AS IS.

typedef enum GtsSummaryState {
    GTS_SUMMARY_STATE_START,
    GTS_SUMMARY_STATE_HANDLE_INPUT,
    GTS_SUMMARY_STATE_FADE_OUT,
    GTS_SUMMARY_STATE_WAIT_TEXT,
    GTS_SUMMARY_STATE_WAIT_TEXT_AND_PAUSE,
    GTS_SUMMARY_STATE_SHOW_TAKE_BACK_PROMPT,
    GTS_SUMMARY_STATE_HANDLE_TAKE_BACK_PROMPT,
    GTS_SUMMARY_STATE_SHOW_MENU,
    GTS_SUMMARY_STATE_HANDLE_MENU,
} GtsSummaryState;

typedef struct GtsWindowRect {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
} GtsWindowRect;

extern int (*const sGtsSummaryStates[])(GtsApp *app);
extern const BgTemplate sGtsSummaryBg0Template;
extern const BgTemplate sGtsSummaryBg1Template;
extern const BgTemplate sGtsSummaryBg2Template;
extern const BgTemplate sGtsSummaryBg3Template;
extern const GtsWindowRect sGtsSummaryInfoWindowRects[14];
// Not const: the original declarations (const changes the scheduling of Gts_DrawMonSummary).
extern u32 sGtsGenderSymbolMsgs[];
extern u32 sGtsGenderSymbolColors[];

void Gts_LoadSubScreenBg(GtsApp *app);
void Gts_ShowSubScreenPanel(GtsApp *app);
void Gts_CreatePanelWindow(GtsApp *app, int caption);
int GtsSummary_Init(GtsApp *app, int state);
int GtsSummary_Main(GtsApp *app, int state);
BOOL GtsSummary_Exit(GtsApp *app, int state);
void GtsSummary_InitBgs(BgConfig *bgConfig);
void GtsSummary_FreeBgs(BgConfig *bgConfig);
void GtsSummary_LoadGraphics(GtsApp *app);
void GtsSummary_CreateMonSprite(GtsApp *app);
void GtsSummary_DeleteMonSprite(GtsApp *app);
void GtsSummary_CreateWindows(GtsApp *app);
void GtsSummary_RemoveWindows(GtsApp *app);
void GtsSummary_CreateStrings(GtsApp *app);
void GtsSummary_DeleteStrings(GtsApp *app);
int GtsSummary_Start(GtsApp *app);
int GtsSummary_HandleInput(GtsApp *app);
int GtsSummary_FadeOut(GtsApp *app);
int GtsSummary_ShowTakeBackPrompt(GtsApp *app);
int GtsSummary_HandleTakeBackPrompt(GtsApp *app);
int GtsSummary_ShowMenu(GtsApp *app);
int GtsSummary_HandleMenu(GtsApp *app);
int GtsSummary_WaitText(GtsApp *app);
int GtsSummary_WaitTextAndPause(GtsApp *app);
void GtsSummary_PrintMonMessage(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4, Pokemon *mon);
void Gts_DrawMonSummary(MsgData *msgData, MsgData *speciesNames, MessageFormat *msgFormat, Window *windows, BoxPokemon *boxMon, GtsMonSummary *summary);
void Gts_DrawTrainerInfo(MsgData *msgData, Window *trainerWindows, const u16 *trainerName, Pokemon *mon, Window *otWindows);
void Gts_LoadMonPicture(Pokemon *mon);
void GtsSummary_DrawWanted(GtsApp *app);

void Gts_LoadSubScreenBg(GtsApp *app) {
    BgConfig *bgConfig = app->bgConfig;

    GfGfxLoader_GXLoadPal(NARC_a_1_0_0, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200, HEAP_ID_61);
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 17, bgConfig, GF_BG_LYR_SUB_1, 0, 0xA800, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 40, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, TRUE, HEAP_ID_61);
    FillBgTilemapRect(bgConfig, GF_BG_LYR_SUB_1, 0, 0, 24, 32, 8, TILEMAP_FILL_OVWT_PAL);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_SUB_1);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_61);
}

void Gts_ShowSubScreenPanel(GtsApp *app) {
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 18, app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 41, app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_61);
    Sprite_SetDrawFlag(app->panelSprite, TRUE);
}

void Gts_CreatePanelWindow(GtsApp *app, int caption) {
    AddWindowParameterized(app->bgConfig, &app->panelWindow, GF_BG_LYR_SUB_0, 13, 19, 16, 4, 1, 1);
    FillWindowPixelBuffer(&app->panelWindow, 0);
    Gts_PrintPanelCaption(&app->panelWindow, app->gtsMsgData, caption);
}

int GtsSummary_Init(GtsApp *app, int state) {
    int levelRange;

    GtsSummary_CreateStrings(app);
    GtsSummary_InitBgs(app->bgConfig);
    GtsSummary_LoadGraphics(app);
    GtsSummary_CreateWindows(app);
    GtsSummary_CreateMonSprite(app);
    Gts_DrawMonSummary(app->gtsMsgData, app->speciesNames, app->msgFormat, app->infoWindows, Mon_GetBoxMon(&app->record.mon), &app->record.summary);
    Gts_DrawTrainerInfo(app->gtsMsgData, &app->infoWindows[7], app->record.trainerName, &app->record.mon, &app->infoWindows[12]);
    levelRange = Gts_FindLevelRange(app->record.wanted.minLevel, app->record.wanted.maxLevel, 0);
    Gts_DrawWantedMonLevelLeft(app->gtsMsgData, app->speciesNames, app->msgFormat, &app->infoWindows[9], app->record.wanted.species, app->record.wanted.gender, levelRange);
    Gts_LoadMonPicture(&app->record.mon);
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
    app->state = GTS_SUMMARY_STATE_START;
    return 2;
}

int GtsSummary_Main(GtsApp *app, int state) {
    return sGtsSummaryStates[app->state](app);
}

BOOL GtsSummary_Exit(GtsApp *app, int state) {
    GtsSummary_DeleteMonSprite(app);
    GtsSummary_DeleteStrings(app);
    GtsSummary_RemoveWindows(app);
    GtsSummary_FreeBgs(app->bgConfig);
    Sprite_SetDrawFlag(app->panelSprite, FALSE);
    Gts_SwitchScreen(app);
    return TRUE;
}

#pragma force_active on

void GtsSummary_InitBgs(BgConfig *bgConfig) {
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;

    bg0 = sGtsSummaryBg0Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    bg1 = sGtsSummaryBg1Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    bg2 = sGtsSummaryBg2Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    bg3 = sGtsSummaryBg3Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_61);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_61);
    Gts_InitSubScreenBgs(bgConfig, 0, FALSE);
}

#pragma force_active reset

void GtsSummary_FreeBgs(BgConfig *bgConfig) {
    Gts_FreeSubScreenBgs(bgConfig);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
}

#pragma force_active on

void GtsSummary_LoadGraphics(GtsApp *app) {
    BgConfig *bgConfig = app->bgConfig;
    NARC *narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x100, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(app->args->options), HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1F, 11, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 20, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xA00, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 28, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_61);
    Gts_ShowSubScreenPanel(app);
    NARC_Delete(narc);
}

void GtsSummary_CreateMonSprite(GtsApp *app) {
    SpriteTemplate template;

    Gts_InitSpriteTemplate(&template, app, &app->headers[0], NNS_G2D_VRAM_TYPE_2DMAIN);
    template.position.x = 208 * FX32_ONE;
    template.position.y = 58 * FX32_ONE;
    app->monSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->monSprite, TRUE);
    Sprite_SetAnimCtrlSeq(app->monSprite, 37);
    Sprite_SetPriority(app->monSprite, 1);
    WirelessStatusIcon_Create();
}

#pragma force_active reset

void GtsSummary_DeleteMonSprite(GtsApp *app) {
    Sprite_Delete(app->monSprite);
}

void GtsSummary_CreateWindows(GtsApp *app) {
    int i;
    int baseTile;

    AddWindowParameterized(app->bgConfig, &app->waitingIconWindow, GF_BG_LYR_MAIN_0, 2, 21, 27, 2, 13, 0x28);
    FillWindowPixelBuffer(&app->waitingIconWindow, 0);
    AddWindowParameterized(app->bgConfig, &app->windows[0], GF_BG_LYR_MAIN_0, 21, 15, 10, 4, 13, 0x5E);
    baseTile = 0x86;
    for (i = 0; i < 14; i++) {
        AddWindowParameterized(app->bgConfig, &app->infoWindows[i], GF_BG_LYR_MAIN_3, sGtsSummaryInfoWindowRects[i].x, sGtsSummaryInfoWindowRects[i].y, sGtsSummaryInfoWindowRects[i].width, sGtsSummaryInfoWindowRects[i].height, 13, baseTile);
        FillWindowPixelBuffer(&app->infoWindows[i], 0);
        baseTile += sGtsSummaryInfoWindowRects[i].width * sGtsSummaryInfoWindowRects[i].height;
    }
    Gts_CreatePanelWindow(app, 2);
}

void GtsSummary_RemoveWindows(GtsApp *app) {
    int i;

    RemoveWindow(&app->panelWindow);
    RemoveWindow(&app->waitingIconWindow);
    RemoveWindow(&app->windows[0]);
    for (i = 0; i < 14; i++) {
        RemoveWindow(&app->infoWindows[i]);
    }
}

void GtsSummary_CreateStrings(GtsApp *app) {
    int i;

    app->msgBuffer = String_New(180, HEAP_ID_61);
    app->titleString = NewString_ReadMsgData(app->gtsMsgData, msg_0775_00039); // GLOBAL TRADE SYSTEM
    for (i = 0; i < 10; i++) {
        app->unk_0BC4[i] = String_New(20, HEAP_ID_61);
    }
}

#pragma force_active on

void GtsSummary_DeleteStrings(GtsApp *app) {
    int i;

    for (i = 0; i < 10; i++) {
        String_Delete(app->unk_0BC4[i]);
    }
    String_Delete(app->msgBuffer);
    String_Delete(app->titleString);
}

#pragma force_active reset

int GtsSummary_Start(GtsApp *app) {
    app->state = GTS_SUMMARY_STATE_HANDLE_INPUT;
    return 3;
}

int GtsSummary_HandleInput(GtsApp *app) {
    if ((gSystem.newKeys & PAD_BUTTON_A) || gSystem.touchNew) {
        GtsSummary_PrintMonMessage(app, msg_0775_00005, 1, 0, 0xF0F, &app->record.mon); // Do what with this Pokemon?
        Gts_SetState(app, GTS_SUMMARY_STATE_WAIT_TEXT, GTS_SUMMARY_STATE_SHOW_MENU);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        app->state = GTS_SUMMARY_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 1, 0);
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return 3;
}

int GtsSummary_FadeOut(GtsApp *app) {
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
    app->state = GTS_SUMMARY_STATE_START;
    return 4;
}

int GtsSummary_ShowTakeBackPrompt(GtsApp *app) {
    app->yesNoPrompt = Gts_CreateYesNoPrompt(app->bgConfig, 12, 0x16A, 8, 1);
    app->state = GTS_SUMMARY_STATE_HANDLE_TAKE_BACK_PROMPT;
    return 3;
}

int GtsSummary_HandleTakeBackPrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = GTS_SUMMARY_STATE_FADE_OUT;
        app->unk_11FC = 1;
        Gts_SetNextScreen(app, 7, 8);
        GtsSummary_DrawWanted(app);
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = GTS_SUMMARY_STATE_START;
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        GtsSummary_DrawWanted(app);
    }
    return 3;
}

int GtsSummary_ShowMenu(GtsApp *app) {
    app->listItems = ListMenuItems_New(2, HEAP_ID_61);
    ListMenuItems_AppendFromMsgData(app->listItems, app->gtsMsgData, msg_0775_00055, 1); // TAKE BACK
    ListMenuItems_AppendFromMsgData(app->listItems, app->gtsMsgData, msg_0775_00056, 2); // LEAVE AS IS
    app->menu = Gts_CreateListMenu(app, 2, 13);
    Gts_DimScreen(TRUE);
    app->state = GTS_SUMMARY_STATE_HANDLE_MENU;
    return 3;
}

int GtsSummary_HandleMenu(GtsApp *app) {
    int result = TouchscreenListMenu_HandleInput(app->menu);
    if (result == 1) {
        Gts_DestroyListMenu(app);
        ListMenuItems_Delete(app->listItems);
        Pokemon *mon = &app->record.mon;
        if (Gts_MonHoldsMail(mon) && Party_GetCount(app->args->party) == PARTY_SIZE) {
            GtsSummary_PrintMonMessage(app, msg_0775_00036, 1, 0, 0xF0F, mon); // Your party is full (the Pokemon holds Mail).
            Gts_SetState(app, GTS_SUMMARY_STATE_WAIT_TEXT, GTS_SUMMARY_STATE_HANDLE_INPUT);
            return 3;
        }
        GtsSummary_PrintMonMessage(app, msg_0775_00006, 1, 0, 0xF0F, mon); // Will you take this Pokemon?
        Gts_SetState(app, GTS_SUMMARY_STATE_WAIT_TEXT, GTS_SUMMARY_STATE_SHOW_TAKE_BACK_PROMPT);
        GtsSummary_DrawWanted(app);
    } else if (result == 2 || result == -2) {
        Gts_DestroyListMenu(app);
        ListMenuItems_Delete(app->listItems);
        app->state = GTS_SUMMARY_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 1, 0);
        GtsSummary_DrawWanted(app);
    }
    return 3;
}

int GtsSummary_WaitText(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->state = app->nextState;
    }
    return 3;
}

int GtsSummary_WaitTextAndPause(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->waitFrames++;
        if (app->waitFrames > 45) {
            app->waitFrames = 0;
            app->state = app->nextState;
        }
    }
    return 3;
}

void GtsSummary_PrintMonMessage(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4, Pokemon *mon) {
    BoxPokemon *boxMon = Mon_GetBoxMon(mon);
    String *string;

    BufferBoxMonSpeciesName(app->msgFormat, 0, boxMon);
    string = NewString_ReadMsgData(app->gtsMsgData, msgId);
    StringExpandPlaceholders(app->msgFormat, app->msgBuffer, string);
    FillWindowPixelBuffer(&app->waitingIconWindow, 15);
    DrawFrameAndWindow2(&app->waitingIconWindow, FALSE, 1, 14);
    app->textPrinterId = AddTextPrinterParameterized(&app->waitingIconWindow, 1, app->msgBuffer, 0, 0, textSpeed, NULL);
    String_Delete(string);
}

#pragma force_active on

// windows[0..6]: nickname and gender, species, "Lv.", level, ITEM, item name, NAME.
void Gts_DrawMonSummary(MsgData *msgData, MsgData *speciesNames, MessageFormat *msgFormat, Window *windows, BoxPokemon *boxMon, GtsMonSummary *summary) {
    String *speciesName;
    String *genderSymbol;
    String *levelLabel;
    String *level;
    String *itemLabel;
    String *nameLabel;
    String *nickname;
    String *itemName;
    int gender;
    int species;
    int monLevel;
    int item;
    int i;

    nickname = String_New(22, HEAP_ID_61);
    itemName = String_New(18, HEAP_ID_61);
    GetBoxMonData(boxMon, MON_DATA_NICKNAME_STRING, nickname);
    species = summary->species;
    gender = summary->gender;
    monLevel = summary->level;
    item = GetBoxMonData(boxMon, MON_DATA_HELD_ITEM, NULL);
    itemLabel = NewString_ReadMsgData(msgData, msg_0775_00073); // ITEM
    genderSymbol = NewString_ReadMsgData(msgData, sGtsGenderSymbolMsgs[gender]);
    levelLabel = NewString_ReadMsgData(msgData, msg_0775_00106); // Lv.
    BufferIntegerAsString(msgFormat, 3, monLevel, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    level = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0775_00107, HEAP_ID_61);
    speciesName = NewString_ReadMsgData(speciesNames, species);
    GetItemNameIntoString(itemName, item, HEAP_ID_61);
    nameLabel = NewString_ReadMsgData(msgData, msg_0775_00059); // NAME
    for (i = 0; i < 6; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
    }
    Gts_PrintAlignedFont0(&windows[0], nickname, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    if (gender != 3) {
        Gts_PrintAlignedFont0(&windows[0], genderSymbol, 64, 0, 0, sGtsGenderSymbolColors[gender]);
    }
    Gts_PrintAlignedFont0(&windows[1], speciesName, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintAlignedFont0(&windows[2], levelLabel, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintAlignedFont0(&windows[3], level, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintAlignedFont0(&windows[4], itemLabel, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    Gts_PrintAlignedFont0(&windows[5], itemName, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintAlignedFont0(&windows[6], nameLabel, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(itemLabel);
    String_Delete(itemName);
    String_Delete(levelLabel);
    String_Delete(level);
    String_Delete(genderSymbol);
    String_Delete(nickname);
    String_Delete(speciesName);
    String_Delete(nameLabel);
}

#pragma force_active reset

// OFFERER and the trainer's name; the label of message 180 and the Pokemon's OT name.
void Gts_DrawTrainerInfo(MsgData *msgData, Window *trainerWindows, const u16 *trainerName, Pokemon *mon, Window *otWindows) {
    String *name = String_New(16, HEAP_ID_61);
    String *otName = String_New(16, HEAP_ID_61);
    String *trainerLabel = NewString_ReadMsgData(msgData, msg_0775_00049); // OFFERER
    String *otLabel;

    CopyU16ArrayToString(name, trainerName);
    otLabel = NewString_ReadMsgData(msgData, msg_0775_00180);
    GetMonData(mon, MON_DATA_OT_NAME_STRING, otName);
    Gts_PrintAlignedFont0(&trainerWindows[0], trainerLabel, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    Gts_PrintAlignedFont0(&trainerWindows[1], name, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintAlignedFont0(&otWindows[0], otLabel, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    Gts_PrintAlignedFont0(&otWindows[1], otName, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    String_Delete(trainerLabel);
    String_Delete(name);
    String_Delete(otLabel);
    String_Delete(otName);
}

// The Pokemon's front picture into the OBJ char data of the mon sprite (and its palette).
void Gts_LoadMonPicture(Pokemon *mon) {
    PokepicTemplate pokepic;
    void *buf = Heap_Alloc(HEAP_ID_61, 0xC80);
    int personality;
    int species;

    GetPokemonSpriteCharAndPlttNarcIds(&pokepic, mon, 2);
    personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    CharTiles_LoadMonObjBlocksFromNarc((NarcId)pokepic.narcID, pokepic.charDataID, HEAP_ID_61, 0, 0, 10, 10, buf, personality, FALSE, 2, species);
    DC_FlushRange(buf, 0xC80);
    GX_LoadOBJ(buf, 0x4A00, 0xC80);
    GfGfxLoader_GXLoadPal((NarcId)pokepic.narcID, pokepic.palDataID, GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_13_OFFSET, 0x20, HEAP_ID_61);
    Heap_Free(buf);
}

void GtsSummary_DrawWanted(GtsApp *app) {
    int levelRange = Gts_FindLevelRange(app->record.wanted.minLevel, app->record.wanted.maxLevel, 0);
    Gts_DrawWantedMonLevelLeft(app->gtsMsgData, app->speciesNames, app->msgFormat, &app->infoWindows[9], app->record.wanted.species, app->record.wanted.gender, levelRange);
}
