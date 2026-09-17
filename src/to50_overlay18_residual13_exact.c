#include "global.h"

#include "sprite_system.h"

#define PTR(p, off, T) (*(T **)((u8 *)(p) + (off)))

void ov18_021F13DC(void *, u32);
void ov18_021F1324(void *, u32);
void ov18_021F2648(void *);
void ov18_021F2270(void *);
void ov18_021F26E4(void *);
void ov18_021F2308(void *);
void ov18_021F310C(void *);
void ov18_021F2F4C(void *);

void ov18_021F2F4C(void *p) {
    ov18_021F1324(p, 60);
    ov18_021F2648(p);
    ov18_021F2270(p);

    SpriteSystem_LoadCharResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x48, TRUE, 1, 0xC5A0);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(PTR(p, 0x850, PaletteData), (PaletteBufferId)2, PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x4B, FALSE, 1, 1, 0xC561);
    SpriteSystem_LoadCellResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x49, TRUE, 0xC55E);
    SpriteSystem_LoadAnimResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x4A, TRUE, 0xC55E);

    SpriteSystem_LoadCharResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x48, TRUE, 2, 0xC59F);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(PTR(p, 0x850, PaletteData), PLTTBUF_SUB_OBJ, PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x4B, FALSE, 1, 2, 0xC560);
    SpriteSystem_LoadCellResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x49, TRUE, 0xC55D);
    SpriteSystem_LoadAnimResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x4A, TRUE, 0xC55D);

    SpriteSystem_LoadCharResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x17, TRUE, 2, 0xC59E);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(PTR(p, 0x850, PaletteData), PLTTBUF_SUB_OBJ, PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x20, FALSE, 1, 2, 0xC55F);
    SpriteSystem_LoadCellResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x18, TRUE, 0xC55C);
    SpriteSystem_LoadAnimResObjFromOpenNarc(PTR(p, 0x668, SpriteSystem), PTR(p, 0x66C, SpriteManager), PTR(p, 0x854, NARC), 0x19, TRUE, 0xC55C);
}

void ov18_021F310C(void *p) {
    ov18_021F13DC(p, 60);
    ov18_021F26E4(p);
    ov18_021F2308(p);
    SpriteManager_UnloadCharObjById(PTR(p, 0x66C, SpriteManager), 0xC59F);
    SpriteManager_UnloadPlttObjById(PTR(p, 0x66C, SpriteManager), 0xC560);
    SpriteManager_UnloadCellObjById(PTR(p, 0x66C, SpriteManager), 0xC55D);
    SpriteManager_UnloadAnimObjById(PTR(p, 0x66C, SpriteManager), 0xC55D);
    SpriteManager_UnloadCharObjById(PTR(p, 0x66C, SpriteManager), 0xC5A0);
    SpriteManager_UnloadPlttObjById(PTR(p, 0x66C, SpriteManager), 0xC561);
    SpriteManager_UnloadCellObjById(PTR(p, 0x66C, SpriteManager), 0xC55E);
    SpriteManager_UnloadAnimObjById(PTR(p, 0x66C, SpriteManager), 0xC55E);
}
