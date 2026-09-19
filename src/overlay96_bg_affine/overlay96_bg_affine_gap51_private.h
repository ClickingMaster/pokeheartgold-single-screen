#ifndef TO53_OVERLAY96_BG_AFFINE_GAP51_PRIVATE_H
#define TO53_OVERLAY96_BG_AFFINE_GAP51_PRIVATE_H

#include "global.h"

typedef struct Ov96BgAffineGap51Work {
    int unk00;
    void *bg;
    u8 pad08[0x10];
    int paletteFadeValue;
    int timer;
    u8 pad20[8];
    int useDefaultOffset;
    int active;
} Ov96BgAffineGap51Work;

void OS_WaitVBlankIntr(void);
void SetBgAffine(void *, int, void *, int, int);
void Bg_SetTextDimAndAffineParams(void *, int, int, int, void *, int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void *PokeathlonCourse_GetHeapAllocPtr4(void *);
void BeginNormalPaletteFade(int, int, int, int, int, int, int);

void ov96_021EFA3C(Ov96BgAffineGap51Work *);
void ov96_021EFACC(Ov96BgAffineGap51Work *);
void ov96_021EFB20(void *, int);

#endif
