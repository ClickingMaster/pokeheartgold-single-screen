#include "global.h"

#include "sprite_system.h"

NarcId ov18_021E5900(void);
int ov18_021E5908(void);
void ov18_021F1CB4(void *p);

void ov18_021F1CB4(void *p) {
    NarcId narcId = ov18_021E5900();

    SpriteSystem_LoadCharResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 77, TRUE, 2, 0xC599);
    SpriteSystem_LoadPaletteBuffer(*(PaletteData **)((u8 *)p + 0x850), (PaletteBufferId)3, *(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), narcId, ov18_021E5908(), FALSE, 1, 2, 0xC55B);
    SpriteSystem_LoadCellResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 78, TRUE, 0xC558);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 79, TRUE, 0xC558);
}
