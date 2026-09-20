#include "global.h"

#include "sprite_system.h"

#define P(p, o) (*(void **)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef struct Ov40Work {
    u32 mode;
    u8 pad004[0x10];
    NARC *narc;
    SpriteSystem *system;
    SpriteManager *manager;
    u8 pad020[8];
    PaletteData *palette;
} Ov40Work;

extern int ov40_0222DB30(void *, int);

void ov40_0222C750(void *work);

void ov40_0222C750(void *work) {
    Ov40Work *w = work;
    NARC *narc;
    SpriteSystem *system;
    SpriteManager *manager;
    PaletteData *palette;

    narc = w->narc;
    system = w->system;
    manager = w->manager;
    palette = w->palette;

    if (w->mode == 0) {
        SpriteSystem_LoadPaletteBufferFromOpenNarc(palette, (PaletteBufferId)2, system, manager, narc, ov40_0222DB30(work, 2), 0, 3, 1, 0x270f);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(palette, (PaletteBufferId)3, system, manager, narc, ov40_0222DB30(work, 2), 0, 3, 2, 0x2710);
    } else {
        SpriteSystem_LoadPaletteBufferFromOpenNarc(palette, (PaletteBufferId)2, system, manager, narc, 0x33, 0, 3, 1, 0x270f);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(palette, (PaletteBufferId)3, system, manager, narc, 0x33, 0, 3, 2, 0x2710);
    }
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, narc, 0x41, 0, 1, 0x270f);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, narc, 0x41, 0, 2, 0x2710);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, narc, 0x2e, 0, 0x270f);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, narc, 0x2f, 0, 0x270f);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, narc, 0x2e, 0, 0x2710);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, narc, 0x2f, 0, 0x2710);
}
