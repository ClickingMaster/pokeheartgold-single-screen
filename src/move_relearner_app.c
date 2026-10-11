#include "move_relearner_app.h"

#include "global.h"

#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "move.h"
#include "move_relearner.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02026E30.h"
#include "type_icon_resources.h"
#include "unk_0208805C.h"
#include "unk_02088288.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

#define MOVE_RELEARNER_NUM_WINDOWS  15
#define MOVE_RELEARNER_NUM_SPRITES  12
#define MOVE_RELEARNER_VISIBLE_ROWS 4

// Main states (the return values of the state handlers).
enum MoveRelearnerState {
    MOVE_RELEARNER_STATE_WAIT_FADE,
    MOVE_RELEARNER_STATE_INPUT,
    MOVE_RELEARNER_STATE_WAIT_MESSAGE,
    MOVE_RELEARNER_STATE_SHOW_YES_NO,
    MOVE_RELEARNER_STATE_HANDLE_YES_NO,
    MOVE_RELEARNER_STATE_TEACH_MOVE,
    MOVE_RELEARNER_STATE_PRINT_LEARNED,
    MOVE_RELEARNER_STATE_ASK_AFTER_SUMMARY,
    MOVE_RELEARNER_STATE_FADE_OUT,
    MOVE_RELEARNER_STATE_EXIT,
    MOVE_RELEARNER_STATE_FADE_OUT_TO_SUMMARY,
    MOVE_RELEARNER_STATE_OPEN_SUMMARY,
    MOVE_RELEARNER_STATE_RUN_SUMMARY,
    MOVE_RELEARNER_STATE_BUTTON_FLASH,
    MOVE_RELEARNER_STATE_ASK_GIVE_UP,
    MOVE_RELEARNER_STATE_ASK_TEACH,
};

// The yes/no question on screen (MoveRelearner.question).
enum MoveRelearnerQuestion {
    MOVE_RELEARNER_QUESTION_TEACH,         // "Teach the move X?"
    MOVE_RELEARNER_QUESTION_GIVE_UP,       // "Give up trying to teach a new move?"
    MOVE_RELEARNER_QUESTION_DELETE_MOVE,   // "Can't learn more than four moves. Delete an older move?"
    MOVE_RELEARNER_QUESTION_STOP_TEACHING, // "Stop trying to teach X?"
    MOVE_RELEARNER_QUESTION_FORGET_MOVE,   // "X is the move that you want forgotten?"
};

// Camera and Pokemon picture of the 3D top screen.
typedef struct MoveRelearnerMonPic {
    Camera *camera;                 // 0x00
    PokepicManager *pokepicManager; // 0x04
    u8 filler_08[0x10];             // 0x08
    Pokepic *pokepic;               // 0x18
} MoveRelearnerMonPic;              // size: 0x1C

// Palette flash of a pressed bottom-screen button, then MoveRelearner_Main continues with nextState.
typedef struct MoveRelearnerButtonFlash {
    u8 x;                   // 0x0
    u8 y;                   // 0x1
    u8 width;               // 0x2
    u8 height;              // 0x3
    u16 timer;              // 0x4
    u16 state;              // 0x6
    int nextState;          // 0x8
} MoveRelearnerButtonFlash; // size: 0xC

typedef struct MoveRelearner {
    MoveRelearnerArgs *args;                            // 0x000
    BgConfig *bgConfig;                                 // 0x004
    Window windows[MOVE_RELEARNER_NUM_WINDOWS];         // 0x008
    MsgData *msgData;                                   // 0x0F8
    MessageFormat *msgFormat;                           // 0x0FC
    String *string;                                     // 0x100
    String *ppLabel;                                    // 0x104
    String *ppFormat;                                   // 0x108
    u8 filler_10C[4];                                   // 0x10C
    ListMenuItem *moveList;                             // 0x110
    u8 filler_114[4];                                   // 0x114
    YesNoPrompt *yesNoPrompt;                           // 0x118
    SpriteSystem *spriteSystem;                         // 0x11C
    SpriteManager *spriteManager;                       // 0x120
    ManagedSprite *sprites[MOVE_RELEARNER_NUM_SPRITES]; // 0x124, see MoveRelearner_InitSprites
    MoveRelearnerMonPic monPic;                         // 0x154
    PokemonSummaryArgs summary;                         // 0x170
    OverlayManager *summaryManager;                     // 0x1AC
    int nextState;                                      // 0x1B0, state after the current message
    MenuInputState menuInputState;                      // 0x1B4
    u8 numMoves;                                        // 0x1B8, entries of moveList
    u8 textPrinterId;                                   // 0x1B9
    u8 question;                                        // 0x1BA, enum MoveRelearnerQuestion
    u8 unk_1BB;                                         // 0x1BB
    u8 unk_1BC;                                         // 0x1BC
    u8 filler_1BD[3];                                   // 0x1BD
    void *screenFile;                                   // 0x1C0
    NNSG2dScreenData *screenData;                       // 0x1C4, button tiles copied to the bottom screen
    GridInputHandler *gridInput;                        // 0x1C8
    u16 lastGridInput;                                  // 0x1CC
    u16 learnButtonActive;                              // 0x1CE
    MoveRelearnerButtonFlash buttonFlash;               // 0x1D0
} MoveRelearner;                                        // size: 0x1DC

// One row of the yes/no answer table. sMoveRelearnerYesActions and sMoveRelearnerNoActions
// label the same table (the second one 4 bytes in), so each one's "other" member is the
// callback of the opposite answer.
typedef struct MoveRelearnerYesNoAction {
    int (*callback)(MoveRelearner *relearner);
    int (*other)(MoveRelearner *relearner);
} MoveRelearnerYesNoAction;

// Defined in type_icon_resources.c; not declared in unk_02077678.h.
void TypeIcon_LoadPltt(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int vram, int resId);
u32 TypeIcon_GetPalette(u32 type);
int MoveCategoryIcon_GetCharFile(u32 category);
u32 MoveCategoryIcon_GetPalette(u32 category);
NarcId MoveCategoryIcon_GetNarc(u32 category); // the definition takes no argument; the call passes one
void MoveCategoryIcon_LoadChar(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int vram, int category, int resId);

extern const u8 sMoveRelearnerSummaryPages[2];
extern const VecFx32 sMoveRelearnerCameraPos;
extern const GraphicsModes sMoveRelearnerGraphicsModes;
extern const GridCallbacks sMoveRelearnerGridCallbacks;
extern const OamCharTransferParam sMoveRelearnerOamCharTransferParam;
extern const SpriteResourceCountsListUnion sMoveRelearnerSpriteResourceCounts;
extern const BgTemplate sMoveRelearnerBgTemplate_Main3;
extern const BgTemplate sMoveRelearnerBgTemplate_Main2;
extern const BgTemplate sMoveRelearnerBgTemplate_Sub0;
extern const BgTemplate sMoveRelearnerBgTemplate_Sub2;
extern const BgTemplate sMoveRelearnerBgTemplate_Sub3;
extern const OamManagerParam sMoveRelearnerOamManagerParam;
extern const TouchscreenHitbox sMoveRelearnerGridHitboxes[];
extern const GraphicsBanks sMoveRelearnerGraphicsBanks;
extern const MoveRelearnerYesNoAction sMoveRelearnerYesActions[];
extern const MoveRelearnerYesNoAction sMoveRelearnerNoActions[];
extern const DpadMenuBox sMoveRelearnerGridDpadBoxes[];
extern const u32 sMoveRelearnerMessageIds[][11]; // [MoveRelearnerArgs.type][message]
extern const WindowTemplate sMoveRelearnerWindowTemplates[];
extern const ManagedSpriteTemplate sMoveRelearnerSpriteTemplates[];

void MoveRelearner_Setup(MoveRelearner *relearner);
void MoveRelearner_Teardown(MoveRelearner *relearner);
void MoveRelearner_VBlankCB(MoveRelearner *relearner);
void MoveRelearner_SetGfxBanks(void);
void MoveRelearner_InitBgs(BgConfig *bgConfig);
void MoveRelearner_FreeBgs(BgConfig *bgConfig);
void MoveRelearner_LoadBgGraphics(MoveRelearner *relearner, NARC *narc);
void MoveRelearner_FreeScreenFile(MoveRelearner *relearner);
void MoveRelearner_AddWindows(MoveRelearner *relearner);
void MoveRelearner_RemoveWindows(MoveRelearner *relearner);
void MoveRelearner_InitStrings(MoveRelearner *relearner);
void MoveRelearner_FreeStrings(MoveRelearner *relearner);
int MoveRelearner_WaitFade(MoveRelearner *relearner);
int MoveRelearner_HandleInput(MoveRelearner *relearner);
int MoveRelearner_WaitMessage(MoveRelearner *relearner);
int MoveRelearner_ShowYesNo(MoveRelearner *relearner);
int MoveRelearner_HandleYesNo(MoveRelearner *relearner);
int MoveRelearner_TeachMove(MoveRelearner *relearner);
int MoveRelearner_PrintLearned(MoveRelearner *relearner);
int MoveRelearner_AskAfterSummary(MoveRelearner *relearner);
int MoveRelearner_FadeOut(MoveRelearner *relearner);
void MoveRelearner_PrintSelectedMove(MoveRelearner *relearner);
void MoveRelearner_PrintString(MoveRelearner *relearner, u32 windowId, u32 fontId, u32 color, u32 alignment, u8 y);
void MoveRelearner_FormatNumber(MoveRelearner *relearner, u32 msgId, s32 value, u32 numDigits, u8 mode);
void MoveRelearner_PrintStaticTexts(MoveRelearner *relearner);
u32 MoveRelearner_CountEligibleMoves(MoveRelearner *relearner);
void MoveRelearner_CopyButtonTiles(MoveRelearner *relearner, u32 x, u32 y, u32 width, u8 height, u8 srcX, u8 srcY);
void MoveRelearner_PrintListRow(MoveRelearner *relearner, int scrollOffset, int row);
void MoveRelearner_PrintMoveList(MoveRelearner *relearner);
void MoveRelearner_InitMoveList(MoveRelearner *relearner);
void MoveRelearner_FreeMoveList(MoveRelearner *relearner);
void MoveRelearner_PrintMoveInfo(MoveRelearner *relearner, s32 move);
void MoveRelearner_FormatMessage(MoveRelearner *relearner, u32 message);
u16 MoveRelearner_GetSelectedMove(MoveRelearner *relearner);
u16 MoveRelearner_GetMoveToReplace(MoveRelearner *relearner);
void MoveRelearner_PrintMessage(MoveRelearner *relearner, u32 message);
void MoveRelearner_ClearMessage(MoveRelearner *relearner);
BOOL MoveRelearner_TextPrinterCallback(TextPrinterTemplate *template, u16 glyphId);
u32 MoveRelearner_CountKnownMoves(MoveRelearner *relearner);
int MoveRelearner_OnTeachYes(MoveRelearner *relearner);
int MoveRelearner_OnTeachNo(MoveRelearner *relearner);
int MoveRelearner_OnGiveUpYes(MoveRelearner *relearner);
int MoveRelearner_OnGiveUpNo(MoveRelearner *relearner);
int MoveRelearner_OnDeleteMoveYes(MoveRelearner *relearner);
int MoveRelearner_OnDeleteMoveNo(MoveRelearner *relearner);
int MoveRelearner_OnStopTeachingYes(MoveRelearner *relearner);
int MoveRelearner_OnStopTeachingNo(MoveRelearner *relearner);
int MoveRelearner_OnForgetMoveYes(MoveRelearner *relearner);
int MoveRelearner_OnForgetMoveNo(MoveRelearner *relearner);
void MoveRelearner_InitSpriteSystem(MoveRelearner *relearner);
void MoveRelearner_FreeSprites(MoveRelearner *relearner);
void MoveRelearner_LoadSpriteResources(MoveRelearner *relearner, NARC *narc);
void MoveRelearner_SetTypeIcon(MoveRelearner *relearner, u16 move, u16 index);
void MoveRelearner_UpdateListTypeIcons(MoveRelearner *relearner);
void MoveRelearner_SetCategoryIcon(MoveRelearner *relearner, int move);
void MoveRelearner_InitSprites(MoveRelearner *relearner, NARC *narc);
void MoveRelearner_CreateCamera(Camera **camera, enum HeapID heapID);
void MoveRelearner_CreatePokepic(MoveRelearnerMonPic *monPic, Pokemon *mon, enum HeapID heapID);
void MoveRelearner_Init3D(MoveRelearner *relearner);
void MoveRelearner_Draw3D(MoveRelearnerMonPic *monPic);
void MoveRelearner_Free3D(MoveRelearnerMonPic *monPic);
void MoveRelearner_SetCursorPos(MoveRelearner *relearner, u32 pos, int palette);
void MoveRelearner_UpdateScrollArrows(MoveRelearner *relearner, int which, BOOL animate);
int MoveRelearner_FadeOutToSummary(MoveRelearner *relearner);
int MoveRelearner_OpenSummary(MoveRelearner *relearner);
int MoveRelearner_RunSummary(MoveRelearner *relearner);
void MoveRelearner_CreateGridInput(MoveRelearner *relearner);
void MoveRelearner_FreeGridInput(MoveRelearner *relearner);
void MoveRelearner_GridNoop(void);
void MoveRelearner_GridOnKeyMove(MoveRelearner *relearner, s32 input, u32 direction);
void MoveRelearner_GridOnTouch(MoveRelearner *relearner, int input);
void MoveRelearner_SetLearnButtonActive(MoveRelearner *relearner, u32 active);
void MoveRelearner_UpdateScrollButtons(MoveRelearner *relearner);
void MoveRelearner_AnimateScrollArrows(MoveRelearner *relearner, s32 direction);
void MoveRelearner_DrawListRows(MoveRelearner *relearner, u32 selected);
int MoveRelearner_StartReturnButtonFlash(MoveRelearner *relearner, int nextState);
int MoveRelearner_StartLearnButtonFlash(MoveRelearner *relearner, int nextState);
int MoveRelearner_UpdateButtonFlash(MoveRelearner *relearner);
int MoveRelearner_AskGiveUp(MoveRelearner *relearner);
int MoveRelearner_AskTeachAfterFlash(MoveRelearner *relearner);
int MoveRelearner_AskTeach(MoveRelearner *relearner);
// Defined in to80_extra1791_run01.c (after the overlay's data). Not in the public header: its includers
// (launch_application.c) would see their static-local symbol numbering shift.
void MoveRelearner_DimSubScreen(int on);

BOOL MoveRelearner_Init(OverlayManager *man, int *state) {
    MoveRelearner *relearner;

    Heap_Create(HEAP_ID_3, HEAP_ID_66, 9 << 14);
    relearner = OverlayManager_CreateAndGetData(man, sizeof(MoveRelearner), HEAP_ID_66);
    memset(relearner, 0, sizeof(MoveRelearner));
    relearner->args = OverlayManager_GetArgs(man);
    MoveRelearner_Setup(relearner);
    MoveRelearner_SetCursorPos(relearner, (u8)relearner->args->cursorPos, 3);
    relearner->nextState = MOVE_RELEARNER_STATE_INPUT;
    MoveRelearner_UpdateScrollArrows(relearner, 0, FALSE);
    Main_SetVBlankIntrCB((GFIntrCB)MoveRelearner_VBlankCB, relearner);
    return TRUE;
}

BOOL MoveRelearner_Main(OverlayManager *man, int *state) {
    MoveRelearner *relearner = OverlayManager_GetData(man);

    switch (*state) {
    case MOVE_RELEARNER_STATE_WAIT_FADE:
        *state = MoveRelearner_WaitFade(relearner);
        break;
    case MOVE_RELEARNER_STATE_INPUT:
        *state = MoveRelearner_HandleInput(relearner);
        break;
    case MOVE_RELEARNER_STATE_WAIT_MESSAGE:
        *state = MoveRelearner_WaitMessage(relearner);
        break;
    case MOVE_RELEARNER_STATE_SHOW_YES_NO:
        *state = MoveRelearner_ShowYesNo(relearner);
        break;
    case MOVE_RELEARNER_STATE_HANDLE_YES_NO:
        *state = MoveRelearner_HandleYesNo(relearner);
        break;
    case MOVE_RELEARNER_STATE_TEACH_MOVE:
        *state = MoveRelearner_TeachMove(relearner);
        break;
    case MOVE_RELEARNER_STATE_PRINT_LEARNED:
        *state = MoveRelearner_PrintLearned(relearner);
        break;
    case MOVE_RELEARNER_STATE_ASK_AFTER_SUMMARY:
        *state = MoveRelearner_AskAfterSummary(relearner);
        break;
    case MOVE_RELEARNER_STATE_FADE_OUT:
        *state = MoveRelearner_FadeOut(relearner);
        break;
    case MOVE_RELEARNER_STATE_EXIT:
        return TRUE;
    case MOVE_RELEARNER_STATE_FADE_OUT_TO_SUMMARY:
        *state = MoveRelearner_FadeOutToSummary(relearner);
        break;
    case MOVE_RELEARNER_STATE_OPEN_SUMMARY:
        *state = MoveRelearner_OpenSummary(relearner);
        return FALSE;
    case MOVE_RELEARNER_STATE_RUN_SUMMARY:
        *state = MoveRelearner_RunSummary(relearner);
        return FALSE;
    case MOVE_RELEARNER_STATE_BUTTON_FLASH:
        *state = MoveRelearner_UpdateButtonFlash(relearner);
        break;
    case MOVE_RELEARNER_STATE_ASK_GIVE_UP:
        *state = MoveRelearner_AskGiveUp(relearner);
        break;
    case MOVE_RELEARNER_STATE_ASK_TEACH:
        *state = MoveRelearner_AskTeachAfterFlash(relearner);
        break;
    }
    MoveRelearner_Draw3D(&relearner->monPic);
    SpriteSystem_DrawSprites(relearner->spriteManager);
    return FALSE;
}

BOOL MoveRelearner_Exit(OverlayManager *man, int *state) {
    MoveRelearner *relearner = OverlayManager_GetData(man);

    MoveRelearner_Teardown(relearner);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_66);
    return TRUE;
}

// Also used to rebuild the screens after the summary screen returns.
void MoveRelearner_Setup(MoveRelearner *relearner) {
    NARC *narc;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(GX_PLANEMASK_NONE);
    GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    relearner->menuInputState = MenuInputStateMgr_GetState(relearner->args->menuInputStatePtr);
    relearner->bgConfig = BgConfig_Alloc(HEAP_ID_66);
    narc = NARC_New(NARC_a_1_1_0, HEAP_ID_66);
    MoveRelearner_SetGfxBanks();
    MoveRelearner_InitBgs(relearner->bgConfig);
    MoveRelearner_LoadBgGraphics(relearner, narc);
    MoveRelearner_InitSprites(relearner, narc);
    MoveRelearner_Init3D(relearner);
    MoveRelearner_InitStrings(relearner);
    MoveRelearner_AddWindows(relearner);
    MoveRelearner_InitMoveList(relearner);
    MoveRelearner_PrintSelectedMove(relearner);
    MoveRelearner_PrintStaticTexts(relearner);
    MoveRelearner_CreateGridInput(relearner);
    sub_020880CC(0, HEAP_ID_66);
    Main_SetVBlankIntrCB((GFIntrCB)MoveRelearner_VBlankCB, relearner);
    NARC_Delete(narc);
}

void MoveRelearner_Teardown(MoveRelearner *relearner) {
    MoveRelearner_FreeGridInput(relearner);
    MoveRelearner_FreeMoveList(relearner);
    MoveRelearner_RemoveWindows(relearner);
    MoveRelearner_FreeScreenFile(relearner);
    MoveRelearner_FreeBgs(relearner->bgConfig);
    MoveRelearner_FreeStrings(relearner);
    MoveRelearner_Free3D(&relearner->monPic);
    MoveRelearner_FreeSprites(relearner);
    MenuInputStateMgr_SetState(relearner->args->menuInputStatePtr, relearner->menuInputState);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    GF_DestroyVramTransferManager();
    Main_SetVBlankIntrCB(NULL, NULL);
}

void MoveRelearner_VBlankCB(MoveRelearner *relearner) {
    DoScheduledBgGpuUpdates(relearner->bgConfig);
    PokepicManager_HandleLoadImgAndOrPltt(relearner->monPic.pokepicManager);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

#pragma force_active on

void MoveRelearner_SetGfxBanks(void) {
    GraphicsBanks banks = sMoveRelearnerGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void MoveRelearner_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes = sMoveRelearnerGraphicsModes;
    BgTemplate main2;
    BgTemplate main3;
    BgTemplate sub0;
    BgTemplate sub2;
    BgTemplate sub3;

    SetBothScreensModesAndDisable(&modes);

    main2 = sMoveRelearnerBgTemplate_Main2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    main3 = sMoveRelearnerBgTemplate_Main3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    sub0 = sMoveRelearnerBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    sub2 = sMoveRelearnerBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);

    sub3 = sMoveRelearnerBgTemplate_Sub3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sub3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, HEAP_ID_66);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, HEAP_ID_66);
}

#pragma force_active reset

void MoveRelearner_FreeBgs(BgConfig *bgConfig) {
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    Heap_FreeExplicit(HEAP_ID_66, bgConfig);
    reg_GX_POWCNT |= 0x8000;
}

#pragma force_active on

void MoveRelearner_LoadBgGraphics(MoveRelearner *relearner, NARC *narc) {
    BgConfig_LoadAssetFromOpenNarc(relearner->bgConfig, HEAP_ID_66, narc, NARC_a_1_1_0, 1, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_CHAR, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(relearner->bgConfig, HEAP_ID_66, narc, NARC_a_1_1_0, 4, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_SCRN, 0x800, 0);
    BgConfig_LoadAssetFromOpenNarc(relearner->bgConfig, HEAP_ID_66, narc, NARC_a_1_1_0, 0, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_PLTT, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(relearner->bgConfig, HEAP_ID_66, narc, NARC_a_1_1_0, 1, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_CHAR, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(relearner->bgConfig, HEAP_ID_66, narc, NARC_a_1_1_0, 2, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_SCRN, 0x800, 0);
    BgConfig_LoadAssetFromOpenNarc(relearner->bgConfig, HEAP_ID_66, narc, NARC_a_1_1_0, 0, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_PLTT, 0, 0);
    relearner->screenFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 3, FALSE, &relearner->screenData, HEAP_ID_66);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_66);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_66);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_66);
    LoadUserFrameGfx2(relearner->bgConfig, GF_BG_LYR_SUB_0, 61, 13, Options_GetFrame(relearner->args->options), HEAP_ID_66);
}

#pragma force_active reset

void MoveRelearner_FreeScreenFile(MoveRelearner *relearner) {
    Heap_Free(relearner->screenFile);
}

#pragma force_active on

void MoveRelearner_AddWindows(MoveRelearner *relearner) {
    u32 i;

    FontID_Alloc(4, HEAP_ID_66);
    for (i = 0; i < MOVE_RELEARNER_NUM_WINDOWS; i++) {
        AddWindow(relearner->bgConfig, &relearner->windows[i], &sMoveRelearnerWindowTemplates[i]);
        FillWindowPixelBuffer(&relearner->windows[i], 0);
    }
    ClearWindowTilemapAndScheduleTransfer(&relearner->windows[8]);
    relearner->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_66);
}

#pragma force_active reset

void MoveRelearner_RemoveWindows(MoveRelearner *relearner) {
    u32 i;
    Window *window;

    YesNoPrompt_Destroy(relearner->yesNoPrompt);
    for (i = 0, window = relearner->windows; i < MOVE_RELEARNER_NUM_WINDOWS; i++, window++) {
        RemoveWindow(window);
    }
    FontID_Release(4);
}

void MoveRelearner_InitStrings(MoveRelearner *relearner) {
    relearner->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0747_bin, HEAP_ID_66);
    relearner->msgFormat = MessageFormat_New(HEAP_ID_66);
    relearner->string = String_New(256, HEAP_ID_66);
    relearner->ppLabel = NewString_ReadMsgData(relearner->msgData, 25);  // "PP"
    relearner->ppFormat = NewString_ReadMsgData(relearner->msgData, 31); // "{0}/{1}"
}

void MoveRelearner_FreeStrings(MoveRelearner *relearner) {
    DestroyMsgData(relearner->msgData);
    MessageFormat_Delete(relearner->msgFormat);
    String_Delete(relearner->string);
    String_Delete(relearner->ppLabel);
    String_Delete(relearner->ppFormat);
}

int MoveRelearner_WaitFade(MoveRelearner *relearner) {
    if (IsPaletteFadeFinished() == TRUE) {
        return relearner->nextState;
    }
    return MOVE_RELEARNER_STATE_WAIT_FADE;
}

#pragma force_active on

int MoveRelearner_HandleInput(MoveRelearner *relearner) {
    u32 input;

    relearner->lastGridInput = GridInputHandler_GetNextInput(relearner->gridInput);
    input = GridInputHandler_HandleInput_AllowHold(relearner->gridInput);
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
        // a row of the move list
        if (!System_GetTouchNew()) {
            if (relearner->args->scrollOffset + input < relearner->numMoves) {
                PlaySE(SEQ_SE_DP_DECIDE);
                MoveRelearner_DrawListRows(relearner, input);
                return MoveRelearner_AskTeach(relearner);
            }
        } else if (relearner->args->scrollOffset + input < relearner->numMoves) {
            PlaySE(SEQ_SE_DP_DECIDE);
            MoveRelearner_DrawListRows(relearner, input);
        } else {
            MoveRelearner_DrawListRows(relearner, 5);
        }
        break;
    case 6:
    case (u32)GRID_MENU_CANCEL:
        // "RETURN"
        PlaySE(SEQ_SE_DP_DECIDE);
        MoveRelearner_DrawListRows(relearner, 5);
        MoveRelearner_SetLearnButtonActive(relearner, FALSE);
        return MoveRelearner_StartReturnButtonFlash(relearner, MOVE_RELEARNER_STATE_ASK_GIVE_UP);
    case 7:
        // "TEACH"
        PlaySE(SEQ_SE_DP_DECIDE);
        return MoveRelearner_StartLearnButtonFlash(relearner, MOVE_RELEARNER_STATE_ASK_TEACH);
    case 4:
    case 5:
    case (u32)GRID_MENU_NOTHING_CHOSEN:
    case (u32)GRID_MENU_CURSOR_MOVE:
    case (u32)GRID_MENU_BUTTON_MODE:
        break;
    }
    return MOVE_RELEARNER_STATE_INPUT;
}

#pragma force_active reset

int MoveRelearner_WaitMessage(MoveRelearner *relearner) {
    if (TextPrinterCheckActive(relearner->textPrinterId) == 0) {
        return relearner->nextState;
    } else {
        return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
    }
}

int MoveRelearner_ShowYesNo(MoveRelearner *relearner) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = relearner->bgConfig;
    template.bgId = GF_BG_LYR_SUB_0;
    template.tileStart = 1;
    template.plttSlot = 11;
    template.x = 26;
    template.y = 10;
    template.ignoreTouchFlag = relearner->menuInputState;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(relearner->yesNoPrompt, &template);
    MoveRelearner_DimSubScreen(1);
    return MOVE_RELEARNER_STATE_HANDLE_YES_NO;
}

int MoveRelearner_HandleYesNo(MoveRelearner *relearner) {
    int nextState;

    switch (YesNoPrompt_HandleInput(relearner->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        nextState = sMoveRelearnerYesActions[relearner->question].callback(relearner);
        MoveRelearner_DimSubScreen(0);
        break;
    case YESNORESPONSE_NO:
        nextState = sMoveRelearnerNoActions[relearner->question].callback(relearner);
        MoveRelearner_DimSubScreen(0);
        break;
    default:
        return MOVE_RELEARNER_STATE_HANDLE_YES_NO;
    }
    relearner->menuInputState = YesNoPrompt_IsInTouchMode(relearner->yesNoPrompt);
    YesNoPrompt_Reset(relearner->yesNoPrompt);
    return nextState;
}

int MoveRelearner_TeachMove(MoveRelearner *relearner) {
    u32 value = MoveRelearner_GetSelectedMove(relearner);
    MoveRelearnerArgs *args = relearner->args;

    SetMonData(args->mon, MON_DATA_MOVE1 + args->moveSlot, &value);
    value = 0;
    args = relearner->args;
    SetMonData(args->mon, MON_DATA_MOVE1_PP_UPS + args->moveSlot, &value);
    value = GetMoveMaxPP(MoveRelearner_GetSelectedMove(relearner), 0);
    args = relearner->args;
    SetMonData(args->mon, MON_DATA_MOVE1_PP + args->moveSlot, &value);
    relearner->args->cancelled = FALSE;
    return MOVE_RELEARNER_STATE_FADE_OUT;
}

int MoveRelearner_PrintLearned(MoveRelearner *relearner) {
    MoveRelearner_PrintMessage(relearner, 6);
    relearner->nextState = MOVE_RELEARNER_STATE_TEACH_MOVE;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}

#pragma force_active on

int MoveRelearner_AskAfterSummary(MoveRelearner *relearner) {
    if (relearner->args->moveSlot < MAX_MON_MOVES) {
        MoveRelearner_PrintMessage(relearner, 10);
        relearner->question = MOVE_RELEARNER_QUESTION_FORGET_MOVE;
    } else {
        MoveRelearner_PrintMessage(relearner, 7);
        relearner->question = MOVE_RELEARNER_QUESTION_STOP_TEACHING;
    }
    relearner->nextState = MOVE_RELEARNER_STATE_SHOW_YES_NO;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}

#pragma force_active reset

int MoveRelearner_FadeOut(MoveRelearner *relearner) {
    sub_020880CC(1, HEAP_ID_66);
    relearner->nextState = MOVE_RELEARNER_STATE_EXIT;
    return MOVE_RELEARNER_STATE_WAIT_FADE;
}

void MoveRelearner_PrintSelectedMove(MoveRelearner *relearner) {
    u16 move = MoveRelearner_GetSelectedMove(relearner);

    if (move != 0xFFFF) {
        MoveRelearner_PrintMoveInfo(relearner, move);
    } else {
        MoveRelearner_PrintMoveInfo(relearner, -2);
    }
    MoveRelearner_UpdateListTypeIcons(relearner);
}

// Prints relearner->string. alignment: 0 left, 1 right, 2 centered.
void MoveRelearner_PrintString(MoveRelearner *relearner, u32 windowId, u32 fontId, u32 color, u32 alignment, u8 y) {
    u8 textWidth;
    u8 x;

    switch (alignment) {
    case 0:
        x = 0;
        break;
    case 1:
        textWidth = FontID_String_GetWidth(fontId, relearner->string, 0);
        x = (u8)(GetWindowWidth(&relearner->windows[windowId]) * 8) - textWidth;
        break;
    case 2:
        textWidth = FontID_String_GetWidth(fontId, relearner->string, 0);
        x = ((s32)(u8)(GetWindowWidth(&relearner->windows[windowId]) * 8) - textWidth) / 2;
        break;
    default:
        break;
    }

    // The font id is passed without truncation to u8: the callee takes it as u32 (see the FontID TODO).
    ((u8(*)(Window *, u32, String *, u32, u32, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(&relearner->windows[windowId], fontId, relearner->string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void MoveRelearner_FormatNumber(MoveRelearner *relearner, u32 msgId, s32 value, u32 numDigits, u8 mode) {
    String *format = NewString_ReadMsgData(relearner->msgData, msgId);

    BufferIntegerAsString(relearner->msgFormat, 0, value, numDigits, (PrintingMode)mode, TRUE);
    StringExpandPlaceholders(relearner->msgFormat, relearner->string, format);
    String_Delete(format);
}

#pragma force_active on

void MoveRelearner_PrintStaticTexts(MoveRelearner *relearner) {
    String *format;
    MsgData *moveNames;
    BoxPokemon *boxMon;
    u32 i;

    ReadMsgDataIntoString(relearner->msgData, 36, relearner->string); // "TEACH WHICH MOVE?"
    MoveRelearner_PrintString(relearner, 7, 0, MAKE_TEXT_COLOR(1, 2, 0), 0, 4);
    ScheduleWindowCopyToVram(&relearner->windows[7]);
    ReadMsgDataIntoString(relearner->msgData, 35, relearner->string); // "TEACH"
    MoveRelearner_PrintString(relearner, 8, 4, MAKE_TEXT_COLOR(15, 14, 0), 2, 4);
    ReadMsgDataIntoString(relearner->msgData, 34, relearner->string); // "RETURN"
    MoveRelearner_PrintString(relearner, 9, 4, MAKE_TEXT_COLOR(15, 14, 0), 2, 4);
    ScheduleWindowCopyToVram(&relearner->windows[9]);
    ReadMsgDataIntoString(relearner->msgData, 22, relearner->string); // "CATEGORY"
    MoveRelearner_PrintString(relearner, 0, 0, MAKE_TEXT_COLOR(15, 14, 0), 0, 0);
    ScheduleWindowCopyToVram(&relearner->windows[0]);
    ReadMsgDataIntoString(relearner->msgData, 23, relearner->string); // "POWER"
    MoveRelearner_PrintString(relearner, 1, 0, MAKE_TEXT_COLOR(15, 14, 0), 0, 0);
    ScheduleWindowCopyToVram(&relearner->windows[1]);
    ReadMsgDataIntoString(relearner->msgData, 24, relearner->string); // "ACCURACY"
    MoveRelearner_PrintString(relearner, 2, 0, MAKE_TEXT_COLOR(15, 14, 0), 0, 0);
    ScheduleWindowCopyToVram(&relearner->windows[2]);

    format = String_New(256, HEAP_ID_66);
    ReadMsgDataIntoString(relearner->msgData, 39, format); // "LEARNED MOVES"
    BufferBoxMonNickname(relearner->msgFormat, 0, Mon_GetBoxMon(relearner->args->mon));
    StringExpandPlaceholders(relearner->msgFormat, relearner->string, format);
    MoveRelearner_PrintString(relearner, 12, 0, MAKE_TEXT_COLOR(1, 2, 0), 0, 4);
    ScheduleWindowCopyToVram(&relearner->windows[12]);

    // the Pokemon's current moves
    moveNames = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0750_bin, HEAP_ID_66);
    boxMon = Mon_GetBoxMon(relearner->args->mon);
    for (i = 0; i < MAX_MON_MOVES; i++) {
        u16 move = GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, NULL);
        if (move == MOVE_NONE) {
            ManagedSprite_SetDrawFlag(relearner->sprites[8 + i], FALSE);
        } else {
            ManagedSprite_SetDrawFlag(relearner->sprites[8 + i], TRUE);
            MoveRelearner_SetTypeIcon(relearner, move, (u16)(i + 4));
            ReadMsgDataIntoString(moveNames, move, relearner->string);
            MoveRelearner_PrintString(relearner, 13, 0, MAKE_TEXT_COLOR(15, 14, 0), 0, (u8)(i * 32));
            AddTextPrinterParameterizedWithColor(&relearner->windows[13], 0, relearner->ppLabel, 16, 16 + i * 32, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
            BufferIntegerAsString(relearner->msgFormat, 0, GetBoxMonData(boxMon, MON_DATA_MOVE1_PP + i, NULL), 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            BufferIntegerAsString(relearner->msgFormat, 1, GetBoxMonData(boxMon, MON_DATA_MOVE1_MAX_PP + i, NULL), 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
            StringExpandPlaceholders(relearner->msgFormat, relearner->string, relearner->ppFormat);
            AddTextPrinterParameterizedWithColor(&relearner->windows[13], 0, relearner->string, 45, 16 + i * 32, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        }
    }
    ScheduleWindowCopyToVram(&relearner->windows[13]);

    ReadMsgDataIntoString(relearner->msgData, 37, format); // species name
    BufferBoxMonSpeciesName(relearner->msgFormat, 0, boxMon);
    StringExpandPlaceholders(relearner->msgFormat, relearner->string, format);
    AddTextPrinterParameterizedWithColor(&relearner->windows[14], 0, relearner->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ReadMsgDataIntoString(relearner->msgData, 38, format); // "Lv. {0}"
    BufferIntegerAsString(relearner->msgFormat, 0, GetBoxMonData(boxMon, MON_DATA_LEVEL, NULL), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(relearner->msgFormat, relearner->string, format);
    MoveRelearner_PrintString(relearner, 14, 0, MAKE_TEXT_COLOR(1, 2, 0), 1, 16);
    ScheduleWindowCopyToVram(&relearner->windows[14]);
    DestroyMsgData(moveNames);
    String_Delete(format);
}

#pragma force_active reset

u32 MoveRelearner_CountEligibleMoves(MoveRelearner *relearner) {
    u16 *moves = relearner->args->eligibleMoves;
    u32 i;

    for (i = 0; i < 256; i++) {
        if (moves[i] == 0xFFFF) {
            break;
        }
    }
    return i;
}

// Copies a rectangle of the button screen (screenData) to the bottom screen's bg 3.
void MoveRelearner_CopyButtonTiles(MoveRelearner *relearner, u32 x, u32 y, u32 width, u8 height, u8 srcX, u8 srcY) {
    NNSG2dScreenData *screenData = relearner->screenData;

    // The coordinates are passed without truncation to u8: the original callee prototype took them as u32.
    ((void (*)(BgConfig *, u8, u32, u32, u32, u32, const void *, u32, u32, u32, u32))CopyToBgTilemapRect)(relearner->bgConfig, GF_BG_LYR_SUB_3, x, y, width, height, screenData->rawData, srcX, srcY, (u8)(screenData->screenWidth / 8), (u8)(screenData->screenHeight / 8));
    ScheduleBgTilemapBufferTransfer(relearner->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active on

void MoveRelearner_PrintListRow(MoveRelearner *relearner, int scrollOffset, int row) {
    int index = scrollOffset + row;
    int y = row * 32;

    // The printer calls use the original prototype with int coordinates: then y + 16 is computed twice, as in the original.
    if (index >= relearner->numMoves) {
        MoveRelearner_CopyButtonTiles(relearner, 0, (u8)(row * 4 + 4), 16, 4, 0, 4);
        return;
    }
    ((u8(*)(Window *, u32, String *, int, int, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(&relearner->windows[10], 0, relearner->moveList[index].text, 0, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    ((u8(*)(Window *, u32, String *, int, int, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(&relearner->windows[10], 0, relearner->ppLabel, 16, y + 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    int maxPP = GetMoveMaxPP((u16)relearner->moveList[index].value, 0);
    BufferIntegerAsString(relearner->msgFormat, 0, maxPP, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    BufferIntegerAsString(relearner->msgFormat, 1, maxPP, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(relearner->msgFormat, relearner->string, relearner->ppFormat);
    ((u8(*)(Window *, u32, String *, int, int, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(&relearner->windows[10], 0, relearner->string, 45, y + 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

#pragma force_active reset

void MoveRelearner_PrintMoveList(MoveRelearner *relearner) {
    u32 i;

    FillWindowPixelBuffer(&relearner->windows[10], 0);
    for (i = 0; i < MOVE_RELEARNER_VISIBLE_ROWS; i++) {
        MoveRelearner_PrintListRow(relearner, (u8)relearner->args->scrollOffset, (u8)i);
    }
    ScheduleWindowCopyToVram(&relearner->windows[10]);
    ScheduleBgTilemapBufferTransfer(relearner->bgConfig, GF_BG_LYR_SUB_3);
    MoveRelearner_UpdateListTypeIcons(relearner);
}

void MoveRelearner_InitMoveList(MoveRelearner *relearner) {
    u32 i;
    MsgData *moveNames;

    relearner->numMoves = MoveRelearner_CountEligibleMoves(relearner);
    relearner->moveList = ListMenuItems_New(relearner->numMoves, HEAP_ID_66);
    moveNames = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0750_bin, HEAP_ID_66);

    for (i = 0; i < relearner->numMoves; i++) {
        u16 move = relearner->args->eligibleMoves[i];
        if (move != 0xFFFF) {
            ListMenuItems_AppendFromMsgData(relearner->moveList, moveNames, move, move);
        } else {
            ListMenuItems_AppendFromMsgData(relearner->moveList, relearner->msgData, 32, -2);
            break;
        }
    }
    DestroyMsgData(moveNames);
    relearner->unk_1BB = 0;
    relearner->unk_1BC = relearner->args->cursorPos;
    MoveRelearner_PrintMoveList(relearner);
}

void MoveRelearner_FreeMoveList(MoveRelearner *relearner) {
    ListMenuItems_Delete(relearner->moveList);
}

// move -2: none (clears the info).
void MoveRelearner_PrintMoveInfo(MoveRelearner *relearner, s32 move) {
    u32 value;
    MsgData *moveDescriptions;

    FillWindowPixelBuffer(&relearner->windows[5], 0);
    FillWindowPixelBuffer(&relearner->windows[3], 0);
    FillWindowPixelBuffer(&relearner->windows[4], 0);
    ManagedSprite_SetDrawFlag(relearner->sprites[3], FALSE);

    if (move != -2) {
        // move is passed without truncation to u16: the original callee prototype took it as u32
        value = ((u32(*)(u32, MoveAttr))GetMoveAttr)(move, MOVEATTR_POWER);
        if (value <= 1) {
            ReadMsgDataIntoString(relearner->msgData, 33, relearner->string); // "---"
        } else {
            MoveRelearner_FormatNumber(relearner, 29, value, 3, PRINTING_MODE_LEFT_ALIGN);
        }
        MoveRelearner_PrintString(relearner, 3, 0, MAKE_TEXT_COLOR(1, 2, 0), 1, 0);

        value = ((u32(*)(u32, MoveAttr))GetMoveAttr)(move, MOVEATTR_ACCURACY);
        if (value == 0) {
            ReadMsgDataIntoString(relearner->msgData, 33, relearner->string); // "---"
        } else {
            MoveRelearner_FormatNumber(relearner, 30, value, 3, PRINTING_MODE_LEFT_ALIGN);
        }
        MoveRelearner_PrintString(relearner, 4, 0, MAKE_TEXT_COLOR(1, 2, 0), 1, 0);

        moveDescriptions = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0749_bin, HEAP_ID_66);
        ReadMsgDataIntoString(moveDescriptions, move, relearner->string);
        MoveRelearner_PrintString(relearner, 5, 0, MAKE_TEXT_COLOR(1, 2, 0), 0, 0);
        DestroyMsgData(moveDescriptions);
        MoveRelearner_SetCategoryIcon(relearner, (u16)move);
        ManagedSprite_SetDrawFlag(relearner->sprites[3], TRUE);
        ScheduleWindowCopyToVram(&relearner->windows[0]);
        ScheduleWindowCopyToVram(&relearner->windows[1]);
        ScheduleWindowCopyToVram(&relearner->windows[2]);
    } else {
        ClearWindowTilemapAndScheduleTransfer(&relearner->windows[0]);
        ClearWindowTilemapAndScheduleTransfer(&relearner->windows[1]);
        ClearWindowTilemapAndScheduleTransfer(&relearner->windows[2]);
    }
    ScheduleWindowCopyToVram(&relearner->windows[5]);
    ScheduleWindowCopyToVram(&relearner->windows[3]);
    ScheduleWindowCopyToVram(&relearner->windows[4]);
}

// Fills the placeholders of message `message` (a column of sMoveRelearnerMessageIds) into relearner->string.
void MoveRelearner_FormatMessage(MoveRelearner *relearner, u32 message) {
    String *format;
    BoxPokemon *boxMon;

    switch (message) {
    case 0:
        boxMon = Mon_GetBoxMon(relearner->args->mon);
        BufferBoxMonNickname(relearner->msgFormat, 0, boxMon);
        break;
    case 1:
        BufferMoveName(relearner->msgFormat, 1, MoveRelearner_GetSelectedMove(relearner));
        break;
    case 2:
        boxMon = Mon_GetBoxMon(relearner->args->mon);
        BufferBoxMonNickname(relearner->msgFormat, 0, boxMon);
        break;
    case 3:
        boxMon = Mon_GetBoxMon(relearner->args->mon);
        BufferBoxMonNickname(relearner->msgFormat, 0, boxMon);
        BufferMoveName(relearner->msgFormat, 1, MoveRelearner_GetSelectedMove(relearner));
        break;
    case 4:
        boxMon = Mon_GetBoxMon(relearner->args->mon);
        BufferBoxMonNickname(relearner->msgFormat, 0, boxMon);
        BufferMoveName(relearner->msgFormat, 1, MoveRelearner_GetSelectedMove(relearner));
        break;
    case 5:
        boxMon = Mon_GetBoxMon(relearner->args->mon);
        BufferBoxMonNickname(relearner->msgFormat, 0, boxMon);
        BufferMoveName(relearner->msgFormat, 1, MoveRelearner_GetMoveToReplace(relearner));
        break;
    case 6:
        boxMon = Mon_GetBoxMon(relearner->args->mon);
        BufferBoxMonNickname(relearner->msgFormat, 0, boxMon);
        BufferMoveName(relearner->msgFormat, 1, MoveRelearner_GetSelectedMove(relearner));
        break;
    case 7:
        BufferMoveName(relearner->msgFormat, 1, MoveRelearner_GetSelectedMove(relearner));
        break;
    case 8:
        boxMon = Mon_GetBoxMon(relearner->args->mon);
        BufferBoxMonNickname(relearner->msgFormat, 0, boxMon);
        BufferMoveName(relearner->msgFormat, 1, MoveRelearner_GetSelectedMove(relearner));
        break;
    case 9:
        BufferPlayersName(relearner->msgFormat, 2, relearner->args->profile);
        break;
    case 10:
        BufferMoveName(relearner->msgFormat, 0, MoveRelearner_GetMoveToReplace(relearner));
        break;
    }

    format = NewString_ReadMsgData(relearner->msgData, sMoveRelearnerMessageIds[relearner->args->type][message]);
    StringExpandPlaceholders(relearner->msgFormat, relearner->string, format);
    String_Delete(format);
}

u16 MoveRelearner_GetSelectedMove(MoveRelearner *relearner) {
    MoveRelearnerArgs *args = relearner->args;
    return args->eligibleMoves[args->scrollOffset + args->cursorPos];
}

u16 MoveRelearner_GetMoveToReplace(MoveRelearner *relearner) {
    MoveRelearnerArgs *args = relearner->args;
    return GetMonData(args->mon, args->moveSlot + MON_DATA_MOVE1, NULL);
}

#pragma force_active on

void MoveRelearner_PrintMessage(MoveRelearner *relearner, u32 message) {
    FillWindowPixelBuffer(&relearner->windows[6], 15);
    DrawFrameAndWindow2(&relearner->windows[6], FALSE, 61, 13);
    MoveRelearner_FormatMessage(relearner, message);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    relearner->textPrinterId = AddTextPrinterParameterized(&relearner->windows[6], 1, relearner->string, 0, 0, Options_GetTextFrameDelay(relearner->args->options), MoveRelearner_TextPrinterCallback);
}

#pragma force_active reset

void MoveRelearner_ClearMessage(MoveRelearner *relearner) {
    ClearFrameAndWindow2(&relearner->windows[6], TRUE);
    ClearWindowTilemapAndScheduleTransfer(&relearner->windows[6]);
}

BOOL MoveRelearner_TextPrinterCallback(TextPrinterTemplate *template, u16 glyphId) {
    switch (glyphId) {
    case 1:
        return GF_IsAnySEPlaying();
    case 2:
        return IsFanfarePlaying();
    case 3:
        PlaySE(SEQ_SE_DP_KON);
        break;
    case 4:
        PlayFanfare(SEQ_ME_LVUP);
        break;
    case 5:
        return IsSEPlaying(SEQ_SE_DP_KON);
    }
    return FALSE;
}

// The first empty move slot (MAX_MON_MOVES if the Pokemon knows four moves).
u32 MoveRelearner_CountKnownMoves(MoveRelearner *relearner) {
    u8 i;

    for (i = 0; i < MAX_MON_MOVES; i++) {
        if (!GetMonData(relearner->args->mon, i + MON_DATA_MOVE1, NULL)) {
            break;
        }
    }
    return i;
}

int MoveRelearner_OnTeachYes(MoveRelearner *relearner) {
    MoveRelearner_PrintMessage(relearner, 3);
    relearner->args->moveSlot = MoveRelearner_CountKnownMoves(relearner);
    relearner->nextState = MOVE_RELEARNER_STATE_TEACH_MOVE;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}

int MoveRelearner_OnTeachNo(MoveRelearner *relearner) {
    MoveRelearner_ClearMessage(relearner);
    if (relearner->learnButtonActive == FALSE) {
        MoveRelearner_DrawListRows(relearner, 5);
    }
    return MOVE_RELEARNER_STATE_INPUT;
}

int MoveRelearner_OnGiveUpYes(MoveRelearner *relearner) {
    relearner->args->cancelled = TRUE;
    return MOVE_RELEARNER_STATE_FADE_OUT;
}

int MoveRelearner_OnGiveUpNo(MoveRelearner *relearner) {
    MoveRelearner_ClearMessage(relearner);
    return MOVE_RELEARNER_STATE_INPUT;
}

int MoveRelearner_OnDeleteMoveYes(MoveRelearner *relearner) {
    relearner->nextState = MOVE_RELEARNER_STATE_FADE_OUT_TO_SUMMARY;
    return MOVE_RELEARNER_STATE_WAIT_FADE;
}

int MoveRelearner_OnDeleteMoveNo(MoveRelearner *relearner) {
    MoveRelearner_PrintMessage(relearner, 7);
    relearner->question = MOVE_RELEARNER_QUESTION_STOP_TEACHING;
    relearner->nextState = MOVE_RELEARNER_STATE_SHOW_YES_NO;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}

int MoveRelearner_OnStopTeachingYes(MoveRelearner *relearner) {
    MoveRelearner_PrintMessage(relearner, 8);
    relearner->nextState = MOVE_RELEARNER_STATE_FADE_OUT;
    relearner->args->cancelled = TRUE;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}

int MoveRelearner_OnStopTeachingNo(MoveRelearner *relearner) {
    MoveRelearner_PrintMessage(relearner, 4);
    relearner->question = MOVE_RELEARNER_QUESTION_DELETE_MOVE;
    relearner->nextState = MOVE_RELEARNER_STATE_SHOW_YES_NO;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}

int MoveRelearner_OnForgetMoveYes(MoveRelearner *relearner) {
    MoveRelearner_PrintMessage(relearner, 5);
    relearner->nextState = MOVE_RELEARNER_STATE_PRINT_LEARNED;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}

int MoveRelearner_OnForgetMoveNo(MoveRelearner *relearner) {
    relearner->nextState = MOVE_RELEARNER_STATE_FADE_OUT_TO_SUMMARY;
    return MOVE_RELEARNER_STATE_WAIT_FADE;
}

#pragma force_active on

void MoveRelearner_InitSpriteSystem(MoveRelearner *relearner) {
    SpriteResourceCountsListUnion resourceCounts = sMoveRelearnerSpriteResourceCounts;
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;

    GF_CreateVramTransferManager(64, HEAP_ID_66);
    relearner->spriteSystem = SpriteSystem_Alloc(HEAP_ID_66);
    relearner->spriteManager = SpriteManager_New(relearner->spriteSystem);
    oamManagerParam = sMoveRelearnerOamManagerParam;
    oamCharTransferParam = sMoveRelearnerOamCharTransferParam;
    SpriteSystem_Init(relearner->spriteSystem, &oamManagerParam, &oamCharTransferParam, 32);
    SpriteSystem_InitSprites(relearner->spriteSystem, relearner->spriteManager, MOVE_RELEARNER_NUM_SPRITES);
    SpriteSystem_InitManagerWithCapacities(relearner->spriteSystem, relearner->spriteManager, &resourceCounts);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

#pragma force_active reset

void MoveRelearner_FreeSprites(MoveRelearner *relearner) {
    u32 i;

    for (i = 0; i < MOVE_RELEARNER_NUM_SPRITES; i++) {
        Sprite_DeleteAndFreeResources(relearner->sprites[i]);
    }
    SpriteSystem_FreeResourcesAndManager(relearner->spriteSystem, relearner->spriteManager);
    SpriteSystem_Free(relearner->spriteSystem);
}

void MoveRelearner_LoadSpriteResources(MoveRelearner *relearner, NARC *narc) {
    u32 i;

    SpriteSystem_LoadCharResObjFromOpenNarc(relearner->spriteSystem, relearner->spriteManager, narc, 6, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB8A8);
    SpriteSystem_LoadCharResObjFromOpenNarc(relearner->spriteSystem, relearner->spriteManager, narc, 11, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB8A9);
    for (i = 0xB8AB; i <= 0xB8B2; i++) {
        if (i < 0xB8AF) {
            TypeIcon_LoadChar(relearner->spriteSystem, relearner->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, 0, i);
        } else {
            TypeIcon_LoadChar(relearner->spriteSystem, relearner->spriteManager, NNS_G2D_VRAM_TYPE_2DMAIN, 0, i);
        }
    }
    MoveCategoryIcon_LoadChar(relearner->spriteSystem, relearner->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, 0, 0xB8AA);
    SpriteSystem_LoadPlttResObjFromOpenNarc(relearner->spriteSystem, relearner->spriteManager, narc, 5, FALSE, 4, NNS_G2D_VRAM_TYPE_2DSUB, 0xB8A8);
    TypeIcon_LoadPltt(relearner->spriteSystem, relearner->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, 0xB8A9);
    TypeIcon_LoadPltt(relearner->spriteSystem, relearner->spriteManager, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB8AA);
    SpriteSystem_LoadCellResObjFromOpenNarc(relearner->spriteSystem, relearner->spriteManager, narc, 7, FALSE, 0xB8A8);
    SpriteSystem_LoadCellResObjFromOpenNarc(relearner->spriteSystem, relearner->spriteManager, narc, 10, FALSE, 0xB8A9);
    SpriteSystem_LoadAnimResObjFromOpenNarc(relearner->spriteSystem, relearner->spriteManager, narc, 8, FALSE, 0xB8A8);
    SpriteSystem_LoadAnimResObjFromOpenNarc(relearner->spriteSystem, relearner->spriteManager, narc, 9, FALSE, 0xB8A9);
    TypeIcon_LoadCellAnim(relearner->spriteSystem, relearner->spriteManager, 0xB8AA, 0xB8AA);
}

// index 0-3: the type icon of a list row, 4-7: of one of the Pokemon's current moves.
void MoveRelearner_SetTypeIcon(MoveRelearner *relearner, u16 move, u16 index) {
    int type;
    NarcId narc;
    int file;

    if (relearner->args->showContestType == FALSE) {
        type = GetMoveAttr(move, MOVEATTR_TYPE);
    } else {
        type = GetMoveAttr(move, MOVEATTR_CONTEST_TYPE) + 18;
    }
    narc = TypeIcon_GetNarc();
    file = TypeIcon_GetCharFile(type);
    SpriteSystem_ReplaceCharResObj(relearner->spriteSystem, relearner->spriteManager, narc, file, TRUE, 0xB8AB + index);
    if (index < 4) {
        ManagedSprite_SetPaletteOverride(relearner->sprites[index + 4], TypeIcon_GetPalette(type) + 4);
    } else {
        ManagedSprite_SetPaletteOverride(relearner->sprites[index + 4], TypeIcon_GetPalette(type));
    }
}

void MoveRelearner_UpdateListTypeIcons(MoveRelearner *relearner) {
    u32 i = 0;
    s32 y = 40;
    u8 *shifted = (u8 *)relearner; // advanced by one sprite per row (the original strength reduction)

    for (; i < MOVE_RELEARNER_VISIBLE_ROWS;) {
        ManagedSprite_SetPositionXY(((MoveRelearner *)shifted)->sprites[4], 22, y);
        if (relearner->args->scrollOffset + i >= relearner->numMoves) {
            ManagedSprite_SetDrawFlag(((MoveRelearner *)shifted)->sprites[4], FALSE);
        } else {
            ManagedSprite_SetDrawFlag(((MoveRelearner *)shifted)->sprites[4], TRUE);
            MoveRelearner_SetTypeIcon(relearner, relearner->args->eligibleMoves[relearner->args->scrollOffset + i], i);
        }
        i++;
        y += 32;
        shifted += 4;
    }
}

void MoveRelearner_SetCategoryIcon(MoveRelearner *relearner, int move) {
    int category = ((u32(*)(u32, MoveAttr))GetMoveAttr)(move, MOVEATTR_CLASS); // no truncation of move to u16, as in the original
    NarcId narc = MoveCategoryIcon_GetNarc(category);
    int file = MoveCategoryIcon_GetCharFile(category);

    SpriteSystem_ReplaceCharResObj(relearner->spriteSystem, relearner->spriteManager, narc, file, TRUE, 0xB8AA);
    ManagedSprite_SetPaletteOverride(relearner->sprites[3], MoveCategoryIcon_GetPalette(category) + 4);
}

// Sprites: 0/1 scroll arrows (down/up), 2 cursor, 3 category icon of the selected move,
// 4-7 type icons of the list rows, 8-11 type icons of the Pokemon's current moves.
#pragma force_active on

void MoveRelearner_InitSprites(MoveRelearner *relearner, NARC *narc) {
    u32 i;

    MoveRelearner_InitSpriteSystem(relearner);
    MoveRelearner_LoadSpriteResources(relearner, narc);
    for (i = 0; i < MOVE_RELEARNER_NUM_SPRITES; i++) {
        relearner->sprites[i] = SpriteSystem_NewSprite(relearner->spriteSystem, relearner->spriteManager, &sMoveRelearnerSpriteTemplates[i]);
        ManagedSprite_SetAnimateFlag(relearner->sprites[i], FALSE);
    }
}

void MoveRelearner_CreateCamera(Camera **camera, enum HeapID heapID) {
    VecFx32 pos = sMoveRelearnerCameraPos;
    CameraAngle angle = { 0, 0, 0, 0 };

    *camera = Camera_New(heapID);
    Camera_Init_FromPosDistanceAndAngle(&pos, 0x10000, &angle, 0x5C1, 1, *camera);
    Camera_SetPerspectiveClippingPlane(0, 0x64000, *camera);
    Camera_ClearFixedTarget(*camera);
    Camera_SetStaticPtr(*camera);
}

#pragma force_active reset

void MoveRelearner_CreatePokepic(MoveRelearnerMonPic *monPic, Pokemon *mon, enum HeapID heapID) {
    PokepicTemplate template;
    int species;
    int form;

    monPic->pokepicManager = PokepicManager_Create(heapID);
    GetPokemonSpriteCharAndPlttNarcIds(&template, mon, 2);
    monPic->pokepic = PokepicManager_CreatePokepic(monPic->pokepicManager, &template, 52, 128, 0, 0, NULL, NULL);
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    form = GetMonData(mon, MON_DATA_FORM, NULL);
    Pokepic_SetAttr(monPic->pokepic, POKEPIC_HFLIP, 1 ^ GetMonBaseStat_HandleAlternateForm(species, form, BASE_FLIP));
}

#pragma force_active on

void MoveRelearner_Init3D(MoveRelearner *relearner) {
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_AntiAlias(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, (GXFogBlend)0, (GXFogSlope)0, 0);
    G3X_SetClearColor(0, 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    MI_CpuFill8(&relearner->monPic, 0, sizeof(MoveRelearnerMonPic));
    MoveRelearner_CreateCamera(&relearner->monPic.camera, HEAP_ID_66);
    MoveRelearner_CreatePokepic(&relearner->monPic, relearner->args->mon, HEAP_ID_66);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
}

#pragma force_active reset

void MoveRelearner_Draw3D(MoveRelearnerMonPic *monPic) {
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_Identity();
    NNS_G3dGlbFlushP();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(monPic->pokepicManager);
    G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

void MoveRelearner_Free3D(MoveRelearnerMonPic *monPic) {
    Camera_Delete(monPic->camera);
    PokepicManager_Delete(monPic->pokepicManager);
    MI_CpuFill8(monPic, 0, sizeof(MoveRelearnerMonPic));
}

#pragma force_active on

// pos 0-3: list rows, 4/5: the bottom buttons ("TEACH", "RETURN"), else the yes/no position.
void MoveRelearner_SetCursorPos(MoveRelearner *relearner, u32 pos, int palette) {
    if (pos < 4) {
        ManagedSprite_SetPositionXY(relearner->sprites[2], 0, (s16)(pos * 32 + 32));
        ManagedSprite_SetAnimationFrame(relearner->sprites[2], 0);
    } else if (pos < 6) {
        ManagedSprite_SetPositionXY(relearner->sprites[2], (s16)((pos - 4) * 40), 168);
        ManagedSprite_SetAnimationFrame(relearner->sprites[2], 1);
    } else {
        ManagedSprite_SetPositionXY(relearner->sprites[2], 192, 160);
        ManagedSprite_SetAnimationFrame(relearner->sprites[2], 2);
    }
    ManagedSprite_SetPaletteOverride(relearner->sprites[2], palette);
}

// which: 0 the down arrow, 1 the up arrow is pressed.
void MoveRelearner_UpdateScrollArrows(MoveRelearner *relearner, int which, BOOL animate) {
    int anim;

    if (which == 0 && animate) {
        anim = 1;
        ManagedSprite_ResetSpriteAnimCtrlState(relearner->sprites[0]);
    } else {
        anim = 0;
    }
    if (relearner->args->scrollOffset + 4 < relearner->numMoves) {
        ManagedSprite_SetAnim(relearner->sprites[0], anim);
    } else {
        ManagedSprite_SetAnim(relearner->sprites[0], anim + 2);
    }
    ManagedSprite_SetAnimateFlag(relearner->sprites[0], anim);

    if (which == 1 && animate) {
        anim = 1;
        ManagedSprite_ResetSpriteAnimCtrlState(relearner->sprites[1]);
    } else {
        anim = 0;
    }
    if (relearner->args->scrollOffset) {
        ManagedSprite_SetAnim(relearner->sprites[1], anim + 4);
    } else {
        ManagedSprite_SetAnim(relearner->sprites[1], anim + 6);
    }
    ManagedSprite_SetAnimateFlag(relearner->sprites[1], anim);
}

#pragma force_active reset

int MoveRelearner_FadeOutToSummary(MoveRelearner *relearner) {
    sub_020880CC(1, HEAP_ID_66);
    relearner->nextState = MOVE_RELEARNER_STATE_OPEN_SUMMARY;
    return MOVE_RELEARNER_STATE_WAIT_FADE;
}

int MoveRelearner_OpenSummary(MoveRelearner *relearner) {
    u8 pages[2];

    pages[0] = sMoveRelearnerSummaryPages[0];
    pages[1] = sMoveRelearnerSummaryPages[1];
    MoveRelearner_Teardown(relearner);
    relearner->summary.party = (Party *)relearner->args->mon; // the summary shows this one Pokemon (partyCount 1)
    relearner->summary.options = relearner->args->options;
    relearner->summary.unk11 = 0;
    relearner->summary.partySlot = 0;
    relearner->summary.partyCount = 1;
    relearner->summary.moveToLearn = MoveRelearner_GetSelectedMove(relearner);
    relearner->summary.unk12 = 2;
    relearner->summary.unk2C = 1;
    relearner->summary.unk28 = 0;
    relearner->summary.menuInputStatePtr = NULL;
    sub_02089D40(&relearner->summary, pages);
    relearner->summaryManager = OverlayManager_New(&gOverlayTemplate_PokemonSummary, &relearner->summary, HEAP_ID_66);
    return MOVE_RELEARNER_STATE_RUN_SUMMARY;
}

int MoveRelearner_RunSummary(MoveRelearner *relearner) {
    if (OverlayManager_Run(relearner->summaryManager)) {
        OverlayManager_Delete(relearner->summaryManager);
        MoveRelearner_Setup(relearner);
        MoveRelearner_DrawListRows(relearner, relearner->args->cursorPos);
        MoveRelearner_SetCursorPos(relearner, (u8)relearner->args->cursorPos, 3);
        relearner->args->moveSlot = relearner->summary.unk16; // the move slot chosen in the summary
        relearner->nextState = MOVE_RELEARNER_STATE_ASK_AFTER_SUMMARY;
        return MOVE_RELEARNER_STATE_WAIT_FADE;
    }
    return MOVE_RELEARNER_STATE_RUN_SUMMARY;
}

void MoveRelearner_CreateGridInput(MoveRelearner *relearner) {
    relearner->gridInput = GridInputHandler_Create(sMoveRelearnerGridHitboxes, sMoveRelearnerGridDpadBoxes, &sMoveRelearnerGridCallbacks, relearner, TRUE, 0, HEAP_ID_66);
    MoveRelearner_SetLearnButtonActive(relearner, FALSE);
    MoveRelearner_UpdateScrollButtons(relearner);
}

void MoveRelearner_FreeGridInput(MoveRelearner *relearner) {
    GridInputHandler_Free(relearner->gridInput);
}

void MoveRelearner_GridNoop(void) {
}

// Grid inputs: 0-3 list rows, 4 scroll down, 5 scroll up, 6 "RETURN", 7 "TEACH".
void MoveRelearner_GridOnKeyMove(MoveRelearner *relearner, s32 input, u32 direction) {
    MoveRelearner_SetLearnButtonActive(relearner, FALSE);
    MoveRelearner_DrawListRows(relearner, 5);

    if (input <= 3) {
        PlaySE(SEQ_SE_DP_DECIDE);
        relearner->args->cursorPos = input;
        if (relearner->args->scrollOffset + relearner->args->cursorPos < relearner->numMoves) {
            MoveRelearner_PrintMoveInfo(relearner, MoveRelearner_GetSelectedMove(relearner));
        } else {
            MoveRelearner_PrintMoveInfo(relearner, -2);
        }
    } else if (input == 6) {
        if (direction == 3 && relearner->args->scrollOffset + 4 < relearner->numMoves) {
            PlaySE(SEQ_SE_DP_DECIDE);
            input = 3;
            GridInputHandler_SetNextInput(relearner->gridInput, input);
            relearner->args->scrollOffset++;
            MoveRelearner_PrintMoveList(relearner);
            MoveRelearner_PrintMoveInfo(relearner, MoveRelearner_GetSelectedMove(relearner));
            MoveRelearner_UpdateScrollButtons(relearner);
            MoveRelearner_AnimateScrollArrows(relearner, 1);
        } else if (direction == 0) {
            if (relearner->args->scrollOffset != 0) {
                PlaySE(SEQ_SE_DP_DECIDE);
                relearner->args->scrollOffset--;
                MoveRelearner_PrintMoveList(relearner);
                MoveRelearner_PrintMoveInfo(relearner, MoveRelearner_GetSelectedMove(relearner));
                MoveRelearner_UpdateScrollButtons(relearner);
                MoveRelearner_AnimateScrollArrows(relearner, -1);
            }
            input = 0;
            GridInputHandler_SetNextInput(relearner->gridInput, input);
        } else {
            PlaySE(SEQ_SE_DP_DECIDE);
            MoveRelearner_PrintMoveInfo(relearner, -2);
        }
    } else {
        PlaySE(SEQ_SE_DP_DECIDE);
    }
    MoveRelearner_SetCursorPos(relearner, (u8)input, 3);
}

#pragma force_active on

void MoveRelearner_GridOnTouch(MoveRelearner *relearner, int input) {
    if (input <= 3) {
        relearner->args->cursorPos = input;
        if (relearner->args->scrollOffset + relearner->args->cursorPos < relearner->numMoves) {
            MoveRelearner_DrawListRows(relearner, relearner->args->scrollOffset);
            MoveRelearner_SetLearnButtonActive(relearner, TRUE);
            MoveRelearner_PrintMoveInfo(relearner, MoveRelearner_GetSelectedMove(relearner));
        } else {
            MoveRelearner_DrawListRows(relearner, 5);
            MoveRelearner_SetLearnButtonActive(relearner, FALSE);
            MoveRelearner_PrintMoveInfo(relearner, -2);
        }
    } else if (input == 4) {
        PlaySE(SEQ_SE_DP_DECIDE);
        MoveRelearnerArgs *args = relearner->args;
        input = args->cursorPos;
        args->scrollOffset++;
        MoveRelearner_PrintMoveInfo(relearner, MoveRelearner_GetSelectedMove(relearner));
        MoveRelearner_PrintMoveList(relearner);
        MoveRelearner_DrawListRows(relearner, 5);
        MoveRelearner_SetLearnButtonActive(relearner, FALSE);
        MoveRelearner_UpdateScrollButtons(relearner);
        MoveRelearner_AnimateScrollArrows(relearner, 1);
        GridInputHandler_SetNextInput(relearner->gridInput, (u8)input);
    } else if (input == 5) {
        PlaySE(SEQ_SE_DP_DECIDE);
        MoveRelearnerArgs *args = relearner->args;
        input = args->cursorPos;
        args->scrollOffset--;
        MoveRelearner_PrintMoveInfo(relearner, MoveRelearner_GetSelectedMove(relearner));
        MoveRelearner_PrintMoveList(relearner);
        MoveRelearner_DrawListRows(relearner, 5);
        MoveRelearner_SetLearnButtonActive(relearner, FALSE);
        MoveRelearner_UpdateScrollButtons(relearner);
        MoveRelearner_AnimateScrollArrows(relearner, -1);
        GridInputHandler_SetNextInput(relearner->gridInput, (u8)input);
    } else if (input == 6) {
        MoveRelearner_DrawListRows(relearner, 5);
        MoveRelearner_SetLearnButtonActive(relearner, FALSE);
        if (relearner->lastGridInput != 6) {
            input = relearner->args->cursorPos;
            GridInputHandler_SetNextInput(relearner->gridInput, (u8)input);
        }
    } else if (input == 7) {
        input = relearner->args->cursorPos;
        GridInputHandler_SetNextInput(relearner->gridInput, (u8)input);
    }
    MoveRelearner_SetCursorPos(relearner, (u8)input, 3);
}

#pragma force_active reset

void MoveRelearner_SetLearnButtonActive(MoveRelearner *relearner, u32 active) {
    if (active == TRUE) {
        GridInputHandler_SetEnabledFlag(relearner->gridInput, 7);
        ScheduleWindowCopyToVram(&relearner->windows[8]);
        MoveRelearner_CopyButtonTiles(relearner, 15, 20, 8, 4, 8, 12);
        relearner->learnButtonActive = TRUE;
    } else {
        GridInputHandler_ClearEnabledFlag(relearner->gridInput, 7);
        ClearWindowTilemapAndScheduleTransfer(&relearner->windows[8]);
        MoveRelearner_CopyButtonTiles(relearner, 15, 20, 8, 4, 0, 12);
        relearner->learnButtonActive = FALSE;
    }
}

void MoveRelearner_UpdateScrollButtons(MoveRelearner *relearner) {
    if (relearner->numMoves < 4) {
        GridInputHandler_ClearEnabledFlag(relearner->gridInput, 4);
        GridInputHandler_ClearEnabledFlag(relearner->gridInput, 5);
        return;
    }

    if (relearner->args->scrollOffset == 0) {
        GridInputHandler_ClearEnabledFlag(relearner->gridInput, 5);
    } else {
        GridInputHandler_SetEnabledFlag(relearner->gridInput, 5);
    }

    if (relearner->args->scrollOffset + 4 >= relearner->numMoves) {
        GridInputHandler_ClearEnabledFlag(relearner->gridInput, 4);
    } else {
        GridInputHandler_SetEnabledFlag(relearner->gridInput, 4);
    }
}

void MoveRelearner_AnimateScrollArrows(MoveRelearner *relearner, s32 direction) {
    ManagedSprite_SetAnimateFlag(relearner->sprites[1], TRUE);
    ManagedSprite_SetAnimateFlag(relearner->sprites[0], TRUE);
    ManagedSprite_SetAnimationFrame(relearner->sprites[1], 0);
    ManagedSprite_SetAnimationFrame(relearner->sprites[0], 0);

    if (direction > 0) {
        if (relearner->args->scrollOffset + 4 < relearner->numMoves) {
            ManagedSprite_SetAnim(relearner->sprites[0], 1);
        } else {
            ManagedSprite_SetAnim(relearner->sprites[0], 3);
        }
        ManagedSprite_SetAnim(relearner->sprites[1], 4);
    } else {
        if (relearner->args->scrollOffset == 0) {
            ManagedSprite_SetAnim(relearner->sprites[1], 7);
        } else {
            ManagedSprite_SetAnim(relearner->sprites[1], 5);
        }
        ManagedSprite_SetAnim(relearner->sprites[0], 0);
    }
}

#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_common_subs on

// Row backgrounds: the selected row highlighted, rows past the end of the list empty.
void MoveRelearner_DrawListRows(MoveRelearner *relearner, u32 selected) {
    u32 i;
    u32 y;

    i = 0;
    y = 4;
    do {
        if (i == selected) {
            MoveRelearner_CopyButtonTiles(relearner, 0, (u8)y, 16, 4, 0, 8);
        } else if (i >= relearner->numMoves) {
            MoveRelearner_CopyButtonTiles(relearner, 0, (u8)y, 16, 4, 0, 4);
        } else {
            MoveRelearner_CopyButtonTiles(relearner, 0, (u8)y, 16, 4, 0, 0);
        }
        y += 4;
        i++;
    } while (i < MOVE_RELEARNER_VISIBLE_ROWS);
}

#pragma opt_common_subs reset
#pragma opt_lifetimes reset
#pragma force_active reset

int MoveRelearner_StartReturnButtonFlash(MoveRelearner *relearner, int nextState) {
    MoveRelearnerButtonFlash *flash = &relearner->buttonFlash;

    flash->x = 24;
    flash->y = 20;
    flash->width = 8;
    flash->height = 4;
    flash->timer = 0;
    flash->state = 0;
    flash->nextState = nextState;
    return MOVE_RELEARNER_STATE_BUTTON_FLASH;
}

int MoveRelearner_StartLearnButtonFlash(MoveRelearner *relearner, int nextState) {
    MoveRelearnerButtonFlash *flash = &relearner->buttonFlash;

    flash->x = 15;
    flash->y = 20;
    flash->width = 8;
    flash->height = 4;
    flash->timer = 0;
    flash->state = 0;
    flash->nextState = nextState;
    return MOVE_RELEARNER_STATE_BUTTON_FLASH;
}

#pragma force_active on

int MoveRelearner_UpdateButtonFlash(MoveRelearner *relearner) {
    MoveRelearnerButtonFlash *flash = &relearner->buttonFlash;

    switch (flash->state) {
    case 0:
        BgTilemapRectChangePalette(relearner->bgConfig, GF_BG_LYR_SUB_3, flash->x, flash->y, flash->width, flash->height, 7);
        ScheduleBgTilemapBufferTransfer(relearner->bgConfig, GF_BG_LYR_SUB_3);
        flash->state++;
        break;
    case 1:
        flash->timer++;
        if (flash->timer == 4) {
            BgTilemapRectChangePalette(relearner->bgConfig, GF_BG_LYR_SUB_3, flash->x, flash->y, flash->width, flash->height, 0);
            ScheduleBgTilemapBufferTransfer(relearner->bgConfig, GF_BG_LYR_SUB_3);
            flash->timer = 0;
            flash->state++;
        }
        break;
    case 2:
        flash->timer++;
        if (flash->timer == 2) {
            return flash->nextState;
        }
        break;
    }
    return MOVE_RELEARNER_STATE_BUTTON_FLASH;
}

#pragma force_active reset

int MoveRelearner_AskGiveUp(MoveRelearner *relearner) {
    MoveRelearner_PrintMessage(relearner, 2);
    relearner->question = MOVE_RELEARNER_QUESTION_GIVE_UP;
    relearner->nextState = MOVE_RELEARNER_STATE_SHOW_YES_NO;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}

int MoveRelearner_AskTeachAfterFlash(MoveRelearner *relearner) {
    return MoveRelearner_AskTeach(relearner);
}

int MoveRelearner_AskTeach(MoveRelearner *relearner) {
    u8 question;

    if (MoveRelearner_CountKnownMoves(relearner) < MAX_MON_MOVES) {
        MoveRelearner_PrintMessage(relearner, 1);
        question = MOVE_RELEARNER_QUESTION_TEACH;
    } else {
        MoveRelearner_PrintMessage(relearner, 4);
        question = MOVE_RELEARNER_QUESTION_DELETE_MOVE;
    }
    relearner->question = question;
    relearner->nextState = MOVE_RELEARNER_STATE_SHOW_YES_NO;
    return MOVE_RELEARNER_STATE_WAIT_MESSAGE;
}
