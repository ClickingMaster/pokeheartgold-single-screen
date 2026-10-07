#pragma force_active on
#include "global.h"
#include "sprite_system.h"

typedef struct Row {
    u32 unk0;
    ManagedSprite *first;
    ManagedSprite *second;
    ManagedSprite *cells[20];
    ManagedSprite *tops[5];
} Row; // size 0x70

typedef struct Work {
    u8 unk0[8];
    SpriteSystem *system;
    SpriteManager *manager;
    u8 unk10[0x34];
    void *aux;
    u8 unk48[0xC];
    Row rows[4];
} Work;

typedef struct RowSpec {
    u32 unk0;
    u32 cell;
    u32 top;
} RowSpec;

extern RowSpec ov96_0221CCC8[4];
Work *PokeathlonCourse_GetHeapAllocPtr4(void *course);
ManagedSprite *ov96_0220D1A0(SpriteSystem *system, SpriteManager *manager, s16 x, s16 y, u16 res, u8 priority);
void ov96_0220A254(void *course, u8 idx, int flag);
void ov96_02209F8C(void *course);

// @fn ov96_02209F8C
void ov96_02209F8C(void *course) {
    Work *work;
    int i;
    Row *row;
    int x;
    RowSpec *spec;
    int j;
    int y;
    s16 cx;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    GF_ASSERT(work != NULL);
    GF_ASSERT(work->system != NULL);
    GF_ASSERT(work->manager != NULL);
    GF_ASSERT(work->aux != NULL);
    for (i = 0, row = work->rows, x = 120, spec = ov96_0221CCC8; i < 4; row++, x += 32, spec++, i++) {
        s16 px = x;
        row->first = ov96_0220D1A0(work->system, work->manager, px, 152, i + 13, 3);
        ov96_0220A254(course, i, 0);
        row->second = ov96_0220D1A0(work->system, work->manager, px, 152, 18, 3);
        ManagedSprite_SetDrawFlag(row->second, 0);
        for (j = 0, y = 0; j < 5; y += 16, j++) {
            row->tops[j] = ov96_0220D1A0(work->system, work->manager, (s16)x + 6, (s16)(136 - y) - 20, spec->top, 44 - j);
            ManagedSprite_SetDrawFlag(row->tops[j], 0);
        }
        cx = x;
        for (j = 0; j < 20; j++) {
            row->cells[j] = ov96_0220D1A0(work->system, work->manager, cx, 136 - j * 4, spec->cell, 24 - j);
            ManagedSprite_SetDrawFlag(row->cells[j], 0);
        }
    }
}
