#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Width {
    s8 left;
    u8 width;
    s8 advance;
};

struct Glyph {
    u8 width, height;
    u16 cell;
    u8 pad[3];
    u8 rotation;
    u8 image[1];
};

struct FontData {
    u16 a, defaultIndex;
    u32 b;
    struct Glyph *glyph;
};

struct Font {
    struct FontData *data;
};

struct Char {
    struct Width *width;
    void *image;
};
struct Canvas;

struct Ops {
    void (*draw)(struct Canvas *, struct Font *, int, int, int, struct Char *);
};

struct Canvas {
    u8 p[0x14];
    struct Ops *ops;
};

extern u16 NNS_G2dFontFindGlyphIndex(struct Font *, u16);
extern struct Width *NNS_G2dFontGetCharWidthsFromIndex(struct Font *, u16);
int NNS_G2dCharCanvasDrawChar(struct Canvas *, struct Font *, int, int, int, u16);

int NNS_G2dCharCanvasDrawChar(struct Canvas *c, struct Font *f, int x, int y, int color, u16 code) {
    u16 idx = NNS_G2dFontFindGlyphIndex(f, code);
    struct Char ch;
    if (idx == 0xFFFF) {
        idx = f->data->defaultIndex;
    }
    ch.width = NNS_G2dFontGetCharWidthsFromIndex(f, idx);
    ch.image = f->data->glyph->image + idx * f->data->glyph->cell;
    switch (f->data->glyph->rotation) {
    case 0:
    case 7:
        x += ch.width->left;
        break;
    case 1:
    case 2:
        x -= f->data->glyph->width;
        y += ch.width->left;
        break;
    case 3:
    case 4:
        x -= ch.width->left + ch.width->width;
        y -= f->data->glyph->height;
        break;
    case 5:
    case 6:
        y -= ch.width->left + f->data->glyph->height;
        break;
    }
    c->ops->draw(c, f, x, y, color, &ch);
    return ch.width->advance;
}
