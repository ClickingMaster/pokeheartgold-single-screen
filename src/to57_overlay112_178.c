#include "to57_overlay112_178_private.h"

void ov112_021F1A40(Ov112Work178 *work) {
    SpriteSystem *system = work->spriteSystem;
    SpriteManager *manager = work->spriteManager;

    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 12, FALSE, 12, 1, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 13, TRUE, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 15, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 14, TRUE, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 16, FALSE, 1, 1, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 17, TRUE, 1, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 19, TRUE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 18, TRUE, 1);
}

ManagedSprite *ov112_021F1AF4(SpriteSystem *system, SpriteManager *manager, u16 x, u16 y, u8 animation, u8 priority) {
    ManagedSpriteTemplate template = ov112_021FF398;
    ManagedSprite *sprite;

    template.x = x;
    template.y = y;
    template.drawPriority = priority;
    template.animation = animation;
    sprite = SpriteSystem_NewSpriteWithYOffset(system, manager, &template, 0x20C000);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

ManagedSprite *ov112_021F1B44(SpriteSystem *system, SpriteManager *manager, u16 x, u16 y, u8 animation, u8 priority) {
    ManagedSpriteTemplate template = ov112_021FF3CC;
    ManagedSprite *sprite;

    template.x = x;
    template.y = y;
    template.drawPriority = priority;
    template.animation = animation;
    sprite = SpriteSystem_NewSpriteWithYOffset(system, manager, &template, 0x20C000);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

void ov112_021F1B94(Ov112Work178 *work) {
    work->sprite70 = ov112_021F1AF4(work->spriteSystem, work->spriteManager, 0x20, 0xB0, 8, 0);
    work->sprite74 = ov112_021F1AF4(work->spriteSystem, work->spriteManager, 0x50, 0xB0, 9, 0);
    work->sprite78 = ov112_021F1AF4(work->spriteSystem, work->spriteManager, 0xD8, 0xB0, 10, 0);
    work->sprite7C = ov112_021F1AF4(work->spriteSystem, work->spriteManager, 0x80, 0x44, 0, 5);
    work->sprite80 = ov112_021F1AF4(work->spriteSystem, work->spriteManager, 0x68, 0x38, 12, 2);
    work->sprite84 = ov112_021F1AF4(work->spriteSystem, work->spriteManager, 0x98, 0x38, 12, 1);
    work->sprite94 = ov112_021F1AF4(work->spriteSystem, work->spriteManager, 0x68, 0x28, 26, 2);
    work->sprite88 = ov112_021F1B44(work->spriteSystem, work->spriteManager, 0x68, 0x28, 8, 2);
    work->sprite8C = ov112_021F1B44(work->spriteSystem, work->spriteManager, 0x98, 0x28, 8, 2);
    work->sprite90 = ov112_021F1B44(work->spriteSystem, work->spriteManager, 0x80, 0x28, 8, 2);
    ov112_021F1F80(work);
    ov112_021F1CC8(work, 0);
    ManagedSprite_SetDrawFlag(work->sprite70, FALSE);
    ManagedSprite_SetDrawFlag(work->sprite74, work->value13D < work->limit13E - 1);
}
