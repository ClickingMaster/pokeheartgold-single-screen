#include "overlay96_bg_affine_gap51_private.h"

void ov96_021EFA3C(Ov96BgAffineGap51Work *work) {
    int offset = 0;
    s32 matrix[4];

    if (work->useDefaultOffset == 0) {
        offset = 0x20;
    }
    matrix[0] = 0x125;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0x125;
    OS_WaitVBlankIntr();
    SetBgAffine(work->bg, 7, matrix, 0x80, 0x60);
    SetBgAffine(work->bg, 6, matrix, 0x80, 0x60);
    Bg_SetTextDimAndAffineParams(work->bg, 7, 5, offset, matrix, 0x80, 0x60);
    Bg_SetTextDimAndAffineParams(work->bg, 6, 5, offset, matrix, 0x80, 0x60);
    GfGfx_EngineBTogglePlanes(4, 1);
    GfGfx_EngineBTogglePlanes(8, 1);
    work->timer = 0;
    work->active = 1;
}

void ov96_021EFACC(Ov96BgAffineGap51Work *work) {
    s32 matrix[4];
    s32 scale = (++work->timer << 12) / 14;

    matrix[0] = scale;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = scale;
    OS_WaitVBlankIntr();
    SetBgAffine(work->bg, 7, matrix, 0x80, 0x60);
    SetBgAffine(work->bg, 6, matrix, 0x80, 0x60);
    if (work->timer >= 14) {
        work->timer = 0;
        work->active = 0;
    }
}

void ov96_021EFB20(void *course, int value) {
    Ov96BgAffineGap51Work *work = PokeathlonCourse_GetHeapAllocPtr4(course);

    BeginNormalPaletteFade(0, 0, 0, 0x7fff, 4, 1, *(int *)((u8 *)work + 0xc));
    work->paletteFadeValue = value;
}
