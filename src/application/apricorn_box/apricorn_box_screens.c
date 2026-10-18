#include "application/apricorn_box/apricorn_box_screens.h"

#include "global.h"

#include "constants/items.h"
#include "constants/sndseq.h"

#include "application/apricorn_box/apricorn_box_screen_internal.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0021.h"

#include "bg_window.h"
#include "camera.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "list_menu_items.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "palette.h"
#include "particle_system.h"
#include "party.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_text.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

u8 ApricornBox_GetJuiceServings(SaveApricornBox *box);
int ApricornBox_GetMixProgressLevel(SaveApricornBox *box);
BOOL ApricornBox_QueueApricorn(SaveApricornBox *box, int apricorn, u32 value);
u32 ApricornBox_GetMixState(SaveApricornBox *apricornBox);
int ApricornJuice_GetTopMixIndex(void *data);

// Moves a sprite in a straight line to (x, y) (task).
typedef struct ApricornBoxMoveTask {
    ApricornBoxScreen *owner; // 0x00
    u8 state;                 // 0x04
    u8 frames;                // 0x05
    u8 frame;                 // 0x06
    u8 apricorn;              // 0x07
    s16 x;                    // 0x08
    s16 y;                    // 0x0A
    fx32 dx;                  // 0x0C
    fx32 dy;                  // 0x10
    fx32 curX;                // 0x14
    fx32 curY;                // 0x18
    Sprite *sprite;           // 0x1C
} ApricornBoxMoveTask;        // size: 0x20

typedef struct ApricornBoxListDesc {
    u16 count;
    u8 x;
    u8 y;
    const u8 *messages;
} ApricornBoxListDesc;

extern const u8 sApricornBoxQuantityDigitX[];
extern const TouchscreenListMenuTemplate sApricornBoxListMenuTemplates[2];
extern const ResdatIdList sApricornBoxSpriteResources;
extern const GraphicsModes sApricornBoxGraphicsModes;
extern const u8 sApricornBoxListMenu0Messages[3];
extern const u8 sApricornBoxListMenu1Messages[2];
extern const ApricornBoxListDesc sApricornBoxListMenus[2];
extern const OamCharTransferParam sApricornBoxOamCharTransferParam;
extern const BgTemplate sApricornBoxBgTemplate_Sub3;
extern const BgTemplate sApricornBoxBgTemplate_Main0;
extern const u16 sApricornBoxApricornPositions[7][2];
extern const BgTemplate sApricornBoxBgTemplate_Main3;
extern const BgTemplate sApricornBoxBgTemplate_Main1;
extern const BgTemplate sApricornBoxBgTemplate_Sub0;
extern const BgTemplate sApricornBoxBgTemplate_Main2;
extern const OamManagerParam sApricornBoxOamManagerParam;
extern const GraphicsBanks sApricornBoxGraphicsBanks;
extern const WindowTemplate sApricornBoxWindowTemplates[];
extern const UnmanagedSpriteTemplate sApricornBoxSpriteTemplates[7];

static void ApricornBoxScreen_InitFromArgs(ApricornBoxScreen *screen);
static void ApricornBoxScreen_RestoreMenuInputState(ApricornBoxScreen *screen);
static BOOL ApricornBoxScreen_Setup(ApricornBoxScreen *screen);
static BOOL ApricornBoxScreen_Teardown(ApricornBoxScreen *screen);
static int ApricornBoxScreen_FadeIn(ApricornBoxScreen *screen);
static int ApricornBoxScreen_FadeOut(ApricornBoxScreen *screen);
static int ApricornBoxScreen_ShowBlenderNews(ApricornBoxScreen *screen);
static int ApricornBoxScreen_ViewState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_BlenderState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_KurtState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_KurtQuantityState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_LiftApricornState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_DragApricornState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_PutBackApricornState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_BlendApricornState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_JuiceMenuState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_WaitMessageState(ApricornBoxScreen *screen);
static int ApricornBoxScreen_WaitJuiceTasteState(ApricornBoxScreen *screen);
static BOOL ApricornBoxScreen_IsMessageDone(ApricornBoxScreen *screen);
static void ApricornBoxScreen_ResetDisplay(void);
static void ApricornBoxScreen_InitGraphics(ApricornBoxScreen *screen);
static void ApricornBoxScreen_FreeGraphics(ApricornBoxScreen *screen);
static void ApricornBoxScreen_DrawScreen(ApricornBoxScreen *screen);
static void ApricornBoxScreen_LoadBoxState(ApricornBoxScreen *screen);
static void ApricornBoxScreen_SetCursorAnimActive(ApricornBoxScreen *screen, int active);
static void ApricornBoxScreen_DrawApricornCount(ApricornBoxScreen *screen, u8 index);
static void ApricornBoxScreen_ShowQuantityPanel(ApricornBoxScreen *screen, int show);
static void ApricornBoxScreen_PrintBlenderStatus(ApricornBoxScreen *screen);
static void ApricornBoxScreen_PrintHeader(ApricornBoxScreen *screen, int msgNo);
static void ApricornBoxScreen_PrintDescription(ApricornBoxScreen *screen);
static void ApricornBoxScreen_ClearMessage(ApricornBoxScreen *screen);
static void ApricornBoxScreen_PrintJuiceTaste(ApricornBoxScreen *screen);
static void ApricornBoxScreen_CreateJuiceMenu(ApricornBoxScreen *screen);
static int ApricornBoxScreen_HandleJuiceMenu(ApricornBoxScreen *screen);
static void ApricornBoxScreen_ShowYesNo(ApricornBoxScreen *screen);
static int ApricornBoxScreen_HandleYesNo(ApricornBoxScreen *screen);
static BOOL ApricornBoxScreen_AnimateHand(ApricornBoxScreen *screen, int active);
static inline int ApricornBoxScreen_Abs(int n);
static void ApricornBoxScreen_StartMoveTask(ApricornBoxScreen *screen, int index, int type);
static void ApricornBoxScreen_VBlankCB(ApricornBoxScreen *screen);
static void ApricornBoxScreen_SetGfxBanks(void);
static void ApricornBoxScreen_InitBgs(ApricornBoxScreen *screen);
static void ApricornBoxScreen_FreeBgs(ApricornBoxScreen *screen);
static void ApricornBoxScreen_LoadBgGraphics(ApricornBoxScreen *screen);
static void ApricornBoxScreen_FreeBgGraphics(ApricornBoxScreen *screen);
static void ApricornBoxScreen_InitStrings(ApricornBoxScreen *screen);
static void ApricornBoxScreen_FreeStrings(ApricornBoxScreen *screen);
static void ApricornBoxScreen_AddWindows(ApricornBoxScreen *screen);
static void ApricornBoxScreen_RemoveWindows(ApricornBoxScreen *screen);
static void ApricornBoxScreen_CreateListMenuItems(ApricornBoxScreen *screen);
static void ApricornBoxScreen_FreeListMenuItems(ApricornBoxScreen *screen);
static void ApricornBoxScreen_InitSpriteSystem(ApricornBoxScreen *screen);
static void ApricornBoxScreen_FreeSpriteSystem(ApricornBoxScreen *screen);
static void ApricornBoxScreen_InitSprites(ApricornBoxScreen *screen);
static void ApricornBoxScreen_FreeSprites(ApricornBoxScreen *screen);
static void ApricornBoxScreen_CreateSprites(ApricornBoxScreen *screen);
static void ApricornBoxScreen_DeleteSprites(ApricornBoxScreen *screen);
static void ApricornBoxScreen_MoveTask(SysTask *task, ApricornBoxMoveTask *move);
static void ApricornBoxScreen_ReadMessage(ApricornBoxScreen *screen, int msgNo);
static u16 ApricornBoxScreen_GetJuiceTasteMsg(ApricornBoxScreen *screen, ApricornJuiceEntry *entry, int line);

static u32 sApricornBoxConfirmButtonColors[8] = { MAKE_TEXT_COLOR(15, 3, 0), MAKE_TEXT_COLOR(15, 5, 0) };

BOOL ApricornBoxScreen_Init(OverlayManager *man, int *state) {
    ApricornBoxScreen *screen;

    switch (*state) {
    case 0:
        ApricornBoxScreen_ResetDisplay();
        Heap_Create(HEAP_ID_3, HEAP_ID_134, 2 << 16);
        screen = OverlayManager_CreateAndGetData(man, sizeof(ApricornBoxScreen), HEAP_ID_134);
        MI_CpuFill8(screen, 0, sizeof(ApricornBoxScreen));
        screen->heapId = HEAP_ID_134;
        screen->app = OverlayManager_GetArgs(man);
        ApricornBoxScreen_InitFromArgs(screen);
        (*state)++;
        break;
    case 1:
        screen = OverlayManager_GetData(man);
        if (ApricornBoxScreen_Setup(screen)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL ApricornBoxScreen_Main(OverlayManager *man, int *state) {
    ApricornBoxScreen *screen = OverlayManager_GetData(man);
    switch (*state) {
    case APRICORN_BOX_SCREEN_STATE_FADE_IN:
        *state = ApricornBoxScreen_FadeIn(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_FADE_OUT:
        *state = ApricornBoxScreen_FadeOut(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_BLENDER_NEWS:
        *state = ApricornBoxScreen_ShowBlenderNews(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_VIEW:
        *state = ApricornBoxScreen_ViewState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_BLENDER:
        *state = ApricornBoxScreen_BlenderState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_KURT:
        *state = ApricornBoxScreen_KurtState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_KURT_QUANTITY:
        *state = ApricornBoxScreen_KurtQuantityState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_LIFT_APRICORN:
        *state = ApricornBoxScreen_LiftApricornState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_DRAG_APRICORN:
        *state = ApricornBoxScreen_DragApricornState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_PUT_BACK_APRICORN:
        *state = ApricornBoxScreen_PutBackApricornState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_BLEND_APRICORN:
        *state = ApricornBoxScreen_BlendApricornState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_JUICE_MENU:
        *state = ApricornBoxScreen_JuiceMenuState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_WAIT_MESSAGE:
        *state = ApricornBoxScreen_WaitMessageState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_WAIT_JUICE_TASTE:
        *state = ApricornBoxScreen_WaitJuiceTasteState(screen);
        break;
    case APRICORN_BOX_SCREEN_STATE_EXIT:
        return TRUE;
    }
    SpriteSystem_DrawSprites(screen->spriteManager);
    return FALSE;
}

BOOL ApricornBoxScreen_Exit(OverlayManager *man, int *state) {
    ApricornBoxScreen *screen;
    screen = OverlayManager_GetData(man);
    if (ApricornBoxScreen_Teardown(screen) == FALSE) {
        return FALSE;
    }
    ApricornBoxScreen_RestoreMenuInputState(screen);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_134);
    return TRUE;
}

static void ApricornBoxScreen_InitFromArgs(ApricornBoxScreen *screen) {
    Options *options;

    screen->menuInputState = MenuInputStateMgr_GetState(screen->app->args->menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(screen->app->args->saveData);
    screen->textFrameDelay = Options_GetTextFrameDelay(options);
    screen->frame = Options_GetFrame(options);
    screen->mode = screen->app->mode;
    screen->steps = screen->app->args->steps;
    screen->apricornBox = Save_ApricornBox_Get(screen->app->args->saveData);
    ApricornBoxScreen_LoadBoxState(screen);
    if (screen->mode == APRICORN_BOX_MODE_USE_JUICE) {
        screen->cursorArea = 2;
        screen->cursorRow = 0;
        screen->lastRowLeft = screen->app->lastRows[0];
        screen->lastRowRight = screen->app->lastRows[1];
        screen->mode = APRICORN_BOX_MODE_BLENDER;
        screen->app->mode = APRICORN_BOX_MODE_BLENDER;
    }
}

static void ApricornBoxScreen_RestoreMenuInputState(ApricornBoxScreen *screen) {
    ApricornBoxApp *app = screen->app;
    ApricornBoxArgs *args = app->args;
    MenuInputStateMgr_SetState(args->menuInputStatePtr, screen->menuInputState);
}

static BOOL ApricornBoxScreen_Setup(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        ApricornBoxScreen_InitGraphics(screen);
        screen->subState++;
        break;
    case 1:
        ApricornBoxScreen_DrawScreen(screen);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(1);
        screen->subState = 0;
        return TRUE;
    }
    return FALSE;
}

static BOOL ApricornBoxScreen_Teardown(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        GF_TouchpadStopSampling();
        ApricornBoxScreen_FreeGraphics(screen);
        screen->subState++;
        break;
    case 1:
    default:
        ApricornBoxScreen_ResetDisplay();
        screen->subState = 0;
        return TRUE;
    }
    return FALSE;
}

static int ApricornBoxScreen_FadeIn(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, screen->heapId);
        screen->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            screen->subState = 0;
            switch (screen->mode) {
            case APRICORN_BOX_MODE_VIEW:
                break;
            case APRICORN_BOX_MODE_BLENDER:
                if (ApricornBox_GetMixState(screen->apricornBox) != 1) {
                    return APRICORN_BOX_SCREEN_STATE_BLENDER_NEWS;
                }
                return APRICORN_BOX_SCREEN_STATE_BLENDER;
            case APRICORN_BOX_MODE_KURT:
                return APRICORN_BOX_SCREEN_STATE_KURT;
            default:
                break;
            }
            return APRICORN_BOX_SCREEN_STATE_VIEW;
        }
        break;
    }
    return APRICORN_BOX_SCREEN_STATE_FADE_IN;
}

static int ApricornBoxScreen_FadeOut(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, screen->heapId);
        screen->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            screen->subState = 0;
            return APRICORN_BOX_SCREEN_STATE_EXIT;
        }
        break;
    }
    return APRICORN_BOX_SCREEN_STATE_FADE_OUT;
}

static int ApricornBoxScreen_ShowBlenderNews(ApricornBoxScreen *screen) {
    if (screen->subState == 0) {
        u32 kind;
        ApricornBoxScreen_SetApricornsFocus(screen, 1);
        kind = ApricornBox_GetMixState(screen->apricornBox);
        switch (kind) {
        case 0:
            ApricornBoxScreen_PrintMessage(screen, 22, screen->textFrameDelay);
            break;
        case 2:
            ApricornBoxScreen_PrintMessage(screen, 23, TEXT_SPEED_NOTRANSFER);
            break;
        case 3:
            ApricornBoxScreen_PrintMessage(screen, 24, TEXT_SPEED_NOTRANSFER);
            break;
        }
        ApricornBox_SetMixState(screen->apricornBox, 1);
        screen->subState++;
    } else if (ApricornBoxScreen_IsMessageDone(screen)) {
        ApricornBoxScreen_SetApricornsFocus(screen, 0);
        ApricornBoxScreen_ClearMessage(screen);
        screen->subState = 0;
        return APRICORN_BOX_SCREEN_STATE_BLENDER;
    }
    return APRICORN_BOX_SCREEN_STATE_BLENDER_NEWS;
}

static int ApricornBoxScreen_ViewState(ApricornBoxScreen *screen) {
    return ApricornBoxScreen_HandleViewInput(screen);
}

static int ApricornBoxScreen_BlenderState(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        ApricornBoxScreen_SetApricornsFocus(screen, 0);
        screen->subState++;
        break;
    case 1: {
        int result = ApricornBoxScreen_HandleBlenderInput(screen);
        if (result != 4) {
            screen->subState = 0;
        }
        return result;
    }
    }
    return APRICORN_BOX_SCREEN_STATE_BLENDER;
}

static int ApricornBoxScreen_KurtState(ApricornBoxScreen *screen) {
    return ApricornBoxScreen_HandleKurtInput(screen);
}

static int ApricornBoxScreen_KurtQuantityState(ApricornBoxScreen *screen) {
    int result;
    switch (screen->subState) {
    case 0:
        ApricornBoxScreen_SetApricornsFocus(screen, 2);
        ApricornBoxScreen_ShowQuantityPanel(screen, 1);
        ApricornBoxScreen_PrintHeader(screen, 11);
        screen->subState++;
        break;
    case 1:
        result = ApricornBoxScreen_HandleQuantityInput(screen);
        if (result == 6) {
            break;
        }
        screen->quantityInputResult = result;
        screen->timer = 4;
        screen->subState++;
        break;
    case 2:
        if (screen->timer-- != 0) {
            break;
        }
        if (screen->quantityInputResult == 1) {
            ApricornBoxScreen_SetApricornsFocus(screen, 0);
            ApricornBoxScreen_ShowQuantityPanel(screen, 0);
            ApricornBoxScreen_PrintHeader(screen, 10);
            screen->subState = 0;
            return APRICORN_BOX_SCREEN_STATE_KURT;
        }
        ApricornBoxScreen_PrintHeader(screen, 12);
        ApricornBoxScreen_ShowQuantityPanel(screen, 0);
        ApricornBoxScreen_ShowYesNo(screen);
        screen->subState++;
        break;
    case 3:
        result = ApricornBoxScreen_HandleYesNo(screen);
        if (result < 0) {
            break;
        }
        screen->subState = 0;
        if (result != 0) {
            ApricornBox_SetKurtApricorn(screen->apricornBox, screen->selectedApricorn, screen->quantity);
            return APRICORN_BOX_SCREEN_STATE_FADE_OUT;
        }
        ApricornBoxScreen_SetApricornsFocus(screen, 0);
        ApricornBoxScreen_PrintHeader(screen, 10);
        // fallthrough
    default:
        screen->subState = 0;
        return APRICORN_BOX_SCREEN_STATE_KURT;
    }
    return APRICORN_BOX_SCREEN_STATE_KURT_QUANTITY;
}

static int ApricornBoxScreen_LiftApricornState(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        ApricornBoxScreen_SetApricornsFocus(screen, 2);
        screen->subState++;
        break;
    case 1:
        if (!ApricornBoxScreen_AnimateHand(screen, 1)) {
            break;
        }
        ApricornBoxScreen_StartMoveTask(screen, 0, 0);
        ApricornBoxScreen_StartMoveTask(screen, 1, 0);
        screen->subState++;
        break;
    case 2:
        if (screen->activeTasks != 0) {
            break;
        }
        PlaySE(SEQ_SE_GS_BON01);
        Sprite_SetAnimCtrlSeq(screen->sprites[10], 21);
        screen->subState++;
        // fallthrough
    case 3:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            screen->subState = 0;
            PlaySE(SEQ_SE_DP_BOX01);
            return APRICORN_BOX_SCREEN_STATE_BLEND_APRICORN;
        }
        if (gSystem.newKeys & PAD_BUTTON_B) {
            screen->subState = 0;
            PlaySE(SEQ_SE_DP_HYUN2);
            return APRICORN_BOX_SCREEN_STATE_PUT_BACK_APRICORN;
        }
        if (System_GetTouchNew()) {
            screen->subState = 0;
            PlaySE(SEQ_SE_DP_HYUN2);
            if (screen->menuInputState != MENU_INPUT_STATE_TOUCH) {
                screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
            }
            return APRICORN_BOX_SCREEN_STATE_PUT_BACK_APRICORN;
        }
        break;
    default:
        screen->subState = 0;
        return APRICORN_BOX_SCREEN_STATE_BLENDER;
    }
    return APRICORN_BOX_SCREEN_STATE_LIFT_APRICORN;
}

static int ApricornBoxScreen_DragApricornState(ApricornBoxScreen *screen) {
    int result;
    switch (screen->subState) {
    case 0:
        ApricornBoxScreen_SetApricornsFocus(screen, 2);
        screen->subState++;
        break;
    case 1:
        if (ApricornBoxScreen_AnimateHand(screen, 1)) {
            screen->subState++;
        }
        break;
    case 2:
        result = ApricornBoxScreen_HandleDrag(screen);
        switch (result) {
        case 0:
            screen->subState = 0;
            return APRICORN_BOX_SCREEN_STATE_PUT_BACK_APRICORN;
        case 1:
            screen->subState = 0;
            return APRICORN_BOX_SCREEN_STATE_BLEND_APRICORN;
        default:
            break;
        }
        break;
    }
    return APRICORN_BOX_SCREEN_STATE_DRAG_APRICORN;
}

static int ApricornBoxScreen_PutBackApricornState(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        ApricornBoxScreen_StartMoveTask(screen, 0, 1);
        ApricornBoxScreen_StartMoveTask(screen, 1, 1);
        Sprite_SetAnimCtrlSeq(screen->sprites[10], 20);
        screen->subState++;
        break;
    case 1:
        if (screen->activeTasks == 0) {
            screen->subState++;
        }
        break;
    case 2:
        if (!ApricornBoxScreen_AnimateHand(screen, 0)) {
            break;
        }
        // fallthrough
    default:
        ApricornBoxScreen_SetApricornsFocus(screen, 0);
        screen->subState = 0;
        return APRICORN_BOX_SCREEN_STATE_BLENDER;
    }
    return APRICORN_BOX_SCREEN_STATE_PUT_BACK_APRICORN;
}

static int ApricornBoxScreen_BlendApricornState(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        if (screen->numBlending >= 5) {
            ApricornBoxScreen_PrintHeader(screen, 7);
            ApricornBoxScreen_PrintMessage(screen, 26, TEXT_SPEED_NOTRANSFER);
            screen->subState = 1;
        } else {
            screen->cursorArea = 2;
            screen->cursorRow = 0;
            screen->subState = 2;
        }
        break;
    case 1:
        if (ApricornBoxScreen_IsMessageDone(screen)) {
            ApricornBoxScreen_ClearMessage(screen);
            screen->subState = 0;
            return APRICORN_BOX_SCREEN_STATE_PUT_BACK_APRICORN;
        }
        break;
    case 2:
        if (!ApricornBoxScreen_AnimateHand(screen, 0)) {
            break;
        }
        Sprite_SetDrawFlag(screen->sprites[screen->selectedApricorn + 2], FALSE);
        Sprite_SetAnimCtrlSeq(screen->sprites[10], 22);
        Sprite_ResetAnimCtrlState(screen->sprites[10]);
        screen->subState++;
        break;
    case 3:
        if (Sprite_IsAnimated(screen->sprites[10])) {
            break;
        }
        PlaySE(SEQ_SE_GS_BON01);
        Sprite_SetAnimCtrlSeq(screen->sprites[10], 20);
        ApricornBox_QueueApricorn(screen->apricornBox, screen->selectedApricorn, screen->steps);
        ApricornBoxScreen_LoadBoxState(screen);
        ApricornBoxScreen_StartMoveTask(screen, 1, 0);
        screen->subState++;
        break;
    case 4:
        if (screen->activeTasks != 0) {
            break;
        }
        if (screen->numBlending >= 5) {
            ApricornBoxScreen_PrintHeader(screen, 7);
        } else {
            ApricornBoxScreen_PrintHeader(screen, 6);
        }
        ApricornBoxScreen_DrawApricornCount(screen, screen->selectedApricorn);
        ApricornBoxScreen_SetApricornsFocus(screen, 0);
        screen->subState = 0;
        return APRICORN_BOX_SCREEN_STATE_BLENDER;
    }
    return APRICORN_BOX_SCREEN_STATE_BLEND_APRICORN;
}

static int ApricornBoxScreen_JuiceMenuState(ApricornBoxScreen *screen) {
    int result;
    switch (screen->subState) {
    case 0:
        ApricornBoxScreen_CreateJuiceMenu(screen);
        ApricornBoxScreen_PrintHeader(screen, 8);
        ApricornBoxScreen_SetApricornsFocus(screen, 1);
        screen->subState++;
        break;
    case 1:
        result = ApricornBoxScreen_HandleJuiceMenu(screen);
        if (result == -1) {
            break;
        }
        screen->subState = 0;
        if ((u32)result >= 2 || result == -2) {
            ApricornBoxScreen_PrintBlenderStatus(screen);
            return APRICORN_BOX_SCREEN_STATE_BLENDER;
        }
        if (result == 1) {
            ApricornBoxScreen_PrintHeader(screen, 9);
            ApricornBoxScreen_PrintJuiceTaste(screen);
            ApricornBoxScreen_SetApricornsFocus(screen, 1);
            return APRICORN_BOX_SCREEN_STATE_WAIT_JUICE_TASTE;
        }
        screen->app->mode = APRICORN_BOX_MODE_USE_JUICE;
        screen->app->partySlot = 0;
        screen->app->lastRows[0] = screen->lastRowLeft;
        screen->app->lastRows[1] = screen->lastRowRight;
        return APRICORN_BOX_SCREEN_STATE_FADE_OUT;
    }
    return APRICORN_BOX_SCREEN_STATE_JUICE_MENU;
}

static int ApricornBoxScreen_WaitMessageState(ApricornBoxScreen *screen) {
    return ApricornBoxScreen_IsMessageDone(screen) ? APRICORN_BOX_SCREEN_STATE_BLENDER : APRICORN_BOX_SCREEN_STATE_WAIT_MESSAGE;
}

static int ApricornBoxScreen_WaitJuiceTasteState(ApricornBoxScreen *screen) {
    if (ApricornBoxScreen_IsMessageDone(screen)) {
        ApricornBoxScreen_ClearMessage(screen);
        ApricornBoxScreen_PrintBlenderStatus(screen);
        return APRICORN_BOX_SCREEN_STATE_BLENDER;
    }
    return APRICORN_BOX_SCREEN_STATE_WAIT_JUICE_TASTE;
}

static BOOL ApricornBoxScreen_IsMessageDone(ApricornBoxScreen *screen) {
    if (screen->textPrinterId != 0xFF && TextPrinterCheckActive(screen->textPrinterId)) {
        return FALSE;
    }
    if (System_GetTouchNew()) {
        PlaySE(SEQ_SE_DP_SELECT);
        screen->menuInputState = MENU_INPUT_STATE_TOUCH;
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        screen->textPrinterId = 0xFF;
        return TRUE;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_KEY_UP | PAD_KEY_DOWN | PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        PlaySE(SEQ_SE_DP_SELECT);
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        screen->textPrinterId = 0xFF;
        return TRUE;
    }
    return FALSE;
}

static void ApricornBoxScreen_ResetDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}

static void ApricornBoxScreen_InitGraphics(ApricornBoxScreen *screen) {
    ApricornBoxScreen_InitBgs(screen);
    ApricornBoxScreen_LoadBgGraphics(screen);
    ApricornBoxScreen_InitStrings(screen);
    ApricornBoxScreen_AddWindows(screen);
    ApricornBoxScreen_CreateListMenuItems(screen);
    ApricornBoxScreen_InitSprites(screen);
    Main_SetVBlankIntrCB((GFIntrCB)ApricornBoxScreen_VBlankCB, screen);
}

static void ApricornBoxScreen_FreeGraphics(ApricornBoxScreen *screen) {
    ApricornBoxScreen_FreeSprites(screen);
    ApricornBoxScreen_FreeListMenuItems(screen);
    ApricornBoxScreen_RemoveWindows(screen);
    ApricornBoxScreen_FreeStrings(screen);
    ApricornBoxScreen_FreeBgGraphics(screen);
    ApricornBoxScreen_FreeBgs(screen);
}

static void ApricornBoxScreen_DrawScreen(ApricornBoxScreen *screen) {
    int i;
    int width;
    int x;
    u8 mode;
    ApricornBoxScreen_DrawCloseButton(screen, 0);
    ApricornBoxScreen_UpdateCursor(screen);
    ReadMsgDataIntoString(screen->msgData, msg_0021_00000, screen->string);
    width = (u8)FontID_String_GetWidth(0, screen->string, 0);
    x = (80 - width) / 2;
    AddTextPrinterParameterizedWithColor(&screen->windows[3], 0, screen->string, x, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    for (i = 0; i < 7; i++) {
        ApricornBoxScreen_DrawApricornCount(screen, i);
    }
    mode = screen->mode;
    switch (mode) {
    case APRICORN_BOX_MODE_BLENDER:
        ApricornBoxScreen_PrintBlenderStatus(screen);
        return;
    case APRICORN_BOX_MODE_KURT:
        ApricornBoxScreen_PrintHeader(screen, 10);
        return;
    case APRICORN_BOX_MODE_VIEW:
    default:
        break;
    }
    ApricornBoxScreen_PrintHeader(screen, 0);
}

static void ApricornBoxScreen_LoadBoxState(ApricornBoxScreen *screen) {
    int i;
    screen->numBlending = ApricornBox_CountQueuedApricorns(screen->apricornBox);
    screen->juiceServings = ApricornBox_GetJuiceServings(screen->apricornBox);
    ApricornBox_GetJuice(screen->apricornBox, 8, (ApricornJuice *)screen->juiceRecord);
    screen->juiceMildness = screen->juiceRecord[2];
    screen->topMixIndex = ApricornJuice_GetTopMixIndex(screen->juiceRecord);
    screen->flavorTotal = ApricornJuice_GetFlavorTotal((ApricornJuice *)screen->juiceRecord);
    ApricornJuice_RankFlavors(screen->juiceRecord, &screen->juice);
    screen->mixStage = ApricornBox_GetMixProgressLevel(screen->apricornBox);
    for (i = 0; i < 7; i++) {
        screen->apricornCounts[i] = ApricornBox_CountApricorn(screen->apricornBox, i);
    }
}

void ApricornBoxScreen_DrawCloseButton(ApricornBoxScreen *screen, int sequence) {
    u32 width;
    Sprite_SetAnimCtrlSeq(screen->sprites[0], sequence);
    Sprite_ResetAnimCtrlState(screen->sprites[0]);
    FillWindowPixelBuffer(&screen->windows[2], 0);
    width = FontID_String_GetWidth(4, screen->closeText, 0);
    AddTextPrinterParameterizedWithColor(&screen->windows[2], 4, screen->closeText, (48 - width) >> 1, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&screen->windows[2]);
}

void ApricornBoxScreen_DrawConfirmButton(ApricornBoxScreen *screen, int index, int sequence) {
    struct {
        int value;
    } width;

    Window *window = &screen->windows[index + 6];
    FillWindowPixelBuffer(window, 0);
    ReadMsgDataIntoString(screen->msgData, index + msg_0021_00005, screen->string2);
    width.value = FontID_String_GetWidth(4, screen->string2, 0);
    AddTextPrinterParameterizedWithColor(window, 4, screen->string2, (48 - width.value) / 2, 0, TEXT_SPEED_NOTRANSFER, sApricornBoxConfirmButtonColors[index], NULL);
    ScheduleWindowCopyToVram(window);
    Sprite_SetAnimCtrlSeq(screen->sprites[index + 9], sequence + (index * 2 + 28));
    Sprite_ResetAnimCtrlState(screen->sprites[index + 9]);
}

void ApricornBoxScreen_UpdateCursor(ApricornBoxScreen *screen) {
    int index = ApricornBoxScreen_GetCursorTarget(screen);
    s16 x;
    s16 y;
    switch (index) {
    case 7:
        x = 200;
        y = 92;
        ApricornBoxScreen_PrintBlenderStatus(screen);
        break;
    case 8:
        x = 240;
        y = 168;
        break;
    default:
        x = sApricornBoxApricornPositions[index][0] + 4;
        y = sApricornBoxApricornPositions[index][1] - 16;
        break;
    }
    Sprite_SetPositionXY(screen->sprites[1], x, y);
    ApricornBoxScreen_PrintDescription(screen);
}

static void ApricornBoxScreen_SetCursorAnimActive(ApricornBoxScreen *screen, int active) {
    Sprite_SetAnimActiveFlag(screen->sprites[1], active);
}

static void ApricornBoxScreen_DrawApricornCount(ApricornBoxScreen *screen, u8 index) {
    u8 *counts;
    int windowOffset;
    u8 *windowBase;
    Sprite **spriteSlot;
    u8 width;
    windowBase = (u8 *)screen->windows;
    windowOffset = (index + 11) * sizeof(Window);
    counts = screen->apricornCounts;
    if (counts[index] == 0) {
        spriteSlot = &screen->sprites[index + 2];
        Sprite_SetDrawFlag(*spriteSlot, FALSE);
        ClearWindowTilemapAndScheduleTransfer((Window *)(windowBase + windowOffset));
    } else {
        spriteSlot = &screen->sprites[index + 2];
        Sprite_SetDrawFlag(*spriteSlot, TRUE);
        FillWindowPixelBuffer((Window *)(windowBase + windowOffset), 0);
        BufferIntegerAsString(screen->msgFormat, 0, counts[index], 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        StringExpandPlaceholders(screen->msgFormat, screen->string, screen->numberTemplate);
        width = FontID_String_GetWidth(0, screen->string, 0);
        AddTextPrinterParameterizedWithColor((Window *)(windowBase + windowOffset), 0, screen->string, (16 - width) / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram((Window *)(windowBase + windowOffset));
    }
    Sprite_SetPositionXY(*spriteSlot, sApricornBoxApricornPositions[index][0], sApricornBoxApricornPositions[index][1]);
}

void ApricornBoxScreen_PrintQuantity(ApricornBoxScreen *screen, int value) {
    u8 digits[2];
    int i;
    digits[0] = value % 10;
    digits[1] = value / 10;
    for (i = 0; i < 2; i++) {
        BufferIntegerAsString(screen->msgFormat, 0, digits[i], 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(screen->msgFormat, screen->string, screen->numberTemplate);
        FillWindowPixelBuffer(&screen->windows[i + 8], 0);
        AddTextPrinterParameterizedWithColor(&screen->windows[i + 8], 0, screen->string, sApricornBoxQuantityDigitX[i], 3, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&screen->windows[i + 8]);
        if (screen->apricornCounts[screen->selectedApricorn] < 10) {
            break;
        }
    }
}

static void ApricornBoxScreen_ShowQuantityPanel(ApricornBoxScreen *screen, int show) {
    int i;
    if (show) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(screen->narc, 6, screen->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, screen->heapId);
        ApricornBoxScreen_PrintQuantity(screen, screen->quantity);
        ApricornBoxScreen_DrawConfirmButton(screen, 0, 0);
        ApricornBoxScreen_DrawConfirmButton(screen, 1, 0);
        FillWindowPixelBuffer(&screen->windows[5], 0);
        AddTextPrinterParameterizedWithColor(&screen->windows[5], 0, screen->apricornNames[screen->selectedApricorn], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&screen->windows[5]);
        ReadMsgDataIntoString(screen->msgData, msg_0021_00026, screen->string2);
        FillWindowPixelBuffer(&screen->windows[10], 0);
        AddTextPrinterParameterizedWithColor(&screen->windows[10], 0, screen->string2, 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&screen->windows[10]);
    } else {
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, 17);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, 32, 24, 17);
        ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_1);
        ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_0);
    }
    for (i = 9; i <= 14; i++) {
        Sprite_SetDrawFlag(screen->sprites[i], show);
        Sprite_SetAnimActiveFlag(screen->sprites[i], FALSE);
    }
    if (screen->apricornCounts[screen->selectedApricorn] < 10) {
        Sprite_SetAnimCtrlSeq(screen->sprites[13], 25);
        Sprite_SetAnimCtrlSeq(screen->sprites[14], 27);
    } else {
        Sprite_SetAnimCtrlSeq(screen->sprites[13], 24);
        Sprite_SetAnimCtrlSeq(screen->sprites[14], 26);
    }
}

static void ApricornBoxScreen_PrintBlenderStatus(ApricornBoxScreen *screen) {
    if (screen->numBlending == 0) {
        ApricornBoxScreen_PrintHeader(screen, 1);
    } else {
        ApricornBoxScreen_PrintHeader(screen, screen->mixStage + 1);
    }
}

static void ApricornBoxScreen_PrintHeader(ApricornBoxScreen *screen, int msgNo) {
    Window *window = &screen->windows[1];
    FillWindowPixelBuffer(window, 0);
    ApricornBoxScreen_ReadMessage(screen, msgNo);
    AddTextPrinterParameterized(window, 0, screen->string2, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(window);
}

static void ApricornBoxScreen_PrintDescription(ApricornBoxScreen *screen) {
    Window *window = &screen->windows[4];
    u32 selection;
    FillWindowPixelBuffer(window, 15);
    selection = ApricornBoxScreen_GetCursorTarget(screen);
    if (selection == 8) {
        ScheduleWindowCopyToVram(window);
        return;
    }
    if (selection <= 6) {
        if (screen->apricornCounts[selection] == 0) {
            ScheduleWindowCopyToVram(window);
            return;
        }
        ApricornBoxScreen_ReadMessage(screen, selection + 13);
    } else if (screen->juiceServings != 0) {
        ApricornBoxScreen_ReadMessage(screen, 21);
    } else {
        ApricornBoxScreen_ReadMessage(screen, 20);
    }
    AddTextPrinterParameterized(window, 0, screen->string2, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(window);
}

void ApricornBoxScreen_PrintMessage(ApricornBoxScreen *screen, int msgNo, int speed) {
    Window *window = &screen->windows[0];
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, TRUE, 1, 13);
    ApricornBoxScreen_ReadMessage(screen, msgNo);
    if (speed != TEXT_SPEED_NOTRANSFER && speed != TEXT_SPEED_INSTANT) {
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        screen->textPrinterId = AddTextPrinterParameterized(window, 1, screen->string2, 0, 0, speed, NULL);
    } else {
        AddTextPrinterParameterized(window, 1, screen->string2, 0, 0, speed, NULL);
        screen->textPrinterId = 0xFF;
    }
    ScheduleWindowCopyToVram(window);
}

static void ApricornBoxScreen_ClearMessage(ApricornBoxScreen *screen) {
    ClearFrameAndWindow2(&screen->windows[0], TRUE);
    ClearWindowTilemapAndScheduleTransfer(&screen->windows[0]);
}

static void ApricornBoxScreen_PrintJuiceTaste(ApricornBoxScreen *screen) {
    Window *window = &screen->windows[0];
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, TRUE, 1, 13);
    {
        int msgNo = ApricornBoxScreen_GetJuiceTasteMsg(screen, &screen->juice.entries[0], 0);
        ReadMsgDataIntoString(screen->msgData, msgNo, screen->string2);
    }
    AddTextPrinterParameterized(window, 1, screen->string2, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    if (screen->juice.entries[0].stat == 5) {
        ScheduleWindowCopyToVram(window);
        return;
    }
    if (screen->juice.numEntries > 1) {
        {
            int msgNo = ApricornBoxScreen_GetJuiceTasteMsg(screen, &screen->juice.entries[1], 1);
            ReadMsgDataIntoString(screen->msgData, msgNo, screen->string2);
        }
        AddTextPrinterParameterized(window, 1, screen->string2, 0, 16, TEXT_SPEED_NOTRANSFER, NULL);
    }
    if (screen->juice.numEntries > 2) {
        {
            int msgNo = ApricornBoxScreen_GetJuiceTasteMsg(screen, &screen->juice.entries[2], 2);
            ReadMsgDataIntoString(screen->msgData, msgNo, screen->string2);
        }
        AddTextPrinterParameterized(window, 1, screen->string2, 0, 32, TEXT_SPEED_NOTRANSFER, NULL);
    } else if ((u8)(screen->juice.numEntries + 255) <= 1) {
        ReadMsgDataIntoString(screen->msgData, msg_0021_00122, screen->string2);
        AddTextPrinterParameterized(window, 1, screen->string2, 0, (screen->juice.numEntries - 1) * 16 + 16, TEXT_SPEED_NOTRANSFER, NULL);
    }
    ScheduleWindowCopyToVram(window);
}

static void ApricornBoxScreen_CreateJuiceMenu(ApricornBoxScreen *screen) {
    TouchscreenListMenuHeader header;
    u8 y;
    u8 x;
    MI_CpuFill8(&header, 0, sizeof(header));
    header.template = sApricornBoxListMenuTemplates[1];
    header.listMenuItems = screen->listMenuItems;
    header.bgConfig = screen->bgConfig;
    header.numWindows = sApricornBoxListMenus[screen->mode - 1].count;
    y = sApricornBoxListMenus[screen->mode - 1].y;
    x = sApricornBoxListMenus[screen->mode - 1].x;
    screen->listMenu = TouchscreenListMenu_Create(screen->spawner, &header, (u8)screen->menuInputState, x, y, 8, 0);
}

static int ApricornBoxScreen_HandleJuiceMenu(ApricornBoxScreen *screen) {
    int result = TouchscreenListMenu_HandleInput(screen->listMenu);
    if (result == -1) {
        return -1;
    }
    screen->menuInputState = (MenuInputState)TouchscreenListMenu_WasLastInputTouch(screen->listMenu);
    TouchscreenListMenu_Destroy(screen->listMenu);
    return result;
}

static void ApricornBoxScreen_ShowYesNo(ApricornBoxScreen *screen) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = screen->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = 0x6A;
    template.plttSlot = 14;
    template.x = 25;
    template.y = 5;
    template.ignoreTouchFlag = screen->menuInputState;
    template.initialCursorPos = 1;
    YesNoPrompt_InitFromTemplate(screen->yesNoPrompt, &template);
}

static int ApricornBoxScreen_HandleYesNo(ApricornBoxScreen *screen) {
    int result = YesNoPrompt_HandleInput(screen->yesNoPrompt);
    int answer;
    switch (result) {
    case YESNORESPONSE_YES:
        answer = 1;
        break;
    case YESNORESPONSE_NO:
        answer = 0;
        break;
    default:
        return -1;
    }
    screen->menuInputState = YesNoPrompt_IsInTouchMode(screen->yesNoPrompt);
    YesNoPrompt_Reset(screen->yesNoPrompt);
    return answer;
}

void ApricornBoxScreen_SetApricornsFocus(ApricornBoxScreen *screen, int mode) {
    int i;
    if (mode == 1 || mode == 2) {
        for (i = 0; i < 7; i++) {
            if (i == screen->selectedApricorn && mode == 2) {
                Sprite_SetOamMode(screen->sprites[i + 2], GX_OAM_MODE_NORMAL);
            } else {
                Sprite_SetOamMode(screen->sprites[i + 2], GX_OAM_MODE_XLU);
            }
        }
        ApricornBoxScreen_SetCursorAnimActive(screen, 0);
        ApricornBoxScreen_DrawCloseButton(screen, 2);
        if (screen->selectedApricorn < 7) {
            Sprite_SetDrawPriority(screen->sprites[screen->selectedApricorn + 2], 2);
        }
    } else {
        for (i = 0; i < 7; i++) {
            Sprite_SetOamMode(screen->sprites[i + 2], GX_OAM_MODE_NORMAL);
        }
        ApricornBoxScreen_SetCursorAnimActive(screen, 1);
        ApricornBoxScreen_DrawCloseButton(screen, 0);
        if (screen->selectedApricorn < 7) {
            Sprite_SetDrawPriority(screen->sprites[screen->selectedApricorn + 2], 3);
        }
    }
}

static BOOL ApricornBoxScreen_AnimateHand(ApricornBoxScreen *screen, int active) {
    if (active) {
        switch (screen->animState) {
        case 0:
            Sprite_SetAnimCtrlSeq(screen->sprites[1], 4);
            screen->timer = 0;
            screen->animState++;
            break;
        case 1: {
            u8 old;
            Sprite_OffsetPositionXY(screen->sprites[1], 0, 2);
            Sprite_OffsetPositionXY(screen->sprites[screen->selectedApricorn + 2], 0, -1);
            old = screen->timer;
            screen->timer = screen->timer + 1;
            if (old >= 2) {
                Sprite_SetAnimCtrlSeq(screen->sprites[1], 5);
                screen->timer = 0;
                screen->animState = 0;
                return TRUE;
            }
            break;
        }
        }
    } else {
        switch (screen->animState) {
        case 0:
            Sprite_SetAnimCtrlSeq(screen->sprites[1], 4);
            screen->timer = 0;
            screen->animState++;
            break;
        case 1: {
            u8 old;
            Sprite_OffsetPositionXY(screen->sprites[1], 0, -2);
            old = screen->timer;
            screen->timer = screen->timer + 1;
            if (old >= 3) {
                Sprite_SetAnimCtrlSeq(screen->sprites[1], 3);
                screen->timer = 0;
                screen->animState = 0;
                return TRUE;
            }
            break;
        }
        }
    }
    return FALSE;
}

static inline int ApricornBoxScreen_Abs(int n) {
    return n < 0 ? -n : n;
}

static void ApricornBoxScreen_StartMoveTask(ApricornBoxScreen *screen, int index, int type) {
    ApricornBoxMoveTask *move = Heap_Alloc(screen->heapId, sizeof(ApricornBoxMoveTask));
    s16 x;
    s16 y;
    s16 dx;
    s16 dy;
    float frames;
    u16 base;
    MI_CpuFill8(move, 0, sizeof(ApricornBoxMoveTask));
    move->owner = screen;
    move->apricorn = screen->selectedApricorn;
    if (index == 0) {
        move->sprite = screen->sprites[screen->selectedApricorn + 2];
    } else {
        move->sprite = screen->sprites[1];
    }
    Sprite_GetPositionXY(move->sprite, &x, &y);
    if (type == 0) {
        move->x = 192 + index * 4;
        move->y = 96 + index * -10;
    } else {
        base = sApricornBoxApricornPositions[move->apricorn][0];
        move->x = base + index * 4;
        base = sApricornBoxApricornPositions[move->apricorn][1];
        move->y = base + index * -10;
    }
    dx = move->x - x;
    dy = move->y - y;
    if (ApricornBoxScreen_Abs(dx) >= ApricornBoxScreen_Abs(dy)) {
        move->frames = ApricornBoxScreen_Abs(dx) / 8;
    } else {
        move->frames = ApricornBoxScreen_Abs(dy) / 8;
    }
    move->curX = (int)(x > 0 ? 0.5f + (float)(x << FX32_SHIFT) : (float)(x << FX32_SHIFT) - 0.5f);
    move->curY = (int)(y > 0 ? 0.5f + (float)(y << FX32_SHIFT) : (float)(y << FX32_SHIFT) - 0.5f);
    move->frame = 0;
    if (move->frames > 0) {
        frames = 0.5f + (float)(move->frames << FX32_SHIFT);
    } else {
        frames = (float)(move->frames << FX32_SHIFT) - 0.5f;
    }
    move->dx = FX_Div(dx > 0 ? 0.5f + (float)(dx << FX32_SHIFT) : (float)(dx << FX32_SHIFT) - 0.5f, (int)frames);
    if (move->frames > 0) {
        frames = 0.5f + (float)(move->frames << FX32_SHIFT);
    } else {
        frames = (float)(move->frames << FX32_SHIFT) - 0.5f;
    }
    move->dy = FX_Div(dy > 0 ? 0.5f + (float)(dy << FX32_SHIFT) : (float)(dy << FX32_SHIFT) - 0.5f, (int)frames);
    SysTask_CreateOnMainQueue((SysTaskFunc)ApricornBoxScreen_MoveTask, move, 0);
    screen->activeTasks++;
}

static void ApricornBoxScreen_VBlankCB(ApricornBoxScreen *screen) {
    if (screen->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(screen->bgConfig);
    ((vu32 *)0x027E0000)[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

static void ApricornBoxScreen_SetGfxBanks(void) {
    GraphicsBanks banks = sApricornBoxGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void ApricornBoxScreen_InitBgs(ApricornBoxScreen *screen) {
    ApricornBoxScreen_SetGfxBanks();
    screen->bgConfig = BgConfig_Alloc(screen->heapId);
    {
        GraphicsModes modes = sApricornBoxGraphicsModes;
        SetBothScreensModesAndDisable(&modes);
    }
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    {
        BgTemplate template = sApricornBoxBgTemplate_Sub0;
        InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_SUB_0);
    {
        BgTemplate template = sApricornBoxBgTemplate_Sub3;
        InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_SUB_3, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_SUB_3);
    {
        BgTemplate template = sApricornBoxBgTemplate_Main0;
        InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_0, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_0);
    {
        BgTemplate template = sApricornBoxBgTemplate_Main1;
        InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_1);
    {
        BgTemplate template = sApricornBoxBgTemplate_Main2;
        InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_2);
    {
        BgTemplate template = sApricornBoxBgTemplate_Main3;
        InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_3, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x40, 0, screen->heapId);
}

static void ApricornBoxScreen_FreeBgs(ApricornBoxScreen *screen) {
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_SUB_0);
    Heap_Free(screen->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}

static void ApricornBoxScreen_LoadBgGraphics(ApricornBoxScreen *screen) {
    NARC *narc = NARC_New(NARC_a_2_1_8, screen->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, screen->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, screen->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, screen->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, screen->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, screen->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, screen->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, screen->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, screen->heapId);
    screen->narc = narc;
    LoadUserFrameGfx2(screen->bgConfig, GF_BG_LYR_MAIN_0, 1, 13, screen->frame, screen->heapId);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, screen->heapId);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, screen->heapId);
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG3, 6, 26);
}

static void ApricornBoxScreen_FreeBgGraphics(ApricornBoxScreen *screen) {
    NARC_Delete(screen->narc);
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 0);
}

static void ApricornBoxScreen_InitStrings(ApricornBoxScreen *screen) {
    int i;
    int j;
    FontID_Alloc(4, screen->heapId);
    screen->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0021_bin, screen->heapId);
    screen->msgFormat = MessageFormat_New_Custom(16, 32, screen->heapId);
    screen->string = String_New(256, screen->heapId);
    screen->string2 = String_New(256, screen->heapId);
    screen->closeText = NewString_ReadMsgData(screen->msgData, msg_0021_00001);
    screen->numberTemplate = NewString_ReadMsgData(screen->msgData, msg_0021_00025);
    for (i = 0; i < 22; i++) {
        screen->infoTexts[i] = NewString_ReadMsgData(screen->msgData, i + msg_0021_00027);
    }
    for (j = 0; j < 7; j++) {
        screen->apricornNames[j] = NewString_ReadMsgData(screen->msgData, j + msg_0021_00007);
    }
}

static void ApricornBoxScreen_FreeStrings(ApricornBoxScreen *screen) {
    int i;
    for (i = 0; i < 7; i++) {
        String_Delete(screen->apricornNames[i]);
    }
    {
        u8 *cursor;
        int j;
        j = 0;
        cursor = (u8 *)screen;
        for (; j < 22; j++, cursor += sizeof(String *)) {
            String_Delete(((ApricornBoxScreen *)cursor)->infoTexts[0]);
        }
    }
    String_Delete(screen->numberTemplate);
    String_Delete(screen->closeText);
    String_Delete(screen->string2);
    String_Delete(screen->string);
    MessageFormat_Delete(screen->msgFormat);
    DestroyMsgData(screen->msgData);
    FontID_Release(4);
}

static void ApricornBoxScreen_AddWindows(ApricornBoxScreen *screen) {
    int i = 0;
    int tile;
    int y;
    const WindowTemplate *template = sApricornBoxWindowTemplates;
    Window *window = &screen->windows[0];
    for (; i < 11; i++) {
        AddWindow(screen->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        template ++;
        window++;
    }
    tile = 444;
    i = 0;
    y = 6;
    for (; i < 7; i++) {
        AddWindowParameterized(screen->bgConfig, &screen->windows[i + 11], GF_BG_LYR_MAIN_2, (u8)((i % 2) * 6 + 5), (u8)y, 2, 2, 12, (u16)tile);
        FillWindowPixelBuffer(&screen->windows[i + 11], 0);
        tile += 4;
        y += 2;
    }
    screen->yesNoPrompt = YesNoPrompt_Create(screen->heapId);
}

static void ApricornBoxScreen_RemoveWindows(ApricornBoxScreen *screen) {
    int i = 0;
    Window *window = &screen->windows[0];
    for (; i < 18; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
    YesNoPrompt_Destroy(screen->yesNoPrompt);
}

static void ApricornBoxScreen_CreateListMenuItems(ApricornBoxScreen *screen) {
    int i;
    if (screen->mode != APRICORN_BOX_MODE_VIEW) {
        int offset = (screen->mode - 1) * 8;
        const ApricornBoxListDesc *desc = (const ApricornBoxListDesc *)((const u8 *)sApricornBoxListMenus + offset);
        screen->listMenuItems = ListMenuItems_New(*(const u16 *)((const u8 *)sApricornBoxListMenus + offset), screen->heapId);
        for (i = 0; i < desc->count; i++) {
            ListMenuItems_AppendFromMsgData(screen->listMenuItems, screen->msgData, desc->messages[i], i);
        }
    }
}

static void ApricornBoxScreen_FreeListMenuItems(ApricornBoxScreen *screen) {
    if (screen->mode) {
        ListMenuItems_Delete(screen->listMenuItems);
        screen->listMenuItems = NULL;
    }
}

static void ApricornBoxScreen_InitSpriteSystem(ApricornBoxScreen *screen) {
    GF_CreateVramTransferManager(32, screen->heapId);
    screen->spriteSystem = SpriteSystem_Alloc(screen->heapId);
    SpriteSystem_Init(screen->spriteSystem, &sApricornBoxOamManagerParam, &sApricornBoxOamCharTransferParam, 3);
    thunk_ClearMainOAM(screen->heapId);
    thunk_ClearSubOAM(screen->heapId);
    screen->spriteManager = SpriteManager_New(screen->spriteSystem);
    SpriteSystem_InitSprites(screen->spriteSystem, screen->spriteManager, 15);
    SpriteSystem_LoadResourceDataFromFileIdListEx(screen->spriteSystem, screen->spriteManager, &sApricornBoxSpriteResources, 0, 0);
    screen->spawner = TouchscreenListMenuSpawner_Create(screen->heapId, NULL);
}

static void ApricornBoxScreen_FreeSpriteSystem(ApricornBoxScreen *screen) {
    TouchscreenListMenuSpawner_Destroy(screen->spawner);
    SpriteSystem_DestroySpriteManager(screen->spriteSystem, screen->spriteManager);
    screen->spriteManager = NULL;
    SpriteSystem_Free(screen->spriteSystem);
    screen->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(screen->heapId);
}

static void ApricornBoxScreen_InitSprites(ApricornBoxScreen *screen) {
    ApricornBoxScreen_InitSpriteSystem(screen);
    ApricornBoxScreen_CreateSprites(screen);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void ApricornBoxScreen_FreeSprites(ApricornBoxScreen *screen) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    ApricornBoxScreen_DeleteSprites(screen);
    ApricornBoxScreen_FreeSpriteSystem(screen);
}

static void ApricornBoxScreen_CreateSprites(ApricornBoxScreen *screen) {
    int y;
    int i;
    const UnmanagedSpriteTemplate *template = sApricornBoxSpriteTemplates;
    const s16 *position;
    u8 *cursor;
    for (i = 0; i <= 1; template ++, i++) {
        screen->sprites[i] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, template);
        Sprite_SetDrawFlag(screen->sprites[i], TRUE);
        Sprite_SetAnimActiveFlag(screen->sprites[i], TRUE);
    }
    Sprite_SetPriority(screen->sprites[1], 2);
    position = (const s16 *)sApricornBoxApricornPositions;
    for (i = 0; i < 7; position += 2, i++) {
        cursor = (u8 *)screen + (u8)(i + 2) * sizeof(Sprite *);
        ((ApricornBoxScreen *)cursor)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sApricornBoxSpriteTemplates[2]);
        Sprite_SetPositionXY(((ApricornBoxScreen *)cursor)->sprites[0], position[0], position[1]);
        Sprite_SetAnimActiveFlag(((ApricornBoxScreen *)cursor)->sprites[0], FALSE);
        Sprite_SetDrawFlag(((ApricornBoxScreen *)cursor)->sprites[0], TRUE);
        Sprite_SetAnimCtrlSeq(((ApricornBoxScreen *)cursor)->sprites[0], i + 9);
        Sprite_SetPriority(((ApricornBoxScreen *)cursor)->sprites[0], 2);
    }
    switch (screen->mode) {
    case APRICORN_BOX_MODE_BLENDER: {
        u8 *target;
        for (i = 0; i < 2; i++) {
            target = (u8 *)screen + (u8)(i + 9) * sizeof(Sprite *);
            ((ApricornBoxScreen *)target)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sApricornBoxSpriteTemplates[i + 3]);
            Sprite_SetDrawFlag(((ApricornBoxScreen *)target)->sprites[0], TRUE);
            Sprite_SetAnimActiveFlag(((ApricornBoxScreen *)target)->sprites[0], TRUE);
        }
        Sprite_SetAnimCtrlSeq(screen->sprites[9], screen->juiceServings + 16);
        Sprite_SetPaletteOverride(screen->sprites[9], screen->topMixIndex + 5);
        break;
    }
    case APRICORN_BOX_MODE_KURT: {
        int anim;
        for (i = 0, y = 112, anim = 28; i < 2; anim += 2, y += 24, i++) {
            cursor = (u8 *)screen + (u8)(i + 9) * sizeof(Sprite *);
            ((ApricornBoxScreen *)cursor)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sApricornBoxSpriteTemplates[5]);
            Sprite_SetPositionXY(((ApricornBoxScreen *)cursor)->sprites[0], 160, y);
            Sprite_SetDrawFlag(((ApricornBoxScreen *)cursor)->sprites[0], FALSE);
            Sprite_SetAnimActiveFlag(((ApricornBoxScreen *)cursor)->sprites[0], TRUE);
            Sprite_SetAnimCtrlSeq(((ApricornBoxScreen *)cursor)->sprites[0], anim);
            Sprite_SetPriority(((ApricornBoxScreen *)cursor)->sprites[0], 1);
        }
        for (i = 0; i < 4; i++) {
            int column;
            cursor = (u8 *)screen + (u8)(i + 11) * sizeof(Sprite *);
            ((ApricornBoxScreen *)cursor)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sApricornBoxSpriteTemplates[6]);
            column = i % 2;
            Sprite_SetPositionXY(((ApricornBoxScreen *)cursor)->sprites[0], 103 + (i / 2) * -22, 96 + column * 40);
            Sprite_SetDrawFlag(((ApricornBoxScreen *)cursor)->sprites[0], FALSE);
            Sprite_SetAnimActiveFlag(((ApricornBoxScreen *)cursor)->sprites[0], FALSE);
            Sprite_SetAnimCtrlSeq(((ApricornBoxScreen *)cursor)->sprites[0], 24 + column * 2);
            Sprite_SetPriority(((ApricornBoxScreen *)cursor)->sprites[0], 1);
        }
        break;
    }
    }
}

static void ApricornBoxScreen_DeleteSprites(ApricornBoxScreen *screen) {
    int i;
    for (i = 0; i < 15; i++) {
        if (screen->sprites[i] != NULL) {
            Sprite_Delete(screen->sprites[i]);
        }
    }
}

static void ApricornBoxScreen_MoveTask(SysTask *task, ApricornBoxMoveTask *move) {
    ApricornBoxScreen *owner = move->owner;
    switch (move->state) {
    case 0: {
        VecFx32 position;
        u8 frame;
        position.x = move->curX;
        position.y = move->curY;
        position.z = 0;
        Sprite_SetMatrix(move->sprite, &position);
        move->curX += move->dx;
        move->curY += move->dy;
        frame = move->frame++;
        if (frame >= move->frames) {
            move->state++;
        }
        break;
    }
    case 1:
    default:
        Sprite_SetPositionXY(move->sprite, move->x, move->y);
        owner->activeTasks -= 1;
        Heap_Free(move);
        SysTask_Destroy(task);
        break;
    }
}

static void ApricornBoxScreen_ReadMessage(ApricornBoxScreen *screen, int msgNo) {
    switch (msgNo) {
    case 6: {
        int count = ApricornBox_CountQueuedApricorns(screen->apricornBox);
        BufferIntegerAsString(screen->msgFormat, 0, 5 - count, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
        break;
    }
    case 12:
        if (screen->quantity > 1) {
            BufferItemNamePlural(screen->msgFormat, 0, ITEM_RED_APRICORN + screen->selectedApricorn);
        } else {
            BufferItemName(screen->msgFormat, 0, ITEM_RED_APRICORN + screen->selectedApricorn);
        }
        BufferIntegerAsString(screen->msgFormat, 1, screen->quantity, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
        break;
    case 21: {
        int juice = ApricornJuice_GetFlavorNameIndex(screen->juiceRecord);
        ReadMsgDataIntoString(screen->msgData, juice + msg_0021_00019, screen->string);
        BufferString(screen->msgFormat, 0, screen->string, 2, 0, 2);
        BufferIntegerAsString(screen->msgFormat, 1, screen->flavorTotal, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferIntegerAsString(screen->msgFormat, 2, screen->juiceMildness, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferIntegerAsString(screen->msgFormat, 3, 5 - screen->numBlending, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
        break;
    }
    default:
        ReadMsgDataIntoString(screen->msgData, msgNo + msg_0021_00027, screen->string2);
        return;
    }
    ReadMsgDataIntoString(screen->msgData, msgNo + msg_0021_00027, screen->string);
    StringExpandPlaceholders(screen->msgFormat, screen->string2, screen->string);
}

static u16 ApricornBoxScreen_GetJuiceTasteMsg(ApricornBoxScreen *screen, ApricornJuiceEntry *entry, int line) {
    u8 high = entry->strength - 1;
    int nibble = entry->strength;
    int low;
    switch (line) {
    case 2:
        low = entry->stat;
        if (low == 6) {
            return 122;
        }
        if (low == 5) {
            return 121;
        }
        if (nibble == 0) {
            return low + 123;
        }
        return low * 3 + high + 106;
    case 1:
        low = entry->stat;
        if (low == 6) {
            return 122;
        }
        return low * 4 + high + 86;
    case 0:
    default:
        low = entry->stat;
        if (low == 6) {
            return 131;
        }
        if (low == 5) {
            return high + 128;
        }
        return low * 6 + high + 56;
    }
}
