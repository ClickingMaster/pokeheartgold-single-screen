#pragma force_active on
#include "global.h"
#include "sprite_system.h"

extern const ManagedSpriteTemplate ov96_0221D17C;

ManagedSprite *ov96_022104D8(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u16 animation, u16 priority);

// @fn ov96_022104D8
ManagedSprite *ov96_022104D8(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u16 animation, u16 priority) {
    ManagedSpriteTemplate template = ov96_0221D17C;
    ManagedSprite *sprite;

    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template.x = x;
    template.y = y;
    template.animation = animation;
    template.drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, 480 * FX32_ONE);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}
