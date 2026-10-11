#include <nitro/gx/gx_vramcnt.h>

#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0196.h"

#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "unk_0202C034.h"

// Field bottom-screen app 6: when the Friend Roster is full, the player picks the entry to delete
// (field_friendship_exchange.c). *result: -1 while running, -2 = QUIT, otherwise the roster index.

#define FRIEND_ROSTER_APP_NUM_WINDOWS    18
#define FRIEND_ROSTER_APP_NUM_SPRITES    4
#define FRIEND_ROSTER_APP_NAMES_PER_PAGE 8
#define FRIEND_ROSTER_APP_NUM_PAGES      4

#define FRIEND_ROSTER_APP_SPRITE_CURSOR 0
#define FRIEND_ROSTER_APP_SPRITE_CANCEL 1
#define FRIEND_ROSTER_APP_INPUT_CANCEL  8

enum FriendRosterAppState {
    FRIEND_ROSTER_APP_STATE_INPUT = 0,
    FRIEND_ROSTER_APP_STATE_BLINK,
    FRIEND_ROSTER_APP_STATE_REDRAW,
    FRIEND_ROSTER_APP_STATE_DONE,
};

// Blink of a touched sprite (isBg = 0, target = sprite index) or of a name rect on a sub BG.
typedef struct FriendRosterAppBlink {
    u8 isBg : 1;
    u8 target : 7;
    u8 seqA : 4;
    u8 seqB : 4;
    u8 phase;
    u8 timer;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} FriendRosterAppBlink;

typedef struct FriendRosterAppData {
    BgConfig *bgConfig;
    void *unk_04;
    FieldSystem *fieldSystem;
    int *unk_0C;
    SysTask *task;
    UnkStruct_021D2230 *roster;
    GridInputHandler *gridInput;
    FriendRosterAppBlink blink;
    Window windows[FRIEND_ROSTER_APP_NUM_WINDOWS]; // 0 QUIT, 1 page number, 2-9 and 10-17 the two name sets
    MsgData *msgData;
    MessageFormat *msgFormat;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    SpriteResource *resObjs[4];
    Sprite *sprites[FRIEND_ROSTER_APP_NUM_SPRITES];
    u8 state;
    u8 nextState;
    s8 page;
    u8 bufferToggle;
    int selection;
    int *result;
} FriendRosterAppData; // size: 0x2B4

extern const TouchscreenHitbox sFriendRosterAppPageArrowHitboxes[];
extern const GridCallbacks sFriendRosterAppGridCallbacks;
extern const u8 sFriendRosterAppSpriteSpecs[FRIEND_ROSTER_APP_NUM_SPRITES][4];
extern const BgTemplate sFriendRosterAppBgTemplateSub2;
extern const BgTemplate sFriendRosterAppBgTemplateSub0;
extern const BgTemplate sFriendRosterAppBgTemplateSub1;
extern const TouchscreenHitbox sFriendRosterAppGridHitboxes[];
extern const DpadMenuBox sFriendRosterAppGridDpadBoxes[];
extern const WindowTemplate sFriendRosterAppWindowTemplates[];

SysTask *FriendRosterApp_Init(BgConfig *bgConfig, void *a1, FieldSystem *fieldSystem, int *result);
void FriendRosterApp_Exit(void *unused, SysTask *task);
BOOL FriendRosterApp_IsExitDone(void);
void FriendRosterApp_SetVramBanks(void);
void FriendRosterApp_InitBgs(BgConfig *bgConfig);
void FriendRosterApp_FreeBgs(BgConfig *bgConfig);
void FriendRosterApp_LoadBgGraphics(FriendRosterAppData *data, NARC *narc);
void FriendRosterApp_InitText(FriendRosterAppData *data);
void FriendRosterApp_FreeText(FriendRosterAppData *data);
void FriendRosterApp_InitWindows(FriendRosterAppData *data);
void FriendRosterApp_RemoveWindows(FriendRosterAppData *data);
void FriendRosterApp_PrintPageNumber(FriendRosterAppData *data);
void FriendRosterApp_PrintNames(FriendRosterAppData *data);
void FriendRosterApp_InitSprites(FriendRosterAppData *data, NARC *narc);
void FriendRosterApp_FreeSprites(FriendRosterAppData *data);
void FriendRosterApp_CreateResourceManagers(FriendRosterAppData *data);
void FriendRosterApp_DestroyResourceManagers(FriendRosterAppData *data);
void FriendRosterApp_LoadSpriteResources(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charId, int plttId, int cellId, int animId);
void FriendRosterApp_CreateSpriteHeader(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority);
void FriendRosterApp_CreateSprite(FriendRosterAppData *data, int index, SpriteResourcesHeader *header, const u8 *spec);
void FriendRosterApp_CreateSprites(FriendRosterAppData *data, NARC *narc);
void FriendRosterApp_DeleteSprites(FriendRosterAppData *data);
void FriendRosterApp_AnimateSprites(FriendRosterAppData *data);
void FriendRosterApp_SetCursorPos(FriendRosterAppData *data, int pos);
void FriendRosterApp_InitGridInput(FriendRosterAppData *data);
void FriendRosterApp_FreeGridInput(FriendRosterAppData *data);
void FriendRosterApp_GridNoop(void);
void FriendRosterApp_OnCursorMove(void *data, int newTarget, int prevTarget);
void FriendRosterApp_Task(SysTask *task, void *taskData);
int FriendRosterApp_HandleInput(FriendRosterAppData *data);
int FriendRosterApp_UpdateBlink(FriendRosterAppData *data);
int FriendRosterApp_ChangePage(FriendRosterAppData *data, int delta);
void FriendRosterApp_SetBlinkPalette(FriendRosterAppData *data, u8 palette);
BOOL FriendRosterApp_StepBlink(FriendRosterAppData *data);
int FriendRosterApp_StartSpriteBlink(FriendRosterAppData *data, u8 target, u8 seqA, u8 seqB, u8 nextState);
int FriendRosterApp_StartNameBlink(FriendRosterAppData *data, u8 x, u8 y, u8 nextState);

SysTask *FriendRosterApp_Init(BgConfig *bgConfig, void *a1, FieldSystem *fieldSystem, int *result) {
    SysTask *task;
    FriendRosterAppData *data;
    NARC *narc;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);
    G2S_BlendNone();
    task = CreateSysTaskAndEnvironment(FriendRosterApp_Task, sizeof(FriendRosterAppData), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->bgConfig = bgConfig;
    data->unk_04 = a1;
    data->fieldSystem = fieldSystem;
    data->unk_0C = result;
    data->task = task;
    data->roster = sub_0202C6F4(fieldSystem->saveData);
    data->result = result;
    *data->result = -1;
    FontID_Alloc(4, HEAP_ID_8);
    narc = NARC_New(NARC_a_2_5_9, HEAP_ID_8);
    FriendRosterApp_SetVramBanks();
    FriendRosterApp_InitBgs(bgConfig);
    FriendRosterApp_LoadBgGraphics(data, narc);
    FriendRosterApp_InitText(data);
    FriendRosterApp_InitWindows(data);
    FriendRosterApp_InitSprites(data, narc);
    FriendRosterApp_InitGridInput(data);
    NARC_Delete(narc);
    return task;
}

void FriendRosterApp_Exit(void *unused, SysTask *task) {
    FriendRosterAppData *data = SysTask_GetData(task);
    FriendRosterApp_FreeGridInput(data);
    FriendRosterApp_FreeSprites(data);
    FriendRosterApp_RemoveWindows(data);
    FriendRosterApp_FreeText(data);
    FriendRosterApp_FreeBgs(data->bgConfig);
    FontID_Release(4);
    DestroySysTaskAndEnvironment(task);
    Heap_Destroy(HEAP_ID_8);
}

BOOL FriendRosterApp_IsExitDone(void) {
    return TRUE;
}

void FriendRosterApp_SetVramBanks(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
}

void FriendRosterApp_InitBgs(BgConfig *bgConfig) {
    BgTemplate bg4;
    BgTemplate bg5;
    BgTemplate bg6;

    GXS_SetGraphicsMode(GX_BGMODE_0);
    bg4 = sFriendRosterAppBgTemplateSub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bg4, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_8);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    bg5 = sFriendRosterAppBgTemplateSub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bg5, GF_BG_TYPE_TEXT);
    bg6 = sFriendRosterAppBgTemplateSub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bg6, GF_BG_TYPE_TEXT);
}

void FriendRosterApp_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
}

void FriendRosterApp_LoadBgGraphics(FriendRosterAppData *data, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, data->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_8);
}

void FriendRosterApp_InitText(FriendRosterAppData *data) {
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0196_bin, HEAP_ID_8);
    data->msgFormat = MessageFormat_New(HEAP_ID_8);
}

void FriendRosterApp_FreeText(FriendRosterAppData *data) {
    MessageFormat_Delete(data->msgFormat);
    DestroyMsgData(data->msgData);
}

void FriendRosterApp_InitWindows(FriendRosterAppData *data) {
    String *string;
    u16 i;
    u16 center;
    u16 x;

    for (i = 0; i < FRIEND_ROSTER_APP_NUM_WINDOWS; i++) {
        AddWindow(data->bgConfig, &data->windows[i], &sFriendRosterAppWindowTemplates[i]);
    }
    FillWindowPixelBuffer(&data->windows[0], 0);
    string = NewString_ReadMsgData(data->msgData, msg_0196_00029);
    center = GetWindowWidth(&data->windows[0]) * 8 / 2;
    x = center - FontID_String_GetWidth(4, string, 0) / 2;
    AddTextPrinterParameterizedWithColor(&data->windows[0], 4, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
    String_Delete(string);
    CopyWindowPixelsToVram_TextMode(&data->windows[0]);
    ScheduleWindowCopyToVram(&data->windows[0]);
    FriendRosterApp_PrintPageNumber(data);
    FriendRosterApp_PrintNames(data);
}

void FriendRosterApp_RemoveWindows(FriendRosterAppData *data) {
    u32 i = 0;
    Window *window = &data->windows[0];
    do {
        RemoveWindow(window);
        i++;
        window++;
    } while (i < FRIEND_ROSTER_APP_NUM_WINDOWS);
}

void FriendRosterApp_PrintPageNumber(FriendRosterAppData *data) {
    String *string;
    String *buf;
    String *template;
    u16 width;
    u16 center;
    u16 x;

    FillWindowPixelBuffer(&data->windows[1], 0);
    string = NewString_ReadMsgData(data->msgData, msg_0196_00028);
    width = FontID_String_GetWidth(0, string, 0);
    center = (GetWindowWidth(&data->windows[1]) * 8 - width) / 2;
    AddTextPrinterParameterizedWithColor(&data->windows[1], 0, string, center, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
    String_Delete(string);
    buf = String_New(4, HEAP_ID_8);
    template = NewString_ReadMsgData(data->msgData, msg_0196_00027);
    BufferIntegerAsString(data->msgFormat, 0, FRIEND_ROSTER_APP_NUM_PAGES, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(data->msgFormat, buf, template);
    AddTextPrinterParameterizedWithColor(&data->windows[1], 0, buf, center + width, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
    String_Delete(template);
    template = NewString_ReadMsgData(data->msgData, msg_0196_00027);
    BufferIntegerAsString(data->msgFormat, 0, data->page + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(data->msgFormat, buf, template);
    x = center - FontID_String_GetWidth(0, buf, 0);
    AddTextPrinterParameterizedWithColor(&data->windows[1], 0, buf, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
    String_Delete(template);
    String_Delete(buf);
    CopyWindowPixelsToVram_TextMode(&data->windows[1]);
    ScheduleWindowCopyToVram(&data->windows[1]);
}

// Prints the current page into the name window set that is not on screen, then swaps sets.
void FriendRosterApp_PrintNames(FriendRosterAppData *data) {
    PlayerProfile *profile;
    String *template;
    String *buf;
    u32 center;
    Window *windows;
    u16 i;

    if (data->bufferToggle == 0) {
        windows = &data->windows[2];
    } else {
        windows = &data->windows[2 + FRIEND_ROSTER_APP_NAMES_PER_PAGE];
    }
    template = NewString_ReadMsgData(data->msgData, msg_0196_00030);
    buf = String_New(0x40, HEAP_ID_8);
    profile = PlayerProfile_New(HEAP_ID_8);
    center = (u16)(GetWindowWidth(&windows[0]) * 8 / 2);
    for (i = 0; i < FRIEND_ROSTER_APP_NAMES_PER_PAGE; i++) {
        Save_Profile_PlayerName_Set(profile, sub_0202C254(data->roster, i + data->page * FRIEND_ROSTER_APP_NAMES_PER_PAGE));
        BufferPlayersName(data->msgFormat, 0, profile);
        StringExpandPlaceholders(data->msgFormat, buf, template);
        FillWindowPixelBuffer(&windows[i], 0);
        AddTextPrinterParameterizedWithColor(&windows[i], 4, buf, center - FontID_String_GetWidth(0, buf, 0) / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
        CopyWindowPixelsToVram_TextMode(&windows[i]);
        ScheduleWindowCopyToVram(&windows[i]);
    }
    Heap_Free(profile);
    String_Delete(buf);
    String_Delete(template);
    data->bufferToggle ^= 1;
}

void FriendRosterApp_InitSprites(FriendRosterAppData *data, NARC *narc) {
    data->spriteList = G2dRenderer_Init(FRIEND_ROSTER_APP_NUM_SPRITES, &data->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&data->renderer, 0, FX32_CONST(256));
    FriendRosterApp_CreateResourceManagers(data);
    FriendRosterApp_CreateSprites(data, narc);
}

void FriendRosterApp_FreeSprites(FriendRosterAppData *data) {
    FriendRosterApp_DeleteSprites(data);
    FriendRosterApp_DestroyResourceManagers(data);
    SpriteList_Delete(data->spriteList);
}

void FriendRosterApp_CreateResourceManagers(FriendRosterAppData *data) {
    u32 i;
    for (i = 0; i < 4; i++) {
        data->resMans[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, HEAP_ID_8);
    }
}

void FriendRosterApp_DestroyResourceManagers(FriendRosterAppData *data) {
    u32 i;
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(data->resMans[i]);
    }
}

void FriendRosterApp_LoadSpriteResources(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charId, int plttId, int cellId, int animId) {
    resObjs[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CHAR], narc, charFileId, TRUE, charId, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_8);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resObjs[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(resObjs[GF_GFX_RES_TYPE_CHAR]);
    resObjs[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_PLTT], narc, plttFileId, FALSE, plttId, NNS_G2D_VRAM_TYPE_2DSUB, plttNum, HEAP_ID_8);
    SpriteTransfer_CreatePlttTransferTask(resObjs[GF_GFX_RES_TYPE_PLTT]);
    sub_0200A740(resObjs[GF_GFX_RES_TYPE_PLTT]);
    resObjs[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CELL], narc, cellFileId, TRUE, cellId, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    resObjs[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_ANIM], narc, animFileId, TRUE, animId, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
}

void FriendRosterApp_CreateSpriteHeader(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority) {
    int charId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_CHAR]);
    int plttId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_PLTT]);
    int cellId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_CELL]);
    int animId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_ANIM]);
    CreateSpriteResourcesHeader(header, charId, plttId, cellId, animId, -1, -1, 0, priority, resMans[GF_GFX_RES_TYPE_CHAR], resMans[GF_GFX_RES_TYPE_PLTT], resMans[GF_GFX_RES_TYPE_CELL], resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

// spec: {x, y, drawPriority, animSeq}
void FriendRosterApp_CreateSprite(FriendRosterAppData *data, int index, SpriteResourcesHeader *header, const u8 *spec) {
    SpriteTemplate template;
    template.spriteList = data->spriteList;
    template.header = header;
    template.position.x = spec[0] << FX32_SHIFT;
    template.position.y = (spec[1] << FX32_SHIFT) + FX32_CONST(256);
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = spec[2];
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_8;
    data->sprites[index] = Sprite_CreateAffine(&template);
    Sprite_SetAnimCtrlSeq(data->sprites[index], spec[3]);
}

void FriendRosterApp_CreateSprites(FriendRosterAppData *data, NARC *narc) {
    SpriteResourcesHeader header;
    u32 i;
    FriendRosterApp_LoadSpriteResources(data->resObjs, data->resMans, narc, 4, 7, 5, 6, 1, 0x781, 0x781, 0x781, 0x781);
    FriendRosterApp_CreateSpriteHeader(data->resObjs, data->resMans, &header, 1);
    for (i = 0; i < FRIEND_ROSTER_APP_NUM_SPRITES; i++) {
        FriendRosterApp_CreateSprite(data, i, &header, sFriendRosterAppSpriteSpecs[i]);
    }
}

void FriendRosterApp_DeleteSprites(FriendRosterAppData *data) {
    u32 i;
    for (i = 0; i < FRIEND_ROSTER_APP_NUM_SPRITES; i++) {
        Sprite_Delete(data->sprites[i]);
    }
    SpriteTransfer_DeleteCharTransferTask(data->resObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(data->resObjs[GF_GFX_RES_TYPE_PLTT]);
}

void FriendRosterApp_AnimateSprites(FriendRosterAppData *data) {
    u32 i;
    for (i = 0; i < FRIEND_ROSTER_APP_NUM_SPRITES; i++) {
        Sprite_UpdateAnim(data->sprites[i], FX32_ONE);
    }
}

void FriendRosterApp_SetCursorPos(FriendRosterAppData *data, int pos) {
    const DpadMenuBox *box;
    VecFx32 vec;

    box = GridInputHandler_GetDpadBox(data->gridInput, pos);
    vec.x = box->left << FX32_SHIFT;
    vec.y = (box->top << FX32_SHIFT) + FX32_CONST(256);
    Sprite_SetMatrix(data->sprites[FRIEND_ROSTER_APP_SPRITE_CURSOR], &vec);
    if (pos == FRIEND_ROSTER_APP_INPUT_CANCEL) {
        Sprite_SetAnimCtrlSeq(data->sprites[FRIEND_ROSTER_APP_SPRITE_CURSOR], 1);
    } else {
        Sprite_SetAnimCtrlSeq(data->sprites[FRIEND_ROSTER_APP_SPRITE_CURSOR], 0);
    }
}

void FriendRosterApp_InitGridInput(FriendRosterAppData *data) {
    data->gridInput = GridInputHandler_Create(sFriendRosterAppGridHitboxes, sFriendRosterAppGridDpadBoxes, &sFriendRosterAppGridCallbacks, data, TRUE, 0, HEAP_ID_8);
    FriendRosterApp_SetCursorPos(data, 0);
}

void FriendRosterApp_FreeGridInput(FriendRosterAppData *data) {
    GridInputHandler_Free(data->gridInput);
}

void FriendRosterApp_GridNoop(void) {
}

void FriendRosterApp_OnCursorMove(void *data, int newTarget, int prevTarget) {
    FriendRosterApp_SetCursorPos(data, newTarget);
}

void FriendRosterApp_Task(SysTask *task, void *taskData) {
    FriendRosterAppData *data = taskData;

    switch (data->state) {
    case FRIEND_ROSTER_APP_STATE_INPUT:
        data->state = FriendRosterApp_HandleInput(data);
        break;
    case FRIEND_ROSTER_APP_STATE_BLINK:
        data->state = FriendRosterApp_UpdateBlink(data);
        break;
    case FRIEND_ROSTER_APP_STATE_REDRAW:
        FriendRosterApp_PrintPageNumber(data);
        FriendRosterApp_PrintNames(data);
        data->state = FRIEND_ROSTER_APP_STATE_INPUT;
        break;
    case FRIEND_ROSTER_APP_STATE_DONE:
        *data->result = data->selection;
        break;
    }
    FriendRosterApp_AnimateSprites(data);
    SpriteList_RenderAndAnimateSprites(data->spriteList);
}

int FriendRosterApp_HandleInput(FriendRosterAppData *data) {
    u32 input;
    int touch;

    touch = TouchscreenHitbox_FindRectAtTouchNew(sFriendRosterAppPageArrowHitboxes);
    if (touch == 0) {
        PlaySE(SEQ_SE_DP_SELECT78);
        return FriendRosterApp_ChangePage(data, -1);
    }
    if (touch == 1) {
        PlaySE(SEQ_SE_DP_SELECT78);
        return FriendRosterApp_ChangePage(data, 1);
    }
    input = GridInputHandler_HandleInput_AllowHold(data->gridInput);
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        PlaySE(SEQ_SE_DP_DECIDE);
        data->selection = input + data->page * FRIEND_ROSTER_APP_NAMES_PER_PAGE;
        return FriendRosterApp_StartNameBlink(data, input % 2 * 16 + 2, input / 2 * 5, FRIEND_ROSTER_APP_STATE_DONE);
    case FRIEND_ROSTER_APP_INPUT_CANCEL:
    case GRID_MENU_CANCEL:
        PlaySE(SEQ_SE_DP_DECIDE);
        data->selection = -2;
        return FriendRosterApp_StartSpriteBlink(data, FRIEND_ROSTER_APP_SPRITE_CANCEL, 3, 2, FRIEND_ROSTER_APP_STATE_DONE);
    case GRID_MENU_NOTHING_CHOSEN:
        input = GridInputHandler_GetNextInput(data->gridInput);
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            if (input == 1 || input == 3 || input == 5 || input == 7) {
                PlaySE(SEQ_SE_DP_SELECT);
                return FriendRosterApp_ChangePage(data, 1);
            }
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            if (input == 0 || input == 2 || input == 4 || input == 6) {
                PlaySE(SEQ_SE_DP_SELECT);
                return FriendRosterApp_ChangePage(data, -1);
            }
        }
        break;
    case GRID_MENU_CURSOR_MOVE:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case GRID_MENU_BUTTON_MODE:
        break;
    }
    return FRIEND_ROSTER_APP_STATE_INPUT;
}

int FriendRosterApp_UpdateBlink(FriendRosterAppData *data) {
    if (FriendRosterApp_StepBlink(data) == FALSE) {
        return data->nextState;
    }
    return FRIEND_ROSTER_APP_STATE_BLINK;
}

// Moves to the previous/next page (wrapping) and blinks the page arrow; redraws after the blink.
int FriendRosterApp_ChangePage(FriendRosterAppData *data, int delta) {
    int seqA;
    int seqB;

    data->page += delta;
    if (data->page < 0) {
        data->page = FRIEND_ROSTER_APP_NUM_PAGES - 1;
    }
    if (data->page > FRIEND_ROSTER_APP_NUM_PAGES - 1) {
        data->page = 0;
    }
    if (delta > 0) {
        seqA = 3;
        seqB = 6;
    } else {
        seqA = 2;
        seqB = 4;
    }
    return FriendRosterApp_StartSpriteBlink(data, seqA, seqB + 1, seqB, FRIEND_ROSTER_APP_STATE_REDRAW);
}

void FriendRosterApp_SetBlinkPalette(FriendRosterAppData *data, u8 palette) {
    if (data->blink.isBg == 0) {
        Sprite_SetAnimCtrlSeq(data->sprites[data->blink.target], palette);
    } else {
        BgTilemapRectChangePalette(data->bgConfig, data->blink.target, data->blink.x, data->blink.y, data->blink.width, data->blink.height, palette);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, data->blink.target);
    }
}

BOOL FriendRosterApp_StepBlink(FriendRosterAppData *data) {
    FriendRosterAppBlink *blink = &data->blink;

    switch (blink->phase) {
    case 0:
        FriendRosterApp_SetBlinkPalette(data, blink->seqA);
        blink->phase++;
        break;
    case 1:
        blink->timer++;
        if (blink->timer == 4) {
            FriendRosterApp_SetBlinkPalette(data, blink->seqB);
            blink->timer = 0;
            blink->phase++;
        }
        break;
    case 2:
        blink->timer++;
        if (blink->timer == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

int FriendRosterApp_StartSpriteBlink(FriendRosterAppData *data, u8 target, u8 seqA, u8 seqB, u8 nextState) {
    data->blink.isBg = FALSE;
    data->blink.timer = 0;
    data->blink.phase = 0;
    data->blink.target = target;
    data->blink.seqA = seqA;
    data->blink.seqB = seqB;
    data->nextState = nextState;
    return FRIEND_ROSTER_APP_STATE_BLINK;
}

int FriendRosterApp_StartNameBlink(FriendRosterAppData *data, u8 x, u8 y, u8 nextState) {
    int flags = *(u8 *)&data->blink;
    flags &= ~1;
    flags |= 1; // isBg = TRUE
    *(u8 *)&data->blink = flags;
    data->blink.timer = 0;
    data->blink.phase = 0;
    data->blink.target = GF_BG_LYR_SUB_1;
    data->blink.seqA = 1;
    data->blink.seqB = 0;
    data->blink.x = x;
    data->blink.y = y;
    data->blink.width = 12;
    data->blink.height = 4;
    data->nextState = nextState;
    return FRIEND_ROSTER_APP_STATE_BLINK;
}
