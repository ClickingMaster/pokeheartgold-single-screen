#include "overlay96_course_anim_gap3_internal.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov96_021F4B34(u8 *outer) {
    u8 *p = outer + 0x68;
    u16 frame;
    u16 alpha;

    U16(p, 0x1A)
    ++;
    frame = U16(p, 0x1A);
    if (frame > 50) {
        ov96_021F4724(p);
        return;
    }
    if (frame == 5) {
        GfGfx_EngineBTogglePlanes(8, TRUE);
    } else if (frame > 15 && (frame - 15) <= 4) {
        alpha = ((frame - 15) * 16) / 4;
        G2x_SetBlendAlpha_(0x04001050, 8, 0x34, 16 - alpha, alpha);
    }
    ov96_021F4DAC(U16(p, 0x1A), ov96_0221C050, PTR(p, 0x90));
    ov96_021F4DAC(U16(p, 0x1A), ov96_0221C080, PTR(p, 0x94));
}

void ov96_021F4BB4(u8 *outer) {
    u8 *p = outer + 0x68;
    s32 matrixA[4];
    s32 matrixB[3];
    u16 alpha;
    u16 frame;

    U16(p, 0x1A)
    ++;
    if (U16(p, 0x1A) > 80) {
        ov96_021F4724(p);
        return;
    }
    if (U16(p, 0x1A) == 41) {
        ov96_021EB52C(PTR(p, 0xC4), 1, 1);
    } else if (U16(p, 0x1A) == 61) {
        ov96_021EB52C(PTR(p, 0xC4), 1, 0);
    }
    frame = U16(p, 0x1A);
    if (frame <= 40) {
        matrixA[0] = (float)frame / 40.0f * 4096.0f;
        matrixA[1] = 0;
        matrixA[2] = 0;
        matrixA[3] = matrixA[0];
        OS_WaitVBlankIntr();
        SetBgAffine(PTR(outer, 8), 7, matrixA, 0x80, 0x60);
        return;
    }
    if (frame > 40 && frame <= 60) {
        matrixB[2] = 0;
        matrixB[0] = (((frame - 40) * 0x90) / 20 + 0x38) << 12;
        matrixB[1] = 0x290000;
        Sprite_SetMatrix(ov96_021EB5B8(PTR(p, 0xC4)), matrixB);
        return;
    }
    ov96_021EB52C(PTR(p, 0xC4), 1, 0);
    alpha = ((U16(p, 0x1A) - 60) << 4) / 20;
    G2x_SetBlendAlpha_(0x04001050, 8, 0x24, 16 - alpha, alpha);
}

void ov96_021F4CAC(u8 *p) {
    u8 tries;
    int a;
    int b;
    s32 matrix[3];
    void *sprite;

    a = (s32)LCRandom() % 4;
    tries = 0;
    do {
        b = (s32)LCRandom() % 4;
        if (a != b) {
            break;
        }
        tries++;
    } while (tries < 10);
    if (tries == 10) {
        b = (a + 1) % 4;
    }
    a = a * 3 + (s32)LCRandom() % 3;
    b = b * 3 + (s32)LCRandom() % 3;
    ov96_021F4504(p, 0, (u8)a);
    ov96_021F4504(p, 1, (u8)b);
    sprite = ov96_021EB5B8(PTR(p, 0x90));
    *(const s32(*)[3])matrix = *(const s32(*)[3])ov96_0221C050;
    matrix[1] += 0x200000;
    Sprite_SetMatrix(sprite, matrix);
    sprite = ov96_021EB5B8(PTR(p, 0x94));
    *(const s32(*)[3])matrix = *(const s32(*)[3])ov96_0221C080;
    matrix[1] += 0x200000;
    Sprite_SetMatrix(sprite, matrix);
    ov96_021EB52C(PTR(p, 0x90), 1, 1);
    ov96_021EB52C(PTR(p, 0x94), 1, 1);
}
