#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0430.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_render.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "safari_area_customizer.h"
#include "safari_zone.h"
#include "sav_system_info.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "swap_3d_buffers.h"
#include "safari_zone_encounters.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

#define NUM_OBJECTS_PER_PAGE 6 // objects in the list (3 rows of 2); row 3 holds the back button

// The fields of SafariDecorationArgs (overlay_02.h) this app reads and writes, with the four placement spots
// (one per object size class, filled in by the field) as an array.
typedef struct SafariDecorationSpot {
    u8 valid;
    u8 x;
    u8 unk_2;
    u8 z;
} SafariDecorationSpot;

typedef struct SafariDecorationArgsView {
    SaveData *saveData;                   // 0x00
    MenuInputStateMgr *menuInputStateMgr; // 0x04
    SafariDecorationSpot spots[4];        // 0x08
    u8 isSurfing;                         // 0x18
    u8 area;                              // 0x19
    u8 height;                            // 0x1A
    u16 placedObject;                     // 0x1C: 255 if nothing was placed
    u8 *cursorPos;                        // 0x20: list position kept by the field between visits
} SafariDecorationArgsView;

// One entry of the list of objects the player can place.
typedef struct SafariDecorationObject {
    u8 objectId;               // 0x0
    u8 sizeClass;              // 0x1: index into SafariDecorationArgsView.spots
    SafariObjectConfig config; // 0x2
} SafariDecorationObject;      // size: 0x5

typedef struct SafariDecorationAnim {
    void *file;           // 0x00
    void *resAnm;         // 0x04
    NNSG3dAnmObj *anmObj; // 0x08
    fx32 frame;           // 0x0C
    u8 filler_10[4];      // 0x10
} SafariDecorationAnim;   // size: 0x14

typedef struct SafariDecorationModel {
    NNSG3dResFileHeader *file;     // 0x00
    NNSG3dResMdlSet *mdlSet;       // 0x04
    NNSG3dResMdl *mdl;             // 0x08
    NNSG3dResTex *tex;             // 0x0C
    NNSG3dRenderObj renderObj;     // 0x10
    SafariDecorationAnim anims[4]; // 0x64
    u16 loaded;                    // 0xB4
    u16 numAnims;                  // 0xB6
} SafariDecorationModel;           // size: 0xB8

// The 3D preview of the object under the cursor (top screen).
typedef struct SafariDecorationModelView {
    Camera *camera;              // 0x00
    NNSFndAllocator allocator;   // 0x04
    enum HeapID heapId;          // 0x14
    NARC *modelNarc;             // 0x18
    NARC *modelInfoNarc;         // 0x1C
    NARC *animNarc;              // 0x20
    SafariDecorationModel model; // 0x24
    VecFx32 target;              // 0xDC
    void *plttVramManWork;       // 0xE8
    void *texVramManWork;        // 0xEC
    u8 visible;                  // 0xF0
    u8 timeOfDay;                // 0xF1
} SafariDecorationModelView;     // size: 0xF4

// Header of a model's entry in the model info NARC.
typedef struct SafariDecorationModelInfo {
    u8 animated;
    u8 type;
    u8 filler_02[6];
    u32 animIds[4];
} SafariDecorationModelInfo;

typedef struct SafariDecoration {
    enum HeapID heapId;                  // 0x000
    int state;                           // 0x004
    int setupState;                      // 0x008
    int nextState;                       // 0x00C
    u32 touchMode;                       // 0x010
    u32 scrollTasks;                     // 0x014: running page scroll tasks
    u8 textSpeed;                        // 0x018
    u8 frame;                            // 0x019
    u8 area;                             // 0x01A
    SafariDecorationArgsView *args;      // 0x01C
    SafariZone *safariZone;              // 0x020
    SafariZoneAreaSet areaSet;           // 0x024
    MsgData *msgData;                    // 0x30C
    MessageFormat *msgFormat;            // 0x310
    String *tmpString;                   // 0x314
    String *string;                      // 0x318
    String *placeMessages[4];            // 0x31C
    String *pageTemplate;                // 0x32C
    String *backText;                    // 0x330
    SafariDecorationObject *objects;     // 0x334
    SafariDecorationModelView modelView; // 0x338
    u8 timeOfDay;                        // 0x42C
    u8 numObjects;                       // 0x42D
    u8 numPages;                         // 0x42E
    u8 unk_42F;                          // 0x42F
    u8 page;                             // 0x430
    u8 cursorPos;                        // 0x431
    u8 selection;                        // 0x432
    u8 selectedObjectId;                 // 0x433
    u8 modelUpdatePending;               // 0x434
    u8 objectSlotsLeft;                  // 0x435
    BgConfig *bgConfig;                  // 0x438
    SpriteSystem *spriteSystem;          // 0x43C
    SpriteManager *spriteManager;        // 0x440
    Sprite *sprites[4];                  // 0x444: 0 cursor, 1 back button, 2/3 page arrows
    Window windows[12];                  // 0x454: 6 + the 6 object name windows of a page
    YesNoPrompt *yesNoPrompt;            // 0x514
    void *listFrameScreenFile;           // 0x518
    NNSG2dScreenData *listFrameScreen;   // 0x51C
    void *listScreenFile;                // 0x520
    NNSG2dScreenData *listScreen;        // 0x524
    void *confirmScreenFile;             // 0x528
    NNSG2dScreenData *confirmScreen;     // 0x52C
} SafariDecoration;                      // size: 0x530

typedef struct SafariDecorationScrollTask {
    u32 *taskCount;           // 0x00
    SafariDecoration *decor;  // 0x04
    u8 dir;                   // 0x08
    u8 timer;                 // 0x09
    u8 filler_0A[10];         // 0x0A
} SafariDecorationScrollTask; // size: 0x14

typedef struct SafariDecorationCameraPos {
    u16 distance;
    s16 x;
    s16 y;
    s16 z;
} SafariDecorationCameraPos;

typedef struct SafariDecorationObjectGroup {
    u8 count;
    const u8 *objects;
} SafariDecorationObjectGroup;

extern const CameraAngle gSafariDecorationCameraAngle;
extern const VecFx32 gSafariDecorationCameraOffset;
extern const SafariDecorationCameraPos gSafariDecorationObjectCameraPositions[];
extern const u32 gSafariDecorationBackButtonColors[];
extern const ResdatIdList gSafariDecorationSpriteResources;
extern const GraphicsModes gSafariDecorationGraphicsModes;
extern const OamCharTransferParam gSafariDecorationOamCharTransferParam;
extern const BgTemplate gSafariDecorationBgTemplate_Sub1;
extern const BgTemplate gSafariDecorationBgTemplate_Sub2;
extern const BgTemplate gSafariDecorationBgTemplate_Main2;
extern const BgTemplate gSafariDecorationBgTemplate_Main3;
extern const BgTemplate gSafariDecorationBgTemplate_Main1;
extern const BgTemplate gSafariDecorationBgTemplate_Sub3;
extern const BgTemplate gSafariDecorationBgTemplate_Sub0;
extern const OamManagerParam gSafariDecorationOamManagerParam;
extern const GraphicsBanks gSafariDecorationGraphicsBanks;
extern const TouchscreenHitbox gSafariDecorationListHitboxes[];
extern const WindowTemplate gSafariDecorationWindowTemplates[];
extern const UnmanagedSpriteTemplate gSafariDecorationSpriteTemplates[];
extern const SafariDecorationObjectGroup gSafariDecorationObjectGroups[];
extern const u8 gSafariDecorationTrainerGroups[][4];

void SafariDecoration_InitFromArgs(SafariDecoration *decor, SafariDecorationArgsView *args);
void SafariDecoration_SaveResults(SafariDecoration *decor);
void SafariDecoration_ResetGraphics(void);
int SafariDecoration_State_SelectObject(SafariDecoration *decor);
int SafariDecoration_State_ConfirmPlace(SafariDecoration *decor);
int SafariDecoration_State_FadeIn(SafariDecoration *decor);
int SafariDecoration_State_FadeOut(SafariDecoration *decor);
void SafariDecoration_PlaceObject(SafariDecoration *decor);
void SafariDecorationModelView_Init(SafariDecorationModelView *view, int timeOfDay, enum HeapID heapId);
void SafariDecorationModelView_Draw(SafariDecorationModelView *view);
void SafariDecorationModelView_Free(SafariDecorationModelView *view);
void SafariDecorationModelView_SetObject(SafariDecorationModelView *view, u16 objectId, const SafariObjectConfig *config);
void SafariDecorationModelView_SetVisible(SafariDecorationModelView *view, u8 visible);
void SafariDecorationModelView_LoadObject(SafariDecorationModelView *view, int objectId, const SafariObjectConfig *config);
void SafariDecorationModelView_UnloadObject(SafariDecorationModelView *view, SafariDecorationModel *model);
void SafariDecorationModel_Draw(SafariDecorationModel *model);
void SafariDecorationModelView_LoadAnim(SafariDecorationModelView *view, SafariDecorationModel *model, SafariDecorationAnim *anim, int fileId);
void SafariDecorationModelView_FreeAnim(SafariDecorationModelView *view, SafariDecorationAnim *anim);
void SafariDecorationModel_AddAnim(SafariDecorationModel *model, SafariDecorationAnim *anim);
void SafariDecorationModel_RemoveAnim(SafariDecorationModel *model, SafariDecorationAnim *anim);
void SafariDecorationAnim_Advance(SafariDecorationAnim *anim);
void SafariDecorationModelView_InitCamera(SafariDecorationModelView *view, enum HeapID heapId);
BOOL SafariDecoration_Setup(SafariDecoration *decor);
BOOL SafariDecoration_Teardown(SafariDecoration *decor);
int SafariDecoration_HandleListInput(SafariDecoration *decor);
int SafariDecoration_WaitMessageDismiss(SafariDecoration *decor);
void SafariDecoration_SetConfirmMode(SafariDecoration *decor, BOOL confirm);
void SafariDecoration_OpenYesNoPrompt(SafariDecoration *decor);
int SafariDecoration_HandleYesNoPrompt(SafariDecoration *decor);
int SafariDecoration_ChooseObject(SafariDecoration *decor, u32 cursorPos);
int SafariDecoration_HandleListButtons(SafariDecoration *decor);
int SafariDecoration_HandleListTouch(SafariDecoration *decor, u32 *touched);
void SafariDecoration_VBlankCB(void *cbArg);
void SafariDecoration_SetGfxBanks(void);
void SafariDecoration_InitBgLayers(SafariDecoration *decor);
void SafariDecoration_FreeBgLayers(SafariDecoration *decor);
void SafariDecoration_LoadBgGraphics(SafariDecoration *decor);
void SafariDecoration_FreeScreenData(SafariDecoration *decor);
void SafariDecoration_InitModelView(SafariDecoration *decor);
void SafariDecoration_FreeModelView(SafariDecoration *decor);
void SafariDecoration_CreateStrings(SafariDecoration *decor);
void SafariDecoration_DeleteStrings(SafariDecoration *decor);
void SafariDecoration_AddWindows(SafariDecoration *decor);
void SafariDecoration_RemoveWindows(SafariDecoration *decor);
void SafariDecoration_InitSprites(SafariDecoration *decor);
void SafariDecoration_FreeSprites(SafariDecoration *decor);
void SafariDecoration_CreateSprites(SafariDecoration *decor);
void SafariDecoration_DeleteSprites(SafariDecoration *decor);
void SafariDecoration_SetBackButtonState(SafariDecoration *decor, int state);
void SafariDecoration_DrawSubScreen(SafariDecoration *decor, int confirm);
void SafariDecoration_DrawObjectList(SafariDecoration *decor);
void SafariDecoration_PrintSelectedObjectName(SafariDecoration *decor);
void SafariDecoration_MoveCursor(SafariDecoration *decor, u32 cursorPos);
void SafariDecoration_PrintMessage(SafariDecoration *decor, int msg);
void SafariDecoration_ShowErrorMessage(SafariDecoration *decor, int msg);
void SafariDecoration_PrintObjectDescription(SafariDecoration *decor);
void SafariDecoration_ShowObjectList(SafariDecoration *decor);
int SafariDecoration_CheckObjectPlaceable(SafariDecoration *decor, int index);
void SafariDecoration_StartPageScroll(SafariDecoration *decor, u8 dir, u32 *taskCount, u8 priority);
void SafariDecoration_Task_PageScroll(SysTask *task, void *data);
void SafariDecoration_RequestModelUpdate(SafariDecoration *decor);
SafariDecorationObject *SafariDecoration_CreateObjectList(u32 trainerId, int unlockLevel, u8 gender, u8 *numObjects, enum HeapID heapId);
u8 SafariDecoration_GetSizeClass(u8 width, u8 height);

// FX32 conversion of the camera table entries, rounded to nearest.
static inline fx32 SafariDecoration_RoundS16ToFx32(s16 n) {
    float f;

    if (n > 0) {
        f = 0.5f + (float)(n * FX32_ONE);
    } else {
        f = (float)(n * FX32_ONE) - 0.5f;
    }
    return (fx32)f;
}

static inline fx32 SafariDecoration_RoundU16ToFx32(u16 n) {
    float f;

    if (n) {
        f = 0.5f + (float)(n * FX32_ONE);
    } else {
        f = (float)(n * FX32_ONE) - 0.5f;
    }
    return (fx32)f;
}

BOOL SafariDecoration_Init(OverlayManager *man, int *state) {
    switch (*state) {
    case 0: {
        SafariDecorationArgsView *args;
        SafariDecoration *decor;

        SafariDecoration_ResetGraphics();
        Heap_Create(HEAP_ID_3, HEAP_ID_95, 0x48000);
        args = OverlayManager_GetArgs(man);
        decor = OverlayManager_CreateAndGetData(man, sizeof(SafariDecoration), HEAP_ID_95);
        MI_CpuFill8(decor, 0, sizeof(SafariDecoration));
        decor->heapId = HEAP_ID_95;
        SafariDecoration_InitFromArgs(decor, args);
        Sound_SetSceneAndPlayBGM(0x43, 0, 0);
        (*state)++;
        break;
    }
    case 1:
        if (SafariDecoration_Setup(OverlayManager_GetData(man))) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL SafariDecoration_Exit(OverlayManager *man, int *state) {
    SafariDecoration *decor = OverlayManager_GetData(man);
    enum HeapID heapId;

#pragma unused(state)
    if (!SafariDecoration_Teardown(decor)) {
        return FALSE;
    }
    SafariDecoration_ResetGraphics();
    SafariDecoration_SaveResults(decor);
    heapId = decor->heapId;
    MI_CpuFill8(decor, 0, sizeof(SafariDecoration));
    OverlayManager_FreeData(man);
    Heap_Destroy(heapId);
    return TRUE;
}

BOOL SafariDecoration_Main(OverlayManager *man, int *state) {
    SafariDecoration *decor = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        *state = SafariDecoration_State_FadeIn(decor);
        break;
    case 1:
        *state = SafariDecoration_State_SelectObject(decor);
        break;
    case 2:
        *state = SafariDecoration_State_ConfirmPlace(decor);
        break;
    case 3:
        *state = SafariDecoration_State_FadeOut(decor);
        break;
    case 4:
        return TRUE;
    }
    SpriteSystem_DrawSprites(decor->spriteManager);
    SafariDecorationModelView_Draw(&decor->modelView);
    return FALSE;
}

#pragma force_active on
void SafariDecoration_InitFromArgs(SafariDecoration *decor, SafariDecorationArgsView *args) {
    Options *options;
    PlayerProfile *profile;
    int unlockLevel;
    u32 trainerId;
    u8 gender;

    decor->safariZone = Save_SafariZone_Get(args->saveData);
    SafariZone_CopyAreaSet(decor->safariZone, 0, &decor->areaSet);
    options = Save_PlayerData_GetOptionsAddr(args->saveData);
    decor->textSpeed = Options_GetTextSpeed(options);
    decor->frame = Options_GetFrame(options);
    decor->touchMode = MenuInputStateMgr_GetState(args->menuInputStateMgr);
    decor->args = args;
    decor->cursorPos = *args->cursorPos % NUM_OBJECTS_PER_PAGE;
    decor->page = *decor->args->cursorPos / NUM_OBJECTS_PER_PAGE;
    profile = Save_PlayerData_GetProfile(args->saveData);
    unlockLevel = SafariZone_GetObjectUnlockLevel(decor->safariZone);
    trainerId = PlayerProfile_GetTrainerID(profile);
    gender = (u8)PlayerProfile_GetTrainerGender(profile);
    decor->objects = SafariDecoration_CreateObjectList(trainerId, unlockLevel, gender, &decor->numObjects, decor->heapId);
    decor->area = args->area;
    if (decor->area > 5) {
        decor->area = 0;
    }
    decor->objectSlotsLeft = SAFARI_ZONE_MAX_OBJECTS - decor->areaSet.areas[decor->area].active_object_count;
    decor->numPages = decor->numObjects / NUM_OBJECTS_PER_PAGE;
    if (decor->numObjects % NUM_OBJECTS_PER_PAGE > 0) {
        decor->numPages++;
    }
    if (decor->page >= decor->numPages) {
        decor->page = 0;
    }
    decor->args->placedObject = 255;
    decor->timeOfDay = GF_RTC_GetTimeOfDayByHour(Save_SysInfo_RTC_Get(args->saveData)->time.hour);
    if (decor->timeOfDay == RTC_TIMEOFDAY_LATE) {
        decor->timeOfDay = RTC_TIMEOFDAY_NITE;
    }
}
#pragma force_active reset

void SafariDecoration_SaveResults(SafariDecoration *decor) {
    Heap_Free(decor->objects);
    SafariZone_SetAreaSet(decor->safariZone, 0, &decor->areaSet);
    if (decor->cursorPos >= NUM_OBJECTS_PER_PAGE) {
        decor->cursorPos = NUM_OBJECTS_PER_PAGE - 1;
    }
    *decor->args->cursorPos = decor->cursorPos + NUM_OBJECTS_PER_PAGE * decor->page;
    MenuInputStateMgr_SetState(decor->args->menuInputStateMgr, (MenuInputState)decor->touchMode);
}

void SafariDecoration_ResetGraphics(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
    reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}

int SafariDecoration_State_SelectObject(SafariDecoration *decor) {
    switch (decor->state) {
    case 0:
        decor->state = SafariDecoration_HandleListInput(decor);
        break;
    case 1:
        decor->state = 0;
        return 2;
    case 2:
        decor->state = SafariDecoration_WaitMessageDismiss(decor);
        break;
    case 3:
        decor->state = 0;
        return decor->nextState;
    }
    return 1;
}

int SafariDecoration_State_ConfirmPlace(SafariDecoration *decor) {
    switch (decor->state) {
    case 0:
        SafariDecoration_SetConfirmMode(decor, TRUE);
        SafariDecoration_OpenYesNoPrompt(decor);
        decor->state++;
        break;
    case 1: {
        int response = SafariDecoration_HandleYesNoPrompt(decor);
        if (response < 0) {
            break;
        }
        if (response == TRUE) {
            SafariDecoration_PlaceObject(decor);
            decor->state = 0;
            return 3;
        }
        decor->state++;
        break;
    }
    default:
        SafariDecoration_SetConfirmMode(decor, FALSE);
        decor->state = 0;
        return 1;
    }
    return 2;
}

int SafariDecoration_State_FadeIn(SafariDecoration *decor) {
    switch (decor->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, decor->heapId);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        decor->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            decor->state = 0;
            return 1;
        }
        break;
    }
    return 0;
}

int SafariDecoration_State_FadeOut(SafariDecoration *decor) {
    switch (decor->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, decor->heapId);
        decor->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            decor->state = 0;
            return 4;
        }
        break;
    }
    return 3;
}

void SafariDecoration_PlaceObject(SafariDecoration *decor) {
    SAFARIZONE_OBJECT object;
    SafariDecorationObject *entry;

    MI_CpuFill8(&object, 0, sizeof(object));
    entry = &decor->objects[decor->selection];
    object.id = entry->objectId;
    object.x = decor->args->spots[entry->sizeClass].x % 32;
    object.z = decor->args->spots[entry->sizeClass].z % 32;
    object.y = decor->args->height;
    SafariZone_AddObjectToArea(&decor->areaSet, decor->args->area, &object);
    decor->args->placedObject = object.id;
}

#pragma force_active on
void SafariDecorationModelView_Init(SafariDecorationModelView *view, int timeOfDay, enum HeapID heapId) {
    u32 size;

    MI_CpuFill8(view, 0, sizeof(SafariDecorationModelView));
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    reg_G3X_DISP3DCNT &= ~0x3002;
    reg_G3X_DISP3DCNT &= 0xCFFB;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & ~0x3000) | 8;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & ~0x3000) | 16;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & ~0x3000) | 32;
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(RGB_BLACK, 0, 0x7FFF, 63, FALSE);
    reg_G3_SWAP_BUFFERS = 2;
    reg_G3_VIEWPORT = 0xBFFF0000;
    view->heapId = heapId;
    view->timeOfDay = timeOfDay;
    size = NNS_GfdGetLnkTexVramManagerWorkSize(128);
    view->texVramManWork = Heap_Alloc(view->heapId, size);
    GF_3DVramMan_InitLinkedListTexVramManager(0x20000, 0, view->texVramManWork, size, TRUE);
    size = NNS_GfdGetLnkPlttVramManagerWorkSize(256);
    view->plttVramManWork = Heap_Alloc(view->heapId, size);
    GF_3DVramMan_InitLinkedListPlttVramManager(0x4000, view->plttVramManWork, size, TRUE);
    HeapExp_FndInitAllocator(&view->allocator, heapId, 32);
    view->modelNarc = NARC_New(NARC_a_0_4_0, heapId);
    view->modelInfoNarc = NARC_New(NARC_a_1_0_7, heapId);
    view->animNarc = NARC_New(NARC_a_1_0_6, heapId);
    SafariDecorationModelView_InitCamera(view, heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 1; // BG0 (3D) priority 1
}
#pragma force_active reset

void SafariDecorationModelView_Draw(SafariDecorationModelView *view) {
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    NNS_G3dGeFlushBuffer();
    reg_G3_MTX_MODE = GX_MTXMODE_PROJECTION;
    reg_G3_MTX_IDENTITY = 0;
    reg_G3_MTX_MODE = GX_MTXMODE_POSITION_VECTOR;
    reg_G3_MTX_IDENTITY = 0;
    if (view->visible && view->model.loaded) {
        SafariDecorationModel_Draw(&view->model);
    }
    reg_G3_SWAP_BUFFERS = 1;
}

void SafariDecorationModelView_Free(SafariDecorationModelView *view) {
    if (view->model.loaded != FALSE) {
        SafariDecorationModelView_UnloadObject(view, &view->model);
    }
    Camera_Delete(view->camera);
    Heap_Free(view->plttVramManWork);
    Heap_Free(view->texVramManWork);
    NARC_Delete(view->animNarc);
    NARC_Delete(view->modelInfoNarc);
    NARC_Delete(view->modelNarc);
}

void SafariDecorationModelView_SetObject(SafariDecorationModelView *view, u16 objectId, const SafariObjectConfig *config) {
    if (view->model.loaded != FALSE) {
        SafariDecorationModelView_UnloadObject(view, &view->model);
    }
    view->visible = TRUE;
    if (config) {
        SafariDecorationModelView_LoadObject(view, objectId, config);
    }
}

void SafariDecorationModelView_SetVisible(SafariDecorationModelView *view, u8 visible) {
    view->visible = visible;
    if (view->visible == FALSE) {
        Thunk_G3X_Reset();
        Camera_PushLookAtToNNSGlb();
        NNS_G3dGeFlushBuffer();
        reg_G3_SWAP_BUFFERS = 1;
    }
}

#pragma force_active on
void SafariDecorationModelView_LoadObject(SafariDecorationModelView *view, int objectId, const SafariObjectConfig *config) {
    SafariDecorationModelInfo info;
    SafariDecorationModel *model = &view->model;
    const SafariDecorationCameraPos *pos;

    model->file = GfGfxLoader_LoadFromOpenNarc(view->modelNarc, config->buildModel, FALSE, view->heapId, FALSE);
    model->mdlSet = NNS_G3dGetMdlSet(model->file);
    model->mdl = NNS_G3dGetMdlByIdx(model->mdlSet, 0);
    model->tex = NNS_G3dGetTex(model->file);
    if (model->tex) {
        GF3dRender_AllocAndLoadTexResources(model->tex);
        GF3dRender_BindModelSet(model->file, model->tex);
    }
    NNS_G3dRenderObjInit(&model->renderObj, model->mdl);
    DC_FlushAll();
    model->loaded = TRUE;
    NARC_ReadWholeMember(view->modelInfoNarc, config->buildModel, &info);
    if (info.animated && info.type != 8) {
        int i;
        for (i = 0; i < 4; i++) {
            u32 fileId = info.animIds[i];
            if (fileId == 0xFFFFFFFF) {
                break;
            }
            SafariDecorationModelView_LoadAnim(view, model, &model->anims[model->numAnims], fileId);
            SafariDecorationModel_AddAnim(model, &model->anims[model->numAnims]);
            model->numAnims++;
        }
    }
    pos = &gSafariDecorationObjectCameraPositions[objectId];
    view->target.x = SafariDecoration_RoundS16ToFx32(pos->x);
    view->target.y = SafariDecoration_RoundS16ToFx32(pos->y);
    view->target.z = SafariDecoration_RoundS16ToFx32(pos->z);
    Camera_SetDistance(SafariDecoration_RoundU16ToFx32(pos->distance), view->camera);
}
#pragma force_active reset

void SafariDecorationModelView_UnloadObject(SafariDecorationModelView *view, SafariDecorationModel *model) {
    int i;
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;

    for (i = 0; i < model->numAnims; i++) {
        SafariDecorationModel_RemoveAnim(model, &model->anims[i]);
        SafariDecorationModelView_FreeAnim(view, &model->anims[i]);
    }
    if (model->tex != NULL) {
        NNS_G3dTexReleaseTexKey(model->tex, &texKey, &tex4x4Key);
        NNS_GfdFreeTexVram(texKey);
        NNS_GfdFreeTexVram(tex4x4Key);
        NNS_GfdFreePlttVram(NNS_G3dPlttReleasePlttKey(model->tex));
    }
    if (model->file != NULL) {
        Heap_Free(model->file);
    }
    MI_CpuFill8(model, 0, sizeof(SafariDecorationModel));
}

void SafariDecorationModel_Draw(SafariDecorationModel *model) {
    int i;
    MtxFx33 rotation;
    VecFx32 translation;
    VecFx32 scale;

    for (i = 0; i < model->numAnims; i++) {
        SafariDecorationAnim_Advance(&model->anims[i]);
    }
    translation.x = 0;
    translation.y = 0;
    translation.z = 0;
    scale.x = FX32_ONE;
    scale.y = FX32_ONE;
    scale.z = FX32_ONE;
    MTX_Identity33_(&rotation);
    GF3dRender_DrawModel(&model->renderObj, &translation, &rotation, &scale);
}

void SafariDecorationModelView_LoadAnim(SafariDecorationModelView *view, SafariDecorationModel *model, SafariDecorationAnim *anim, int fileId) {
    anim->file = GfGfxLoader_LoadFromOpenNarc(view->animNarc, fileId, FALSE, view->heapId, FALSE);
    anim->resAnm = NNS_G3dGetAnmByIdx(anim->file, 0);
    anim->anmObj = NNS_G3dAllocAnmObj(&view->allocator, anim->resAnm, model->mdl);
    NNS_G3dAnmObjInit(anim->anmObj, anim->resAnm, model->mdl, model->tex);
}

void SafariDecorationModelView_FreeAnim(SafariDecorationModelView *view, SafariDecorationAnim *anim) {
    if (anim->file) {
        NNS_G3dFreeAnmObj(&view->allocator, anim->anmObj);
        Heap_Free(anim->file);
        MI_CpuFill8(anim, 0, sizeof(SafariDecorationAnim));
    }
}

void SafariDecorationModel_AddAnim(SafariDecorationModel *model, SafariDecorationAnim *anim) {
    NNS_G3dRenderObjAddAnmObj(&model->renderObj, anim->anmObj);
}

void SafariDecorationModel_RemoveAnim(SafariDecorationModel *model, SafariDecorationAnim *anim) {
    NNS_G3dRenderObjRemoveAnmObj(&model->renderObj, anim->anmObj);
}

void SafariDecorationAnim_Advance(SafariDecorationAnim *anim) {
    fx32 frame = (anim->frame + FX32_ONE) % (((NNSG3dResAnmCommon *)anim->anmObj->resAnm)->numFrame << FX32_SHIFT);

    anim->frame = frame;
    anim->anmObj->frame = frame;
}

void SafariDecorationModelView_InitCamera(SafariDecorationModelView *view, enum HeapID heapId) {
    view->camera = Camera_New(heapId);
    view->target.x = 0;
    view->target.y = 0;
    view->target.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(&view->target, 0x15B000, &gSafariDecorationCameraAngle, 0x5C1, 0, TRUE, view->camera);
    Camera_SetStaticPtr(view->camera);
    Camera_SetPerspectiveClippingPlane(0x64000, 0x4B0000, view->camera);
    Camera_OffsetLookAtPosAndTarget(&gSafariDecorationCameraOffset, view->camera);
}

BOOL SafariDecoration_Setup(SafariDecoration *decor) {
    switch (decor->setupState) {
    case 0:
        SafariDecoration_InitBgLayers(decor);
        SafariDecoration_LoadBgGraphics(decor);
        SafariDecoration_InitModelView(decor);
        SafariDecoration_CreateStrings(decor);
        SafariDecoration_AddWindows(decor);
        SafariDecoration_InitSprites(decor);
        SafariDecoration_CreateSprites(decor);
        break;
    case 1:
        SafariDecoration_ShowObjectList(decor);
        Main_SetVBlankIntrCB(SafariDecoration_VBlankCB, decor);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(4);
        decor->setupState = 0;
        return TRUE;
    }
    decor->setupState++;
    return FALSE;
}

BOOL SafariDecoration_Teardown(SafariDecoration *decor) {
    switch (decor->setupState) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        SafariDecoration_DeleteSprites(decor);
        SafariDecoration_FreeSprites(decor);
        SafariDecoration_RemoveWindows(decor);
        SafariDecoration_DeleteStrings(decor);
        SafariDecoration_FreeModelView(decor);
        SafariDecoration_FreeScreenData(decor);
        SafariDecoration_FreeBgLayers(decor);
        break;
    case 2:
        GF_TouchpadStopSampling();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        decor->setupState = 0;
        return TRUE;
    }
    decor->setupState++;
    return FALSE;
}

int SafariDecoration_HandleListInput(SafariDecoration *decor) {
    u32 touched = FALSE;
    int result = SafariDecoration_HandleListTouch(decor, &touched);

    if (touched == FALSE) {
        result = SafariDecoration_HandleListButtons(decor);
    }
    return result;
}

int SafariDecoration_WaitMessageDismiss(SafariDecoration *decor) {
    int done = FALSE;

    if (System_GetTouchNew()) {
        done = TRUE;
        decor->touchMode = MENU_INPUT_STATE_TOUCH;
    } else {
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN)) {
            decor->touchMode = MENU_INPUT_STATE_BUTTONS;
        }
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            done = TRUE;
        }
    }
    if (done) {
        PlaySE(SEQ_SE_DP_BUTTON3);
        ClearFrameAndWindow2(&decor->windows[1], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&decor->windows[1]);
        return 0;
    }
    return 2;
}

void SafariDecoration_SetConfirmMode(SafariDecoration *decor, BOOL confirm) {
    BOOL draw;

    if (confirm == TRUE) {
        SafariDecoration_DrawSubScreen(decor, TRUE);
        SafariDecoration_SetBackButtonState(decor, 2);
        SafariDecoration_PrintSelectedObjectName(decor);
        SafariDecoration_PrintMessage(decor, 0);
        draw = FALSE;
    } else {
        SafariDecoration_DrawSubScreen(decor, FALSE);
        ClearWindowTilemapAndScheduleTransfer(&decor->windows[4]);
        SafariDecoration_SetBackButtonState(decor, 0);
        SafariDecoration_DrawObjectList(decor);
        SafariDecoration_PrintMessage(decor, 2);
        draw = TRUE;
    }
    Sprite_SetDrawFlag(decor->sprites[2], draw);
    Sprite_SetDrawFlag(decor->sprites[3], draw);
    Sprite_SetDrawFlag(decor->sprites[0], draw);
}

void SafariDecoration_OpenYesNoPrompt(SafariDecoration *decor) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = decor->bgConfig;
    template.bgId = GF_BG_LYR_SUB_0;
    template.tileStart = 0x3A6;
    template.plttSlot = 14;
    template.x = 26;
    template.y = 4;
    template.ignoreTouchFlag = decor->touchMode;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplateWithPalette(decor->yesNoPrompt, &template, NULL);
}

int SafariDecoration_HandleYesNoPrompt(SafariDecoration *decor) {
    int result;

    switch (YesNoPrompt_HandleInput(decor->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        result = TRUE;
        break;
    case YESNORESPONSE_NO:
        result = FALSE;
        break;
    default:
        return -1;
    }
    decor->touchMode = YesNoPrompt_IsInTouchMode(decor->yesNoPrompt);
    YesNoPrompt_Reset(decor->yesNoPrompt);
    return result;
}

int SafariDecoration_ChooseObject(SafariDecoration *decor, u32 cursorPos) {
    u8 result;

    if (cursorPos >= NUM_OBJECTS_PER_PAGE) {
        PlaySE(SEQ_SE_DP_SELECT);
        SafariDecoration_SetBackButtonState(decor, 1);
        decor->nextState = 3;
        return 3;
    }

    decor->selection = decor->cursorPos + NUM_OBJECTS_PER_PAGE * decor->page;
    if (decor->selection >= decor->numObjects) {
        return 0;
    }

    result = SafariDecoration_CheckObjectPlaceable(decor, decor->selection);
    if (result != 0) {
        PlaySE(SEQ_SE_DP_BOX03);
        SafariDecoration_ShowErrorMessage(decor, result - 1);
        return 2;
    }

    PlaySE(SEQ_SE_DP_SELECT);
    decor->selectedObjectId = decor->objects[decor->selection].objectId;
    return 1;
}

#pragma force_active on
int SafariDecoration_HandleListButtons(SafariDecoration *decor) {
    int scroll = 0;
    u32 keys;
    u8 row;
    u8 column;

    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN)) {
        decor->touchMode = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return SafariDecoration_ChooseObject(decor, NUM_OBJECTS_PER_PAGE);
    }
    if (keys & PAD_BUTTON_A) {
        return SafariDecoration_ChooseObject(decor, decor->cursorPos);
    }
    if (!(keys & (PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN))) {
        return 0;
    }
    column = (int)decor->cursorPos % 2;
    row = decor->cursorPos / 2;
    if (keys & PAD_KEY_RIGHT) {
        if (row == 3) {
            return 0;
        }
        if (column == 1) {
            if (decor->page < decor->numPages - 1 && row < 3) {
                decor->page++;
                scroll = 2;
            } else {
                return 0;
            }
        } else {
            column++;
        }
    } else if (keys & PAD_KEY_LEFT) {
        if (row == 3) {
            return 0;
        }
        if (column == 0) {
            if (decor->page && row < 3) {
                decor->page--;
                scroll = 1;
            } else {
                return 0;
            }
        } else {
            column--;
        }
    } else if (keys & PAD_KEY_UP) {
        row = (int)(row + 3) % 4;
    } else if (keys & PAD_KEY_DOWN) {
        row = (int)(row + 1) % 4;
    }
    decor->cursorPos = column + row * 2;
    if (scroll) {
        SafariDecoration_StartPageScroll(decor, scroll - 1, &decor->scrollTasks, 0);
        return 0;
    }
    PlaySE(SEQ_SE_DP_BUTTON3);
    SafariDecoration_MoveCursor(decor, decor->cursorPos);
    SafariDecoration_PrintObjectDescription(decor);
    return 0;
}
#pragma force_active reset

int SafariDecoration_HandleListTouch(SafariDecoration *decor, u32 *touched) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(gSafariDecorationListHitboxes);

    if (hit == -1) {
        *touched = FALSE;
        return 0;
    }
    if (hit == 7) {
        if (decor->page != 0) {
            decor->page--;
            SafariDecoration_StartPageScroll(decor, 0, &decor->scrollTasks, 0);
            *touched = TRUE;
        }
        return 0;
    }
    if (hit == 8) {
        if (decor->page < decor->numPages - 1) {
            decor->page++;
            SafariDecoration_StartPageScroll(decor, 1, &decor->scrollTasks, 0);
            *touched = TRUE;
        }
        return 0;
    }
    if (hit != decor->cursorPos) {
        decor->cursorPos = hit;
        SafariDecoration_MoveCursor(decor, decor->cursorPos);
        SafariDecoration_PrintObjectDescription(decor);
    }
    *touched = TRUE;
    return SafariDecoration_ChooseObject(decor, decor->cursorPos);
}

void SafariDecoration_VBlankCB(void *cbArg) {
    SafariDecoration *decor = cbArg;
    vu32 *dtcm = (vu32 *)0x027E0000;

    if (decor->modelUpdatePending != FALSE) {
        u8 index = decor->cursorPos + NUM_OBJECTS_PER_PAGE * decor->page;
        if (decor->cursorPos >= NUM_OBJECTS_PER_PAGE || index >= decor->numObjects) {
            SafariDecorationModelView_SetObject(&decor->modelView, 24, NULL);
        } else {
            SafariDecorationObject *object = &decor->objects[index];
            SafariDecorationModelView_SetObject(&decor->modelView, object->objectId, &object->config);
        }
        decor->modelUpdatePending = FALSE;
    }
    if (decor->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(decor->bgConfig);
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

void SafariDecoration_SetGfxBanks(void) {
    GraphicsBanks banks = gSafariDecorationGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on
void SafariDecoration_InitBgLayers(SafariDecoration *decor) {
    GraphicsModes modes;
    BgTemplate main1;
    BgTemplate main2;
    BgTemplate main3;
    BgTemplate sub0;
    BgTemplate sub1;
    BgTemplate sub2;
    BgTemplate sub3;

    SafariDecoration_SetGfxBanks();
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    decor->bgConfig = BgConfig_Alloc(decor->heapId);
    modes = gSafariDecorationGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    main1 = gSafariDecorationBgTemplate_Main1;
    InitBgFromTemplate(decor->bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(decor->bgConfig, GF_BG_LYR_MAIN_1);
    main2 = gSafariDecorationBgTemplate_Main2;
    InitBgFromTemplate(decor->bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(decor->bgConfig, GF_BG_LYR_MAIN_2);
    main3 = gSafariDecorationBgTemplate_Main3;
    InitBgFromTemplate(decor->bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(decor->bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = gSafariDecorationBgTemplate_Sub0;
    InitBgFromTemplate(decor->bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(decor->bgConfig, GF_BG_LYR_SUB_0);
    sub1 = gSafariDecorationBgTemplate_Sub1;
    InitBgFromTemplate(decor->bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(decor->bgConfig, GF_BG_LYR_SUB_1);
    sub2 = gSafariDecorationBgTemplate_Sub2;
    InitBgFromTemplate(decor->bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(decor->bgConfig, GF_BG_LYR_SUB_2);
    sub3 = gSafariDecorationBgTemplate_Sub3;
    InitBgFromTemplate(decor->bgConfig, GF_BG_LYR_SUB_3, &sub3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(decor->bgConfig, GF_BG_LYR_SUB_3);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, decor->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, decor->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 32, 0, decor->heapId);
}
#pragma force_active reset

void SafariDecoration_FreeBgLayers(SafariDecoration *decor) {
    FreeBgTilemapBuffer(decor->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(decor->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(decor->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(decor->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(decor->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(decor->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(decor->bgConfig, GF_BG_LYR_MAIN_1);
    Heap_Free(decor->bgConfig);
}

#pragma force_active on
void SafariDecoration_LoadBgGraphics(SafariDecoration *decor) {
    NARC *narc = NARC_New(NARC_a_2_2_3, decor->heapId);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, decor->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, decor->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, decor->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, decor->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, decor->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, decor->heapId);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, decor->heapId);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, decor->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, decor->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, decor->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 3, decor->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, decor->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, decor->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, decor->heapId);
    decor->listFrameScreenFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, FALSE, &decor->listFrameScreen, decor->heapId);
    decor->listScreenFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 6, FALSE, &decor->listScreen, decor->heapId);
    decor->confirmScreenFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 7, FALSE, &decor->confirmScreen, decor->heapId);
    NARC_Delete(narc);
    LoadUserFrameGfx2(decor->bgConfig, GF_BG_LYR_SUB_0, 994, 12, decor->frame, decor->heapId);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, decor->heapId);
    ScheduleBgTilemapBufferTransfer(decor->bgConfig, GF_BG_LYR_SUB_3);
    ScheduleBgTilemapBufferTransfer(decor->bgConfig, GF_BG_LYR_MAIN_3);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 28, 4);
}
#pragma force_active reset

void SafariDecoration_FreeScreenData(SafariDecoration *decor) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 0);
    Heap_Free(decor->confirmScreenFile);
    Heap_Free(decor->listScreenFile);
    Heap_Free(decor->listFrameScreenFile);
}

void SafariDecoration_InitModelView(SafariDecoration *decor) {
    SafariDecorationModelView_Init(&decor->modelView, decor->timeOfDay, decor->heapId);
}

void SafariDecoration_FreeModelView(SafariDecoration *decor) {
    SafariDecorationModelView_Free(&decor->modelView);
}

#pragma force_active on
void SafariDecoration_CreateStrings(SafariDecoration *decor) {
    int i;

    FontID_Alloc(4, decor->heapId);
    decor->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0430_bin, decor->heapId);
    decor->msgFormat = MessageFormat_New_Custom(2, 16, decor->heapId);
    decor->tmpString = String_New(256, decor->heapId);
    decor->string = String_New(256, decor->heapId);
    decor->pageTemplate = NewString_ReadMsgData(decor->msgData, msg_0430_00012);
    decor->backText = NewString_ReadMsgData(decor->msgData, msg_0430_00008);
    for (i = 0; i < 4; i++) {
        decor->placeMessages[i] = NewString_ReadMsgData(decor->msgData, i);
    }
}
#pragma force_active reset

void SafariDecoration_DeleteStrings(SafariDecoration *decor) {
    int i;

    for (i = 0; i < 4; i++) {
        String_Delete(decor->placeMessages[i]);
    }
    String_Delete(decor->backText);
    String_Delete(decor->pageTemplate);
    String_Delete(decor->string);
    String_Delete(decor->tmpString);
    MessageFormat_Delete(decor->msgFormat);
    DestroyMsgData(decor->msgData);
    FontID_Release(4);
}

#pragma force_active on
void SafariDecoration_AddWindows(SafariDecoration *decor) {
    int i;
    u16 baseTile;

    for (i = 0; i <= 5; i++) {
        AddWindow(decor->bgConfig, &decor->windows[i], &gSafariDecorationWindowTemplates[i]);
        FillWindowPixelBuffer(&decor->windows[i], 0);
    }
    baseTile = 0x2E2;
    for (i = 0; i < NUM_OBJECTS_PER_PAGE; i++) {
        AddWindowParameterized(decor->bgConfig, &decor->windows[i + 6], GF_BG_LYR_SUB_1, (u8)(1 + (i % 2) * 16), (u8)(6 + (i / 2) * 5), 14, 2, 4, baseTile);
        baseTile -= 28;
    }
    decor->yesNoPrompt = YesNoPrompt_Create(decor->heapId);
}
#pragma force_active reset

void SafariDecoration_RemoveWindows(SafariDecoration *decor) {
    int i;

    YesNoPrompt_Destroy(decor->yesNoPrompt);
    for (i = 0; i < 12; i++) {
        ClearWindowTilemapAndCopyToVram(&decor->windows[i]);
        RemoveWindow(&decor->windows[i]);
    }
}

void SafariDecoration_InitSprites(SafariDecoration *decor) {
    GF_CreateVramTransferManager(32, decor->heapId);
    decor->spriteSystem = SpriteSystem_Alloc(decor->heapId);
    SpriteSystem_Init(decor->spriteSystem, &gSafariDecorationOamManagerParam, &gSafariDecorationOamCharTransferParam, 1);
    decor->spriteManager = SpriteManager_New(decor->spriteSystem);
    SpriteSystem_InitSprites(decor->spriteSystem, decor->spriteManager, 4);
    SpriteSystem_LoadResourceDataFromFileIdListEx(decor->spriteSystem, decor->spriteManager, &gSafariDecorationSpriteResources, 0, 0);
    thunk_ClearMainOAM(decor->heapId);
    thunk_ClearSubOAM(decor->heapId);
}

void SafariDecoration_FreeSprites(SafariDecoration *decor) {
    SpriteSystem_DestroySpriteManager(decor->spriteSystem, decor->spriteManager);
    SpriteSystem_Free(decor->spriteSystem);
    decor->spriteSystem = NULL;
    decor->spriteManager = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(decor->heapId);
    thunk_ClearSubOAM(decor->heapId);
}

void SafariDecoration_CreateSprites(SafariDecoration *decor) {
    const UnmanagedSpriteTemplate *template = gSafariDecorationSpriteTemplates;
    int i;

    for (i = 0; i < 4; i++) {
        decor->sprites[i] = SpriteSystem_CreateSpriteFromResourceHeader(decor->spriteSystem, decor->spriteManager, template);
        Sprite_SetDrawFlag(decor->sprites[i], TRUE);
        template ++;
    }
    thunk_Sprite_SetPriority(decor->sprites[0], 1);
}

void SafariDecoration_DeleteSprites(SafariDecoration *decor) {
    int i;

    for (i = 0; i < 4; i++) {
        thunk_Sprite_Delete(decor->sprites[i]);
    }
}

void SafariDecoration_SetBackButtonState(SafariDecoration *decor, int state) {
    int width;

    Sprite_SetAnimCtrlSeq(decor->sprites[1], state + 2);
    Sprite_ResetAnimCtrlState(decor->sprites[1]);
    FillWindowPixelBuffer(&decor->windows[2], 0);
    width = FontID_String_GetWidth(4, decor->backText, 0);
    AddTextPrinterParameterizedWithColor(&decor->windows[2], 4, decor->backText, (u32)(48 - width) / 2, 4, TEXT_SPEED_NOTRANSFER, gSafariDecorationBackButtonColors[state], NULL);
    ScheduleWindowCopyToVram(&decor->windows[2]);
}

void SafariDecoration_DrawSubScreen(SafariDecoration *decor, int confirm) {
    NNSG2dScreenData *screen = confirm == FALSE ? decor->listScreen : decor->confirmScreen;

    FillBgTilemapRect(decor->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 5, 32, 14, TILEMAP_FILL_OVWT_PAL);
    FillBgTilemapRect(decor->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 5, 32, 14, TILEMAP_FILL_OVWT_PAL);
    CopyToBgTilemapRect(decor->bgConfig, GF_BG_LYR_SUB_3, 0, 0, 32, 24, screen->rawData, 0, 0, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(decor->bgConfig, GF_BG_LYR_SUB_1);
    ScheduleBgTilemapBufferTransfer(decor->bgConfig, GF_BG_LYR_SUB_2);
    ScheduleBgTilemapBufferTransfer(decor->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active on
void SafariDecoration_DrawObjectList(SafariDecoration *decor) {
    int i;
    Window *windows;

    FillBgTilemapRect(decor->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 5, 32, 14, TILEMAP_FILL_OVWT_PAL);
    windows = decor->windows;
    for (i = 0; i < NUM_OBJECTS_PER_PAGE; i++) {
        int index = i + decor->page * NUM_OBJECTS_PER_PAGE;
        FillWindowPixelBuffer(&windows[i + 6], 0);
        if (index >= decor->numObjects) {
            ScheduleWindowCopyToVram(&windows[i + 6]);
        } else {
            u8 unavailable;
            NNSG2dScreenData *screen;
            int x;

            if (SafariDecoration_CheckObjectPlaceable(decor, (u8)index) > 0) {
                unavailable = TRUE;
            } else {
                unavailable = FALSE;
            }
            screen = decor->listFrameScreen;
            CopyToBgTilemapRect(decor->bgConfig, GF_BG_LYR_SUB_2, (u8)((i % 2) * 16), (u8)((i / 2 + 1) * 5), 16, 4, screen->rawData, unavailable * 16, 0, (u8)(screen->screenWidth / 8), (u8)(screen->screenHeight / 8));
            ReadMsgDataIntoString(decor->msgData, decor->objects[index].objectId + 14, decor->string);
            x = 112 - FontID_String_GetWidth(4, decor->string, 0);
            AddTextPrinterParameterizedWithColor(&windows[i + 6], 4, decor->string, x / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
            ScheduleWindowCopyToVram(&windows[i + 6]);
        }
    }
    FillWindowPixelBuffer(&decor->windows[3], 0);
    BufferIntegerAsString(decor->msgFormat, 0, decor->page + 1, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    BufferIntegerAsString(decor->msgFormat, 1, decor->numPages, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(decor->msgFormat, decor->tmpString, decor->pageTemplate);
    AddTextPrinterParameterizedWithColor(&decor->windows[3], 0, decor->tmpString, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
    ScheduleWindowCopyToVram(&decor->windows[3]);
    ScheduleBgTilemapBufferTransfer(decor->bgConfig, GF_BG_LYR_SUB_1);
    ScheduleBgTilemapBufferTransfer(decor->bgConfig, GF_BG_LYR_SUB_2);
    if (decor->page == 0) {
        Sprite_SetAnimCtrlSeq(decor->sprites[2], 7);
    } else {
        Sprite_SetAnimCtrlSeq(decor->sprites[2], 5);
    }
    if (decor->page == decor->numPages - 1) {
        Sprite_SetAnimCtrlSeq(decor->sprites[3], 10);
    } else {
        Sprite_SetAnimCtrlSeq(decor->sprites[3], 8);
    }
}
#pragma force_active reset

void SafariDecoration_PrintSelectedObjectName(SafariDecoration *decor) {
    int selection;
    int x;

    FillWindowPixelBuffer(&decor->windows[4], 0);
    selection = decor->selection;
    ReadMsgDataIntoString(decor->msgData, decor->objects[selection].objectId + 14, decor->string);
    x = 112 - FontID_String_GetWidth(4, decor->string, 0);
    AddTextPrinterParameterizedWithColor(&decor->windows[4], 4, decor->string, x / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&decor->windows[4]);
    ClearWindowTilemapAndScheduleTransfer(&decor->windows[3]);
}

void SafariDecoration_MoveCursor(SafariDecoration *decor, u32 cursorPos) {
    int anim;
    s16 x;
    s16 y;

    if (cursorPos >= NUM_OBJECTS_PER_PAGE) {
        x = 224;
        y = 180;
        anim = 1;
    } else {
        x = ((s32)cursorPos % 2) * 128;
        y = (cursorPos / 2 + 1) * 40;
        anim = 0;
    }
    Sprite_SetPositionXY(decor->sprites[0], x, y);
    Sprite_SetAnimCtrlSeq(decor->sprites[0], anim);
}

void SafariDecoration_PrintMessage(SafariDecoration *decor, int msg) {
    BOOL expand = FALSE;

    FillWindowPixelBuffer(&decor->windows[0], 0);
    switch (msg) {
    case 0:
        ReadMsgDataIntoString(decor->msgData, decor->selectedObjectId + 14, decor->tmpString);
        BufferString(decor->msgFormat, 0, decor->tmpString, 1, 0, 2);
        expand = TRUE;
        break;
    case 2:
        if (decor->objectSlotsLeft == 0) {
            msg = 3;
        } else {
            BufferIntegerAsString(decor->msgFormat, 0, decor->objectSlotsLeft, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            expand = TRUE;
        }
        break;
    }
    if (expand) {
        ReadMsgDataIntoString(decor->msgData, msg, decor->tmpString);
        StringExpandPlaceholders(decor->msgFormat, decor->string, decor->tmpString);
    } else {
        ReadMsgDataIntoString(decor->msgData, msg, decor->string);
    }
    AddTextPrinterParameterizedWithColor(&decor->windows[0], 0, decor->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
    ScheduleWindowCopyToVram(&decor->windows[0]);
}

void SafariDecoration_ShowErrorMessage(SafariDecoration *decor, int msg) {
    FillWindowPixelBuffer(&decor->windows[1], 15);
    ReadMsgDataIntoString(decor->msgData, msg + 4, decor->string);
    DrawFrameAndWindow2(&decor->windows[1], TRUE, 994, 12);
    AddTextPrinterParameterizedWithColor(&decor->windows[1], 1, decor->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    ScheduleWindowCopyToVram(&decor->windows[1]);
}

void SafariDecoration_PrintObjectDescription(SafariDecoration *decor) {
    u8 index;

    SafariDecoration_RequestModelUpdate(decor);
    FillWindowPixelBuffer(&decor->windows[5], 0);
    index = decor->page * NUM_OBJECTS_PER_PAGE + decor->cursorPos;
    if (decor->cursorPos >= NUM_OBJECTS_PER_PAGE || index >= decor->numObjects) {
        ScheduleWindowCopyToVram(&decor->windows[5]);
        return;
    }
    ReadMsgDataIntoString(decor->msgData, decor->objects[index].objectId + 38, decor->string);
    AddTextPrinterParameterizedWithColor(&decor->windows[5], 0, decor->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&decor->windows[5]);
}

void SafariDecoration_ShowObjectList(SafariDecoration *decor) {
    SafariDecoration_SetConfirmMode(decor, FALSE);
    SafariDecoration_MoveCursor(decor, decor->cursorPos);
    SafariDecoration_PrintObjectDescription(decor);
}

#pragma force_active on
// 0: can be placed, 1: no room for an object of its size, 2/3: needs (2) or doesn't need (3) water,
// 4: the area is full. 1-4 are shown as messages 4-7.
int SafariDecoration_CheckObjectPlaceable(SafariDecoration *decor, int index) {
    if (!decor->objectSlotsLeft) {
        return 4;
    }
    if (decor->args->isSurfing != decor->objects[index].config.isAnimated) {
        if (decor->args->isSurfing == FALSE) {
            return 2;
        }
        return 3;
    }
    if (decor->args->spots[decor->objects[index].sizeClass].valid == FALSE) {
        return 1;
    }
    return 0;
}

void SafariDecoration_StartPageScroll(SafariDecoration *decor, u8 dir, u32 *taskCount, u8 priority) {
    SafariDecorationScrollTask *task = Heap_Alloc(decor->heapId, sizeof(SafariDecorationScrollTask));

    MI_CpuFill8(task, 0, sizeof(SafariDecorationScrollTask));
    task->decor = decor;
    task->taskCount = taskCount;
    task->dir = dir;
    Sprite_SetAnimCtrlSeq(decor->sprites[dir + 2], dir * 3 + 6);
    PlaySE(SEQ_SE_DP_SELECT5);
    if (task->taskCount != NULL) {
        (*task->taskCount)++;
    }
    SysTask_CreateOnMainQueue(SafariDecoration_Task_PageScroll, task, priority);
}
#pragma force_active reset

void SafariDecoration_Task_PageScroll(SysTask *task, void *data) {
    SafariDecorationScrollTask *scroll = data;
    u8 timer = scroll->timer;

    scroll->timer = timer + 1;
    if (timer >= 2) {
        SafariDecoration_DrawObjectList(scroll->decor);
        SafariDecoration_PrintObjectDescription(scroll->decor);
        SafariDecoration_MoveCursor(scroll->decor, scroll->decor->cursorPos);
        (*scroll->taskCount)--;
        Heap_Free(data);
        SysTask_Destroy(task);
    }
}

void SafariDecoration_RequestModelUpdate(SafariDecoration *decor) {
    decor->modelUpdatePending = TRUE;
    SafariDecorationModelView_SetVisible(&decor->modelView, FALSE);
}

#pragma force_active on
SafariDecorationObject *SafariDecoration_CreateObjectList(u32 trainerId, int unlockLevel, u8 gender, u8 *numObjects, enum HeapID heapId) {
    u8 count = 0;
    u8 index;
    u32 mask = 0;
    int i;
    int j;
    SafariDecorationObject *objects;
    const u8 *groups = gSafariDecorationTrainerGroups[trainerId % 10];

    for (i = 0; i < unlockLevel; i++) {
        const SafariDecorationObjectGroup *group = &gSafariDecorationObjectGroups[groups[i]];
        for (j = 0; j < group->count; j++) {
            mask |= 1 << group->objects[j];
            count++;
        }
    }
    objects = Heap_Alloc(heapId, count * sizeof(SafariDecorationObject));
    MI_CpuFill8(objects, 0, count * sizeof(SafariDecorationObject));
    index = 0;
    for (i = 0; i < 24; i++) {
        if (mask & 1) {
            SafariDecorationObject *object = &objects[index];
            object->objectId = i;
            GetSafariObjectConfig(&object->config, i, gender);
            object->sizeClass = SafariDecoration_GetSizeClass(object->config.width, object->config.height);
            index++;
        }
        mask >>= 1;
    }
    *numObjects = index;
    return objects;
}
#pragma force_active reset

u8 SafariDecoration_GetSizeClass(u8 width, u8 height) {
    if (width == 1) {
        if (height != 1) {
            return 1;
        }
        return 0;
    }
    if (height == 1) {
        return 2;
    }
    return 3;
}
