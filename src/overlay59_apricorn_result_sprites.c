#include "overlay59_apricorn_result_sprites_private.h"
#define SPRITE_AT(base) (*(Sprite **)((u8 *)(base) + 0x260))

void ov59_0223B8E4(Ov59R11Work *work) {
    int i;
    int j;
    const u32 *order;
    int yMain;
    int ySub;
    int x;
    u8 *slot;
    u8 *cursor = (u8 *)work;
    const UnmanagedSpriteTemplate *template = ov59_0223CA90;

    i = 0;
    cursor = (u8 *)work;
    do {
        SPRITE_AT(cursor) = SpriteSystem_CreateSpriteFromResourceHeader(work->spriteSystem, work->spriteManager, template);
        Sprite_SetDrawFlag(SPRITE_AT(cursor), TRUE);
        Sprite_SetAnimActiveFlag(SPRITE_AT(cursor), TRUE);
        Sprite_SetPriority(SPRITE_AT(cursor), 1);
        template++;
        cursor += 4;
        i++;
    } while (i <= 2);
    Sprite_SetDrawFlag(work->sprites[2], FALSE);
    Sprite_SetAffineOverwriteMode(work->sprites[2], 2);
    i = 0;
    order = ov59_0223C99C;
    yMain = 40;
    ySub = 56;
    do {
        slot = (u8 *)work + (u8)(*order + 3) * 4;
        SPRITE_AT(slot) = SpriteSystem_CreateSpriteFromResourceHeader(work->spriteSystem, work->spriteManager, &ov59_0223CB08);
        Sprite_SetPositionXY(SPRITE_AT(slot), 80, (s16)yMain);
        Sprite_SetAnimActiveFlag(SPRITE_AT(slot), TRUE);
        Sprite_SetDrawFlag(SPRITE_AT(slot), TRUE);
        j = 0;
        x = 64;
        do {
            slot = (u8 *)work + (u8)(*order * 5 + 8 + j) * 4;
            SPRITE_AT(slot) = SpriteSystem_CreateSpriteFromResourceHeader(work->spriteSystem, work->spriteManager, &ov59_0223CB30);
            Sprite_SetPositionXY(SPRITE_AT(slot), (s16)x, (s16)ySub);
            Sprite_SetAnimActiveFlag(SPRITE_AT(slot), TRUE);
            Sprite_SetDrawFlag(SPRITE_AT(slot), TRUE);
            Sprite_SetAffineOverwriteMode(SPRITE_AT(slot), 2);
            j++;
            x += 16;
        } while (j < 5);
        order++;
        yMain += 32;
        ySub += 32;
        i++;
    } while (i < 5);
}
