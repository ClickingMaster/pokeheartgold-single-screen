#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "math_util.h"

typedef struct Input {
    u16 pos : 7;
    u16 type : 4;
    u16 state : 3;
    u16 owner : 2;
} Input;

typedef struct Entry {
    ManagedSprite *main[2];
    ManagedSprite *sub[2];
    ManagedSprite *extraA;
    ManagedSprite *extraB;
    u32 angle : 16;
    u32 prevState : 8;
    u32 rest : 8;
} Entry;

typedef struct Display {
    u32 unk0;
    Entry entries[2];
} Display;

int ov96_021E5F24(void *course);
u8 *PokeathlonCourse_GetHeapAllocPtr4(void *course);
s16 ov96_0220E6DC(s16 value, int type);
void ov96_0220DB3C(Entry *entry, u32 mode, BOOL draw);
void ov96_0220DBE8(Entry *entry, int animation, s16 x, s16 y);
void ov96_0220DBCC(Entry *entry, f32 scale);
void ov96_0220D428(void *data, u8 valueC, u8 valueD);
void ov96_0220DC38(u32 value);
void PlaySE(u16 seq);
void ov96_0220DC7C(Display *display, Input *input, void *course);

// @fn ov96_0220DC7C
void ov96_0220DC7C(Display *display, Input *input, void *course) {
    u8 myId = ov96_021E5F24(course);
    u8 *heap = PokeathlonCourse_GetHeapAllocPtr4(course);
    Entry *entry;
    int i;
    s16 y, ys;
    int side;
    s16 x;

    for (i = 0, entry = display->entries; i < 2; i++, entry++, input++) {
        y = input->pos * 8 / 2;
        if (input->state == 2) {
            y += 480;
        }
        ys = y;
        if (y >= 288) {
            side = 1;
        } else {
            side = 0;
        }
        if (side) {
            ys -= 288;
        }
        x = ov96_0220E6DC(y, input->type);
        if (input->state == 0) {
            ov96_0220DB3C(entry, 2, FALSE);
            ManagedSprite_SetDrawFlag(entry->extraA, FALSE);
            ManagedSprite_SetDrawFlag(entry->extraB, FALSE);
        } else if (input->state >= 3 && entry->prevState < 3) {
            ov96_0220DB3C(entry, 2, FALSE);
            ov96_0220DBE8(entry, input->state, x, ys - 40);
            if (input->owner == myId) {
                ov96_0220D428(heap + 0x6a8, 4, 4);
                ov96_0220DC38(input->state);
            } else {
                ManagedSprite_SetDrawFlag(entry->extraA, FALSE);
            }
            PlaySE(0x8c4);
        } else if (input->state == 1) {
            ov96_0220DB3C(entry, side, TRUE);
            ManagedSprite_SetDrawFlag(entry->extraA, FALSE);
            ManagedSprite_SetDrawFlag(entry->extraB, FALSE);
        }
        if (entry->prevState == 0 && input->state == 1) {
            entry->angle = 0;
            ov96_0220DBCC(entry, 1.2f);
            PlaySE(0x8c3);
        }
        if (input->state == 1 || input->state == 2) {
            if (side && x > 16) {
                ManagedSprite_SetAnimNoRestart(entry->sub[side], 1);
            }
            {
                f32 scale = 1.2f + (f32)(GF_SinDeg(entry->angle) / 18) / 4096.0f;
                entry->angle += 10;
                ov96_0220DBCC(entry, scale);
            }
            ManagedSprite_SetPositionXYWithSubscreenOffset(entry->main[side], x, ys - 40, 30 * FX32_ONE * 16);
            ManagedSprite_SetPositionXYWithSubscreenOffset(entry->sub[side], x, ys, 30 * FX32_ONE * 16);
        }
        entry->prevState = input->state;
    }
}
