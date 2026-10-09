#pragma force_active on
#include "global.h"

typedef struct Player {
    u8 unk0[0x30];
    fx32 x;
    fx32 y;
    u8 unk38[0x24];
    u16 slot;
    u8 unk5E[0x1A];
    u32 flag;
} Player; // size 0x7C

typedef struct Ball {
    u8 unk0[8];
    fx32 y;
    u8 unkC[0x2D];
    u8 flag;
    u8 unk3A[0x12];
} Ball; // size 0x4C

typedef struct Work {
    u8 unk0[0x5C];
    Player players[4][3];
    Ball balls[2];
} Work;

typedef struct Packet {
    u8 x[12];
    u8 y[12];
    u8 ballY[2];
    u8 ballAttr[2];
    u32 packed;
    u32 flags;
} Packet;

u8 ov96_02213EC4(Ball *ball);
void ov96_02211DE4(Work *work, Packet *out);

// @fn ov96_02211DE4
void ov96_02211DE4(Work *work, Packet *out) {
    Work *w = work;
    Ball *ball;
    u8 attr;
    int ballY;
    u8 val;
    int i;
    Player (*players)[3];
    int shift;
    u32 posBits;
    u32 flagBits;
    u8 cur;
    u32 attrBits;

    posBits = 0;
    flagBits = 0;
    attrBits = 0;
    for (i = 0, shift = 0, players = w->players; i < 12; i++, shift += 2) {
        Player *p = &players[i / 3][i % 3];
        int y = p->y / FX32_ONE;
        int x = p->x / FX32_ONE;
        u8 flag = p->flag;
        u8 slot = p->slot - 1;
        out->x[i] = x;
        out->y[i] = y;
        posBits += slot << shift;
        flagBits += flag << shift;
    }
    ball = w->balls;
    for (i = 0; i < 2; i++) {
        ballY = ball->y / FX32_ONE;
        attr = ov96_02213EC4(ball);
        out->ballY[i] = ballY;
        out->ballAttr[i] = attr;
        attrBits += ball->flag << (i * 2);
        ball++;
    }
    cur = ((u8 *)w)[0x73C];
    val = *(u32 *)((u8 *)w + 0x6F4 + cur * 4);
    ((u8 *)w)[0x73C] = (cur + 1) % 4;
    out->packed = posBits;
    out->packed += attrBits << 24;
    out->packed += cur << 28;
    {
        u32 *p = &out->packed;
        if (((u8 *)w)[0x664] == 2) {
            *p += 1 << 30;
        }
    }
    if (((u8 *)w)[0x6B0] != 0) {
        out->packed = out->packed - (2 << 30);
    }
    out->flags = 0;
    out->flags += flagBits;
    out->flags += ((u8 *)w)[0x73E] << 24;
    out->flags += val << 25;
}
