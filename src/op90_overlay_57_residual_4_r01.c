#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct HitboxEntry {
    int active;
    u8 type;
    u8 x;
    u8 y;
    u8 unk07;
    ManagedSprite *sprite;
    u8 unk0C[4];
} HitboxEntry;
typedef struct Ov57Work {
    u8 unk000[0xDC];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u8 unkE4[0x268];
    HitboxEntry entries[8];
} Ov57Work;
void ov57_0223809C(Ov57Work *work, u8 type, u8 index);
void sub_02091054(u8 type);
BOOL ov57_02238260(Ov57Work *work, u8 index);

// @fn ov57_02238260
BOOL ov57_02238260(Ov57Work *work, u8 index) {
    ManagedSpriteTemplate template;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    HitboxEntry *entry = &work->entries[index];
    if (work->entries[index].active == 1) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    ov57_0223809C(work, entry->type, index);
    template.x = entry->x;
    template.y = entry->y;
    template.z = 0;
    template.animation = 0;
    template.pal = 0;
    template.vramTransfer = 0;
    template.drawPriority = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.bgPriority = 1;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    sub_02091054(entry->type);
    template.resIdList[0] = 20000 + index;
    template.resIdList[1] = 21006;
    template.resIdList[2] = 22038;
    template.resIdList[3] = 23036;
    entry->sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    ManagedSprite_TickFrame(entry->sprite);
    return TRUE;
}
