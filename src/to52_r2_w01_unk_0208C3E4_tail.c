#include "unk_0208C3E4_tail_internal.h"

void sub_0208C57C(SummaryState *state) {
    Window *window = (Window *)&state->raw[0x214];
    u32 value;
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, PTR(state, 0x234, String), 0, 0, 0xFF, 0xE0F00, 0);
    value = U8(state, 0x242);
    value <<= 24;
    value >>= 31;
    if (value == 0) {
        value = U8(state, 0x243);
        value <<= 30;
        value >>= 30;
        if (value == 0) {
            ReadMsgDataIntoString(PTR(state, 0x7A0, MsgData), 1, PTR(state, 0x7AC, String));
            sub_0208C778(state, window, 0x30400, 1);
        } else if (value == 1) {
            ReadMsgDataIntoString(PTR(state, 0x7A0, MsgData), 2, PTR(state, 0x7AC, String));
            sub_0208C778(state, window, 0x50600, 1);
        }
    }
    ScheduleWindowCopyToVram(window);
}

void sub_0208C614(SummaryState *state) {
    Window *window = (Window *)&state->raw[0x204];
    String *source;
    u32 value;
    FillWindowPixelBuffer(window, 0);
    value = U32(state, 0x280);
    value <<= 3;
    value >>= 31;
    if (value == 0) {
        sub_0200CDAC(PTR(state, 0x79C, void), 1, window, 0, 5);
        source = NewString_ReadMsgData(PTR(state, 0x7A0, MsgData), 3);
        value = BYTE7(state, 0x242).value;
        BufferIntegerAsString(PTR(state, 0x7A8, void), 0, value, 3, 0, 1);
        StringExpandPlaceholders(PTR(state, 0x7A8, void), PTR(state, 0x7AC, String), source);
        String_Delete(source);
        AddTextPrinterParameterizedWithColor(window, 0, PTR(state, 0x7AC, String), 0x10, 0, 0xFF, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(window);
}

void sub_0208C6B4(SummaryState *state) {
    String *source;
    FillWindowPixelBuffer((Window *)&state->raw[0x1F4], 0);
    if (U16(state, 0x23E) != 0) {
        BufferItemName(PTR(state, 0x7A8, void), 0, U16(state, 0x23E));
        source = NewString_ReadMsgData(PTR(state, 0x7A0, MsgData), 5);
        StringExpandPlaceholders(PTR(state, 0x7A8, void), PTR(state, 0x7AC, String), source);
        String_Delete(source);
    } else {
        ReadMsgDataIntoString(PTR(state, 0x7A0, MsgData), 6, PTR(state, 0x7AC, String));
    }
    sub_0208C778(state, (Window *)&state->raw[0x1F4], 0x10200, 0);
    ScheduleWindowCopyToVram((Window *)&state->raw[0x64]);
    ScheduleWindowCopyToVram((Window *)&state->raw[0x1F4]);
}
