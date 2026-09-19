#include "global.h"
#include "nitro/gx/g2.h"

#define S8AT(p, o) (*(s8 *)((u8 *)(p) + (o)))
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern void BG_SetMaskColor(u32, u32);
extern void Heap_Free(void *);
extern void GfGfx_EngineATogglePlanes(u32, u32);
extern void GfGfx_EngineBTogglePlanes(u32, u32);
extern void ov74_0222AF28(void *);
extern void ov74_0222B288(void *);
extern void ov74_0222B3A4(void *);
extern void ov74_0222B144(void *);
extern void sub_02034DE0(void);
extern void sub_0203A914(void);
extern void sub_02038D28(void *);
extern u32 sub_02038D80(void);
extern void sub_02038D64(void);
extern void *ov74_0223D0A8;

void ov74_0222BED8(void *p);
void ov74_0222BEF0(void *p);
void ov74_0222BF08(void *p);
void ov74_0222BF18(void);
BOOL ov74_0222BF2C(void *p, int mode, int step);
BOOL ov74_0222BFA0(void *p, int mode, int step);
void ov74_0222C014(void *p);
void *ov74_0222CD7C(void);
void ov74_0222CD88(void *p);
void ov74_0222CE6C(u32 value);
void ov74_0222CE10(void *p);
void ov74_0222CEC0(void);

void ov74_0222BED8(void *p) {
    G2x_SetBlendBrightness_((volatile u16 *)0x04000050, 0x22, S8AT(p, 8));
}
void ov74_0222BEF0(void *p) {
    G2x_SetBlendBrightness_((volatile u16 *)0x04001050, 0x22, S8AT(p, 8));
}
void ov74_0222BF08(void *p) {
    ov74_0222BED8(p);
    ov74_0222BEF0(p);
}
void ov74_0222BF18(void) {
    BG_SetMaskColor(1, 0);
    BG_SetMaskColor(5, 0);
}
BOOL ov74_0222BF2C(void *p, int mode, int step) {
    BOOL done = TRUE;
    int value;

    switch (mode) {
    case 0:
        value = S8AT(p, 8);
        if (value > 0) {
            value -= step;
            if (value > 0) {
                S8AT(p, 8) = value;
                done = FALSE;
            } else {
                S8AT(p, 8) = 0;
            }
        } else if (value < 0) {
            value += step;
            if (value < 0) {
                S8AT(p, 8) = value;
                done = FALSE;
            } else {
                S8AT(p, 8) = 0;
            }
        }
        break;
    case 1:
        value = S8AT(p, 8) - step;
        if (value > -16) {
            S8AT(p, 8) = value;
            done = FALSE;
        } else {
            S8AT(p, 8) = -16;
        }
        break;
    case 2:
        value = S8AT(p, 8) + step;
        if (value < 16) {
            S8AT(p, 8) = value;
            done = FALSE;
        } else {
            S8AT(p, 8) = 16;
        }
        break;
    }
    ov74_0222BF08(p);
    return done;
}
BOOL ov74_0222BFA0(void *p, int mode, int step) {
    BOOL done = TRUE;
    int value;

    switch (mode) {
    case 0:
        value = S8AT(p, 8);
        if (value > 0) {
            value -= step;
            if (value > 0) {
                S8AT(p, 8) = value;
                done = FALSE;
            } else {
                S8AT(p, 8) = 0;
            }
        } else if (value < 0) {
            value += step;
            if (value < 0) {
                S8AT(p, 8) = value;
                done = FALSE;
            } else {
                S8AT(p, 8) = 0;
            }
        }
        break;
    case 1:
        value = S8AT(p, 8) - step;
        if (value > -16) {
            S8AT(p, 8) = value;
            done = FALSE;
        } else {
            S8AT(p, 8) = -16;
        }
        break;
    case 2:
        value = S8AT(p, 8) + step;
        if (value < 16) {
            S8AT(p, 8) = value;
            done = FALSE;
        } else {
            S8AT(p, 8) = 16;
        }
        break;
    }
    ov74_0222BED8(p);
    return done;
}
