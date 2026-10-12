#include <nitro/gx/gx_vramcnt.h>

#include "global.h"

#include "constants/sndseq.h"
#include "constants/sprites.h"

#include "field/field_control.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0196.h"

#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "font.h"
#include "geometry_math.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "map_object.h"
#include "math_util.h"
#include "menu_input_state.h"
#include "msgdata.h"
#include "player_avatar.h"
#include "pm_string.h"
#include "script_manager.h"
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
#include "unk_0200A090.h"
#include "field_interaction.h"

// Field bottom-screen app 4: the Dowsing Machine (field_use_item.c Task_ActivateDowsingMchnUI).
// While the stylus is held in the search circle, four indicators turn toward the nearest hidden
// item, a signal meter rises and a tile grid lights up around the touch point; the dowsing SE
// plays with a pitch from the signal. Also hosts the action, running shoes and EXIT buttons.

#define DOWSING_MCHN_NUM_STRINGS       5
#define DOWSING_MCHN_NUM_ANGLES        4
#define DOWSING_MCHN_GRID_SIZE         15
#define DOWSING_MCHN_WINDOW_EXIT       4
#define DOWSING_MCHN_SPRITE_ACTION     0
#define DOWSING_MCHN_SPRITE_EXIT       3
#define DOWSING_MCHN_SPRITE_INDICATORS 4

// One indicator direction, in degrees.
typedef struct DowsingMchnAngle {
    u32 displayed;
    u32 target;
    u32 next;
} DowsingMchnAngle;

typedef struct DowsingMchn {
    u8 unk_000[0x10];
    BgConfig *bgConfig;
    u16 *lastTouchMenuInput;
    FieldSystem *fieldSystem;
    void *args;
    SysTask *task;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    SpriteResource *buttonResObjs[4];
    SpriteResource *indicatorResObjs[4];
    Sprite *sprites[8]; // 0 action, 1/2 running shoes, 3 EXIT, 4-7 indicators
    String *strings[DOWSING_MCHN_NUM_STRINGS];
    Window windows[DOWSING_MCHN_NUM_STRINGS]; // CHECK, TALK, FISHING, NEXT, EXIT
    HiddenItemResponse *hiddenItems;
    s16 itemX; // nearest hidden item, relative to the player
    s16 itemY;
    BOOL itemFound;
    int level; // proximity level 0-4
    u32 touchX;
    u32 touchY;
    DowsingMchnAngle angles[DOWSING_MCHN_NUM_ANGLES];
    u16 angleSteps : 15;
    u16 anglesInitialized : 1;
    s16 signal;
    u8 grid[DOWSING_MCHN_GRID_SIZE][DOWSING_MCHN_GRID_SIZE];
    int actionType;
    u16 seTimer;
    s16 sePitch;
    BOOL touchActive;
} DowsingMchn; // size: 0x340

// Sprite parameter block of DowsingMchn_CreateSprite on the stack.
typedef struct DowsingMchnSpriteParams {
    volatile u8 x;
    volatile u8 y;
    volatile u8 drawPriority;
    volatile u8 animSeq;
} DowsingMchnSpriteParams;

// Defined in field/bottom_screen_app.c; its header does not declare these.
BOOL IsSpriteIdWithoutTalkAction(int spriteId);
BOOL IsScriptIdWithoutTalkAction(int scriptId);

extern const TouchscreenHitbox sDowsingMchnActionButtonHitbox;
extern const TouchscreenHitbox sDowsingMchnExitButtonHitbox;
extern const TouchscreenHitbox sDowsingMchnRunningShoesHitbox;
extern const TouchscreenHitbox sDowsingMchnSearchAreaHitbox[];
// Interleaved {signalCap, gridValue} pairs, one per level.
extern const u8 sDowsingMchnLevelSignalCaps[];
extern const u8 sDowsingMchnLevelGridValues[];
extern const u8 sDowsingMchnActionSpriteParams[];
extern const u8 sDowsingMchnRunningShoesSpriteParams[];
extern const u8 sDowsingMchnRunningShoesSprite2Params[];
extern const u8 sDowsingMchnExitSpriteParams[];
extern void (*const sDowsingMchnStepAngleFuncs[])(DowsingMchnAngle *angle);
extern const BgTemplate sDowsingMchnBgTemplateSub0;
extern const BgTemplate sDowsingMchnBgTemplateSub1;
extern const BgTemplate sDowsingMchnBgTemplateSub2;
extern const u16 sDowsingMchnGridMask[DOWSING_MCHN_GRID_SIZE];
extern const u16 sDowsingMchnGridTiles[];
extern const WindowTemplate sDowsingMchnWindowTemplates[DOWSING_MCHN_NUM_STRINGS];
extern const u8 sDowsingMchnActionTypeToWindow[];
extern const u8 sDowsingMchnLevelSignalGains[];

SysTask *DowsingMchn_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void DowsingMchn_Exit(void *unused, SysTask *task);
BOOL DowsingMchn_IsExitDone(void);
void DowsingMchn_SetVramBanks(void);
void DowsingMchn_InitBgs(BgConfig *bgConfig);
void DowsingMchn_FreeBgs(BgConfig *bgConfig);
void DowsingMchn_LoadBgGraphics(DowsingMchn *dowsing, NARC *narc);
void DowsingMchn_LoadStrings(DowsingMchn *dowsing);
void DowsingMchn_FreeStrings(DowsingMchn *dowsing);
void DowsingMchn_CreateWindows(DowsingMchn *dowsing);
void DowsingMchn_RemoveWindows(DowsingMchn *dowsing);
void DowsingMchn_CreateSprites(DowsingMchn *dowsing, NARC *narc);
void DowsingMchn_DestroySprites(DowsingMchn *dowsing);
void DowsingMchn_CreateResourceManagers(DowsingMchn *dowsing);
void DowsingMchn_DestroyResourceManagers(DowsingMchn *dowsing);
void DowsingMchn_LoadSpriteResources(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charId, int plttId, int cellId, int animId);
void DowsingMchn_CreateResourcesHeader(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority);
void DowsingMchn_CreateSprite(DowsingMchn *dowsing, int index, SpriteResourcesHeader *header, const u8 *params);
void DowsingMchn_CreateButtonSprites(DowsingMchn *dowsing);
void DowsingMchn_DestroyButtonSprites(DowsingMchn *dowsing);
void DowsingMchn_CreateIndicatorSprites(DowsingMchn *dowsing, NARC *narc);
void DowsingMchn_DestroyIndicatorSprites(DowsingMchn *dowsing);
void DowsingMchn_Task(SysTask *task, void *taskData);
void DowsingMchn_GetHiddenItemLocs(DowsingMchn *dowsing);
void DowsingMchn_FreeHiddenItemLocs(DowsingMchn *dowsing);
void DowsingMchn_FindNearestHiddenItem(DowsingMchn *dowsing);
void DowsingMchn_GetIndicatorPos(VecFx32 *pos, int deg);
u16 DowsingMchn_GetTouchAngle(int x, int y);
u16 DowsingMchn_Atan2Idx(int x, int y);
u32 DowsingMchn_AngleIdxToDegrees(int idx);
void DowsingMchn_StepAngleLevel0(DowsingMchnAngle *angle);
void DowsingMchn_SetTargetAnglesLevel0(DowsingMchn *dowsing);
void DowsingMchn_StepAngleLevel1(DowsingMchnAngle *angle);
void DowsingMchn_SetTargetAnglesLevel1(DowsingMchn *dowsing);
void DowsingMchn_StepAngleLevel2(DowsingMchnAngle *angle);
void DowsingMchn_SetTargetAnglesLevel2(DowsingMchn *dowsing);
void DowsingMchn_StepAngleLevel3(DowsingMchnAngle *angle);
void DowsingMchn_SetTargetAnglesLevel3(DowsingMchn *dowsing);
void DowsingMchn_StepAngleLevel4(DowsingMchnAngle *angle);
void DowsingMchn_SetTargetAnglesLevel4(DowsingMchn *dowsing);
void DowsingMchn_UpdateTargetAngles(DowsingMchn *dowsing);
void DowsingMchn_UpdateSignal(DowsingMchn *dowsing, int delta);
void DowsingMchn_GetIndicatorAnim(DowsingMchn *dowsing, u16 *blink, u16 *palette);
void DowsingMchn_UpdateIndicatorAppearance(DowsingMchn *dowsing);
void DowsingMchn_SetSpriteRotation(Sprite *sprite, int deg);
void DowsingMchn_UpdateIndicatorPositions(DowsingMchn *dowsing);
u8 DowsingMchn_GetGridCellTarget(DowsingMchn *dowsing, u16 x, u16 y);
void DowsingMchn_UpdateGrid(DowsingMchn *dowsing, BOOL touching);
void DowsingMchn_PlaySignalSE(DowsingMchn *dowsing);
BOOL DowsingMchn_UpdateTouchPos(DowsingMchn *dowsing);
void DowsingMchn_UpdateActionLabel(DowsingMchn *dowsing);
BOOL DowsingMchn_HandleButton(const TouchscreenHitbox *hitbox, Sprite *sprite, int idleAnim, int pressedAnim);
void DowsingMchn_HandleActionButton(DowsingMchn *dowsing);
void DowsingMchn_HandleRunningShoesButton(DowsingMchn *dowsing);
void DowsingMchn_HandleExitButton(DowsingMchn *dowsing);
BOOL DowsingMchn_ShouldHideButtons(DowsingMchn *dowsing);

SysTask *DowsingMchn_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args) {
    DowsingMchn *dowsing;
    SysTask *task;
    NARC *narc;
    LocalMapObject *facingObj;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);
    G2S_BlendNone();
    task = CreateSysTaskAndEnvironment(DowsingMchn_Task, sizeof(DowsingMchn), 10, HEAP_ID_8);
    dowsing = SysTask_GetData(task);
    dowsing->bgConfig = bgConfig;
    dowsing->lastTouchMenuInput = lastTouchMenuInput;
    dowsing->fieldSystem = fieldSystem;
    dowsing->args = args;
    dowsing->task = task;
    dowsing->touchActive = FALSE;
    dowsing->actionType = FieldSystem_GetFacingActionType(dowsing->fieldSystem);
    if (dowsing->actionType == 1) {
        FieldSystem_GetFacingObject(dowsing->fieldSystem, &facingObj);
        if (IsScriptIdWithoutTalkAction(MapObject_GetScriptID(facingObj)) == TRUE || IsSpriteIdWithoutTalkAction(MapObject_GetSpriteID(facingObj)) == TRUE) {
            dowsing->actionType = 0;
        }
    }
    narc = NARC_New(NARC_a_2_5_1, HEAP_ID_8);
    DowsingMchn_SetVramBanks();
    DowsingMchn_InitBgs(bgConfig);
    DowsingMchn_LoadBgGraphics(dowsing, narc);
    DowsingMchn_LoadStrings(dowsing);
    DowsingMchn_CreateWindows(dowsing);
    DowsingMchn_CreateSprites(dowsing, narc);
    NARC_Delete(narc);
    return task;
}

void DowsingMchn_Exit(void *unused, SysTask *task) {
    DowsingMchn *dowsing = SysTask_GetData(task);
    DowsingMchn_FreeHiddenItemLocs(dowsing);
    DowsingMchn_DestroySprites(dowsing);
    DowsingMchn_RemoveWindows(dowsing);
    DowsingMchn_FreeStrings(dowsing);
    DowsingMchn_FreeBgs(dowsing->bgConfig);
    DestroySysTaskAndEnvironment(task);
    Heap_Destroy(HEAP_ID_8);
}

BOOL DowsingMchn_IsExitDone(void) {
    return TRUE;
}

void DowsingMchn_SetVramBanks(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
}

// force_active only sets a section flag here; kept so the object stays identical.
#pragma force_active on

void DowsingMchn_InitBgs(BgConfig *bgConfig) {
    BgTemplate sub0;
    BgTemplate sub1;
    BgTemplate sub2;

    GXS_SetGraphicsMode(GX_BGMODE_0);
    sub0 = sDowsingMchnBgTemplateSub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_8);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = sDowsingMchnBgTemplateSub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    sub2 = sDowsingMchnBgTemplateSub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
}

#pragma force_active reset

void DowsingMchn_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
}

void DowsingMchn_LoadBgGraphics(DowsingMchn *dowsing, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, dowsing->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, dowsing->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 1, dowsing->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_8);
}

void DowsingMchn_LoadStrings(DowsingMchn *dowsing) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0196_bin, HEAP_ID_8);
    dowsing->strings[1] = NewString_ReadMsgData(msgData, msg_0196_00017); // TALK
    dowsing->strings[0] = NewString_ReadMsgData(msgData, msg_0196_00018); // CHECK
    dowsing->strings[2] = NewString_ReadMsgData(msgData, msg_0196_00022); // FISHING
    dowsing->strings[3] = NewString_ReadMsgData(msgData, msg_0196_00023); // NEXT
    dowsing->strings[4] = NewString_ReadMsgData(msgData, msg_0196_00021); // EXIT
    DestroyMsgData(msgData);
}

void DowsingMchn_FreeStrings(DowsingMchn *dowsing) {
    u32 i;
    for (i = 0; i < DOWSING_MCHN_NUM_STRINGS; i++) {
        String_Delete(dowsing->strings[i]);
    }
}

#pragma force_active on

// Prints each label into its own window; only the current action label and EXIT are shown.
void DowsingMchn_CreateWindows(DowsingMchn *dowsing) {
    u32 i;

    FontID_Alloc(4, HEAP_ID_8);
    for (i = 0; i < DOWSING_MCHN_NUM_STRINGS; i++) {
        AddWindow(dowsing->bgConfig, &dowsing->windows[i], &sDowsingMchnWindowTemplates[i]);
        FillWindowPixelBuffer(&dowsing->windows[i], 0);
        AddTextPrinterParameterizedWithColor(&dowsing->windows[i], 4, dowsing->strings[i], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 1, 0), NULL);
        CopyWindowPixelsToVram_TextMode(&dowsing->windows[i]);
    }
    ScheduleWindowCopyToVram(&dowsing->windows[sDowsingMchnActionTypeToWindow[dowsing->actionType]]);
    ScheduleWindowCopyToVram(&dowsing->windows[DOWSING_MCHN_WINDOW_EXIT]);
}

#pragma force_active reset

void DowsingMchn_RemoveWindows(DowsingMchn *dowsing) {
    u32 i;
    for (i = 0; i < DOWSING_MCHN_NUM_STRINGS; i++) {
        RemoveWindow(&dowsing->windows[i]);
    }
    FontID_Release(4);
}

void DowsingMchn_CreateSprites(DowsingMchn *dowsing, NARC *narc) {
    dowsing->spriteList = G2dRenderer_Init(8, &dowsing->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&dowsing->renderer, 0, FX32_CONST(256));
    DowsingMchn_CreateResourceManagers(dowsing);
    DowsingMchn_CreateButtonSprites(dowsing);
    DowsingMchn_CreateIndicatorSprites(dowsing, narc);
}

void DowsingMchn_DestroySprites(DowsingMchn *dowsing) {
    DowsingMchn_DestroyIndicatorSprites(dowsing);
    DowsingMchn_DestroyButtonSprites(dowsing);
    DowsingMchn_DestroyResourceManagers(dowsing);
    SpriteList_Delete(dowsing->spriteList);
}

void DowsingMchn_CreateResourceManagers(DowsingMchn *dowsing) {
    u32 i;
    for (i = 0; i < 4; i++) {
        dowsing->resMans[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_8);
    }
}

void DowsingMchn_DestroyResourceManagers(DowsingMchn *dowsing) {
    u32 i;
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(dowsing->resMans[i]);
    }
}

#pragma force_active on

void DowsingMchn_LoadSpriteResources(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charId, int plttId, int cellId, int animId) {
    resObjs[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CHAR], narc, charFileId, TRUE, charId, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_8);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resObjs[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(resObjs[GF_GFX_RES_TYPE_CHAR]);
    resObjs[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_PLTT], narc, plttFileId, FALSE, plttId, NNS_G2D_VRAM_TYPE_2DSUB, plttNum, HEAP_ID_8);
    SpriteTransfer_CreatePlttTransferTask(resObjs[GF_GFX_RES_TYPE_PLTT]);
    sub_0200A740(resObjs[GF_GFX_RES_TYPE_PLTT]);
    resObjs[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CELL], narc, cellFileId, TRUE, cellId, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    resObjs[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_ANIM], narc, animFileId, TRUE, animId, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
}

#pragma force_active reset

void DowsingMchn_CreateResourcesHeader(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority) {
    int charId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_CHAR]);
    int plttId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_PLTT]);
    int cellId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_CELL]);
    int animId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_ANIM]);
    CreateSpriteResourcesHeader(header, charId, plttId, cellId, animId, -1, -1, 0, priority, resMans[GF_GFX_RES_TYPE_CHAR], resMans[GF_GFX_RES_TYPE_PLTT], resMans[GF_GFX_RES_TYPE_CELL], resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

// params: {x, y, drawPriority, animSeq}
void DowsingMchn_CreateSprite(DowsingMchn *dowsing, int index, SpriteResourcesHeader *header, const u8 *params) {
    SpriteTemplate template;

    template.spriteList = dowsing->spriteList;
    template.header = header;
    template.position.x = params[0] << FX32_SHIFT;
    template.position.y = (params[1] << FX32_SHIFT) + FX32_CONST(256);
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = params[2];
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_8;
    dowsing->sprites[index] = Sprite_CreateAffine(&template);
    Sprite_SetAnimCtrlSeq(dowsing->sprites[index], params[3]);
}

// The button graphics are shared with the touchscreen start menu (NARC a/0/1/4).
void DowsingMchn_CreateButtonSprites(DowsingMchn *dowsing) {
    NARC *narc;
    SpriteResourcesHeader header;

    narc = NARC_New(NARC_a_0_1_4, HEAP_ID_8);
    DowsingMchn_LoadSpriteResources(dowsing->buttonResObjs, dowsing->resMans, narc, 0x46, 7, 0x44, 0x45, 4, 0x1F2, 0x1F2, 0x1F2, 0x1F2);
    NARC_Delete(narc);
    DowsingMchn_CreateResourcesHeader(dowsing->buttonResObjs, dowsing->resMans, &header, 1);
    DowsingMchn_CreateSprite(dowsing, 0, &header, sDowsingMchnActionSpriteParams);
    DowsingMchn_CreateSprite(dowsing, 1, &header, sDowsingMchnRunningShoesSpriteParams);
    DowsingMchn_CreateSprite(dowsing, 2, &header, sDowsingMchnRunningShoesSprite2Params);
    DowsingMchn_CreateSprite(dowsing, 3, &header, sDowsingMchnExitSpriteParams);
    if (PlayerAvatar_CheckRunningShoesLock(FieldSystem_GetPlayerAvatar(dowsing->fieldSystem)) == 0) {
        Sprite_SetAnimCtrlSeq(dowsing->sprites[1], 3);
        Sprite_SetAnimCtrlSeq(dowsing->sprites[2], 11);
    } else {
        Sprite_SetAnimCtrlSeq(dowsing->sprites[1], 4);
        Sprite_SetAnimCtrlSeq(dowsing->sprites[2], 7);
    }
}

void DowsingMchn_DestroyButtonSprites(DowsingMchn *dowsing) {
    Sprite_Delete(dowsing->sprites[0]);
    Sprite_Delete(dowsing->sprites[1]);
    Sprite_Delete(dowsing->sprites[2]);
    SpriteTransfer_DeleteCharTransferTask(dowsing->buttonResObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(dowsing->buttonResObjs[GF_GFX_RES_TYPE_PLTT]);
}

#pragma force_active on

// The indicators start at (0, 0); DowsingMchn_UpdateIndicatorPositions places them.
void DowsingMchn_CreateIndicatorSprites(DowsingMchn *dowsing, NARC *narc) {
    SpriteResourcesHeader header;
    DowsingMchnSpriteParams params = { 0 };
    u32 i;

    DowsingMchn_LoadSpriteResources(dowsing->indicatorResObjs, dowsing->resMans, narc, 4, 7, 5, 6, 9, 499, 499, 499, 499);
    DowsingMchn_CreateResourcesHeader(dowsing->indicatorResObjs, dowsing->resMans, &header, 2);
    for (i = 0; i < DOWSING_MCHN_NUM_ANGLES; i++) {
        DowsingMchn_CreateSprite(dowsing, i + DOWSING_MCHN_SPRITE_INDICATORS, &header, (const u8 *)&params);
    }
}

#pragma force_active reset

void DowsingMchn_DestroyIndicatorSprites(DowsingMchn *dowsing) {
    u32 i;
    for (i = 0; i < DOWSING_MCHN_NUM_ANGLES; i++) {
        Sprite_Delete(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS]);
    }
    SpriteTransfer_DeleteCharTransferTask(dowsing->indicatorResObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(dowsing->indicatorResObjs[GF_GFX_RES_TYPE_PLTT]);
}

#pragma force_active on

void DowsingMchn_Task(SysTask *task, void *taskData) {
    DowsingMchn *dowsing = taskData;

    if (dowsing->fieldSystem->unkD2_7 && FieldSystem_IsPlayerMovementAllowed(dowsing->fieldSystem)) {
        dowsing->fieldSystem->unkD2_7 = FALSE;
    }
    if (dowsing->touchActive == FALSE && TouchscreenHitbox_FindHitboxAtTouchNew(sDowsingMchnSearchAreaHitbox) != -1) {
        dowsing->touchActive = TRUE;
    }
    if (FieldSystem_TaskIsRunning(dowsing->fieldSystem) == TRUE || TouchscreenHitbox_FindHitboxAtTouchHeld(sDowsingMchnSearchAreaHitbox) == -1 || dowsing->touchActive == FALSE) {
        DowsingMchn_UpdateSignal(dowsing, -1);
        DowsingMchn_UpdateGrid(dowsing, FALSE);
        dowsing->anglesInitialized = FALSE;
        dowsing->seTimer = 0;
        dowsing->sePitch = 0;
        dowsing->touchActive = FALSE;
        StopSE(SEQ_SE_GS_DOWSING_SINGLE, 0);
    } else if (dowsing->touchActive == TRUE) {
        DowsingMchn_UpdateTouchPos(dowsing);
        DowsingMchn_UpdateTargetAngles(dowsing);
        DowsingMchn_UpdateSignal(dowsing, 1);
        DowsingMchn_UpdateGrid(dowsing, TRUE);
        DowsingMchn_PlaySignalSE(dowsing);
    }
    DowsingMchn_UpdateIndicatorAppearance(dowsing);
    DowsingMchn_UpdateIndicatorPositions(dowsing);
    DowsingMchn_HandleActionButton(dowsing);
    DowsingMchn_HandleRunningShoesButton(dowsing);
    DowsingMchn_HandleExitButton(dowsing);
    SpriteList_RenderAndAnimateSprites(dowsing->spriteList);
}

#pragma force_active reset

void DowsingMchn_GetHiddenItemLocs(DowsingMchn *dowsing) {
    dowsing->hiddenItems = AllocAndFetchNearbyHiddenItems(dowsing->fieldSystem, HEAP_ID_8);
}

void DowsingMchn_FreeHiddenItemLocs(DowsingMchn *dowsing) {
    if (dowsing->hiddenItems != NULL) {
        Heap_Free(dowsing->hiddenItems);
        dowsing->hiddenItems = NULL;
    }
}

#pragma force_active on

// Nearest by |x| + |y|; the list ends with an entry whose unk4 is 0xFF.
void DowsingMchn_FindNearestHiddenItem(DowsingMchn *dowsing) {
    int offset;
    u16 minDistance;

    DowsingMchn_GetHiddenItemLocs(dowsing);
    dowsing->itemFound = FALSE;
    for (offset = 0;; offset += sizeof(HiddenItemResponse)) {
        HiddenItemResponse *item = (HiddenItemResponse *)((u8 *)dowsing->hiddenItems + offset);
        if (item->unk4 == 0xFF) {
            break;
        }
        if (dowsing->itemFound == FALSE) {
            s16 x = item->x;
            minDistance = abs(x) + abs(item->y);
            dowsing->itemX = x;
            dowsing->itemY = ((HiddenItemResponse *)((u8 *)dowsing->hiddenItems + offset))->y;
            dowsing->itemFound = TRUE;
        } else {
            s16 x = item->x;
            u16 distance = abs(x) + abs(item->y);
            if (minDistance > distance) {
                minDistance = distance;
                dowsing->itemX = x;
                dowsing->itemY = ((HiddenItemResponse *)((u8 *)dowsing->hiddenItems + offset))->y;
            }
        }
    }
    DowsingMchn_FreeHiddenItemLocs(dowsing);
}

#pragma force_active reset

// Point on the circle of radius 68 around (84, 100) of the sub screen.
void DowsingMchn_GetIndicatorPos(VecFx32 *pos, int deg) {
    pos->x = FX_Mul(GF_CosDeg((u16)deg), FX32_CONST(68)) + FX32_CONST(84);
    pos->y = FX_Mul(GF_SinDeg((u16)deg), FX32_CONST(68)) + FX32_CONST(256 + 100);
}

u16 DowsingMchn_GetTouchAngle(int x, int y) {
    return FX_Atan2Idx((y - 100) << FX32_SHIFT, (x - 84) << FX32_SHIFT);
}

u16 DowsingMchn_Atan2Idx(int x, int y) {
    return FX_Atan2Idx(y << FX32_SHIFT, x << FX32_SHIFT);
}

u32 DowsingMchn_AngleIdxToDegrees(int idx) {
    return (u32)(360 * (u16)idx) >> 16;
}

void DowsingMchn_StepAngleLevel0(DowsingMchnAngle *angle) {
    angle->next = (angle->target + angle->displayed * 5) / 6;
}

// No item in range: the indicators spread around the touch direction.
void DowsingMchn_SetTargetAnglesLevel0(DowsingMchn *dowsing) {
    u32 touchDeg = DowsingMchn_AngleIdxToDegrees(DowsingMchn_GetTouchAngle(dowsing->touchX, dowsing->touchY));
    dowsing->angles[0].target = touchDeg;
    dowsing->angles[1].target = touchDeg + 300;
    dowsing->angles[2].target = touchDeg + 120;
    dowsing->angles[3].target = touchDeg + 216;
    dowsing->angleSteps = 6;
}

void DowsingMchn_StepAngleLevel1(DowsingMchnAngle *angle) {
    angle->next = (angle->target + angle->displayed * 3) / 4;
}

void DowsingMchn_SetTargetAnglesLevel1(DowsingMchn *dowsing) {
    u32 touchDeg = DowsingMchn_AngleIdxToDegrees(DowsingMchn_GetTouchAngle(dowsing->touchX, dowsing->touchY));
    u32 itemDeg = DowsingMchn_AngleIdxToDegrees(DowsingMchn_Atan2Idx(dowsing->itemX, dowsing->itemY));
    dowsing->angles[0].target = (u32)(itemDeg + touchDeg * 3) / 4;
    dowsing->angles[1].target = (u32)(itemDeg + 120 + (touchDeg + 30) * 3) / 4;
    dowsing->angles[2].target = (u32)(itemDeg - 135 + (touchDeg - 36) * 3) / 4;
    dowsing->angles[3].target = (u32)(itemDeg + 90 + (touchDeg + 90) * 3) / 4;
    dowsing->angleSteps = 4;
}

void DowsingMchn_StepAngleLevel2(DowsingMchnAngle *angle) {
    angle->next = (angle->target + angle->displayed * 2) / 3;
}

void DowsingMchn_SetTargetAnglesLevel2(DowsingMchn *dowsing) {
    u32 touchDeg = DowsingMchn_AngleIdxToDegrees(DowsingMchn_GetTouchAngle(dowsing->touchX, dowsing->touchY));
    u32 itemDeg = DowsingMchn_AngleIdxToDegrees(DowsingMchn_Atan2Idx(dowsing->itemX, dowsing->itemY));
    dowsing->angles[0].target = (u32)(itemDeg + touchDeg * 2) / 3;
    dowsing->angles[1].target = (u32)(itemDeg + 135 + (touchDeg + 30) * 2) / 3;
    dowsing->angles[2].target = (u32)(itemDeg - 120 + (touchDeg - 36) * 2) / 3;
    dowsing->angles[3].target = (u32)(itemDeg + 60 + (touchDeg + 90) * 2) / 3;
    dowsing->angleSteps = 3;
}

void DowsingMchn_StepAngleLevel3(DowsingMchnAngle *angle) {
    angle->next = (angle->target + angle->displayed) / 2;
}

void DowsingMchn_SetTargetAnglesLevel3(DowsingMchn *dowsing) {
    u32 touchDeg = DowsingMchn_AngleIdxToDegrees(DowsingMchn_GetTouchAngle(dowsing->touchX, dowsing->touchY));
    u32 itemDeg = DowsingMchn_AngleIdxToDegrees(DowsingMchn_Atan2Idx(dowsing->itemX, dowsing->itemY));
    dowsing->angles[0].target = (u32)(touchDeg + itemDeg) / 3;
    dowsing->angles[1].target = (u32)(itemDeg + 60 + touchDeg) / 2;
    dowsing->angles[2].target = (u32)(itemDeg - 72 + touchDeg) / 2;
    dowsing->angles[3].target = (u32)(touchDeg + 90 + (itemDeg + 45)) / 2;
    dowsing->angleSteps = 2;
}

void DowsingMchn_StepAngleLevel4(DowsingMchnAngle *angle) {
    angle->next = (angle->displayed + angle->target * 2) / 3;
}

// Item within 8 tiles: every indicator aims at (a point near) the item's tile.
void DowsingMchn_SetTargetAnglesLevel4(DowsingMchn *dowsing) {
    s32 x = dowsing->itemX * 8 + 84;
    s32 y = dowsing->itemY * 8 + 100;

    dowsing->angles[0].target = DowsingMchn_AngleIdxToDegrees(DowsingMchn_Atan2Idx(x - dowsing->touchX, y - dowsing->touchY));
    dowsing->angles[1].target = DowsingMchn_AngleIdxToDegrees(DowsingMchn_Atan2Idx(x + 10 - dowsing->touchX, y + 12 - dowsing->touchY));
    dowsing->angles[2].target = DowsingMchn_AngleIdxToDegrees(DowsingMchn_Atan2Idx(x - 12 - dowsing->touchX, y - 4 - dowsing->touchY));
    dowsing->angles[3].target = DowsingMchn_AngleIdxToDegrees(DowsingMchn_Atan2Idx(x - 8 - dowsing->touchX, y - 8 - dowsing->touchY));
    dowsing->angleSteps = 3;
}

#pragma force_active on

// Picks the proximity level from the nearest item, sets the targets and steps the angles,
// unwrapping them so that each indicator turns the short way round.
void DowsingMchn_UpdateTargetAngles(DowsingMchn *dowsing) {
    u32 i;
    s16 x;
    s16 y;

    DowsingMchn_FindNearestHiddenItem(dowsing);
    if (dowsing->itemFound == FALSE) {
        DowsingMchn_SetTargetAnglesLevel0(dowsing);
        dowsing->level = 0;
    } else {
        x = abs(dowsing->itemX);
        y = abs(dowsing->itemY);
        if (x <= 8 && y <= 8) {
            DowsingMchn_SetTargetAnglesLevel4(dowsing);
            dowsing->level = 4;
        } else if (x <= 12 && y <= 12) {
            DowsingMchn_SetTargetAnglesLevel3(dowsing);
            dowsing->level = 3;
        } else if (x <= 17 && y <= 17) {
            DowsingMchn_SetTargetAnglesLevel2(dowsing);
            dowsing->level = 2;
        } else {
            DowsingMchn_SetTargetAnglesLevel1(dowsing);
            dowsing->level = 1;
        }
    }
    if (!dowsing->anglesInitialized) {
        for (i = 0; i < DOWSING_MCHN_NUM_ANGLES; i++) {
            dowsing->angles[i].displayed = dowsing->angles[i].target;
            dowsing->angles[i].next = dowsing->angles[i].target;
        }
        dowsing->anglesInitialized = TRUE;
        dowsing->angleSteps = 1;
    } else {
        for (i = 0; i < DOWSING_MCHN_NUM_ANGLES; i++) {
            dowsing->angles[i].target %= 360;
            dowsing->angles[i].displayed %= 360;
            if (abs((s32)(dowsing->angles[i].target - dowsing->angles[i].displayed)) >= 180) {
                if (dowsing->angles[i].target > dowsing->angles[i].displayed) {
                    dowsing->angles[i].displayed += 360;
                } else if (dowsing->angles[i].target < dowsing->angles[i].displayed) {
                    dowsing->angles[i].target += 360;
                }
            }
            sDowsingMchnStepAngleFuncs[dowsing->level](&dowsing->angles[i]);
        }
    }
}

#pragma opt_propagation off

void DowsingMchn_UpdateSignal(DowsingMchn *dowsing, int delta) {
    if (delta >= 0) {
        int gain = delta * sDowsingMchnLevelSignalGains[dowsing->level];
        dowsing->signal = dowsing->signal + gain;
        int cap = sDowsingMchnLevelSignalCaps[dowsing->level * 2];
        if (dowsing->signal > cap) {
            dowsing->signal = cap;
        }
    } else if (dowsing->signal > 0) {
        dowsing->signal--;
    }
}

#pragma opt_propagation reset
#pragma force_active reset

void DowsingMchn_GetIndicatorAnim(DowsingMchn *dowsing, u16 *blink, u16 *palette) {
    if (dowsing->signal > sDowsingMchnLevelSignalCaps[dowsing->level * 2]) {
        dowsing->signal = sDowsingMchnLevelSignalCaps[dowsing->level * 2];
    }
    *blink = (dowsing->signal / 10) & 1;
    *palette = dowsing->signal / 20;
}

void DowsingMchn_UpdateIndicatorAppearance(DowsingMchn *dowsing) {
    u32 i;
    u16 blink;
    u16 palette;

    if (dowsing->signal == 0) {
        for (i = 0; i < DOWSING_MCHN_NUM_ANGLES; i++) {
            Sprite_SetDrawFlag(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS], FALSE);
        }
        return;
    }
    DowsingMchn_GetIndicatorAnim(dowsing, &blink, &palette);
    for (i = 0; i < DOWSING_MCHN_NUM_ANGLES; i++) {
        Sprite_SetPalIndexRespectVramOffset(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS], palette + 1);
        Sprite_SetAnimCtrlSeq(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS], blink);
        Sprite_SetDrawFlag(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS], TRUE);
    }
}

void DowsingMchn_SetSpriteRotation(Sprite *sprite, int deg) {
    Sprite_SetRotationAndAffineType(sprite, GF_DegreeToSinCosIdx((u16)deg), 2);
}

#pragma force_active on

// Moves the indicators one step: the last step lands on the target, the others on `next`.
void DowsingMchn_UpdateIndicatorPositions(DowsingMchn *dowsing) {
    u32 i;
    VecFx32 pos;

    if (dowsing->angleSteps) {
        dowsing->angleSteps--;
        if (!dowsing->angleSteps) {
            for (i = 0; i < DOWSING_MCHN_NUM_ANGLES; i++) {
                dowsing->angles[i].displayed = dowsing->angles[i].target;
                DowsingMchn_GetIndicatorPos(&pos, dowsing->angles[i].displayed);
                Sprite_SetMatrix(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS], &pos);
                DowsingMchn_SetSpriteRotation(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS], dowsing->angles[i].displayed);
            }
        } else {
            for (i = 0; i < DOWSING_MCHN_NUM_ANGLES; i++) {
                dowsing->angles[i].displayed = dowsing->angles[i].next;
                DowsingMchn_GetIndicatorPos(&pos, dowsing->angles[i].displayed);
                Sprite_SetMatrix(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS], &pos);
                DowsingMchn_SetSpriteRotation(dowsing->sprites[i + DOWSING_MCHN_SPRITE_INDICATORS], dowsing->angles[i].displayed);
                sDowsingMchnStepAngleFuncs[dowsing->level](&dowsing->angles[i]);
            }
        }
    }
}

#pragma force_active reset

// Level 4: brighter the closer the cell is to the item; otherwise a fixed value per level.
u8 DowsingMchn_GetGridCellTarget(DowsingMchn *dowsing, u16 x, u16 y) {
    u32 level = dowsing->level;

    if (level == 4) {
        int strength = 16 - (abs(dowsing->itemX + 7 - x) + abs(dowsing->itemY + 7 - y));
        if (strength < 5) {
            strength = 5;
        } else if (strength > 16) {
            strength = 16;
        }
        return strength * 10 - 1;
    }
    return sDowsingMchnLevelGridValues[level * 2];
}

#pragma force_active on

// Cells within distance 4 of the touched cell rise toward their target, the others fade out.
void DowsingMchn_UpdateGrid(DowsingMchn *dowsing, BOOL touching) {
    s16 x;
    s16 y;
    s16 touchX;
    s16 touchY;
    u8 target;

    if (touching == TRUE) {
        touchX = (dowsing->touchX >> 3) - 3;
        touchY = (dowsing->touchY >> 3) - 5;
    }
    for (y = 0; y < DOWSING_MCHN_GRID_SIZE; y++) {
        for (x = 0; x < DOWSING_MCHN_GRID_SIZE; x++) {
            if (sDowsingMchnGridMask[y] & (1 << x)) {
                continue;
            }
            if (touching == TRUE) {
                if (x >= touchX - 3 && x <= touchX + 3 && y >= touchY - 3 && y <= touchY + 3 && abs(touchX - x) + abs(touchY - y) <= 4) {
                    target = DowsingMchn_GetGridCellTarget(dowsing, x, y);
                    if (dowsing->grid[y][x] < target) {
                        dowsing->grid[y][x] += 10;
                        if (dowsing->grid[y][x] > target) {
                            dowsing->grid[y][x] = target;
                        }
                    } else {
                        dowsing->grid[y][x] = target;
                    }
                } else {
                    if (dowsing->grid[y][x] >= 2) {
                        dowsing->grid[y][x] -= 2;
                    } else {
                        dowsing->grid[y][x] = 0;
                    }
                }
            } else {
                if (dowsing->grid[y][x] >= 2) {
                    dowsing->grid[y][x] -= 2;
                } else {
                    dowsing->grid[y][x] = 0;
                }
            }
            if (dowsing->grid[y][x] == 0) {
                FillBgTilemapRect(dowsing->bgConfig, GF_BG_LYR_SUB_2, 0x1001, x + 3, y + 5, 1, 1, TILEMAP_FILL_OVWT_PAL);
            } else {
                FillBgTilemapRect(dowsing->bgConfig, GF_BG_LYR_SUB_2, sDowsingMchnGridTiles[dowsing->grid[y][x] / 10], x + 3, y + 5, 1, 1, TILEMAP_FILL_OVWT_PAL);
            }
        }
    }
    ScheduleBgTilemapBufferTransfer(dowsing->bgConfig, GF_BG_LYR_SUB_2);
}

#pragma opt_propagation off

// Every 11 frames; the pitch rises with the signal and, at level 4, with the item's closeness.
void DowsingMchn_PlaySignalSE(DowsingMchn *dowsing) {
    u16 blink;
    u16 palette;
    s16 pitch;

    DowsingMchn_GetIndicatorAnim(dowsing, &blink, &palette);
    pitch = (blink + 2 * palette) * 64 - 960;
    if (dowsing->level == 4) {
        int touchX = (dowsing->touchX >> 3) - 3;
        int touchY = (dowsing->touchY >> 3) - 5;
        pitch = pitch + (16 - (abs(dowsing->itemX + 7 - touchX) + abs(dowsing->itemY + 7 - touchY))) * 64;
    }
    if (dowsing->seTimer == 0) {
        PlaySE_SetPitch(SEQ_SE_GS_DOWSING_SINGLE, pitch);
    }
    if (dowsing->seTimer == 10) {
        dowsing->seTimer = 0;
    } else {
        dowsing->seTimer++;
    }
    if (dowsing->sePitch != pitch) {
        dowsing->sePitch = pitch;
    }
}

#pragma opt_propagation reset
#pragma force_active reset

// Returns TRUE if the stylus has not moved.
BOOL DowsingMchn_UpdateTouchPos(DowsingMchn *dowsing) {
    u32 x;
    u32 y;

    System_GetTouchHeldCoords(&x, &y);
    if (dowsing->touchX != x || dowsing->touchY != y) {
        dowsing->touchX = x;
        dowsing->touchY = y;
        return FALSE;
    }
    return TRUE;
}

// Action types: 0/2 CHECK, 1 TALK, 3 FISHING, 4 NEXT (a textbox is open).
void DowsingMchn_UpdateActionLabel(DowsingMchn *dowsing) {
    int actionType;

    if (dowsing->fieldSystem->textbox_open == TRUE) {
        actionType = 4;
    } else if (!FieldSystem_IsPlayerMovementAllowed(dowsing->fieldSystem)) {
        LocalMapObject *playerObj = PlayerAvatar_GetMapObject(FieldSystem_GetPlayerAvatar(dowsing->fieldSystem));
        u32 spriteId = MapObject_GetSpriteID(playerObj);
        if ((u32)(spriteId - SPRITE_FISHINGHERO) <= SPRITE_FISH_HEROINE - SPRITE_FISHINGHERO) {
            actionType = sub_0205F330(playerObj) == 1 ? 3 : 4;
        } else {
            actionType = dowsing->actionType;
        }
    } else {
        actionType = FieldSystem_GetFacingActionType(dowsing->fieldSystem);
        if (actionType == 1) {
            LocalMapObject *facingObj;
            FieldSystem_GetFacingObject(dowsing->fieldSystem, &facingObj);
            if (IsScriptIdWithoutTalkAction(MapObject_GetScriptID(facingObj)) == TRUE || IsSpriteIdWithoutTalkAction(MapObject_GetSpriteID(facingObj)) == TRUE) {
                actionType = 0;
            }
        }
    }
    if (actionType != dowsing->actionType) {
        dowsing->actionType = actionType;
        ScheduleWindowCopyToVram(&dowsing->windows[sDowsingMchnActionTypeToWindow[dowsing->actionType]]);
    }
}

// Shows the pressed animation on a new touch; TRUE while the button stays pressed.
BOOL DowsingMchn_HandleButton(const TouchscreenHitbox *hitbox, Sprite *sprite, int idleAnim, int pressedAnim) {
    BOOL held = TouchscreenHitbox_TouchHeldIsIn(hitbox);
    u16 anim = Sprite_GetAnimationNumber(sprite);

    if (held == TRUE && System_GetTouchNew()) {
        Sprite_SetAnimCtrlSeq(sprite, pressedAnim);
        return TRUE;
    }
    if (held == TRUE && anim == pressedAnim) {
        return TRUE;
    }
    Sprite_SetAnimCtrlSeq(sprite, idleAnim);
    return FALSE;
}

void DowsingMchn_HandleActionButton(DowsingMchn *dowsing) {
    if (FieldSystem_IsFieldMapRunning(dowsing->fieldSystem) == TRUE) {
        DowsingMchn_UpdateActionLabel(dowsing);
    }
    if (DowsingMchn_HandleButton(&sDowsingMchnActionButtonHitbox, dowsing->sprites[DOWSING_MCHN_SPRITE_ACTION], 5, 6)) {
        gSystem.simulatedInputs = TRUE;
    }
}

void DowsingMchn_HandleRunningShoesButton(DowsingMchn *dowsing) {
    PlayerAvatar *avatar;
    u16 lock;

    if (PlayerAvatar_GetState(dowsing->fieldSystem->playerAvatar) == 1 || DowsingMchn_ShouldHideButtons(dowsing) == TRUE) {
        Sprite_SetDrawFlag(dowsing->sprites[1], FALSE);
        Sprite_SetDrawFlag(dowsing->sprites[2], FALSE);
        return;
    }
    Sprite_SetDrawFlag(dowsing->sprites[1], TRUE);
    Sprite_SetDrawFlag(dowsing->sprites[2], TRUE);
    if (TouchscreenHitbox_TouchNewIsIn(&sDowsingMchnRunningShoesHitbox) == TRUE) {
        avatar = FieldSystem_GetPlayerAvatar(dowsing->fieldSystem);
        lock = PlayerAvatar_CheckRunningShoesLock(avatar);
        if (lock == 0) {
            Sprite_SetAnimCtrlSeq(dowsing->sprites[1], 4);
            Sprite_SetAnimCtrlSeq(dowsing->sprites[2], 7);
        } else {
            Sprite_SetAnimCtrlSeq(dowsing->sprites[1], 3);
            Sprite_SetAnimCtrlSeq(dowsing->sprites[2], 11);
        }
        // Called with an unnarrowed u32 lock (player_avatar.h declares u16).
        ((void (*)(PlayerAvatar *, u32))PlayerAvatar_SetRunningShoesLock)(avatar, lock ^ 1);
    }
}

// lastTouchMenuInput 11 makes field_control.c close the app.
void DowsingMchn_HandleExitButton(DowsingMchn *dowsing) {
    if (DowsingMchn_ShouldHideButtons(dowsing) == TRUE) {
        ClearWindowTilemapAndScheduleTransfer(&dowsing->windows[DOWSING_MCHN_WINDOW_EXIT]);
        Sprite_SetAnimCtrlSeq(dowsing->sprites[DOWSING_MCHN_SPRITE_EXIT], 8);
        Sprite_SetDrawFlag(dowsing->sprites[DOWSING_MCHN_SPRITE_EXIT], FALSE);
        return;
    }
    ScheduleWindowCopyToVram(&dowsing->windows[DOWSING_MCHN_WINDOW_EXIT]);
    Sprite_SetDrawFlag(dowsing->sprites[DOWSING_MCHN_SPRITE_EXIT], TRUE);
    if (DowsingMchn_HandleButton(&sDowsingMchnExitButtonHitbox, dowsing->sprites[DOWSING_MCHN_SPRITE_EXIT], 8, 9)) {
        dowsing->fieldSystem->lastTouchMenuInput = 11;
        MenuInputStateMgr_SetState(&dowsing->fieldSystem->menuInputState, MENU_INPUT_STATE_TOUCH);
    }
}

BOOL DowsingMchn_ShouldHideButtons(DowsingMchn *dowsing) {
    if (FieldSystem_TaskIsRunning(dowsing->fieldSystem) == TRUE && dowsing->fieldSystem->unkD2_7 == 0) {
        return TRUE;
    }
    return FALSE;
}
