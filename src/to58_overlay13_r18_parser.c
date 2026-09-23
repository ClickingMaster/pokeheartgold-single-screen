#include "global.h"
#include <string.h>
extern u8 ov13_0224E09C[];
extern u8 ov13_0224E284[];
extern u8 ov13_0224E2B0[];
extern u8 ov13_0224E0B0[];
extern u8 ov13_0224E2E8[];
extern u8 ov13_0224E0C4[];
typedef struct Blob64 { u64 v[8]; } Blob64;
typedef struct Pair8 { u32 a, b; } Pair8;
extern Blob64 ov13_0224E144;
extern const Blob64 ov13_0224E380;
extern int ov13_02224D00(u8 *, const char *, int);

int ov13_02224DB4(void);
int ov13_02224DB4(void) {
    volatile int result = 1;
    int i;
    u8 *src;
    char buf[33];
    char *p;
    u8 *out;
    strcpy((char *)ov13_0224E09C, (char *)ov13_0224E284);
    switch (*(u32 *)ov13_0224E2B0) {
    case 0:
        *(u32 *)(ov13_0224E0B0 + 0xC) = 0;
        break;
    case 1:
        if (*(u32 *)(ov13_0224E2B0 + 4) == 0) {
            result = -7;
            break;
        }
        *(u32 *)(ov13_0224E0B0 + 0x10) = *(u32 *)(ov13_0224E2B0 + 4);
        src = ov13_0224E2E8;
        out = ov13_0224E0C4;
        i = 0;
        p = buf;
        for (; i < 4; i++, src += 0x28, out += 0x20) {
            memcpy(buf, src, 0x20);
            buf[32] = 0;
            switch (strlen(buf)) {
            case 0:
                break;
            case 5:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 1;
                memcpy(out, p, 5);
                break;
            case 10:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 1;
                ov13_02224D00(out, p, 10);
                break;
            case 13:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 2;
                memcpy(out, p, 13);
                break;
            case 26:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 2;
                ov13_02224D00(out, p, 26);
                break;
            case 16:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 3;
                memcpy(out, p, 16);
                break;
            case 32:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 3;
                ov13_02224D00(out, p, 32);
                break;
            default:
                result = -7;
                break;
            }
        }
        break;
    case 2:
        *(u32 *)(ov13_0224E0B0 + 0xC) = 4;
        memcpy(&ov13_0224E144, &ov13_0224E380, 64);
        break;
    case 3:
        *(u32 *)(ov13_0224E0B0 + 0xC) = 5;
        memcpy(&ov13_0224E144, &ov13_0224E380, 64);
        break;
    default:
        result = -7;
        break;
    }
    return result;
}
