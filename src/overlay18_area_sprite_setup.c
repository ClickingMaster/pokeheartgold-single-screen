#include "global.h"

#include "sprite_system.h"

extern const ManagedSpriteTemplate ov18_021FA984[];
extern const ManagedSpriteTemplate ov18_021FAB58;
extern const ManagedSpriteTemplate ov18_021FAB8C;
void ov18_021F299C(void *p);

void ov18_021F299C(void *p) {
    const ManagedSpriteTemplate *tmpl = ov18_021FA984;
    u32 i = 0;
    u8 *dest = (u8 *)p;
    for (; i <= 7; i++) {
        *(ManagedSprite **)(dest + 0x670) = SpriteSystem_NewSprite(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), tmpl);
        tmpl++;
        dest += 4;
    }
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)p + 0x688), FALSE);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)p + 0x68C), FALSE);
    *(ManagedSprite **)((u8 *)p + 0x694) = SpriteSystem_NewSpriteWithYOffset(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), &ov18_021FAB58, 0x200000);
    *(ManagedSprite **)((u8 *)p + 0x698) = SpriteSystem_NewSpriteWithYOffset(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), &ov18_021FAB8C, 0x200000);
}
