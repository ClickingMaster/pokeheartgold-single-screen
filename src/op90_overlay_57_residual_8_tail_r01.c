#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "touchscreen.h"
typedef struct Pos57 {
    s16 x;
    s16 y;
} Pos57;
typedef struct PosTable57 {
    Pos57 pos[13];
} PosTable57;
typedef struct Work57 {
    u8 unk000[0xDC];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u8 unkE4[0x10C];
    TouchscreenHitbox hitboxes[21];
    u8 unk244[0x1D0];
    ManagedSprite *sprites[13];
} Work57;
extern const PosTable57 ov57_0223BE48;
extern const PosTable57 ov57_0223BE7C;
void ov57_022385A4(TouchscreenHitbox *hitbox, ManagedSprite *sprite, s16 x, s16 y);
void ov57_02239EB4(Work57 *work);
// @fn ov57_02239EB4
void ov57_02239EB4(Work57 *work) {
    ManagedSpriteTemplate template;
    PosTable57 positions;
    PosTable57 hitPositions;
    int i;
    SpriteSystem *spriteSystem = work->spriteSystem;
    ManagedSprite **sprites;
    SpriteManager *spriteManager = work->spriteManager;
    template.x = 0;
    template.y = 0;
    template.z = 0;
    sprites = work->sprites;
    template.animation = 0;
    template.vramTransfer = 0;
    template.drawPriority = 60;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.bgPriority = 1;
    template.pal = 1;
    template.resIdList[4] = -1;
    template.resIdList[0] = 25026;
    template.resIdList[5] = -1;
    template.resIdList[1] = 26004;
    template.resIdList[2] = 27028;
    template.resIdList[3] = 28027;
    sprites[8] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    template.resIdList[0] = 25029;
    template.resIdList[1] = 26004;
    template.resIdList[2] = 27031;
    template.resIdList[3] = 28030;
    sprites[9] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    template.bgPriority = 1;
    template.pal = 1;
    template.resIdList[0] = 35023;
    template.resIdList[1] = 26004;
    template.resIdList[2] = 27025;
    template.resIdList[3] = 28024;
    sprites[10] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    template.bgPriority = 1;
    template.pal = 0;
    template.resIdList[0] = 25020;
    template.resIdList[1] = 26004;
    template.resIdList[2] = 27022;
    template.resIdList[3] = 28021;
    sprites[11] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    sprites[12] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    positions = ov57_0223BE48;
    hitPositions = ov57_0223BE7C;
    for (i = 8; i < 13; i++) {
        ManagedSprite_SetPositionXY(work->sprites[i], positions.pos[i].x, positions.pos[i].y);
        ManagedSprite_TickFrame(work->sprites[i]);
        ManagedSprite_SetAnimationFrame(work->sprites[i], 0);
        ov57_022385A4(&work->hitboxes[i], work->sprites[i], hitPositions.pos[i].x, hitPositions.pos[i].y);
    }
}
