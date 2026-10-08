#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteSystem SpriteSystem;
typedef struct A4B4Entry {
    ManagedSprite *sprite;
    u8 unk04[0x3C];
} A4B4Entry;
typedef struct A4B4Work {
    u8 state;
    u8 unk01;
    u8 count;
    u8 unk03[5];
    void *owner;
    u8 unk0C[4];
    SpriteSystem *spriteSystem;
    u8 unk14[0x10];
    A4B4Entry entries[19];
    u8 unk4E4[0x30];
    int status[20];
} A4B4Work;
void Sprite_DeleteAndFreeResources(ManagedSprite *sprite);
void SpriteSystem_DrawSprites(SpriteSystem *spriteSystem);
void ov07_0221C448(void *owner, void *task);
void ov07_0222A4B4(void *task, A4B4Work *work);

// @fn ov07_0222A4B4
void ov07_0222A4B4(void *task, A4B4Work *work) {
    int i;
    BOOL done;
    BOOL finished;
    switch (work->state) {
    case 0:
        done = TRUE;
        for (i = 0; i < work->count; i++) {
            if (work->status[i] != 1) {
                done = FALSE;
                break;
            }
        }
        if (done == TRUE) {
            work->state++;
        }
        break;
    case 1:
        for (i = 0; i < work->count; i++) {
            work->status[i] = 2;
        }
        work->state++;
        break;
    case 2:
        finished = TRUE;
        for (i = 0; i < work->count; i++) {
            if (work->status[i] != 3) {
                finished = FALSE;
                break;
            }
        }
        if (finished == TRUE) {
            work->state++;
        }
        break;
    case 3:
        for (i = 0; i < work->count; i++) {
            Sprite_DeleteAndFreeResources(work->entries[i].sprite);
        }
        work->state++;
        break;
    default:
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}
