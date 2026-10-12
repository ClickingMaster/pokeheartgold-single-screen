#include "main.h"

#include "brightness.h"
#include "communication_error.h"
#include "font.h"
#include "gf_rtc.h"
#include "math_util.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "save_data_read_error.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_system.h"
#include "sys_task.h"
#include "system.h"
#include "timer_3.h"
#include "igt_counter.h"
#include "touchpad.h"
#include "swap_3d_buffers.h"
#include "unk_02035900.h"
#include "comm_state.h"
#include "wfc_user_info_warning.h"

FS_EXTERN_OVERLAY(intro_title);
FS_EXTERN_OVERLAY(OVY_36);

extern const OverlayManagerTemplate gApplication_IntroMovie;
extern const OverlayManagerTemplate ov36_App_MainMenu_SelectOption_Continue;

struct MainOverlayState {
    FSOverlayID mainOverlayId;
    OverlayManager *overlayManager;
    FSOverlayID queuedMainOverlayId;
    const OverlayManagerTemplate *queuedMainOverlayTemplate;
    struct MainOverlayArgs args;
};

int _02111864;
PMBackLightSwitch gBacklightTop;
struct MainOverlayState gMainOverlayState;

static void HandleCommunicationErrorScreen(void);
static void DoSoftReset(u32 param);
static void Main_RunOverlayManager(void);
static void ShowCommunicationErrorAndReset(u32 a0, int a1);
static BOOL sub_02001098(int a0);

void NitroMain(void) {
    InitSystemForTheGame();
    InitGraphicMemory();
    InitKeypadAndTouchpad();
    Sys_SetAgbCartridgeVersion(0);
    PM_GetBackLight(&gBacklightTop, NULL);
    Init3DBufferSwapRequestState();
    GF_InitRTCWork();
    Main_ResetOverlayManager();
    FontWork_Init();
    FontID_Alloc(0, HEAP_ID_3);
    FontID_Alloc(1, HEAP_ID_3);
    FontID_Alloc(3, HEAP_ID_3);
    gMainOverlayState.args.unk_00 = -1;
    gMainOverlayState.args.saveData = SaveData_New();
    sub_02005D00();
    InitSoundData(Save_Chatot_Get(gMainOverlayState.args.saveData), Save_PlayerData_GetOptionsAddr(gMainOverlayState.args.saveData));
    Init_Timer3();
    if (Comm_LoadAndInitDwc(3) == 3) {
        ShowWFCUserInfoWarning(HEAP_ID_3, 0);
    }
    if (!Save_FlashChipIsDetected(gMainOverlayState.args.saveData)) {
        ShowSaveDataReadError(HEAP_ID_DEFAULT);
    } else {
        switch (OS_GetResetParameter()) {
        case 0:
            gMainOverlayState.args.unk_04 = 0;
            RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_IntroMovie);
            break;
        case 1:
            SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
            SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
            gMainOverlayState.args.unk_04 = 1;
            RegisterMainOverlay(FS_OVERLAY_ID(OVY_36), &ov36_App_MainMenu_SelectOption_Continue);
            break;
        default:
            GF_ASSERT(FALSE);
            break;
        }
    }
    gSystem.unk70 = 1;
    gSystem.frameCounter = 0;
    InitializeMainRNG();
    ScreenBrightnessData_InitAll();
    IGTCounter_Init();
    _02111864 = 0;
    OS_InitTick();
    while (1) {
        HandleCommunicationErrorScreen();
        HandleDSLidAction();
        ReadKeypadAndTouchpad();
        if ((gSystem.heldKeysRaw & (PAD_BUTTON_START | PAD_BUTTON_SELECT | PAD_BUTTON_L | PAD_BUTTON_R)) == (PAD_BUTTON_START | PAD_BUTTON_SELECT | PAD_BUTTON_L | PAD_BUTTON_R)) {
            if (!gSystem.softResetDisabled) {
                DoSoftReset(0); // no return
            }
        }
        if (Comm_Update()) {
            HandleCommunicationErrorScreen();
            OS_GetTick();
            Main_RunOverlayManager();
            OS_GetTick();
            SysTaskQueue_RunTasks(gSystem.mainTaskQueue);
            SysTaskQueue_RunTasks(gSystem.printTaskQueue);
            if (!gSystem.frameCounter) {
                OS_WaitIrq(TRUE, OS_IE_VBLANK);
                gSystem.vblankCounter++;
            }
        }
        GF_RTC_UpdateOnFrame();
        IGTCounter_Update();
        Handle3DBufferSwapRequest();
        SysTaskQueue_RunTasks(gSystem.printTaskQueue);
        OS_WaitIrq(TRUE, OS_IE_VBLANK);
        gSystem.vblankCounter++;
        gSystem.frameCounter = 0;
        DoAllScreenBrightnessTransitionStep();
        HandleFadeUpdateFrame();
        if (gSystem.vBlankIntr != NULL) {
            gSystem.vBlankIntr(gSystem.vBlankIntrArg);
        }
        DoSoundUpdateFrame();
        SysTaskQueue_RunTasks(gSystem.vwaitTaskQueue);
    }
}

void Main_ResetOverlayManager(void) {
    gMainOverlayState.mainOverlayId = FS_OVERLAY_ID_NONE;
    gMainOverlayState.overlayManager = NULL;
    gMainOverlayState.queuedMainOverlayId = FS_OVERLAY_ID_NONE;
    gMainOverlayState.queuedMainOverlayTemplate = NULL;
}

static void Main_RunOverlayManager(void) {
    if (gMainOverlayState.overlayManager == NULL) {
        if (gMainOverlayState.queuedMainOverlayTemplate == NULL) {
            return;
        }
        if (gMainOverlayState.queuedMainOverlayId != FS_OVERLAY_ID_NONE) {
            HandleLoadOverlay(gMainOverlayState.queuedMainOverlayId, OVY_LOAD_NORMAL);
        }
        gMainOverlayState.mainOverlayId = gMainOverlayState.queuedMainOverlayId;
        gMainOverlayState.overlayManager = OverlayManager_New(gMainOverlayState.queuedMainOverlayTemplate, &gMainOverlayState.args, HEAP_ID_DEFAULT);
        gMainOverlayState.queuedMainOverlayId = FS_OVERLAY_ID_NONE;
        gMainOverlayState.queuedMainOverlayTemplate = NULL;
    }
    if (OverlayManager_Run(gMainOverlayState.overlayManager)) {
        OverlayManager_Delete(gMainOverlayState.overlayManager);
        gMainOverlayState.overlayManager = NULL;
        if (gMainOverlayState.mainOverlayId != FS_OVERLAY_ID_NONE) {
            UnloadOverlayByID(gMainOverlayState.mainOverlayId);
        }
    }
}

void RegisterMainOverlay(FSOverlayID overlayId, const OverlayManagerTemplate *template) {
    GF_ASSERT(gMainOverlayState.queuedMainOverlayTemplate == NULL);
    gMainOverlayState.queuedMainOverlayId = overlayId;
    gMainOverlayState.queuedMainOverlayTemplate = template;
}

static void WaitForVBlank(void) {
    Comm_Update();
    OS_WaitIrq(TRUE, OS_IE_VBLANK);
    gSystem.vblankCounter++;
    gSystem.frameCounter = 0;
    if (gSystem.vBlankIntr != NULL) {
        gSystem.vBlankIntr(gSystem.vBlankIntrArg);
    }
}

static void WaitForVBlankOrReset(u32 param) {
    if (CommState_IsLinkDown() && CARD_TryWaitBackupAsync() == TRUE) {
        OS_ResetSystem(param);
    }
    WaitForVBlank();
}

static void HandleCommunicationErrorScreen(void) {
    int r1 = CommState_GetErrorScreen();
    switch (r1) {
    case 5:
        ShowCommunicationErrorAndReset(0, r1);
        break;
    case 1:
        ShowCommunicationErrorAndReset(1, r1);
        break;
    case 2:
        ShowCommunicationErrorAndReset(0, r1);
        break;
    case 3:
        ShowCommunicationErrorAndReset(1, r1);
        break;
    }
}

static void DoSoftReset(u32 param) {
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_WHITE);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_WHITE);
    if (CommState_StopForReset()) {
        Save_Cancel(SaveData_Get());
    }
    while (TRUE) {
        HandleDSLidAction();
        WaitForVBlankOrReset(param);
    }
}

static void ShowCommunicationErrorAndReset(u32 a0, int a1) {
    int sp4;
    int r7;
    int r4;
    int r5;

    r4 = 0;
    sp4 = CommState_GetLinkMode();
    r7 = CommState_IsWifi();
    gSystem.touchpadReadAuto = FALSE;
    if (a1 == 3) {
        r5 = 3;
    } else {
        if (a0 == 0) {
            if (CommState_IsWifi() == TRUE) {
                r5 = 6;
            } else {
                r5 = 2;
            }
        } else {
            if (CommState_IsWifi() == TRUE) {
                r5 = 5;
            } else {
                r5 = 0;
            }
        }
    }
    ShowCommunicationError(HEAP_ID_DEFAULT, r5, 0);
    CommState_StopForReset();
    WaitForVBlank();
    DoSoundUpdateFrame();
    while (1) {
        HandleDSLidAction();
        ReadKeypadAndTouchpad();
        if (a1 == 1 && r5 == 0 && sub_02001098(sp4) && r7 == 0 && (gSystem.newKeys & PAD_BUTTON_B) && r4 >= 10) {
            a0 = 0;
            break;
        }
        if ((gSystem.newKeys & PAD_BUTTON_A) && r4 >= 10) {
            break;
        }
        DoSoundUpdateFrame();
        WaitForVBlank();
        if (r4 < 10) {
            r4++;
        }
    }
    DoSoftReset(a0);
}

static BOOL sub_02001098(int a0) {
    switch (a0) {
    case 0:
    case 7:
    case 9:
    case 13:
    case 18:
    case 26:
        return TRUE;
    default:
        return FALSE;
    }
}

void InitializeMainRNG(void) {
    u32 seed = RngSeedFromRTC();
    SetMTRNGSeed(seed);
    SetLCRNGSeed(seed);
}

void HandleDSLidAction(void) {
    PMBackLightSwitch top, bottom;
    PMWakeUpTrigger trigger;
    if (PAD_DetectFold()) {
        if (!gSystem.lidClosedPauseDisabled) {
            GF_TouchpadPauseOnLidClose();
            GF_MicPauseOnLidClose();
            if (CTRDG_IsPulledOut() == TRUE) {
                _02111864 = 1;
            }
            while (1) {
                trigger = PM_TRIGGER_COVER_OPEN | PM_TRIGGER_CARD;
                if (gSystem.agbCartridgeVersion && !_02111864) {
                    trigger |= PM_TRIGGER_CARTRIDGE;
                }
                PM_GoSleepMode(trigger, PM_PAD_LOGIC_OR, 0);
                if (CARD_IsPulledOut()) {
                    PM_ForceToPowerOff();
                } else if (PAD_DetectFold()) {
                    _02111864 = 1;
                    continue;
                }
                break;
            }
            GF_TouchpadResumeOnLidOpen();
            GF_MicResumeOnLidOpen();
        } else {
            GF_TouchpadPauseOnLidClose();
            PM_GetBackLight(&top, &bottom);
            if (top == PM_BACKLIGHT_ON) {
                PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF);
            }
        }
    } else {
        PM_GetBackLight(&top, &bottom);
        GF_TouchpadResumeOnLidOpen();
        if (top == PM_BACKLIGHT_OFF) {
            PM_SetBackLight(PM_LCD_ALL, gBacklightTop);
        }
    }
}
