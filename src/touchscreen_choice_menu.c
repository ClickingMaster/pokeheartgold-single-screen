#include <nitro/gx/gx_vramcnt.h>

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0191.h"

#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "pm_string.h"
#include "render_text.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "task.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"

// Field bottom-screen app 3 (ScrCmd_TouchscreenMenuHide, sBottomScreenApps[3] of field/bottom_screen_app.c):
// the touch YES/NO buttons of ScrCmd_GetMenuChoice and the touch multichoice list of ScrCmd_MenuExec
// (up to 28 rows of the field MoveTutorMenu). FieldSystem_SetTouchscreenChoiceMenuState sets the state (3 YES/NO, 7 list) and the
// result pointer; scripts wait for state 6 or for *result != 0xEEEE.

#define TOUCHSCREEN_CHOICE_MENU_MAX_ITEMS 28

enum TouchscreenChoiceMenuState {
    TOUCHSCREEN_CHOICE_MENU_STATE_INIT,
    TOUCHSCREEN_CHOICE_MENU_STATE_IDLE,
    TOUCHSCREEN_CHOICE_MENU_STATE_WAIT_RETURN,
    TOUCHSCREEN_CHOICE_MENU_STATE_SHOW_YES_NO,
    TOUCHSCREEN_CHOICE_MENU_STATE_YES_NO_INPUT,
    TOUCHSCREEN_CHOICE_MENU_STATE_YES_NO_END,
    TOUCHSCREEN_CHOICE_MENU_STATE_DONE,
    TOUCHSCREEN_CHOICE_MENU_STATE_SHOW_LIST,
    TOUCHSCREEN_CHOICE_MENU_STATE_LIST_INPUT,
    TOUCHSCREEN_CHOICE_MENU_STATE_LIST_END,
    TOUCHSCREEN_CHOICE_MENU_STATE_LIST_WRITE_RESULT,
    TOUCHSCREEN_CHOICE_MENU_STATE_CANCEL_YES_NO,
};

typedef struct TouchscreenChoiceMenuData {
    u32 state;
    u16 *result;
    u32 unk_08;
    struct ScriptMenu *menu; // ScrCmd_MenuExec list
    int frameCounter;
    u32 unk_14;
    BgConfig *bgConfig;
    SysTask *task;
    u16 *lastTouchMenuInput;
    FieldSystem *fieldSystem;
    Window yesWindow;
    Window noWindow;
    u32 unk_48;
    MsgData *msgData;
    MessageFormat *msgFormat;
    Window listWindows[TOUCHSCREEN_CHOICE_MENU_MAX_ITEMS];
    int itemCount;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    SpriteResource *resObjs[4];
    SpriteResourcesHeader header;
    Sprite *cursorSprite;
    u8 unk_38C[8];
    int selection;
    u32 unk_398;
    void (*selectionCallback)(void *arg, int selection); // ScrCmd_MenuExec: MenuExec_PrintItemHelp
    void *selectionCallbackArg;
} TouchscreenChoiceMenuData; // size: 0x3A4

// field_move_tutor_menu_internal.h declares these, but its prototypes conflict with text.h.
ListMenuItem *ScriptMenu_GetListItems(struct ScriptMenu *menu);
u8 ScriptMenu_GetCount(struct ScriptMenu *menu);
u8 ScriptMenu_IsCancellable(struct ScriptMenu *menu);

typedef BOOL (*TouchscreenChoiceMenuStateFunc)(TouchscreenChoiceMenuData *data);

// {x, y} of the cursor on YES / NO; the x and the y column each have a label.
typedef struct TouchscreenChoiceMenuCoord {
    u16 value;
    u16 nextColumn;
} TouchscreenChoiceMenuCoord;

extern const TouchscreenChoiceMenuCoord sTouchscreenChoiceMenuYesNoCursorX[];
extern const TouchscreenChoiceMenuCoord sTouchscreenChoiceMenuYesNoCursorY[];
extern const TouchscreenHitbox sTouchscreenChoiceMenuYesNoHitboxes[];
extern const BgTemplate sTouchscreenChoiceMenuBgTemplate_Sub0;
extern const BgTemplate sTouchscreenChoiceMenuBgTemplate_Sub1;
extern const BgTemplate sTouchscreenChoiceMenuBgTemplate_Sub2;
// [itemCount - 2][selection * 3]: {x, y, anim} of the list cursor; each column has its own label.
extern const u8 sTouchscreenChoiceMenuListCursorX[][24];
extern const u8 sTouchscreenChoiceMenuListCursorY[][24];
extern const u8 sTouchscreenChoiceMenuListCursorAnim[][24];
// [itemCount - 2]: per list size, the neighbour of each row for up / down / left / right (-1: none).
extern const s8 *sTouchscreenChoiceMenuListNeighbors[];
extern const TouchscreenHitbox *sTouchscreenChoiceMenuListHitboxes[];
extern const WindowTemplate *sTouchscreenChoiceMenuListWindowTemplates[];
extern TouchscreenChoiceMenuStateFunc sTouchscreenChoiceMenuStateFuncs[];

SysTask *TouchscreenChoiceMenu_Create(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, u16 *result);
void TouchscreenChoiceMenu_Destroy(BgConfig *bgConfig, SysTask *task);
BOOL TouchscreenChoiceMenu_IsExitDone(void *unused);
void TouchscreenChoiceMenu_SetCallback(SysTask *task, void (*callback)(void *arg, int selection), void *arg);
void TouchscreenChoiceMenu_Task(SysTask *task, void *taskData);
void TouchscreenChoiceMenu_LoadBgGraphics(TouchscreenChoiceMenuData *data);
void TouchscreenChoiceMenu_ShowYesNo(TouchscreenChoiceMenuData *data);
void TouchscreenChoiceMenu_HideYesNo(TouchscreenChoiceMenuData *data);
void TouchscreenChoiceMenu_ShowList(TouchscreenChoiceMenuData *data);
void TouchscreenChoiceMenu_HideList(TouchscreenChoiceMenuData *data);
void TouchscreenChoiceMenu_LoadSpriteResources(TouchscreenChoiceMenuData *data);
void TouchscreenChoiceMenu_CreateCursor(TouchscreenChoiceMenuData *data, int selection);
void TouchscreenChoiceMenu_PrintMessage(Window *window, MsgData *msgData, u32 msgId);
void TouchscreenChoiceMenu_LoadMessages(TouchscreenChoiceMenuData *data, u32 unused);
void TouchscreenChoiceMenu_FreeMessages(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_Init(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_ShowYesNo(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_YesNoInput(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_YesNoEnd(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_Done(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_WaitReturn(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_Idle(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_ShowList(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_MoveListSelection(int *selection, int count, int dir);
BOOL TouchscreenChoiceMenu_State_ListInput(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_ListEnd(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_ListWriteResult(TouchscreenChoiceMenuData *data);
void TouchscreenChoiceMenu_SetYesNoCursorPos(TouchscreenChoiceMenuData *data, int selection);
void TouchscreenChoiceMenu_SetListCursorPos(TouchscreenChoiceMenuData *data, int count, int selection);
void TouchscreenChoiceMenu_SetListCursorPressed(TouchscreenChoiceMenuData *data, int count);
BOOL TouchscreenChoiceMenu_HandleYesNoInput(TouchscreenChoiceMenuData *data);
BOOL TouchscreenChoiceMenu_State_CancelYesNo(TouchscreenChoiceMenuData *data);

SysTask *TouchscreenChoiceMenu_Create(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, u16 *result) {
    SysTask *task;
    TouchscreenChoiceMenuData *data;
    vu32 *dispCnt = &reg_GXS_DB_DISPCNT;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    *dispCnt = (*dispCnt & 0xFFCFFFEF) | GX_OBJVRAMMODE_CHAR_1D_32K;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sTouchscreenChoiceMenuBgTemplate_Sub0, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sTouchscreenChoiceMenuBgTemplate_Sub1, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sTouchscreenChoiceMenuBgTemplate_Sub2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_FIELD1);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_FIELD1);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_FIELD1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    task = CreateSysTaskAndEnvironment(TouchscreenChoiceMenu_Task, sizeof(TouchscreenChoiceMenuData), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->task = task;
    data->unk_14 = 0;
    data->bgConfig = bgConfig;
    data->lastTouchMenuInput = lastTouchMenuInput;
    data->fieldSystem = fieldSystem;
    data->unk_48 = 0;
    data->state = TOUCHSCREEN_CHOICE_MENU_STATE_INIT;
    data->result = result;
    data->selectionCallback = NULL;
    data->selectionCallbackArg = NULL;
    data->yesWindow.pixelBuffer = NULL;
    data->noWindow.pixelBuffer = NULL;
    FontID_Alloc(4, HEAP_ID_8);
    TouchscreenChoiceMenu_LoadMessages(data, data->fieldSystem->startMenuState);
    TouchscreenChoiceMenu_LoadBgGraphics(data);
    TouchscreenChoiceMenu_LoadSpriteResources(data);
    TouchscreenChoiceMenu_CreateCursor(data, 0);
    *dispCnt &= 0xFFFF1FFF; // no windows
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    return task;
}

void TouchscreenChoiceMenu_Destroy(BgConfig *bgConfig, SysTask *task) {
    TouchscreenChoiceMenuData *data = SysTask_GetData(task);
    int i;

    SpriteTransfer_DeleteCharTransferTask(data->resObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(data->resObjs[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(data->resMans[i]);
    }
    SpriteList_Delete(data->spriteList);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    TouchscreenChoiceMenu_FreeMessages(data);
    FontID_Release(4);
    DestroySysTaskAndEnvironment(task);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    Heap_Destroy(HEAP_ID_8);
}

BOOL TouchscreenChoiceMenu_IsExitDone(void *unused) {
    return TRUE;
}

// FieldSystem_SetTouchscreenChoiceMenuCallback (ScrCmd_MenuExec): called with the list index on every selection change.
void TouchscreenChoiceMenu_SetCallback(SysTask *task, void (*callback)(void *arg, int selection), void *arg) {
    TouchscreenChoiceMenuData *data = SysTask_GetData(task);
    data->selectionCallback = callback;
    data->selectionCallbackArg = arg;
}

void TouchscreenChoiceMenu_Task(SysTask *task, void *taskData) {
    TouchscreenChoiceMenuData *data = taskData;

    if (!FieldSystem_TaskIsRunning(data->fieldSystem)) {
        if (data->fieldSystem->startMenuState == 2) {
            data->fieldSystem->startMenuState = 3;
        }
        G2S_BlendNone();
    } else if (sTouchscreenChoiceMenuStateFuncs[data->state](data) == TRUE) {
        data->fieldSystem->startMenuState = 0;
        FieldSystem_SwitchBottomScreenApp(data->fieldSystem, 0, NULL); // back to the start menu (app 0)
    }
    SpriteList_RenderAndAnimateSprites(data->spriteList);
}

void TouchscreenChoiceMenu_LoadBgGraphics(TouchscreenChoiceMenuData *data) {
    NNSG2dScreenData *screenData;
    void *nscr;
    NARC *narc = NARC_New(NARC_a_2_3_7, HEAP_ID_8);

    G2S_BlendNone();
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0xA0, HEAP_ID_8);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, data->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_8);
    nscr = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, FALSE, &screenData, HEAP_ID_8);
    BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_2, screenData->rawData, screenData->szByte);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_2);
    Heap_Free(nscr);
    NARC_Delete(narc);
}

void TouchscreenChoiceMenu_ShowYesNo(TouchscreenChoiceMenuData *data) {
    NARC *narc = NARC_New(NARC_a_2_3_7, HEAP_ID_8);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, data->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_8);
    NARC_Delete(narc);
    AddWindowParameterized(data->bgConfig, &data->yesWindow, GF_BG_LYR_SUB_1, 12, 8, 8, 2, 4, 0x80);
    AddWindowParameterized(data->bgConfig, &data->noWindow, GF_BG_LYR_SUB_1, 12, 14, 8, 2, 4, 0x90);
    TouchscreenChoiceMenu_PrintMessage(&data->yesWindow, data->msgData, msg_0191_00046);
    TouchscreenChoiceMenu_PrintMessage(&data->noWindow, data->msgData, msg_0191_00047);
}

void TouchscreenChoiceMenu_HideYesNo(TouchscreenChoiceMenuData *data) {
    BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_1);
    if (data->yesWindow.pixelBuffer != NULL) {
        RemoveWindow(&data->yesWindow);
    }
    if (data->noWindow.pixelBuffer != NULL) {
        RemoveWindow(&data->noWindow);
    }
}

#pragma force_active on

// The list background of NARC 239 is file itemCount (2-8).
void TouchscreenChoiceMenu_ShowList(TouchscreenChoiceMenuData *data) {
    ListMenuItem *items = ScriptMenu_GetListItems(data->menu);
    int count = ScriptMenu_GetCount(data->menu);
    NARC *narc = NARC_New(NARC_a_2_3_7, HEAP_ID_8);
    int i;

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, count, data->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_8);
    NARC_Delete(narc);
    for (i = 0; i < count; i++) {
        AddWindow(data->bgConfig, &data->listWindows[i], &sTouchscreenChoiceMenuListWindowTemplates[count - 2][i]);
    }
    for (i = 0; i < count; i++) {
        FillWindowPixelBuffer(&data->listWindows[i], 0);
        AddTextPrinterParameterizedWithColor(&data->listWindows[i], 4, items[i].text, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(2, 1, 0), NULL);
        ScheduleWindowCopyToVram(&data->listWindows[i]);
    }
    data->itemCount = count;
}

#pragma force_active reset

void TouchscreenChoiceMenu_HideList(TouchscreenChoiceMenuData *data) {
    int count = ScriptMenu_GetCount(data->menu);
    int i;

    BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_1);
    for (i = 0; i < count; i++) {
        RemoveWindow(&data->listWindows[i]);
    }
}

#pragma force_active on

void TouchscreenChoiceMenu_LoadSpriteResources(TouchscreenChoiceMenuData *data) {
    int i;

    data->spriteList = G2dRenderer_Init(21, &data->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&data->renderer, 0, FX32_CONST(256));
    for (i = 0; i < 4; i++) {
        data->resMans[i] = Create2DGfxResObjMan(10, (GfGfxResType)i, HEAP_ID_8);
    }
    data->resObjs[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_2_3_7, 12, FALSE, 10, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_8);
    data->resObjs[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_PLTT], NARC_a_2_3_7, 11, FALSE, 10, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_8);
    data->resObjs[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_2_3_7, 13, FALSE, 10, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    data->resObjs[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_2_3_7, 14, FALSE, 10, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(data->resObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(data->resObjs[GF_GFX_RES_TYPE_PLTT]);
}

void TouchscreenChoiceMenu_CreateCursor(TouchscreenChoiceMenuData *data, int selection) {
    SpriteTemplate template;

    CreateSpriteResourcesHeader(&data->header, 10, 10, 10, 10, -1, -1, FALSE, 0, data->resMans[GF_GFX_RES_TYPE_CHAR], data->resMans[GF_GFX_RES_TYPE_PLTT], data->resMans[GF_GFX_RES_TYPE_CELL], data->resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = data->spriteList;
    template.header = &data->header;
    template.position.z = 0;
    template.position.x = sTouchscreenChoiceMenuYesNoCursorX[selection].value << FX32_SHIFT;
    template.position.y = (sTouchscreenChoiceMenuYesNoCursorY[selection].value + 256) << FX32_SHIFT;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 1;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_8;
    data->cursorSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimCtrlSeq(data->cursorSprite, 1);
    Sprite_SetAnimActiveFlag(data->cursorSprite, TRUE);
    Sprite_SetDrawFlag(data->cursorSprite, FALSE);
}

#pragma force_active reset

void TouchscreenChoiceMenu_PrintMessage(Window *window, MsgData *msgData, u32 msgId) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(2, 1, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
}

void TouchscreenChoiceMenu_LoadMessages(TouchscreenChoiceMenuData *data, u32 unused) {
    data->msgFormat = MessageFormat_New(HEAP_ID_8);
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0191_bin, HEAP_ID_8);
}

void TouchscreenChoiceMenu_FreeMessages(TouchscreenChoiceMenuData *data) {
    DestroyMsgData(data->msgData);
    MessageFormat_Delete(data->msgFormat);
}

BOOL TouchscreenChoiceMenu_State_Init(TouchscreenChoiceMenuData *data) {
    data->state = TOUCHSCREEN_CHOICE_MENU_STATE_IDLE;
    return FALSE;
}

BOOL TouchscreenChoiceMenu_State_ShowYesNo(TouchscreenChoiceMenuData *data) {
    data->selection = 0;
    TouchscreenChoiceMenu_ShowYesNo(data);
    Sprite_SetDrawFlag(data->cursorSprite, TRUE);
    TouchscreenChoiceMenu_SetYesNoCursorPos(data, data->selection);
    Sprite_SetAnimCtrlSeq(data->cursorSprite, 1);
    data->state = TOUCHSCREEN_CHOICE_MENU_STATE_YES_NO_INPUT;
    return FALSE;
}

BOOL TouchscreenChoiceMenu_State_YesNoInput(TouchscreenChoiceMenuData *data) {
    TouchscreenChoiceMenu_HandleYesNoInput(data);
    return FALSE;
}

BOOL TouchscreenChoiceMenu_State_YesNoEnd(TouchscreenChoiceMenuData *data) {
    if (!Sprite_IsAnimated(data->cursorSprite)) {
        Sprite_SetDrawFlag(data->cursorSprite, FALSE);
        TouchscreenChoiceMenu_HideYesNo(data);
        *data->result = data->selection;
        data->state = TOUCHSCREEN_CHOICE_MENU_STATE_DONE;
    }
    return FALSE;
}

BOOL TouchscreenChoiceMenu_State_Done(TouchscreenChoiceMenuData *data) {
    Sprite_SetDrawFlag(data->cursorSprite, FALSE);
    data->state = TOUCHSCREEN_CHOICE_MENU_STATE_IDLE;
    return FALSE;
}

BOOL TouchscreenChoiceMenu_State_WaitReturn(TouchscreenChoiceMenuData *data) {
    data->frameCounter++;
    if (data->frameCounter > 20) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// Touching the bottom screen while nothing is shown advances the script text like an A press.
BOOL TouchscreenChoiceMenu_State_Idle(TouchscreenChoiceMenuData *data) {
    if (gSystem.touchHeld) {
        gSystem.simulatedInputs = PAD_BUTTON_A;
    }
    return FALSE;
}

BOOL TouchscreenChoiceMenu_State_ShowList(TouchscreenChoiceMenuData *data) {
    data->state = TOUCHSCREEN_CHOICE_MENU_STATE_LIST_INPUT;
    data->selection = 0;
    TouchscreenChoiceMenu_ShowList(data);
    Sprite_SetDrawFlag(data->cursorSprite, TRUE);
    TouchscreenChoiceMenu_SetListCursorPos(data, data->itemCount, data->selection);
    if (data->selectionCallback) {
        data->selectionCallback(data->selectionCallbackArg, data->selection);
    }
    *data->result = 0xEEEE; // no choice yet
    return FALSE;
}

#pragma force_active reset

BOOL TouchscreenChoiceMenu_MoveListSelection(int *selection, int count, int dir) {
    int cur = *selection;
    s8 next = *(dir + &sTouchscreenChoiceMenuListNeighbors[count - 2][cur * 4]); // operand order kept for codegen
    if (next == -1 || cur == next) {
        return FALSE;
    }
    *selection = next;
    return TRUE;
}

#pragma force_active on

#define TouchscreenChoiceMenu_NotifySelection(data)                                     \
    do {                                                                                \
        if ((data)->selectionCallback) {                                                \
            (data)->selectionCallback((data)->selectionCallbackArg, (data)->selection); \
        }                                                                               \
    } while (0)

#define TouchscreenChoiceMenu_MoveSelection(data, dir)                                             \
    do {                                                                                           \
        if (TouchscreenChoiceMenu_MoveListSelection(&(data)->selection, (data)->itemCount, dir)) { \
            TouchscreenChoiceMenu_SetListCursorPos(data, (data)->itemCount, (data)->selection);    \
            PlaySE(SEQ_SE_DP_SELECT);                                                              \
            TouchscreenChoiceMenu_NotifySelection(data);                                           \
        }                                                                                          \
    } while (0)

BOOL TouchscreenChoiceMenu_State_ListInput(TouchscreenChoiceMenuData *data) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(sTouchscreenChoiceMenuListHitboxes[data->itemCount - 2]);

    if (hit != -1) {
        data->selection = hit;
        TouchscreenChoiceMenu_SetListCursorPos(data, data->itemCount, data->selection);
        PlaySE(SEQ_SE_DP_SELECT);
        data->state = TOUCHSCREEN_CHOICE_MENU_STATE_LIST_END;
        TouchscreenChoiceMenu_SetListCursorPressed(data, data->itemCount);
        TouchscreenChoiceMenu_NotifySelection(data);
    } else if (gSystem.newKeys & PAD_KEY_UP) {
        TouchscreenChoiceMenu_MoveSelection(data, 0);
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        TouchscreenChoiceMenu_MoveSelection(data, 1);
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        TouchscreenChoiceMenu_MoveSelection(data, 2);
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        TouchscreenChoiceMenu_MoveSelection(data, 3);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        TouchscreenChoiceMenu_SetListCursorPressed(data, data->itemCount);
        data->state = TOUCHSCREEN_CHOICE_MENU_STATE_LIST_END;
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        if (ScriptMenu_IsCancellable(data->menu)) { // cancellable: select the last row
            data->selection = data->itemCount - 1;
            TouchscreenChoiceMenu_SetListCursorPos(data, data->itemCount, data->selection);
            TouchscreenChoiceMenu_SetListCursorPressed(data, data->itemCount);
            data->state = TOUCHSCREEN_CHOICE_MENU_STATE_LIST_END;
            PlaySE(SEQ_SE_DP_SELECT);
        }
    }
    return FALSE;
}

#undef TouchscreenChoiceMenu_MoveSelection
#undef TouchscreenChoiceMenu_NotifySelection

#pragma force_active reset

BOOL TouchscreenChoiceMenu_State_ListEnd(TouchscreenChoiceMenuData *data) {
    if (!Sprite_IsAnimated(data->cursorSprite)) {
        Sprite_SetDrawFlag(data->cursorSprite, FALSE);
        TouchscreenChoiceMenu_HideList(data);
        data->state = TOUCHSCREEN_CHOICE_MENU_STATE_LIST_WRITE_RESULT;
    }
    return FALSE;
}

BOOL TouchscreenChoiceMenu_State_ListWriteResult(TouchscreenChoiceMenuData *data) {
    ListMenuItem *items = ScriptMenu_GetListItems(data->menu);
    *data->result = items[data->selection].value;
    data->state = TOUCHSCREEN_CHOICE_MENU_STATE_IDLE;
    return FALSE;
}

void TouchscreenChoiceMenu_SetYesNoCursorPos(TouchscreenChoiceMenuData *data, int selection) {
    VecFx32 pos;
    pos.x = sTouchscreenChoiceMenuYesNoCursorX[selection].value << FX32_SHIFT;
    pos.y = (sTouchscreenChoiceMenuYesNoCursorY[selection].value + 256) << FX32_SHIFT;
    pos.z = 0;
    Sprite_SetMatrix(data->cursorSprite, &pos);
}

#pragma force_active on

void TouchscreenChoiceMenu_SetListCursorPos(TouchscreenChoiceMenuData *data, int count, int selection) {
    VecFx32 pos;
    count -= 2;
    selection *= 3;
    pos.x = sTouchscreenChoiceMenuListCursorX[count][selection] << FX32_SHIFT;
    pos.y = (sTouchscreenChoiceMenuListCursorY[count][selection] + 256) << FX32_SHIFT;
    pos.z = 0;
    Sprite_SetAnimCtrlSeq(data->cursorSprite, sTouchscreenChoiceMenuListCursorAnim[count][selection]);
    Sprite_SetMatrix(data->cursorSprite, &pos);
}

#pragma force_active reset

void TouchscreenChoiceMenu_SetListCursorPressed(TouchscreenChoiceMenuData *data, int count) {
    Sprite_SetAnimCtrlSeq(data->cursorSprite, sTouchscreenChoiceMenuListCursorAnim[count - 2][0] + 2);
}

// 0 = YES, 1 = NO; B selects NO.
BOOL TouchscreenChoiceMenu_HandleYesNoInput(TouchscreenChoiceMenuData *data) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(sTouchscreenChoiceMenuYesNoHitboxes);

    if (hit != -1) {
        if (hit == 0) {
            data->selection = 0;
            TouchscreenChoiceMenu_SetYesNoCursorPos(data, data->selection);
            Sprite_SetAnimCtrlSeq(data->cursorSprite, 3);
            PlaySE(SEQ_SE_DP_SELECT);
            data->state = TOUCHSCREEN_CHOICE_MENU_STATE_YES_NO_END;
        } else if (hit == 1) {
            data->selection = 1;
            TouchscreenChoiceMenu_SetYesNoCursorPos(data, data->selection);
            Sprite_SetAnimCtrlSeq(data->cursorSprite, 3);
            PlaySE(SEQ_SE_DP_SELECT);
            data->state = TOUCHSCREEN_CHOICE_MENU_STATE_YES_NO_END;
        }
    } else if (gSystem.newKeys & PAD_KEY_UP) {
        if (data->selection == 1) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        data->selection = 0;
        TouchscreenChoiceMenu_SetYesNoCursorPos(data, data->selection);
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        if (data->selection == 0) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        data->selection = 1;
        TouchscreenChoiceMenu_SetYesNoCursorPos(data, data->selection);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        Sprite_SetAnimCtrlSeq(data->cursorSprite, 3);
        data->state = TOUCHSCREEN_CHOICE_MENU_STATE_YES_NO_END;
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        data->selection = 1;
        TouchscreenChoiceMenu_SetYesNoCursorPos(data, data->selection);
        Sprite_SetAnimCtrlSeq(data->cursorSprite, 3);
        PlaySE(SEQ_SE_DP_SELECT);
        data->state = TOUCHSCREEN_CHOICE_MENU_STATE_YES_NO_END;
    }
    return FALSE;
}

BOOL TouchscreenChoiceMenu_State_CancelYesNo(TouchscreenChoiceMenuData *data) {
    TouchscreenChoiceMenu_HideYesNo(data);
    return TouchscreenChoiceMenu_State_Done(data);
}
