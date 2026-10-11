#include "comm_club_buttons.h"

#include "global.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

// Field bottom-screen app 7: one or two big touch buttons standing in for A/B in the Communication Club.

typedef struct CommClubButtonsLayout {
    const WindowTemplate *windows;
    const TouchscreenHitbox *hitboxes;
    u8 x;
    u8 y;
    u16 spacing;
} CommClubButtonsLayout;

typedef struct CommClubButtonsData {
    BgConfig *bgConfig;
    void *unk_04;
    void *unk_08;
    CommClubButtonsArgs *unk_0C;
    CommClubButtonsArgs *args;
    SysTask *task;
    Window windows[2];
    u16 tilemap[0xC0]; // one button, 32x6 tiles
    u16 state;
    u16 count;
    s32 pressedButton;
} CommClubButtonsData; // size: 0x1C0

void CommClubButtons_Task(SysTask *task, void *taskData);
void CommClubButtons_SetVramBanks(void);
void CommClubButtons_InitBgs(BgConfig *bgConfig);
static void CommClubButtons_FreeBgs(BgConfig *bgConfig);
void CommClubButtons_AddWindows(CommClubButtonsData *data);
static void CommClubButtons_RemoveWindows(CommClubButtonsData *data);
void CommClubButtons_LoadGraphics(CommClubButtonsData *data);
void CommClubButtons_DrawButtons(CommClubButtonsData *data);
void CommClubButtons_SetButtonPalette(CommClubButtonsData *data, u8 palette);

const TouchscreenHitbox sCommClubButtonsOneButtonHitboxes[] = {
    { 0x50, 0x7F, 0x00, 0xFF },
    { 0xFF, 0x00, 0x00, 0x00 },
};

const WindowTemplate sCommClubButtonsOneButtonWindowTemplates[] = {
    { 4, 1, 12, 30, 2, 4, 1 },
};

const TouchscreenHitbox sCommClubButtonsTwoButtonHitboxes[] = {
    { 0x28, 0x57, 0x00, 0xFF },
    { 0x60, 0x8F, 0x00, 0xFF },
    { 0xFF, 0x00, 0x00, 0x00 },
};

const WindowTemplate sCommClubButtonsTwoButtonWindowTemplates[] = {
    { 4, 1, 7,  30, 2, 4, 1    },
    { 4, 1, 14, 30, 2, 4, 0x3D },
};

// Indexed by button count - 1.
const CommClubButtonsLayout sCommClubButtonsLayouts[] = {
    { sCommClubButtonsOneButtonWindowTemplates, sCommClubButtonsOneButtonHitboxes, 0, 10, 0 },
    { sCommClubButtonsTwoButtonWindowTemplates, sCommClubButtonsTwoButtonHitboxes, 0, 5,  1 },
};

SysTask *CommClubButtons_Init(BgConfig *bgConfig, void *a1, void *a2, CommClubButtonsArgs *args) {
    SysTask *task;
    CommClubButtonsData *data;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);
    reg_G2S_DB_BLDCNT = 0;
    task = CreateSysTaskAndEnvironment(CommClubButtons_Task, sizeof(CommClubButtonsData), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->bgConfig = bgConfig;
    data->unk_04 = a1;
    data->unk_08 = a2;
    data->unk_0C = args;
    data->task = task;
    data->args = args;
    data->count = data->args->count;
    data->args->selection = 0xFFFF;
    data->args->closeRequest = 0;
    CommClubButtons_SetVramBanks();
    CommClubButtons_InitBgs(bgConfig);
    CommClubButtons_AddWindows(data);
    CommClubButtons_LoadGraphics(data);
    CommClubButtons_DrawButtons(data);
    return task;
}

void CommClubButtons_Exit(void *a0, SysTask *task) {
    CommClubButtonsData *data = SysTask_GetData(task);
    CommClubButtons_RemoveWindows(data);
    CommClubButtons_FreeBgs(data->bgConfig);
    DestroySysTaskAndEnvironment(task);
    Heap_Destroy(HEAP_ID_8);
}

BOOL CommClubButtons_IsExitDone(void) {
    return TRUE;
}

void CommClubButtons_Task(SysTask *task, void *taskData) {
    CommClubButtonsData *data = taskData;
    const CommClubButtonsLayout *layout = &sCommClubButtonsLayouts[data->count - 1];

    if (data->state < 2) {
        if (data->args->closeRequest == 1) {
            data->state = 2;
        } else if (data->args->closeRequest == 2) {
            data->state = 4;
        }
    }

    switch (data->state) {
    case 0:
        data->pressedButton = TouchscreenHitbox_FindRectAtTouchNew(layout->hitboxes);
        if (data->pressedButton != -1) {
            CommClubButtons_SetButtonPalette(data, 1);
            data->args->selection = data->pressedButton;
            data->state = 1;
        }
        break;
    case 1: {
        int touch = TouchscreenHitbox_FindRectAtTouchHeld(layout->hitboxes);
        data->args->selection = 0xFFFF;
        if (touch != data->pressedButton) {
            CommClubButtons_SetButtonPalette(data, 0);
            data->state = 0;
        }
        break;
    }
    case 2:
        BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_0);
        BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_1);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_0);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_1);
        data->state = 3;
        break;
    case 3:
        break;
    case 4:
        BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_0);
        BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_1);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_0);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_1);
        data->state = 5;
        break;
    case 5:
        if (gSystem.touchHeld) {
            gSystem.simulatedInputs = 1;
        }
        break;
    }
}

void CommClubButtons_SetVramBanks(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & 0xFFCFFFEF) | 0x10;
}

void CommClubButtons_InitBgs(BgConfig *bgConfig) {
    GXS_SetGraphicsMode(GX_BGMODE_0);

    {
        BgTemplate template = {
            0, 0, 0x800, 0, 1, 0, 15, 0, 0, 0, 0, 0, 0
        };
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_8);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }

    {
        BgTemplate template = {
            0, 0, 0x800, 0, 1, 0, 14, 1, 0, 1, 0, 0, 0
        };
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, 0);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    }

    {
        BgTemplate template = {
            0, 0, 0x800, 0, 1, 0, 13, 1, 0, 2, 0, 0, 0
        };
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &template, 0);
    }
}

static void CommClubButtons_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
}

void CommClubButtons_AddWindows(CommClubButtonsData *data) {
    u32 i;
    const CommClubButtonsLayout *layout = &sCommClubButtonsLayouts[data->count - 1];

    for (i = 0; i < data->count; i++) {
        AddWindow(data->bgConfig, &data->windows[i], &layout->windows[i]);
        FillWindowPixelBuffer(&data->windows[i], 0);
    }
}

static void CommClubButtons_RemoveWindows(CommClubButtonsData *data) {
    u32 i;

    for (i = 0; i < data->count; i++) {
        RemoveWindow(&data->windows[i]);
    }
}

void CommClubButtons_LoadGraphics(CommClubButtonsData *data) {
    NNSG2dScreenData *scrnData;
    void *raw;
    NARC *narc = NARC_New(NARC_a_2_3_7, HEAP_ID_8);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0xA0, HEAP_ID_8);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, data->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_8);
    raw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 10, FALSE, &scrnData, HEAP_ID_8);
    MIi_CpuCopy16((u16 *)((u8 *)scrnData->rawData + 0x180), data->tilemap, 0x180);
    Heap_Free(raw);
    NARC_Delete(narc);
}

void CommClubButtons_DrawButtons(CommClubButtonsData *data) {
    u16 i;
    String *str;
    const CommClubButtonsLayout *layout;
    MsgData *msgData;
    u16 halfWidth;
    u16 x;

    FontID_Alloc(4, HEAP_ID_8);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0191_bin, HEAP_ID_8);
    layout = &sCommClubButtonsLayouts[data->count - 1];

    for (i = 0; i < data->count; i++) {
        LoadRectToBgTilemapRect(data->bgConfig, GF_BG_LYR_SUB_1, data->tilemap, layout->x, (u8)(layout->y + (layout->spacing + 6) * i), 0x20, 6);
        str = NewString_ReadMsgData(msgData, data->args->msgIds[i]);
        halfWidth = GetWindowWidth(&data->windows[i]) * 8 / 2;
        x = halfWidth - FontID_String_GetWidth(4, str, 0) / 2;
        AddTextPrinterParameterizedWithColor(&data->windows[i], 4, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(2, 1, 0), NULL);
        CopyWindowPixelsToVram_TextMode(&data->windows[i]);
        ScheduleWindowCopyToVram(&data->windows[i]);
        String_Delete(str);
    }

    DestroyMsgData(msgData);
    FontID_Release(4);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_1);
}

void CommClubButtons_SetButtonPalette(CommClubButtonsData *data, u8 palette) {
    const CommClubButtonsLayout *layout = &sCommClubButtonsLayouts[data->count - 1];
    BgTilemapRectChangePalette(data->bgConfig, GF_BG_LYR_SUB_1, layout->x, (u8)(layout->y + (layout->spacing + 6) * data->pressedButton), 0x20, 6, palette);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_1);
}
