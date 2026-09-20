#include "overlay112_014_ir_private.h"

void ov112_021E6A6C(int result) {
    u8 data[4];
    u16 value;

    ov112_021FFAA4.field14 = 0;
    if (result == 0) {
        ov112_021E5E18(12);
        ov112_021E5A5C();
        return;
    }
    if ((u32)result > 2) {
        ov112_021E5E18(12);
        ov112_021E5A5C();
        return;
    }

    ov112_021FFAA4.tick3C = OS_GetTick();
    switch (ov112_021FFB24.command1C) {
    case 0xF0:
        ov112_021E5E28();
        MI_CpuCopy8((u8 *)ov112_021FFAA4.field20 + 0x10, ov112_021FFB64, 0x28);
        ov112_021E5A68(ov112_021FFB64, 0x74, ov112_021FFB24.command1C, 1);
        break;
    case 0xFE:
        ov112_021E5E28();
        ov112_021E5A68((void *)ov112_021FFAA4.field28, 8, ov112_021FFB24.command1C, 1);
        break;
    case 0xE2:
        data[0] = 0x80;
        value = *(u16 *)&ov112_021FFAA4;
        data[1] = value >> 8;
        data[2] = value;
        data[3] = (data[1] + 1) + data[2];
        ov112_021E5A68(data, 4, 0xA, 0);
        break;
    case 0xE0:
        ov112_021E5E28();
        ov112_021E5A68((u8 *)ov112_021FFAA4.field20 + 0x10, 0x28, ov112_021FFB24.command1C, 1);
        break;
    case 0x2A:
        ov112_021E5E28();
        ov112_021E5A68((u8 *)ov112_021FFAA4.field20 + 0x10, 0x28, ov112_021FFB24.command1D, 1);
        ov112_021E5E18(15);
        ov112_021E5A5C();
        break;
    case 0x32:
    case 0x40:
    case 0x52:
        ov112_021E5E48();
        ov112_021E5A68(NULL, 0, 0x20, 1);
        break;
    case 0x60:
    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        *(u32 *)((u8 *)ov112_021FFAA4.field20 + 0x60) = 0;
        ov112_021E5A68(NULL, 0, 0x20, 1);
        break;
    }
}

void ov112_021E6BDC(void) {
    ov112_021FFB24.field20 = ov112_021FFB24.field24;
    ov112_021FFB24.field28 = ov112_021FFB24.field2C;
    ov112_021FFB24.field30 = ov112_021FFB24.field32;
    ov112_021FFB24.field36 = 0;
    ov112_021FFB24.field3A = 0;
}

#define FAIL_RESPONSE(command, error)          \
    do {                                       \
        ov112_021E5A68(NULL, 0, (command), 1); \
        ov112_021E5E18((error));               \
        ov112_021E5A5C();                      \
        return;                                \
    } while (0)

void ov112_021E6BF8(void) {
    u8 command = ov112_021FFB24.command1C;

    switch (command) {
    case 0x32:
        if (ov112_021FFAA4.field18 != 0 && ov112_021FFAA4.field24->flag0 != 0) {
            FAIL_RESPONSE(0x36, 4);
        }
        ov112_021E5A68(ov112_021FFAA4.field20, 0x68, 0x32, 1);
        return;

    case 0x40:
        if (ov112_021FFAA4.field24->flag0 == 0) {
            FAIL_RESPONSE(0x44, 5);
        }
        if (ov112_021FFAA4.field24->flag1 == 0) {
            FAIL_RESPONSE(0x44, 6);
        }
        if (ov112_021FFAA4.field24->field0C != ov112_021FFAA4.field20->field0C) {
            FAIL_RESPONSE(0x44, 11);
        }
        if (ov112_021FFAA4.field24->field04 != ov112_021FFAA4.field20->field04) {
            FAIL_RESPONSE(0x44, 8);
        }
        if (((u32)ov112_021FFAA4.field24->field0A >> 2) != ((u32)ov112_021FFAA4.field20->field0A >> 2)) {
            FAIL_RESPONSE(0x44, 9);
        }
        if (ov112_021FFAA4.field24->field5D > ov112_021FFAA4.field20->field5D) {
            FAIL_RESPONSE(0x44, 10);
        }
        ov112_021E5A68(ov112_021FFAA4.field20, 0x68, 0x40, 1);
        return;

    case 0x52:
        if (ov112_021FFAA4.field24->flag0 == 0) {
            FAIL_RESPONSE(0x56, 5);
        }
        if (ov112_021FFAA4.field24->flag1 == 1) {
            FAIL_RESPONSE(0x56, 7);
        }
        if (ov112_021FFAA4.field24->field00 != ov112_021FFAA4.field20->field00) {
            FAIL_RESPONSE(0x56, 8);
        }
        if (((u32)ov112_021FFAA4.field24->field08 >> 2) != ((u32)ov112_021FFAA4.field20->field08 >> 2)) {
            FAIL_RESPONSE(0x56, 9);
        }
        if (ov112_021FFAA4.field24->field0C != ov112_021FFAA4.field20->field0C) {
            FAIL_RESPONSE(0x56, 11);
        }
        if (ov112_021FFAA4.field24->field5D > ov112_021FFAA4.field20->field5D) {
            FAIL_RESPONSE(0x56, 10);
        }
        ov112_021E5A68(ov112_021FFAA4.field20, 0x68, 0x52, 1);
        return;

    case 0x60:
        if (ov112_021FFAA4.field24->flag0 == 0) {
            ov112_021E5A68(ov112_021FFAA4.field20, 0x68, 0x64, 1);
            ov112_021E5E18(5);
            ov112_021E5A5C();
            return;
        }
        if (ov112_021FFAA4.field24->field5D > ov112_021FFAA4.field20->field5D) {
            ov112_021E5A68(ov112_021FFAA4.field20, 0x68, 0x64, 1);
            ov112_021E5E18(10);
            ov112_021E5A5C();
            return;
        }
        ov112_021E5A68(ov112_021FFAA4.field20, 0x68, 0x60, 1);
        return;

    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        if (ov112_021FFAA4.field24->flag0 == 0) {
            FAIL_RESPONSE(0xD8, 5);
        }
        if (ov112_021FFAA4.field24->field00 != ov112_021FFAA4.field20->field00) {
            FAIL_RESPONSE(0xD8, 8);
        }
        if (((u32)ov112_021FFAA4.field24->field08 >> 2) != ((u32)ov112_021FFAA4.field20->field08 >> 2)) {
            FAIL_RESPONSE(0xD8, 9);
        }
        if (ov112_021FFAA4.field24->field5D > ov112_021FFAA4.field20->field5D) {
            FAIL_RESPONSE(0xD8, 10);
        }
        ov112_021E5A68(ov112_021FFAA4.field20, 0x68, (u8)(command - 0x10), 1);
        return;

    default:
        ov112_021E5E18(12);
        ov112_021E5A5C();
        return;
    }
}
