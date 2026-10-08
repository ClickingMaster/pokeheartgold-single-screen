#pragma force_active on
#include "global.h"
typedef struct Wipe74 {
    u8 unk000[0xC00];
    void *hblank;
    u8 unkC04[4];
    fx32 speed;
    fx32 accel;
    BOOL active;
    int mode;
} Wipe74;
typedef struct Work74 {
    u8 unk0000[0x30E8];
    Wipe74 wipe;
} Work74;
u16 *sub_02014A4C(void *);
void sub_02014A8C(void *);
BOOL ov74_0222F6C4(Work74 *work);
// @fn ov74_0222F6C4
BOOL ov74_0222F6C4(Work74 *work) {
    Wipe74 *wipe = &work->wipe;
    fx32 pos = 84 << 12;
    int i;
    int x;
    u16 *buf;
    if (wipe->active == FALSE) {
        return TRUE;
    }
    if (wipe->mode == 1) {
        wipe->speed += wipe->accel;
        wipe->accel += wipe->accel;
        if (wipe->speed / FX32_ONE > 1000) {
            wipe->active = FALSE;
            wipe->speed = 0;
            pos = 168;
            wipe->accel = 0;
        }
    } else {
        wipe->speed -= wipe->accel;
        wipe->accel /= 2;
        if (wipe->accel < 1024) {
            wipe->active = FALSE;
            wipe->speed = FX32_ONE;
            wipe->accel = 0;
        }
    }
    buf = sub_02014A4C(wipe->hblank);
    for (i = 84; i < 168; i++) {
        x = pos / FX32_ONE;
        if (x < 0) {
            x = 0;
        }
        if (x > 168) {
            x = 168;
        }
        buf[i * 4 + 3] = x - i;
        buf[i * 4 + 1] = buf[i * 4 + 3];
        buf[(168 - i) * 4 + 3] = i - x;
        buf[(168 - i) * 4 + 1] = buf[(168 - i) * 4 + 3];
        pos += wipe->speed;
    }
    DC_FlushRange(buf, 0x600);
    sub_02014A8C(wipe->hblank);
    return FALSE;
}
