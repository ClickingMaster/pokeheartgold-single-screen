#include "bug_contest_swap_mon.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "unk_0208805C.h"
#include "yes_no_prompt.h"

// Bug-Catching Contest: shows the Pokemon just caught and, when the player already holds one, asks
// whether to swap it for the new one (the two panels slide apart and the kept one stays).

#define SWAP_MON_PANEL_WINDOWS 6

enum BugContestSwapMonState {
    SWAP_MON_STATE_SHOW_CAUGHT_MON,
    SWAP_MON_STATE_CHOOSE_MON,
};

enum SwapMonPanelWindow {
    SWAP_MON_PANEL_WIN_NAME,
    SWAP_MON_PANEL_WIN_GENDER,
    SWAP_MON_PANEL_WIN_LEVEL,
    SWAP_MON_PANEL_WIN_HP,
    SWAP_MON_PANEL_WIN_MAX_HP,
    SWAP_MON_PANEL_WIN_HP_BAR,
};

// One Pokemon's panel: a frame sprite with the Pokemon's icon, name, gender, level and HP windows.
typedef struct SwapMonPanel {
    Pokemon *mon;
    ManagedSprite *sprites[2]; // frame, icon
    Window windows[SWAP_MON_PANEL_WINDOWS];
    void *scrolling; // non-NULL while the panel slides to targetY
    s16 targetY;
    s16 x;
    s16 y;
    u16 unk_76;
} SwapMonPanel;

typedef struct SwapMonPanels {
    enum HeapID heapID;
    BgConfig *bgConfig;
    struct SwapMonText *text;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    s8 iconPalette;
    SwapMonPanel *panels[2];
} SwapMonPanels;

typedef struct SwapMonText {
    BgConfig *bgConfig;
    MessagePrinter *messagePrinter;
    MessageFormat *messageFormat;
    MsgData *msgData;
    String *string;
    Options *options;
    enum HeapID heapID;
    Window messageBox;
    int printerId;
    u32 waitFrames : 16;
    u32 bgLayer : 3;
    u32 skipWait : 1;
    u32 unused : 12;
} SwapMonText;

typedef struct BugContestSwapMon {
    enum HeapID heapID;
    BugContestSwapMonArgs *args;
    BgConfig *bgConfig;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *cursorSprite;
    YesNoPrompt *yesNoPrompt;
    NARC *narc;
    SwapMonPanels *panels;
    SwapMonText *text;
    s8 iconPalette;
    int state;
    int substate;
} BugContestSwapMon;

typedef struct SwapMonPanelWindowTemplate {
    s16 x;
    s16 y;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile;
} SwapMonPanelWindowTemplate;

typedef BOOL (*BugContestSwapMonStateFunc)(BugContestSwapMon *swapMon);

// SwapMonText_PrintString(WithColor) pass x and y to SwapMonText_PrintOnWindow untruncated
typedef void (*SwapMonTextPrintOnWindowUntruncated)(BgConfig *bgConfig, Window *window, MessageFormat *messageFormat, MsgData *msgData, int msgId, int bgLayer, int x, int y, u32 color, enum HeapID heapID);

// message_printer_number.h declares glyphId as u8; SwapMonText_PrintNumber passes it untruncated
void sub_0200CE7C(MessagePrinter *msgPrinter, int glyphId, u32 num, u32 ndigits, PrintingMode mode, Window *window, u32 x, u32 y);

typedef struct BugContestSwapMonBgTemplates {
    BgTemplate templates[5];
} BugContestSwapMonBgTemplates;

extern const u8 _021E6B74[5];
extern const GraphicsModes ov111_021E6B7C;
extern const OamCharTransferParam ov111_021E6B8C;
extern const SpriteResourceCountsListUnion ov111_021E6BA0;
extern const OamManagerParam ov111_021E6BB8;
extern const GraphicsBanks ov111_021E6BD8;
extern const ManagedSpriteTemplate ov111_021E6C00;
extern const ManagedSpriteTemplate ov111_021E6C34;
extern const SwapMonPanelWindowTemplate ov111_021E6C68[SWAP_MON_PANEL_WINDOWS];
extern const BugContestSwapMonBgTemplates ov111_021E6CA4;
extern const BugContestSwapMonStateFunc _021E6D40[];

BOOL BugContestSwapMon_ShowCaughtMon(BugContestSwapMon *swapMon);
BOOL BugContestSwapMon_ChooseMon(BugContestSwapMon *swapMon);
void BugContestSwapMon_ShowBothMons(BugContestSwapMon *swapMon);
void BugContestSwapMon_InitYesNoPrompt(BugContestSwapMon *swapMon);
int BugContestSwapMon_GetInitialState(int noPokemonCaught);
void BugContestSwapMon_SetBanks(void);
void BugContestSwapMon_ResetDisplay(void);
void BugContestSwapMon_SetResult(BugContestSwapMon *swapMon, int choice);
void BugContestSwapMon_DrawHpBar(Window *window, u16 hp, u16 maxHp);
void BugContestSwapMon_VBlank(void *data);
void BugContestSwapMon_Setup(OverlayManager *man);
void BugContestSwapMon_Teardown(OverlayManager *man);
void BugContestSwapMon_InitBgs(BugContestSwapMon *swapMon);
void BugContestSwapMon_FreeBgs(BugContestSwapMon *swapMon);
void BugContestSwapMon_LoadBgGfx(BugContestSwapMon *swapMon);
void BugContestSwapMon_InitSpriteSystem(BugContestSwapMon *swapMon);
void BugContestSwapMon_FreeSpriteSystem(SpriteSystem *spriteSystem, SpriteManager *spriteManager);
void BugContestSwapMon_LoadSpriteGfx(BugContestSwapMon *swapMon);
void BugContestSwapMon_SetMonIcon(ManagedSprite *sprite, Pokemon *mon, int paletteOffset, enum HeapID heapID);
void BugContestSwapMon_LoadSpriteChar(ManagedSprite *sprite, void *data, u32 size);
ManagedSprite *BugContestSwapMon_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, u8 priority);
ManagedSprite *BugContestSwapMon_NewMonIconSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 slot);
SwapMonPanel *SwapMonPanel_New(SwapMonPanels *panels, u16 x, u16 y, int slot, Pokemon *mon);
void SwapMonPanel_Draw(SwapMonPanel *panel, SwapMonText *text);
void SwapMonPanel_StartScroll(SwapMonPanel *panel, void *scrolling, s16 targetY);
void SwapMonPanel_Scroll(SwapMonPanel *panel);
BOOL SwapMonPanel_IsScrollDone(SwapMonPanel *panel);
void SwapMonPanel_Delete(SwapMonPanel *panel);
SwapMonPanels *SwapMonPanels_New(enum HeapID heapID, BgConfig *bgConfig, SwapMonText *text, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s8 iconPalette);
void SwapMonPanels_Delete(SwapMonPanels *panels);
void SwapMonPanels_Add(SwapMonPanels *panels, int slot, u16 x, u16 y, Pokemon *mon);
void SwapMonPanels_StartScroll(SwapMonPanels *panels, int slot, void *scrolling, s16 targetY);
void SwapMonPanels_Scroll(SwapMonPanels *panels);
BOOL SwapMonPanels_IsScrollDone(SwapMonPanels *panels);
SwapMonText *SwapMonText_New(enum HeapID heapID);
void SwapMonText_Init(SwapMonText *text, BgConfig *bgConfig, int bgLayer, Options *options);
void SwapMonText_Delete(SwapMonText *text);
BOOL SwapMonText_IsPrinting(SwapMonText *text);
void SwapMonText_PrintMessage(SwapMonText *text, int msgId);
void SwapMonText_PrintString(SwapMonText *text, Window *window, int msgId, int x, u8 y);
void SwapMonText_PrintStringWithColor(SwapMonText *text, Window *window, int msgId, int x, u8 y, u32 color);
void SwapMonText_PrintNumber(SwapMonText *text, Window *window, int value, int digits, BOOL withGlyph, int glyphId);
void SwapMonText_CloseMessageBox(SwapMonText *text);
MessageFormat *SwapMonText_GetMessageFormat(SwapMonText *text);
void SwapMonText_LoadGfx(BgConfig *bgConfig, int bgLayer, enum HeapID heapID);
u8 SwapMonText_ShowMessageBox(BgConfig *bgConfig, Window *window, String **string, MessageFormat *messageFormat, MsgData *msgData, int msgId, u8 bgLayer, enum HeapID heapID, Options *options);
void SwapMonText_PrintOnWindow(BgConfig *bgConfig, Window *window, MessageFormat *messageFormat, MsgData *msgData, int msgId, int bgLayer, u8 x, u8 y, u32 color, enum HeapID heapID);

BOOL BugContestSwapMon_Init(OverlayManager *man, int *state) {
    BugContestSwapMon *swapMon;
    switch (*state) {
    case 0:
        BugContestSwapMon_Setup(man);
        (*state)++;
        break;
    case 1:
        swapMon = OverlayManager_GetData(man);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, swapMon->heapID);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            *state = 0;
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL BugContestSwapMon_Exit(OverlayManager *man, int *state) {
    BugContestSwapMon *swapMon = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, swapMon->heapID);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            BugContestSwapMon_Teardown(man);
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL BugContestSwapMon_Main(OverlayManager *man, int *state) {
    BugContestSwapMon *swapMon = OverlayManager_GetData(man);
    if (swapMon == NULL) {
        GF_AssertFail();
    }
    if (_021E6D40[swapMon->state](swapMon)) {
        return TRUE;
    }
    return FALSE;
}

BOOL BugContestSwapMon_ShowCaughtMon(BugContestSwapMon *swapMon) {
    int msgId = -1;
    Pokemon *mon;
    MessageFormat *messageFormat = SwapMonText_GetMessageFormat(swapMon->text);
    if (SwapMonText_IsPrinting(swapMon->text)) {
        return FALSE;
    }
    switch (swapMon->substate) {
    case 0:
        mon = swapMon->args->newlyCaughtMon;
        SwapMonPanels_Add(swapMon->panels, 0, 128, 72, mon);
        BufferBoxMonSpeciesName(messageFormat, 0, Mon_GetBoxMon(mon));
        msgId = 0;
        swapMon->substate = 2;
        break;
    case 1:
        if (!SwapMonText_IsPrinting(swapMon->text)) {
            swapMon->substate = 2;
        }
        break;
    case 2:
        if (System_GetTouchNew() || (gSystem.newKeys & PAD_BUTTON_A)) {
            PlaySE(SEQ_SE_DP_SELECT);
            swapMon->substate = 3;
        }
        break;
    case 3:
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    if (msgId != -1) {
        SwapMonText_PrintMessage(swapMon->text, msgId);
    }
    return FALSE;
}

BOOL BugContestSwapMon_ChooseMon(BugContestSwapMon *swapMon) {
    int msgId = -1;
    SwapMonText_GetMessageFormat(swapMon->text);
    if (SwapMonText_IsPrinting(swapMon->text)) {
        return FALSE;
    }
    switch (swapMon->substate) {
    case 0:
        msgId = 1;
        BugContestSwapMon_ShowBothMons(swapMon);
        swapMon->substate = msgId;
        break;
    case 1:
        BugContestSwapMon_InitYesNoPrompt(swapMon);
        swapMon->substate = 2;
        break;
    case 2:
        switch (YesNoPrompt_HandleInput(swapMon->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(swapMon->yesNoPrompt);
            ManagedSprite_SetDrawFlag(swapMon->cursorSprite, FALSE);
            SwapMonPanels_StartScroll(swapMon->panels, 0, (void *)TRUE, 72);
            SwapMonPanels_StartScroll(swapMon->panels, 1, (void *)TRUE, 242);
            BugContestSwapMon_SetResult(swapMon, 1);
            swapMon->substate = 3;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(swapMon->yesNoPrompt);
            ManagedSprite_SetDrawFlag(swapMon->cursorSprite, FALSE);
            SwapMonPanels_StartScroll(swapMon->panels, 0, (void *)TRUE, -50);
            SwapMonPanels_StartScroll(swapMon->panels, 1, (void *)TRUE, 72);
            BugContestSwapMon_SetResult(swapMon, 2);
            swapMon->substate = 4;
            break;
        }
        break;
    case 3:
        SwapMonPanels_Scroll(swapMon->panels);
        if (SwapMonPanels_IsScrollDone(swapMon->panels)) {
            msgId = 2;
            swapMon->substate = 5;
        }
        break;
    case 4:
        SwapMonPanels_Scroll(swapMon->panels);
        if (SwapMonPanels_IsScrollDone(swapMon->panels)) {
            msgId = 3;
            swapMon->substate = 5;
        }
        break;
    case 5:
        if (System_GetTouchNew() || (gSystem.newKeys & PAD_BUTTON_A)) {
            PlaySE(SEQ_SE_DP_SELECT);
            swapMon->substate = 6;
        }
        break;
    case 6:
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    if (msgId != -1) {
        SwapMonText_PrintMessage(swapMon->text, msgId);
    }
    return FALSE;
}

void BugContestSwapMon_ShowBothMons(BugContestSwapMon *swapMon) {
    MessageFormat *messageFormat = SwapMonText_GetMessageFormat(swapMon->text);
    Pokemon *newlyCaughtMon = swapMon->args->newlyCaughtMon;
    Pokemon *currentMon = swapMon->args->currentMon;
    ManagedSprite *sprite;
    SwapMonPanels_Add(swapMon->panels, 0, 128, 32, newlyCaughtMon);
    SwapMonPanels_Add(swapMon->panels, 1, 128, 112, currentMon);
    BufferBoxMonSpeciesName(messageFormat, 0, Mon_GetBoxMon(newlyCaughtMon));
    BufferBoxMonSpeciesName(messageFormat, 1, Mon_GetBoxMon(currentMon));
    sprite = BugContestSwapMon_NewSprite(swapMon->spriteSystem, swapMon->spriteManager, 128, 72, 1, 0);
    swapMon->cursorSprite = sprite;
    ManagedSprite_SetPaletteOverride(sprite, 1);
}

void BugContestSwapMon_InitYesNoPrompt(BugContestSwapMon *swapMon) {
    YesNoPromptTemplate template = { 0 };
    template.bgConfig = swapMon->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_1;
    template.tileStart = 200;
    template.plttSlot = 6;
    template.x = 25;
    template.y = 10;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(swapMon->yesNoPrompt, &template);
}

int BugContestSwapMon_GetInitialState(int noPokemonCaught) {
    switch (noPokemonCaught) {
    case FALSE:
        goto chooseMon;
    case TRUE:
        goto showCaughtMon;
    default:
        goto invalid;
    }
chooseMon:
    return SWAP_MON_STATE_CHOOSE_MON;
showCaughtMon:
    return SWAP_MON_STATE_SHOW_CAUGHT_MON;
invalid:
    GF_AssertFail();
    GF_AssertFail();
    return SWAP_MON_STATE_SHOW_CAUGHT_MON;
}

void BugContestSwapMon_SetBanks(void) {
    GraphicsBanks banks = ov111_021E6BD8;

    GfGfx_SetBanks(&banks);
}

void BugContestSwapMon_ResetDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
    reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
}

void BugContestSwapMon_SetResult(BugContestSwapMon *swapMon, int choice) {
    switch (choice) {
    case 1: {
        BugContestSwapMonArgs *args = swapMon->args;
        args->unk10 = args->newlyCaughtMon;
        return;
    }
    case 2: {
        BugContestSwapMonArgs *args = swapMon->args;
        args->unk10 = args->currentMon;
        return;
    }
    default:
        GF_AssertFail();
    }
}

void BugContestSwapMon_DrawHpBar(Window *window, u16 hp, u16 maxHp) {
    BgConfig *bgConfig = GetWindowBgConfig(window);
    u8 bgId = GetWindowBgId(window);
    u8 x = GetWindowX(window);
    u8 y = GetWindowY(window);
    u32 pixels = CalculateHpBarPixelsLength(hp, maxHp, 48);
    u8 tile;
    u8 i;

    switch (CalculateHpBarColor(hp, maxHp, 48)) {
    case 0:
    case 3:
    case 4:
        tile = 1;
        break;
    case 2:
        tile = 10;
        break;
    case 1:
        tile = 19;
        break;
    default:
        GF_AssertFail();
        break;
    }
    for (i = 0; i < 6; i++) {
        u16 fill;
        if (pixels >= 8) {
            fill = tile + 8;
        } else {
            fill = tile + pixels;
        }
        FillBgTilemapRect(bgConfig, bgId, fill, x + i, y, 1, 1, TILEMAP_FILL_OVWT_PAL);
        if (pixels < 8) {
            pixels = 0;
        } else {
            pixels = (u8)(pixels - 8);
        }
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}

void BugContestSwapMon_VBlank(void *data) {
    BugContestSwapMon *swapMon = data;
    vu32 *dtcm = (vu32 *)0x027E0000;

    if (swapMon == NULL) {
        GF_AssertFail();
    }
    if (swapMon->spriteManager == NULL) {
        GF_AssertFail();
    }
    if (swapMon->bgConfig == NULL) {
        GF_AssertFail();
    }
    SpriteSystem_DrawSprites(swapMon->spriteManager);
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(swapMon->bgConfig);
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

#pragma force_active on
void BugContestSwapMon_Setup(OverlayManager *man) {
    BugContestSwapMonArgs *args = OverlayManager_GetArgs(man);
    BugContestSwapMon *swapMon;
    GF_ASSERT(args);
    Heap_Create(HEAP_ID_3, HEAP_ID_148, 0x30000);
    swapMon = OverlayManager_CreateAndGetData(man, sizeof(BugContestSwapMon), HEAP_ID_148);
    MI_CpuFill8(swapMon, 0, sizeof(BugContestSwapMon));
    swapMon->heapID = HEAP_ID_148;
    swapMon->args = args;
    swapMon->state = BugContestSwapMon_GetInitialState((u8)args->noPokemonCaught);
    BugContestSwapMon_ResetDisplay();
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    BugContestSwapMon_SetBanks();
    BugContestSwapMon_InitBgs(swapMon);
    BugContestSwapMon_LoadBgGfx(swapMon);
    BugContestSwapMon_InitSpriteSystem(swapMon);
    BugContestSwapMon_LoadSpriteGfx(swapMon);
    swapMon->yesNoPrompt = YesNoPrompt_Create(swapMon->heapID);
    swapMon->text = SwapMonText_New(swapMon->heapID);
    SwapMonText_Init(swapMon->text, swapMon->bgConfig, GF_BG_LYR_MAIN_1, args->options);
    swapMon->panels = SwapMonPanels_New(swapMon->heapID, swapMon->bgConfig, swapMon->text, swapMon->spriteSystem, swapMon->spriteManager, swapMon->iconPalette);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    Main_SetVBlankIntrCB(BugContestSwapMon_VBlank, swapMon);
}
#pragma force_active reset

void BugContestSwapMon_Teardown(OverlayManager *man) {
    BugContestSwapMon *swapMon = OverlayManager_GetData(man);
    YesNoPrompt_Destroy(swapMon->yesNoPrompt);
    SwapMonPanels_Delete(swapMon->panels);
    SwapMonText_Delete(swapMon->text);
    if (swapMon->cursorSprite != NULL) {
        Sprite_DeleteAndFreeResources(swapMon->cursorSprite);
        swapMon->cursorSprite = NULL;
    }
    BugContestSwapMon_FreeSpriteSystem(swapMon->spriteSystem, swapMon->spriteManager);
    BugContestSwapMon_FreeBgs(swapMon);
    BugContestSwapMon_ResetDisplay();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_148);
}

#pragma force_active on
void BugContestSwapMon_InitBgs(BugContestSwapMon *swapMon) {
    GraphicsModes modes;
    BugContestSwapMonBgTemplates templates;
    int i;
    const u8 *bgIds;
    const BgTemplate *bgTemplate;

    swapMon->bgConfig = BgConfig_Alloc(swapMon->heapID);
    modes = ov111_021E6B7C;
    SetBothScreensModesAndDisable(&modes);
    templates = ov111_021E6CA4;
    bgIds = _021E6B74;
    i = 0;
    bgTemplate = templates.templates;
    for (; i < 5; bgTemplate++, bgIds++, i++) {
        InitBgFromTemplate(swapMon->bgConfig, (GFBgLayer)*bgIds, bgTemplate, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(swapMon->bgConfig, (GFBgLayer)*bgIds);
        BG_FillCharDataRange(swapMon->bgConfig, (GFBgLayer)*bgIds, 0, 1, 0);
    }
    swapMon->narc = NARC_New(NARC_a_1_7_3, swapMon->heapID);
}
#pragma force_active reset

void BugContestSwapMon_FreeBgs(BugContestSwapMon *swapMon) {
    int i;
    NARC_Delete(swapMon->narc);
    for (i = 0; i < 5; i++) {
        FreeBgTilemapBuffer(swapMon->bgConfig, _021E6B74[i]);
    }
    Heap_Free(swapMon->bgConfig);
}

void BugContestSwapMon_LoadBgGfx(BugContestSwapMon *swapMon) {
    if (swapMon->bgConfig == NULL) {
        GF_AssertFail();
    }
    GfGfxLoader_GXLoadPalFromOpenNarc(swapMon->narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x200, swapMon->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(swapMon->narc, 1, swapMon->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, swapMon->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(swapMon->narc, 2, swapMon->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, swapMon->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(swapMon->narc, 4, swapMon->bgConfig, GF_BG_LYR_MAIN_2, 1, 0, TRUE, swapMon->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(swapMon->narc, 4, swapMon->bgConfig, GF_BG_LYR_MAIN_3, 1, 0, TRUE, swapMon->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(swapMon->narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200, swapMon->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(swapMon->narc, 1, swapMon->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, swapMon->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(swapMon->narc, 3, swapMon->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, swapMon->heapID);
}

void BugContestSwapMon_InitSpriteSystem(BugContestSwapMon *swapMon) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion resourceCounts;

    swapMon->spriteSystem = SpriteSystem_Alloc(swapMon->heapID);
    swapMon->spriteManager = SpriteManager_New(swapMon->spriteSystem);
    oamManagerParam = ov111_021E6BB8;
    oamCharTransferParam = ov111_021E6B8C;
    oamCharTransferParam.maxTasks = 32;
    SpriteSystem_Init(swapMon->spriteSystem, &oamManagerParam, &oamCharTransferParam, 32);
    SpriteSystem_InitSprites(swapMon->spriteSystem, swapMon->spriteManager, 32);
    resourceCounts = ov111_021E6BA0;
    SpriteSystem_InitManagerWithCapacities(swapMon->spriteSystem, swapMon->spriteManager, &resourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(swapMon->spriteSystem), 0, FX32_CONST(524));
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void BugContestSwapMon_FreeSpriteSystem(SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    SpriteSystem_FreeResourcesAndManager(spriteSystem, spriteManager);
    SpriteSystem_Free(spriteSystem);
}

void BugContestSwapMon_LoadSpriteGfx(BugContestSwapMon *swapMon) {
    int i;
    NARC *narc;
    SpriteSystem *spriteSystem = swapMon->spriteSystem;
    SpriteManager *spriteManager = swapMon->spriteManager;

    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, swapMon->narc, 5, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, swapMon->narc, 6, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, swapMon->narc, 7, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, swapMon->narc, 8, TRUE, 0);
    narc = NARC_New(NARC_poketool_icongra_poke_icon, swapMon->heapID);
    for (i = 0; i < 2; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, swapMon->narc, 9, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, i + 1);
    }
    swapMon->iconPalette = SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, narc, sub_02074490(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, sub_0207449C(), FALSE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, sub_020744A8(), FALSE, 1);
    NARC_Delete(narc);
}

void BugContestSwapMon_SetMonIcon(ManagedSprite *sprite, Pokemon *mon, int paletteOffset, enum HeapID heapID) {
    NNSG2dCharacterData *charData;
    u8 palette;
    NARC *narc = NARC_New(NARC_poketool_icongra_poke_icon, heapID);
    void *raw = GfGfxLoader_GetCharDataFromOpenNarc(narc, Pokemon_GetIconNaix(mon), FALSE, &charData, heapID);

    BugContestSwapMon_LoadSpriteChar(sprite, charData->pRawData, 0x400);
    Heap_Free(raw);
    palette = paletteOffset + Pokemon_GetIconPalette(mon);
    ManagedSprite_SetPaletteOverride(sprite, palette);
    ManagedSprite_SetAnim(sprite, 1);
    ManagedSprite_SetAnimSpeed(sprite, FX32_ONE);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    NARC_Delete(narc);
}

void BugContestSwapMon_LoadSpriteChar(ManagedSprite *sprite, void *data, u32 size) {
    NNS_G2D_VRAM_TYPE vramType = Sprite_GetVramType(sprite->sprite);
    u32 location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite->sprite), vramType);

    DC_FlushRange(data, size);
    switch (vramType) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ(data, location, size);
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ(data, location, size);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

ManagedSprite *BugContestSwapMon_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, u8 priority) {
    ManagedSpriteTemplate template = ov111_021E6C34;
    ManagedSprite *sprite;

    template.x = x;
    template.y = y;
    template.drawPriority = priority;
    template.animation = animation;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(524));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

ManagedSprite *BugContestSwapMon_NewMonIconSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 slot) {
    ManagedSpriteTemplate template = ov111_021E6C00;

    if (slot >= 2) {
        GF_AssertFail();
    }
    template.resIdList[0] = slot + 1;
    template.x = x;
    template.y = y;
    return SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(524));
}

#pragma force_active on
SwapMonPanel *SwapMonPanel_New(SwapMonPanels *panels, u16 x, u16 y, int slot, Pokemon *mon) {
    SwapMonPanel *panel;
    u8 bgId = slot == 0 ? GF_BG_LYR_MAIN_2 : GF_BG_LYR_MAIN_3;
    panel = Heap_Alloc(panels->heapID, sizeof(SwapMonPanel));
    s32 i;
    MI_CpuFill8(panel, 0, sizeof(SwapMonPanel));
    panel->mon = mon;
    panel->x = x;
    panel->y = y;
    panel->sprites[0] = BugContestSwapMon_NewSprite(panels->spriteSystem, panels->spriteManager, panel->x, panel->y, 0, 1);
    panel->sprites[1] = BugContestSwapMon_NewMonIconSprite(panels->spriteSystem, panels->spriteManager, panel->x - 44, panel->y - 8, slot);
    for (i = 0; i < SWAP_MON_PANEL_WINDOWS; i++) {
        AddWindowParameterized(panels->bgConfig, &panel->windows[i], bgId, (u8)((u8)(x >> 3) + ov111_021E6C68[i].x), (u8)((u8)(y >> 3) + ov111_021E6C68[i].y), ov111_021E6C68[i].width, ov111_021E6C68[i].height, ov111_021E6C68[i].palette, ov111_021E6C68[i].baseTile);
    }
    return panel;
}

void SwapMonPanel_Draw(SwapMonPanel *panel, SwapMonText *text) {
    MessageFormat *messageFormat = SwapMonText_GetMessageFormat(text);
    Pokemon *mon = panel->mon;
    int hp;
    int maxHp;
    int genderMsg;
    int color;
    GF_ASSERT(panel);
    GF_ASSERT(text);
    hp = GetMonData(mon, MON_DATA_HP, NULL);
    maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    BufferBoxMonSpeciesName(messageFormat, 0, Mon_GetBoxMon(mon));
    SwapMonText_PrintString(text, &panel->windows[SWAP_MON_PANEL_WIN_NAME], 4, 4, 1);
    color = MAKE_TEXT_COLOR(7, 8, 0);
    switch (GetMonGender(mon)) {
    case MON_MALE:
        genderMsg = 5;
        break;
    case MON_FEMALE:
        genderMsg = 6;
        color = MAKE_TEXT_COLOR(3, 4, 0);
        break;
    case MON_GENDERLESS:
        genderMsg = 7;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    SwapMonText_PrintStringWithColor(text, &panel->windows[SWAP_MON_PANEL_WIN_GENDER], genderMsg, 0, 1, color);
    SwapMonText_PrintNumber(text, &panel->windows[SWAP_MON_PANEL_WIN_LEVEL], GetMonData(mon, MON_DATA_LEVEL, NULL), 3, TRUE, 1);
    SwapMonText_PrintNumber(text, &panel->windows[SWAP_MON_PANEL_WIN_HP], hp, 3, FALSE, 0);
    SwapMonText_PrintNumber(text, &panel->windows[SWAP_MON_PANEL_WIN_MAX_HP], maxHp, 3, TRUE, 0);
    BugContestSwapMon_DrawHpBar(&panel->windows[SWAP_MON_PANEL_WIN_HP_BAR], (u16)hp, (u16)maxHp);
}
#pragma force_active reset

void SwapMonPanel_StartScroll(SwapMonPanel *panel, void *scrolling, s16 targetY) {
    panel->scrolling = scrolling;
    panel->targetY = targetY;
}

#pragma force_active on
void SwapMonPanel_Scroll(SwapMonPanel *panel) {
    BgConfig *bgConfig;
    Window *window;
    u8 bgId;
    int i;
    int step;
    struct {
        s16 y;
        s16 x;
    } pos;

    if (panel == NULL) {
        GF_AssertFail();
    }
    window = &panel->windows[0];
    bgConfig = GetWindowBgConfig(window);
    bgId = GetWindowBgId(window);
    {
        int current = panel->y;
        int target = panel->targetY;
        int distance = current - target;
        if (distance < 0) {
            distance = -distance;
        }
        distance = (s16)distance;
        step = distance >> 2;
        if (distance <= 1 || step == 0) {
            panel->scrolling = NULL;
        } else {
            if (current > target) {
                step = (s16)(~0u * (u32)step); // -step
            }
            panel->y += step;
            if (panel->y < 232) {
                ScheduleSetBgPosText(bgConfig, bgId, BG_POS_OP_SUB_Y, step);
                for (i = 0; i < 2; i++) {
                    ManagedSprite_GetPositionXYWithSubscreenOffset(panel->sprites[i], &pos.x, &pos.y, FX32_CONST(524));
                    pos.y += step;
                    ManagedSprite_SetPositionXYWithSubscreenOffset(panel->sprites[i], pos.x, pos.y, FX32_CONST(524));
                }
            }
        }
    }
    BgCommitTilemapBufferToVram(bgConfig, bgId);
}
#pragma force_active reset

BOOL SwapMonPanel_IsScrollDone(SwapMonPanel *panel) {
    return panel->scrolling == NULL;
}

void SwapMonPanel_Delete(SwapMonPanel *panel) {
    int i;

    GF_ASSERT(panel != NULL);
    for (i = 0; i < 2; i++) {
        if (panel->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(panel->sprites[i]);
            panel->sprites[i] = NULL;
        }
    }
    for (i = 0; i < SWAP_MON_PANEL_WINDOWS; i++) {
        ClearWindowTilemapAndCopyToVram(&panel->windows[i]);
        RemoveWindow(&panel->windows[i]);
    }
    Heap_Free(panel);
}

SwapMonPanels *SwapMonPanels_New(enum HeapID heapID, BgConfig *bgConfig, SwapMonText *text, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s8 iconPalette) {
    SwapMonPanels *panels = Heap_Alloc(heapID, sizeof(SwapMonPanels));

    MI_CpuFill8(panels, 0, sizeof(SwapMonPanels));
    panels->bgConfig = bgConfig;
    panels->heapID = heapID;
    panels->text = text;
    panels->spriteSystem = spriteSystem;
    panels->spriteManager = spriteManager;
    panels->iconPalette = iconPalette;
    return panels;
}

void SwapMonPanels_Delete(SwapMonPanels *panels) {
    int i;

    GF_ASSERT(panels != NULL);
    for (i = 0; i < 2; i++) {
        if (panels->panels[i] != NULL) {
            SwapMonPanel_Delete(panels->panels[i]);
        }
    }
    Heap_Free(panels);
}

void SwapMonPanels_Add(SwapMonPanels *panels, int slot, u16 x, u16 y, Pokemon *mon) {
    SwapMonPanel *panel = SwapMonPanel_New(panels, x, y, slot, mon);

    BugContestSwapMon_SetMonIcon(panel->sprites[1], panel->mon, panels->iconPalette, panels->heapID);
    SwapMonPanel_Draw(panel, panels->text);
    panels->panels[slot] = panel;
}

void SwapMonPanels_StartScroll(SwapMonPanels *panels, int slot, void *scrolling, s16 targetY) {
    SwapMonPanel_StartScroll(panels->panels[slot], scrolling, targetY);
}

#pragma force_active on
void SwapMonPanels_Scroll(SwapMonPanels *panels) {
    int i;
    u8 *slot = (u8 *)panels;
    // walks panels[] with a byte pointer
    for (i = 0; i < 2; i++, slot += sizeof(SwapMonPanel *)) {
        if (!SwapMonPanel_IsScrollDone(((SwapMonPanels *)slot)->panels[0])) {
            SwapMonPanel_Scroll(((SwapMonPanels *)slot)->panels[0]);
        }
    }
}
#pragma force_active reset

BOOL SwapMonPanels_IsScrollDone(SwapMonPanels *panels) {
    int i;
    u8 *slot = (u8 *)panels;
    for (i = 0; i < 2; i++, slot += sizeof(SwapMonPanel *)) {
        if (!SwapMonPanel_IsScrollDone(((SwapMonPanels *)slot)->panels[0])) {
            return FALSE;
        }
    }
    return TRUE;
}

#pragma force_active on
SwapMonText *SwapMonText_New(enum HeapID heapID) {
    SwapMonText *text = Heap_Alloc(heapID, sizeof(SwapMonText));
    MI_CpuFill8(text, 0, sizeof(SwapMonText));
    text->heapID = heapID;
    text->messagePrinter = MessagePrinter_New(1, 2, 0, heapID);
    return text;
}

void SwapMonText_Init(SwapMonText *text, BgConfig *bgConfig, int bgLayer, Options *options) {
    text->bgConfig = bgConfig;
    text->printerId = -1;
    text->bgLayer = bgLayer;
    text->options = options;
    text->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0029_bin, text->heapID);
    text->messageFormat = MessageFormat_New(text->heapID);
    ResetAllTextPrinters();
    SwapMonText_LoadGfx(text->bgConfig, (u8)text->bgLayer, text->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, text->heapID);
    BG_FillCharDataRange(text->bgConfig, (GFBgLayer)text->bgLayer, 0, 1, 0);
}
#pragma force_active reset

void SwapMonText_Delete(SwapMonText *text) {
    SwapMonText_CloseMessageBox(text);
    DestroyMsgData(text->msgData);
    MessageFormat_Delete(text->messageFormat);
    if (text->string != NULL) {
        String_Delete(text->string);
    }
    MessagePrinter_Delete(text->messagePrinter);
    Heap_Free(text);
}

BOOL SwapMonText_IsPrinting(SwapMonText *text) {
    if (text->printerId != -1) {
        if (!TextPrinterCheckActive((u8)text->printerId)) {
            text->waitFrames = 0;
            String_Delete(text->string);
            text->string = NULL;
            text->printerId = -1;
        }
        return TRUE;
    }
    if (text->waitFrames != 0) {
        if (text->skipWait == TRUE) {
            text->waitFrames = 0;
            text->skipWait = FALSE;
        } else {
            text->waitFrames--;
            return TRUE;
        }
    }
    return FALSE;
}

void SwapMonText_PrintMessage(SwapMonText *text, int msgId) {
    text->printerId = SwapMonText_ShowMessageBox(text->bgConfig, &text->messageBox, &text->string, text->messageFormat, text->msgData, msgId, (u8)text->bgLayer, text->heapID, text->options);
}

void SwapMonText_PrintString(SwapMonText *text, Window *window, int msgId, int x, u8 y) {
    ((SwapMonTextPrintOnWindowUntruncated)SwapMonText_PrintOnWindow)(text->bgConfig, window, text->messageFormat, text->msgData, msgId, (u8)text->bgLayer, x, y, MAKE_TEXT_COLOR(1, 2, 0), text->heapID);
}

void SwapMonText_PrintStringWithColor(SwapMonText *text, Window *window, int msgId, int x, u8 y, u32 color) {
    ((SwapMonTextPrintOnWindowUntruncated)SwapMonText_PrintOnWindow)(text->bgConfig, window, text->messageFormat, text->msgData, msgId, (u8)text->bgLayer, x, y, color, text->heapID);
}

void SwapMonText_PrintNumber(SwapMonText *text, Window *window, int value, int digits, BOOL withGlyph, int glyphId) {
    FillWindowPixelBuffer(window, 0);
    if (withGlyph) {
        sub_0200CE7C(text->messagePrinter, glyphId, value, digits, PRINTING_MODE_RIGHT_ALIGN, window, 0, 0);
    } else {
        PrintUIntOnWindow(text->messagePrinter, value, digits, PRINTING_MODE_RIGHT_ALIGN, window, 0, 0);
    }
    ScheduleWindowCopyToVram(window);
}

void SwapMonText_CloseMessageBox(SwapMonText *text) {
    int printerId = text->printerId;
    if (printerId != -1) {
        RemoveTextPrinter((u8)printerId);
    }
    if (text->messageBox.bgConfig != NULL) {
        ClearFrameAndWindow2(&text->messageBox, FALSE);
        ClearWindowTilemapAndCopyToVram(&text->messageBox);
        RemoveWindow(&text->messageBox);
    }
}

MessageFormat *SwapMonText_GetMessageFormat(SwapMonText *text) {
    if (text == NULL) {
        GF_AssertFail();
    }
    if (text->messageFormat == NULL) {
        GF_AssertFail();
    }
    return text->messageFormat;
}

void SwapMonText_LoadGfx(BgConfig *bgConfig, int bgLayer, enum HeapID heapID) {
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, heapID);
    LoadUserFrameGfx2(bgConfig, (GFBgLayer)bgLayer, 978, 13, 0, heapID);
}

#pragma force_active on
u8 SwapMonText_ShowMessageBox(BgConfig *bgConfig, Window *window, String **string, MessageFormat *messageFormat, MsgData *msgData, int msgId, u8 bgLayer, enum HeapID heapID, Options *options) {
    int textSpeed;
    u8 printerId;
    if (bgConfig == NULL) {
        GF_AssertFail();
    }
    if (options == NULL) {
        GF_AssertFail();
    }
    if (window->bgConfig == NULL) {
        int bgId;
        AddWindowParameterized(bgConfig, window, bgLayer, 2, 19, 27, 4, 15, 1);
        bgId = GetWindowBgId(window);
        LoadUserFrameGfx2(bgConfig, (GFBgLayer)bgId, 978, 13, (u8)Options_GetFrame(options), heapID);
    }
    textSpeed = Options_GetTextFrameDelay(options);
    FillWindowPixelBuffer(window, 15);
    *string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msgId, heapID);
    printerId = AddTextPrinterParameterizedWithColor(window, 1, *string, 0, 0, textSpeed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    DrawFrameAndWindow2(window, FALSE, 978, 13);
    return printerId;
}

void SwapMonText_PrintOnWindow(BgConfig *bgConfig, Window *window, MessageFormat *messageFormat, MsgData *msgData, int msgId, int bgLayer, u8 x, u8 y, u32 color, enum HeapID heapID) {
    String *string;
    FillWindowPixelBuffer(window, 0);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msgId, heapID);
    AddTextPrinterParameterizedWithColor(window, 0, string, x, y, 0, color, NULL);
    String_Delete(string);
}
#pragma force_active reset
