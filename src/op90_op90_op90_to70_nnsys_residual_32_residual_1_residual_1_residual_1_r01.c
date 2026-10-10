#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 word;
} Oam;

typedef struct {
    u32 unused;
    int affineMode;
} RenderMode;

extern RenderMode *_021DA498;
extern u16 _02109310[3][4];
extern u16 _021092F8[3][4];

typedef struct {
    int x, y;
} Vector;

typedef MtxFx22 Matrix;

static inline void Transform(const Vector *in, const Matrix *m, Vector *out) {
    Vector temporary;
    Vector *dest = out;
    if (in == out) {
        dest = &temporary;
    }
    dest->x = (s32)(((u64)((s64)m->_10 * in->y) + 0x1000 + (u64)((s64)m->_00 * in->x)) >> 12);
    dest->y = (s32)(((u64)((s64)m->_11 * in->y) + 0x1000 + (u64)((s64)m->_01 * in->x)) >> 12);
    if (dest == &temporary) {
        *out = temporary;
    }
}

static inline u32 Effect(Oam *o) {
    u32 mode = o->word & 0x300;
    if (mode != 0x100 && mode != 0x300) {
        mode |= o->word & 0x30000000;
    }
    return mode;
}

static inline u32 DoubleMode(u32 mode) {
    return mode == 0x300;
}

static inline int Double(Oam *o) {
    return Effect(o) == 0x300;
}

void DoAffineTransforme_(Matrix *, Oam *, Vector *);

static inline void SetEffect(Oam *o, u32 effect) {
    if (effect != 0x100) {
        o->word = (o->word & 0xc1fffcff) | effect;
        return;
    }
    o->word &= 0xc1fffcff;
    o->word |= effect;
}

static inline int CenterX(Matrix *m, int hw, int hh, int wx) {
    u32 dx = 0 - (u32)wx;
    dx += (u32)m->_00 * (u32)hw;
    u32 tail = (u32)m->_10 * (u32)hh;
    return (s32)(tail + dx);
}

static inline int CenterY(Matrix *m, int hw, int hh, int hy) {
    u32 dy = 0 - (u32)hy;
    dy += (u32)m->_01 * (u32)hw;
    u32 tail = (u32)m->_11 * (u32)hh;
    return (s32)(tail + dy);
}

void DoAffineTransforme_(Matrix *m, Oam *o, Vector *offset) {
    u32 finalMode;
    Vector v;
    int x;
    int y;
    u32 word;
    u32 packed;
    int shape;
    u32 size;
    int mode;
    u32 effect;
    int hw;
    int hh;
    int wx;
    int hy;
    x = (s16)((o->word & 0x1ff0000) >> 16);
    if (x > 255) {
        x = (s16)(x | 0xff00);
    }
    v.x = x << 12;
    y = o->word & 255;
    if (y > 127) {
        y = (s16)(y | 0xff00);
    }
    v.y = y << 12;
    if (Effect(o) == 0x300) {
        word = o->word;
        finalMode = word & 0x300;
        if (finalMode != 0x100 && finalMode != 0x300) {
            finalMode |= word & 0x30000000;
        }
        packed = word & 0xc000c000;
        shape = (int)(packed & 0xc000) >> 14;
        size = (packed & 0xc0000000) >> 30;
        v.x += _02109310[shape][size] << 11;
        v.y += _021092F8[shape][size] << 11;
    }
    Transform(&v, m, &v);
    mode = _021DA498->affineMode;
    if (mode) {
        effect = mode == 2 ? 0x300 : 0x100;
        SetEffect(o, effect);
    }
    word = o->word;
    finalMode = word & 0x300;
    if (finalMode != 0x100 && finalMode != 0x300) {
        finalMode |= word & 0x30000000;
    }
    packed = word & 0xc000c000;
    shape = (int)(packed & 0xc000) >> 14;
    size = (packed & 0xc0000000) >> 30;
    hw = _02109310[shape][size] >> 1;
    hh = _021092F8[shape][size] >> 1;
    wx = hw << 12;
    hy = hh << 12;
    v.x += -wx + m->_00 * hw + m->_10 * hh;
    v.y += -hy + m->_01 * hw + m->_11 * hh;
    if (DoubleMode(finalMode)) {
        v.x -= wx;
        v.y -= hy;
    }
    v.x += offset->x;
    v.y += offset->y;
    o->word = (o->word & 0xfe00ff00) | ((v.y >> 12) & 255) | (((u32)(v.x >> 12) << 23) >> 7);
}
