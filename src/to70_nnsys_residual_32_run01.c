#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

struct RawOam {
    u16 a, b, c;
};

struct ScratchCell {
    u16 n, pad;
    struct RawOam *oams;
};

struct Surface {
    u8 pad[0x20];
    void (*before)(void *, struct ScratchCell *, u16);
    void (*after)(void *, struct ScratchCell *, u16);
};

struct Renderer {
    u8 pad[0x30];
    int draw;
    u32 pad34;
    u16 oam[3];
};
extern struct Renderer *_021DA498;
extern void DrawOamToSurface3D_(u16 *);
extern void DrawCellToSurface3D_(struct Surface *, struct ScratchCell *);

void DrawCellToSurface3D_(struct Surface *surface, struct ScratchCell *cell) {
    struct Renderer *r = _021DA498;
    u16 i = 0;
    for (; i < cell->n; i++) {
        struct RawOam *p;
        _021DA498->draw = 1;
        p = &cell->oams[i];
        r->oam[0] = p->a;
        r->oam[1] = p->b;
        r->oam[2] = p->c;
        if (surface->before) {
            surface->before(_021DA498, cell, i);
        }
        if (_021DA498->draw) {
            DrawOamToSurface3D_(r->oam);
        }
        if (surface->after) {
            surface->after(_021DA498, cell, i);
        }
    }
}
