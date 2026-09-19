#include "private.h"

void ov40_02243C54(u8 *w) {
    int special = 0;
    void *system = PTR(w, 0x224);
    void *manager = PTR(w, 0x228);
    R41SpriteTemplate template;
    int i;
    u8 *specialEntry;
    u8 *entry;
    int y;

    template.x = 0x130;
    template.y = 0xE0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 10;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.pal = 0;
    template.vram = 1;
    template.resIdList[0] = 0x3E8;
    template.resIdList[1] = 0x3E8;
    template.resIdList[2] = 0x3E8;
    template.resIdList[3] = 0x3E8;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;

    i = 0;
    if ((int)WORD(w, 0x204) + 2 > 0) {
        specialEntry = w;
        y = 0x4C;
        entry = w;
        do {
            if (i == (int)special + WORD(specialEntry, 0x150) + 1) {
                PTR(specialEntry, 0x15C) = SpriteSystem_NewSprite(system, manager, &template);
                ManagedSprite_SetDrawFlag(PTR(specialEntry, 0x15C), FALSE);
                ManagedSprite_SetPositionXY(PTR(specialEntry, 0x15C), y, 0x18);
                ManagedSprite_SetAnim(PTR(specialEntry, 0x15C), 0x16);
                ManagedSprite_TickFrame(PTR(specialEntry, 0x15C));
                specialEntry += 0x1C;
                special++;
            } else {
                PTR(entry, 0xC) = SpriteSystem_NewSprite(system, manager, &template);
                ManagedSprite_SetDrawFlag(PTR(entry, 0xC), FALSE);
                ManagedSprite_SetPositionXY(PTR(entry, 0xC), y, 0x18);
                ManagedSprite_SetAnim(PTR(entry, 0xC), ov40_02244054(WORD(entry, 0), WORD(entry, 8)));
                ManagedSprite_SetAffineOverwriteMode(PTR(entry, 0xC), 0);
                ManagedSprite_TickFrame(PTR(entry, 0xC));
                entry += 0x1C;
            }
            i++;
            y += 8;
        } while (i < (int)WORD(w, 0x204) + 2);
    }
}
