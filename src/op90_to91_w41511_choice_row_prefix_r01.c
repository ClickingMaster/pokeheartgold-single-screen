#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "palette.h"

typedef struct {
    ManagedSprite *sprite;
    u8 unk4[0x24];
} Ov40Choice;

typedef struct {
    int flag;
    u8 unk4[0x20];
} Ov40ChoiceInfo;

typedef struct {
    u8 unk0[0x18];
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    u8 unk20[8];
    PaletteData *plttData;
    u8 unk2C[0x58 - 0x2C];
    int unk58;
    u8 unk5C[0x534 - 0x5C];
    Ov40Choice choices[10];
    u8 unk6C4[0x6D8 - 0x6C4];
    int count;
    u8 unk6DC[0x6E0 - 0x6DC];
    int unk6E0;
    u8 unk6E4[0x818 - 0x6E4];
    Ov40ChoiceInfo *info;
} Ov40Work;

extern const u8 ov40_02244DC0[];
void ov40_0222D288(ManagedSprite *sprite, s16 x, s16 y);
void ov40_0222D294(ManagedSprite *sprite, s16 *x, s16 *y);
void ov40_0222CCAC(Ov40Work *work);

// @fn ov40_0222CCAC
void ov40_0222CCAC(Ov40Work *work) {
    s16 x;
    s16 y;
    ManagedSpriteTemplate template;
    Ov40Choice *choice = work->choices;
    int i;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;

    spriteSys = work->spriteSys;
    spriteMan = work->spriteMan;
    plttData = work->plttData;
    template.x = 42;
    template.y = (5 - work->unk6E0) * 16 + 201;
    if (work->unk6E0 == 2) {
        template.y = (4 - work->unk6E0) * 16 + 201;
    }
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = 3;
    template.vramTransfer = 0;
    template.resIdList[0] = 10001;
    template.pal = 1;
    template.resIdList[1] = 9999;
    template.resIdList[2] = 10001;
    template.resIdList[3] = 10001;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    for (i = 0; i < 5; i++) {
        template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        template.resIdList[0] = 10002;
        template.resIdList[1] = 10000;
        template.resIdList[2] = 10002;
        template.resIdList[3] = 10002;
        template.drawPriority = ov40_02244DC0[i + 5];
        choice[i].sprite = SpriteSystem_NewSprite(spriteSys, spriteMan, &template);
        ManagedSprite_TickFrame(choice[i].sprite);
        ManagedSprite_SetDrawFlag(choice[i].sprite, TRUE);
        ov40_0222D288(choice[i].sprite, template.x + 8, template.y);
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.resIdList[0] = 10001;
        template.resIdList[1] = 9999;
        template.resIdList[2] = 10001;
        template.resIdList[3] = 10001;
        template.drawPriority = ov40_02244DC0[i];
        choice[i + 5].sprite = SpriteSystem_NewSprite(spriteSys, spriteMan, &template);
        ManagedSprite_TickFrame(choice[i + 5].sprite);
        ov40_0222D288(choice[i + 5].sprite, template.x, template.y);
        ManagedSprite_SetDrawFlag(choice[i + 5].sprite, work->info[i].flag);
    }
    i = 0;
    do {
        if (work->count == 0) {
            break;
        }
        ov40_0222D294(work->choices[i].sprite, &x, &y);
        if (work->count - 1 == i) {
            y = 169;
            ManagedSprite_SetPaletteOverrideOffset(work->choices[i].sprite, 1);
        } else {
            x -= (work->count - i) * 4;
            y = (work->count - i - 1) * 16 + ((5 - (work->count - i)) * 36 + 25);
            ManagedSprite_SetPaletteOverrideOffset(work->choices[i].sprite, 2);
        }
        ov40_0222D288(work->choices[i].sprite, x, y);
        i++;
    } while (i < work->count);
    PaletteData_BlendPalettes(plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, work->unk58);
}
