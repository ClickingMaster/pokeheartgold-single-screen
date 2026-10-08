#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "sys_task_api.h"

typedef struct {
    ManagedSprite *sprites[2];
    SysTask *task;
    u8 unkC[4];
    f32 scale;
    u8 unk14[2];
    s16 x;
    s8 dir;
    u8 phase;
    u8 unk1A[6];
    int unk20;
    int unk24;
    int unk28;
    int state;
} Ov40Bubble;

typedef struct {
    u8 unk0[0x18];
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    u8 unk20[0x6D8 - 0x20];
    int unk6D8;
    u8 unk6DC[0x6F8 - 0x6DC];
    Ov40Bubble bubbles[6];
} Ov40Work;

typedef struct {
    s8 v[6];
} Ov40S8x6;
typedef struct {
    s16 v[6];
} Ov40S16x6;
typedef struct {
    f32 v[6];
} Ov40F32x6;

extern const u8 ov40_02244DA8[];
extern const Ov40F32x6 ov40_02244DF0;
void ov40_0222D288(ManagedSprite *sprite, s16 a1, int a2);
void ov40_0222D048(SysTask *task, void *data);
void ov40_0222C884(Ov40Work *work);

// @fn ov40_0222C884
void ov40_0222C884(Ov40Work *work) {
    ManagedSpriteTemplate template;
    Ov40S16x6 ys;
    Ov40F32x6 scales;
    Ov40S8x6 xs;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int i, j;
    BOOL busy;

    spriteSys = work->spriteSys;
    spriteMan = work->spriteMan;
    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.vramTransfer = 0;
    template.pal = 0;
    template.bgPriority = 2;
    template.resIdList[0] = 9999;
    template.resIdList[1] = 9999;
    template.resIdList[2] = 9999;
    template.resIdList[3] = 9999;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    ys = *(const Ov40S16x6 *)(ov40_02244DA8 + 0x22);
    scales = ov40_02244DF0;
    xs = *(const Ov40S8x6 *)ov40_02244DA8;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
                template.resIdList[0] = 9999;
                template.resIdList[1] = 9999;
                template.resIdList[2] = 9999;
                template.resIdList[3] = 9999;
            } else {
                template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
                template.resIdList[0] = 10000;
                template.resIdList[1] = 10000;
                template.resIdList[2] = 10000;
                template.resIdList[3] = 10000;
            }
            work->bubbles[i].sprites[j] = SpriteSystem_NewSprite(spriteSys, spriteMan, &template);
            ov40_0222D288(work->bubbles[i].sprites[j], ys.v[i], 0x60);
            ManagedSprite_TickFrame(work->bubbles[i].sprites[j]);
            ManagedSprite_SetAffineOverwriteMode(work->bubbles[i].sprites[j], 2);
            ManagedSprite_SetAffineScale(work->bubbles[i].sprites[j], scales.v[i], 1.0f);
            work->bubbles[i].unk28 = 0;
            work->bubbles[i].phase = i % 3;
            work->bubbles[i].state = 0;
            work->bubbles[i].scale = scales.v[i];
            work->bubbles[i].unk24 = xs.v[i] * 2 + 10;
            if (i < 3) {
                work->bubbles[i].unk20 = 0;
                work->bubbles[i].x = xs.v[i] + 8;
                work->bubbles[i].dir = -1;
            } else {
                work->bubbles[i].unk20 = 0;
                work->bubbles[i].x = 255 - (xs.v[i] + 8);
                work->bubbles[i].dir = 1;
            }
        }
        work->bubbles[i].task = SysTask_CreateOnVBlankQueue(ov40_0222D048, &work->bubbles[i], 5);
    }
    if (work->unk6D8 != 0) {
        do {
            busy = FALSE;
            for (i = 0; i < 6; i++) {
                if (work->bubbles[i].state != 3) {
                    ov40_0222D048(work->bubbles[i].task, &work->bubbles[i]);
                    busy = TRUE;
                }
            }
        } while (busy);
    }
}
