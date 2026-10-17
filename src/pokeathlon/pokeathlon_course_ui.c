#include "pokeathlon/pokeathlon_course_ui.h"

#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_course_ceremony.h"

#include "assert.h"
#include "char_tiles.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "options.h"
#include "player_data.h"
#include "pokemon.h"

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
u8 TextPrinterCheckActive(u8 printerId);
void RemoveTextPrinter(u8 printerId);
void ResetAllTextPrinters(void);

static void PokeathlonEventBanner_PrintText(Window *window, MsgData *msgData, String *string, u32 msgId, u32 font);
static void PokeathlonCourseMessageBox_LoadFrame(BgConfig *bgConfig, int layer, enum HeapID heapId);
static u8 PokeathlonCourseMessageBox_PrintToWindow(BgConfig *bgConfig, Window *window, String **string, u32 textSpeed, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, u8 bgLayer, enum HeapID heapId);

// The window templates of the event banner (the first field is not read).
typedef struct PokeathlonEventBannerWindows {
    u16 unused;
    WindowTemplate templates[2];
} PokeathlonEventBannerWindows;

static const PokeathlonEventBannerWindows sPokeathlonEventBannerWindows = {
    .unused = 7,
    .templates = {
                  {
            .bgId = GF_BG_LYR_SUB_2,
            .left = 7,
            .top = 8,
            .width = 18,
            .height = 4,
            .palette = 0,
            .baseTile = 1,
        },
                  {
            .bgId = GF_BG_LYR_SUB_2,
            .left = 4,
            .top = 13,
            .width = 24,
            .height = 2,
            .palette = 0,
            .baseTile = 0x49,
        },
                  },
};

BOOL PokeathlonCourseCeremony_IsOpeningFinished(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyOpeningData *data = (PokeathlonCourseCeremonyOpeningData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->allFinished;
}

BOOL PokeathlonCourseCeremony_IsClosingFinished(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->allFinished;
}

PokeathlonEventBanner *PokeathlonEventBanner_New(u32 event, enum HeapID heapId) {
    PokeathlonEventBanner *banner = Heap_Alloc(heapId, sizeof(PokeathlonEventBanner));
    MI_CpuFill8(banner, 0, sizeof(PokeathlonEventBanner));
    banner->heapId = heapId;
    banner->event = event;
    banner->narc = NARC_New(NARC_a_2_1_9, heapId);
    return banner;
}

void PokeathlonEventBanner_Free(PokeathlonEventBanner *banner) {
    int i = 0;
    Window *window = &banner->nameWindow;
    while (i < 2) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        i++;
        window++;
    }
    NARC_Delete(banner->narc);
    Heap_Free(banner);
}

void PokeathlonEventBanner_AddWindows(PokeathlonEventBanner *banner, BgConfig *bgConfig) {
    const WindowTemplate *template = sPokeathlonEventBannerWindows.templates;
    Window *window;
    int i;
    banner->bgConfig = bgConfig;
    i = 0;
    window = &banner->nameWindow;
    while (i < 2) {
        AddWindow(banner->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        ClearWindowTilemap(window);
        i++;
        template ++;
        window++;
    }
}

void PokeathlonEventBanner_LoadGraphics(PokeathlonEventBanner *banner) {
    u8 event = banner->event;
    GfGfxLoader_GXLoadPalFromOpenNarc(banner->narc, 11, GF_PAL_LOCATION_SUB_BGEXT, (enum GFPalSlotOffset)(6 << 12), 0, banner->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(banner->narc, event + 12, banner->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, banner->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(banner->narc, event + 22, banner->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, banner->heapId);
}

void PokeathlonEventBanner_PrintTexts(PokeathlonEventBanner *banner) {
    u8 event = banner->event;
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, banner->heapId);
    String *string = String_New(256, banner->heapId);
    PokeathlonEventBanner_PrintText(&banner->nameWindow, msgData, string, event + msg_0309_00166, 4);
    PokeathlonEventBanner_PrintText(&banner->placeWindow, msgData, string, event + msg_0309_00187, 0);
    String_Delete(string);
    DestroyMsgData(msgData);
}

static void PokeathlonEventBanner_PrintText(Window *window, MsgData *msgData, String *string, u32 msgId, u32 font) {
    ReadMsgDataIntoString(msgData, msgId, string);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, font, string, 0, 0, 0xFF, 0x10200, NULL); // no transfer; colours 1, 2, 0
    ScheduleWindowCopyToVram(window);
}

PokeathlonCourseMessageBox *PokeathlonCourseMessageBox_New(enum HeapID heapId) {
    PokeathlonCourseMessageBox *box = Heap_Alloc(heapId, sizeof(PokeathlonCourseMessageBox));
    MI_CpuFill8(box, 0, sizeof(PokeathlonCourseMessageBox));
    box->heapId = heapId;
    return box;
}

void PokeathlonCourseMessageBox_Init(PokeathlonCourseMessageBox *box, BgConfig *bgConfig, u32 bgLayer, u32 linkMode, SaveData *saveData) {
    u32 textSpeed = 4;
    if (linkMode == 0) {
        textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
    }
    box->bgConfig = bgConfig;
    box->printerId = -1;
    box->bgLayer = bgLayer;
    box->linkMode = linkMode;
    box->textSpeed = textSpeed;
    box->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, box->heapId);
    box->npcMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0311_bin, box->heapId);
    box->msgFormat = MessageFormat_New(box->heapId);
    ResetAllTextPrinters();
    PokeathlonCourseMessageBox_LoadFrame(box->bgConfig, (u8)box->bgLayer, box->heapId);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)480, box->heapId);
    BG_FillCharDataRange(box->bgConfig, (GFBgLayer)box->bgLayer, 0, 1, 0);
}

void PokeathlonCourseMessageBox_Free(PokeathlonCourseMessageBox *box) {
    DestroyMsgData(box->msgData);
    DestroyMsgData(box->npcMsgData);
    MessageFormat_Delete(box->msgFormat);
    if (box->string != NULL) {
        String_Delete(box->string);
    }
    Heap_Free(box);
}

// Returns whether the box is still busy (printing, or holding a printed message in link mode).
BOOL PokeathlonCourseMessageBox_Update(PokeathlonCourseMessageBox *box) {
    if (box->printerId != -1) {
        if (!TextPrinterCheckActive((u8)box->printerId)) {
            box->holdTimer = 20;
            String_Delete(box->string);
            box->string = NULL;
            box->printerId = -1;
        }
        box->busy = TRUE;
    } else {
        GF_ASSERT(box->printerId == -1);
        if (box->holdTimer != 0) {
            if (box->linkMode == 0) {
                box->busy = FALSE;
            } else {
                box->holdTimer--;
                box->busy = box->holdTimer != 0;
            }
        } else {
            box->busy = FALSE;
        }
    }
    return box->busy;
}

void PokeathlonCourseMessageBox_Print(PokeathlonCourseMessageBox *box, u32 msgId) {
    box->printerId = PokeathlonCourseMessageBox_PrintToWindow(box->bgConfig, &box->window, &box->string, box->textSpeed, box->msgFormat, box->msgData, msgId, (u8)box->bgLayer, box->heapId);
}

void PokeathlonCourseMessageBox_PrintNpcLine(PokeathlonCourseMessageBox *box, u32 msgId) {
    box->printerId = PokeathlonCourseMessageBox_PrintToWindow(box->bgConfig, &box->window, &box->string, box->textSpeed, box->msgFormat, box->npcMsgData, msgId, (u8)box->bgLayer, box->heapId);
}

void PokeathlonCourseMessageBox_Close(PokeathlonCourseMessageBox *box) {
    if (box->printerId != -1) {
        RemoveTextPrinter((u8)box->printerId);
    }
    if (box->window.bgConfig != NULL) {
        ClearFrameAndWindow2(&box->window, FALSE);
        ClearWindowTilemapAndCopyToVram(&box->window);
        RemoveWindow(&box->window);
    }
}

MessageFormat *PokeathlonCourseMessageBox_GetMsgFormat(PokeathlonCourseMessageBox *box) {
    if (box == NULL) {
        GF_AssertFail();
    }
    if (box->msgFormat == NULL) {
        GF_AssertFail();
    }
    return box->msgFormat;
}

void PokeathlonCourseMessageBox_ShowWaitingIcon(PokeathlonCourseMessageBox *box) {
    if (box == NULL) {
        GF_AssertFail();
    }
    if (box->waitingIcon != NULL) {
        GF_AssertFail();
    }
    box->waitingIcon = WaitingIcon_New(&box->window, 0x3D2);
}

void PokeathlonCourseMessageBox_HideWaitingIcon(PokeathlonCourseMessageBox *box) {
    if (box == NULL) {
        GF_AssertFail();
    }
    if (box->waitingIcon == NULL) {
        GF_AssertFail();
    }
    WaitingIcon_Delete(box->waitingIcon);
}

static void PokeathlonCourseMessageBox_LoadFrame(BgConfig *bgConfig, int layer, enum HeapID heapId) {
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1C0, heapId);
    LoadUserFrameGfx2(bgConfig, (GFBgLayer)layer, 0x3D2, 13, 0, heapId);
}

static u8 PokeathlonCourseMessageBox_PrintToWindow(BgConfig *bgConfig, Window *window, String **string, u32 textSpeed, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, u8 bgLayer, enum HeapID heapId) {
    u8 printerId;
    if (window->bgConfig == NULL) {
        AddWindowParameterized(bgConfig, window, bgLayer, 2, 19, 27, 4, 15, 1);
    }
    FillWindowPixelBuffer(window, 15);
    *string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, heapId);
    printerId = AddTextPrinterParameterizedWithColor(window, 1, *string, 0, 0, textSpeed, 0x1020F, NULL); // colours 1, 2, 15
    DrawFrameAndWindow2(window, FALSE, 0x3D2, 13);
    return printerId;
}

BOOL PokeathlonCourseMessageBox_IsBusy(PokeathlonCourseMessageBox *box) {
    return box->busy;
}
