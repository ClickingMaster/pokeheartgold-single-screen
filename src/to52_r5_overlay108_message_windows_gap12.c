#include "to52_r5_overlay108_message_windows_gap12_private.h"

void ov108_021EA334(void *work, int message) {
    BOOL expand = FALSE;
    FillWindowPixelBuffer((u8 *)work + 0x454, 0);
    switch (message) {
    case 0:
        ReadMsgDataIntoString(PTR_AT(work, 0x30C), U8_AT(work, 0x433) + 0xE, PTR_AT(work, 0x314));
        BufferString(PTR_AT(work, 0x310), 0, PTR_AT(work, 0x314), 1, 0, 2);
        expand = TRUE;
        break;
    case 2:
        if (U8_AT(work, 0x435) == 0) {
            message = 3;
        } else {
            BufferIntegerAsString(PTR_AT(work, 0x310), 0, U8_AT(work, 0x435), 2, 1, 1);
            expand = TRUE;
        }
        break;
    }
    if (expand) {
        ReadMsgDataIntoString(PTR_AT(work, 0x30C), message, PTR_AT(work, 0x314));
        StringExpandPlaceholders(PTR_AT(work, 0x310), PTR_AT(work, 0x318), PTR_AT(work, 0x314));
    } else {
        ReadMsgDataIntoString(PTR_AT(work, 0x30C), message, PTR_AT(work, 0x318));
    }
    AddTextPrinterParameterizedWithColor((u8 *)work + 0x454, 0, PTR_AT(work, 0x318), 0, 0, 0xFF, 0x30400, 0);
    ScheduleWindowCopyToVram((u8 *)work + 0x454);
}

void ov108_021EA418(void *work, int message) {
    FillWindowPixelBuffer((u8 *)work + 0x464, 0xF);
    ReadMsgDataIntoString(PTR_AT(work, 0x30C), message + 4, PTR_AT(work, 0x318));
    DrawFrameAndWindow2((u8 *)work + 0x464, 1, 0x3E2, 0xC);
    AddTextPrinterParameterizedWithColor((u8 *)work + 0x464, 1, PTR_AT(work, 0x318), 0, 0, 0xFF, 0x1020F, 0);
    ScheduleWindowCopyToVram((u8 *)work + 0x464);
}

void ov108_021EA47C(void *work) {
    u8 index;
    u8(*records)[5];
    ov108_021EA624(work);
    FillWindowPixelBuffer((u8 *)work + 0x4A4, 0);
    index = U8_AT(work, 0x430) * 6 + U8_AT(work, 0x431);
    if (U8_AT(work, 0x431) >= 6 || index >= U8_AT(work, 0x42D)) {
        ScheduleWindowCopyToVram((u8 *)work + 0x4A4);
        return;
    }
    records = PTR_AT(work, 0x334);
    ReadMsgDataIntoString(PTR_AT(work, 0x30C), records[index][0] + 0x26, PTR_AT(work, 0x318));
    AddTextPrinterParameterizedWithColor((u8 *)work + 0x4A4, 0, PTR_AT(work, 0x318), 0, 0, 0xFF, 0x10200, 0);
    ScheduleWindowCopyToVram((u8 *)work + 0x4A4);
}
