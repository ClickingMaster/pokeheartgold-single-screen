#include "photo_album_app.h"

#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "photo_album.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "sound_playback.h"
#include "oam_manager.h"
#include "touchpad.h"
#include "unk_02068F84.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

#define PHOTOS_PER_PAGE 12

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

extern const u8 _021E7884[];
extern const u8 ov109_021E7890[];
extern const TouchscreenListMenuTemplate ov109_021E789C;
extern const ResdatIdList ov109_021E78A8;
extern const GraphicsModes ov109_021E78B8;
extern const OamCharTransferParam ov109_021E78C8;
extern const fx32 ov109_021E78DC[];
extern const WindowTemplate ov109_021E78F4[3];
extern const BgTemplate ov109_021E790C;
extern const BgTemplate ov109_021E7928;
extern const BgTemplate ov109_021E7944;
extern const BgTemplate ov109_021E7960;
extern const BgTemplate ov109_021E797C;
extern const BgTemplate ov109_021E7998;
extern const BgTemplate ov109_021E79B4;
extern const OamManagerParam ov109_021E79D0;
extern const GraphicsBanks ov109_021E79F0;
extern const TouchscreenHitbox ov109_021E7A18[];
extern u8 ov109_021E7A58[];

void PhotoAlbumApp_ResetDisplay(void);
void PhotoAlbumApp_InitState(PhotoAlbumApp *app);
void PhotoAlbumApp_Finish(PhotoAlbumApp *app);
BOOL PhotoAlbumApp_Setup(PhotoAlbumApp *app);
BOOL PhotoAlbumApp_Teardown(PhotoAlbumApp *app);
int PhotoAlbumApp_State_FadeIn(PhotoAlbumApp *app);
int PhotoAlbumApp_State_FadeOut(PhotoAlbumApp *app);
int PhotoAlbumApp_State_Grid(PhotoAlbumApp *app);
int PhotoAlbumApp_State_FlipToPrevPage(PhotoAlbumApp *app);
int PhotoAlbumApp_State_FlipToNextPage(PhotoAlbumApp *app);
int PhotoAlbumApp_State_ContextMenu(PhotoAlbumApp *app);
int PhotoAlbumApp_State_MovePhoto(PhotoAlbumApp *app);
int PhotoAlbumApp_State_MovePhotoDone(PhotoAlbumApp *app);
int PhotoAlbumApp_State_DeletePrompt(PhotoAlbumApp *app);
void PhotoAlbumApp_InitGraphics(PhotoAlbumApp *app);
void PhotoAlbumApp_FreeGraphics(PhotoAlbumApp *app);
void PhotoAlbumApp_InitEntries(PhotoAlbumApp *app);
void PhotoAlbumApp_WritePhotosToSave(PhotoAlbumApp *app);
void PhotoAlbumApp_SwapEntries(PhotoAlbumApp *app, int a, int b);
void PhotoAlbumApp_RemoveEntry(PhotoAlbumApp *app, u8 selected);
void PhotoAlbumApp_DrawInitialScreen(PhotoAlbumApp *app);
int PhotoAlbumApp_HandleGridInput(PhotoAlbumApp *app);
int PhotoAlbumApp_HandleGridSelection(PhotoAlbumApp *app, int input);
int PhotoAlbumApp_HandleGridTouch(PhotoAlbumApp *app, int *a);
int PhotoAlbumApp_HandleGridKeys(PhotoAlbumApp *app);
int PhotoAlbumApp_HandleMoveInput(PhotoAlbumApp *app);
int PhotoAlbumApp_RunMoveDone(PhotoAlbumApp *app);
int PhotoAlbumApp_HandleMoveSelection(PhotoAlbumApp *app, u32 input);
int PhotoAlbumApp_HandleMoveTouch(PhotoAlbumApp *app, int *a);
int PhotoAlbumApp_HandleMoveKeys(PhotoAlbumApp *app);
int PhotoAlbumApp_FlipToPrevPage(PhotoAlbumApp *app);
int PhotoAlbumApp_FlipToNextPage(PhotoAlbumApp *app);
int PhotoAlbumApp_RunContextMenu(PhotoAlbumApp *app);
int PhotoAlbumApp_RunDeletePrompt(PhotoAlbumApp *app);
void PhotoAlbumApp_VBlankCB(PhotoAlbumApp *app);
void PhotoAlbumApp_ApplyBgToggle(PhotoAlbumApp *app);
void PhotoAlbumApp_RequestBgToggle(PhotoAlbumApp *app, int on);
void PhotoAlbumApp_ApplyZoom(PhotoAlbumApp *app);
void PhotoAlbumApp_SetZoom(PhotoAlbumApp *app, fx32 v);
void PhotoAlbumApp_SetBanks(void);
void PhotoAlbumApp_InitBgs(PhotoAlbumApp *app);
void PhotoAlbumApp_FreeBgs(PhotoAlbumApp *app);
void PhotoAlbumApp_LoadBgGfx(PhotoAlbumApp *app);
void PhotoAlbumApp_FreeScreenData(PhotoAlbumApp *app);
void PhotoAlbumApp_LoadStrings(PhotoAlbumApp *app);
void PhotoAlbumApp_FreeStrings(PhotoAlbumApp *app);
void PhotoAlbumApp_CreateWindows(PhotoAlbumApp *app);
void PhotoAlbumApp_RemoveWindows(PhotoAlbumApp *app);
void PhotoAlbumApp_InitSpriteSystem(PhotoAlbumApp *app);
void PhotoAlbumApp_FreeSpriteSystem(PhotoAlbumApp *app);
void PhotoAlbumApp_InitSpritesAndMenus(PhotoAlbumApp *app);
void PhotoAlbumApp_FreeSpritesAndMenus(PhotoAlbumApp *app);
void PhotoAlbumApp_CreateSprites(PhotoAlbumApp *app);
void PhotoAlbumApp_DeleteSprites(PhotoAlbumApp *app);
void ov109_021E6F7C(PhotoAlbumApp *app, int position, int draw);
void ov109_021E6FD4(PhotoAlbumApp *app, int bgId, int mode, int index);
void PhotoAlbumApp_DrawGridColumns(PhotoAlbumApp *app, int bgId, int page, u8 start);
void PhotoAlbumApp_DrawPage(PhotoAlbumApp *app, int bgId, int mask, u8 page);
void PhotoAlbumApp_DrawPhotoIcon(PhotoAlbumApp *app, int index, u8 bgId, u8 destX, u8 destY);
void PhotoAlbumApp_DrawGridIcon(PhotoAlbumApp *app, Photo *photo, int bgId, u8 x, u8 y);
void PhotoAlbumApp_RedrawSwappedIcons(PhotoAlbumApp *app, int oldIndex, int newIndex);
void PhotoAlbumApp_PrintPhotoDetails(PhotoAlbumApp *app, Photo *photo);
void PhotoAlbumApp_PrintCloseButton(PhotoAlbumApp *app, int a);
void PhotoAlbumApp_PrintMessage(PhotoAlbumApp *app, int msgIdx, BOOL drawFrame, u8 textDelay);
void PhotoAlbumApp_SetCursorPos(PhotoAlbumApp *app, u8 x, u8 y, int draw);
void PhotoAlbumApp_UpdateSelectionSprite(PhotoAlbumApp *app, int draw);
void PhotoAlbumApp_UpdatePageArrows(PhotoAlbumApp *app);
void PhotoAlbumApp_AnimatePageArrow(PhotoAlbumApp *app, int idx);
int PhotoAlbumApp_ToggleMenuInputState(PhotoAlbumApp *app);
void PhotoAlbumApp_OpenContextMenu(PhotoAlbumApp *app);
int PhotoAlbumApp_HandleContextMenu(PhotoAlbumApp *app);
void PhotoAlbumApp_OpenDeletePrompt(PhotoAlbumApp *app);
int PhotoAlbumApp_HandleDeletePrompt(PhotoAlbumApp *app);
void PhotoAlbumApp_StartCloseButtonAnim(PhotoAlbumApp *app);
void Task_PhotoAlbumApp_WaitCloseButtonAnim(SysTask *task, PhotoAlbumTask *data);
u8 Photo_CountPartyMons(Photo *photo);

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
    case 0:
        *state = PhotoAlbumApp_State_FadeIn(app);
        break;
    case 1:
        *state = PhotoAlbumApp_State_Grid(app);
        break;
    case 2:
        *state = PhotoAlbumApp_State_FlipToPrevPage(app);
        break;
    case 3:
        *state = PhotoAlbumApp_State_FlipToNextPage(app);
        break;
    case 4:
        *state = PhotoAlbumApp_State_ContextMenu(app);
        break;
    case 5:
        *state = PhotoAlbumApp_State_MovePhoto(app);
        break;
    case 6:
        *state = PhotoAlbumApp_State_MovePhotoDone(app);
        break;
    case 7:
        *state = PhotoAlbumApp_State_DeletePrompt(app);
        break;
    case 8:
        *state = PhotoAlbumApp_State_FadeOut(app);
        break;
    case 9:
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

void PhotoAlbumApp_ResetDisplay(void) {
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

#pragma force_active on
void PhotoAlbumApp_InitState(PhotoAlbumApp *app) {
    Options *options;
    int cursorPos;
    int gridPos;
    app->nextMenuState = MenuInputStateMgr_GetState(app->args->menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(app->args->saveData);
    app->textDelay = Options_GetTextFrameDelay(options);
    app->frame = Options_GetFrame(options);
    cursorPos = app->args->cursorPos;
    gridPos = ov109_021E7890[cursorPos % PHOTOS_PER_PAGE];
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
#pragma force_active reset

void PhotoAlbumApp_Finish(PhotoAlbumApp *app) {
    PhotoAlbumApp_WritePhotosToSave(app);
    Heap_Free(app->photos);
    MenuInputStateMgr_SetState(app->args->menuInputStatePtr, (MenuInputState)app->nextMenuState);
}

BOOL PhotoAlbumApp_Setup(PhotoAlbumApp *app) {
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

BOOL PhotoAlbumApp_Teardown(PhotoAlbumApp *app) {
    GF_TouchpadStopSampling();
    PhotoAlbumApp_FreeGraphics(app);
    return TRUE;
}

int PhotoAlbumApp_State_FadeIn(PhotoAlbumApp *app) {
    switch (app->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, app->heapId);
        ++app->state;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            app->state = 0;
            return 1;
        }
        break;
    }
    return 0;
}

int PhotoAlbumApp_State_FadeOut(PhotoAlbumApp *app) {
    switch (app->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, app->heapId);
        ++app->state;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            app->state = 0;
            return 9;
        }
        break;
    }
    return 8;
}

int PhotoAlbumApp_State_Grid(PhotoAlbumApp *app) {
    return PhotoAlbumApp_HandleGridInput(app);
}

int PhotoAlbumApp_State_FlipToPrevPage(PhotoAlbumApp *app) {
    if (!PhotoAlbumApp_FlipToPrevPage(app)) {
        return 2;
    }
    if (app->mode == 1) {
        return 5;
    }
    return 1;
}

int PhotoAlbumApp_State_FlipToNextPage(PhotoAlbumApp *app) {
    if (!PhotoAlbumApp_FlipToNextPage(app)) {
        return 3;
    }
    if (app->mode == 1) {
        return 5;
    }
    return 1;
}

int PhotoAlbumApp_State_ContextMenu(PhotoAlbumApp *app) {
    return PhotoAlbumApp_RunContextMenu(app);
}

int PhotoAlbumApp_State_MovePhoto(PhotoAlbumApp *app) {
    return PhotoAlbumApp_HandleMoveInput(app);
}

int PhotoAlbumApp_State_MovePhotoDone(PhotoAlbumApp *app) {
    if (PhotoAlbumApp_RunMoveDone(app)) {
        return 1;
    }
    return 6;
}

int PhotoAlbumApp_State_DeletePrompt(PhotoAlbumApp *app) {
    return PhotoAlbumApp_RunDeletePrompt(app);
}

void PhotoAlbumApp_InitGraphics(PhotoAlbumApp *app) {
    PhotoAlbumApp_InitBgs(app);
    PhotoAlbumApp_LoadBgGfx(app);
    PhotoAlbumApp_LoadStrings(app);
    PhotoAlbumApp_CreateWindows(app);
    PhotoAlbumApp_InitSpritesAndMenus(app);
    Main_SetVBlankIntrCB((void (*)(void *))PhotoAlbumApp_VBlankCB, app);
}

void PhotoAlbumApp_FreeGraphics(PhotoAlbumApp *app) {
    PhotoAlbumApp_FreeSpritesAndMenus(app);
    PhotoAlbumApp_RemoveWindows(app);
    PhotoAlbumApp_FreeStrings(app);
    PhotoAlbumApp_FreeScreenData(app);
    PhotoAlbumApp_FreeBgs(app);
}

#pragma force_active on
void PhotoAlbumApp_InitEntries(PhotoAlbumApp *app) {
    u8 i;
    app->numPhotos = app->numPhotosPrev;
    MI_CpuFill8(app->entries, 0, sizeof(app->entries));
    for (i = 0; i < app->numPhotos; ++i) {
        PhotoEntry *entry = &app->entries[i];
        entry->idx = i;
        entry->photo = &app->photos[i];
    }
}
#pragma force_active reset

void PhotoAlbumApp_WritePhotosToSave(PhotoAlbumApp *app) {
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

void PhotoAlbumApp_SwapEntries(PhotoAlbumApp *app, int a, int b) {
    PhotoEntry tmp;
    MI_CpuCopy8(&app->entries[a], &tmp, sizeof(PhotoEntry));
    MI_CpuCopy8(&app->entries[b], &app->entries[a], sizeof(PhotoEntry));
    MI_CpuCopy8(&tmp, &app->entries[b], sizeof(PhotoEntry));
}

#pragma force_active on
// Removes the photo in slot `selected` from the list and moves the cursor onto a remaining photo.
void PhotoAlbumApp_RemoveEntry(PhotoAlbumApp *app, u8 selected) {
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
        gridPos = ov109_021E7890[selected % PHOTOS_PER_PAGE];
        app->cursorX = gridPos % 4;
        app->cursorY = (u32)gridPos >> 2;
    }
}
#pragma force_active reset

void PhotoAlbumApp_DrawInitialScreen(PhotoAlbumApp *app) {
    PhotoAlbumApp_DrawPage(app, 3, 3, app->curPage);
    PhotoAlbumApp_UpdatePageArrows(app);
    PhotoAlbumApp_PrintCloseButton(app, 0);
    PhotoAlbumApp_PrintMessage(app, 0, 1, 0);
    PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
}

int PhotoAlbumApp_HandleGridInput(PhotoAlbumApp *app) {
    int done = 0;
    if (app->fadeRunning) {
        return 1;
    }
    PhotoAlbumApp_HandleGridTouch(app, &done);
    if (!done) {
        PhotoAlbumApp_HandleGridKeys(app);
    }
}

#pragma force_active on
// Handles a selection on the photo grid: 0-11 = photo, 12 = close, 13/14 = previous/next page.
int PhotoAlbumApp_HandleGridSelection(PhotoAlbumApp *app, int input) {
    u8 selected;
    switch (input) {
    case 12:
        app->args->photoWasSelected = FALSE;
        PlaySE(SEQ_SE_DP_SELECT);
        PhotoAlbumApp_PrintCloseButton(app, 1);
        return 8;
    case 13:
        if (app->curPage == 0) {
            return 1;
        }
        PhotoAlbumApp_AnimatePageArrow(app, 2);
        PlaySE(SEQ_SE_GS_ALBUMMEKURU);
        return 2;
    case 14:
        if (app->curPage == app->numPages - 1) {
            return 1;
        }
        PhotoAlbumApp_AnimatePageArrow(app, 3);
        PlaySE(SEQ_SE_GS_ALBUMMEKURU);
        return 3;
    }
    selected = (u8)(_021E7884[input] + app->curPage * PHOTOS_PER_PAGE);
    if (selected >= app->numPhotos) {
        return 1;
    }
    app->slotIdx = selected;
    app->spriteCol = app->cursorX;
    app->spriteRow = app->cursorY;
    PlaySE(SEQ_SE_DP_SELECT);
    PhotoAlbumApp_OpenContextMenu(app);
    PhotoAlbumApp_PrintPhotoDetails(app, app->entries[selected].photo);
    PhotoAlbumApp_UpdateSelectionSprite(app, 1);
    return 4;
}
#pragma force_active reset

int PhotoAlbumApp_HandleGridTouch(PhotoAlbumApp *app, int *a) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(ov109_021E7A18);
    if (hit == -1) {
        return 1;
    }
    if (app->nextMenuState == 0) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }
    *a = 1;
    if ((u32)hit < 12) {
        app->cursorX = hit & 3;
        app->cursorY = (u32)hit >> 2;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        PlaySE(SEQ_SE_DP_SELECT);
    }
    PhotoAlbumApp_HandleGridSelection(app, (u8)hit);
}

int PhotoAlbumApp_HandleGridKeys(PhotoAlbumApp *app) {
    u32 keys;
    u8 hit;
    if (app->nextMenuState == 1 && (gSystem.newKeys & 0xF3)) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_A) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = 12;
        } else {
            hit += app->cursorY * 4;
        }
        return PhotoAlbumApp_HandleGridSelection(app, hit);
    }
    if (keys & PAD_BUTTON_B) {
        return PhotoAlbumApp_HandleGridSelection(app, 12);
    }
    if (keys & PAD_KEY_LEFT) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = 12;
        } else {
            hit += app->cursorY * 4;
        }
        if (hit == 12) {
            return 1;
        }
        if (app->cursorX != 0) {
            PlaySE(0x5DC);
            app->cursorX--;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        } else if (app->curPage != 0) {
            return PhotoAlbumApp_HandleGridSelection(app, 13);
        }
    } else if (keys & PAD_KEY_RIGHT) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = 12;
        } else {
            hit += app->cursorY * 4;
        }
        if (hit == 12) {
            return 1;
        }
        if (app->cursorX < 3) {
            PlaySE(0x5DC);
            app->cursorX++;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        } else if (app->curPage < app->numPages - 1) {
            return PhotoAlbumApp_HandleGridSelection(app, 14);
        }
    } else if (keys & PAD_KEY_UP) {
        if (app->cursorY == 4 && app->cursorX < 3) {
            app->cursorX = 3;
        }
        PlaySE(0x5DC);
        app->cursorY = (app->cursorY + 3) % 4;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    } else if (keys & PAD_KEY_DOWN) {
        PlaySE(0x5DC);
        app->cursorY = (app->cursorY + 1) % 4;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    }
    return 1;
}

int PhotoAlbumApp_HandleMoveInput(PhotoAlbumApp *app) {
    int done = 0;
    PhotoAlbumApp_HandleMoveTouch(app, &done);
    if (!done) {
        PhotoAlbumApp_HandleMoveKeys(app);
    }
}

int PhotoAlbumApp_RunMoveDone(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        if (app->timer++ < 15) {
            return 0;
        }
        PhotoAlbumApp_DrawPage(app, 3, 3, app->curPage);
        PhotoAlbumApp_PrintMessage(app, 3, 0, app->textDelay);
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
        app->mode = 0;
        PlaySE(0x5DC);
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        PhotoAlbumApp_PrintMessage(app, 0, 0, 0);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, 3);
        app->timer = 0;
        app->subState = 0;
        return 1;
    }
    return 0;
}

int PhotoAlbumApp_HandleMoveSelection(PhotoAlbumApp *app, u32 input) {
    u8 slot;
    switch (input) {
    case 12:
        PlaySE(0x5DC);
        PhotoAlbumApp_PrintPhotoDetails(app, 0);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_StartCloseButtonAnim(app);
        PhotoAlbumApp_PrintMessage(app, 0, 0, 0);
        app->mode = 0;
        return 1;
    case 13:
        if (app->curPage == 0) {
            return 5;
        }
        PhotoAlbumApp_AnimatePageArrow(app, 2);
        PlaySE(0x920);
        return 2;
    case 14:
        if (app->curPage == app->numPages - 1) {
            return 5;
        }
        PhotoAlbumApp_AnimatePageArrow(app, 3);
        PlaySE(0x920);
        return 3;
    default:
        slot = _021E7884[input] + app->curPage * 12;
        if (slot >= app->numPhotos) {
            return 5;
        }
        PlaySE(0x5DC);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_PrintPhotoDetails(app, 0);
        if (slot != app->slotIdx) {
            PhotoAlbumApp_SwapEntries(app, app->slotIdx, slot);
            PhotoAlbumApp_RedrawSwappedIcons(app, app->slotIdx, slot);
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 0);
            app->timer = 0;
            return 6;
        }
        app->mode = 0;
        PhotoAlbumApp_PrintMessage(app, 0, 0, 0);
        return 1;
    }
}

int PhotoAlbumApp_HandleMoveTouch(PhotoAlbumApp *app, int *a) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(ov109_021E7A18);
    if (hit == -1) {
        return 5;
    }
    if (app->nextMenuState == 0) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }
    *a = 1;
    if ((u32)hit < 12) {
        app->cursorX = hit & 3;
        app->cursorY = (u32)hit >> 2;
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        PlaySE(SEQ_SE_DP_SELECT);
    }
    PhotoAlbumApp_HandleMoveSelection(app, (u8)hit);
}

int PhotoAlbumApp_HandleMoveKeys(PhotoAlbumApp *app) {
    u8 hit;

    if (app->nextMenuState == 1 && (gSystem.newKeys & 0xF3)) {
        PhotoAlbumApp_ToggleMenuInputState(app);
    }

    if (gSystem.newKeys & PAD_BUTTON_A) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = 12;
        } else {
            hit += app->cursorY * 4;
        }
        return PhotoAlbumApp_HandleMoveSelection(app, hit);
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        return PhotoAlbumApp_HandleMoveSelection(app, 12);
    }
    if (gSystem.newKeys & PAD_KEY_LEFT) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = 12;
        } else {
            hit += app->cursorY * 4;
        }
        if (hit == 12) {
            return 1;
        }
        if (app->cursorX != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorX--;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        } else if (app->curPage != 0) {
            app->cursorX = (app->cursorX + 3) % 4;
            return PhotoAlbumApp_HandleMoveSelection(app, 13);
        }
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        hit = app->cursorX;
        if (app->cursorY >= 3) {
            hit = 12;
        } else {
            hit += app->cursorY * 4;
        }
        if (hit == 12) {
            return 1;
        }
        if (app->cursorX < 3) {
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorX++;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        } else if (app->curPage < app->numPages - 1) {
            app->cursorX = (app->cursorX + 1) % 4;
            return PhotoAlbumApp_HandleMoveSelection(app, 14);
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
    return 5;
}

int PhotoAlbumApp_FlipToPrevPage(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 0);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_RequestBgToggle(app, 1);
        PhotoAlbumApp_SetZoom(app, FX32_ONE);
        ov109_021E6F7C(app, 1, 1);
        PhotoAlbumApp_DrawPage(app, 2, 1, app->curPage);
        PhotoAlbumApp_DrawPage(app, 3, 1, app->curPage - 1);
        app->pageFlipPhase = 0;
        app->subState++;
        break;
    case 1:
        app->pageFlipPhase++;
        PhotoAlbumApp_SetZoom(app, ov109_021E78DC[app->pageFlipPhase]);
        if (app->pageFlipPhase >= 4) {
            ov109_021E6F7C(app, 2, 1);
            PhotoAlbumApp_DrawPage(app, 2, 2, app->curPage - 1);
            app->subState++;
        }
        break;
    case 2:
        app->pageFlipPhase = app->pageFlipPhase - 1;
        PhotoAlbumApp_SetZoom(app, ov109_021E78DC[app->pageFlipPhase]);
        if (app->pageFlipPhase == 0) {
            PhotoAlbumApp_DrawPage(app, 3, 2, app->curPage - 1);
            PhotoAlbumApp_RequestBgToggle(app, 0);
            PhotoAlbumApp_SetZoom(app, FX32_ONE);
            app->curPage--;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
            PhotoAlbumApp_UpdatePageArrows(app);
            if (app->mode == 1) {
                PhotoAlbumApp_UpdateSelectionSprite(app, 1);
            }
            app->subState = 0;
            return 1;
        }
        break;
    }
    return 0;
}

int PhotoAlbumApp_FlipToNextPage(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 0);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_RequestBgToggle(app, 1);
        PhotoAlbumApp_SetZoom(app, FX32_ONE);
        ov109_021E6F7C(app, 2, 1);
        PhotoAlbumApp_DrawPage(app, 2, 2, app->curPage);
        PhotoAlbumApp_DrawPage(app, 3, 2, app->curPage + 1);
        app->pageFlipPhase = 0;
        app->subState++;
        break;
    case 1:
        app->pageFlipPhase++;
        PhotoAlbumApp_SetZoom(app, ov109_021E78DC[app->pageFlipPhase]);
        if (app->pageFlipPhase >= 4) {
            ov109_021E6F7C(app, 1, 1);
            PhotoAlbumApp_DrawPage(app, 2, 1, app->curPage + 1);
            app->subState++;
        }
        break;
    case 2:
        app->pageFlipPhase = app->pageFlipPhase - 1;
        PhotoAlbumApp_SetZoom(app, ov109_021E78DC[app->pageFlipPhase]);
        if (app->pageFlipPhase == 0) {
            PhotoAlbumApp_DrawPage(app, 3, 1, app->curPage + 1);
            PhotoAlbumApp_RequestBgToggle(app, 0);
            PhotoAlbumApp_SetZoom(app, FX32_ONE);
            app->curPage++;
            PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
            PhotoAlbumApp_UpdatePageArrows(app);
            if (app->mode == 1) {
                PhotoAlbumApp_UpdateSelectionSprite(app, 1);
            }
            app->subState = 0;
            return 1;
        }
        break;
    }
    return 0;
}

int PhotoAlbumApp_RunContextMenu(PhotoAlbumApp *app) {
    int r = PhotoAlbumApp_HandleContextMenu(app);
    if (r < 0) {
        r = 4;
    }
    return r;
}

int PhotoAlbumApp_RunDeletePrompt(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        PhotoAlbumApp_OpenDeletePrompt(app);
        ++app->subState;
        break;
    case 1:
        if (PhotoAlbumApp_HandleDeletePrompt(app) != 7) {
            app->subState = 0;
            return 1;
        }
        break;
    }
    return 7;
}

void PhotoAlbumApp_VBlankCB(PhotoAlbumApp *app) {
    if (app->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    PhotoAlbumApp_ApplyBgToggle(app);
    PhotoAlbumApp_ApplyZoom(app);
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(app->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void PhotoAlbumApp_ApplyBgToggle(PhotoAlbumApp *app) {
    if (app->bgToggleReq) {
        ToggleBgLayer(GF_BG_LYR_MAIN_2, app->bgToggleVal);
    }
    app->bgToggleReq = FALSE;
}

void PhotoAlbumApp_RequestBgToggle(PhotoAlbumApp *app, int on) {
    app->bgToggleVal = on;
    app->bgToggleReq = TRUE;
}

void PhotoAlbumApp_ApplyZoom(PhotoAlbumApp *app) {
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

void PhotoAlbumApp_SetZoom(PhotoAlbumApp *app, fx32 v) {
    app->zoom = v;
    app->zoomChanged = TRUE;
}

void PhotoAlbumApp_SetBanks(void) {
    GraphicsBanks banks = ov109_021E79F0;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on
void PhotoAlbumApp_InitBgs(PhotoAlbumApp *app) {
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
    modes = ov109_021E78B8;
    SetBothScreensModesAndDisable(&modes);
    // POWCNT1: main engine on the bottom screen
    *(volatile u16 *)0x04000304 &= ~0x8000;
    template4 = ov109_021E7944;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_0, &template4, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    template7 = ov109_021E797C;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_3, &template7, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_3);
    template6 = ov109_021E7998;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_2, &template6, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_2);
    template0 = ov109_021E790C;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_0, &template0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0);
    template1 = ov109_021E7928;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_1, &template1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    template2 = ov109_021E7960;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_2, &template2, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    template3 = ov109_021E79B4;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_3, &template3, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, app->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 32, 0, app->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, app->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 64, 0, app->heapId);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_X, -4);
}
#pragma force_active reset

void PhotoAlbumApp_FreeBgs(PhotoAlbumApp *app) {
    vu16 *reg = (vu16 *)0x04000304;
    BgSetPosTextAndCommit(app->bgConfig, 7, (enum BgPosAdjustOp)0, 0);
    FreeBgTilemapBuffer(app->bgConfig, 3);
    FreeBgTilemapBuffer(app->bgConfig, 2);
    FreeBgTilemapBuffer(app->bgConfig, 1);
    FreeBgTilemapBuffer(app->bgConfig, 0);
    FreeBgTilemapBuffer(app->bgConfig, 6);
    FreeBgTilemapBuffer(app->bgConfig, 7);
    FreeBgTilemapBuffer(app->bgConfig, 4);
    Heap_Free(app->bgConfig);
    *reg |= (u32)reg >> 11;
}

void PhotoAlbumApp_LoadBgGfx(PhotoAlbumApp *app) {
    NARC *narc = NARC_New((NarcId)0xAB, (enum HeapID)app->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, (enum GFPalLoadLocation)0, (enum GFPalSlotOffset)0, 0, (enum HeapID)app->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, (enum GFPalLoadLocation)4, (enum GFPalSlotOffset)0, 0, (enum HeapID)app->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 9, app->bgConfig, (GFBgLayer)6, 0, 0, 0, (enum HeapID)app->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->bgConfig, (GFBgLayer)7, 0, 0, 0, (enum HeapID)app->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, app->bgConfig, (GFBgLayer)6, 0, 0, 0, (enum HeapID)app->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->bgConfig, (GFBgLayer)3, 0, 0, 0, (enum HeapID)app->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, app->bgConfig, (GFBgLayer)3, 0, 0, 0, (enum HeapID)app->heapId);
    app->screenFile_B0 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, 0, &app->screen_AC, (enum HeapID)app->heapId);
    app->photoIconsFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, 0, &app->photoIcons, (enum HeapID)app->heapId);
    app->pageArrowsFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 7, 0, &app->pageArrows, (enum HeapID)app->heapId);
    NARC_Delete(narc);
    LoadFontPal1((enum GFPalLoadLocation)0, (enum GFPalSlotOffset)0x180, (enum HeapID)app->heapId);
    LoadUserFrameGfx2(app->bgConfig, (GFBgLayer)0, 1, 0xD, app->frame, (enum HeapID)app->heapId);
}

void PhotoAlbumApp_FreeScreenData(PhotoAlbumApp *app) {
    Heap_Free(app->pageArrowsFile);
    Heap_Free(app->photoIconsFile);
    Heap_Free(app->screenFile_B0);
}

void PhotoAlbumApp_LoadStrings(PhotoAlbumApp *app) {
    int i;
    FontID_Alloc(4, app->heapId);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (NarcId)0x1B, 0, app->heapId);
    app->messageFormat = MessageFormat_New_Custom(6, 0x16, app->heapId);
    app->str_2C = String_New(0x80, app->heapId);
    app->str_30 = NewString_ReadMsgData(app->msgData, 0);
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

void PhotoAlbumApp_FreeStrings(PhotoAlbumApp *app) {
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

void PhotoAlbumApp_CreateWindows(PhotoAlbumApp *app) {
    int i;
    for (i = 0; i < 3; ++i) {
        AddWindow(app->bgConfig, &app->windows[i], &ov109_021E78F4[i]);
        FillWindowPixelBuffer(&app->windows[i], 0);
    }
    app->yesNoPrompt = YesNoPrompt_Create(app->heapId);
}

void PhotoAlbumApp_RemoveWindows(PhotoAlbumApp *app) {
    int i;
    for (i = 0; i < 3; ++i) {
        ClearWindowTilemapAndCopyToVram(&app->windows[i]);
        RemoveWindow(&app->windows[i]);
    }
    YesNoPrompt_Destroy(app->yesNoPrompt);
}

void PhotoAlbumApp_InitSpriteSystem(PhotoAlbumApp *app) {
    GF_CreateVramTransferManager(0x20, app->heapId);
    app->spriteSystem = SpriteSystem_Alloc(app->heapId);
    SpriteSystem_Init(app->spriteSystem, &ov109_021E79D0, &ov109_021E78C8, 3);
    thunk_ClearMainOAM(app->heapId);
    thunk_ClearSubOAM(app->heapId);
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 5);
    sub_0200D2A4(app->spriteSystem, app->spriteManager, &ov109_021E78A8, 2, 1);
}

void PhotoAlbumApp_FreeSpriteSystem(PhotoAlbumApp *app) {
    SpriteSystem_DestroySpriteManager(app->spriteSystem, app->spriteManager);
    app->spriteManager = NULL;
    SpriteSystem_Free(app->spriteSystem);
    app->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(app->heapId);
}

void PhotoAlbumApp_InitSpritesAndMenus(PhotoAlbumApp *app) {
    PhotoAlbumApp_InitSpriteSystem(app);
    PhotoAlbumApp_CreateSprites(app);
    app->listMenuSpawner = TouchscreenListMenuSpawner_Create(app->heapId, NULL);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void PhotoAlbumApp_FreeSpritesAndMenus(PhotoAlbumApp *app) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    PhotoAlbumApp_DeleteSprites(app);
    TouchscreenListMenuSpawner_Destroy(app->listMenuSpawner);
    PhotoAlbumApp_FreeSpriteSystem(app);
}

#pragma force_active on
void PhotoAlbumApp_CreateSprites(PhotoAlbumApp *app) {
    u8 *template = ov109_021E7A58;
    int i;
    for (i = 0; i < 5; i++) {
        app->sprites[i] = SpriteSystem_CreateSpriteFromResourceHeader(app->spriteSystem, app->spriteManager, (UnmanagedSpriteTemplate *)template);
        Sprite_SetDrawFlag(app->sprites[i], TRUE);
        Sprite_SetAnimSpeed(app->sprites[i], FX32_ONE);
        template += sizeof(UnmanagedSpriteTemplate);
    }
    Sprite_SetAnimActiveFlag(app->sprites[0], FALSE);
    Sprite_SetDrawFlag(app->sprites[1], FALSE);
    Sprite_SetPriority(app->sprites[4], 2);
    Sprite_SetAnimActiveFlag(app->sprites[4], TRUE);
}
#pragma force_active reset

void PhotoAlbumApp_DeleteSprites(PhotoAlbumApp *app) {
    int i;
    for (i = 0; i < 5; ++i) {
        thunk_Sprite_Delete(app->sprites[i]);
    }
}

#pragma force_active on
void ov109_021E6F7C(PhotoAlbumApp *app, int position, int draw) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    if (draw) {
        CopyToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, (u8)(16 * (position / 2)), 0, 16, 24, app->pageArrows->rawData, (u8)(16 * (position / 2)), 0, (u8)(app->pageArrows->screenWidth >> 3), (u8)(app->pageArrows->screenHeight >> 3));
    }
}

void ov109_021E6FD4(PhotoAlbumApp *app, int bgId, int mode, int index) {
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

void PhotoAlbumApp_DrawGridColumns(PhotoAlbumApp *app, int bgId, int page, u8 start) {
    int row;
    int x;
    u8 n;
    for (row = 0; row < 3; row++) {
        for (x = start; x < start + 2; x++) {
            n = _021E7884[row * 4 + x] + page * PHOTOS_PER_PAGE;
            if (n < app->numPhotos) {
                PhotoAlbumApp_DrawGridIcon(app, app->entries[n].photo, bgId, x, row);
            } else {
                PhotoAlbumApp_DrawGridIcon(app, NULL, bgId, x, row);
            }
        }
    }
}
#pragma force_active reset

void PhotoAlbumApp_DrawPage(PhotoAlbumApp *app, int bgId, int mask, u8 page) {
    if (mask & 1) {
        PhotoAlbumApp_DrawGridColumns(app, bgId, page, 0);
        ov109_021E6FD4(app, bgId, 1, page);
    }
    if (mask & 2) {
        PhotoAlbumApp_DrawGridColumns(app, bgId, page, 2);
        ov109_021E6FD4(app, bgId, 2, page);
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, (u8)bgId);
}

void PhotoAlbumApp_DrawPhotoIcon(PhotoAlbumApp *app, int index, u8 bgId, u8 destX, u8 destY) {
    NNSG2dScreenData *tiles;
    int rem;
    int quot;

    tiles = app->photoIcons;
    rem = index % 6;
    quot = index / 6;
    CopyToBgTilemapRect(app->bgConfig, bgId, destX, destY, 5, 5, tiles->rawData, (u8)(rem * 5), (u8)(quot * 5), (u8)(tiles->screenWidth >> 3), (u8)(tiles->screenHeight >> 3));
}

#pragma force_active on
void PhotoAlbumApp_DrawGridIcon(PhotoAlbumApp *app, Photo *photo, int bgId, u8 x, u8 y) {
    u8 icon = 0;
    if (photo) {
        icon = photo->iconId + 1;
    }
    PhotoAlbumApp_DrawPhotoIcon(app, icon, (u8)bgId, (u8)((x * 6 + 4) + (u8)(x >> 1)), (u8)(y * 5 + 1));
}
#pragma force_active reset

void PhotoAlbumApp_RedrawSwappedIcons(PhotoAlbumApp *app, int oldIndex, int newIndex) {
    int page;
    u8 packed;

    page = oldIndex / 12;
    if (page == app->curPage) {
        packed = ov109_021E7890[oldIndex % 12];
        PhotoAlbumApp_DrawGridIcon(app, NULL, 3, packed % 4, packed >> 2);
    }
    page = newIndex / 12;
    if (page == app->curPage) {
        packed = ov109_021E7890[newIndex % 12];
        PhotoAlbumApp_DrawGridIcon(app, NULL, 3, packed % 4, packed >> 2);
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 3);
}

#pragma force_active on
// Prints the details of `photo` (where, which Pokemon, when) on the top screen; NULL clears them.
void PhotoAlbumApp_PrintPhotoDetails(PhotoAlbumApp *app, Photo *photo) {
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
#pragma force_active reset

void PhotoAlbumApp_PrintCloseButton(PhotoAlbumApp *app, int a) {
    FillWindowPixelBuffer(&app->windows[0], 0);
    if (a) {
        Sprite_SetAnimCtrlSeq(app->sprites[4], 9);
        Sprite_ResetAnimCtrlState(app->sprites[4]);
    }
    AddTextPrinterParameterizedWithColor(&app->windows[0], 4, app->str_30, (0x40 - FontID_String_GetWidth(4, app->str_30, 0)) >> 1, 0, 0xFF, 0x00010500, NULL);
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
}

#pragma force_active on
void PhotoAlbumApp_PrintMessage(PhotoAlbumApp *app, int msgIdx, BOOL drawFrame, u8 textDelay) {
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
#pragma force_active reset

void PhotoAlbumApp_SetCursorPos(PhotoAlbumApp *app, u8 x, u8 y, int draw) {
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

void PhotoAlbumApp_UpdateSelectionSprite(PhotoAlbumApp *app, int draw) {
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

void PhotoAlbumApp_UpdatePageArrows(PhotoAlbumApp *app) {
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

void PhotoAlbumApp_AnimatePageArrow(PhotoAlbumApp *app, int idx) {
    Sprite_ResetAnimCtrlState(app->sprites[idx]);
    Sprite_SetAnimActiveFlag(app->sprites[idx], TRUE);
    if (idx == 2) {
        Sprite_SetAnimCtrlSeq(app->sprites[idx], 1);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[idx], 4);
    }
}

int PhotoAlbumApp_ToggleMenuInputState(PhotoAlbumApp *app) {
    if (app->nextMenuState == 0) {
        app->nextMenuState = 1;
    } else {
        app->nextMenuState = 0;
    }
    return 0;
}

void PhotoAlbumApp_OpenContextMenu(PhotoAlbumApp *app) {
    TouchscreenListMenuHeader header;
    MI_CpuFill8(&header, 0, sizeof(header));
    header.template = ov109_021E789C;
    header.listMenuItems = app->listMenuItems;
    header.bgConfig = app->bgConfig;
    header.numWindows = 4;
    app->listMenu = TouchscreenListMenu_Create(app->listMenuSpawner, &header, (u8)app->nextMenuState, 0x18, 0xB, 6, 0);
    PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 0);
}

int PhotoAlbumApp_HandleContextMenu(PhotoAlbumApp *app) {
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
    case 0:
        app->args->photoWasSelected = 1;
        app->args->cursorPos = app->slotIdx;
        return 8;
    case 1:
        PhotoAlbumApp_PrintMessage(app, 4, 0, 0);
        app->mode = 2;
        return 7;
    case 2:
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        PhotoAlbumApp_PrintMessage(app, 2, 0, 0);
        app->mode = 1;
        return 5;
    default:
        PhotoAlbumApp_PrintPhotoDetails(app, 0);
        PhotoAlbumApp_UpdateSelectionSprite(app, 0);
        PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
        return 1;
    }
}

void PhotoAlbumApp_OpenDeletePrompt(PhotoAlbumApp *app) {
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

int PhotoAlbumApp_HandleDeletePrompt(PhotoAlbumApp *app) {
    int result = YesNoPrompt_HandleInput(app->yesNoPrompt);
    switch (result) {
    case 1:
        PhotoAlbumApp_RemoveEntry(app, app->slotIdx);
        break;
    case 2:
        break;
    default:
        return 7;
    }
    {
        u32 touch = YesNoPrompt_IsInTouchMode(app->yesNoPrompt);
        if (touch != app->nextMenuState) {
            app->nextMenuState = touch;
        }
    }
    YesNoPrompt_Reset(app->yesNoPrompt);
    PhotoAlbumApp_UpdateSelectionSprite(app, 0);
    if (result == 1) {
        PhotoAlbumApp_DrawPage(app, 3, 3, app->curPage);
        PhotoAlbumApp_UpdatePageArrows(app);
    }
    PhotoAlbumApp_SetCursorPos(app, app->cursorX, app->cursorY, 1);
    PhotoAlbumApp_PrintPhotoDetails(app, 0);
    PhotoAlbumApp_PrintMessage(app, 0, 0, 0);
    app->mode = 0;
    return 1;
}

void PhotoAlbumApp_StartCloseButtonAnim(PhotoAlbumApp *app) {
    PhotoAlbumTask *task = Heap_AllocAtEnd(app->heapId, sizeof(PhotoAlbumTask));
    MI_CpuFill8(task, 0, sizeof(PhotoAlbumTask));
    task->app = app;
    SysTask_CreateOnMainQueue((void (*)(SysTask *, void *))Task_PhotoAlbumApp_WaitCloseButtonAnim, task, 0);
    PhotoAlbumApp_PrintCloseButton(app, 1);
    app->fadeRunning = TRUE;
}

void Task_PhotoAlbumApp_WaitCloseButtonAnim(SysTask *task, PhotoAlbumTask *data) {
    if (!Sprite_IsAnimated(data->app->sprites[4])) {
        PhotoAlbumApp_PrintCloseButton(data->app, 0);
        data->app->fadeRunning = FALSE;
        MI_CpuFill8(data, 0, sizeof(PhotoAlbumTask));
        Heap_Free(data);
        SysTask_Destroy(task);
    }
}

u8 Photo_CountPartyMons(Photo *photo) {
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
