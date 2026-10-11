#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "math_util.h"
#include "heap.h"
typedef struct Particle07 {
    void *ctx;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u8 unk0C;
    u8 unk0D;
    u8 unk0E;
    u8 unk0F;
    ManagedSprite *sprite;
    u8 unk14[0x24];
    f32 scale;
    u32 *unk3C;
} Particle07;
typedef struct Effect07 {
    u8 unk00;
    u8 unk01;
    u8 count;
    u8 unk03[5];
    void *ctx;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    Particle07 particles[20];
    u32 unk514[20];
} Effect07;
typedef struct Spawn07 {
    s16 y;
    s16 x;
    s16 yRange;
    s16 xRange;
} Spawn07;
extern const Spawn07 ov07_022367CE[];
enum HeapID BattleAnimSystem_GetHeapID(void *ctx);
void BattleAnimSystem_GetSpriteTemplate(ManagedSpriteTemplate *template, void *ctx);
void BattleAnimUtil_SetBlendWithBattleBg(void *ctx, int a1, int a2);
u8 BattleAnimSystem_GetArg(void *ctx, int a1);
void BattleAnimSystem_CreateTask(void *ctx, void *func, void *data, u32 priority);
void ov07_0222A328(void *ctx, Particle07 *particle);
void ov07_0222A4B4(void *ctx, Effect07 *effect);
void ov07_0222A58C(void *ctx, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
// @fn ov07_0222A58C
void ov07_0222A58C(void *ctx, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    Effect07 *effect;
    int i;
    int j;
    s16 x;
    s16 y;
    effect = Heap_Alloc(BattleAnimSystem_GetHeapID(ctx), sizeof(Effect07));
    GF_ASSERT(effect != NULL);
    effect->unk01 = 0;
    effect->unk00 = 0;
    effect->spriteSystem = spriteSystem;
    effect->spriteManager = spriteManager;
    effect->ctx = ctx;
    BattleAnimSystem_GetSpriteTemplate(&template, ctx);
    BattleAnimUtil_SetBlendWithBattleBg(effect->ctx, -1, -1);
    effect->count = BattleAnimSystem_GetArg(effect->ctx, 0);
    effect->count = 12;
    effect->particles[0].sprite = sprite;
    j = 0;
    for (i = 0; i < effect->count; i++) {
        if (i != 0) {
            effect->particles[i].sprite = SpriteSystem_NewSprite(effect->spriteSystem, effect->spriteManager, &template);
        }
        sprite = effect->particles[i].sprite;
        effect->particles[i].unk0C = 0;
        effect->particles[i].unk0D = 0;
        effect->particles[i].unk0E = i + (LCRandom() % 10 + 10);
        effect->particles[i].unk0F = LCRandom() % 6;
        effect->particles[i].scale = 1.0f;
        effect->particles[i].ctx = effect->ctx;
        effect->particles[i].spriteSystem = effect->spriteSystem;
        effect->particles[i].spriteManager = effect->spriteManager;
        effect->particles[i].unk3C = &effect->unk514[i];
        x = ov07_022367CE[j].x + LCRandom() % ov07_022367CE[j].xRange;
        y = ov07_022367CE[j].y + LCRandom() % ov07_022367CE[j].yRange;
        ManagedSprite_SetPositionXY(sprite, x, y);
        ManagedSprite_SetAffineOverwriteMode(sprite, 2);
        ManagedSprite_SetAffineScale(sprite, effect->particles[i].scale, effect->particles[i].scale);
        ManagedSprite_SetDrawFlag(sprite, FALSE);
        BattleAnimSystem_CreateTask(ctx, ov07_0222A328, &effect->particles[i], 0x44B);
        j = (j + 1) % 6;
    }
    BattleAnimSystem_CreateTask(ctx, ov07_0222A4B4, effect, 0x44D);
}
