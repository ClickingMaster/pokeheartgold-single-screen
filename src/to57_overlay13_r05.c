#include "to57_overlay13_r05_private.h"

int ov13_02221C78(const u8 *src, u8 *dst) {
    const u8 *entry;
    u8 *dst80;
    int length;

    ov13_02222978(dst, 0, 0x104);
    dst80 = dst;
    entry = src;
    dst80 += 0x80;
    for (;;) {
        length = ov13_02222A84(*(const u16 *)(entry + 2));
        if (length <= 0) {
            return -1;
        }
        switch (entry[0]) {
        case 0:
            ov13_02222968(dst, entry + 6, length);
            break;
        case 1:
            ov13_02222968(dst80, entry + 6, length);
            break;
        case 2:
            ov13_02222968(dst + 0x100, entry + 6, length);
            break;
        case 3:
        case 4:
            if (ov13_02222A84(entry[6]) <= 0) {
                return -2;
            }
            break;
        case 5:
            _0224CF98.unk34 = ov13_02222A5C(ov13_02221D34(entry + 6, length));
            break;
        case 6:
            _0224CF98.unk38 = ov13_02222A5C(ov13_02221D34(entry + 6, length));
            break;
        default:
            return -1;
        }
        if (*(const u16 *)(entry + 4) == 0) {
            break;
        }
        entry = src + ov13_02222A84(*(const u16 *)(entry + 4));
    }
    return 0;
}

int ov13_02221D34(const u8 *src, int length) {
    int offset = length - 1;
    int value = 0;
    const u8 *p = src + offset;
    int i = value;
    for (; i < length; i++) {
        value = (value << 8) + *p--;
    }
    return value;
}

int ov13_02221D58(const u8 *src, u8 *dst) {
    const u8 *entry = src + 6;
    const u8 *base;
    u8 *dst08 = dst + 8;
    u8 *dstF0 = dst + 0xF0;
    u8 *dstB0 = dst + 0xB0;
    u8 *dst70 = dst + 0x70;
    u8 *dst30 = dst + 0x30;
    u32 length;
    base = entry;

    for (;;) {
        length = ov13_02222A84(*(const u16 *)(entry + 2));
        switch (entry[0]) {
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            if (length > 5) {
                return -1;
            }
            break;
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
            if (length > 13) {
                return -1;
            }
            break;
        case 0x15:
        case 0x25:
            if (length > 33) {
                return -1;
            }
            break;
        }
        switch (entry[0]) {
        case 0x10:
        case 0x20:
            ov13_02222968(dst30, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x11:
        case 0x21:
            ov13_02222968(dst70, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x12:
        case 0x22:
            ov13_02222968(dstB0, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x13:
        case 0x23:
            ov13_02222968(dstF0, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x15:
        case 0x25:
            if (length != 0 && entry[5 + length] != 0) {
                return -1;
            }
            ov13_02222968(dst08, entry + 6, length);
            break;
        default:
            return -1;
        }
        if (*(const u16 *)(entry + 4) == 0) {
            break;
        }
        entry = base + ov13_02222A84(*(const u16 *)(entry + 4));
    }
    return 0;
}

int ov13_02221EAC(const u8 *src, u8 *dst) {
    const u8 *entry = src + 6;
    const u8 *base;
    u8 *dst08 = dst + 8;
    u8 *dst30 = dst + 0x30;
    u32 length;
    base = entry;

    for (;;) {
        length = ov13_02222A84(*(const u16 *)(entry + 2));
        switch (entry[0]) {
        case 0x30:
        case 0x40:
            if (length > 0x40) {
                return -1;
            }
            break;
        case 0x35:
        case 0x45:
            if (length > 0x21) {
                return -1;
            }
            break;
        }
        switch (entry[0]) {
        case 0x30:
        case 0x40:
            ov13_02222968(dst30, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x35:
        case 0x45:
            if (length != 0 && entry[5 + length] != 0) {
                return -1;
            }
            ov13_02222968(dst08, entry + 6, length);
            break;
        default:
            return -1;
        }
        if (*(const u16 *)(entry + 4) == 0) {
            break;
        }
        entry = base + ov13_02222A84(*(const u16 *)(entry + 4));
    }
    return 0;
}
