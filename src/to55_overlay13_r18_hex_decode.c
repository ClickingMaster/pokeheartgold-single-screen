#include "global.h"

int ov13_02224D00(u8 *dst, const char *src, int length);

int ov13_02224D00(u8 *dst, const char *src, int length) {
    int len = length;
    int c;
    int nibble = 0;
    const char *text = src;
    int i = nibble;

    for (; i < len; i++) {
        c = text[i];
        switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            nibble += c - '0';
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            nibble += c - 'a' + 10;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            nibble += c - 'A' + 10;
            break;
        default:
            return 0;
        }
        if ((i % 2) == 0) {
            nibble <<= 4;
        } else {
            dst[i / 2] = nibble;
            nibble = 0;
        }
    }
    return 1;
}
