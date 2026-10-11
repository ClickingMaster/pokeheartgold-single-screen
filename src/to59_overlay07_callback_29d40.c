#include "global.h"

#include "sprite_system.h"

#define F(p, t, o) (*(t *)((u8 *)(p) + (o)))
typedef struct Ov07Work {
    u8 state;
    u8 index[3];
} Ov07Work;

extern const u8 ov07_02236778[];
void BattleAnimSystem_DestroyTask(void *, void *);
void ov07_02229D40(void *, Ov07Work *);

void ov07_02229D40(void *owner, Ov07Work *work) {
    int i;
    s16 x, y;
    ManagedSprite *sprite;
    u8 *cursor;
    const u8 *table;
    switch (F(work, u8, 0)) {
    case 0:
        if (F(work, u8, 6) < 15) {
            F(work, u8, 6)++;
        }
        if (F(work, u8, 7) != 0) {
            F(work, u8, 7)--;
        }
        i = 0;
        *(volatile u16 *)0x04000052 = F(work, u8, 6) | (F(work, u8, 7) << 8);
        F(work, u8, 4)++;
        if (i < F(work, u8, 5)) {
            table = ov07_02236778;
            cursor = (u8 *)work;
            do {
            sprite = F(cursor, ManagedSprite *, 0x14);
            ManagedSprite_GetPositionXY(sprite, &x, &y);
            if (F(work, u8, 4) >= table[0]) {
                work->index[i]++;
                if (work->index[i] >= table[1]) {
                    work->index[i] = 0;
                }
            }
            cursor += 4;
            table += 2;
            i++;
            } while (i < F(work, u8, 5));
        }
        if (F(work, u8, 4) >= 45) {
            F(work, u8, 0)++;
        }
        break;
    case 1:
        if (F(work, u8, 6) != 0) {
            F(work, u8, 6)--;
        }
        if (F(work, u8, 7) < 15) {
            F(work, u8, 7)++;
        }
        if (F(work, u8, 6) == 0 && F(work, u8, 7) == 15) {
            F(work, u8, 0)++;
        }
        *(volatile u16 *)0x04000052 = F(work, u8, 6) | (F(work, u8, 7) << 8);
        break;
    default:
        i = 0;
        if (i < F(work, u8, 5)) {
            cursor = (u8 *)work;
            do {
                Sprite_DeleteAndFreeResources(F(cursor, ManagedSprite *, 0x14));
                cursor += 4;
                i++;
            } while (i < F(work, u8, 5));
        }
        BattleAnimSystem_DestroyTask(F(work, void *, 8), owner);
        Heap_Free(work);
        return;
    }
    i = 0;
    if (i < F(work, u8, 5)) {
        cursor = (u8 *)work;
        do {
            Sprite_TickFrame(*(Sprite **)F(cursor, ManagedSprite *, 0x14));
            cursor += 4;
            i++;
        } while (i < F(work, u8, 5));
    }
    SpriteSystem_DrawSprites(F(work, SpriteManager *, 0x10));
}
