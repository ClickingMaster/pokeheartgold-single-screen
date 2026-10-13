#include "apricorn_box_screens.h"

#include "global.h"

#include "constants/items.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "apricorn_box_overlay_internal.h"
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
#include "swap_3d_buffers.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// Defined in the apricorn save code (src/apricorn_box.c) and OVY_58; no header declares them yet.
u8 ApricornBox_GetJuiceServings(SaveApricornBox *box);
int ApricornBox_GetMixProgressLevel(SaveApricornBox *box);
BOOL ApricornBox_QueueApricorn(SaveApricornBox *box, int apricorn, u32 value);
u32 ApricornBox_GetMixState(SaveApricornBox *apricornBox);
int sub_02031FE8(void *data);
int ApricornBox_UseJuiceServing(SaveApricornBox *box);
void ApricornBox_FeedAprijuice(ApricornJuice *record, Party *party, int slot);
void ParticleSystem_DeleteAllEmitters(SPLEmitter *emitter);

// Output of ApricornJuice_RankFlavors: the juice made from the blended apricorns.
typedef struct ApricornBoxJuice {
    ApricornJuiceEntry entries[3]; // 0x00
    u8 unk_06[6];                  // 0x06
    u8 numEntries;                 // 0x0C
} ApricornBoxJuice;

// The Apricorn Box screen (OVY_59 first app): view the apricorns, put them into the blender, give them to Kurt.
typedef struct ApricornBoxScreen {
    enum HeapID heapId;                  // 0x000
    ApricornBoxApp *app;                 // 0x004
    u8 filler_08[8];                     // 0x008
    SaveApricornBox *apricornBox;        // 0x010
    u8 juiceServings;                           // 0x014, ApricornBox_GetJuiceServings
    u8 flavorTotal;                           // 0x015, ApricornJuice_GetFlavorTotal(juiceRecord)
    u8 juiceMildness;                           // 0x016, juiceRecord byte 2
    u8 unk_17;                           // 0x017, sub_02031FE8(juiceRecord)
    u8 numBlending;                      // 0x018, apricorns in the blender (ApricornBox_CountQueuedApricorns, at most 5)
    u8 mixStage;                         // 0x019, ApricornBox_GetMixProgressLevel
    u8 apricornCounts[7];                // 0x01A
    u8 filler_21;                        // 0x021
    u8 juiceRecord[8];                   // 0x022, ApricornJuice (unaligned)
    ApricornBoxJuice juice;              // 0x02A
    u32 steps;                           // 0x038
    u16 subState;                        // 0x03C
    u16 animState;                       // 0x03E
    MenuInputState menuInputState;       // 0x040
    u8 mode;                             // 0x044, ApricornBoxApp.mode
    u8 textFrameDelay;                   // 0x045
    u8 frame;                            // 0x046
    u8 textPrinterId;                    // 0x047
    u8 quantityInputResult;              // 0x048
    u8 cursorArea;                       // 0x049, 0/1: apricorn columns, 2/3: buttons
    u8 cursorRow;                        // 0x04A
    u8 lastRowLeft;                      // 0x04B
    u8 lastRowRight;                     // 0x04C
    u8 selectedApricorn;                 // 0x04D
    u8 quantity;                         // 0x04E
    u8 maxQuantity;                      // 0x04F
    u8 activeTasks;                      // 0x050
    vu8 timer;                           // 0x051, volatile in AnimateHand (read twice there)
    u8 unk_52;                           // 0x052
    u8 overBlender;                      // 0x053
    BgConfig *bgConfig;                  // 0x054
    TouchscreenListMenuSpawner *spawner; // 0x058
    MsgData *msgData;                    // 0x05C
    MessageFormat *msgFormat;            // 0x060
    String *string;                      // 0x064
    String *string2;                     // 0x068
    String *closeText;                   // 0x06C, message 1 "CLOSE"
    String *unk_70;                      // 0x070
    String *numberTemplate;              // 0x074, message 25
    String *unk_78[22];                  // 0x078, messages 27-48
    String *apricornNames[7];            // 0x0D0, messages 7-13
    u8 filler_EC[0x3C];                  // 0x0EC
    Window windows[18];                  // 0x128
    YesNoPrompt *yesNoPrompt;            // 0x248
    SpriteSystem *spriteSystem;          // 0x24C
    SpriteManager *spriteManager;        // 0x250
    Sprite *sprites[15];                 // 0x254
    NARC *narc;                          // 0x290
    ListMenuItem *listMenuItems;         // 0x294
    TouchscreenListMenu *listMenu;       // 0x298
} ApricornBoxScreen;                     // size: 0x29C

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
    u16 unk_02;
    const u8 *messages;
} ApricornBoxListDesc;

extern const u32 sApricornBoxConfirmButtonColors[];
extern const u8 sApricornBoxQuantityDigitX[];
extern const TouchscreenListMenuTemplate sApricornBoxListMenuTemplates[2];
extern const ResdatIdList sApricornBoxSpriteResources;
extern const GraphicsModes sApricornBoxGraphicsModes;
extern const ApricornBoxListDesc sApricornBoxListMenus[];
extern const u8 sApricornBoxListMenuX[];
extern const u8 sApricornBoxListMenuY[];
extern const OamCharTransferParam sApricornBoxOamCharTransferParam;
extern const BgTemplate sApricornBoxBgTemplate_Sub3;
extern const BgTemplate sApricornBoxBgTemplate_Main0;
extern const u16 sApricornBoxApricornPositions[];
extern const u16 sApricornBoxApricornPositionsY[];
extern const BgTemplate sApricornBoxBgTemplate_Main3;
extern const BgTemplate sApricornBoxBgTemplate_Main1;
extern const BgTemplate sApricornBoxBgTemplate_Sub0;
extern const BgTemplate sApricornBoxBgTemplate_Main2;
extern const OamManagerParam sApricornBoxOamManagerParam;
extern const GraphicsBanks sApricornBoxGraphicsBanks;
extern const WindowTemplate sApricornBoxWindowTemplates[];
extern const UnmanagedSpriteTemplate sApricornBoxSpriteTemplates[];
extern const UnmanagedSpriteTemplate sApricornBoxApricornSpriteTemplate;
extern const UnmanagedSpriteTemplate sApricornBoxConfirmButtonSpriteTemplate;
extern const UnmanagedSpriteTemplate sApricornBoxQuantityArrowSpriteTemplate;
extern const TouchscreenHitbox sApricornBoxQuantityHitboxes[];
extern const TouchscreenHitbox sApricornBoxHitboxes[];
extern const TouchscreenHitbox sApricornBoxBlenderHitbox[];
extern const u32 sAprijuiceGenderColors[];
extern const u32 sAprijuiceButtonColors[];
extern const TouchscreenHitbox sAprijuiceHitboxes[];
extern const ResdatIdList sAprijuiceSpriteResources;
extern const GraphicsModes sAprijuiceGraphicsModes;
extern const OamCharTransferParam sAprijuiceOamCharTransferParam;
extern const u32 sAprijuiceStatOrder[];
extern const BgTemplate sAprijuiceBgTemplate_Sub3;
extern const BgTemplate sAprijuiceBgTemplate_Main1;
extern const BgTemplate sAprijuiceBgTemplate_Main2;
extern const BgTemplate sAprijuiceBgTemplate_Main3;
extern const OamManagerParam sAprijuiceOamManagerParam;
extern const WindowTemplate sAprijuiceWindowTemplates[];
extern const GraphicsBanks sAprijuiceGraphicsBanks;
extern const UnmanagedSpriteTemplate sAprijuiceSpriteTemplates[];
extern const UnmanagedSpriteTemplate sAprijuiceModifierSpriteTemplate;
extern const UnmanagedSpriteTemplate sAprijuiceStarSpriteTemplate;

void ApricornBoxScreen_InitFromArgs(ApricornBoxScreen *screen);
void ApricornBoxScreen_RestoreMenuInputState(ApricornBoxScreen *screen);
BOOL ApricornBoxScreen_Setup(ApricornBoxScreen *screen);
BOOL ApricornBoxScreen_Teardown(ApricornBoxScreen *screen);
int ApricornBoxScreen_FadeIn(ApricornBoxScreen *screen);
int ApricornBoxScreen_FadeOut(ApricornBoxScreen *screen);
int ApricornBoxScreen_ShowBlenderNews(ApricornBoxScreen *screen);
int ApricornBoxScreen_ViewState(ApricornBoxScreen *screen);
int ApricornBoxScreen_BlenderState(ApricornBoxScreen *screen);
int ApricornBoxScreen_KurtState(ApricornBoxScreen *screen);
int ApricornBoxScreen_KurtQuantityState(ApricornBoxScreen *screen);
int ApricornBoxScreen_LiftApricornState(ApricornBoxScreen *screen);
int ApricornBoxScreen_DragApricornState(ApricornBoxScreen *screen);
int ApricornBoxScreen_PutBackApricornState(ApricornBoxScreen *screen);
int ApricornBoxScreen_BlendApricornState(ApricornBoxScreen *screen);
int ApricornBoxScreen_JuiceMenuState(ApricornBoxScreen *screen);
int ApricornBoxScreen_WaitMessageState(ApricornBoxScreen *screen);
int ApricornBoxScreen_WaitJuiceTasteState(ApricornBoxScreen *screen);
BOOL ApricornBoxScreen_IsMessageDone(ApricornBoxScreen *screen);
void ApricornBoxScreen_ResetDisplay(void);
void ApricornBoxScreen_InitGraphics(ApricornBoxScreen *screen);
void ApricornBoxScreen_FreeGraphics(ApricornBoxScreen *screen);
void ApricornBoxScreen_DrawScreen(ApricornBoxScreen *screen);
void ApricornBoxScreen_LoadBoxState(ApricornBoxScreen *screen);
void ApricornBoxScreen_DrawCloseButton(ApricornBoxScreen *screen, int sequence);
void ApricornBoxScreen_DrawConfirmButton(ApricornBoxScreen *screen, int index, int sequence);
void ApricornBoxScreen_UpdateCursor(ApricornBoxScreen *screen);
void ApricornBoxScreen_SetCursorAnimActive(ApricornBoxScreen *screen, int active);
void ApricornBoxScreen_DrawApricornCount(ApricornBoxScreen *screen, u8 index);
void ApricornBoxScreen_PrintQuantity(ApricornBoxScreen *screen, int value);
void ApricornBoxScreen_ShowQuantityPanel(ApricornBoxScreen *screen, int show);
void ApricornBoxScreen_PrintBlenderStatus(ApricornBoxScreen *screen);
void ApricornBoxScreen_PrintHeader(ApricornBoxScreen *screen, int msgNo);
void ApricornBoxScreen_PrintDescription(ApricornBoxScreen *screen);
void ApricornBoxScreen_PrintMessage(ApricornBoxScreen *screen, int msgNo, int speed);
void ApricornBoxScreen_ClearMessage(ApricornBoxScreen *screen);
void ApricornBoxScreen_PrintJuiceTaste(ApricornBoxScreen *screen);
void ApricornBoxScreen_CreateJuiceMenu(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleJuiceMenu(ApricornBoxScreen *screen);
void ApricornBoxScreen_ShowYesNo(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleYesNo(ApricornBoxScreen *screen);
void ApricornBoxScreen_SetApricornsFocus(ApricornBoxScreen *screen, int mode);
BOOL ApricornBoxScreen_AnimateHand(ApricornBoxScreen *screen, int active);
void ApricornBoxScreen_StartMoveTask(ApricornBoxScreen *screen, int index, int type);
void ApricornBoxScreen_VBlankCB(ApricornBoxScreen *screen);
void ApricornBoxScreen_SetGfxBanks(void);
void ApricornBoxScreen_InitBgs(ApricornBoxScreen *screen);
void ApricornBoxScreen_FreeBgs(ApricornBoxScreen *screen);
void ApricornBoxScreen_LoadBgGraphics(ApricornBoxScreen *screen);
void ApricornBoxScreen_FreeBgGraphics(ApricornBoxScreen *screen);
void ApricornBoxScreen_InitStrings(ApricornBoxScreen *screen);
void ApricornBoxScreen_FreeStrings(ApricornBoxScreen *screen);
void ApricornBoxScreen_AddWindows(ApricornBoxScreen *screen);
void ApricornBoxScreen_RemoveWindows(ApricornBoxScreen *screen);
void ApricornBoxScreen_CreateListMenuItems(ApricornBoxScreen *screen);
void ApricornBoxScreen_FreeListMenuItems(ApricornBoxScreen *screen);
void ApricornBoxScreen_InitSpriteSystem(ApricornBoxScreen *screen);
void ApricornBoxScreen_FreeSpriteSystem(ApricornBoxScreen *screen);
void ApricornBoxScreen_InitSprites(ApricornBoxScreen *screen);
void ApricornBoxScreen_FreeSprites(ApricornBoxScreen *screen);
void ApricornBoxScreen_CreateSprites(ApricornBoxScreen *screen);
void ApricornBoxScreen_DeleteSprites(ApricornBoxScreen *screen);
void ApricornBoxScreen_MoveTask(SysTask *task, ApricornBoxMoveTask *move);
void ApricornBoxScreen_ReadMessage(ApricornBoxScreen *screen, int msgNo);
u16 ApricornBoxScreen_GetJuiceTasteMsg(ApricornBoxScreen *screen, ApricornJuiceEntry *entry, int line);
u8 ApricornBoxScreen_GetCursorTarget(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleViewInput(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleBlenderInput(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleDrag(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleKurtInput(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleQuantityInput(ApricornBoxScreen *screen);
int ApricornBoxScreen_ViewSelect(ApricornBoxScreen *screen, u32 input);
int ApricornBoxScreen_ViewKeys(ApricornBoxScreen *screen);
int ApricornBoxScreen_ViewTouch(ApricornBoxScreen *screen, int *hit);
int ApricornBoxScreen_BlenderSelect(ApricornBoxScreen *screen, u32 input);
int ApricornBoxScreen_BlenderKeys(ApricornBoxScreen *screen);
int ApricornBoxScreen_BlenderTouch(ApricornBoxScreen *screen, int *hit);
int ApricornBoxScreen_KurtSelect(ApricornBoxScreen *screen, u32 input);
int ApricornBoxScreen_KurtKeys(ApricornBoxScreen *screen);
int ApricornBoxScreen_KurtTouch(ApricornBoxScreen *screen, int *hit);
int ApricornBoxScreen_QuantitySelect(ApricornBoxScreen *screen, int input);
int ApricornBoxScreen_StepQuantity(int value, int max, int delta);
int ApricornBoxScreen_QuantityKeys(ApricornBoxScreen *screen);
int ApricornBoxScreen_QuantityTouch(ApricornBoxScreen *screen, int *hit);
BOOL ApricornBoxScreen_MoveCursor(ApricornBoxScreen *screen);
void ApricornBoxScreen_SwitchColumn(ApricornBoxScreen *screen, int direction);

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
    case 0:
        *state = ApricornBoxScreen_FadeIn(screen);
        break;
    case 1:
        *state = ApricornBoxScreen_FadeOut(screen);
        break;
    case 2:
        *state = ApricornBoxScreen_ShowBlenderNews(screen);
        break;
    case 3:
        *state = ApricornBoxScreen_ViewState(screen);
        break;
    case 4:
        *state = ApricornBoxScreen_BlenderState(screen);
        break;
    case 5:
        *state = ApricornBoxScreen_KurtState(screen);
        break;
    case 6:
        *state = ApricornBoxScreen_KurtQuantityState(screen);
        break;
    case 7:
        *state = ApricornBoxScreen_LiftApricornState(screen);
        break;
    case 8:
        *state = ApricornBoxScreen_DragApricornState(screen);
        break;
    case 9:
        *state = ApricornBoxScreen_PutBackApricornState(screen);
        break;
    case 10:
        *state = ApricornBoxScreen_BlendApricornState(screen);
        break;
    case 11:
        *state = ApricornBoxScreen_JuiceMenuState(screen);
        break;
    case 14:
        *state = ApricornBoxScreen_WaitMessageState(screen);
        break;
    case 15:
        *state = ApricornBoxScreen_WaitJuiceTasteState(screen);
        break;
    case 16:
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

void ApricornBoxScreen_InitFromArgs(ApricornBoxScreen *screen) {
    Options *options;

    screen->menuInputState = MenuInputStateMgr_GetState(screen->app->args->menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(screen->app->args->saveData);
    screen->textFrameDelay = Options_GetTextFrameDelay(options);
    screen->frame = Options_GetFrame(options);
    screen->mode = screen->app->mode;
    screen->steps = screen->app->args->steps;
    screen->apricornBox = Save_ApricornBox_Get(screen->app->args->saveData);
    ApricornBoxScreen_LoadBoxState(screen);
    if (screen->mode == 3) {
        screen->cursorArea = 2;
        screen->cursorRow = 0;
        screen->lastRowLeft = screen->app->lastRows[0];
        screen->lastRowRight = screen->app->lastRows[1];
        screen->mode = 1;
        screen->app->mode = 1;
    }
}

void ApricornBoxScreen_RestoreMenuInputState(ApricornBoxScreen *screen) {
    ApricornBoxApp *app = screen->app;
    ApricornBoxArgs *args = app->args;
    MenuInputStateMgr_SetState(args->menuInputStatePtr, screen->menuInputState);
}

BOOL ApricornBoxScreen_Setup(ApricornBoxScreen *screen) {
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

BOOL ApricornBoxScreen_Teardown(ApricornBoxScreen *screen) {
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

int ApricornBoxScreen_FadeIn(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, screen->heapId);
        screen->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            screen->subState = 0;
            switch (screen->mode) {
            case 0:
                break;
            case 1:
                if (ApricornBox_GetMixState(screen->apricornBox) != 1) {
                    return 2;
                }
                return 4;
            case 2:
                return 5;
            default:
                break;
            }
            return 3;
        }
        break;
    }
    return 0;
}

int ApricornBoxScreen_FadeOut(ApricornBoxScreen *screen) {
    switch (screen->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, screen->heapId);
        screen->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            screen->subState = 0;
            return 16;
        }
        break;
    }
    return 1;
}

int ApricornBoxScreen_ShowBlenderNews(ApricornBoxScreen *screen) {
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
        return 4;
    }
    return 2;
}

int ApricornBoxScreen_ViewState(ApricornBoxScreen *screen) {
    return ApricornBoxScreen_HandleViewInput(screen);
}

#pragma force_active on

int ApricornBoxScreen_BlenderState(ApricornBoxScreen *screen) {
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
    return 4;
}

#pragma force_active reset

int ApricornBoxScreen_KurtState(ApricornBoxScreen *screen) {
    return ApricornBoxScreen_HandleKurtInput(screen);
}

int ApricornBoxScreen_KurtQuantityState(ApricornBoxScreen *screen) {
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
            return 5;
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
            return 1;
        }
        ApricornBoxScreen_SetApricornsFocus(screen, 0);
        ApricornBoxScreen_PrintHeader(screen, 10);
        // fallthrough
    default:
        screen->subState = 0;
        return 5;
    }
    return 6;
}

int ApricornBoxScreen_LiftApricornState(ApricornBoxScreen *screen) {
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
            return 10;
        }
        if (gSystem.newKeys & PAD_BUTTON_B) {
            screen->subState = 0;
            PlaySE(SEQ_SE_DP_HYUN2);
            return 9;
        }
        if (System_GetTouchNew()) {
            screen->subState = 0;
            PlaySE(SEQ_SE_DP_HYUN2);
            if (screen->menuInputState != MENU_INPUT_STATE_TOUCH) {
                screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
            }
            return 9;
        }
        break;
    default:
        screen->subState = 0;
        return 4;
    }
    return 7;
}

int ApricornBoxScreen_DragApricornState(ApricornBoxScreen *screen) {
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
            return 9;
        case 1:
            screen->subState = 0;
            return 10;
        default:
            break;
        }
        break;
    }
    return 8;
}

int ApricornBoxScreen_PutBackApricornState(ApricornBoxScreen *screen) {
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
        return 4;
    }
    return 9;
}

int ApricornBoxScreen_BlendApricornState(ApricornBoxScreen *screen) {
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
            return 9;
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
        return 4;
    }
    return 10;
}

int ApricornBoxScreen_JuiceMenuState(ApricornBoxScreen *screen) {
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
            return 4;
        }
        if (result == 1) {
            ApricornBoxScreen_PrintHeader(screen, 9);
            ApricornBoxScreen_PrintJuiceTaste(screen);
            ApricornBoxScreen_SetApricornsFocus(screen, 1);
            return 15;
        }
        screen->app->mode = 3;
        screen->app->partySlot = 0;
        screen->app->lastRows[0] = screen->lastRowLeft;
        screen->app->lastRows[1] = screen->lastRowRight;
        return 1;
    }
    return 11;
}

int ApricornBoxScreen_WaitMessageState(ApricornBoxScreen *screen) {
    return ApricornBoxScreen_IsMessageDone(screen) ? 4 : 14;
}

int ApricornBoxScreen_WaitJuiceTasteState(ApricornBoxScreen *screen) {
    if (ApricornBoxScreen_IsMessageDone(screen)) {
        ApricornBoxScreen_ClearMessage(screen);
        ApricornBoxScreen_PrintBlenderStatus(screen);
        return 4;
    }
    return 15;
}

BOOL ApricornBoxScreen_IsMessageDone(ApricornBoxScreen *screen) {
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
    if (gSystem.newKeys & 0xCF3) {
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

void ApricornBoxScreen_ResetDisplay(void) {
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

void ApricornBoxScreen_InitGraphics(ApricornBoxScreen *screen) {
    ApricornBoxScreen_InitBgs(screen);
    ApricornBoxScreen_LoadBgGraphics(screen);
    ApricornBoxScreen_InitStrings(screen);
    ApricornBoxScreen_AddWindows(screen);
    ApricornBoxScreen_CreateListMenuItems(screen);
    ApricornBoxScreen_InitSprites(screen);
    Main_SetVBlankIntrCB((GFIntrCB)ApricornBoxScreen_VBlankCB, screen);
}

void ApricornBoxScreen_FreeGraphics(ApricornBoxScreen *screen) {
    ApricornBoxScreen_FreeSprites(screen);
    ApricornBoxScreen_FreeListMenuItems(screen);
    ApricornBoxScreen_RemoveWindows(screen);
    ApricornBoxScreen_FreeStrings(screen);
    ApricornBoxScreen_FreeBgGraphics(screen);
    ApricornBoxScreen_FreeBgs(screen);
}

void ApricornBoxScreen_DrawScreen(ApricornBoxScreen *screen) {
    int i;
    int width;
    int x;
    u8 mode;
    ApricornBoxScreen_DrawCloseButton(screen, 0);
    ApricornBoxScreen_UpdateCursor(screen);
    ReadMsgDataIntoString(screen->msgData, 0, screen->string);
    width = (u8)FontID_String_GetWidth(0, screen->string, 0);
    x = (80 - width) / 2;
    AddTextPrinterParameterizedWithColor(&screen->windows[3], 0, screen->string, x, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    for (i = 0; i < 7; i++) {
        ApricornBoxScreen_DrawApricornCount(screen, i);
    }
    mode = screen->mode;
    switch (mode) {
    case 1:
        ApricornBoxScreen_PrintBlenderStatus(screen);
        return;
    case 2:
        ApricornBoxScreen_PrintHeader(screen, 10);
        return;
    case 0:
    default:
        break;
    }
    ApricornBoxScreen_PrintHeader(screen, 0);
}

void ApricornBoxScreen_LoadBoxState(ApricornBoxScreen *screen) {
    int i;
    screen->numBlending = ApricornBox_CountQueuedApricorns(screen->apricornBox);
    screen->juiceServings = ApricornBox_GetJuiceServings(screen->apricornBox);
    ApricornBox_GetJuice(screen->apricornBox, 8, (ApricornJuice *)screen->juiceRecord);
    screen->juiceMildness = screen->juiceRecord[2];
    screen->unk_17 = sub_02031FE8(screen->juiceRecord);
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
    // a struct of locals keeps the original stack layout
    struct {
        int value;
    } width;

    Window *window = &screen->windows[index + 6];
    FillWindowPixelBuffer(window, 0);
    ReadMsgDataIntoString(screen->msgData, index + 5, screen->string2);
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
        x = sApricornBoxApricornPositions[index * 2] + 4;
        y = sApricornBoxApricornPositionsY[index * 2] - 16;
        break;
    }
    Sprite_SetPositionXY(screen->sprites[1], x, y);
    ApricornBoxScreen_PrintDescription(screen);
}

void ApricornBoxScreen_SetCursorAnimActive(ApricornBoxScreen *screen, int active) {
    Sprite_SetAnimActiveFlag(screen->sprites[1], active);
}

void ApricornBoxScreen_DrawApricornCount(ApricornBoxScreen *screen, u8 index) {
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
    Sprite_SetPositionXY(*spriteSlot, sApricornBoxApricornPositions[index * 2], sApricornBoxApricornPositionsY[index * 2]);
}

#pragma force_active on

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

#pragma force_active reset

void ApricornBoxScreen_ShowQuantityPanel(ApricornBoxScreen *screen, int show) {
    int i;
    if (show) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(screen->narc, 6, screen->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, screen->heapId);
        ApricornBoxScreen_PrintQuantity(screen, screen->quantity);
        ApricornBoxScreen_DrawConfirmButton(screen, 0, 0);
        ApricornBoxScreen_DrawConfirmButton(screen, 1, 0);
        FillWindowPixelBuffer(&screen->windows[5], 0);
        AddTextPrinterParameterizedWithColor(&screen->windows[5], 0, screen->apricornNames[screen->selectedApricorn], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&screen->windows[5]);
        ReadMsgDataIntoString(screen->msgData, 26, screen->string2);
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

void ApricornBoxScreen_PrintBlenderStatus(ApricornBoxScreen *screen) {
    if (screen->numBlending == 0) {
        ApricornBoxScreen_PrintHeader(screen, 1);
    } else {
        ApricornBoxScreen_PrintHeader(screen, screen->mixStage + 1);
    }
}

void ApricornBoxScreen_PrintHeader(ApricornBoxScreen *screen, int msgNo) {
    Window *window = &screen->windows[1];
    FillWindowPixelBuffer(window, 0);
    ApricornBoxScreen_ReadMessage(screen, msgNo);
    AddTextPrinterParameterized(window, 0, screen->string2, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(window);
}

void ApricornBoxScreen_PrintDescription(ApricornBoxScreen *screen) {
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

void ApricornBoxScreen_ClearMessage(ApricornBoxScreen *screen) {
    ClearFrameAndWindow2(&screen->windows[0], TRUE);
    ClearWindowTilemapAndScheduleTransfer(&screen->windows[0]);
}

#pragma force_active on
#pragma opt_propagation off

void ApricornBoxScreen_PrintJuiceTaste(ApricornBoxScreen *screen) {
    // the window is addressed through an offset variable, as in the original
    u32 offset = offsetof(ApricornBoxScreen, windows);
    FillWindowPixelBuffer((Window *)((u8 *)screen + offset), 15);
    DrawFrameAndWindow2((Window *)((u8 *)screen + offset), TRUE, 1, 13);
    {
        int msgNo = ApricornBoxScreen_GetJuiceTasteMsg(screen, &screen->juice.entries[0], 0);
        ReadMsgDataIntoString(screen->msgData, msgNo, screen->string2);
    }
    AddTextPrinterParameterized((Window *)((u8 *)screen + offset), 1, screen->string2, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    if (screen->juice.entries[0].stat == 5) {
        ScheduleWindowCopyToVram((Window *)((u8 *)screen + offset));
        return;
    }
    if (screen->juice.numEntries > 1) {
        {
            int msgNo = ApricornBoxScreen_GetJuiceTasteMsg(screen, &screen->juice.entries[1], 1);
            ReadMsgDataIntoString(screen->msgData, msgNo, screen->string2);
        }
        AddTextPrinterParameterized((Window *)((u8 *)screen + offset), 1, screen->string2, 0, 16, TEXT_SPEED_NOTRANSFER, NULL);
    }
    if (screen->juice.numEntries > 2) {
        {
            int msgNo = ApricornBoxScreen_GetJuiceTasteMsg(screen, &screen->juice.entries[2], 2);
            ReadMsgDataIntoString(screen->msgData, msgNo, screen->string2);
        }
        AddTextPrinterParameterized((Window *)((u8 *)screen + offset), 1, screen->string2, 0, 32, TEXT_SPEED_NOTRANSFER, NULL);
    } else if ((u8)(screen->juice.numEntries + 255) <= 1) {
        ReadMsgDataIntoString(screen->msgData, 122, screen->string2);
        AddTextPrinterParameterized((Window *)((u8 *)screen + offset), 1, screen->string2, 0, (screen->juice.numEntries - 1) * 16 + 16, TEXT_SPEED_NOTRANSFER, NULL);
    }
    ScheduleWindowCopyToVram((Window *)((u8 *)screen + offset));
}

#pragma opt_propagation reset

void ApricornBoxScreen_CreateJuiceMenu(ApricornBoxScreen *screen) {
    TouchscreenListMenuHeader header;
    u8 y;
    u8 x;
    MI_CpuFill8(&header, 0, sizeof(header));
    header.template = sApricornBoxListMenuTemplates[1];
    header.listMenuItems = screen->listMenuItems;
    header.bgConfig = screen->bgConfig;
    header.numWindows = sApricornBoxListMenus[screen->mode - 1].count;
    y = sApricornBoxListMenuY[(screen->mode - 1) * 8];
    x = sApricornBoxListMenuX[(screen->mode - 1) * 8];
    screen->listMenu = TouchscreenListMenu_Create(screen->spawner, &header, (u8)screen->menuInputState, x, y, 8, 0);
}

#pragma force_active reset

int ApricornBoxScreen_HandleJuiceMenu(ApricornBoxScreen *screen) {
    int result = TouchscreenListMenu_HandleInput(screen->listMenu);
    if (result == -1) {
        return -1;
    }
    screen->menuInputState = (MenuInputState)TouchscreenListMenu_WasLastInputTouch(screen->listMenu);
    TouchscreenListMenu_Destroy(screen->listMenu);
    return result;
}

void ApricornBoxScreen_ShowYesNo(ApricornBoxScreen *screen) {
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

int ApricornBoxScreen_HandleYesNo(ApricornBoxScreen *screen) {
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

#pragma force_active on

BOOL ApricornBoxScreen_AnimateHand(ApricornBoxScreen *screen, int active) {
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

void ApricornBoxScreen_StartMoveTask(ApricornBoxScreen *screen, int index, int type) {
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
        base = sApricornBoxApricornPositions[move->apricorn * 2];
        move->x = base + index * 4;
        base = sApricornBoxApricornPositionsY[move->apricorn * 2];
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

#pragma force_active reset

void ApricornBoxScreen_VBlankCB(ApricornBoxScreen *screen) {
    if (screen->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(screen->bgConfig);
    ((vu32 *)0x027E0000)[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

void ApricornBoxScreen_SetGfxBanks(void) {
    GraphicsBanks banks = sApricornBoxGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void ApricornBoxScreen_InitBgs(ApricornBoxScreen *screen) {
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

void ApricornBoxScreen_FreeBgs(ApricornBoxScreen *screen) {
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_SUB_0);
    Heap_Free(screen->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}

#pragma force_active on

void ApricornBoxScreen_LoadBgGraphics(ApricornBoxScreen *screen) {
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
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, 0, 8, 6, 26);
}

#pragma force_active reset

void ApricornBoxScreen_FreeBgGraphics(ApricornBoxScreen *screen) {
    NARC_Delete(screen->narc);
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, 0, 0, 31, 0);
}

#pragma force_active on

void ApricornBoxScreen_InitStrings(ApricornBoxScreen *screen) {
    int i;
    int j;
    FontID_Alloc(4, screen->heapId);
    screen->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0021_bin, screen->heapId);
    screen->msgFormat = MessageFormat_New_Custom(16, 32, screen->heapId);
    screen->string = String_New(256, screen->heapId);
    screen->string2 = String_New(256, screen->heapId);
    screen->closeText = NewString_ReadMsgData(screen->msgData, 1);
    screen->numberTemplate = NewString_ReadMsgData(screen->msgData, 25);
    for (i = 0; i < 22; i++) {
        screen->unk_78[i] = NewString_ReadMsgData(screen->msgData, i + 27);
    }
    for (j = 0; j < 7; j++) {
        screen->apricornNames[j] = NewString_ReadMsgData(screen->msgData, j + 7);
    }
}

#pragma force_active reset

void ApricornBoxScreen_FreeStrings(ApricornBoxScreen *screen) {
    int i;
    for (i = 0; i < 7; i++) {
        String_Delete(screen->apricornNames[i]);
    }
    {
        // a byte cursor, as in the original (keeps its register allocation)
        u8 *cursor;
        int j;
        j = 0;
        cursor = (u8 *)screen;
        for (; j < 22; j++, cursor += sizeof(String *)) {
            String_Delete(((ApricornBoxScreen *)cursor)->unk_78[0]);
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

#pragma force_active on

void ApricornBoxScreen_AddWindows(ApricornBoxScreen *screen) {
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

#pragma force_active reset

void ApricornBoxScreen_RemoveWindows(ApricornBoxScreen *screen) {
    int i = 0;
    Window *window = &screen->windows[0];
    for (; i < 18; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
    YesNoPrompt_Destroy(screen->yesNoPrompt);
}

void ApricornBoxScreen_CreateListMenuItems(ApricornBoxScreen *screen) {
    int i;
    if (screen->mode != 0) {
        int offset = (screen->mode - 1) * 8;
        const ApricornBoxListDesc *desc = (const ApricornBoxListDesc *)((const u8 *)sApricornBoxListMenus + offset);
        screen->listMenuItems = ListMenuItems_New(*(const u16 *)((const u8 *)sApricornBoxListMenus + offset), screen->heapId);
        for (i = 0; i < desc->count; i++) {
            ListMenuItems_AppendFromMsgData(screen->listMenuItems, screen->msgData, desc->messages[i], i);
        }
    }
}

void ApricornBoxScreen_FreeListMenuItems(ApricornBoxScreen *screen) {
    if (screen->mode) {
        ListMenuItems_Delete(screen->listMenuItems);
        screen->listMenuItems = NULL;
    }
}

#pragma force_active on

void ApricornBoxScreen_InitSpriteSystem(ApricornBoxScreen *screen) {
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

void ApricornBoxScreen_FreeSpriteSystem(ApricornBoxScreen *screen) {
    TouchscreenListMenuSpawner_Destroy(screen->spawner);
    SpriteSystem_DestroySpriteManager(screen->spriteSystem, screen->spriteManager);
    screen->spriteManager = NULL;
    SpriteSystem_Free(screen->spriteSystem);
    screen->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(screen->heapId);
}

#pragma force_active reset

void ApricornBoxScreen_InitSprites(ApricornBoxScreen *screen) {
    ApricornBoxScreen_InitSpriteSystem(screen);
    ApricornBoxScreen_CreateSprites(screen);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void ApricornBoxScreen_FreeSprites(ApricornBoxScreen *screen) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    ApricornBoxScreen_DeleteSprites(screen);
    ApricornBoxScreen_FreeSpriteSystem(screen);
}

#pragma force_active on

void ApricornBoxScreen_CreateSprites(ApricornBoxScreen *screen) {
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
    // the sprite slots below are addressed through a byte cursor, as in the original
    for (i = 0; i < 7; position += 2, i++) {
        cursor = (u8 *)screen + (u8)(i + 2) * sizeof(Sprite *);
        ((ApricornBoxScreen *)cursor)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sApricornBoxApricornSpriteTemplate);
        Sprite_SetPositionXY(((ApricornBoxScreen *)cursor)->sprites[0], position[0], position[1]);
        Sprite_SetAnimActiveFlag(((ApricornBoxScreen *)cursor)->sprites[0], FALSE);
        Sprite_SetDrawFlag(((ApricornBoxScreen *)cursor)->sprites[0], TRUE);
        Sprite_SetAnimCtrlSeq(((ApricornBoxScreen *)cursor)->sprites[0], i + 9);
        Sprite_SetPriority(((ApricornBoxScreen *)cursor)->sprites[0], 2);
    }
    switch (screen->mode) {
    case 1: {
        u8 *target;
        for (i = 0; i < 2; i++) {
            target = (u8 *)screen + (u8)(i + 9) * sizeof(Sprite *);
            ((ApricornBoxScreen *)target)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sApricornBoxSpriteTemplates[i + 3]);
            Sprite_SetDrawFlag(((ApricornBoxScreen *)target)->sprites[0], TRUE);
            Sprite_SetAnimActiveFlag(((ApricornBoxScreen *)target)->sprites[0], TRUE);
        }
        Sprite_SetAnimCtrlSeq(screen->sprites[9], screen->juiceServings + 16);
        Sprite_SetPaletteOverride(screen->sprites[9], screen->unk_17 + 5);
        break;
    }
    case 2: {
        int anim;
        for (i = 0, y = 112, anim = 28; i < 2; anim += 2, y += 24, i++) {
            cursor = (u8 *)screen + (u8)(i + 9) * sizeof(Sprite *);
            ((ApricornBoxScreen *)cursor)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sApricornBoxConfirmButtonSpriteTemplate);
            Sprite_SetPositionXY(((ApricornBoxScreen *)cursor)->sprites[0], 160, y);
            Sprite_SetDrawFlag(((ApricornBoxScreen *)cursor)->sprites[0], FALSE);
            Sprite_SetAnimActiveFlag(((ApricornBoxScreen *)cursor)->sprites[0], TRUE);
            Sprite_SetAnimCtrlSeq(((ApricornBoxScreen *)cursor)->sprites[0], anim);
            Sprite_SetPriority(((ApricornBoxScreen *)cursor)->sprites[0], 1);
        }
        for (i = 0; i < 4; i++) {
            int column;
            cursor = (u8 *)screen + (u8)(i + 11) * sizeof(Sprite *);
            ((ApricornBoxScreen *)cursor)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sApricornBoxQuantityArrowSpriteTemplate);
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

#pragma force_active reset

void ApricornBoxScreen_DeleteSprites(ApricornBoxScreen *screen) {
    int i;
    for (i = 0; i < 15; i++) {
        if (screen->sprites[i] != NULL) {
            Sprite_Delete(screen->sprites[i]);
        }
    }
}

#pragma force_active on

void ApricornBoxScreen_MoveTask(SysTask *task, ApricornBoxMoveTask *move) {
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

#pragma force_active reset

void ApricornBoxScreen_ReadMessage(ApricornBoxScreen *screen, int msgNo) {
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
        int juice = sub_02032004(screen->juiceRecord);
        ReadMsgDataIntoString(screen->msgData, juice + 19, screen->string);
        BufferString(screen->msgFormat, 0, screen->string, 2, 0, 2);
        BufferIntegerAsString(screen->msgFormat, 1, screen->flavorTotal, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferIntegerAsString(screen->msgFormat, 2, screen->juiceMildness, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferIntegerAsString(screen->msgFormat, 3, 5 - screen->numBlending, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
        break;
    }
    default:
        ReadMsgDataIntoString(screen->msgData, msgNo + 27, screen->string2);
        return;
    }
    ReadMsgDataIntoString(screen->msgData, msgNo + 27, screen->string);
    StringExpandPlaceholders(screen->msgFormat, screen->string2, screen->string);
}

#pragma force_active on

u16 ApricornBoxScreen_GetJuiceTasteMsg(ApricornBoxScreen *screen, ApricornJuiceEntry *entry, int line) {
    u8 high = entry->unk_04 - 1;
    int nibble = entry->unk_04;
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

u8 ApricornBoxScreen_GetCursorTarget(ApricornBoxScreen *screen) {
    switch (screen->cursorArea) {
    case 0:
        return screen->cursorRow * 2;
    case 1:
        return screen->cursorRow * 2 + 1;
    case 2:
        if (screen->mode == 1) {
            return 7;
        }
        return 8;
    case 3:
        return 8;
    }
    return 0;
}

#pragma force_active reset

int ApricornBoxScreen_HandleViewInput(ApricornBoxScreen *screen) {
    int hit = 0;
    int result = ApricornBoxScreen_ViewTouch(screen, &hit);
    if (hit == 0) {
        result = ApricornBoxScreen_ViewKeys(screen);
    }
    return result;
}

int ApricornBoxScreen_HandleBlenderInput(ApricornBoxScreen *screen) {
    int hit = 0;
    int result = ApricornBoxScreen_BlenderTouch(screen, &hit);
    if (hit == 0) {
        result = ApricornBoxScreen_BlenderKeys(screen);
    }
    return result;
}

int ApricornBoxScreen_HandleDrag(ApricornBoxScreen *screen) {
    s16 x;
    s16 y;
    u8 hit;
    Sprite_GetPositionXY(screen->sprites[screen->selectedApricorn + 2], &x, &y);
    hit = TouchscreenHitbox_PointIsIn(sApricornBoxBlenderHitbox, x, y);
    if (System_GetTouchHeld()) {
        x = gSystem.touchX;
        y = gSystem.touchY;
        Sprite_SetPositionXY(screen->sprites[screen->selectedApricorn + 2], x, y);
        Sprite_SetPositionXY(screen->sprites[1], x, y - 6);
        if (hit) {
            if (screen->overBlender == 0) {
                screen->overBlender = 1;
                PlaySE(SEQ_SE_GS_BON01);
                Sprite_SetAnimCtrlSeq(screen->sprites[10], 21);
            }
        } else if (screen->overBlender != 0) {
            screen->overBlender = 0;
            PlaySE(SEQ_SE_GS_BON01);
            Sprite_SetAnimCtrlSeq(screen->sprites[10], 20);
        }
        return -1;
    }
    screen->overBlender = 0;
    if (hit) {
        PlaySE(SEQ_SE_DP_BOX01);
        return 1;
    }
    PlaySE(SEQ_SE_DP_HYUN2);
    return 0;
}

int ApricornBoxScreen_HandleKurtInput(ApricornBoxScreen *screen) {
    int hit = 0;
    int result = ApricornBoxScreen_KurtTouch(screen, &hit);
    if (hit == 0) {
        result = ApricornBoxScreen_KurtKeys(screen);
    }
    return result;
}

int ApricornBoxScreen_HandleQuantityInput(ApricornBoxScreen *screen) {
    int hit = 0;
    int result = ApricornBoxScreen_QuantityTouch(screen, &hit);
    if (hit == 0) {
        result = ApricornBoxScreen_QuantityKeys(screen);
    }
    return result;
}

int ApricornBoxScreen_ViewSelect(ApricornBoxScreen *screen, u32 input) {
    if (input <= 6) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        screen->selectedApricorn = input;
        return 3;
    }
    if (input == 8) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawCloseButton(screen, 1);
        return 1;
    }
    return 3;
}

int ApricornBoxScreen_ViewKeys(ApricornBoxScreen *screen) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return ApricornBoxScreen_ViewSelect(screen, 8);
    }
    if (keys & PAD_BUTTON_A) {
        return ApricornBoxScreen_ViewSelect(screen, ApricornBoxScreen_GetCursorTarget(screen));
    }
    if (ApricornBoxScreen_MoveCursor(screen)) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        return 3;
    }
    return 3;
}

int ApricornBoxScreen_ViewTouch(ApricornBoxScreen *screen, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(sApricornBoxHitboxes);
    if (input == -1 || input == 7) {
        return 3;
    }
    *hit = 1;
    screen->menuInputState = MENU_INPUT_STATE_TOUCH;
    screen->lastRowRight = 0;
    screen->lastRowLeft = screen->lastRowRight;
    screen->cursorRow = 0;
    if (input == 8) {
        screen->cursorArea = 2;
    } else {
        screen->cursorArea = input % 2;
        screen->cursorRow = input / 2;
    }
    return ApricornBoxScreen_ViewSelect(screen, input);
}

int ApricornBoxScreen_BlenderSelect(ApricornBoxScreen *screen, u32 input) {
    if (input <= 6) {
        ApricornBoxScreen_UpdateCursor(screen);
        if (screen->apricornCounts[input] == 0) {
            return 4;
        }
        PlaySE(SEQ_SE_DP_BOX02);
        screen->selectedApricorn = input;
        return screen->menuInputState == MENU_INPUT_STATE_BUTTONS ? 7 : 8;
    }
    if (input == 8) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawCloseButton(screen, 1);
        return 1;
    }
    if (input == 7) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        screen->selectedApricorn = input;
        if (screen->juiceServings == 0 && screen->numBlending == 0) {
            ApricornBoxScreen_PrintMessage(screen, 25, TEXT_SPEED_NOTRANSFER);
            ApricornBoxScreen_SetApricornsFocus(screen, 1);
            return 15;
        }
        if (screen->juiceServings == 0 && screen->numBlending != 0) {
            ApricornBoxScreen_PrintMessage(screen, 27, TEXT_SPEED_NOTRANSFER);
            ApricornBoxScreen_SetApricornsFocus(screen, 1);
            return 15;
        }
        if (screen->juiceServings != 0 && screen->numBlending != 0) {
            ApricornBoxScreen_PrintMessage(screen, 28, TEXT_SPEED_NOTRANSFER);
            ApricornBoxScreen_SetApricornsFocus(screen, 1);
            return 15;
        }
        return 11;
    }
    return 4;
}

int ApricornBoxScreen_BlenderKeys(ApricornBoxScreen *screen) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return ApricornBoxScreen_BlenderSelect(screen, 8);
    }
    if (keys & PAD_BUTTON_A) {
        return ApricornBoxScreen_BlenderSelect(screen, ApricornBoxScreen_GetCursorTarget(screen));
    }
    if (ApricornBoxScreen_MoveCursor(screen)) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        return 4;
    }
    return 4;
}

int ApricornBoxScreen_BlenderTouch(ApricornBoxScreen *screen, int *hit) {
    int input;
    int value;
    input = TouchscreenHitbox_FindHitboxAtTouchNew(sApricornBoxHitboxes);
    // the constants go through a variable, as in the original
    value = 0;
    if (input == ~value) {
        return 4;
    }
    value = 1;
    *hit = value;
    screen->menuInputState = (MenuInputState)value;
    value = 0;
    screen->lastRowRight = value;
    screen->lastRowLeft = screen->lastRowRight;
    screen->cursorRow = value;
    switch (input) {
    case 7:
        screen->cursorArea = 2;
        break;
    case 8:
        screen->cursorArea = 3;
        break;
    default:
        screen->cursorArea = input % 2;
        screen->cursorRow = input / 2;
        break;
    }
    return ApricornBoxScreen_BlenderSelect(screen, input);
}

int ApricornBoxScreen_KurtSelect(ApricornBoxScreen *screen, u32 input) {
    if (input <= 6) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        if (screen->apricornCounts[input] == 0) {
            return 5;
        }
        screen->selectedApricorn = input;
        screen->quantity = 1;
        screen->maxQuantity = screen->apricornCounts[screen->selectedApricorn];
        return 6;
    }
    if (input == 8) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawCloseButton(screen, 1);
        return 1;
    }
    return 5;
}

int ApricornBoxScreen_KurtKeys(ApricornBoxScreen *screen) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return ApricornBoxScreen_KurtSelect(screen, 8);
    }
    if (keys & PAD_BUTTON_A) {
        return ApricornBoxScreen_KurtSelect(screen, ApricornBoxScreen_GetCursorTarget(screen));
    }
    if (ApricornBoxScreen_MoveCursor(screen)) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        return 5;
    }
    return 5;
}

int ApricornBoxScreen_KurtTouch(ApricornBoxScreen *screen, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(sApricornBoxHitboxes);
    if (input == -1 || input == 7) {
        return 5;
    }
    *hit = 1;
    screen->menuInputState = MENU_INPUT_STATE_TOUCH;
    screen->lastRowRight = 0;
    screen->lastRowLeft = screen->lastRowRight;
    screen->cursorRow = 0;
    if (input == 8) {
        screen->cursorArea = 2;
    } else {
        screen->cursorArea = input % 2;
        screen->cursorRow = input / 2;
    }
    return ApricornBoxScreen_KurtSelect(screen, input);
}

int ApricornBoxScreen_QuantitySelect(ApricornBoxScreen *screen, int input) {
    if (input == 0) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawConfirmButton(screen, 0, 1);
        return 0;
    }
    if (input == 1) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawConfirmButton(screen, 1, 1);
        return 1;
    }
    switch (input) {
    case 2:
        screen->quantity = ApricornBoxScreen_StepQuantity(screen->quantity, screen->maxQuantity, 1);
        break;
    case 4:
        screen->quantity = ApricornBoxScreen_StepQuantity(screen->quantity, screen->maxQuantity, 10);
        break;
    case 3:
        screen->quantity = ApricornBoxScreen_StepQuantity(screen->quantity, screen->maxQuantity, -1);
        break;
    case 5:
        screen->quantity = ApricornBoxScreen_StepQuantity(screen->quantity, screen->maxQuantity, -10);
        break;
    default:
        return 6;
    }
    PlaySE(SEQ_SE_DP_SELECT);
    ApricornBoxScreen_PrintQuantity(screen, screen->quantity);
    return 6;
}

int ApricornBoxScreen_StepQuantity(int value, int max, int delta) {
    if (delta > 0) {
        if (value == max) {
            return 1;
        }
        value += delta;
        if (value > max) {
            return max;
        }
    } else {
        if (value == 1) {
            return max;
        }
        value += delta;
        if (value <= 0) {
            return 1;
        }
    }
    return value;
}

int ApricornBoxScreen_QuantityKeys(ApricornBoxScreen *screen) {
    u32 keys = gSystem.newKeys;
    int down;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return ApricornBoxScreen_QuantitySelect(screen, 1);
    }
    if (keys & PAD_BUTTON_A) {
        return ApricornBoxScreen_QuantitySelect(screen, 0);
    }
    if (keys & PAD_KEY_UP) {
        return ApricornBoxScreen_QuantitySelect(screen, 2);
    }
    down = keys & PAD_KEY_DOWN;
    if (down) {
        return ApricornBoxScreen_QuantitySelect(screen, 3);
    }
    if (keys & PAD_KEY_RIGHT) {
        return ApricornBoxScreen_QuantitySelect(screen, 4);
    }
    if (keys & PAD_KEY_LEFT) {
        return ApricornBoxScreen_QuantitySelect(screen, 5);
    }
    if (down != 0) {
        PlaySE(SEQ_SE_DP_SELECT);
        return 6;
    }
    return 6;
}

int ApricornBoxScreen_QuantityTouch(ApricornBoxScreen *screen, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(sApricornBoxQuantityHitboxes);
    if (input == -1) {
        return 6;
    }
    if ((u32)(input - 4) <= 1 && screen->maxQuantity < 10) {
        return 6;
    }
    if (input >= 2) {
        Sprite **sprites = screen->sprites;
        u32 offset = (input + 9) * sizeof(Sprite *);
        Sprite_SetAnimActiveFlag(*(Sprite **)((u8 *)sprites + offset), TRUE);
        Sprite_ResetAnimCtrlState(*(Sprite **)((u8 *)sprites + offset));
    }
    *hit = 1;
    screen->menuInputState = MENU_INPUT_STATE_TOUCH;
    return ApricornBoxScreen_QuantitySelect(screen, input);
}

BOOL ApricornBoxScreen_MoveCursor(ApricornBoxScreen *screen) {
    u32 keys;
    switch (screen->cursorArea) {
    case 0:
        keys = gSystem.newKeys;
        if (keys & PAD_KEY_RIGHT) {
            ApricornBoxScreen_SwitchColumn(screen, 0);
            return TRUE;
        }
        if ((keys & PAD_KEY_UP) && screen->cursorRow != 0) {
            screen->lastRowRight = 0;
            screen->cursorRow--;
            return TRUE;
        }
        if ((keys & PAD_KEY_DOWN) && screen->cursorRow < 3) {
            screen->lastRowRight = 0;
            screen->cursorRow++;
            return TRUE;
        }
        break;
    case 1:
        keys = gSystem.newKeys;
        if (keys & PAD_KEY_LEFT) {
            ApricornBoxScreen_SwitchColumn(screen, 1);
            return TRUE;
        }
        if (keys & PAD_KEY_RIGHT) {
            screen->lastRowRight = screen->cursorRow;
            screen->cursorRow = 0;
            screen->cursorArea++;
            return TRUE;
        }
        if ((keys & PAD_KEY_UP) && screen->cursorRow != 0) {
            screen->lastRowLeft = 0;
            screen->cursorRow--;
            return TRUE;
        }
        if ((keys & PAD_KEY_DOWN) && screen->cursorRow < 2) {
            screen->lastRowLeft = 0;
            screen->cursorRow++;
            return TRUE;
        }
        break;
    case 2:
        keys = gSystem.newKeys;
        if (keys & PAD_KEY_LEFT) {
            screen->cursorRow = screen->lastRowRight;
            screen->cursorArea--;
            return TRUE;
        }
        if (screen->mode == 2) {
            return FALSE;
        }
        if ((keys & (PAD_KEY_RIGHT | PAD_KEY_DOWN)) && screen->mode != 0) {
            screen->cursorArea++;
            return TRUE;
        }
        break;
    case 3:
        keys = gSystem.newKeys;
        if (keys & (PAD_KEY_LEFT | PAD_KEY_UP)) {
            screen->cursorArea--;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void ApricornBoxScreen_SwitchColumn(ApricornBoxScreen *screen, int direction) {
    u8 row;
    if (direction == 0) {
        screen->lastRowLeft = screen->cursorRow;
        screen->cursorArea = 1;
        row = screen->cursorRow;
        switch (row) {
        case 1:
        case 2:
            if (row != screen->lastRowRight && row - 1 != screen->lastRowRight) {
                screen->cursorRow = 1;
            } else {
                screen->cursorRow = screen->lastRowRight;
            }
            break;
        case 3:
            screen->cursorRow = 2;
            break;
        case 0:
        default:
            screen->cursorRow = 0;
            break;
        }
    } else {
        screen->lastRowRight = screen->cursorRow;
        screen->cursorArea = 0;
        switch (screen->cursorRow) {
        case 0:
            row = screen->lastRowLeft;
            if (row > 1) {
                screen->cursorRow = 1;
            } else {
                screen->cursorRow = row;
            }
            break;
        case 1:
            row = screen->lastRowLeft;
            if (row == 0) {
                screen->cursorRow = 1;
            } else if (row == 3) {
                screen->cursorRow = 2;
            } else {
                screen->cursorRow = row;
            }
            break;
        case 2:
            row = screen->lastRowLeft;
            if (row < 2) {
                screen->cursorRow = 2;
            } else {
                screen->cursorRow = row;
            }
            break;
        default:
            screen->cursorRow = 0;
            break;
        }
    }
}

// A party Pokemon that can drink the juice.
typedef struct AprijuiceMon {
    Pokemon *mon;                                  // 0x00
    u16 species;                                   // 0x04
    u8 level;                                      // 0x06
    u8 gender;                                     // 0x07
    u8 slot;                                       // 0x08
    u8 hFlip;                                     // 0x09
    u8 filler_0A[2];                               // 0x0A
    String *name;                                  // 0x0C
    PartyExtraSub aprijuice;                       // 0x10
    u8 filler_15;                                  // 0x15
    struct PokeathlonPerformanceStars stars;       // 0x16
    struct PokeathlonTodayPerformance performance; // 0x1E
} AprijuiceMon;                                    // size: 0x34

// The 3D part of the Aprijuice screen: the Pokemon picture and the particle effect.
typedef struct AprijuiceScene {
    enum HeapID heapId;             // 0x00
    GF3DVramMan *vramMan;           // 0x04
    Camera *camera;                 // 0x08
    PokepicManager *pokepicManager; // 0x0C
    u8 filler_10[0x10];             // 0x10
    Pokepic *pokepic;               // 0x20
    SPLEmitter *emitter;            // 0x24
} AprijuiceScene;                   // size: 0x28

// The Aprijuice screen (OVY_59 second app): give the juice to a party Pokemon and show its Performance change.
typedef struct AprijuiceScreen {
    enum HeapID heapId;                         // 0x000
    ApricornBoxApp *app;                        // 0x004
    SaveData *saveData;                         // 0x008
    u8 filler_0C[4];                            // 0x00C
    Party *party;                               // 0x010
    SaveApricornBox *apricornBox;               // 0x014
    ApricornJuice juiceRecord;             // 0x018
    ApricornBoxJuice juice;                     // 0x020
    struct PokeathlonPerformanceStars oldStars; // 0x02E
    u8 stat;                                    // 0x036
    u8 starCount;                               // 0x037
    u8 starsShown;                              // 0x038
    s8 starDelta[5];                            // 0x039
    u8 numChanged;                              // 0x03E
    u8 filler_3F;                               // 0x03F
    u16 subState;                               // 0x040
    u16 animState;                              // 0x042
    MenuInputState menuInputState;              // 0x044
    u8 juiceUsed;                               // 0x048
    u8 numMons;                                 // 0x049
    u8 selectedMon;                             // 0x04A
    u8 textFrameDelay;                          // 0x04B
    u8 frame;                                   // 0x04C
    u8 textPrinterId;                           // 0x04D
    u8 filler_4E;                               // 0x04E
    u8 activeTasks;                             // 0x04F
    u8 filler_50;                               // 0x050
    u8 timer;                                   // 0x051
    u8 filler_52[2];                            // 0x052
    BgConfig *bgConfig;                         // 0x054
    u8 filler_58[4];                            // 0x058
    MsgData *msgData;                           // 0x05C
    MessageFormat *msgFormat;                   // 0x060
    String *string;                             // 0x064
    String *string2;                            // 0x068
    String *levelTemplate;                      // 0x06C, message 4
    String *genderSymbols[2];                   // 0x070, messages 10-11
    AprijuiceScene *scene;                      // 0x078
    AprijuiceMon mons[PARTY_SIZE];              // 0x07C
    Window windows[10];                         // 0x1B4
    YesNoPrompt *yesNoPrompt;                   // 0x254
    SpriteSystem *spriteSystem;                 // 0x258
    SpriteManager *spriteManager;               // 0x25C
    Sprite *sprites[34];                        // 0x260
    NARC *narc;                                 // 0x2E8
    u8 filler_2EC[8];                           // 0x2EC
} AprijuiceScreen;                              // size: 0x2F4

// Pops a Performance star in (grows and spins) or out (shrinks and hides it) (task).
typedef struct AprijuiceStarTask {
    AprijuiceScreen *owner; // 0x00
    u8 hide;                // 0x04
    u8 filler_05;           // 0x05
    u8 timer;               // 0x06
    u8 filler_07;           // 0x07
    fx32 scaleStep;         // 0x08
    u16 angle;              // 0x0C
    s16 angleStep;          // 0x0E
    VecFx32 scale;          // 0x10
    Sprite *sprite;         // 0x1C
} AprijuiceStarTask;        // size: 0x20

// Shakes the Pokemon picture (task).
typedef struct AprijuiceShakeTask {
    AprijuiceScreen *owner; // 0x00
    u8 counter;             // 0x04
    u8 phase;               // 0x05
    u8 delta;               // 0x06
} AprijuiceShakeTask;       // size: 0x08

// Particle effect and white fade of the Pokemon picture (task).
typedef struct AprijuiceFadeTask {
    AprijuiceScreen *owner; // 0x00
    u8 timer;               // 0x04
    u8 state;               // 0x05
} AprijuiceFadeTask;        // size: 0x08

void AprijuiceScreen_InitFromArgs(AprijuiceScreen *screen);
void AprijuiceScreen_WriteResult(AprijuiceScreen *screen);
BOOL AprijuiceScreen_Setup(AprijuiceScreen *screen);
BOOL AprijuiceScreen_Teardown(AprijuiceScreen *screen);
int AprijuiceScreen_FadeIn(AprijuiceScreen *screen);
int AprijuiceScreen_FadeOut(AprijuiceScreen *screen);
int AprijuiceScreen_InputState(AprijuiceScreen *screen);
int AprijuiceScreen_DrinkState(AprijuiceScreen *screen);
int AprijuiceScreen_StatChangesState(AprijuiceScreen *screen);
void AprijuiceScreen_ResetDisplay(void);
void AprijuiceScreen_InitGraphics(AprijuiceScreen *screen);
void AprijuiceScreen_FreeGraphics(AprijuiceScreen *screen);
void AprijuiceScreen_DrawScreen(AprijuiceScreen *screen);
void AprijuiceScreen_LoadParty(AprijuiceScreen *screen);
void AprijuiceScreen_FreeParty(AprijuiceScreen *screen);
void AprijuiceScreen_DrawButtons(AprijuiceScreen *screen, u32 state);
int AprijuiceScreen_HandleInput(AprijuiceScreen *screen);
BOOL AprijuiceScreen_PlayDrinkAnim(AprijuiceScreen *screen);
BOOL AprijuiceScreen_PrintNothingChanged(AprijuiceScreen *screen);
BOOL AprijuiceScreen_ShowStatChange(AprijuiceScreen *screen);
int AprijuiceScreen_Select(AprijuiceScreen *screen, int input);
int AprijuiceScreen_HandleKeys(AprijuiceScreen *screen);
int AprijuiceScreen_HandleTouch(AprijuiceScreen *screen, int *hit);
void AprijuiceScreen_VBlankCB(AprijuiceScreen *screen);
void AprijuiceScreen_SetGfxBanks(void);
void AprijuiceScreen_InitBgs(AprijuiceScreen *screen);
void AprijuiceScreen_FreeBgs(AprijuiceScreen *screen);
void AprijuiceScreen_LoadBgGraphics(AprijuiceScreen *screen);
void AprijuiceScreen_FreeBgGraphics(AprijuiceScreen *screen);
void AprijuiceScreen_InitStrings(AprijuiceScreen *screen);
void AprijuiceScreen_FreeStrings(AprijuiceScreen *screen);
void AprijuiceScreen_AddWindows(AprijuiceScreen *screen);
void AprijuiceScreen_RemoveWindows(AprijuiceScreen *screen);
void AprijuiceScreen_InitSpriteSystem(AprijuiceScreen *screen);
void AprijuiceScreen_FreeSpriteSystem(AprijuiceScreen *screen);
void AprijuiceScreen_InitSprites(AprijuiceScreen *screen);
void AprijuiceScreen_FreeSprites(AprijuiceScreen *screen);
void AprijuiceScreen_CreateSprites(AprijuiceScreen *screen);
void AprijuiceScreen_DeleteSprites(AprijuiceScreen *screen);
void AprijuiceMon_Init(AprijuiceMon *entry, Party *party, Pokemon *mon, u8 slot, enum HeapID heapId);
void AprijuiceMon_Free(AprijuiceMon *entry);
void AprijuiceScreen_UseJuice(AprijuiceScreen *screen);
void AprijuiceScreen_DrawMessageBox(AprijuiceScreen *screen);
void AprijuiceScreen_PrintMessage(AprijuiceScreen *screen, int message);
void AprijuiceScreen_ShowMon(AprijuiceScreen *screen, int index);
void AprijuiceScreen_DrawStatStars(AprijuiceScreen *screen, AprijuiceMon *entry, u8 stat);
void AprijuiceScreen_DrawAllStars(AprijuiceScreen *screen, AprijuiceMon *entry);
void AprijuiceScreen_SetStarSprite(AprijuiceScreen *screen, int index, u8 sequence, int draw);
void AprijuiceScreen_StartStarTask(AprijuiceScreen *screen, int stat, int shrink);
void AprijuiceScreen_StarTask(SysTask *task, AprijuiceStarTask *star);
void AprijuiceScreen_StartShakeTask(AprijuiceScreen *screen);
void AprijuiceScreen_ShakeTask(SysTask *task, AprijuiceShakeTask *shake);
void AprijuiceScreen_StartFadeTask(AprijuiceScreen *screen);
void AprijuiceScreen_FadeTask(SysTask *task, AprijuiceFadeTask *fade);
AprijuiceScene *AprijuiceScene_New(enum HeapID heapId);
void AprijuiceScene_Draw(AprijuiceScene *scene);
void AprijuiceScene_Delete(AprijuiceScene *scene);
void AprijuiceScene_TransferPokepicGfx(AprijuiceScene *scene);
void AprijuiceScene_CreatePokepic(AprijuiceScene *scene, AprijuiceMon *entry);
void AprijuiceScene_DeletePokepic(AprijuiceScene *scene);
void AprijuiceScene_StartParticles(AprijuiceScene *scene, int resource);
BOOL AprijuiceScene_AreParticlesActive(AprijuiceScene *scene);
void AprijuiceScene_Init3D(void);
void AprijuiceScene_ReserveVram(GF3DVramMan *vramMan);
u32 AprijuiceScene_AllocTexVram(u32 size, BOOL is4x4);
u32 AprijuiceScene_AllocPlttVram(u32 size, BOOL is4color);
void AprijuiceScene_SetEmitterPosition(SPLEmitter *emitter);
SPLEmitter *AprijuiceScene_CreateParticleSystem(enum HeapID heapId);
void AprijuiceScene_InitParticles(AprijuiceScene *scene);
void AprijuiceScene_FreeParticles(AprijuiceScene *scene);
void AprijuiceScene_InitCamera(AprijuiceScene *scene, enum HeapID heapId);

BOOL AprijuiceScreen_Init(OverlayManager *man, int *state) {
    AprijuiceScreen *screen;
    switch (*state) {
    case 0:
        AprijuiceScreen_ResetDisplay();
        Heap_Create(HEAP_ID_3, HEAP_ID_134, 9 << 16);
        screen = OverlayManager_CreateAndGetData(man, sizeof(AprijuiceScreen), HEAP_ID_134);
        MI_CpuFill8(screen, 0, sizeof(AprijuiceScreen));
        screen->heapId = HEAP_ID_134;
        screen->app = OverlayManager_GetArgs(man);
        AprijuiceScreen_InitFromArgs(screen);
        (*state)++;
        break;
    case 1:
        screen = OverlayManager_GetData(man);
        if (AprijuiceScreen_Setup(screen)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL AprijuiceScreen_Main(OverlayManager *man, int *state) {
    AprijuiceScreen *screen = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        *state = AprijuiceScreen_FadeIn(screen);
        break;
    case 1:
        *state = AprijuiceScreen_FadeOut(screen);
        break;
    case 2:
        *state = AprijuiceScreen_InputState(screen);
        break;
    case 4:
        *state = AprijuiceScreen_DrinkState(screen);
        break;
    case 5:
        *state = AprijuiceScreen_StatChangesState(screen);
        break;
    case 8:
        return TRUE;
    }
    SpriteSystem_DrawSprites(screen->spriteManager);
    AprijuiceScene_Draw(screen->scene);
    return FALSE;
}

BOOL AprijuiceScreen_Exit(OverlayManager *man, int *state) {
    AprijuiceScreen *screen = OverlayManager_GetData(man);
    if (!AprijuiceScreen_Teardown(screen)) {
        return FALSE;
    }
    AprijuiceScreen_WriteResult(screen);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_134);
    return TRUE;
}

void AprijuiceScreen_InitFromArgs(AprijuiceScreen *screen) {
    int i;
    Options *options;
    u8 *entry;
    screen->menuInputState = MenuInputStateMgr_GetState(screen->app->args->menuInputStatePtr);
    screen->saveData = screen->app->args->saveData;
    options = Save_PlayerData_GetOptionsAddr(screen->saveData);
    screen->textFrameDelay = Options_GetTextFrameDelay(options);
    screen->frame = Options_GetFrame(options);
    screen->apricornBox = Save_ApricornBox_Get(screen->saveData);
    ApricornBox_GetJuice(screen->apricornBox, screen->app->juiceIndex, &screen->juiceRecord);
    ApricornJuice_RankFlavors(&screen->juiceRecord, &screen->juice);
    AprijuiceScreen_LoadParty(screen);
    i = 0;
    if (i < screen->numMons) {
        // a byte cursor over the entries, as in the original
        entry = (u8 *)screen;
        do {
            if (screen->app->partySlot == ((AprijuiceScreen *)entry)->mons[0].slot) {
                screen->selectedMon = i;
                return;
            }
            i++;
            entry += sizeof(AprijuiceMon);
        } while (i < screen->numMons);
    }
}

void AprijuiceScreen_WriteResult(AprijuiceScreen *screen) {
    AprijuiceMon *entry;
    AprijuiceScreen_FreeParty(screen);
    MenuInputStateMgr_SetState(screen->app->args->menuInputStatePtr, screen->menuInputState);
    entry = &screen->mons[screen->selectedMon];
    screen->app->partySlot = entry->slot;
    screen->app->mode = screen->juiceUsed != 0 ? 1 : 3;
    if (screen->app->args->juiceUsedVar != NULL) {
        *screen->app->args->juiceUsedVar = screen->juiceUsed;
    }
}

BOOL AprijuiceScreen_Setup(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        AprijuiceScreen_InitGraphics(screen);
        screen->subState++;
        break;
    case 1:
        AprijuiceScreen_DrawScreen(screen);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(1);
        screen->subState = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL AprijuiceScreen_Teardown(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        GF_TouchpadStopSampling();
        AprijuiceScreen_FreeGraphics(screen);
        screen->subState++;
        break;
    case 1:
    default:
        AprijuiceScreen_ResetDisplay();
        screen->subState = 0;
        return TRUE;
    }
    return FALSE;
}

int AprijuiceScreen_FadeIn(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, screen->heapId);
        screen->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            screen->subState = 0;
            return 2;
        }
        break;
    }
    return 0;
}

int AprijuiceScreen_FadeOut(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, screen->heapId);
        screen->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            screen->subState = 0;
            return 8;
        }
        break;
    }
    return 1;
}

int AprijuiceScreen_InputState(AprijuiceScreen *screen) {
    return AprijuiceScreen_HandleInput(screen);
}

int AprijuiceScreen_DrinkState(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        if (AprijuiceScreen_PlayDrinkAnim(screen)) {
            if (screen->numChanged != 0) {
                screen->subState = 0;
                screen->stat = 0;
                return 5;
            }
            screen->subState++;
        }
        break;
    case 1:
        if (AprijuiceScreen_PrintNothingChanged(screen)) {
            screen->subState = 0;
            return 1;
        }
        break;
    }
    return 4;
}

int AprijuiceScreen_StatChangesState(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        while (screen->stat < 5) {
            if (screen->starDelta[screen->stat] != 0) {
                break;
            }
            screen->stat++;
        }
        screen->subState++;
        // fallthrough
    case 1:
        if (AprijuiceScreen_ShowStatChange(screen)) {
            screen->subState = 0;
            screen->stat++;
            screen->numChanged--;
            if (screen->numChanged == 0) {
                return 1;
            }
        }
        break;
    }
    return 5;
}

void AprijuiceScreen_ResetDisplay(void) {
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

void AprijuiceScreen_InitGraphics(AprijuiceScreen *screen) {
    AprijuiceScreen_InitBgs(screen);
    AprijuiceScreen_LoadBgGraphics(screen);
    AprijuiceScreen_InitStrings(screen);
    AprijuiceScreen_AddWindows(screen);
    AprijuiceScreen_InitSprites(screen);
    screen->scene = AprijuiceScene_New(screen->heapId);
    Main_SetVBlankIntrCB((GFIntrCB)AprijuiceScreen_VBlankCB, screen);
}

void AprijuiceScreen_FreeGraphics(AprijuiceScreen *screen) {
    AprijuiceScene_Delete(screen->scene);
    AprijuiceScreen_FreeSprites(screen);
    AprijuiceScreen_RemoveWindows(screen);
    AprijuiceScreen_FreeStrings(screen);
    AprijuiceScreen_FreeBgGraphics(screen);
    AprijuiceScreen_FreeBgs(screen);
}

#pragma force_active on

void AprijuiceScreen_DrawScreen(AprijuiceScreen *screen) {
    int i;
    Window *windows;
    AprijuiceScreen_DrawButtons(screen, 2);
    ReadMsgDataIntoString(screen->msgData, 0, screen->string2);
    AddTextPrinterParameterizedWithColor(&screen->windows[3], 0, screen->string2, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&screen->windows[3]);
    i = 0;
    windows = screen->windows;
    for (; i < 5; i++) {
        ReadMsgDataIntoString(screen->msgData, sAprijuiceStatOrder[i] + 5, screen->string2);
        AddTextPrinterParameterizedWithColor(&windows[i + 5], 0, screen->string2, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&windows[i + 5]);
    }
    AprijuiceScreen_ShowMon(screen, screen->selectedMon);
}

void AprijuiceScreen_LoadParty(AprijuiceScreen *screen) {
    int count;
    int i;
    int valid;
    u8 *entries;
    screen->party = SaveArray_Party_Get(screen->saveData);
    count = Party_GetCount(screen->party);
    valid = 0;
    i = valid;
    if (count > 0) {
        entries = (u8 *)screen;
        entries += offsetof(AprijuiceScreen, mons);
        do {
            Pokemon *mon = Party_GetMonByIndex(screen->party, i);
            if (GetMonData(mon, MON_DATA_IS_EGG, NULL) == 0) {
                AprijuiceMon_Init((AprijuiceMon *)(entries + valid++ * sizeof(AprijuiceMon)), screen->party, mon, (u8)i, screen->heapId);
            }
            i++;
        } while (i < count);
    }
    screen->numMons = valid;
}

#pragma force_active reset

void AprijuiceScreen_FreeParty(AprijuiceScreen *screen) {
    int i;
    int count;
    AprijuiceMon *entry;
    i = 0;
    count = screen->numMons;
    if (count > 0) {
        entry = screen->mons;
        do {
            AprijuiceMon_Free(entry);
            i++;
            entry++;
        } while (i < screen->numMons);
    }
}

#pragma force_active on

void AprijuiceScreen_DrawButtons(AprijuiceScreen *screen, u32 state) {
    int i;
    switch (state) {
    case 3:
        ClearWindowTilemapAndScheduleTransfer(&screen->windows[1]);
        Sprite_SetDrawFlag(screen->sprites[0], FALSE);
        Sprite_SetDrawFlag(screen->sprites[1], FALSE);
        break;
    case 2:
        FillWindowPixelBuffer(&screen->windows[1], 0);
        for (i = 0; i < 2; i++) {
            ReadMsgDataIntoString(screen->msgData, i + 1, screen->string2);
            AddTextPrinterParameterizedWithColor(&screen->windows[1], 4, screen->string2, 0, 4 + i * 24, TEXT_SPEED_NOTRANSFER, sAprijuiceButtonColors[i], NULL);
            Sprite_SetDrawFlag(screen->sprites[i], TRUE);
        }
        ScheduleWindowCopyToVram(&screen->windows[1]);
        break;
    case 0:
    case 1:
        Sprite_SetAnimCtrlSeq(screen->sprites[state], state * 2 + 1);
        Sprite_ResetAnimCtrlState(screen->sprites[state]);
        break;
    }
}

int AprijuiceScreen_HandleInput(AprijuiceScreen *screen) {
    int hit = 0;
    int result = AprijuiceScreen_HandleTouch(screen, &hit);
    if (hit == 0) {
        result = AprijuiceScreen_HandleKeys(screen);
    }
    return result;
}

#pragma force_active reset

BOOL AprijuiceScreen_PlayDrinkAnim(AprijuiceScreen *screen) {
    switch (screen->animState) {
    case 0:
        if (Sprite_IsAnimated(screen->sprites[0])) {
            return FALSE;
        }
        AprijuiceScreen_DrawButtons(screen, 3);
        AprijuiceScreen_DrawMessageBox(screen);
        AprijuiceScreen_PrintMessage(screen, 0);
        Sprite_SetDrawFlag(screen->sprites[2], TRUE);
        Sprite_SetAnimCtrlSeq(screen->sprites[2], 10);
        Sprite_ResetAnimCtrlState(screen->sprites[2]);
        PlaySE(SEQ_SE_GS_POT_HYOUJI);
        screen->animState++;
        break;
    case 1:
        if (Sprite_IsAnimated(screen->sprites[2])) {
            return FALSE;
        }
        Sprite_SetAnimCtrlSeq(screen->sprites[2], 11);
        Sprite_ResetAnimCtrlState(screen->sprites[2]);
        screen->animState++;
        break;
    case 2:
        if (Sprite_IsAnimated(screen->sprites[2])) {
            return FALSE;
        }
        AprijuiceScreen_StartShakeTask(screen);
        screen->animState++;
        break;
    case 3:
        if (screen->activeTasks != 0) {
            return FALSE;
        }
        Sprite_SetDrawFlag(screen->sprites[2], FALSE);
        AprijuiceScreen_StartFadeTask(screen);
        screen->animState++;
        break;
    case 4:
        if (screen->activeTasks != 0) {
            return FALSE;
        }
        screen->animState = 6;
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 2, screen->heapId);
            screen->animState++;
        }
        break;
    default:
        if (IsPaletteFadeFinished()) {
            screen->animState = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL AprijuiceScreen_PrintNothingChanged(AprijuiceScreen *screen) {
    switch (screen->animState) {
    case 0:
        AprijuiceScreen_DrawMessageBox(screen);
        AprijuiceScreen_PrintMessage(screen, 1);
        screen->animState++;
        break;
    case 1:
        if (!TextPrinterCheckActive(screen->textPrinterId)) {
            screen->animState = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL AprijuiceScreen_ShowStatChange(AprijuiceScreen *screen) {
    u32 stat = sAprijuiceStatOrder[screen->stat];
    switch (screen->animState) {
    case 0: {
        s8 delta;
        screen->timer = 0;
        screen->starsShown = 0;
        delta = screen->starDelta[screen->stat];
        if (delta < 0) {
            screen->starCount = -delta;
            PlaySE(SEQ_SE_GS_PARAMETER_DOWN);
        } else {
            screen->starCount = delta;
            PlaySE(SEQ_SE_GS_PARAMETER_UP);
        }
        screen->animState++;
    }
    // fallthrough
    case 1:
        if (screen->timer-- != 0) {
            break;
        }
        if (screen->starDelta[screen->stat] < 0) {
            AprijuiceScreen_StartStarTask(screen, (u8)stat, 1);
        } else {
            AprijuiceScreen_StartStarTask(screen, (u8)stat, 0);
        }
        if (screen->starsShown >= screen->starCount) {
            screen->animState++;
        } else {
            screen->timer = 15;
        }
        break;
    case 2:
        if (screen->activeTasks != 0) {
            break;
        }
        AprijuiceScreen_DrawStatStars(screen, &screen->mons[screen->selectedMon], (u8)stat);
        AprijuiceScreen_DrawMessageBox(screen);
        if (screen->starDelta[screen->stat] < 0) {
            AprijuiceScreen_PrintMessage(screen, stat + 7);
        } else {
            AprijuiceScreen_PrintMessage(screen, stat + 2);
        }
        screen->animState++;
        break;
    default:
        if (TextPrinterCheckActive(screen->textPrinterId)) {
            break;
        }
        screen->animState = 0;
        return TRUE;
    }
    return FALSE;
}

int AprijuiceScreen_Select(AprijuiceScreen *screen, int input) {
    switch (input) {
    case 0:
        AprijuiceScreen_UseJuice(screen);
        PlaySE(SEQ_SE_DP_SELECT);
        AprijuiceScreen_DrawButtons(screen, 0);
        return 4;
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        AprijuiceScreen_DrawButtons(screen, 1);
        return 1;
    case 2:
        screen->selectedMon = (screen->selectedMon - 1 + screen->numMons) % screen->numMons;
        PlaySE(SEQ_SE_DP_SELECT);
        AprijuiceScreen_ShowMon(screen, screen->selectedMon);
        return 2;
    case 3:
        screen->selectedMon = (screen->selectedMon + 1) % screen->numMons;
        PlaySE(SEQ_SE_DP_SELECT);
        AprijuiceScreen_ShowMon(screen, screen->selectedMon);
        return 2;
    default:
        return 2;
    }
}

int AprijuiceScreen_HandleKeys(AprijuiceScreen *screen) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return AprijuiceScreen_Select(screen, 1);
    }
    if (keys & PAD_BUTTON_A) {
        return AprijuiceScreen_Select(screen, 0);
    }
    if (keys & PAD_KEY_UP) {
        return AprijuiceScreen_Select(screen, 2);
    }
    if (keys & PAD_KEY_DOWN) {
        return AprijuiceScreen_Select(screen, 3);
    }
    return 2;
}

int AprijuiceScreen_HandleTouch(AprijuiceScreen *screen, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(sAprijuiceHitboxes);
    if (input == -1) {
        return 2;
    }
    *hit = 1;
    screen->menuInputState = MENU_INPUT_STATE_TOUCH;
    return AprijuiceScreen_Select(screen, input);
}

void AprijuiceScreen_VBlankCB(AprijuiceScreen *screen) {
    if (screen->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    AprijuiceScene_TransferPokepicGfx(screen->scene);
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(screen->bgConfig);
    ((vu32 *)0x027E0000)[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

void AprijuiceScreen_SetGfxBanks(void) {
    GraphicsBanks banks = sAprijuiceGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void AprijuiceScreen_InitBgs(AprijuiceScreen *screen) {
    GraphicsModes modes;
    BgTemplate subBg3;
    BgTemplate mainBg1;
    BgTemplate mainBg2;
    BgTemplate mainBg3;
    AprijuiceScreen_SetGfxBanks();
    screen->bgConfig = BgConfig_Alloc(screen->heapId);
    modes = sAprijuiceGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    subBg3 = sAprijuiceBgTemplate_Sub3;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_SUB_3, &subBg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_SUB_3);
    mainBg1 = sAprijuiceBgTemplate_Main1;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_1, &mainBg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_1);
    mainBg2 = sAprijuiceBgTemplate_Main2;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_2, &mainBg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_2);
    mainBg3 = sAprijuiceBgTemplate_Main3;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_3, &mainBg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, screen->heapId);
}

void AprijuiceScreen_FreeBgs(AprijuiceScreen *screen) {
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(screen->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}

void AprijuiceScreen_LoadBgGraphics(AprijuiceScreen *screen) {
    NARC *narc = NARC_New(NARC_a_2_4_2, screen->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, screen->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, screen->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, screen->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, screen->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, screen->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, screen->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, screen->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, screen->heapId);
    screen->narc = narc;
    LoadUserFrameGfx2(screen->bgConfig, GF_BG_LYR_MAIN_1, 1, 13, screen->frame, screen->heapId);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, screen->heapId);
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, 1, 14, 6, 26);
}

void AprijuiceScreen_FreeBgGraphics(AprijuiceScreen *screen) {
    NARC_Delete(screen->narc);
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, 0, 0, 31, 0);
}

void AprijuiceScreen_InitStrings(AprijuiceScreen *screen) {
    int i;
    FontID_Alloc(4, screen->heapId);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    screen->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0022_bin, screen->heapId);
    screen->msgFormat = MessageFormat_New_Custom(16, 16, screen->heapId);
    screen->string = String_New(256, screen->heapId);
    screen->string2 = String_New(256, screen->heapId);
    screen->levelTemplate = NewString_ReadMsgData(screen->msgData, 4);
    i = 0;
    do {
        screen->genderSymbols[i] = NewString_ReadMsgData(screen->msgData, i + 10);
        i++;
    } while (i < 2);
}

void AprijuiceScreen_FreeStrings(AprijuiceScreen *screen) {
    String_Delete(screen->genderSymbols[1]);
    String_Delete(screen->genderSymbols[0]);
    String_Delete(screen->levelTemplate);
    String_Delete(screen->string2);
    String_Delete(screen->string);
    MessageFormat_Delete(screen->msgFormat);
    DestroyMsgData(screen->msgData);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    FontID_Release(4);
}

void AprijuiceScreen_AddWindows(AprijuiceScreen *screen) {
    int i;
    int tile;
    int y;
    for (i = 0; i <= 4; i++) {
        AddWindow(screen->bgConfig, &screen->windows[i], &sAprijuiceWindowTemplates[i]);
        FillWindowPixelBuffer(&screen->windows[i], 0);
    }
    tile = 0xC1;
    i = 0;
    y = 4;
    for (; i < 5; i++) {
        AddWindowParameterized(screen->bgConfig, &screen->windows[i + 5], GF_BG_LYR_MAIN_2, 1, (u8)y, 16, 2, 12, (u16)tile);
        FillWindowPixelBuffer(&screen->windows[i + 5], 0);
        tile += 0x20;
        y += 4;
    }
    screen->yesNoPrompt = YesNoPrompt_Create(screen->heapId);
}

void AprijuiceScreen_RemoveWindows(AprijuiceScreen *screen) {
    int i;
    Window *window;
    i = 0;
    window = screen->windows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        i++;
        window++;
    } while (i < 10);
    YesNoPrompt_Destroy(screen->yesNoPrompt);
}

void AprijuiceScreen_InitSpriteSystem(AprijuiceScreen *screen) {
    GF_CreateVramTransferManager(32, screen->heapId);
    screen->spriteSystem = SpriteSystem_Alloc(screen->heapId);
    SpriteSystem_Init(screen->spriteSystem, &sAprijuiceOamManagerParam, &sAprijuiceOamCharTransferParam, 3);
    thunk_ClearMainOAM(screen->heapId);
    thunk_ClearSubOAM(screen->heapId);
    screen->spriteManager = SpriteManager_New(screen->spriteSystem);
    SpriteSystem_InitSprites(screen->spriteSystem, screen->spriteManager, 34);
    SpriteSystem_LoadResourceDataFromFileIdListEx(screen->spriteSystem, screen->spriteManager, &sAprijuiceSpriteResources, 0, 0);
}

void AprijuiceScreen_FreeSpriteSystem(AprijuiceScreen *screen) {
    SpriteSystem_DestroySpriteManager(screen->spriteSystem, screen->spriteManager);
    screen->spriteManager = NULL;
    SpriteSystem_Free(screen->spriteSystem);
    screen->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(screen->heapId);
}

void AprijuiceScreen_InitSprites(AprijuiceScreen *screen) {
    AprijuiceScreen_InitSpriteSystem(screen);
    AprijuiceScreen_CreateSprites(screen);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void AprijuiceScreen_FreeSprites(AprijuiceScreen *screen) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    AprijuiceScreen_DeleteSprites(screen);
    AprijuiceScreen_FreeSpriteSystem(screen);
}

void AprijuiceScreen_CreateSprites(AprijuiceScreen *screen) {
    int i;
    int j;
    const u32 *stat;
    int yIcon;
    int yStars;
    int x;
    u8 *slot;
    // the sprite slots are addressed through byte cursors, as in the original
    u8 *cursor = (u8 *)screen;
    const UnmanagedSpriteTemplate *template = sAprijuiceSpriteTemplates;
    i = 0;
    cursor = (u8 *)screen;
    do {
        ((AprijuiceScreen *)cursor)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, template);
        Sprite_SetDrawFlag(((AprijuiceScreen *)cursor)->sprites[0], TRUE);
        Sprite_SetAnimActiveFlag(((AprijuiceScreen *)cursor)->sprites[0], TRUE);
        Sprite_SetPriority(((AprijuiceScreen *)cursor)->sprites[0], 1);
        template ++;
        cursor += sizeof(Sprite *);
        i++;
    } while (i <= 2);
    Sprite_SetDrawFlag(screen->sprites[2], FALSE);
    Sprite_SetAffineOverwriteMode(screen->sprites[2], 2);
    i = 0;
    stat = sAprijuiceStatOrder;
    yIcon = 40;
    yStars = 56;
    do {
        slot = (u8 *)screen + (u8)(*stat + 3) * sizeof(Sprite *);
        ((AprijuiceScreen *)slot)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sAprijuiceModifierSpriteTemplate);
        Sprite_SetPositionXY(((AprijuiceScreen *)slot)->sprites[0], 80, (s16)yIcon);
        Sprite_SetAnimActiveFlag(((AprijuiceScreen *)slot)->sprites[0], TRUE);
        Sprite_SetDrawFlag(((AprijuiceScreen *)slot)->sprites[0], TRUE);
        j = 0;
        x = 64;
        do {
            slot = (u8 *)screen + (u8)(*stat * 5 + 8 + j) * sizeof(Sprite *);
            ((AprijuiceScreen *)slot)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sAprijuiceStarSpriteTemplate);
            Sprite_SetPositionXY(((AprijuiceScreen *)slot)->sprites[0], (s16)x, (s16)yStars);
            Sprite_SetAnimActiveFlag(((AprijuiceScreen *)slot)->sprites[0], TRUE);
            Sprite_SetDrawFlag(((AprijuiceScreen *)slot)->sprites[0], TRUE);
            Sprite_SetAffineOverwriteMode(((AprijuiceScreen *)slot)->sprites[0], 2);
            j++;
            x += 16;
        } while (j < 5);
        stat++;
        yIcon += 32;
        yStars += 32;
        i++;
    } while (i < 5);
}

void AprijuiceScreen_DeleteSprites(AprijuiceScreen *screen) {
    int i;
    for (i = 0; i < 34; i++) {
        if (screen->sprites[i] != NULL) {
            Sprite_Delete(screen->sprites[i]);
        }
    }
}

void AprijuiceMon_Init(AprijuiceMon *entry, Party *party, Pokemon *mon, u8 slot, enum HeapID heapId) {
    entry->mon = mon;
    entry->slot = slot;
    entry->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    entry->gender = GetMonData(mon, MON_DATA_GENDER, NULL);
    entry->level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    entry->hFlip = GetMonBaseStat_HandleAlternateForm(entry->species, GetMonData(mon, MON_DATA_FORM, NULL), BASE_FLIP) ^ 1;
    entry->name = String_New(12, heapId);
    GetMonData(mon, MON_DATA_NICKNAME_STRING, entry->name);
    CalcMonPokeathlonPerformance(entry->mon, &entry->performance);
    Party_GetMonAprijuiceModifiers(party, &entry->aprijuice, entry->slot);
}

void AprijuiceMon_Free(AprijuiceMon *entry) {
    String_Delete(entry->name);
}

void AprijuiceScreen_UseJuice(AprijuiceScreen *screen) {
    int i;
    AprijuiceMon *entry = &screen->mons[screen->selectedMon];
    ApricornBox_FeedAprijuice(&screen->juiceRecord, screen->party, entry->slot);
    if (screen->app->juiceIndex == 8) {
        ApricornBox_UseJuiceServing(screen->apricornBox);
    }
    Party_GetMonAprijuiceModifiers(screen->party, &entry->aprijuice, entry->slot);
    screen->oldStars.stars = entry->stars.stars;
    *(u16 *)&screen->oldStars.color[0] = *(u16 *)&entry->stars.color[0];
    *(u16 *)&screen->oldStars.color[2] = *(u16 *)&entry->stars.color[2];
    *(u16 *)&screen->oldStars.color[4] = *(u16 *)&entry->stars.color[4];
    CalcMonPokeathlonStars(&entry->stars, entry->mon, (const s8 *)entry->aprijuice.stats, screen->heapId);
    screen->numChanged = 0;
    for (i = 0; i < 5; i++) {
        screen->starDelta[i] = (u8)((entry->stars.stars >> (sAprijuiceStatOrder[i] * 3)) & 7) - (u8)((screen->oldStars.stars >> (sAprijuiceStatOrder[i] * 3)) & 7);
        if (screen->starDelta[i] != 0) {
            screen->numChanged++;
        }
    }
    screen->juiceUsed = 1;
}

void AprijuiceScreen_DrawMessageBox(AprijuiceScreen *screen) {
    DrawFrameAndWindow2(&screen->windows[0], TRUE, 1, 13);
    FillWindowPixelBuffer(&screen->windows[0], 15);
}

void AprijuiceScreen_PrintMessage(AprijuiceScreen *screen, int message) {
    u32 speed;
    switch (message) {
    case 0: {
        message = screen->juice.entries[0].stat;
        if (message == 6) {
            message = 12;
        } else if (message == 5) {
            message = screen->juice.unk_06[0] + 13;
        } else {
            message += 13;
        }
        speed = 0;
        break;
    }
    case 1:
        message = 18;
        speed = screen->textFrameDelay;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        message += 17;
        speed = screen->textFrameDelay;
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        message += 17;
        speed = screen->textFrameDelay;
        break;
    default:
        speed = screen->textFrameDelay;
        break;
    }
    ReadMsgDataIntoString(screen->msgData, message, screen->string2);
    screen->textPrinterId = AddTextPrinterParameterizedWithColor(&screen->windows[0], 1, screen->string2, 0, 0, speed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    if (speed == 0) {
        ScheduleWindowCopyToVram(&screen->windows[0]);
    }
}

void AprijuiceScreen_ShowMon(AprijuiceScreen *screen, int index) {
    AprijuiceMon *entry = &screen->mons[index];
    FillWindowPixelBuffer(&screen->windows[4], 0);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], 0, entry->name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    if (entry->gender != MON_GENDERLESS) {
        int gender = entry->gender;
        AddTextPrinterParameterizedWithColor(&screen->windows[4], 0, screen->genderSymbols[gender], 64, 0, TEXT_SPEED_NOTRANSFER, sAprijuiceGenderColors[gender], NULL);
    }
    BufferIntegerAsString(screen->msgFormat, 0, entry->level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(screen->msgFormat, screen->string2, screen->levelTemplate);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], 0, screen->string2, 16, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&screen->windows[4]);
    AprijuiceScene_DeletePokepic(screen->scene);
    AprijuiceScene_CreatePokepic(screen->scene, entry);
    AprijuiceScreen_DrawAllStars(screen, entry);
}

#pragma force_active on

void AprijuiceScreen_DrawStatStars(AprijuiceScreen *screen, AprijuiceMon *entry, u8 stat) {
    int i;
    int index;
    u8 level;

    // a struct of locals keeps the original stack layout
    struct {
        const u8 *bytes;
        int offset;
    } bar;

    if ((s8)entry->aprijuice.stats[stat] == 0) {
        Sprite_SetDrawFlag(screen->sprites[stat + 3], FALSE);
    } else {
        Sprite_SetDrawFlag(screen->sprites[stat + 3], TRUE);
        if ((s8)entry->aprijuice.stats[stat] > 0) {
            Sprite_SetAnimCtrlSeq(screen->sprites[stat + 3], 8);
        } else {
            Sprite_SetAnimCtrlSeq(screen->sprites[stat + 3], 9);
        }
    }
    index = stat * 5;
    bar.bytes = (const u8 *)entry + offsetof(AprijuiceMon, performance);
    bar.offset = stat * sizeof(entry->performance.stats[0]);
    level = (u8)((entry->stars.stars >> (stat * 3)) & 7);
    for (i = 0; i < 5; i++, index++) {
        if (i > (s32)(((const struct TodayPerformanceStat *)(bar.bytes + bar.offset))->hi)) {
            AprijuiceScreen_SetStarSprite(screen, index, 0xFF, 0);
        } else if (i > level) {
            AprijuiceScreen_SetStarSprite(screen, index, 7, 1);
        } else {
            AprijuiceScreen_SetStarSprite(screen, index, (u8)(entry->stars.color[stat] + 4), 1);
        }
    }
}

#pragma force_active reset

void AprijuiceScreen_DrawAllStars(AprijuiceScreen *screen, AprijuiceMon *entry) {
    s32 i;
    CalcMonPokeathlonStars(&entry->stars, entry->mon, (const s8 *)entry->aprijuice.stats, screen->heapId);
    for (i = 0; i < 5; i++) {
        AprijuiceScreen_DrawStatStars(screen, entry, (u8)i);
    }
}

void AprijuiceScreen_SetStarSprite(AprijuiceScreen *screen, int index, u8 sequence, int draw) {
    u32 offset;
    Sprite **sprites;
    index += 8;
    offset = index * sizeof(Sprite *);
    sprites = screen->sprites;
    Sprite_SetDrawFlag(*(Sprite **)((u8 *)sprites + offset), draw);
    if (sequence != 0xFF) {
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)sprites + offset), sequence);
        Sprite_ResetAnimCtrlState(*(Sprite **)((u8 *)sprites + offset));
    }
}

void AprijuiceScreen_StartStarTask(AprijuiceScreen *screen, int stat, int shrink) {
    AprijuiceStarTask *star = Heap_Alloc(screen->heapId, sizeof(AprijuiceStarTask));
    u8 level;
    u8 index;
    f32 frames;
    MI_CpuFill8(star, 0, sizeof(AprijuiceStarTask));
    star->owner = screen;
    star->hide = shrink;
    star->timer = 30;
    level = (screen->oldStars.stars >> (stat * 3)) & 7;
    if (shrink == 0) {
        index = level + screen->starsShown + 1;
        star->scale.x = 0x19A;
        star->scale.y = 0x19A;
        star->scale.z = 0;
        if (star->timer != 0) {
            frames = (f32)(star->timer << FX32_SHIFT) + 0.5f;
        } else {
            frames = (f32)(star->timer << FX32_SHIFT) - 0.5f;
        }
        star->scaleStep = FX_Div(FX32_ONE, (fx32)frames);
        star->angleStep = 12;
        star->angle = 0;
    } else {
        index = level - screen->starsShown;
        star->scale.x = 0x119A;
        star->scale.y = 0x119A;
        star->scale.z = FX32_ONE;
        if (star->timer != 0) {
            frames = (f32)(star->timer << FX32_SHIFT) + 0.5f;
        } else {
            frames = (f32)(star->timer << FX32_SHIFT) - 0.5f;
        }
        star->scaleStep = -FX_Div(FX32_ONE, (fx32)frames);
        star->angleStep -= 12;
        star->angle = 360;
    }
    index += stat * 5;
    star->sprite = screen->sprites[index + 8];
    Sprite_SetAffineScale(star->sprite, &star->scale);
    AprijuiceScreen_SetStarSprite(screen, index, screen->oldStars.color[stat] + 4, 1);
    SysTask_CreateOnMainQueue((SysTaskFunc)AprijuiceScreen_StarTask, star, 0);
    screen->activeTasks++;
    screen->starsShown++;
}

void AprijuiceScreen_StarTask(SysTask *task, AprijuiceStarTask *star) {
    AprijuiceScreen *owner = star->owner;
    star->angle += star->angleStep;
    star->scale.x += star->scaleStep;
    star->scale.y += star->scaleStep;
    star->timer--;
    if (star->timer != 0) {
        Sprite_SetAffineZRotation(star->sprite, GF_DegreeToSinCosIdx(star->angle));
        Sprite_SetAffineScale(star->sprite, &star->scale);
        return;
    }
    Sprite_SetAffineZRotation(star->sprite, 0);
    star->scale.x = FX32_ONE;
    star->scale.y = FX32_ONE;
    star->scale.z = FX32_ONE;
    Sprite_SetAffineScale(star->sprite, &star->scale);
    if (star->hide != 0) {
        Sprite_SetDrawFlag(star->sprite, FALSE);
    }
    owner->activeTasks--;
    Heap_Free(star);
    SysTask_Destroy(task);
}

void AprijuiceScreen_StartShakeTask(AprijuiceScreen *screen) {
    AprijuiceShakeTask *shake = Heap_Alloc(screen->heapId, sizeof(AprijuiceShakeTask));
    MI_CpuFill8(shake, 0, sizeof(AprijuiceShakeTask));
    shake->owner = screen;
    shake->counter = 30;
    shake->delta = 12;
    PlaySE(SEQ_SE_GS_JUICE_SIYOU);
    SysTask_CreateOnMainQueue((SysTaskFunc)AprijuiceScreen_ShakeTask, shake, 0);
    screen->activeTasks++;
}

void AprijuiceScreen_ShakeTask(SysTask *task, AprijuiceShakeTask *shake) {
    SysTask *savedTask = task;
    AprijuiceScreen *owner = shake->owner;
    if ((shake->phase % 2) == 0) {
        Pokepic_AddAttr(owner->scene->pokepic, POKEPIC_AFFINEW, shake->delta);
        Pokepic_AddAttr(owner->scene->pokepic, POKEPIC_AFFINEH, -shake->delta);
    } else {
        Pokepic_AddAttr(owner->scene->pokepic, POKEPIC_AFFINEW, -shake->delta);
        Pokepic_AddAttr(owner->scene->pokepic, POKEPIC_AFFINEH, shake->delta);
    }
    if (shake->counter++ >= 8) {
        shake->counter = 0;
        if (shake->phase++ >= 4) {
            Pokepic_SetAttr(owner->scene->pokepic, POKEPIC_AFFINEW, 0x100);
            Pokepic_SetAttr(owner->scene->pokepic, POKEPIC_AFFINEH, 0x100);
            owner->activeTasks--;
            Heap_Free(shake);
            SysTask_Destroy(savedTask);
        }
    }
}

void AprijuiceScreen_StartFadeTask(AprijuiceScreen *screen) {
    AprijuiceFadeTask *fade = Heap_Alloc(screen->heapId, sizeof(AprijuiceFadeTask));
    MI_CpuFill8(fade, 0, sizeof(AprijuiceFadeTask));
    fade->owner = screen;
    SysTask_CreateOnMainQueue((SysTaskFunc)AprijuiceScreen_FadeTask, fade, 0);
    screen->activeTasks++;
}

void AprijuiceScreen_FadeTask(SysTask *task, AprijuiceFadeTask *fade) {
    AprijuiceScreen *owner = fade->owner;
    switch (fade->state) {
    case 0:
        AprijuiceScene_StartParticles(owner->scene, 2);
        PlaySE(SEQ_SE_GS_KOUKAEFFECT);
        fade->timer = 20;
        fade->state++;
        return;
    case 1:
        if (fade->timer-- == 0) {
            Pokepic_StartPaletteFade(owner->scene->pokepic, 0, 16, 4, RGB_WHITE);
            AprijuiceScene_StartParticles(owner->scene, 1);
            AprijuiceScene_StartParticles(owner->scene, 0);
            fade->timer = 25;
            fade->state++;
            return;
        }
        break;
    case 2:
        if (fade->timer-- == 0) {
            PlaySE(SEQ_SE_GS_KIRAKIRA);
            fade->state++;
            return;
        }
        break;
    case 3:
        if (!Pokepic_ResumePaletteFade(owner->scene->pokepic) && !AprijuiceScene_AreParticlesActive(owner->scene)) {
            ParticleSystem_DeleteAllEmitters(owner->scene->emitter);
            fade->state++;
            return;
        }
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 6, owner->heapId);
        Pokepic_StartPaletteFade(owner->scene->pokepic, 16, 0, 4, RGB_WHITE);
        fade->state++;
        return;
    default:
        if (IsPaletteFadeFinished() && !Pokepic_ResumePaletteFade(owner->scene->pokepic)) {
            owner->activeTasks--;
            Heap_Free(fade);
            SysTask_Destroy(task);
        }
        break;
    }
}

AprijuiceScene *AprijuiceScene_New(enum HeapID heapId) {
    AprijuiceScene *scene = Heap_Alloc(heapId, sizeof(AprijuiceScene));
    MI_CpuFill8(scene, 0, sizeof(AprijuiceScene));
    scene->heapId = heapId;
    scene->vramMan = GF_3DVramMan_Create(heapId, 0, 2, 0, 1, AprijuiceScene_Init3D);
    AprijuiceScene_ReserveVram(scene->vramMan);
    AprijuiceScene_InitCamera(scene, heapId);
    AprijuiceScene_InitParticles(scene);
    scene->pokepicManager = PokepicManager_Create(heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 1;
    return scene;
}

void AprijuiceScene_Draw(AprijuiceScene *scene) {
    Thunk_G3X_Reset();
    ParticleSystem_DrawAll();
    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    Camera_PushLookAtToNNSGlb();
    reg_G3_MTX_MODE = GX_MTXMODE_PROJECTION;
    reg_G3_MTX_IDENTITY = 0;
    reg_G3_MTX_MODE = GX_MTXMODE_POSITION_VECTOR;
    reg_G3_MTX_IDENTITY = 0;
    NNS_G3dGlbFlushP();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(scene->pokepicManager);
    ParticleSystem_CalcAll();
    reg_G3_SWAP_BUFFERS = 1;
}

void AprijuiceScene_Delete(AprijuiceScene *scene) {
    Camera_Delete(scene->camera);
    PokepicManager_Delete(scene->pokepicManager);
    AprijuiceScene_FreeParticles(scene);
    GF_3DVramMan_Delete(scene->vramMan);
    Heap_Free(scene);
}

#pragma force_active on

void AprijuiceScene_TransferPokepicGfx(AprijuiceScene *scene) {
    PokepicManager_HandleLoadImgAndOrPltt(scene->pokepicManager);
}

void AprijuiceScene_CreatePokepic(AprijuiceScene *scene, AprijuiceMon *entry) {
    PokepicTemplate template;
    GetPokemonSpriteCharAndPlttNarcIds(&template, entry->mon, 2);
    scene->pokepic = PokepicManager_CreatePokepic(scene->pokepicManager, &template, 204, 112, 0, 0, NULL, NULL);
}

#pragma force_active reset

void AprijuiceScene_DeletePokepic(AprijuiceScene *scene) {
    if (scene->pokepic) {
        Pokepic_Delete(scene->pokepic);
        scene->pokepic = NULL;
    }
}

#pragma force_active on

void AprijuiceScene_StartParticles(AprijuiceScene *scene, int resource) {
    ParticleSystem_CreateEmitterWithParam(scene->emitter, resource, AprijuiceScene_SetEmitterPosition, scene);
    ParticleSystem_SetPerspectiveType(scene->emitter, 1);
}

#pragma force_active reset

BOOL AprijuiceScene_AreParticlesActive(AprijuiceScene *scene) {
    return ParticleSystem_GetActiveEmitterCount(scene->emitter) != 0;
}

#pragma force_active on

void AprijuiceScene_Init3D(void) {
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xFFFFCFFD;
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xCFFB;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & 0xFFFFCFFF) | 8;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & 0xFFFFCFFF) | 16;
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xCFDF;
    G3X_SetFog(FALSE, (GXFogBlend)0, (GXFogSlope)0, 0);
    G3X_SetClearColor(0, 0, 0x7FFF, 63, FALSE);
    reg_G3_SWAP_BUFFERS = 2;
    reg_G3_VIEWPORT = 0xBFFF0000;
}

#pragma force_active reset

void AprijuiceScene_ReserveVram(GF3DVramMan *vramMan) { // vramMan is unused
    u32 tex = NNS_GfdDefaultFuncAllocTexVram(0x8000, FALSE, 0);
    u32 pltt = NNS_GfdDefaultFuncAllocPlttVram(0xA0, FALSE, 0);
    if (tex == 0) {
        GF_AssertFail();
    }
    if (pltt == 0) {
        GF_AssertFail();
    }
    ParticleSystem_ZeroAll();
}

u32 AprijuiceScene_AllocTexVram(u32 size, BOOL is4x4) {
    u32 key = NNS_GfdDefaultFuncAllocTexVram(size, is4x4, 0);
    ParticleSystem_RegisterTexKey(key);
    return (key << 16) >> 13;
}

u32 AprijuiceScene_AllocPlttVram(u32 size, BOOL is4color) {
    u32 key = NNS_GfdDefaultFuncAllocPlttVram(size, is4color, 0);
    ParticleSystem_RegisterPlttKey(key);
    return (key << 16) >> 13;
}

void AprijuiceScene_SetEmitterPosition(SPLEmitter *emitter) {
    VecFx32 offset = { 0, 0, 0 };
    offset.x = 0x3310;
    offset.y = -0x1020;
    offset.z = -0x1AE0;
    emitter->emtr_pos.x = offset.x + emitter->p_res->p_base->pos.x;
    emitter->emtr_pos.y = offset.y + emitter->p_res->p_base->pos.y;
    emitter->emtr_pos.z = offset.z + emitter->p_res->p_base->pos.z;
}

SPLEmitter *AprijuiceScene_CreateParticleSystem(enum HeapID heapId) {
    void *heap = Heap_Alloc(heapId, 0x4800);
    SPLEmitter *emitter = ParticleSystem_New(AprijuiceScene_AllocTexVram, AprijuiceScene_AllocPlttVram, heap, 0x4800, TRUE, heapId);
    Camera *camera = ParticleSystem_GetCamera(emitter);
    if (camera != NULL) {
        Camera_SetPerspectiveClippingPlane(FX32_ONE, 0x384000, camera);
    }
    return emitter;
}

void AprijuiceScene_InitParticles(AprijuiceScene *scene) {
    void *resource;
    scene->emitter = AprijuiceScene_CreateParticleSystem(scene->heapId);
    ParticleSystem_SetPerspectiveType(scene->emitter, 1);
    resource = ParticleSystem_LoadResourceFile(NARC_a_2_4_2, 10, scene->heapId);
    ParticleSystem_SetResource(scene->emitter, resource, 10, TRUE);
}

void AprijuiceScene_FreeParticles(AprijuiceScene *scene) {
    ParticleSystem_Free(scene->emitter);
    Heap_Free(scene->emitter);
}
