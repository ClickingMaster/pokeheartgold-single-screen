#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef union {
    u32 word;

    struct {
        u16 low, high;
    } half;
} Attr;

typedef struct {
    u8 pad[0x38];
    u32 attr1;
    Attr attr2;
} Core;
typedef struct Surface Surface;
typedef struct Renderer Renderer;

struct Surface {
    u8 pad[0x40];
    void (*callback)(Renderer *, Surface *, int, int, void *);
};

struct Renderer {
    u8 pad[0x74];
    Surface *surface;
    u8 gap[12];
    volatile u32 flags;
    u16 priority, palette;
    u32 mode, mosaic;
    u16 offset;
};

extern struct {
    u16 count, pad;
    u32 type;
    Renderer *renderer;
    int index;
} _021D94D8;

extern u32 _021D9668[][6];
extern u32 _021D9678[][6];
extern u32 _021D967C[][6];
extern u32 _021109E4[];

extern struct {
    u32 pad[6];
    u32 x, y;
} _021109DC;

void *NNS_G2dGetRendererPaletteTbl(Renderer *);
u16 NNS_G2dGetPaletteTableValue(void *, u16);
void RndCoreCBFuncBeforeOBJ_(Core *, int, int);

void RndCoreCBFuncBeforeOBJ_(Core *c, int arg1, int arg2) {
    void *table = NNS_G2dGetRendererPaletteTbl(_021D94D8.renderer);
    if (table) {
        u16 mapped = NNS_G2dGetPaletteTableValue(table, (c->attr2.half.low & 0xf000) >> 12);
        c->attr2.word = (c->attr2.word & ~0xf000) | ((mapped & 15) << 12);
    }
    if (_021D94D8.renderer->flags) {
        if (_021D94D8.renderer->flags & 1) {
            c->attr2.word = (c->attr2.word & ~0xc00) | ((_021D94D8.renderer->priority & 3) << 10);
        }
        if (_021D94D8.renderer->flags & 2) {
            c->attr2.word = (c->attr2.word & ~0xf000) | ((_021D94D8.renderer->palette & 15) << 12);
        }
        if (_021D94D8.renderer->flags & 16) {
            u16 offset = _021D94D8.renderer->offset;
            u32 attr = c->attr2.word;
            u32 packed = attr << 16;
            c->attr2.word = (attr & ~0xf000) | ((((packed >> 28) + offset) & 15) << 12);
        }
        if (_021D94D8.renderer->flags & 4) {
            if (_021D94D8.renderer->mosaic) {
                c->attr1 |= 0x1000;
            } else {
                c->attr1 &= ~0x1000;
            }
        }
        if (_021D94D8.renderer->flags & 8) {
            u32 oldAttr = c->attr1;
            int pal = (c->attr2.half.low & 0xf000) >> 12;
            u32 mode = _021D94D8.renderer->mode;
            c->attr1 = (mode << 10) | (oldAttr & ~0xc00);
            c->attr2.half.low = (c->attr2.half.low & ~0xf000) | (pal << 12);
        }
    }
    {
        Renderer *r = _021D94D8.renderer;
        Surface *s = r->surface;
        if (s->callback) {
            void *m;
            if (_021D94D8.type == 0) {
                m = _021D9668[_021D94D8.index];
            } else {
                _021109DC.x = _021D9678[_021D94D8.index][0];
                _021109DC.y = _021D967C[_021D94D8.index][0];
                m = _021109E4;
            }
            s->callback(r, s, arg1, arg2, m);
        }
    }
}
