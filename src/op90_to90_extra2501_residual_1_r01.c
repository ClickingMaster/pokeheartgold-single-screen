#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad0[0x10];
    u8 value;
    u8 pad11[0x0b];
} Sub;
typedef struct {
    u8 pad0[0x20];
    Sub subs[3];
    u8 pad74[8];
    fx32 x;
    fx32 y;
    u8 pad84[0x8b - 0x84];
    u8 x8b;
    u8 pad8c[0x9c - 0x8c];
    u8 x9c;
    u8 x9d;
    u8 x9e;
    u8 pad9f[3];
    u8 xa2;
    u8 padA3[0xd1 - 0xa3];
    u8 xd1;
    u8 padD2[2];
} Ent;
typedef struct {
    u8 pad0[0x30];
    Ent ents[4];
    u8 pad380[0x3c0 - 0x380];
    u8 x3c0;
    u8 pad3c1[0x63c - 0x3c1];
    u8 x63c;
    u8 x63d;
    u16 x63e;
    u16 x640;
    u8 pad642[2];
    u8 x644[4];
} Work;
typedef struct {
    u16 x[4];
    u8 y[4];
    u8 xc[4];
    u8 x10[4];
    u32 bits;
    u16 x18;
    u16 x1a;
    u8 x1c;
    u8 x1d;
    u8 x1e[4];
} Out;
void ov96_021FDA30(Work *work, Out *out);
// @fn ov96_021FDA30
void ov96_021FDA30(Work *work, Out *out) {
    int i;
    int j;
    u32 bits;
    u8 flags;
    u8 mask;
    flags = 0;
    bits = 0;
    mask = 0;
    for (i = 0; i < 4; i++) {
        Ent *ent = &work->ents[i];
        u8 a = 0;
        int b;
        a += ent->x8b;
        a += ent->xa2 * 4;
        flags += ent->x9e << (i * 2);
        b = ent->x9d * 128;
        b += ent->x9c;
        out->y[i] = ent->y / FX32_ONE;
        out->x[i] = ent->x / FX32_ONE;
        out->xc[i] = a;
        out->x10[i] = b;
        for (j = 0; j < 3; j++) {
            bits += ent->subs[j].value << ((j + i * 3) * 2);
        }
        out->x1e[i] = work->x644[i];
        mask += ent->xd1 << i;
    }
    out->x1d = flags;
    out->bits = bits;
    out->bits += work->x3c0 << 24;
    out->bits += work->x63c << 25;
    out->bits += mask << 26;
    out->x1a = work->x63e;
    out->x1c = work->x63d;
    out->x18 = work->x640;
}
