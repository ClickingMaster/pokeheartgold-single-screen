#include "global.h"

#include "sprite_system.h"
#include "unk_02091054.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov57_02238958(void *work);

void ov57_02238958(void *work) {
    SpriteSystem *system = PTR(work, 0xDC);
    int i;
    SpriteManager *manager = PTR(work, 0xE0);
    PaletteData *palette = PTR(work, 0xE8);

    for (i = 0; i < 8; i++) {
        SpriteSystem_LoadCharResObj(system, manager, (NarcId)0x57, sub_02091054(*(u8 *)((u32)work + i + 0x450)), TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0x61A8 + i);
    }
    SpriteSystem_LoadPaletteBuffer(palette, PLTTBUF_SUB_OBJ, system, manager, (NarcId)0x57, 6, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0x6597);
    SpriteSystem_LoadCellResObj(system, manager, (NarcId)0x57, 0x25, TRUE, 0x699D);
    SpriteSystem_LoadAnimResObj(system, manager, (NarcId)0x57, 0x23, TRUE, 0x6D83);
}
