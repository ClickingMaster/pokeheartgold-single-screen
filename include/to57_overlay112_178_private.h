#ifndef TO57_NEW42_PRIVATE_H
#define TO57_NEW42_PRIVATE_H

#include "global.h"

#include "sprite_system.h"

typedef struct Ov112Work178 {
    u8 pad00[0x10];
    NARC *narc;
    u8 pad14[0x54];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprite70;
    ManagedSprite *sprite74;
    ManagedSprite *sprite78;
    ManagedSprite *sprite7C;
    ManagedSprite *sprite80;
    ManagedSprite *sprite84;
    ManagedSprite *sprite88;
    ManagedSprite *sprite8C;
    ManagedSprite *sprite90;
    ManagedSprite *sprite94;
    u8 pad98[0xA5];
    u8 value13D;
    u8 limit13E;
} Ov112Work178;

extern const ManagedSpriteTemplate ov112_021FF398;
extern const ManagedSpriteTemplate ov112_021FF3CC;

void ov112_021F1F80(Ov112Work178 *work);
void ov112_021F1CC8(Ov112Work178 *work, int arg);
void ov112_021F1A40(Ov112Work178 *work);
ManagedSprite *ov112_021F1AF4(SpriteSystem *, SpriteManager *, u16, u16, u8, u8);
ManagedSprite *ov112_021F1B44(SpriteSystem *, SpriteManager *, u16, u16, u8, u8);
void ov112_021F1B94(Ov112Work178 *work);

#endif
