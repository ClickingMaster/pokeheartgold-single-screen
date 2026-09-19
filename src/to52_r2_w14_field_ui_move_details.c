#include "to52_field_ui_move_details_private.h"

void sub_0208D9A0(SummaryState *state, int move) {
    MsgData *msg;
    u32 attr;

    ScheduleWindowCopyToVram((Window *)&state->raw[0x184]);
    ScheduleWindowCopyToVram((Window *)&state->raw[0x194]);
    ScheduleWindowCopyToVram((Window *)&state->raw[0x1a4]);
    FillWindowPixelBuffer((Window *)((u8 *)PTR(state, 0x224, Window) + 0xd0), 0);
    FillWindowPixelBuffer((Window *)((u8 *)PTR(state, 0x224, Window) + 0xe0), 0);
    FillWindowPixelBuffer((Window *)((u8 *)PTR(state, 0x224, Window) + 0xf0), 0);
    attr = GetMoveAttr(move, 2);
    if (attr <= 1) {
        ReadMsgDataIntoString(PTR(state, 0x7a0, MsgData), 0x9a, PTR(state, 0x7ac, String));
    } else {
        sub_0208C87C(state, 0x96, attr, 3, 0);
    }
    sub_0208C778(state, (Window *)((u8 *)PTR(state, 0x224, Window) + 0xd0), 0x10200, 1);
    attr = GetMoveAttr(move, 4);
    if (attr == 0) {
        ReadMsgDataIntoString(PTR(state, 0x7a0, MsgData), 0x9a, PTR(state, 0x7ac, String));
    } else {
        sub_0208C87C(state, 0x97, attr, 3, 0);
    }
    sub_0208C778(state, (Window *)((u8 *)PTR(state, 0x224, Window) + 0xe0), 0x10200, 1);
    msg = NewMsgDataFromNarc(1, 0x1b, 0x2ed, 0x13);
    ReadMsgDataIntoString(msg, move, PTR(state, 0x7ac, String));
    sub_0208C778(state, (Window *)((u8 *)PTR(state, 0x224, Window) + 0xf0), 0x10200, 0);
    DestroyMsgData(msg);
    ScheduleWindowCopyToVram((Window *)((u8 *)PTR(state, 0x224, Window) + 0xd0));
    ScheduleWindowCopyToVram((Window *)((u8 *)PTR(state, 0x224, Window) + 0xe0));
    ScheduleWindowCopyToVram((Window *)((u8 *)PTR(state, 0x224, Window) + 0xf0));
}
