#ifndef OV59_R11_PRIVATE_H
#define OV59_R11_PRIVATE_H
#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
typedef struct Ov59R11Work {
    u8 pad[0x258];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    Sprite *sprites[34];
} Ov59R11Work;
extern const u32 ov59_0223C99C[];
extern const UnmanagedSpriteTemplate ov59_0223CA90[];
extern const UnmanagedSpriteTemplate ov59_0223CB08;
extern const UnmanagedSpriteTemplate ov59_0223CB30;
void ov59_0223B8E4(Ov59R11Work *work);
#endif
