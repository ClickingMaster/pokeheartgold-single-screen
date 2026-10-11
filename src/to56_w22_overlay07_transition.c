#include "to56_w22_overlay07_transition_private.h"

typedef struct B4Work {
    u8 state;
    u8 pad01[3];
    void *owner;
    u8 pad08[4];
    SpriteSystem *spriteSystem;
    u8 pad10[8];
    void *effectManager;
    ManagedSprite *sprite1;
    ManagedSprite *sprite2;
    ManagedSprite *sprite3;
    s32 timer;
    float scaleX;
    float scaleY;
    u8 blendA;
    u8 blendB;
    s8 direction;
    u8 pad37;
    void *effect1;
    void *effect2;
} B4Work;

void ov07_0222B4E0(void *task, B4Work *work);

void ov07_0222B4E0(void *task, B4Work *work) {
    u32 paletteOffset;
    float scale;

    switch (work->state) {
    case 0:
        if (work->timer > 14) {
            work->scaleY -= 0.2f;
            work->scaleX += 0.2f;
            if (work->scaleY <= 0.2) {
                work->state++;
                ManagedSprite_SetDrawFlag(work->sprite1, FALSE);
            } else {
                scale = work->scaleX * (float)work->direction;
                ManagedSprite_SetAffineScale(work->sprite1, scale, work->scaleY);
            }
        } else {
            work->blendA++;
            work->blendB--;
            *(volatile u16 *)0x04000052 = work->blendA | (work->blendB << 8);
            work->timer++;
        }
        break;
    case 1:
        if (ov07_02222EE8(work->effect2)) {
            break;
        }
        ov07_02222EF8(work->effect1);
        ov07_02222EF8(work->effect2);
        paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)work->sprite3), 1);
        work->effect2 = ov07_02222F10(work->effectManager, BattleAnimSystem_GetHeapID(work->owner), 2, (u16)(paletteOffset << 4), 16, 0, 1, 15, 0, 0xffff, 0x44c);
        work->state++;
        break;
    case 2:
    default:
        if (ov07_02222EE8(work->effect2)) {
            break;
        }
        ManagedSprite_TickFrame(work->sprite2);
        ov07_02222EF8(work->effect2);
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprite1);
    ManagedSprite_TickFrame(work->sprite2);
    ManagedSprite_TickFrame(work->sprite3);
    SpriteSystem_DrawSprites(work->spriteSystem);
}
