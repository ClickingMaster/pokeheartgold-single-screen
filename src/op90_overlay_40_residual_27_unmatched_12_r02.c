#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "save_vars_flags.h"
#include "save.h"
#include "sys_vars.h"

typedef struct SpriteSystem SpriteSystem;
typedef struct SpriteManager SpriteManager;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    int pal;
    NNS_G2D_VRAM_TYPE vram;
    int resIdList[6];
    int bgPriority;
    int vramTransfer;
} Ov40SpriteTemplate;

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, const Ov40SpriteTemplate *template);
void ManagedSprite_SetAnim(ManagedSprite *sprite, int anim);
void ManagedSprite_TickFrame(ManagedSprite *sprite);
void ManagedSprite_SetAffineScale(ManagedSprite *sprite, f32 x, f32 y);
void ManagedSprite_SetPaletteOverride(ManagedSprite *sprite, int pal);
void ManagedSprite_SetDrawFlag(ManagedSprite *sprite, int flag);
void ManagedSprite_SetAffineOverwriteMode(ManagedSprite *sprite, int mode);
void ov40_0222D288(ManagedSprite *sprite, s16 x, s16 y);

typedef struct {
    u8 unk0[4];
    f32 scales[5];
    u8 unk18[0x40 - 0x18];
    ManagedSprite *spritesA[5];
    ManagedSprite *spritesB[5];
    ManagedSprite *spritesC[5];
    ManagedSprite *digitSprites[5];
    int value;
} Ov40PrintData;

typedef struct {
    u8 unk0[0x18];
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    u8 unk20[0x830 - 0x20];
    SaveData *saveData;
    u8 unk834[0x860 - 0x834];
    Ov40PrintData *printData;
} Ov40Work;

typedef struct {
    int v[5];
} Ov40Int5;

extern const Ov40Int5 ov40_022451B0;
void ov40_022335F4(Ov40Work *work);

// @fn ov40_022335F4
void ov40_022335F4(Ov40Work *work) {
    int zeros = 0;
    Ov40PrintData *data = work->printData;
    int digits[5];
    int progress[5];
    Ov40SpriteTemplate template;
    Ov40Int5 anims2;
    int started;
    int i;
    int div;
    int value;
    SaveVarsFlags *varsFlags;

    template.x = 0;
    template.y = 256;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 10;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.pal = 0;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.resIdList[0] = 165535;
    template.resIdList[1] = 165535;
    template.resIdList[2] = 165535;
    template.resIdList[3] = 165535;
    for (i = 0; i < 5; i++) {
        data->spritesA[i] = SpriteSystem_NewSprite(work->spriteSys, work->spriteMan, &template);
        ManagedSprite_SetAnim(data->spritesA[i], 1);
        ManagedSprite_TickFrame(data->spritesA[i]);
        ManagedSprite_SetAffineScale(data->spritesA[i], data->scales[i], data->scales[i]);
    }
    for (i = 0; i < 5; i++) {
        template.resIdList[0] = 18181 + i;
        template.resIdList[1] = 18181 + i;
        if (i == 3) {
            template.resIdList[2] = 18182;
            template.resIdList[3] = 18182;
        } else {
            template.resIdList[2] = 18181;
            template.resIdList[3] = 18181;
        }
        data->spritesB[i] = SpriteSystem_NewSprite(work->spriteSys, work->spriteMan, &template);
        if (i == 3) {
            ManagedSprite_SetAnim(data->spritesB[i], 0);
        } else {
            ManagedSprite_SetAnim(data->spritesB[i], 1);
        }
        ManagedSprite_TickFrame(data->spritesB[i]);
        ManagedSprite_SetAffineScale(data->spritesB[i], data->scales[i], data->scales[i]);
        ManagedSprite_SetPaletteOverride(data->spritesB[i], i + 10);
    }
    template.resIdList[0] = 28282;
    template.resIdList[1] = 28282;
    template.resIdList[2] = 28282;
    template.resIdList[3] = 28282;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.drawPriority = 20;
    varsFlags = Save_VarsFlags_Get(work->saveData);
    progress[0] = Save_VarsFlags_GetBattleTowerPrintProgress(varsFlags);
    progress[1] = Save_VarsFlags_GetBattleFactoryPrintProgress(varsFlags);
    progress[2] = Save_VarsFlags_GetBattleArcadePrintProgress(varsFlags);
    progress[3] = Save_VarsFlags_GetBattleCastlePrintProgress(varsFlags);
    progress[4] = Save_VarsFlags_GetBattleHallPrintProgress(varsFlags);
    for (i = 0; i < 5; i++) {
        data->spritesC[i] = SpriteSystem_NewSprite(work->spriteSys, work->spriteMan, &template);
        if (progress[i] == 0 || progress[i] == 1) {
            ManagedSprite_SetAnim(data->spritesC[i], 5);
            ManagedSprite_SetPaletteOverride(data->spritesC[i], 9);
            ManagedSprite_SetDrawFlag(data->spritesB[i], FALSE);
            ManagedSprite_SetDrawFlag(data->spritesA[i], FALSE);
        } else {
            anims2 = ov40_022451B0;
            ManagedSprite_SetAnim(data->spritesC[i], anims2.v[i]);
            ManagedSprite_SetPaletteOverride(data->spritesC[i], anims2.v[i] + 4);
        }
        ManagedSprite_TickFrame(data->spritesC[i]);
        ManagedSprite_SetAffineScale(data->spritesC[i], data->scales[i], data->scales[i]);
    }
    template.resIdList[0] = 29292;
    template.resIdList[1] = 29292;
    template.resIdList[2] = 29292;
    template.resIdList[3] = 29292;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    started = 0;
    template.drawPriority = 0;
    value = data->value;
    div = 1000;
    for (i = 0; i < 4; i++) {
        digits[i] = value / div;
        value = value % div;
        div = div / 10;
        if (digits[i] == 0 && started == 0) {
            zeros++;
        } else {
            started = 1;
        }
    }
    digits[4] = 10;
    for (i = 0; i < 5; i++) {
        data->digitSprites[i] = SpriteSystem_NewSprite(work->spriteSys, work->spriteMan, &template);
        if (digits[i] == 0 && i < zeros) {
            ManagedSprite_SetDrawFlag(data->digitSprites[i], FALSE);
        }
        ManagedSprite_SetAnim(data->digitSprites[i], digits[i]);
        ov40_0222D288(data->digitSprites[i], 88 + i * 16, 72);
        ManagedSprite_TickFrame(data->digitSprites[i]);
    }
    ManagedSprite_SetDrawFlag(data->digitSprites[3], TRUE);
    ManagedSprite_SetDrawFlag(data->digitSprites[4], TRUE);
    for (i = 0; i < 5; i++) {
        ManagedSprite_SetAffineOverwriteMode(data->spritesA[i], 2);
        ManagedSprite_SetAffineOverwriteMode(data->spritesB[i], 2);
        ManagedSprite_SetAffineOverwriteMode(data->spritesC[i], 2);
    }
}
