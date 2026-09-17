#include "global.h"

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

PaletteFadeStateW12 _021D0EF4;

void FadeWork_UpdateFrame(u32 *active, void *work);
void sub_0200FF5C(void *data, int index);
void sub_020100C4(void *data);
void sub_0200FB70(void);

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
