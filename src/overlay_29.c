#include "overlay_29.h"

#include <nitro/gx/gx_vramcnt.h>

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0196.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

#define NUMBER_ENTRY_MAX_DIGITS       6
#define NUMBER_ENTRY_NUM_SPRITES      11
#define NUMBER_ENTRY_SPRITE_CURSOR    0
#define NUMBER_ENTRY_SPRITE_UNDERLINE 1
#define NUMBER_ENTRY_SPRITE_DIGIT_0   5

// Grid input results (GridInputHandler_HandleInput_AllowHold)
#define NUMBER_ENTRY_INPUT_CLEAR   10
#define NUMBER_ENTRY_INPUT_CONFIRM 11
#define NUMBER_ENTRY_INPUT_CANCEL  12

enum NumberEntryState {
    NUMBER_ENTRY_STATE_INPUT = 0,
    NUMBER_ENTRY_STATE_BLINK,
    NUMBER_ENTRY_STATE_DONE,
};

// Descriptor for the "blink" feedback played after each input: either a sprite
// anim-sequence flip (isBg == FALSE, target is a sprite index) or a BG tilemap
// palette flip over a rect (isBg == TRUE, target is a BG layer).
typedef struct NumberEntryBlink {
    u8 isBg : 1;
    u8 target : 7;
    u8 seqA;
    u8 seqB;
    u8 phase : 3;
    u8 timer : 5;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} NumberEntryBlink;

typedef struct NumberEntryData {
    BgConfig *bgConfig;                            // 0x000
    void *unk4;                                    // 0x004
    void *unk8;                                    // 0x008
    NumberEntryArgs *unkC;                         // 0x00C
    NumberEntryArgs *args;                         // 0x010
    SysTask *task;                                 // 0x014
    SpriteList *spriteList;                        // 0x018
    G2dRenderer renderer;                          // 0x01C
    GF_2DGfxResMan *resMans[4];                    // 0x144
    SpriteResource *padResObjs[4];                 // 0x154
    SpriteResource *digitResObjs[4];               // 0x164
    Sprite *sprites[NUMBER_ENTRY_SPRITE_DIGIT_0];  // 0x174 (the all-sprite loops index past this into digitSprites)
    Sprite *digitSprites[NUMBER_ENTRY_MAX_DIGITS]; // 0x188
    Window windows[3];                             // 0x1A0
    GridInputHandler *gridInput;                   // 0x1D0
    NumberEntryBlink blink;                        // 0x1D4
    u32 digits[NUMBER_ENTRY_MAX_DIGITS];           // 0x1DC
    int cursor;                                    // 0x1F4
    u16 state;                                     // 0x1F8
    u16 nextState;                                 // 0x1FA
    u32 numDigits;                                 // 0x1FC
    u32 value;                                     // 0x200
} NumberEntryData;                                 // size: 0x204

static void ov29_0225D620(void);
static void ov29_0225D648(BgConfig *bgConfig);
static void ov29_0225D6B4(BgConfig *bgConfig);
static void ov29_0225D6C8(NumberEntryData *data, NARC *narc);
static void ov29_0225D714(NumberEntryData *data);
static void ov29_0225D7D4(NumberEntryData *data);
static void ov29_0225D7F0(NumberEntryData *data, NARC *narc);
static void ov29_0225D828(NumberEntryData *data);
static void ov29_0225D840(NumberEntryData *data);
static void ov29_0225D864(NumberEntryData *data);
static void ov29_0225D880(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charId, int plttId, int cellId, int animId);
static void ov29_0225D910(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority);
static void ov29_0225D970(NumberEntryData *data, int index, SpriteResourcesHeader *header, const u8 *spec);
static void ov29_0225D9C8(NumberEntryData *data, NARC *narc);
static void ov29_0225DB38(NumberEntryData *data);
static void ov29_0225DB7C(NumberEntryData *data);
static void ov29_0225DB9C(NumberEntryData *data, u32 pos);
static void ov29_0225DBF0(NumberEntryData *data);
static void ov29_0225DC34(NumberEntryData *data);
static void ov29_0225DC44(void *data, int newTarget, int prevTarget);
static void ov29_0225DC48(void *data, int newTarget, int prevTarget);
static void ov29_0225DC50(NumberEntryData *data);
static void ov29_0225DC84(NumberEntryData *data);
static void ov29_0225DCD0(SysTask *task, void *taskData);
static BOOL ov29_0225DEB8(NumberEntryData *data, int delta);
static void ov29_0225DEF4(NumberEntryData *data, u32 digit);
static void ov29_0225DF18(NumberEntryData *data, u8 seq);
static BOOL ov29_0225DF74(NumberEntryData *data);
static void ov29_0225E028(NumberEntryData *data, u8 target, u8 seqA, u8 seqB, u16 nextState);
static void ov29_0225E078(NumberEntryData *data, u8 x, u8 y, u16 nextState);
static void ov29_0225E0E0(NumberEntryData *data);

// All of this overlay's .rodata as ONE const aggregate, fields in retail
// address order (base 0x0225E114, span 0x1F0). MWCC size-sorts separate
// file-scope const objects, which cannot reproduce this interleaved layout
// (1-byte / 8-byte hitbox lists between 16- and 28-byte tables) -- see the
// rodata-consolidate-one-struct-flip pattern.
typedef struct NumberEntryRodata {
    u8 xOffsets1[1];                             // 0x0225E114
    TouchscreenHitbox hitboxes1[2];              // 0x0225E115
    TouchscreenHitbox hitboxes2[3];              // 0x0225E11D
    TouchscreenHitbox hitboxes3[4];              // 0x0225E129 (+3 pad)
    GridCallbacks gridCallbacks;                 // 0x0225E13C
    TouchscreenHitbox hitboxes4[5];              // 0x0225E14C
    TouchscreenHitbox hitboxes5[6];              // 0x0225E160
    WindowTemplate windowTemplates[3];           // 0x0225E178
    TouchscreenHitbox hitboxes6[7];              // 0x0225E190
    const TouchscreenHitbox *hitboxes[7];        // 0x0225E1AC
    BgTemplate bgTemplate4;                      // 0x0225E1C8
    const u8 *xOffsets[7];                       // 0x0225E1E4
    BgTemplate bgTemplate5;                      // 0x0225E200
    u8 spriteSpecs[NUMBER_ENTRY_NUM_SPRITES][4]; // 0x0225E21C
    TouchscreenHitbox padHitboxes[14];           // 0x0225E248
    DpadMenuBox padDpadBoxes[13];                // 0x0225E280
    u8 xOffsets2[4];                             // 0x0225E2E8
    u8 xOffsets3[4];                             // 0x0225E2EC
    u8 xOffsets4[4];                             // 0x0225E2F0
    u8 xOffsets5[8];                             // 0x0225E2F4
    u8 xOffsets6[8];                             // 0x0225E2FC
} NumberEntryRodata;

static const NumberEntryRodata sRodata = {
    // xOffsets1 (0x0225E114)
    { 0x80 },
    // hitboxes1 (0x0225E115)
    {
     { 0x08, 0x27, 0x70, 0x8F },
     { TOUCHSCREEN_RECTLIST_END, 0, 0, 0 },
     },
    // hitboxes2 (0x0225E11D)
    {
     { 0x08, 0x27, 0x60, 0x7F },
     { 0x08, 0x27, 0x80, 0x9F },
     { TOUCHSCREEN_RECTLIST_END, 0, 0, 0 },
     },
    // hitboxes3 (0x0225E129)
    {
     { 0x08, 0x27, 0x50, 0x6F },
     { 0x08, 0x27, 0x70, 0x8F },
     { 0x08, 0x27, 0x90, 0xAF },
     { TOUCHSCREEN_RECTLIST_END, 0, 0, 0 },
     },
    // gridCallbacks (0x0225E13C)
    {
     ov29_0225DC44,
     ov29_0225DC44,
     ov29_0225DC48,
     ov29_0225DC48,
     },
    // hitboxes4 (0x0225E14C)
    {
     { 0x08, 0x27, 0x40, 0x5F },
     { 0x08, 0x27, 0x60, 0x7F },
     { 0x08, 0x27, 0x80, 0x9F },
     { 0x08, 0x27, 0xA0, 0xBF },
     { TOUCHSCREEN_RECTLIST_END, 0, 0, 0 },
     },
    // hitboxes5 (0x0225E160)
    {
     { 0x08, 0x27, 0x30, 0x4F },
     { 0x08, 0x27, 0x50, 0x6F },
     { 0x08, 0x27, 0x70, 0x8F },
     { 0x08, 0x27, 0x90, 0xAF },
     { 0x08, 0x27, 0xB0, 0xCF },
     { TOUCHSCREEN_RECTLIST_END, 0, 0, 0 },
     },
    // windowTemplates (0x0225E178)
    {
     { GF_BG_LYR_SUB_0, 1, 21, 8, 2, 0, 0x001 },
     { GF_BG_LYR_SUB_0, 12, 21, 8, 2, 0, 0x011 },
     { GF_BG_LYR_SUB_0, 23, 21, 8, 2, 0, 0x021 },
     },
    // hitboxes6 (0x0225E190)
    {
     { 0x08, 0x27, 0x20, 0x3F },
     { 0x08, 0x27, 0x40, 0x5F },
     { 0x08, 0x27, 0x60, 0x7F },
     { 0x08, 0x27, 0x80, 0x9F },
     { 0x08, 0x27, 0xA0, 0xBF },
     { 0x08, 0x27, 0xC0, 0xE0 },
     { TOUCHSCREEN_RECTLIST_END, 0, 0, 0 },
     },
    // hitboxes (0x0225E1AC), indexed by numDigits
    {
     NULL,
     sRodata.hitboxes1,
     sRodata.hitboxes2,
     sRodata.hitboxes3,
     sRodata.hitboxes4,
     sRodata.hitboxes5,
     sRodata.hitboxes6,
     },
    // bgTemplate4 (0x0225E1C8)
    {
     0,
     0,
     0x800,
     0,
     GF_BG_SCR_SIZE_256x256,
     GX_BG_COLORMODE_16,
     GX_BG_SCRBASE_0x7800,
     GX_BG_CHARBASE_0x00000,
     GX_BG_EXTPLTT_01,
     0,
     0,
     0,
     FALSE,
     },
    // xOffsets (0x0225E1E4), indexed by numDigits
    {
     NULL,
     sRodata.xOffsets1,
     sRodata.xOffsets2,
     sRodata.xOffsets3,
     sRodata.xOffsets4,
     sRodata.xOffsets5,
     sRodata.xOffsets6,
     },
    // bgTemplate5 (0x0225E200)
    {
     0,
     0,
     0x800,
     0,
     GF_BG_SCR_SIZE_256x256,
     GX_BG_COLORMODE_16,
     GX_BG_SCRBASE_0x7000,
     GX_BG_CHARBASE_0x04000,
     GX_BG_EXTPLTT_01,
     1,
     0,
     0,
     FALSE,
     },
    // spriteSpecs (0x0225E21C): { x, y, drawPriority, animSeq }
    {
     { 0x20, 0x20, 0, 1 },
     { 0x30, 0x24, 0, 0 },
     { 0x28, 0xB0, 1, 3 },
     { 0x80, 0xB0, 1, 3 },
     { 0xD8, 0xB0, 1, 3 },
     { 0x30, 0x18, 1, 1 },
     { 0x50, 0x18, 1, 1 },
     { 0x70, 0x18, 1, 1 },
     { 0x90, 0x18, 1, 1 },
     { 0xB0, 0x18, 1, 1 },
     { 0xD0, 0x18, 1, 1 },
     },
    // padHitboxes (0x0225E248)
    {
     { 0x38, 0x67, 0x08, 0x37 },
     { 0x38, 0x67, 0x38, 0x67 },
     { 0x38, 0x67, 0x68, 0x97 },
     { 0x38, 0x67, 0x98, 0xC7 },
     { 0x38, 0x67, 0xC8, 0xF7 },
     { 0x68, 0x97, 0x08, 0x37 },
     { 0x68, 0x97, 0x38, 0x67 },
     { 0x68, 0x97, 0x68, 0x97 },
     { 0x68, 0x97, 0x98, 0xC7 },
     { 0x68, 0x97, 0xC8, 0xF7 },
     { 0xA0, 0xBF, 0x00, 0x4F },
     { 0xA0, 0xBF, 0x58, 0xA7 },
     { 0xA0, 0xBF, 0xB0, 0xFF },
     { TOUCHSCREEN_RECTLIST_END, 0, 0, 0 },
     },
    // padDpadBoxes (0x0225E280)
    {
     { 0x20, 0x50, 0, 0, 0x00, 0x05, 0x04, 0x01 },
     { 0x50, 0x50, 0, 0, 0x01, 0x06, 0x00, 0x02 },
     { 0x80, 0x50, 0, 0, 0x02, 0x07, 0x01, 0x03 },
     { 0xB0, 0x50, 0, 0, 0x03, 0x08, 0x02, 0x04 },
     { 0xE0, 0x50, 0, 0, 0x04, 0x09, 0x03, 0x00 },
     { 0x20, 0x80, 0, 0, 0x00, 0x8A, 0x09, 0x06 },
     { 0x50, 0x80, 0, 0, 0x01, 0x8A, 0x05, 0x07 },
     { 0x80, 0x80, 0, 0, 0x02, 0x8B, 0x06, 0x08 },
     { 0xB0, 0x80, 0, 0, 0x03, 0x8C, 0x07, 0x09 },
     { 0xE0, 0x80, 0, 0, 0x04, 0x8C, 0x08, 0x05 },
     { 0x28, 0xB0, 0, 0, 0x85, 0x0A, 0x0C, 0x0B },
     { 0x80, 0xB0, 0, 0, 0x87, 0x0B, 0x0A, 0x0C },
     { 0xD8, 0xB0, 0, 0, 0x89, 0x0C, 0x0B, 0x0A },
     },
    // xOffsets2 (0x0225E2E8)
    { 0x70, 0x90, 0, 0 },
    // xOffsets3 (0x0225E2EC)
    { 0x60, 0x80, 0xA0, 0 },
    // xOffsets4 (0x0225E2F0)
    { 0x50, 0x70, 0x90, 0xB0 },
    // xOffsets5 (0x0225E2F4)
    { 0x40, 0x60, 0x80, 0xA0, 0xC0, 0, 0, 0 },
    // xOffsets6 (0x0225E2FC)
    { 0x30, 0x50, 0x70, 0x90, 0xB0, 0xD0, 0, 0 },
};

SysTask *ov29_0225D520(BgConfig *bgConfig, void *a1, void *a2, NumberEntryArgs *args) {
    SysTask *task;
    NumberEntryData *data;
    NARC *narc;
    u32 i;
    u32 max;
    u32 threshold;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);
    G2S_BlendNone();
    task = CreateSysTaskAndEnvironment(ov29_0225DCD0, sizeof(NumberEntryData), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->bgConfig = bgConfig;
    data->unk4 = a1;
    data->unk8 = a2;
    data->unkC = args;
    data->task = task;
    data->args = args;
    args->result = -1;

    threshold = 100000;
    data->numDigits = 0;
    max = data->args->max;
    for (i = 0; i < NUMBER_ENTRY_MAX_DIGITS; i++) {
        if (max >= threshold) {
            data->numDigits = NUMBER_ENTRY_MAX_DIGITS - i;
            break;
        }
        threshold /= 10;
    }
    if (data->numDigits == 0) {
        data->numDigits = 1;
    }

    narc = NARC_New(NARC_a_2_3_6, HEAP_ID_8);
    ov29_0225D620();
    ov29_0225D648(bgConfig);
    ov29_0225D6C8(data, narc);
    ov29_0225D714(data);
    ov29_0225D7F0(data, narc);
    ov29_0225DBF0(data);
    NARC_Delete(narc);
    return task;
}

void ov29_0225D5EC(BgConfig *bgConfig, SysTask *task) {
    NumberEntryData *data = SysTask_GetData(task);
    ov29_0225DC34(data);
    ov29_0225D828(data);
    ov29_0225D7D4(data);
    ov29_0225D6B4(data->bgConfig);
    DestroySysTaskAndEnvironment(task);
    Heap_Destroy(HEAP_ID_8);
}

BOOL ov29_0225D61C(void *a0) {
    return TRUE;
}

static void ov29_0225D620(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
}

static void ov29_0225D648(BgConfig *bgConfig) {
    GXS_SetGraphicsMode(GX_BGMODE_0);
    {
        BgTemplate template = sRodata.bgTemplate4;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_8);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    {
        BgTemplate template = sRodata.bgTemplate5;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
    }
}

static void ov29_0225D6B4(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
}

static void ov29_0225D6C8(NumberEntryData *data, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 9, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_8);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 8, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0, 0, HEAP_ID_8);
}

static void ov29_0225D714(NumberEntryData *data) {
    MsgData *msgData;
    String *string;
    u16 i;
    u16 center;
    u16 x;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0196_bin, HEAP_ID_8);
    FontID_Alloc(4, HEAP_ID_8);
    for (i = 0; i < 3; i++) {
        AddWindow(data->bgConfig, &data->windows[i], &sRodata.windowTemplates[i]);
        FillWindowPixelBuffer(&data->windows[i], 0);
        string = NewString_ReadMsgData(msgData, msg_0196_00024 + i);
        center = GetWindowWidth(&data->windows[i]) * 8 / 2;
        x = center - FontID_String_GetWidth(4, string, 0) / 2;
        AddTextPrinterParameterizedWithColor(&data->windows[i], 4, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
        String_Delete(string);
        CopyWindowPixelsToVram_TextMode(&data->windows[i]);
        ScheduleWindowCopyToVram(&data->windows[i]);
    }
    FontID_Release(4);
    DestroyMsgData(msgData);
}

static void ov29_0225D7D4(NumberEntryData *data) {
    u32 i;
    for (i = 0; i < 3; i++) {
        RemoveWindow(&data->windows[i]);
    }
}

static void ov29_0225D7F0(NumberEntryData *data, NARC *narc) {
    data->spriteList = G2dRenderer_Init(NUMBER_ENTRY_NUM_SPRITES, &data->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&data->renderer, 0, FX32_CONST(256));
    ov29_0225D840(data);
    ov29_0225D9C8(data, narc);
    ov29_0225DC84(data);
}

static void ov29_0225D828(NumberEntryData *data) {
    ov29_0225DB38(data);
    ov29_0225D864(data);
    SpriteList_Delete(data->spriteList);
}

static void ov29_0225D840(NumberEntryData *data) {
    u32 i;
    for (i = 0; i < 4; i++) {
        data->resMans[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_8);
    }
}

static void ov29_0225D864(NumberEntryData *data) {
    u32 i;
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(data->resMans[i]);
    }
}

static void ov29_0225D880(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charId, int plttId, int cellId, int animId) {
    resObjs[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CHAR], narc, charFileId, FALSE, charId, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_8);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resObjs[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(resObjs[GF_GFX_RES_TYPE_CHAR]);

    resObjs[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_PLTT], narc, plttFileId, FALSE, plttId, NNS_G2D_VRAM_TYPE_2DSUB, plttNum, HEAP_ID_8);
    SpriteTransfer_CreatePlttTransferTask(resObjs[GF_GFX_RES_TYPE_PLTT]);
    sub_0200A740(resObjs[GF_GFX_RES_TYPE_PLTT]);

    resObjs[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CELL], narc, cellFileId, FALSE, cellId, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    resObjs[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_ANIM], narc, animFileId, FALSE, animId, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
}

static void ov29_0225D910(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority) {
    int charId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_CHAR]);
    int plttId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_PLTT]);
    int cellId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_CELL]);
    int animId = GF2DGfxResObj_GetResID(resObjs[GF_GFX_RES_TYPE_ANIM]);
    CreateSpriteResourcesHeader(header, charId, plttId, cellId, animId, -1, -1, 0, priority, resMans[GF_GFX_RES_TYPE_CHAR], resMans[GF_GFX_RES_TYPE_PLTT], resMans[GF_GFX_RES_TYPE_CELL], resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

static void ov29_0225D970(NumberEntryData *data, int index, SpriteResourcesHeader *header, const u8 *spec) {
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

static void ov29_0225D9C8(NumberEntryData *data, NARC *narc) {
    SpriteResourcesHeader header;
    u32 i;

    ov29_0225D880(data->padResObjs, data->resMans, narc, 5, 4, 6, 7, 2, 0x399, 0x399, 0x399, 0x399);
    ov29_0225D910(data->padResObjs, data->resMans, &header, 1);
    ov29_0225D970(data, 0, &header, sRodata.spriteSpecs[0]);
    ov29_0225D970(data, 1, &header, sRodata.spriteSpecs[1]);
    ov29_0225D970(data, 2, &header, sRodata.spriteSpecs[2]);
    ov29_0225D970(data, 3, &header, sRodata.spriteSpecs[3]);
    ov29_0225D970(data, 4, &header, sRodata.spriteSpecs[4]);

    ov29_0225D880(data->digitResObjs, data->resMans, narc, 1, 0, 2, 3, 2, 0x39A, 0x39A, 0x39A, 0x39A);
    ov29_0225D910(data->digitResObjs, data->resMans, &header, 1);
    ov29_0225D970(data, 5, &header, sRodata.spriteSpecs[5]);
    ov29_0225D970(data, 6, &header, sRodata.spriteSpecs[6]);
    ov29_0225D970(data, 7, &header, sRodata.spriteSpecs[7]);
    ov29_0225D970(data, 8, &header, sRodata.spriteSpecs[8]);
    ov29_0225D970(data, 9, &header, sRodata.spriteSpecs[9]);
    ov29_0225D970(data, 10, &header, sRodata.spriteSpecs[10]);

    for (i = 0; i < NUMBER_ENTRY_MAX_DIGITS; i++) {
        if (i < data->numDigits) {
            Sprite_SetDrawFlag(data->digitSprites[i], TRUE);
        } else {
            Sprite_SetDrawFlag(data->digitSprites[i], FALSE);
        }
    }
}

static void ov29_0225DB38(NumberEntryData *data) {
    u32 i;
    for (i = 0; i < NUMBER_ENTRY_NUM_SPRITES; i++) {
        Sprite_Delete(data->sprites[i]);
    }
    SpriteTransfer_DeleteCharTransferTask(data->digitResObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(data->digitResObjs[GF_GFX_RES_TYPE_PLTT]);
    SpriteTransfer_DeleteCharTransferTask(data->padResObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(data->padResObjs[GF_GFX_RES_TYPE_PLTT]);
}

static void ov29_0225DB7C(NumberEntryData *data) {
    u32 i;
    for (i = 0; i < NUMBER_ENTRY_NUM_SPRITES; i++) {
        Sprite_UpdateAnim(data->sprites[i], FX32_ONE);
    }
}

static void ov29_0225DB9C(NumberEntryData *data, u32 pos) {
    const DpadMenuBox *box;
    VecFx32 vec;

    box = GridInputHandler_GetDpadBox(data->gridInput, pos);
    vec.x = box->left << FX32_SHIFT;
    vec.y = (box->top << FX32_SHIFT) + FX32_CONST(256);
    Sprite_SetMatrix(data->sprites[NUMBER_ENTRY_SPRITE_CURSOR], &vec);
    if (pos <= 9) {
        Sprite_SetAnimCtrlSeq(data->sprites[NUMBER_ENTRY_SPRITE_CURSOR], 1);
    } else {
        Sprite_SetAnimCtrlSeq(data->sprites[NUMBER_ENTRY_SPRITE_CURSOR], 2);
    }
}

static void ov29_0225DBF0(NumberEntryData *data) {
    data->gridInput = GridInputHandler_Create(sRodata.padHitboxes, sRodata.padDpadBoxes, &sRodata.gridCallbacks, data, TRUE, 0, HEAP_ID_8);
    ov29_0225DB9C(data, 0);
    ov29_0225DC50(data);
}

static void ov29_0225DC34(NumberEntryData *data) {
    GridInputHandler_Free(data->gridInput);
}

static void ov29_0225DC44(void *data, int newTarget, int prevTarget) {
}

static void ov29_0225DC48(void *data, int newTarget, int prevTarget) {
    ov29_0225DB9C(data, newTarget);
}

static void ov29_0225DC50(NumberEntryData *data) {
    VecFx32 vec;

    vec.x = sRodata.xOffsets[data->numDigits][data->cursor] << FX32_SHIFT;
    vec.y = FX32_CONST(292);
    Sprite_SetMatrix(data->sprites[NUMBER_ENTRY_SPRITE_UNDERLINE], &vec);
}

static void ov29_0225DC84(NumberEntryData *data) {
    u32 i;
    const u8 *xOffsets;
    VecFx32 vec;

    xOffsets = sRodata.xOffsets[data->numDigits];
    vec.y = FX32_CONST(280);
    for (i = 0; i < data->numDigits; i++) {
        vec.x = xOffsets[i] << FX32_SHIFT;
        Sprite_SetMatrix(data->digitSprites[i], &vec);
    }
}

static void ov29_0225DCD0(SysTask *task, void *taskData) {
    NumberEntryData *data = taskData;
    u32 input;

    switch (data->state) {
    case NUMBER_ENTRY_STATE_INPUT:
        input = TouchscreenHitbox_FindRectAtTouchNew(sRodata.hitboxes[data->numDigits]);
        if (input != TOUCH_MENU_NO_INPUT) {
            data->cursor = input;
            ov29_0225DC50(data);
            ov29_0225E028(data, input + NUMBER_ENTRY_SPRITE_DIGIT_0, data->digits[data->cursor] + 12, data->digits[data->cursor] + 1, NUMBER_ENTRY_STATE_INPUT);
            PlaySE(SEQ_SE_DP_DECIDE);
        } else if (gSystem.newKeys & PAD_BUTTON_START) {
            GridInputHandler_SetNextInput(data->gridInput, NUMBER_ENTRY_INPUT_CONFIRM);
            ov29_0225DB9C(data, NUMBER_ENTRY_INPUT_CONFIRM);
            PlaySE(SEQ_SE_DP_DECIDE);
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            if (ov29_0225DEB8(data, -1) == TRUE) {
                PlaySE(SEQ_SE_DP_DECIDE);
            }
        } else {
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
            case 8:
            case 9:
                ov29_0225DEF4(data, input);
                ov29_0225DEB8(data, 1);
                ov29_0225E078(data, input % 5 * 6 + 1, input / 5 * 6 + 7, NUMBER_ENTRY_STATE_INPUT);
                PlaySE(SEQ_SE_DP_DECIDE);
                break;
            case NUMBER_ENTRY_INPUT_CLEAR:
                ov29_0225E028(data, 2, 4, 3, NUMBER_ENTRY_STATE_INPUT);
                ov29_0225DEB8(data, -1);
                PlaySE(SEQ_SE_DP_DECIDE);
                break;
            case NUMBER_ENTRY_INPUT_CONFIRM:
                ov29_0225E0E0(data);
                if (data->value > data->args->max) {
                    ov29_0225E028(data, 3, 4, 3, NUMBER_ENTRY_STATE_INPUT);
                    PlaySE(SEQ_SE_DP_DECIDE);
                } else {
                    data->args->result = data->value;
                    ov29_0225E028(data, 3, 4, 3, NUMBER_ENTRY_STATE_DONE);
                    PlaySE(SEQ_SE_DP_DECIDE);
                }
                break;
            case NUMBER_ENTRY_INPUT_CANCEL:
                data->args->result = 0;
                ov29_0225E028(data, 4, 4, 3, NUMBER_ENTRY_STATE_DONE);
                PlaySE(SEQ_SE_DP_DECIDE);
                break;
            }
        }
        break;
    case NUMBER_ENTRY_STATE_BLINK:
        if (!ov29_0225DF74(data)) {
            data->state = data->nextState;
        }
        break;
    case NUMBER_ENTRY_STATE_DONE:
        break;
    }

    ov29_0225DB7C(data);
    SpriteList_RenderAndAnimateSprites(data->spriteList);
}

static BOOL ov29_0225DEB8(NumberEntryData *data, int delta) {
    int prev = data->cursor;

    data->cursor += delta;
    if (data->cursor < 0) {
        data->cursor = 0;
    } else if (data->cursor >= data->numDigits) {
        data->cursor = data->numDigits - 1;
    }
    if (data->cursor == prev) {
        return FALSE;
    }
    ov29_0225DC50(data);
    return TRUE;
}

static void ov29_0225DEF4(NumberEntryData *data, u32 digit) {
    data->digits[data->cursor] = digit;
    Sprite_SetAnimCtrlSeq(data->digitSprites[data->cursor], digit + 1);
}

static void ov29_0225DF18(NumberEntryData *data, u8 seq) {
    if (!data->blink.isBg) {
        Sprite_SetAnimCtrlSeq(data->sprites[data->blink.target], seq);
    } else {
        BgTilemapRectChangePalette(data->bgConfig, data->blink.target, data->blink.x, data->blink.y, data->blink.width, data->blink.height, seq);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, data->blink.target);
    }
}

static BOOL ov29_0225DF74(NumberEntryData *data) {
    NumberEntryBlink *blink = &data->blink;

    switch (blink->phase) {
    case 0:
        ov29_0225DF18(data, blink->seqA);
        blink->phase++;
        break;
    case 1:
        blink->timer++;
        if (blink->timer == 4) {
            ov29_0225DF18(data, blink->seqB);
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

static void ov29_0225E028(NumberEntryData *data, u8 target, u8 seqA, u8 seqB, u16 nextState) {
    data->blink.isBg = FALSE;
    data->blink.timer = 0;
    data->blink.phase = 0;
    data->blink.target = target;
    data->blink.seqA = seqA;
    data->blink.seqB = seqB;
    data->nextState = nextState;
    data->state = NUMBER_ENTRY_STATE_BLINK;
}

static void ov29_0225E078(NumberEntryData *data, u8 x, u8 y, u16 nextState) {
    data->blink.isBg = TRUE;
    data->blink.timer = 0;
    data->blink.phase = 0;
    data->blink.target = GF_BG_LYR_SUB_1;
    data->blink.seqA = 1;
    data->blink.seqB = 0;
    data->blink.x = x;
    data->blink.y = y;
    data->blink.width = 6;
    data->blink.height = 6;
    data->nextState = nextState;
    data->state = NUMBER_ENTRY_STATE_BLINK;
}

static void ov29_0225E0E0(NumberEntryData *data) {
    u32 value = 0;
    u32 i;

    for (i = 0; i < data->numDigits; i++) {
        value = value * 10 + data->digits[i];
    }
    data->value = value;
}
