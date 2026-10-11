#include "to57_overlay08_bank14_private.h"
#pragma force_active on

extern void BattlePartyMenu_PrintMonName(void *, int, int, int, int, int);
extern void BattlePartyMenu_PrintMoveName(void *, int, int, int, int, int, int);
extern void BattlePartyMenu_PrintMovePP(void *, int, int, int);
extern int GetMoveAttr(int, int);
extern const int sBattlePartyMoveNameMsgIds[];
void BattlePartyMenu_PrintContestForgetMovePage(void *);

void BattlePartyMenu_PrintContestForgetMovePage(void *arg) {
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
    BattlePartyMenu_PrintMonName(work, 0, 0, ((u8 *)work->root)[0x11], 0, 0);
    BattlePartyMenu_PrintPPLabel(work, 2, 0, 0);
    string = NewString_ReadMsgData(work->msgData, 0x39);
    AddTextPrinterParameterizedWithColor(&((Window *)work->windowArray)[4], 0, string, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(&((Window *)work->windowArray)[4]);
    if (((u8 *)work->root)[0x34] < 4) {
        u32 index = ((u8 *)work->root)[0x34];
        slot = (u8 *)work + 0x34 + ((u8 *)work->root)[0x11] * 0x50;
        selected = slot + index * 8;
        BattlePartyMenu_PrintMoveName(work, *(u16 *)selected, 1, sBattlePartyMoveNameMsgIds[index], 0, 0, 0xF0E00);
        BattlePartyMenu_PrintMovePP(work, 3, selected[2], selected[3]);
    } else {
        moveAttr = GetMoveAttr(*(u16 *)((u8 *)work->root + 0x24), 5);
        BattlePartyMenu_PrintMoveName(work, *(u16 *)((u8 *)work->root + 0x24), 1, 0x49, 0, 0, 0xF0E00);
        BattlePartyMenu_PrintMovePP(work, 3, moveAttr, moveAttr);
    }
    BattlePartyMenu_PrintForgetButtonLabel(work, 6);
}
