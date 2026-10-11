#pragma force_active on
#include "global.h"
#include "pokepic.h"
typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteSystem SpriteSystem;
typedef struct SpriteManager SpriteManager;
typedef struct ManagedSpriteTemplate {
    u8 data[0x34];
} ManagedSpriteTemplate;
ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, const ManagedSpriteTemplate *template);
void ManagedSprite_SetFlipMode(ManagedSprite *sprite, u8 mode);
void ManagedSprite_SetPositionXY(ManagedSprite *sprite, s16 x, s16 y);
void ManagedSprite_SetPriority(ManagedSprite *sprite, int priority);
void ManagedSprite_SetDrawPriority(ManagedSprite *sprite, int priority);

typedef struct Motion {
    s16 x;
    s16 y;
    u8 unk04[0x20];
} Motion;

typedef struct B320Work {
    u8 unk00[4];
    void *owner;
    SpriteSystem *spriteSystem;
    u8 unk0C[4];
    SpriteManager *spriteManager;
    u8 unk14[8];
    s16 delay;
    s16 drawPriority[8];
    u8 unk2E[2];
    ManagedSprite *sprites[8];
    Motion motion;
} B320Work;

void *BattleAnimUtil_AllocWork(void *owner, u32 size);
void BattleAnimUtil_InitCommonWork(void *owner, void *work);
void ov07_0221F9E8(ManagedSpriteTemplate *template, void *owner);
u32 ov07_0221C468(void *owner);
u32 ov07_0221C470(void *owner);
Pokepic *ov07_0221FA48(void *owner, u32 battler);
void ov07_02222268(Motion *motion, s16 x0, s16 x1, s16 y0, s16 y1, int frames);
u32 BattleAnimUtil_GetBattlerSide(void *owner, u32 battler);
BOOL ov07_022222B4(Motion *motion);
int ov07_0221FB04(void *owner, int a1);
int ov07_0221BFC0(void *owner);
void ov07_0221C410(void *owner, void *func, void *data);
void ov07_0222B2A8(void *task, B320Work *work);
void ov07_0222B320(void *owner, int a1, int a2, ManagedSprite *sprite);

// @fn ov07_0222B320
void ov07_0222B320(void *owner, int a1, int a2, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    B320Work *work;
    int i;
    s16 x0, x1, y0, y1;
    Pokepic *pic;
    Pokepic *pic2;
    int frame;
    int count;
    BOOL running;
    BOOL skip;
    int prio1, prio2;

    work = BattleAnimUtil_AllocWork(owner, sizeof(B320Work));
    work->delay = 10;
    BattleAnimUtil_InitCommonWork(owner, work);
    ov07_0221F9E8(&template, work->owner);
    work->sprites[0] = sprite;
    for (i = 1; i < 8; i++) {
        work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        if (i % 2) {
            ManagedSprite_SetFlipMode(work->sprites[i], 1);
        }
    }

    pic = ov07_0221FA48(work->owner, ov07_0221C468(work->owner));
    x0 = Pokepic_GetAttr(pic, 0);
    y0 = Pokepic_GetAttr(pic, 1);
    pic2 = ov07_0221FA48(work->owner, ov07_0221C470(work->owner));
    x1 = Pokepic_GetAttr(pic2, 0);
    y1 = Pokepic_GetAttr(pic2, 1);
    ov07_02222268(&work->motion, x0, x1, y0, y1, 20);

    frame = 0;
    count = 0;
    skip = FALSE;
    if (BattleAnimUtil_GetBattlerSide(work->owner, ov07_0221C468(work->owner)) == 4) {
        skip = TRUE;
    }
    do {
        running = ov07_022222B4(&work->motion);
        if (frame % 2) {
            if (skip == FALSE) {
                if (count < 8) {
                    ManagedSprite_SetPositionXY(work->sprites[count], work->motion.x, work->motion.y);
                    count++;
                }
            } else {
                skip = FALSE;
            }
        }
        frame++;
    } while (running == TRUE);

    prio1 = ov07_0221FB04(work->owner, 1);
    prio2 = ov07_0221FB04(work->owner, 2);
    for (i = 0; i < 8; i++) {
        work->drawPriority[i] = (8 - i) * 4;
        if (ov07_0221BFC0(work->owner) == 0) {
            if (BattleAnimUtil_GetBattlerSide(work->owner, ov07_0221C468(work->owner)) == 3) {
                if (i < 4) {
                    ManagedSprite_SetPriority(work->sprites[i], prio2);
                } else {
                    ManagedSprite_SetPriority(work->sprites[i], prio1);
                }
            } else {
                if (i >= 4) {
                    ManagedSprite_SetPriority(work->sprites[i], prio2);
                } else {
                    ManagedSprite_SetPriority(work->sprites[i], prio1);
                }
            }
            ManagedSprite_SetDrawPriority(work->sprites[i], 8 - i);
        } else {
            ManagedSprite_SetDrawPriority(work->sprites[i], i);
            ManagedSprite_SetPriority(work->sprites[i], prio2);
        }
    }
    ov07_0221C410(work->owner, ov07_0222B2A8, work);
}
