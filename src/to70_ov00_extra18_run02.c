#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int rand(void);
#pragma force_active on
#define I(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern void ov00_021EC3D8(void *);
extern void ov00_02202B28(void *, void *);
extern int ov00_0220A5A8(void *, void *, s32 *);
extern int ov00_02207E40(void *, void *);
extern void ov00_0220A86C(void *, s32, const void *);
extern void ov00_02202990(void *, s32, s32);
extern void ov00_0220A890(void *, const void *);
extern void ov00_0220A348(void *, const void *, s32);
extern void __msl_assertion_failed(const void *, const void *, const void *, s32);
extern char ov00_0221869C[];
extern char ov00_022186EC[];
extern char ov00_02218860[];
extern char ov00_02218AC4[];
extern char ov00_02218AEC[];
extern char ov00_02218B1C[];
extern char ov00_02218B40[];
extern char ov00_02218B4C[];
extern char ov00_02218B5C[];
int ov00_02202D20(void *ctx, void *filter);
void ov00_02202E08(char *out, int count);
int ov00_0220411C(void *ctx);
int ov00_022041C0(void *ctx, void *node);
int ov00_0220444C(s32 day, s32 month, s32 year);
int ov00_022045AC(void *ctx, s32 value, s32 *day, s32 *month, s32 *year);
void ov00_02204654(void *input, void *output);

extern int ov00_022020EC(void *, void *, const void *);
extern int ov00_0220211C(void *, void *, s32);
extern int ov00_0220267C(void *, void *, void *, s32 *, s32, const void *);
extern void ov00_021FF82C(void *, s32);
extern void ov00_021FF818(void *);
extern void ov00_022066D0(void *, void *);
extern void ov00_022071F4(void *, void *);
extern int ov00_02207F1C(void *, int (*)(void *, void *), s32);
extern void ov00_02204284(void *, s32);
extern char ov00_02218B04[];
extern char ov00_022189C0[];
extern char ov00_02218B18[];

int ov00_0220411C(void *ctx) {
    void *s = P(ctx, 0);
    s32 state;
    int e = ov00_0220A5A8(ctx, P(s, 0x1d4), &state);
    if (e) {
        return e;
    }
    if (state == 4) {
        ov00_0220A86C(ctx, 0x107, ov00_02218AC4);
        ov00_02202990(ctx, 4, 1);
        return 4;
    }
    if (state == 0) {
        return 0;
    }
    if (state != 3) {
        __msl_assertion_failed(ov00_02218AEC, ov00_02218860, ov00_0221869C, 0x2ef);
    }
    I(s, 0x1d8) = 2;
    return 0;
}

int ov00_022041C0(void *ctx, void *node) {
    void *s = P(ctx, 0);
    if (P(node, 8) && !I(s, 0x104)) {
        ov00_021EC3D8(P(P(node, 8), 8));
        P(P(node, 8), 8) = 0;
        ov00_021EC3D8(P(P(node, 8), 12));
        P(P(node, 8), 12) = 0;
        ov00_021EC3D8(P(node, 8));
        P(node, 8) = 0;
    }
    ov00_021EC3D8(P(node, 16));
    P(node, 16) = 0;
    ov00_021EC3D8(P(node, 24));
    P(node, 24) = 0;
    I(node, 20) = 0;
    if (!I(node, 12) || (I(s, 0x104) == 1 && !P(node, 8))) {
        ov00_02207E40(ctx, node);
        return 0;
    }
    return 1;
}

void ov00_02204284(void *ctx, s32 notify) {
    void *state = P(ctx, 0), *p;
    s32 done;
    if (I(state, 0x1d8) == 4) {
        return;
    }
    if (I(state, 0x1d8) != 0) {
        if (notify && I(state, 0x1d8) == 3) {
            ov00_022020EC(ctx, (u8 *)state + 0x1f4, ov00_02218B04);
            ov00_0220211C(ctx, (u8 *)state + 0x1f4, I(state, 0x198));
            ov00_022020EC(ctx, (u8 *)state + 0x1f4, ov00_022189C0);
        }
        ov00_0220267C(ctx, P(state, 0x1d4), (u8 *)state + 0x1f4, &done, 1, ov00_02218B18);
        if (P(state, 0x1d4) != (void *)-1) {
            ov00_021FF82C(P(state, 0x1d4), 2);
            ov00_021FF818(P(state, 0x1d4));
            I(state, 0x1d4) = -1;
        }
        if (P(state, 0x204) != (void *)-1) {
            ov00_021FF82C(P(state, 0x204), 2);
            ov00_021FF818(P(state, 0x204));
            I(state, 0x204) = -1;
        }
        I(state, 0x1d8) = 4;
        I(state, 0x19c) = 0;
        I(state, 0x1a0) = 0;
    }
    ov00_021EC3D8(P(state, 476));
    P(state, 476) = 0;
    ov00_021EC3D8(P(state, 492));
    P(state, 492) = 0;
    ov00_021EC3D8(P(state, 500));
    P(state, 500) = 0;
    ov00_021EC3D8(P(state, 1088));
    P(state, 1088) = 0;
    ov00_021EC3D8(P(state, 1104));
    P(state, 1104) = 0;
    while (P(state, 0x424)) {
        ov00_022066D0(ctx, P(state, 0x424));
    }
    P(state, 0x424) = 0;
    p = P(state, 0x434);
    while (p) {
        void *current = p;
        p = P(p, 0x3c);
        ov00_022071F4(ctx, current);
    }
    P(state, 0x434) = 0;
    while (!ov00_02207F1C(ctx, ov00_022041C0, 0)) {}
}

int ov00_0220444C(s32 day, s32 month, s32 year) {
    if (day == 0 && month == 0 && year == 0) {
        return 1;
    }
    if (day < 0 || month < 0 || year < 0) {
        return 0;
    }
    switch (month) {
    case 0:
        if (day != 0) {
            return 0;
        }
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (day > 31) {
            return 0;
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (day > 30) {
            return 0;
        }
        break;
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            if (day > 29) {
                return 0;
            }
        } else if (day > 28) {
            return 0;
        }
        break;
    default:
        return 0;
    }
    if (year < 1900) {
        return 0;
    }
    if (year > 2079) {
        return 0;
    }
    if (year == 2079) {
        if (month > 6) {
            return 0;
        }
        if (month == 6 && day > 6) {
            return 0;
        }
    }
    return 1;
}

int ov00_022045AC(void *ctx, s32 value, s32 *day, s32 *month, s32 *year) {
    s32 d = (value >> 24) & 255, m = (value >> 16) & 255, y = (u16)value;
    if (!ov00_0220444C(d, m, y)) {
        __msl_assertion_failed(ov00_02218B5C, ov00_02218B40, ov00_02218B1C, 0xb7);
    }
    if (!ov00_0220444C(d, m, y)) {
        ov00_0220A890(ctx, ov00_02218B4C);
        return 2;
    }
    *day = d;
    *month = m;
    *year = y;
    return 0;
}

void ov00_02204654(void *input, void *output) {
    if (!P(input, 0)) {
        *((u8 *)output + 8) = 0;
    } else {
        ov00_0220A348((u8 *)output + 8, P(input, 0), 31);
    }
    if (!P(input, 4)) {
        *((u8 *)output + 39) = 0;
    } else {
        ov00_0220A348((u8 *)output + 39, P(input, 4), 21);
    }
    if (!P(input, 8)) {
        *((u8 *)output + 60) = 0;
    } else {
        ov00_0220A348((u8 *)output + 60, P(input, 8), 51);
    }
    if (!P(input, 12)) {
        *((u8 *)output + 111) = 0;
    } else {
        ov00_0220A348((u8 *)output + 111, P(input, 12), 31);
    }
    if (!P(input, 16)) {
        *((u8 *)output + 142) = 0;
    } else {
        ov00_0220A348((u8 *)output + 142, P(input, 16), 31);
    }
    if (!P(input, 20)) {
        *((u8 *)output + 173) = 0;
    } else {
        ov00_0220A348((u8 *)output + 173, P(input, 20), 76);
    }
    I(output, 252) = I(input, 24);
    ov00_0220A348((u8 *)output + 256, (u8 *)input + 28, 11);
    ov00_0220A348((u8 *)output + 267, (u8 *)input + 39, 3);
    I(output, 272) = I(input, 44);
    I(output, 276) = I(input, 48);
    if (!((u8 *)input + 52)) {
        *((u8 *)output + 280) = 0;
    } else {
        ov00_0220A348((u8 *)output + 280, (u8 *)input + 52, 128);
    }
    I(output, 408) = I(input, 180);
    I(output, 412) = I(input, 184);
    I(output, 416) = I(input, 188);
    I(output, 420) = I(input, 192);
    I(output, 424) = I(input, 196);
    if (!P(input, 200)) {
        *((u8 *)output + 428) = 0;
    } else {
        ov00_0220A348((u8 *)output + 428, P(input, 200), 51);
    }
    I(output, 252) = I(input, 24);
    I(output, 272) = I(input, 44);
    I(output, 276) = I(input, 48);
    I(output, 408) = I(input, 180);
    I(output, 412) = I(input, 184);
    I(output, 416) = I(input, 188);
    I(output, 420) = I(input, 192);
    I(output, 424) = I(input, 196);
    I(output, 480) = I(input, 204);
    I(output, 484) = I(input, 208);
    I(output, 488) = I(input, 212);
    I(output, 492) = I(input, 216);
    I(output, 496) = I(input, 220);
    I(output, 500) = I(input, 224);
    I(output, 504) = I(input, 228);
    I(output, 508) = I(input, 232);
    I(output, 512) = I(input, 236);
}
