#include "global.h"
#include "screen_fade.h"

typedef struct PaletteFadeStateW12 {
    u32 mode;
    u32 mainActive;
    u32 subActive;
    u32 unk0C;
    u32 unk10;
    u8 mainWork[0x30];
    u8 subWork[0x30];
    u8 hblank[0x18];
    u8 params[0xB4];
    u8 status[0x14];
} PaletteFadeStateW12;

typedef struct FadeStatus {
    u8 pad[0xC];
    u16 active;
    u8 mainDone;
    u8 subDone;
    u16 color;
    u8 tail[2];
} FadeStatus;

PaletteFadeStateW12 _021D0EF4;

#define FADE_STATUS ((FadeStatus *)_021D0EF4.status)

void GF_AssertFail(void);
BOOL DoFadeUpdateFrame(void *state, void *mainWork, void *subWork);
void HandleEndFade(void *fade);
void FadeWork_UpdateFrame(u32 *active, void *work);
void sub_0200FF5C(void *data, int index);
void sub_020100C4(void *data);
void sub_0200FE14(int mode, u32 *dst);
void sub_0200FEB0(void *data);
u16 sub_02010018(u8 *data, u16 color);
void sub_0200FE84(void *dst, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9, u16 color);
void sub_02010064(void *param);

#pragma opt_common_subs off
void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType mainType, enum FadeType subType, u16 color, int steps, int framesPerStep, enum HeapID heap) {
    if (steps == 0) GF_AssertFail();
    if (framesPerStep == 0) GF_AssertFail();
    if (FADE_STATUS->active != 0) GF_AssertFail();
    {
        PaletteFadeStateW12 *fade = &_021D0EF4;
        sub_020100C4(&_021D0EF4);
        sub_0200FE14(mode, &_021D0EF4.mode);
        sub_0200FEB0(_021D0EF4.hblank);
        color = sub_02010018((u8 *)&_021D0EF4, color);
        sub_0200FE84(_021D0EF4.mainWork, mainType, steps, framesPerStep, 0, 0, 0, (u32)_021D0EF4.params, (u32)_021D0EF4.hblank, heap, color);
        sub_0200FE84(_021D0EF4.subWork, subType, steps, framesPerStep, 0, 0, 1, (u32)_021D0EF4.params, (u32)_021D0EF4.hblank, heap, color);
        FADE_STATUS->active = 1;
        FadeWork_UpdateFrame(&_021D0EF4.mainActive, _021D0EF4.mainWork);
        FadeWork_UpdateFrame(&_021D0EF4.subActive, _021D0EF4.subWork);
        if (_021D0EF4.unk0C != 0) {
            sub_02010064(fade->mainWork);
            fade->status[0xE] = 1;
        }
        if (fade->unk10 != 0) {
            sub_02010064(fade->subWork);
            fade->status[0xF] = 1;
        }
    }
}
#pragma opt_common_subs reset

void HandleFadeUpdateFrame(void) {
    u8 *fade = (u8 *)&_021D0EF4;
    if (FADE_STATUS->active != 0) {
        if (DoFadeUpdateFrame(fade, fade + 0x14, fade + 0x44) == TRUE) {
            HandleEndFade(fade);
        }
    }
}

BOOL IsPaletteFadeFinished(void) {
    return FADE_STATUS->active == 0;
}

void sub_0200FB70(void) {
    sub_0200FF5C(_021D0EF4.hblank, 0);
    sub_0200FF5C(_021D0EF4.hblank, 1);
    if (_021D0EF4.mainActive != 0) {
        *(u32 *)(_021D0EF4.mainWork + 0xC) = 2;
    }
    if (_021D0EF4.subActive != 0) {
        *(u32 *)(_021D0EF4.subWork + 0xC) = 2;
    }
    FadeWork_UpdateFrame(&_021D0EF4.mainActive, _021D0EF4.mainWork);
    FadeWork_UpdateFrame(&_021D0EF4.subActive, _021D0EF4.subWork);
    *(u16 *)(_021D0EF4.status + 0xC) = 0;
    _021D0EF4.status[0xE] = 0;
    _021D0EF4.status[0xF] = 0;
    sub_020100C4(&_021D0EF4);
}
