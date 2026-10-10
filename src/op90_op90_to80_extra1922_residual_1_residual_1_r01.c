#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    void *info;
    u16 (*decode)(const void **);
} Fnt;

typedef struct {
    void *canvas;
    Fnt *font;
    int charSpace, lineSpace;
} TC;

typedef struct {
    s8 x, y;
} Dir;

typedef struct {
    int color;
    const void *pos;
} DrawState;
struct Canvas;
struct Font;
int NNS_G2dCharCanvasDrawChar(struct Canvas *, struct Font *, int, int, int, u16);
void NNSi_G2dTextCanvasDrawString(TC *, int, int, int, const void *, const void **, Dir);

void NNSi_G2dTextCanvasDrawString(TC *tc, int x, int y, int color, const void *text, const void **next, Dir dir) {
    const void *pos;
    int spacing;
    Fnt *font;
    u16 (*decode)(const void **);
    int w;
    u16 ch;
    int dx;
    int dy;
    spacing = tc->charSpace;
    font = tc->font;
    pos = text;
    dy = dir.y;
    decode = font->decode;
    ch = decode(&pos);
    if (ch != 0) {
        dx = dir.x;
        do {
            if (ch == '\n') {
                break;
            }
            w = spacing + NNS_G2dCharCanvasDrawChar((struct Canvas *)tc->canvas, (struct Font *)font, x, y, color, ch);
            x += w * dx;
            y += w * dy;
            ch = decode(&pos);
        } while (ch != 0);
    }
    if (next) {
        *next = (ch == '\n') ? pos : NULL;
    }
}
