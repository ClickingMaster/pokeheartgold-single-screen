#include "to57_overlay96_r36_sprite_row_private.h"

void ov96_021EC68C(void *raw) {
    Ov96R36SpriteTemplate template = { 0 };
    int i;
    int x = 0x30;
    int palette = 2;

    for (i = 0; i < 3; i++) {
        void *sprite;
        template.resources[0] = 0x6d + i;
        template.resources[1] = 0x6a;
        template.resources[2] = 0x68;
        template.resources[3] = 0x68;
        template.vram = 1;
        template.x = x;
        template.y = 0x70;
        template.bgPriority = 0;
        template.drawPriority = 1;
        {
            void *slot = (u8 *)raw + (0x15 + i) * 4;
            sprite = SpriteSystem_NewSpriteWithYOffset(PTR(raw, 0x18), PTR(raw, 0x1c), &template, 0x20c000);
            PTR(slot, 0x20) = sprite;
            ManagedSprite_SetPaletteOverride(sprite, palette);
            ManagedSprite_SetDrawFlag(PTR(slot, 0x20), 0);
        }
        palette += 3;
        x += 0x50;
    }
}
