#pragma force_active on
#include "global.h"
typedef struct {
    u8 state, timer, stacks, chance, other, pad[3];
    f32 speed, frac, max, accel;
    int done, time, distance, event;
} Item;
typedef struct {
    u8 pad[0x238];
    Item items[12];
} Heap;
typedef struct {
    u8 pad[0x28];
    int accel[5];
    int max[5];
} Params;
void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
u8 PokeathlonCourse_GetParticipantCount(void *course);
u8 *ov96_021E60D8(void *course, int a, int b);
u16 LCRandom(void);
void GF_AssertFail(void);
typedef struct {
    u16 x0[3];
    u8 x6, x7, x8, x9;
    u16 xa[3];
    u16 x10[3];
    u8 x16[0x24 - 0x16];
} Payload;
typedef struct {
    u8 pad[0xd8];
    void *xd8;
    u8 pad2[0x230 - 0xdc];
    u32 x230;
    u8 pad3[4];
    Item items[12];
} Heap2;
extern f32 ov96_0221C444[];
int ov96_021FC164(void *a, int b, int c);
void ov96_021E8228(void *course, int a, int b, int c, int d);
void PlaySE(u32 seq);
void ov96_021FBA3C(void *course, Heap2 *heap, Payload *out, u8 a3, int idx, u8 itemIdx);
void ov96_021FBBB4(void *course, Params *params);
// @fn ov96_021FBA3C
void ov96_021FBA3C(void *course, Heap2 *heap, Payload *out, u8 a3, int idx, u8 itemIdx) {
    Item *item = &heap->items[itemIdx];
    int event;
    int n;
    f32 f;
    if (item->done != 1 && item->state != 2 && item->stacks == 0) {
        if (item->speed < item->max) {
            item->speed += item->accel;
            if (item->speed > item->max) {
                item->speed = item->max;
            }
        }
    }
    if (item->state == 2) {
        item->timer--;
        if (item->timer == 0) {
            item->state = 0;
        }
    } else {
        event = ov96_021FC164(heap->xd8, idx, item->distance);
        if (event != -1 && item->event != event) {
            u8 rnd = LCRandom() % 100;
            if (rnd < item->chance) {
                if (item->stacks < 4) {
                    item->stacks++;
                    item->speed = item->max + ov96_0221C444[item->stacks - 1];
                }
                ov96_021E8228(course, a3, idx, 6, 1);
                ov96_021E8228(course, a3, idx, 2, 1);
            } else if (rnd < item->other) {
                item->state = 2;
                item->timer = 30;
                item->stacks = 0;
                item->speed = 0.5f;
                ov96_021E8228(course, a3, idx, 1, 1);
            } else {
                ov96_021E8228(course, a3, idx, 2, 1);
            }
            item->event = event;
        }
    }
    f = item->speed + item->frac;
    n = f;
    item->frac = f - n;
    item->distance += n;
    out->x0[idx] = item->distance;
    if (item->done == 0 && item->distance >= 0x1000) {
        out->x10[idx] = 1;
        out->xa[idx] = heap->x230;
        item->time = heap->x230;
        item->done = 1;
        PlaySE(0x8a1);
    }
}

// @fn ov96_021FBBB4
void ov96_021FBBB4(void *course, Params *params) {
    Heap *heap = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 count = PokeathlonCourse_GetParticipantCount(course);
    u8 i;
    u8 j;
    for (i = count; i < 4; i++) {
        int base = i - count;
        for (j = 0; j < 3; j++) {
            Item *item = &heap->items[(u8)(j + base * 3)];
            u8 *stats = ov96_021E60D8(course, i, j);
            item->max = params->max[stats[1]] / 10.0f;
            item->accel = params->accel[stats[4]] / 100.0f;
            switch (LCRandom() % 6) {
            case 0:
                item->chance = 10;
                item->other = 50;
                break;
            case 1:
                item->chance = 20;
                item->other = 50;
                break;
            case 2:
                item->chance = 30;
                item->other = 60;
                break;
            case 3:
                item->chance = 20;
                item->other = 70;
                break;
            case 4:
                item->chance = 30;
                item->other = 70;
                break;
            case 5:
                item->chance = 40;
                item->other = 80;
                break;
            default:
                GF_AssertFail();
                break;
            }
        }
    }
}
