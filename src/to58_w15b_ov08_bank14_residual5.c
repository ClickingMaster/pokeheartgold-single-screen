#include "to57_overlay08_bank14_private.h"
#pragma force_active on

extern void ov08_0221DDCC(void *, int, int, int, int, int);
extern void ov08_0221E244(void *, int, int, int, int, int, int);
extern void ov08_0221F07C(void *, int, int, int);
extern int GetMoveAttr(int, int);
extern const int ov08_02224FE0[];
void ov08_02220084(void *);

void ov08_02220084(void *arg) {
    Ov08R2W11Work *work = arg;
    u8 *slot;
    u8 *selected;
    void *string;
    int moveAttr;

    FillWindowPixelBuffer(&((Window *)work->windowArray)[0], 0);
    FillWindowPixelBuffer(&((Window *)work->windowArray)[2], 0);
    FillWindowPixelBuffer(&((Window *)work->windowArray)[3], 0);
    FillWindowPixelBuffer(&((Window *)work->windowArray)[1], 0);
    FillWindowPixelBuffer(&((Window *)work->windowArray)[4], 0);
    FillWindowPixelBuffer(&((Window *)work->windowArray)[5], 0);
    FillWindowPixelBuffer(&((Window *)work->windowArray)[6], 0);
    ov08_0221DDCC(work, 0, 0, ((u8 *)work->root)[0x11], 0, 0);
    ov08_0221E2E8(work, 2, 0, 0);
    string = NewString_ReadMsgData(work->msgData, 0x39);
    AddTextPrinterParameterizedWithColor(&((Window *)work->windowArray)[4], 0, string, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(&((Window *)work->windowArray)[4]);
    if (((u8 *)work->root)[0x34] < 4) {
        u32 index = ((u8 *)work->root)[0x34];
        slot = (u8 *)work + 0x34 + ((u8 *)work->root)[0x11] * 0x50;
        selected = slot + index * 8;
        ov08_0221E244(work, *(u16 *)selected, 1, ov08_02224FE0[index], 0, 0, 0xF0E00);
        ov08_0221F07C(work, 3, selected[2], selected[3]);
    } else {
        moveAttr = GetMoveAttr(*(u16 *)((u8 *)work->root + 0x24), 5);
        ov08_0221E244(work, *(u16 *)((u8 *)work->root + 0x24), 1, 0x49, 0, 0, 0xF0E00);
        ov08_0221F07C(work, 3, moveAttr, moveAttr);
    }
    ov08_0221F1B0(work, 6);
}
