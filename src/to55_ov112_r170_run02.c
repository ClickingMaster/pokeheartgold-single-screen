#include "to55_ov112_r170_private.h"

void ov112_021F1504(void *p, u32 msg) {
    void *str;
    u16 len;
    u16 buf[20];
    if (msg > 0x218) {
        msg = 0x218;
    }
    str = NewString_ReadMsgData(PTR_AT(p, 0x64), msg);
    len = String_GetLength(str) + 1;
    CopyStringToU16Array(str, buf, 0x13);
    ov112_021F1488(p, buf, 4, len);
    String_Delete(str);
}

void ov112_021F1548(void *p, int field, int msg) {
    void *str = NewString_ReadMsgData(PTR_AT(p, 0x60), msg);
    u16 len = String_GetLength(str) + 1;
    u16 buf[12];
    CopyStringToU16Array(str, buf, len);
    ov112_021F1488(p, buf, field, len);
    String_Delete(str);
}

void ov112_021F1584(void *p, const u16 *src, int field, int msg) {
    int i;
    int valid = 1;
    if (src[0] != 0) {
        for (i = 0; i < 11; i++) {
            if (src[i] == 0xffff) {
                valid = 0;
                break;
            }
        }
    }
    if (valid) {
        ov112_021F1548(p, field, msg);
    } else {
        ov112_021F1488(p, src, field, 11);
    }
}

void ov112_021F15CC(void *p, int field, int value, int digits, int mode) {
    BufferIntegerAsString(PTR_AT(p, 0x58), field, value, digits, mode, 1);
}

void ov112_021F15E4(void *p) {
    Ov112RecordWork *work = p;
    u32 offset = 0x13d;
    void *slot = work->records[work->recordIndex];
    RTCDate date = { 0 };
    RTCTime time = { 0 };
    u32 dateOffset = offset - 0x1d;
    offset -= 0xd;
    RTC_ConvertSecondToDateTime((RTCDate *)((u8 *)work + dateOffset), (RTCTime *)((u8 *)work + offset), (s64) * (u32 *)slot);
}

void ov112_021F1624(void *p, int msg1, int msg2) {
    Ov112RecordWork *work = p;
    void *record = work->records[work->recordIndex];
    int value9 = work->value9;
    int value10 = work->value10;
    int hour = work->hour;
    int ampm;
    void *str;
    int x;

    if (hour < 12) {
        ampm = 0x57;
        if (hour == 0) {
            hour = 12;
        }
    } else {
        ampm = 0x58;
        hour -= 12;
        if (hour == 0) {
            hour = 12;
        }
    }
    ov112_021F1488(p, PTR_AT(p, 8), 0, 9);
    ov112_021F1584(p, (u16 *)record + 0x10, 1, U16_AT(record, 0xa));
    ov112_021F1488(p, (u16 *)record + 8, 2, 9);
    ov112_021F1584(p, (u16 *)record + 0x1b, 3, U16_AT(record, 0xc));
    ov112_021F1504(p, U16_AT(record, 0xe));
    ov112_021F1584(p, (u16 *)record + 0x1b, 5, U16_AT(record, 0xc));
    ov112_021F15CC(p, 6, hour, 2, 0);
    ov112_021F15CC(p, 7, U16_AT(record, 0x78), 5, 0);
    ov112_021F1488(p, (u16 *)record + 0x26, 8, 0x15);
    ov112_021F15CC(p, 9, value9, 2, 0);
    ov112_021F15CC(p, 10, value10, 2, 0);
    ov112_021F13CC(p, msg1, msg2);
    ov112_021F15CC(p, 9, value9, 2, 1);
    ov112_021F15CC(p, 10, value10, 2, 1);
    ov112_021F13BC(p, 0, 0x56, 0);
    ov112_021F15CC(p, 6, hour, 2, 1);
    str = ReadMsgData_ExpandPlaceholders(PTR_AT(p, 0x58), PTR_AT(p, 0x5c), ampm, U32_AT(p, 4));
    x = FontID_String_GetCenterAlignmentX(1, str, 0, GetWindowWidth((u8 *)p + 0x28) * 8);
    String_Delete(str);
    ov112_021F13BC(p, 1, ampm, (u8)x);
}
