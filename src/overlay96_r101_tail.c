#include "overlay96_r101_tail_private.h"

typedef struct Ov96FData {
    u8 a;
    u8 b;
    u8 c;
    u8 pad;
    s32 x;
    s32 y;
} Ov96FData;

u32 ov96_0220F1CC(void *course) {
    u8 *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *copy = PokeathlonCourse_GetDataCopyArea(course);
    u32 *remote = ov96_021E8A20(copy + 0xf0);
    u32 *local = ov96_021E8A20(copy);
    if (remote[8] >> 31 == 1) {
        ov96_0220EAC4(work);
        ov96_021E65A4(course);
        ov96_0220E2A8(course);
        return 1;
    }
    if (System_GetTouchNew()) {
        ov96_021E8228(course, (u8)ov96_021E5F24(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        u32 value = local[0] & ~0xff;
        value |= (u8)gSystem.touchX;
        local[0] = value;
        value = (value & ~0xff00) | ((u32)gSystem.touchY << 24 >> 16) | 0x10000;
        local[0] = value;
    } else {
        local[0] &= ~0x10000;
    }
    ov96_0221013C(work + 0x510, course);
    return 0;
}

void ov96_0220F280(void *course) {
    u32 row8;
    int col;
    int row;
    s32 table[20];
    ReadWholeNarcMemberByIdPair(table, 0xaa, 7);
    for (row = 0; row < 4; row++) {
        row8 = (u8)row;
        for (col = 0; col < 3; col++) {
            u8 *in = ov96_021E60D8(course, row, col);
            Ov96FData *out = ov96_0220F378(course, row8, (u8)col);
            s32 v;
            float f;
            if (in[2] >= 5) {
                GF_AssertFail();
            }
            if (in[0] >= 5) {
                GF_AssertFail();
            }
            if (in[1] >= 5) {
                GF_AssertFail();
            }
            out->c = in[2];
            out->a = (u8)table[in[2]];
            out->b = (u8)table[5 + in[2]];
            v = table[10 + in[0]];
            if (v > 0) {
                f = _fadd(0.5f, _fflt(v << 12));
            } else {
                f = _fsub(_fflt(v << 12), 0.5f);
            }
            out->x = _s32_div_f(_ffix(f), 10);
            v = table[15 + in[1]];
            if (v > 0) {
                f = _fadd(0.5f, _fflt(v << 12));
            } else {
                f = _fsub(_fflt(v << 12), 0.5f);
            }
            out->y = _s32_div_f(_ffix(f), 10);
        }
    }
}

void *ov96_0220F378(void *course, u32 row, u32 col) {
    u8 *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (work == NULL) {
        GF_AssertFail();
    }
    if (row >= 4) {
        GF_AssertFail();
    }
    if (col >= 3) {
        GF_AssertFail();
    }
    work += 0x34;
    work += (row * 3 + col) * 12;
    if (work == NULL) {
        GF_AssertFail();
    }
    return work;
}
