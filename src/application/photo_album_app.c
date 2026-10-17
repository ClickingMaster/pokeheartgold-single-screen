#include "photo_album_app.h"

#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "data/resdat.naix"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0000.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "map_landmark_name.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "options.h"
#include "overlay_manager.h"
#include "photo_album.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

#define PHOTOS_PER_PAGE 12

enum {
    PHOTO_ALBUM_STATE_FADE_IN,
    PHOTO_ALBUM_STATE_GRID,
    PHOTO_ALBUM_STATE_FLIP_TO_PREV_PAGE,
    PHOTO_ALBUM_STATE_FLIP_TO_NEXT_PAGE,
    PHOTO_ALBUM_STATE_CONTEXT_MENU,
    PHOTO_ALBUM_STATE_MOVE_PHOTO,
    PHOTO_ALBUM_STATE_MOVE_PHOTO_DONE,
    PHOTO_ALBUM_STATE_DELETE_PROMPT,
    PHOTO_ALBUM_STATE_FADE_OUT,
    PHOTO_ALBUM_STATE_EXIT,
};

enum {
    PHOTO_ALBUM_MODE_NORMAL,
    PHOTO_ALBUM_MODE_MOVE,
    PHOTO_ALBUM_MODE_DELETE,
};

enum {
    PHOTO_ALBUM_INPUT_CLOSE = PHOTOS_PER_PAGE,
    PHOTO_ALBUM_INPUT_PREV_PAGE,
    PHOTO_ALBUM_INPUT_NEXT_PAGE,
};

enum {
    PHOTO_MENU_SELECT,
    PHOTO_MENU_DELETE,
    PHOTO_MENU_MOVE,
};

typedef struct PhotoEntry {
    u8 idx;
    u8 filler_01[3];
    Photo *photo;
} PhotoEntry;

typedef struct PhotoAlbumApp {
    enum HeapID heapId;                          // 0x000
    int state;                                   // 0x004
    u16 subState;                                // 0x008
    u16 timer;                                   // 0x00A
    int nextMenuState;                           // 0x00C
    PhotoAlbumArgs *args;                        // 0x010
    BgConfig *bgConfig;                          // 0x014
    u8 fadeRunning : 1;                          // 0x018
    u8 zoomChanged : 1;                          // 0x018
    u8 bgToggleReq : 1;                          // 0x018
    u8 bgToggleVal : 1;                          // 0x018
    u8 pageFlipPhase : 4;                        // 0x018
    u8 curPage;                                  // 0x019
    u8 numPages;                                 // 0x01A
    u8 cursorX;                                  // 0x01B
    u8 cursorY;                                  // 0x01C
    u8 spriteCol;                                // 0x01D
    u8 spriteRow;                                // 0x01E
    u8 slotIdx;                                  // 0x01F
    u8 mode;                                     // 0x020: 1 = moving a photo, 2 = deleting
    u8 printerId;                                // 0x021
    u8 textDelay;                                // 0x022
    u8 frame;                                    // 0x023
    MsgData *msgData;                            // 0x024
    MessageFormat *messageFormat;                // 0x028
    String *str_2C;                              // 0x02C
    String *str_30;                              // 0x030
    String *strs_34[5];                          // 0x034
    String *strs_48[2];                          // 0x048
    Window windows[3];                           // 0x050
    TouchscreenListMenuSpawner *listMenuSpawner; // 0x080
    TouchscreenListMenu *listMenu;               // 0x084
    ListMenuItem *listMenuItems;                 // 0x088
    YesNoPrompt *yesNoPrompt;                    // 0x08C
    SpriteSystem *spriteSystem;                  // 0x090
    SpriteManager *spriteManager;                // 0x094
    Sprite *sprites[5];                          // 0x098
    NNSG2dScreenData *screen_AC;                 // 0x0AC
    void *screenFile_B0;                         // 0x0B0
    NNSG2dScreenData *photoIcons;                // 0x0B4
    void *photoIconsFile;                        // 0x0B8
    NNSG2dScreenData *pageArrows;                // 0x0BC
    void *pageArrowsFile;                        // 0x0C0
    u8 numPhotosPrev;                            // 0x0C4
    u8 numPhotos;                                // 0x0C5
    u8 filler_C6[0x2];                           // 0x0C6
    Photo *photos;                               // 0x0C8
    PhotoEntry entries[PHOTO_ALBUM_MAX];         // 0x0CC
    PhotoAlbum *photoAlbum;                      // 0x1EC
    fx32 zoom;                                   // 0x1F0
} PhotoAlbumApp;                                 // size: 0x1F4

typedef struct PhotoAlbumTask {
    PhotoAlbumApp *app;
    u8 filler_04[4];
} PhotoAlbumTask;

static void PhotoAlbumApp_ResetDisplay(void);
static void PhotoAlbumApp_InitState(PhotoAlbumApp *app);
static void PhotoAlbumApp_Finish(PhotoAlbumApp *app);
static BOOL PhotoAlbumApp_Setup(PhotoAlbumApp *app);
static BOOL PhotoAlbumApp_Teardown(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_FadeIn(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_FadeOut(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_Grid(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_FlipToPrevPage(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_FlipToNextPage(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_ContextMenu(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_MovePhoto(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_MovePhotoDone(PhotoAlbumApp *app);
static int PhotoAlbumApp_State_DeletePrompt(PhotoAlbumApp *app);
static void PhotoAlbumApp_InitGraphics(PhotoAlbumApp *app);
static void PhotoAlbumApp_FreeGraphics(PhotoAlbumApp *app);
static void PhotoAlbumApp_InitEntries(PhotoAlbumApp *app);
static void PhotoAlbumApp_WritePhotosToSave(PhotoAlbumApp *app);
static void PhotoAlbumApp_SwapEntries(PhotoAlbumApp *app, int a, int b);
static void PhotoAlbumApp_RemoveEntry(PhotoAlbumApp *app, u8 selected);
static void PhotoAlbumApp_DrawInitialScreen(PhotoAlbumApp *app);
static int PhotoAlbumApp_HandleGridInput(PhotoAlbumApp *app);
static int PhotoAlbumApp_HandleGridSelection(PhotoAlbumApp *app, int input);
static int PhotoAlbumApp_HandleGridTouch(PhotoAlbumApp *app, int *a);
static int PhotoAlbumApp_HandleGridKeys(PhotoAlbumApp *app);
static int PhotoAlbumApp_HandleMoveInput(PhotoAlbumApp *app);
static int PhotoAlbumApp_RunMoveDone(PhotoAlbumApp *app);
static int PhotoAlbumApp_HandleMoveSelection(PhotoAlbumApp *app, u32 input);
static int PhotoAlbumApp_HandleMoveTouch(PhotoAlbumApp *app, int *a);
static int PhotoAlbumApp_HandleMoveKeys(PhotoAlbumApp *app);
static int PhotoAlbumApp_FlipToPrevPage(PhotoAlbumApp *app);
static int PhotoAlbumApp_FlipToNextPage(PhotoAlbumApp *app);
static int PhotoAlbumApp_RunContextMenu(PhotoAlbumApp *app);
static int PhotoAlbumApp_RunDeletePrompt(PhotoAlbumApp *app);
static void PhotoAlbumApp_VBlankCB(void *data);
static void PhotoAlbumApp_ApplyBgToggle(PhotoAlbumApp *app);
static void PhotoAlbumApp_RequestBgToggle(PhotoAlbumApp *app, int on);
static void PhotoAlbumApp_ApplyZoom(PhotoAlbumApp *app);
static void PhotoAlbumApp_SetZoom(PhotoAlbumApp *app, fx32 v);
static void PhotoAlbumApp_SetBanks(void);
static void PhotoAlbumApp_InitBgs(PhotoAlbumApp *app);
static void PhotoAlbumApp_FreeBgs(PhotoAlbumApp *app);
static void PhotoAlbumApp_LoadBgGfx(PhotoAlbumApp *app);
static void PhotoAlbumApp_FreeScreenData(PhotoAlbumApp *app);
static void PhotoAlbumApp_LoadStrings(PhotoAlbumApp *app);
static void PhotoAlbumApp_FreeStrings(PhotoAlbumApp *app);
static void PhotoAlbumApp_CreateWindows(PhotoAlbumApp *app);
static void PhotoAlbumApp_RemoveWindows(PhotoAlbumApp *app);
static void PhotoAlbumApp_InitSpriteSystem(PhotoAlbumApp *app);
static void PhotoAlbumApp_FreeSpriteSystem(PhotoAlbumApp *app);
static void PhotoAlbumApp_InitSpritesAndMenus(PhotoAlbumApp *app);
static void PhotoAlbumApp_FreeSpritesAndMenus(PhotoAlbumApp *app);
static void PhotoAlbumApp_CreateSprites(PhotoAlbumApp *app);
static void PhotoAlbumApp_DeleteSprites(PhotoAlbumApp *app);
static void PhotoAlbumApp_DrawPageArrows(PhotoAlbumApp *app, int position, int draw);
static void PhotoAlbumApp_DrawPageArrowBg(PhotoAlbumApp *app, int bgId, int mode, int index);
static void PhotoAlbumApp_DrawGridColumns(PhotoAlbumApp *app, int bgId, int page, u8 start);
static void PhotoAlbumApp_DrawPage(PhotoAlbumApp *app, int bgId, int mask, u8 page);
static void PhotoAlbumApp_DrawPhotoIcon(PhotoAlbumApp *app, int index, u8 bgId, u8 destX, u8 destY);
static void PhotoAlbumApp_DrawGridIcon(PhotoAlbumApp *app, Photo *photo, int bgId, u8 x, u8 y);
static void PhotoAlbumApp_RedrawSwappedIcons(PhotoAlbumApp *app, int oldIndex, int newIndex);
static void PhotoAlbumApp_PrintPhotoDetails(PhotoAlbumApp *app, Photo *photo);
static void PhotoAlbumApp_PrintCloseButton(PhotoAlbumApp *app, int a);
static void PhotoAlbumApp_PrintMessage(PhotoAlbumApp *app, int msgIdx, BOOL drawFrame, u8 textDelay);
static void PhotoAlbumApp_SetCursorPos(PhotoAlbumApp *app, u8 x, u8 y, int draw);
static void PhotoAlbumApp_UpdateSelectionSprite(PhotoAlbumApp *app, int draw);
static void PhotoAlbumApp_UpdatePageArrows(PhotoAlbumApp *app);
static void PhotoAlbumApp_AnimatePageArrow(PhotoAlbumApp *app, int idx);
static int PhotoAlbumApp_ToggleMenuInputState(PhotoAlbumApp *app);
static void PhotoAlbumApp_OpenContextMenu(PhotoAlbumApp *app);
static int PhotoAlbumApp_HandleContextMenu(PhotoAlbumApp *app);
static void PhotoAlbumApp_OpenDeletePrompt(PhotoAlbumApp *app);
static int PhotoAlbumApp_HandleDeletePrompt(PhotoAlbumApp *app);
static void PhotoAlbumApp_StartCloseButtonAnim(PhotoAlbumApp *app);
static void Task_PhotoAlbumApp_WaitCloseButtonAnim(SysTask *task, void *taskData);
static u8 Photo_CountPartyMons(Photo *photo);

static const u8 sPhotoAlbumGridSlots[] = { 0, 1, 6, 7, 2, 3, 8, 9, 4, 5, 10, 11 };

static const ResdatIdList sPhotoAlbumSpriteResources = {
    .charRes = NARC_resdat_resdat_00000002_bin,
    .plttRes = NARC_resdat_resdat_00000003_bin,
    .cellRes = NARC_resdat_resdat_00000001_bin,
    .animRes = NARC_resdat_resdat_00000000_bin,
    .mcelRes = 0xFFFF,
    .manmRes = 0xFFFF,
    .headerId = NARC_resdat_resdat_00000072_bin,
};

static const OamCharTransferParam sPhotoAlbumAppOamCharTransferParam = {
    .maxTasks = 3,
    .sizeMain = 0,
    .sizeSub = 0,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_32K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const BgTemplate sPhotoAlbumBgTemplate3 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_256,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const BgTemplate sPhotoAlbumBgTemplate6 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const BgTemplate sPhotoAlbumBgTemplate0 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const u8 sPhotoAlbumSlotGridPositions[] = { 0, 1, 4, 5, 8, 9, 2, 3, 6, 7, 10, 11 };

static const BgTemplate sPhotoAlbumBgTemplate1 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const BgTemplate sPhotoAlbumBgTemplate2 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_256,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const TouchscreenListMenuTemplate sPhotoAlbumContextMenuTemplate = {
    .wrapAround = 1,
    .centered = 1,
    .xOffset = 0,
    .bgId = 0,
    .plttOffset = 11,
    .unk4 = 0,
    .unk5 = 0,
    .baseTile = 58,
    .charOffset = 31,
    .unkA = 64,
};

static const WindowTemplate sPhotoAlbumWindowTemplates[3] = {
    { GF_BG_LYR_MAIN_1, 24, 21, 8,  2, 1,  0x102 },
    { GF_BG_LYR_MAIN_0, 2,  19, 19, 4, 12, 0x0b6 },
    { GF_BG_LYR_SUB_0,  1,  15, 28, 8, 10, 0x01f },
};

static const GraphicsBanks sPhotoAlbumAppGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

static const GraphicsModes sPhotoAlbumAppGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_5,
    .subMode = GX_BGMODE_3,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const fx32 sPhotoAlbumPageFlipZoom[] = { FX32_ONE, FX32_CONST(0.8), FX32_CONST(0.6), FX32_CONST(0.4), FX32_CONST(0.2), 0 };

static const BgTemplate sPhotoAlbumBgTemplate7 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_256,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const OamManagerParam sPhotoAlbumAppOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const BgTemplate sPhotoAlbumBgTemplate4 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const TouchscreenHitbox sPhotoAlbumGridHitboxes[] = {
    { .rect = { 12, 44, 32, 72 } },
    { .rect = { 12, 44, 80, 120 } },
    { .rect = { 12, 44, 136, 176 } },
    { .rect = { 12, 44, 184, 224 } },
    { .rect = { 52, 84, 32, 72 } },
    { .rect = { 52, 84, 80, 120 } },
    { .rect = { 52, 84, 136, 176 } },
    { .rect = { 52, 84, 184, 224 } },
    { .rect = { 92, 124, 32, 72 } },
    { .rect = { 92, 124, 80, 120 } },
    { .rect = { 92, 124, 136, 176 } },
    { .rect = { 92, 124, 184, 224 } },
    { .rect = { 162, 188, 194, 254 } },
    { .rect = { 48, 80, 8, 24 } },
    { .rect = { 48, 80, 232, 248 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const UnmanagedSpriteTemplate sPhotoAlbumSpriteTemplates[] = {
    { 0, 28,  8,   0, 6, 0,   0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 28,  8,   0, 7, 1,   0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 16,  64,  0, 0, 2,   0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 240, 64,  0, 3, 2,   0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 224, 176, 0, 8, 255, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
};

BOOL PhotoAlbum_Init(OverlayManager *man, int *state) {
    PhotoAlbumApp *app;
    switch (*state) {
    case 0:
        PhotoAlbumApp_ResetDisplay();
        Heap_Create(HEAP_ID_3, HEAP_ID_96, 0x20000);
        app = OverlayManager_CreateAndGetData(man, sizeof(PhotoAlbumApp), HEAP_ID_96);
        MI_CpuFill8(app, 0, sizeof(PhotoAlbumApp));
        app->heapId = HEAP_ID_96;
        app->args = OverlayManager_GetArgs(man);
        PhotoAlbumApp_InitState(app);
        ++(*state);
        break;
    case 1:
        app = OverlayManager_GetData(man);
        if (PhotoAlbumApp_Setup(app)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL PhotoAlbum_Main(OverlayManager *man, int *state) {
    PhotoAlbumApp *app = OverlayManager_GetData(man);
    switch (*state) {
    case PHOTO_ALBUM_STATE_FADE_IN:
        *state = PhotoAlbumApp_State_FadeIn(app);
        break;
    case PHOTO_ALBUM_STATE_GRID:
        *state = PhotoAlbumApp_State_Grid(app);
        break;
    case PHOTO_ALBUM_STATE_FLIP_TO_PREV_PAGE:
        *state = PhotoAlbumApp_State_FlipToPrevPage(app);
        break;
    case PHOTO_ALBUM_STATE_FLIP_TO_NEXT_PAGE:
        *state = PhotoAlbumApp_State_FlipToNextPage(app);
        break;
    case PHOTO_ALBUM_STATE_CONTEXT_MENU:
        *state = PhotoAlbumApp_State_ContextMenu(app);
        break;
    case PHOTO_ALBUM_STATE_MOVE_PHOTO:
        *state = PhotoAlbumApp_State_MovePhoto(app);
        break;
    case PHOTO_ALBUM_STATE_MOVE_PHOTO_DONE:
        *state = PhotoAlbumApp_State_MovePhotoDone(app);
        break;
    case PHOTO_ALBUM_STATE_DELETE_PROMPT:
        *state = PhotoAlbumApp_State_DeletePrompt(app);
        break;
    case PHOTO_ALBUM_STATE_FADE_OUT:
        *state = PhotoAlbumApp_State_FadeOut(app);
        break;
    case PHOTO_ALBUM_STATE_EXIT:
        return TRUE;
    }
    SpriteSystem_DrawSprites(app->spriteManager);
    return FALSE;
}

BOOL PhotoAlbum_Exit(OverlayManager *man, int *state) {
    PhotoAlbumApp *app = OverlayManager_GetData(man);
    if (!PhotoAlbumApp_Teardown(app)) {
        return FALSE;
    }
    PhotoAlbumApp_ResetDisplay();
    PhotoAlbumApp_Finish(app);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_96);
    return TRUE;
}

static void PhotoAlbumApp_ResetDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetMasterBrightnessByColor(PM_LCD_TOP, 0);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, 0);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}

static void PhotoAlbumApp_InitState(PhotoAlbumApp *app) {
    Options *options;
    int cursorPos;
    int gridPos;
    app->nextMenuState = MenuInputStateMgr_GetState(app->args->menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(app->args->saveData);
    app->textDelay = Options_GetTextFrameDelay(options);
    app->frame = Options_GetFrame(options);
    cursorPos = app->args->cursorPos;
    gridPos = sPhotoAlbumSlotGridPositions[cursorPos % PHOTOS_PER_PAGE];
    app->curPage = cursorPos / PHOTOS_PER_PAGE;
    app->cursorX = gridPos % 4;
    app->cursorY = (u32)gridPos >> 2;
    app->photoAlbum = Save_PhotoAlbum_Get(app->args->saveData);
    app->photos = PhotoAlbum_LoadAllInUsePhotos(app->photoAlbum, app->heapId);
    app->numPhotosPrev = PhotoAlbum_GetNumSaved(app->photoAlbum);
    app->numPhotos = app->numPhotosPrev;
    app->numPages = app->numPhotos / PHOTOS_PER_PAGE;
    if (app->numPhotos % PHOTOS_PER_PAGE != 0) {
        app->numPages++;
    }
    if (app->numPhotos == 0) {
        app->numPages = 1;
    }
    PhotoAlbumApp_InitEntries(app);
}

static void PhotoAlbumApp_Finish(PhotoAlbumApp *app) {
    PhotoAlbumApp_WritePhotosToSave(app);
    Heap_Free(app->photos);
    MenuInputStateMgr_SetState(app->args->menuInputStatePtr, (MenuInputState)app->nextMenuState);
}

static BOOL PhotoAlbumApp_Setup(PhotoAlbumApp *app) {
    switch (app->state) {
    case 0:
        PhotoAlbumApp_InitGraphics(app);
        ++app->state;
        break;
    case 1:
        PhotoAlbumApp_DrawInitialScreen(app);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(4);
        app->state = 0;
        return TRUE;
    }
    return FALSE;
}

static BOOL PhotoAlbumApp_Teardown(PhotoAlbumApp *app) {
    GF_TouchpadStopSampling();
    PhotoAlbumApp_FreeGraphics(app);
    return TRUE;
}

static int PhotoAlbumApp_State_FadeIn(PhotoAlbumApp *app) {
    switch (app->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, app->heapId);
        ++app->state;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            app->state = 0;
            return PHOTO_ALBUM_STATE_GRID;
        }
        break;
    }
    return PHOTO_ALBUM_STATE_FADE_IN;
}

static int PhotoAlbumApp_State_FadeOut(PhotoAlbumApp *app) {
    switch (app->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, app->heapId);
        ++app->state;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            app->state = 0;
            return PHOTO_ALBUM_STATE_EXIT;
        }
        break;
    }
    return PHOTO_ALBUM_STATE_FADE_OUT;
}

static int PhotoAlbumApp_State_Grid(PhotoAlbumApp *app) {
    return PhotoAlbumApp_HandleGridInput(app);
}

static int PhotoAlbumApp_State_FlipToPrevPage(PhotoAlbumApp *app) {
    if (!PhotoAlbumApp_FlipToPrevPage(app)) {
        return PHOTO_ALBUM_STATE_FLIP_TO_PREV_PAGE;
    }
    if (app->mode == PHOTO_ALBUM_MODE_MOVE) {
        return PHOTO_ALBUM_STATE_MOVE_PHOTO;
    }
    return PHOTO_ALBUM_STATE_GRID;
}

static int PhotoAlbumApp_State_FlipToNextPage(PhotoAlbumApp *app) {
    if (!PhotoAlbumApp_FlipToNextPage(app)) {
        return PHOTO_ALBUM_STATE_FLIP_TO_NEXT_PAGE;
    }
    if (app->mode == PHOTO_ALBUM_MODE_MOVE) {
        return PHOTO_ALBUM_STATE_MOVE_PHOTO;
    }
    return PHOTO_ALBUM_STATE_GRID;
}

static int PhotoAlbumApp_State_ContextMenu(PhotoAlbumApp *app) {
    return PhotoAlbumApp_RunContextMenu(app);
}

static int PhotoAlbumApp_State_MovePhoto(PhotoAlbumApp *app) {
    return PhotoAlbumApp_HandleMoveInput(app);
}

static int PhotoAlbumApp_State_MovePhotoDone(PhotoAlbumApp *app) {
    if (PhotoAlbumApp_RunMoveDone(app)) {
        return PHOTO_ALBUM_STATE_GRID;
    }
    return PHOTO_ALBUM_STATE_MOVE_PHOTO_DONE;
}

static int PhotoAlbumApp_State_DeletePrompt(PhotoAlbumApp *app) {
    return PhotoAlbumApp_RunDeletePrompt(app);
}

static void PhotoAlbumApp_InitGraphics(PhotoAlbumApp *app) {
    PhotoAlbumApp_InitBgs(app);
    PhotoAlbumApp_LoadBgGfx(app);
    PhotoAlbumApp_LoadStrings(app);
    PhotoAlbumApp_CreateWindows(app);
    PhotoAlbumApp_InitSpritesAndMenus(app);
    Main_SetVBlankIntrCB(PhotoAlbumApp_VBlankCB, app);
}

static void PhotoAlbumApp_FreeGraphics(PhotoAlbumApp *app) {
    PhotoAlbumApp_FreeSpritesAndMenus(app);
    PhotoAlbumApp_RemoveWindows(app);
    PhotoAlbumApp_FreeStrings(app);
    PhotoAlbumApp_FreeScreenData(app);
    PhotoAlbumApp_FreeBgs(app);
}

static void PhotoAlbumApp_InitEntries(PhotoAlbumApp *app) {
    u8 i;
    app->numPhotos = app->numPhotosPrev;
    MI_CpuFill8(app->entries, 0, sizeof(app->entries));
    for (i = 0; i < app->numPhotos; ++i) {
        PhotoEntry *entry = &app->entries[i];
        entry->idx = i;
        entry->photo = &app->photos[i];
    }
}

static void PhotoAlbumApp_WritePhotosToSave(PhotoAlbumApp *app) {
    u8 i = 0;
    while (i < app->numPhotos) {
        PhotoAlbum_SetPhotoAtIndex(app->photoAlbum, app->entries[i].photo, i);
        ++i;
    }
    while (i < app->numPhotosPrev) {
        PhotoAlbum_DeletePhotoByIndex(app->photoAlbum, i);
        ++i;
    }
}

static void PhotoAlbumApp_SwapEntries(PhotoAlbumApp *app, int a, int b) {
    PhotoEntry tmp;
    MI_CpuCopy8(&app->entries[a], &tmp, sizeof(PhotoEntry));
    MI_CpuCopy8(&app->entries[b], &app->entries[a], sizeof(PhotoEntry));
    MI_CpuCopy8(&tmp, &app->entries[b], sizeof(PhotoEntry));
}

// Removes the photo in slot `selected` from the list and moves the cursor onto a remaining photo.
static void PhotoAlbumApp_RemoveEntry(PhotoAlbumApp *app, u8 selected) {
    u8 i;
    int gridPos;
    app->numPhotos--;
    if (selected < app->numPhotos) {
        for (i = selected; i < app->numPhotos; i++) {
            MI_CpuCopy8(&app->entries[i + 1], &app->entries[i], sizeof(PhotoEntry));
        }
    }
    MI_CpuFill8(&app->entries[app->numPhotos], 0, sizeof(PhotoEntry));
    app->numPages = app->numPhotos / PHOTOS_PER_PAGE;
    if (app->numPhotos % PHOTOS_PER_PAGE) {
        app->numPages++;
    }
    if (app->numPhotos == 0) {
        app->numPages = 1;
    }
    if (selected >= app->numPhotos) {
        if (app->numPhotos == 0) {
            app->cursorX = 3;
            app->cursorY = 3;
            app->curPage = 0;
            return;
        }
        if (selected != 0) {
            selected--;
        }
        app->curPage = selected / PHOTOS_PER_PAGE;
        gridPos = sPhotoAlbumSlotGridPositions[selected % PHOTOS_PER_PAGE];
        app->cursorX = gridPos % 4;
        app->cursorY = (u32)gridPos >> 2;
    }
}

static void PhotoAlbumApp_DrawInitialScreen(PhotoAlbumApp *app) {
    PhotoAlbumApp_DrawPage(app, 3, 3, app->curPage);
    PhotoAlbumApp_UpdatePageArrows(app);
    PhotoAlbumApp_PrintCloseButton(app, 0);
    PhotoAlbumApp_PrintMessage(app, 0, TRUE, 0);
    PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
}

static int PhotoAlbumApp_HandleGridInput(PhotoAlbumApp *app) {
    int done = 0;
    if (app->fadeRunning) {
        return PHOTO_ALBUM_STATE_GRID;
    }
    PhotoAlbumApp_HandleGridTouch(app, &done);
    if (!done) {
        PhotoAlbumApp_HandleGridKeys(app);
    }
}

// Handles a selection on the photo grid: 0-11 = photo, 12 = close, 13/14 = previous/next page.
static int PhotoAlbumApp_HandleGridSelection(PhotoAlbumApp *app, int input) {
    u8 selected;
    switch (input) {
    case PHOTO_ALBUM_INPUT_CLOSE:
        app->args->photoWasSelected = FALSE;
        PlaySE(SEQ_SE_DP_SELECT);
        PhotoAlbumApp_PrintCloseButton(app, 1);
        return PHOTO_ALBUM_STATE_FADE_OUT;
    case PHOTO_ALBUM_INPUT_PREV_PAGE:
        if (app->curPage == 0) {
            return PHOTO_ALBUM_STATE_GRID;
        }
        PhotoAlbumApp_AnimatePageArrow(app, 2);
        PlaySE(SEQ_SE_GS_ALBUMMEKURU);
        return PHOTO_ALBUM_STATE_FLIP_TO_PREV_PAGE;
    case PHOTO_ALBUM_INPUT_NEXT_PAGE:
        if (app->curPage == app->numPages - 1) {
            return PHOTO_ALBUM_STATE_GRID;
        }
        PhotoAlbumApp_AnimatePageArrow(app, 3);
        PlaySE(SEQ_SE_GS_ALBUMMEKURU);
        return PHOTO_ALBUM_STATE_FLIP_TO_NEXT_PAGE;
    }
    selected = (u8)(sPhotoAlbumGridSlots[input] + app->curPage * PHOTOS_PER_PAGE);
    if (selected >= app->numPhotos) {
        return PHOTO_ALBUM_STATE_GRID;
    }
    app->slotIdx = selected;
    app->spriteCol = app->cursorX;
    app->spriteRow = app->cursorY;
    PlaySE(SEQ_SE_DP_SELECT);
    PhotoAlbumApp_OpenContextMenu(app);
    PhotoAlbumApp_PrintPhotoDetails(app, app->entries[selected].photo);
    PhotoAlbumApp_UpdateSelectionSprite(app, 1);
    return PHOTO_ALBUM_STATE_CONTEXT_MENU;
}

static int PhotoAlbumApp_HandleGridTouch(PhotoAlbumApp *app, int *a) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(sPhotoAlbumGridHitboxes);
    if (hit == -1) {
        return PHOTO_ALBUM_STATE_GRID;
    }
    if (app->nextMenuState == 0) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }
    *a = 1;
    if ((u32)hit < PHOTOS_PER_PAGE) {
        app->cursorX = hit & 3;
        app->cursorY = (u32)hit >> 2;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        PlaySE(SEQ_SE_DP_SELECT);
    }
    PhotoAlbumApp_HandleGridSelection(app, (u8)hit);
}

static int PhotoAlbumApp_HandleGridKeys(PhotoAlbumApp *app) {
    u32 keys;
    u8 hit;
    if (app->nextMenuState == 1 && (gSystem.newKeys & 0xF3)) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_A) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = PHOTO_ALBUM_INPUT_CLOSE;
        } else {
            hit += app->cursorY * 4;
        }
        return PhotoAlbumApp_HandleGridSelection(app, hit);
    }
    if (keys & PAD_BUTTON_B) {
        return PhotoAlbumApp_HandleGridSelection(app, PHOTO_ALBUM_INPUT_CLOSE);
    }
    if (keys & PAD_KEY_LEFT) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = PHOTO_ALBUM_INPUT_CLOSE;
        } else {
            hit += app->cursorY * 4;
        }
        if (hit == PHOTO_ALBUM_INPUT_CLOSE) {
            return PHOTO_ALBUM_STATE_GRID;
        }
        if (app->cursorX != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorX--;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        } else if (app->curPage != 0) {
            return PhotoAlbumApp_HandleGridSelection(app, PHOTO_ALBUM_INPUT_PREV_PAGE);
        }
    } else if (keys & PAD_KEY_RIGHT) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = PHOTO_ALBUM_INPUT_CLOSE;
        } else {
            hit += app->cursorY * 4;
        }
        if (hit == PHOTO_ALBUM_INPUT_CLOSE) {
            return PHOTO_ALBUM_STATE_GRID;
        }
        if (app->cursorX < 3) {
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorX++;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        } else if (app->curPage < app->numPages - 1) {
            return PhotoAlbumApp_HandleGridSelection(app, PHOTO_ALBUM_INPUT_NEXT_PAGE);
        }
    } else if (keys & PAD_KEY_UP) {
        if (app->cursorY == 4 && app->cursorX < 3) {
            app->cursorX = 3;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        app->cursorY = (app->cursorY + 3) % 4;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    } else if (keys & PAD_KEY_DOWN) {
        PlaySE(SEQ_SE_DP_SELECT);
        app->cursorY = (app->cursorY + 1) % 4;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    }
    return PHOTO_ALBUM_STATE_GRID;
}

static int PhotoAlbumApp_HandleMoveInput(PhotoAlbumApp *app) {
    int done = 0;
    PhotoAlbumApp_HandleMoveTouch(app, &done);
    if (!done) {
        PhotoAlbumApp_HandleMoveKeys(app);
    }
}

static int PhotoAlbumApp_RunMoveDone(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        if (app->timer++ < 15) {
            return 0;
        }
        PhotoAlbumApp_DrawPage(app, 3, 3, app->curPage);
        PhotoAlbumApp_PrintMessage(app, 3, FALSE, app->textDelay);
        app->timer = 0;
        app->subState++;
        break;
    case 1:
        if (app->timer++ < 30) {
            return 0;
        }
        if (TextPrinterCheckActive(app->printerId)) {
            return 0;
        }
        app->timer = 0;
        app->subState++;
        break;
    default:
        app->mode = PHOTO_ALBUM_MODE_NORMAL;
        PlaySE(SEQ_SE_DP_SELECT);
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        PhotoAlbumApp_PrintMessage(app, 0, FALSE, 0);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
        app->timer = 0;
        app->subState = 0;
        return 1;
    }
    return 0;
}

static int PhotoAlbumApp_HandleMoveSelection(PhotoAlbumApp *app, u32 input) {
    u8 slot;
    switch (input) {
    case PHOTO_ALBUM_INPUT_CLOSE:
        PlaySE(SEQ_SE_DP_SELECT);
        PhotoAlbumApp_PrintPhotoDetails(app, NULL);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_StartCloseButtonAnim(app);
        PhotoAlbumApp_PrintMessage(app, 0, FALSE, 0);
        app->mode = PHOTO_ALBUM_MODE_NORMAL;
        return PHOTO_ALBUM_STATE_GRID;
    case PHOTO_ALBUM_INPUT_PREV_PAGE:
        if (app->curPage == 0) {
            return PHOTO_ALBUM_STATE_MOVE_PHOTO;
        }
        PhotoAlbumApp_AnimatePageArrow(app, 2);
        PlaySE(SEQ_SE_GS_ALBUMMEKURU);
        return PHOTO_ALBUM_STATE_FLIP_TO_PREV_PAGE;
    case PHOTO_ALBUM_INPUT_NEXT_PAGE:
        if (app->curPage == app->numPages - 1) {
            return PHOTO_ALBUM_STATE_MOVE_PHOTO;
        }
        PhotoAlbumApp_AnimatePageArrow(app, 3);
        PlaySE(SEQ_SE_GS_ALBUMMEKURU);
        return PHOTO_ALBUM_STATE_FLIP_TO_NEXT_PAGE;
    default:
        slot = sPhotoAlbumGridSlots[input] + app->curPage * PHOTOS_PER_PAGE;
        if (slot >= app->numPhotos) {
            return PHOTO_ALBUM_STATE_MOVE_PHOTO;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_PrintPhotoDetails(app, NULL);
        if (slot != app->slotIdx) {
            PhotoAlbumApp_SwapEntries(app, app->slotIdx, slot);
            PhotoAlbumApp_RedrawSwappedIcons(app, app->slotIdx, slot);
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 0);
            app->timer = PHOTO_ALBUM_STATE_FADE_IN;
            return PHOTO_ALBUM_STATE_MOVE_PHOTO_DONE;
        }
        app->mode = PHOTO_ALBUM_MODE_NORMAL;
        PhotoAlbumApp_PrintMessage(app, 0, FALSE, 0);
        return PHOTO_ALBUM_STATE_GRID;
    }
}

static int PhotoAlbumApp_HandleMoveTouch(PhotoAlbumApp *app, int *a) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(sPhotoAlbumGridHitboxes);
    if (hit == -1) {
        return PHOTO_ALBUM_STATE_MOVE_PHOTO;
    }
    if (app->nextMenuState == 0) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }
    *a = 1;
    if ((u32)hit < PHOTOS_PER_PAGE) {
        app->cursorX = hit & 3;
        app->cursorY = (u32)hit >> 2;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        PlaySE(SEQ_SE_DP_SELECT);
    }
    PhotoAlbumApp_HandleMoveSelection(app, (u8)hit);
}

static int PhotoAlbumApp_HandleMoveKeys(PhotoAlbumApp *app) {
    u8 hit;

    if (app->nextMenuState == 1 && (gSystem.newKeys & 0xF3)) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }

    if (gSystem.newKeys & PAD_BUTTON_A) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = PHOTO_ALBUM_INPUT_CLOSE;
        } else {
            hit += app->cursorY * 4;
        }
        return PhotoAlbumApp_HandleMoveSelection(app, hit);
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        return PhotoAlbumApp_HandleMoveSelection(app, PHOTO_ALBUM_INPUT_CLOSE);
    }
    if (gSystem.newKeys & PAD_KEY_LEFT) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = PHOTO_ALBUM_INPUT_CLOSE;
        } else {
            hit += app->cursorY * 4;
        }
        if (hit == PHOTO_ALBUM_INPUT_CLOSE) {
            return PHOTO_ALBUM_STATE_GRID;
        }
        if (app->cursorX != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorX--;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        } else if (app->curPage != 0) {
            app->cursorX = (app->cursorX + 3) % 4;
            return PhotoAlbumApp_HandleMoveSelection(app, PHOTO_ALBUM_INPUT_PREV_PAGE);
        }
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = PHOTO_ALBUM_INPUT_CLOSE;
        } else {
            hit += app->cursorY * 4;
        }
        if (hit == PHOTO_ALBUM_INPUT_CLOSE) {
            return PHOTO_ALBUM_STATE_GRID;
        }
        if (app->cursorX < 3) {
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorX++;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        } else if (app->curPage < app->numPages - 1) {
            app->cursorX = (app->cursorX + 1) % 4;
            return PhotoAlbumApp_HandleMoveSelection(app, PHOTO_ALBUM_INPUT_NEXT_PAGE);
        }
    } else if (gSystem.newKeys & PAD_KEY_UP) {
        if (app->cursorY == 4 && app->cursorX < 3) {
            app->cursorX = 3;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        app->cursorY = (app->cursorY + 3) % 4;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        PlaySE(SEQ_SE_DP_SELECT);
        app->cursorY = (app->cursorY + 1) % 4;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    }
    return PHOTO_ALBUM_STATE_MOVE_PHOTO;
}

static int PhotoAlbumApp_FlipToPrevPage(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 0);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_RequestBgToggle(app, 1);
        PhotoAlbumApp_SetZoom(app, FX32_ONE);
        PhotoAlbumApp_DrawPageArrows(app, 1, 1);
        PhotoAlbumApp_DrawPage(app, 2, 1, app->curPage);
        PhotoAlbumApp_DrawPage(app, 3, 1, app->curPage - 1);
        app->pageFlipPhase = 0;
        app->subState++;
        break;
    case 1:
        app->pageFlipPhase++;
        PhotoAlbumApp_SetZoom(app, sPhotoAlbumPageFlipZoom[app->pageFlipPhase]);
        if (app->pageFlipPhase >= 4) {
            PhotoAlbumApp_DrawPageArrows(app, 2, 1);
            PhotoAlbumApp_DrawPage(app, 2, 2, app->curPage - 1);
            app->subState++;
        }
        break;
    case 2:
        app->pageFlipPhase = app->pageFlipPhase - 1;
        PhotoAlbumApp_SetZoom(app, sPhotoAlbumPageFlipZoom[app->pageFlipPhase]);
        if (app->pageFlipPhase == 0) {
            PhotoAlbumApp_DrawPage(app, 3, 2, app->curPage - 1);
            PhotoAlbumApp_RequestBgToggle(app, 0);
            PhotoAlbumApp_SetZoom(app, FX32_ONE);
            app->curPage--;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
            PhotoAlbumApp_UpdatePageArrows(app);
            if (app->mode == PHOTO_ALBUM_MODE_MOVE) {
                PhotoAlbumApp_UpdateSelectionSprite(app, 1);
            }
            app->subState = 0;
            return 1;
        }
        break;
    }
    return 0;
}

static int PhotoAlbumApp_FlipToNextPage(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 0);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_RequestBgToggle(app, 1);
        PhotoAlbumApp_SetZoom(app, FX32_ONE);
        PhotoAlbumApp_DrawPageArrows(app, 2, 1);
        PhotoAlbumApp_DrawPage(app, 2, 2, app->curPage);
        PhotoAlbumApp_DrawPage(app, 3, 2, app->curPage + 1);
        app->pageFlipPhase = 0;
        app->subState++;
        break;
    case 1:
        app->pageFlipPhase++;
        PhotoAlbumApp_SetZoom(app, sPhotoAlbumPageFlipZoom[app->pageFlipPhase]);
        if (app->pageFlipPhase >= 4) {
            PhotoAlbumApp_DrawPageArrows(app, 1, 1);
            PhotoAlbumApp_DrawPage(app, 2, 1, app->curPage + 1);
            app->subState++;
        }
        break;
    case 2:
        app->pageFlipPhase = app->pageFlipPhase - 1;
        PhotoAlbumApp_SetZoom(app, sPhotoAlbumPageFlipZoom[app->pageFlipPhase]);
        if (app->pageFlipPhase == 0) {
            PhotoAlbumApp_DrawPage(app, 3, 1, app->curPage + 1);
            PhotoAlbumApp_RequestBgToggle(app, 0);
            PhotoAlbumApp_SetZoom(app, FX32_ONE);
            app->curPage++;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
            PhotoAlbumApp_UpdatePageArrows(app);
            if (app->mode == PHOTO_ALBUM_MODE_MOVE) {
                PhotoAlbumApp_UpdateSelectionSprite(app, 1);
            }
            app->subState = 0;
            return 1;
        }
        break;
    }
    return 0;
}

static int PhotoAlbumApp_RunContextMenu(PhotoAlbumApp *app) {
    int r = PhotoAlbumApp_HandleContextMenu(app);
    if (r < 0) {
        r = PHOTO_ALBUM_STATE_CONTEXT_MENU;
    }
    return r;
}

static int PhotoAlbumApp_RunDeletePrompt(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        PhotoAlbumApp_OpenDeletePrompt(app);
        ++app->subState;
        break;
    case 1:
        if (PhotoAlbumApp_HandleDeletePrompt(app) != PHOTO_ALBUM_STATE_DELETE_PROMPT) {
            app->subState = 0;
            return PHOTO_ALBUM_STATE_GRID;
        }
        break;
    }
    return PHOTO_ALBUM_STATE_DELETE_PROMPT;
}

static void PhotoAlbumApp_VBlankCB(void *data) {
    PhotoAlbumApp *app = data;

    if (app->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    PhotoAlbumApp_ApplyBgToggle(app);
    PhotoAlbumApp_ApplyZoom(app);
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(app->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void PhotoAlbumApp_ApplyBgToggle(PhotoAlbumApp *app) {
    if (app->bgToggleReq) {
        ToggleBgLayer(GF_BG_LYR_MAIN_2, app->bgToggleVal);
    }
    app->bgToggleReq = FALSE;
}

static void PhotoAlbumApp_RequestBgToggle(PhotoAlbumApp *app, int on) {
    app->bgToggleVal = on;
    app->bgToggleReq = TRUE;
}

static void PhotoAlbumApp_ApplyZoom(PhotoAlbumApp *app) {
    MtxFx22 mtx;
    if (app->zoomChanged) {
        mtx._01 = 0;
        mtx._10 = 0;
        mtx._11 = FX_Inv(FX32_ONE);
        mtx._00 = FX_Inv(app->zoom);
        SetBgAffine(app->bgConfig, GF_BG_LYR_MAIN_2, &mtx, 0x80, 0x78);
        app->zoomChanged = FALSE;
    }
}

static void PhotoAlbumApp_SetZoom(PhotoAlbumApp *app, fx32 v) {
    app->zoom = v;
    app->zoomChanged = TRUE;
}

static void PhotoAlbumApp_SetBanks(void) {
    GraphicsBanks banks = sPhotoAlbumAppGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void PhotoAlbumApp_InitBgs(PhotoAlbumApp *app) {
    GraphicsModes modes;
    BgTemplate template4;
    BgTemplate template7;
    BgTemplate template6;
    BgTemplate template0;
    BgTemplate template1;
    BgTemplate template2;
    BgTemplate template3;
    PhotoAlbumApp_SetBanks();
    app->bgConfig = BgConfig_Alloc(app->heapId);
    modes = sPhotoAlbumAppGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    // POWCNT1: main engine on the bottom screen
    *(volatile u16 *)0x04000304 &= ~0x8000;
    template4 = sPhotoAlbumBgTemplate4;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_0, &template4, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    template7 = sPhotoAlbumBgTemplate7;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_3, &template7, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_3);
    template6 = sPhotoAlbumBgTemplate6;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_2, &template6, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_2);
    template0 = sPhotoAlbumBgTemplate0;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_0, &template0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0);
    template1 = sPhotoAlbumBgTemplate1;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_1, &template1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    template2 = sPhotoAlbumBgTemplate2;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_2, &template2, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    template3 = sPhotoAlbumBgTemplate3;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_3, &template3, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, app->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 32, 0, app->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, app->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 64, 0, app->heapId);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_X, -4);
}

static void PhotoAlbumApp_FreeBgs(PhotoAlbumApp *app) {
    vu16 *reg = (vu16 *)0x04000304;
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_3, (enum BgPosAdjustOp)0, 0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_0);
    Heap_Free(app->bgConfig);
    *reg |= (u32)reg >> 11;
}

static void PhotoAlbumApp_LoadBgGfx(PhotoAlbumApp *app) {
    NARC *narc = NARC_New(NARC_a_1_7_1, (enum HeapID)app->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, (enum GFPalLoadLocation)0, (enum GFPalSlotOffset)0, 0, (enum HeapID)app->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, (enum GFPalLoadLocation)4, (enum GFPalSlotOffset)0, 0, (enum HeapID)app->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 9, app->bgConfig, (GFBgLayer)6, 0, 0, FALSE, (enum HeapID)app->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->bgConfig, (GFBgLayer)7, 0, 0, FALSE, (enum HeapID)app->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, app->bgConfig, (GFBgLayer)6, 0, 0, FALSE, (enum HeapID)app->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->bgConfig, (GFBgLayer)3, 0, 0, FALSE, (enum HeapID)app->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, app->bgConfig, (GFBgLayer)3, 0, 0, FALSE, (enum HeapID)app->heapId);
    app->screenFile_B0 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, FALSE, &app->screen_AC, (enum HeapID)app->heapId);
    app->photoIconsFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, FALSE, &app->photoIcons, (enum HeapID)app->heapId);
    app->pageArrowsFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 7, FALSE, &app->pageArrows, (enum HeapID)app->heapId);
    NARC_Delete(narc);
    LoadFontPal1((enum GFPalLoadLocation)0, (enum GFPalSlotOffset)0x180, (enum HeapID)app->heapId);
    LoadUserFrameGfx2(app->bgConfig, (GFBgLayer)0, 1, 0xD, app->frame, (enum HeapID)app->heapId);
}

static void PhotoAlbumApp_FreeScreenData(PhotoAlbumApp *app) {
    Heap_Free(app->pageArrowsFile);
    Heap_Free(app->photoIconsFile);
    Heap_Free(app->screenFile_B0);
}

static void PhotoAlbumApp_LoadStrings(PhotoAlbumApp *app) {
    int i;
    FontID_Alloc(4, app->heapId);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0000_bin, app->heapId);
    app->messageFormat = MessageFormat_New_Custom(6, 0x16, app->heapId);
    app->str_2C = String_New(0x80, app->heapId);
    app->str_30 = NewString_ReadMsgData(app->msgData, msg_0000_00000);
    for (i = 0; i < 5; i++) {
        app->strs_34[i] = NewString_ReadMsgData(app->msgData, i + 5);
    }
    for (i = 0; i < 2; i++) {
        app->strs_48[i] = NewString_ReadMsgData(app->msgData, i + 10);
    }
    app->listMenuItems = ListMenuItems_New(4, app->heapId);
    for (i = 0; i < 4; i++) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, i + 1, i);
    }
}

static void PhotoAlbumApp_FreeStrings(PhotoAlbumApp *app) {
    int i;
    ListMenuItems_Delete(app->listMenuItems);
    app->listMenuItems = NULL;
    for (i = 0; i < 2; ++i) {
        String_Delete(app->strs_48[i]);
    }
    for (i = 0; i < 5; ++i) {
        String_Delete(app->strs_34[i]);
    }
    String_Delete(app->str_30);
    String_Delete(app->str_2C);
    MessageFormat_Delete(app->messageFormat);
    DestroyMsgData(app->msgData);
    FontID_Release(4);
}

static void PhotoAlbumApp_CreateWindows(PhotoAlbumApp *app) {
    int i;
    for (i = 0; i < 3; ++i) {
        AddWindow(app->bgConfig, &app->windows[i], &sPhotoAlbumWindowTemplates[i]);
        FillWindowPixelBuffer(&app->windows[i], 0);
    }
    app->yesNoPrompt = YesNoPrompt_Create(app->heapId);
}

static void PhotoAlbumApp_RemoveWindows(PhotoAlbumApp *app) {
    int i;
    for (i = 0; i < 3; ++i) {
        ClearWindowTilemapAndCopyToVram(&app->windows[i]);
        RemoveWindow(&app->windows[i]);
    }
    YesNoPrompt_Destroy(app->yesNoPrompt);
}

static void PhotoAlbumApp_InitSpriteSystem(PhotoAlbumApp *app) {
    GF_CreateVramTransferManager(0x20, app->heapId);
    app->spriteSystem = SpriteSystem_Alloc(app->heapId);
    SpriteSystem_Init(app->spriteSystem, &sPhotoAlbumAppOamManagerParam, &sPhotoAlbumAppOamCharTransferParam, 3);
    thunk_ClearMainOAM(app->heapId);
    thunk_ClearSubOAM(app->heapId);
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 5);
    SpriteSystem_LoadResourceDataFromFileIdListEx(app->spriteSystem, app->spriteManager, &sPhotoAlbumSpriteResources, 2, 1);
}

static void PhotoAlbumApp_FreeSpriteSystem(PhotoAlbumApp *app) {
    SpriteSystem_DestroySpriteManager(app->spriteSystem, app->spriteManager);
    app->spriteManager = NULL;
    SpriteSystem_Free(app->spriteSystem);
    app->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(app->heapId);
}

static void PhotoAlbumApp_InitSpritesAndMenus(PhotoAlbumApp *app) {
    PhotoAlbumApp_InitSpriteSystem(app);
    PhotoAlbumApp_CreateSprites(app);
    app->listMenuSpawner = TouchscreenListMenuSpawner_Create(app->heapId, NULL);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void PhotoAlbumApp_FreeSpritesAndMenus(PhotoAlbumApp *app) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    PhotoAlbumApp_DeleteSprites(app);
    TouchscreenListMenuSpawner_Destroy(app->listMenuSpawner);
    PhotoAlbumApp_FreeSpriteSystem(app);
}

static void PhotoAlbumApp_CreateSprites(PhotoAlbumApp *app) {
    const UnmanagedSpriteTemplate *template = sPhotoAlbumSpriteTemplates;
    int i;
    for (i = 0; i < 5; i++) {
        app->sprites[i] = SpriteSystem_CreateSpriteFromResourceHeader(app->spriteSystem, app->spriteManager, template);
        Sprite_SetDrawFlag(app->sprites[i], TRUE);
        Sprite_SetAnimSpeed(app->sprites[i], FX32_ONE);
        template ++;
    }
    Sprite_SetAnimActiveFlag(app->sprites[0], FALSE);
    Sprite_SetDrawFlag(app->sprites[1], FALSE);
    Sprite_SetPriority(app->sprites[4], 2);
    Sprite_SetAnimActiveFlag(app->sprites[4], TRUE);
}

static void PhotoAlbumApp_DeleteSprites(PhotoAlbumApp *app) {
    int i;
    for (i = 0; i < 5; ++i) {
        thunk_Sprite_Delete(app->sprites[i]);
    }
}

static void PhotoAlbumApp_DrawPageArrows(PhotoAlbumApp *app, int position, int draw) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    if (draw) {
        CopyToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, (u8)(16 * (position / 2)), 0, 16, 24, app->pageArrows->rawData, (u8)(16 * (position / 2)), 0, (u8)(app->pageArrows->screenWidth >> 3), (u8)(app->pageArrows->screenHeight >> 3));
    }
}

static void PhotoAlbumApp_DrawPageArrowBg(PhotoAlbumApp *app, int bgId, int mode, int index) {
    NNSG2dScreenData *screen;
    u8 x;
    int row;
    if (mode == 1) {
        x = 3;
        row = (u8)(index * 2 + 1);
    } else {
        x = 28;
        row = (u8)(index * 2 + 2);
    }
    screen = app->pageArrows;
    CopyToBgTilemapRect(app->bgConfig, (u8)bgId, x, 15, 1, 2, screen->rawData, (u8)row, 24, screen->screenWidth >> 3, screen->screenHeight >> 3);
}

static void PhotoAlbumApp_DrawGridColumns(PhotoAlbumApp *app, int bgId, int page, u8 start) {
    int row;
    int x;
    u8 n;
    for (row = 0; row < 3; row++) {
        for (x = start; x < start + 2; x++) {
            n = sPhotoAlbumGridSlots[row * 4 + x] + page * PHOTOS_PER_PAGE;
            if (n < app->numPhotos) {
                PhotoAlbumApp_DrawGridIcon(app, app->entries[n].photo, bgId, x, row);
            } else {
                PhotoAlbumApp_DrawGridIcon(app, NULL, bgId, x, row);
            }
        }
    }
}

static void PhotoAlbumApp_DrawPage(PhotoAlbumApp *app, int bgId, int mask, u8 page) {
    if (mask & 1) {
        PhotoAlbumApp_DrawGridColumns(app, bgId, page, 0);
        PhotoAlbumApp_DrawPageArrowBg(app, bgId, 1, page);
    }
    if (mask & 2) {
        PhotoAlbumApp_DrawGridColumns(app, bgId, page, 2);
        PhotoAlbumApp_DrawPageArrowBg(app, bgId, 2, page);
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, (u8)bgId);
}

static void PhotoAlbumApp_DrawPhotoIcon(PhotoAlbumApp *app, int index, u8 bgId, u8 destX, u8 destY) {
    NNSG2dScreenData *tiles;
    int rem;
    int quot;

    tiles = app->photoIcons;
    rem = index % 6;
    quot = index / 6;
    CopyToBgTilemapRect(app->bgConfig, bgId, destX, destY, 5, 5, tiles->rawData, (u8)(rem * 5), (u8)(quot * 5), (u8)(tiles->screenWidth >> 3), (u8)(tiles->screenHeight >> 3));
}

static void PhotoAlbumApp_DrawGridIcon(PhotoAlbumApp *app, Photo *photo, int bgId, u8 x, u8 y) {
    u8 icon = 0;
    if (photo) {
        icon = photo->iconId + 1;
    }
    PhotoAlbumApp_DrawPhotoIcon(app, icon, (u8)bgId, (u8)((x * 6 + 4) + (u8)(x >> 1)), (u8)(y * 5 + 1));
}

static void PhotoAlbumApp_RedrawSwappedIcons(PhotoAlbumApp *app, int oldIndex, int newIndex) {
    int page;
    u8 packed;

    page = oldIndex / 12;
    if (page == app->curPage) {
        packed = sPhotoAlbumSlotGridPositions[oldIndex % 12];
        PhotoAlbumApp_DrawGridIcon(app, NULL, 3, packed % 4, packed >> 2);
    }
    page = newIndex / 12;
    if (page == app->curPage) {
        packed = sPhotoAlbumSlotGridPositions[newIndex % 12];
        PhotoAlbumApp_DrawGridIcon(app, NULL, 3, packed % 4, packed >> 2);
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
}

// Prints the details of `photo` (where, which Pokemon, when) on the top screen; NULL clears them.
static void PhotoAlbumApp_PrintPhotoDetails(PhotoAlbumApp *app, Photo *photo) {
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_2);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_3);
    FillWindowPixelBuffer(&app->windows[2], 0);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_3);
    if (!photo) {
        ScheduleWindowCopyToVram(&app->windows[2]);
        return;
    }
    BufferPlayersName(app->messageFormat, 0, Save_PlayerData_GetProfile(app->args->saveData));
    MapID_GetLandmarkName(photo->mapId, app->heapId, app->str_2C);
    BufferString(app->messageFormat, 1, app->str_2C, 2, 0, 2);
    CopyU16ArrayToString(app->str_2C, photo->leadMonNick);
    BufferString(app->messageFormat, 2, app->str_2C, 2, 0, 2);
    BufferIntegerAsString(app->messageFormat, 3, 2000 + (u8)(photo->date >> 24), 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(app->messageFormat, 4, (u8)(photo->date >> 16), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(app->messageFormat, 5, (u8)(photo->date >> 8), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    if (Photo_CountPartyMons(photo) > 1) {
        StringExpandPlaceholders(app->messageFormat, app->str_2C, app->strs_48[1]);
    } else {
        StringExpandPlaceholders(app->messageFormat, app->str_2C, app->strs_48[0]);
    }
    AddTextPrinterParameterizedWithColor(&app->windows[2], 0, app->str_2C, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 2, 0), NULL);
    ScheduleWindowCopyToVram(&app->windows[2]);
    PhotoAlbumApp_DrawPhotoIcon(app, (u8)(photo->iconId + 1), GF_BG_LYR_SUB_3, 13, 8);
}

static void PhotoAlbumApp_PrintCloseButton(PhotoAlbumApp *app, int a) {
    FillWindowPixelBuffer(&app->windows[0], 0);
    if (a) {
        Sprite_SetAnimCtrlSeq(app->sprites[4], 9);
        Sprite_ResetAnimCtrlState(app->sprites[4]);
    }
    AddTextPrinterParameterizedWithColor(&app->windows[0], 4, app->str_30, (0x40 - FontID_String_GetWidth(4, app->str_30, 0)) >> 1, 0, 0xFF, 0x00010500, NULL);
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
}

static void PhotoAlbumApp_PrintMessage(PhotoAlbumApp *app, int msgIdx, BOOL drawFrame, u8 textDelay) {
    if (drawFrame) {
        DrawFrameAndWindow2(&app->windows[1], TRUE, 1, 13);
    }
    FillWindowPixelBuffer(&app->windows[1], 15);
    if (textDelay == 0) {
        AddTextPrinterParameterizedWithColor(&app->windows[1], 0, app->strs_34[msgIdx], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    } else {
        app->printerId = AddTextPrinterParameterizedWithColor(&app->windows[1], 0, app->strs_34[msgIdx], 0, 0, textDelay, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    }
    ScheduleWindowCopyToVram(&app->windows[1]);
}

static void PhotoAlbumApp_SetCursorPos(PhotoAlbumApp *app, u8 x, u8 y, int draw) {
    s16 px;
    s16 py;
    Sprite_SetDrawFlag(app->sprites[0], draw);
    if (y == 3) {
        px = 0xC0;
        py = 0xA0;
        Sprite_SetAnimationFrame(app->sprites[0], 1);
    } else {
        int xoff = x * 0x30 + 0x1C;
        px = (s16)(xoff + (((u32)x >> 1) << 3));
        py = (s16)(0x28 * y + 8);
        Sprite_SetAnimationFrame(app->sprites[0], 0);
    }
    Sprite_SetPositionXY(app->sprites[0], px, py);
}

static void PhotoAlbumApp_UpdateSelectionSprite(PhotoAlbumApp *app, int draw) {
    int xoff;
    if (app->curPage != (u8)(app->slotIdx / 12)) {
        draw = 0;
    }
    xoff = app->spriteCol * 0x30 + 0x1C;
    Sprite_SetPositionXY(app->sprites[1],
        (s16)(xoff + (((u32)app->spriteCol >> 1) << 3)),
        (s16)(0x28 * app->spriteRow + 8));
    Sprite_SetDrawFlag(app->sprites[1], draw);
}

static void PhotoAlbumApp_UpdatePageArrows(PhotoAlbumApp *app) {
    Sprite_SetAnimActiveFlag(app->sprites[2], FALSE);
    Sprite_SetAnimActiveFlag(app->sprites[3], FALSE);
    if (app->curPage == 0) {
        Sprite_SetAnimCtrlSeq(app->sprites[2], 2);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[2], 0);
    }
    if (app->numPages == 0 || app->curPage == app->numPages - 1) {
        Sprite_SetAnimCtrlSeq(app->sprites[3], 5);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[3], 3);
    }
}

static void PhotoAlbumApp_AnimatePageArrow(PhotoAlbumApp *app, int idx) {
    Sprite_ResetAnimCtrlState(app->sprites[idx]);
    Sprite_SetAnimActiveFlag(app->sprites[idx], TRUE);
    if (idx == 2) {
        Sprite_SetAnimCtrlSeq(app->sprites[idx], 1);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[idx], 4);
    }
}

static int PhotoAlbumApp_ToggleMenuInputState(PhotoAlbumApp *app) {
    if (app->nextMenuState == 0) {
        app->nextMenuState = 1;
    } else {
        app->nextMenuState = 0;
    }
    return 0;
}

static void PhotoAlbumApp_OpenContextMenu(PhotoAlbumApp *app) {
    TouchscreenListMenuHeader header;
    MI_CpuFill8(&header, 0, sizeof(header));
    header.template = sPhotoAlbumContextMenuTemplate;
    header.listMenuItems = app->listMenuItems;
    header.bgConfig = app->bgConfig;
    header.numWindows = 4;
    app->listMenu = TouchscreenListMenu_Create(app->listMenuSpawner, &header, (u8)app->nextMenuState, 0x18, 0xB, 6, 0);
    PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 0);
}

static int PhotoAlbumApp_HandleContextMenu(PhotoAlbumApp *app) {
    int result = TouchscreenListMenu_HandleInput(app->listMenu);
    u32 touch;
    if (result == -1) {
        return -1;
    }
    touch = TouchscreenListMenu_WasLastInputTouch(app->listMenu);
    TouchscreenListMenu_Destroy(app->listMenu);
    if (app->nextMenuState != touch) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }
    switch (result) {
    case PHOTO_MENU_SELECT:
        app->args->photoWasSelected = 1;
        app->args->cursorPos = app->slotIdx;
        return PHOTO_ALBUM_STATE_FADE_OUT;
    case PHOTO_MENU_DELETE:
        PhotoAlbumApp_PrintMessage(app, 4, FALSE, 0);
        app->mode = PHOTO_ALBUM_MODE_DELETE;
        return PHOTO_ALBUM_STATE_DELETE_PROMPT;
    case PHOTO_MENU_MOVE:
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        PhotoAlbumApp_PrintMessage(app, 2, FALSE, 0);
        app->mode = PHOTO_ALBUM_MODE_MOVE;
        return PHOTO_ALBUM_STATE_MOVE_PHOTO;
    default:
        PhotoAlbumApp_PrintPhotoDetails(app, NULL);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        return PHOTO_ALBUM_STATE_GRID;
    }
}

static void PhotoAlbumApp_OpenDeletePrompt(PhotoAlbumApp *app) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = app->bgConfig;
    template.bgId = 0;
    template.tileStart = 0x7A;
    template.plttSlot = 0xE;
    template.x = 0x19;
    template.y = 0x10;
    template.ignoreTouchFlag = (u8)app->nextMenuState;
    template.initialCursorPos = 1;
    YesNoPrompt_InitFromTemplate(app->yesNoPrompt, &template);
}

static int PhotoAlbumApp_HandleDeletePrompt(PhotoAlbumApp *app) {
    int result = YesNoPrompt_HandleInput(app->yesNoPrompt);
    switch (result) {
    case YESNORESPONSE_YES:
        PhotoAlbumApp_RemoveEntry(app, app->slotIdx);
        break;
    case YESNORESPONSE_NO:
        break;
    default:
        return PHOTO_ALBUM_STATE_DELETE_PROMPT;
    }
    {
        u32 touch = YesNoPrompt_IsInTouchMode(app->yesNoPrompt);
        if (touch != app->nextMenuState) {
            app->nextMenuState = touch;
        }
    }
    YesNoPrompt_Reset(app->yesNoPrompt);
    PhotoAlbumApp_UpdateSelectionSprite(app, 0);
    if (result == YESNORESPONSE_YES) {
        PhotoAlbumApp_DrawPage(app, 3, 3, app->curPage);
        PhotoAlbumApp_UpdatePageArrows(app);
    }
    PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    PhotoAlbumApp_PrintPhotoDetails(app, NULL);
    PhotoAlbumApp_PrintMessage(app, 0, FALSE, 0);
    app->mode = PHOTO_ALBUM_MODE_NORMAL;
    return PHOTO_ALBUM_STATE_GRID;
}

static void PhotoAlbumApp_StartCloseButtonAnim(PhotoAlbumApp *app) {
    PhotoAlbumTask *task = Heap_AllocAtEnd(app->heapId, sizeof(PhotoAlbumTask));
    MI_CpuFill8(task, 0, sizeof(PhotoAlbumTask));
    task->app = app;
    SysTask_CreateOnMainQueue(Task_PhotoAlbumApp_WaitCloseButtonAnim, task, 0);
    PhotoAlbumApp_PrintCloseButton(app, 1);
    app->fadeRunning = TRUE;
}

static void Task_PhotoAlbumApp_WaitCloseButtonAnim(SysTask *task, void *taskData) {
    PhotoAlbumTask *data = taskData;

    if (!Sprite_IsAnimated(data->app->sprites[4])) {
        PhotoAlbumApp_PrintCloseButton(data->app, 0);
        data->app->fadeRunning = FALSE;
        MI_CpuFill8(data, 0, sizeof(PhotoAlbumTask));
        Heap_Free(data);
        SysTask_Destroy(task);
    }
}

static u8 Photo_CountPartyMons(Photo *photo) {
    u8 count = 0;
    u8 i;
    for (i = 0; i < PARTY_SIZE; ++i) {
        int species = photo->party[i].species;
        if (species > 0 && species <= NATIONAL_DEX_COUNT) {
            ++count;
        }
    }
    return count;
}
