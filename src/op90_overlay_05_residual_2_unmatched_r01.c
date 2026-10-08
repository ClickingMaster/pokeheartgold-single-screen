#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "text.h"
#include "list_menu_2d.h"
#include "render_window.h"
#include "system.h"
typedef struct Args05 {
    u8 unk00[0x24];
    enum HeapID heapId;
} Args05;
typedef struct Work05 {
    Args05 *args;
    u8 unk004[8];
    BgConfig *bgConfig;
    u8 unk010[0xB78];
    Window msgWindow;
    Window yesNoWindow;
    u8 unkBA8[4];
    MsgData *msgData;
    u8 unkBB0[4];
    String *string;
    u32 printerId;
    int state;
    struct ListMenu2D *menu;
    u8 unkBC4[4];
    int canAdvance;
    int unkBCC;
} Work05;
extern const WindowTemplate ov05_0221EA58;
BOOL ov05_0221E9F8(Work05 *work);
void ov05_0221E9C4(Work05 *work);
void ov05_0221BB30(Work05 *work);
// @fn ov05_0221BB30
void ov05_0221BB30(Work05 *work) {
    if (!ov05_0221E9F8(work)) {
        return;
    }
    switch (work->state) {
    case 0:
        break;
    case 1:
        ov05_0221E9C4(work);
        ReadMsgDataIntoString(work->msgData, 0, work->string);
        work->printerId = AddTextPrinterParameterized(&work->msgWindow, 1, work->string, 0, 0, 0, NULL);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_MAIN_0);
        work->state = 2;
        break;
    case 2:
        if (work->canAdvance == 1 && (gSystem.newKeys & PAD_BUTTON_A)) {
            work->state = 3;
        }
        break;
    case 3:
        if (work->unkBCC == 1) {
            ReadMsgDataIntoString(work->msgData, 2, work->string);
        } else {
            ReadMsgDataIntoString(work->msgData, 1, work->string);
        }
        FillWindowPixelRect(&work->yesNoWindow, 15, 0, 0, 216, 32);
        DrawFrameAndWindow2(&work->yesNoWindow, FALSE, 1, 15);
        work->printerId = AddTextPrinterParameterized(&work->yesNoWindow, 1, work->string, 0, 0, 0, NULL);
        work->menu = CreateYesNoMenu(work->bgConfig, &ov05_0221EA58, 31, 14, 1, work->args->heapId);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_MAIN_0);
        work->state = 4;
        break;
    case 4:
        switch ((u32)Handle2dMenuInput_DeleteOnFinish(work->menu, work->args->heapId)) {
        case 0:
            work->state = 5;
            work->menu = NULL;
            break;
        case -2:
            work->state = 1;
            work->menu = NULL;
            break;
        }
        break;
    case 5:
        ov05_0221E9C4(work);
        ReadMsgDataIntoString(work->msgData, 5, work->string);
        work->printerId = AddTextPrinterParameterized(&work->msgWindow, 1, work->string, 0, 0, 0, NULL);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_MAIN_0);
        work->state = 6;
        break;
    case 6:
        break;
    case 7:
        if (work->menu != NULL) {
            Clear2dMenuWindowAndDelete(work->menu, work->args->heapId);
            work->menu = NULL;
        }
        ClearFrameAndWindow2(&work->msgWindow, FALSE);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_MAIN_0);
        work->state = 8;
        break;
    case 8:
        break;
    }
}
