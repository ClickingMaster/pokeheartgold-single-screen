#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "heap.h"
typedef struct Tri07 {
    s16 v[3];
} Tri07;
typedef struct Move07 {
    s16 x;
    s16 y;
    u8 unk04[0x20];
} Move07;
typedef struct Flash07 {
    u8 count;
    u8 state;
    u8 unk02[2];
    u8 blendA;
    u8 blendB;
    u8 unk06[2];
    s16 x;
    s16 y;
    void *ctx;
    u8 unk10[4];
    SpriteManager *spriteManager;
    ManagedSprite *sprites[4];
    Move07 moves[4];
} Flash07;
extern const Tri07 ov07_02236732[];
extern const Tri07 ov07_02236734[];
extern const Tri07 ov07_02236736[];
void BattleAnimUtil_InitLinearMotion(Move07 *move, s16 startX, s16 endX, s16 startY, s16 endY, u16 frames);
BOOL BattleAnimUtil_UpdateLinearMotion(Move07 *move);
void BattleAnimSystem_DestroyTask(void *ctx, void *task);
void ov07_02227B58(void *task, Flash07 *flash);
// @fn ov07_02227B58
void ov07_02227B58(void *task, Flash07 *flash) {
    int i;
    u8 done;
    int k;
    int step;
    int c;
    int c2;
    switch (flash->state) {
    case 0:
        k = (flash->count % 2) * 2;
        step = (flash->count + 1) / 2;
        c = ov07_02236736[k].v[0] - step;
        BattleAnimUtil_InitLinearMotion(&flash->moves[0], flash->x + ov07_02236732[k].v[0], flash->x + ov07_02236734[k].v[0], flash->y, flash->y, c);
        BattleAnimUtil_InitLinearMotion(&flash->moves[1], flash->x - ov07_02236732[k].v[0], flash->x - ov07_02236734[k].v[0], flash->y, flash->y, c);
        c2 = ov07_02236736[k + 1].v[0] - step;
        BattleAnimUtil_InitLinearMotion(&flash->moves[2], flash->x + ov07_02236732[k + 1].v[0], flash->x + ov07_02236734[k + 1].v[0], flash->y, flash->y, c2);
        BattleAnimUtil_InitLinearMotion(&flash->moves[3], flash->x - ov07_02236732[k + 1].v[0], flash->x - ov07_02236734[k + 1].v[0], flash->y, flash->y, c2);
        flash->count++;
        flash->state++;
    case 1:
        done = 0;
        for (i = 0; i < 4; i++) {
            if (BattleAnimUtil_UpdateLinearMotion(&flash->moves[i]) == FALSE) {
                done++;
            } else {
                ManagedSprite_SetPositionXY(flash->sprites[i], flash->moves[i].x, flash->moves[i].y);
            }
            ManagedSprite_TickFrame(flash->sprites[i]);
        }
        if (done == 4) {
            if (flash->count == 9) {
                flash->state++;
            } else {
                flash->state = 0;
            }
        }
        break;
    case 2:
        if (flash->blendA) {
            flash->blendA--;
        }
        if (flash->blendB < 15) {
            flash->blendB++;
        }
        if (flash->blendA == 0 && flash->blendB == 15) {
            flash->state++;
        }
        reg_G2_BLDALPHA = flash->blendA | (flash->blendB << 8);
        break;
    default:
        BattleAnimSystem_DestroyTask(flash->ctx, task);
        Heap_Free(flash);
        return;
    }
    SpriteSystem_DrawSprites(flash->spriteManager);
}
