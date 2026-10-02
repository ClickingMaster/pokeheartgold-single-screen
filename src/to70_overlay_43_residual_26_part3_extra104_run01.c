#pragma force_active on
#ifndef TO50_OVERLAY43_R26_PART3_PRIVATE_H
#define TO50_OVERLAY43_R26_PART3_PRIVATE_H
#include "global.h"
#pragma force_active on

#include "bg_window.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov43_0222AB20(void *, void *, int);
void ov43_0222A9F4(void *, void *, int);
void ov43_0222D8B8(void *, void *, void *, void *);
void ov43_0222AD98(void *, int, int);
void ov43_0222DAE8(void *, void *, void *, int, int, void *);
void ov43_0222DB94(void *, void *, int);
void Sprite_SetPositionXY(void *, s16, s16);
void ov43_0222AD40(void *, int, int);
void ov43_0222AD00(void *, int);
void ov43_0222DF90(void *, void *);
void ov43_0222DC7C(void *, void *);
void ov43_0222DB84(void *, void *);
void ov43_0222DACC(void *);
void ov43_0222AA70(void *);

void ov43_0222D654(void *, void *, void *, void *);
void ov43_0222D778(void *, void *);
#endif


void ov43_0222D654(void *p, void *state, void *work, void *arg) {
    u16 *screen;

    ov43_0222AB20(work, PTR_AT(state, 4), U8_AT((u8 *)state + S8_AT(state, 0xB), 0x18));
    ov43_0222A9F4(work, (u8 *)p + 0x128, 13);

    screen = PTR_AT(p, 0x114);
    LoadRectToBgTilemapRect(PTR_AT(work, 0), 5, screen + 6, 0, 0, screen[0] >> 3, screen[1] >> 3);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0), 5);

    screen = PTR_AT(p, 0xFC);
    LoadRectToBgTilemapRect(PTR_AT(work, 0), 0, screen + 6, 0, 0, screen[0] >> 3, screen[1] >> 3);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0), 0);

    screen = PTR_AT(p, 0x104);
    LoadRectToBgTilemapRect(PTR_AT(work, 0), 2, screen + 6, 0, 0, screen[0] >> 3, screen[1] >> 3);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0), 2);

    ov43_0222D8B8(p, state, work, arg);
    ov43_0222AD98(work, 0, 0);
    ov43_0222DAE8(p, state, work, 0, S16_AT(p, 0), arg);
    ov43_0222DB94(p, work, 0xFF);
    Sprite_SetPositionXY(PTR_AT(work, 0x1F4), 0xF4, 0x18);
    ov43_0222AD40(work, 2, 0);
    Sprite_SetPositionXY(PTR_AT(work, 0x1F8), 0xF4, 0x88);
    ov43_0222AD40(work, 3, 0);
    ov43_0222AD00(work, 1);
    SetBgPriority(2, 0);
}

void ov43_0222D778(void *p, void *work) {
    int i;
    u8 *row;
    u16 *screen;

    ov43_0222AD00(work, 0);
    Sprite_SetPositionXY(PTR_AT(work, 0x1F4), 8, 0x62);
    ov43_0222AD40(work, 0, 0);
    Sprite_SetPositionXY(PTR_AT(work, 0x1F8), 0xF8, 0x62);
    ov43_0222AD40(work, 1, 0);

    i = 0;
    row = (u8 *)p + 0x14;
    for (; i < 3; i++, row += 0x4C) {
        ov43_0222DF90(row, work);
    }
    ov43_0222DC7C(p, work);
    ov43_0222DB84(p, work);
    ov43_0222DACC(p);

    FillBgTilemapRect(PTR_AT(work, 0), 5, 0, 0, 0, 0x20, 0x20, 0);
    FillBgTilemapRect(PTR_AT(work, 0), 4, 0, 0, 0, 0x20, 0x20, 0);
    FillBgTilemapRect(PTR_AT(work, 0), 2, 0, 0, 0, 0x20, 0x20, 0);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0), 5);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0), 4);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0), 2);

    screen = PTR_AT(p, 0x10C);
    LoadRectToBgTilemapRect(PTR_AT(work, 0), 0, screen + 6, 0, 0, screen[0] >> 3, screen[1] >> 3);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0), 0);
    ov43_0222AA70(work);
    SetBgPriority(2, 2);
}
