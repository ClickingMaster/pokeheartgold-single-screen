#pragma force_active on
#include "global.h"

typedef struct Ov41Unk {
    u8 unk0[4];
    u8 unk4[0xC];
    void *unk10;
    u8 unk14[0xC];
    void *unk20;
    u8 unk24[0x54];
    u8 unk78[4];
} Ov41Unk;

typedef struct Ov41Ctx {
    u8 unk0[0x1D8];
    void *unk1D8;
} Ov41Ctx;

void ov41_02249B44(void *obj, int *x, int *y);
void ov41_02249B94(void *obj, int *x, int *y);
void ov41_02249BAC(void *obj, int *a, int *b, int *c, int *d);
void ov41_02247E34(int y, int dy, int x, int dx, int index, int *outX, int *outY);
int ov41_02249820(void *a0, int x, int y, void *a3);
void ov41_02249A50(void *obj, void *a1);
void ov41_02248324(Ov41Unk *data, void *obj, BOOL flag, Ov41Ctx *ctx);

#pragma opt_propagation off
// @fn ov41_02248324
void ov41_02248324(Ov41Unk *data, void *obj, BOOL flag, Ov41Ctx *ctx) {
    int x0, y0;
    int x1, y1;
    int a, c;
    int outX, outY;
    int b, d;
    Ov41Unk *area;
    int cy;
    int sy;
    void *ptr;
    int dx;
    int dy;
    int sx;
    int ax;
    int i;
    int hit;

    ov41_02249B44(obj, &x0, &y0);
    ov41_02249B94(obj, &x1, &y1);
    ov41_02249BAC(obj, &a, &b, &c, &d);
    ptr = ctx->unk1D8;
    hit = 0;
    sx = x0;
    ax = a;
    dx = sx + x1 - b;
    sy = y0;
    dy = sy + y1 - d;
    cy = c;
    area = data;
    i = 0;
    do {
        ov41_02247E34(sy + cy, dy, sx + ax, dx, i, &outX, &outY);
        hit |= ov41_02249820(area->unk78, outX, outY, ptr);
    } while (++i < 16);
    if (hit) {
        if (flag) {
            ov41_02249A50(obj, data->unk14);
        } else {
            ov41_02249A50(obj, data->unk20);
        }
    } else {
        if (flag) {
            ov41_02249A50(obj, data->unk4);
        } else {
            ov41_02249A50(obj, data->unk10);
        }
    }
}
#pragma opt_propagation reset
