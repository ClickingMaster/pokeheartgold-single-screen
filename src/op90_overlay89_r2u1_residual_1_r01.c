#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "msgdata.h"
#include "pm_string.h"
typedef struct Ov89Work {
    void *args;
    void *save;
    void *bg;
    void *palette;
    u8 pad10[0x0c];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u8 pad24[0x0c];
    MsgData *msgData;
    u8 pad34[0x8f0];
    ManagedSprite *spritesA[6];
    ManagedSprite *spritesB[6];
    ManagedSprite *spritesC[6];
    u8 pad96C[0x19C0 - 0x96C];
    void *textObj;
} Ov89Work;
extern const ManagedSpriteTemplate ov89_0225CB08;
extern const ManagedSpriteTemplate ov89_0225CB3C;
extern const ManagedSpriteTemplate ov89_0225CB70;
void ov89_0225A260(void *, void *, String *, int, u32, u32, u32, u32, u32, u32);
void TextOBJ_SetSpritesDrawFlag(void *, int);
void ov89_02259588(Ov89Work *work);
// @fn ov89_02259588
void ov89_02259588(Ov89Work *work) {
    ManagedSpriteTemplate tmpl;
    int i;
    String *str;
    tmpl = ov89_0225CB08;
    for (i = 0; i < 6; i++) {
        tmpl.x = i * 0x20 + 0x10;
        tmpl.y = 0xB0;
        work->spritesA[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &tmpl);
        ManagedSprite_SetAnim(work->spritesA[i], i);
        Sprite_TickFrame(work->spritesA[i]->sprite);
    }
    tmpl = ov89_0225CB3C;
    for (i = 0; i < 6; i++) {
        tmpl.x = i * 0x20 + 0x10;
        tmpl.y = 0xB0;
        work->spritesB[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &tmpl);
        ManagedSprite_SetAnim(work->spritesB[i], i);
        Sprite_TickFrame(work->spritesB[i]->sprite);
    }
    tmpl = ov89_0225CB70;
    for (i = 0; i < 6; i++) {
        tmpl.x = i * 0x20 + 0x10;
        tmpl.y = 0xB0;
        tmpl.resIdList[0] = i + 0x2711;
        work->spritesC[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &tmpl);
        Sprite_TickFrame(work->spritesC[i]->sprite);
    }
    str = NewString_ReadMsgData(work->msgData, 1);
    ov89_0225A260(work, &work->textObj, str, 2, 0x10203, 0, 0x2713, 0xE0, 0xB0, 1);
    TextOBJ_SetSpritesDrawFlag(work->textObj, TRUE);
    String_Delete(str);
}
