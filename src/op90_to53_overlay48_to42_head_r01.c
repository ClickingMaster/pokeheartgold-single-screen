#pragma force_active on
#include "global.h"

typedef struct {
    u8 unk0[4];
    u32 unk4_0 : 1;
    u32 unk4_1 : 31;
    u8 unk8[0x178 - 0x8];
    u8 camera[0xC3CC - 0x178];
    u8 unkC3CC[0x14];
    u8 unkC3E0[4];
} Ov48Work;

void ov48_022598CC(void *p, VecFx32 *dst);
void ov48_022598DC(void *p, VecFx32 *src);
u32 ov48_02259E5C(void *p, s32 sel);
u16 ov48_02259CFC(void *p);
u32 ov48_02258D54(Ov48Work *work, u32 trg, u32 held);

// @fn ov48_02258D54
u32 ov48_02258D54(Ov48Work *work, u32 trg, u32 held) {
    u16 mode;
    u32 touch;
    u16 speedY;
    u16 speedX;
    u32 ret;
    u32 dx;
    VecFx32 pos;
    s16 x;
    u32 dy;
    s16 y;

    ret = 0;
    ov48_022598CC(work->camera, &pos);
    touch = ov48_02259E5C(work->unkC3E0, 0);
    dx = ov48_02259E5C(work->unkC3E0, 1);
    dy = ov48_02259E5C(work->unkC3E0, 2);
    mode = ov48_02259CFC(work->unkC3CC);
    x = pos.x;
    y = pos.y;
    if ((trg & 1) || (touch & 1)) {
        if (work->unk4_0 == 1) {
            ret = 1;
        }
        return ret;
    }
    if (mode == 0) {
        if (dx != 0 || dy != 0) {
            speedY = dx * 0x55;
            speedX = dy * 0x55;
        } else {
            speedY = 0x200;
            speedX = 0x200;
        }
    } else {
        if (dx != 0 || dy != 0) {
            speedY = dx * 10;
            speedX = dy * 10;
        } else {
            speedY = 0x20;
            speedX = 0x20;
        }
    }
    if ((held & 0x20) || (touch & 0x20)) {
        if (work->unk4_0 == 1) {
            pos.y += speedY;
        } else if (y < -10208) {
            pos.y += speedY;
        }
    }
    if ((held & 0x10) || (touch & 0x10)) {
        if (work->unk4_0 == 1) {
            pos.y -= speedY;
        } else if (y > -13184) {
            pos.y -= speedY;
        }
    }
    if ((held & 0x40) || (touch & 0x40)) {
        if (work->unk4_0 == 1) {
            if (x + speedX < 0x3E00) {
                pos.x += speedX;
            } else {
                pos.x = 0x3E00;
            }
        } else if (x < 0x2020) {
            pos.x += speedX;
        }
    }
    if ((held & 0x80) || (touch & 0x80)) {
        if (work->unk4_0 == 1) {
            if (x - speedX > -0x3E00) {
                pos.x -= speedX;
            } else {
                pos.x = -0x3E00;
            }
        } else if (x > 0x1300) {
            pos.x -= speedX;
        }
    }
    ov48_022598DC(work->camera, &pos);
    return 0;
}
