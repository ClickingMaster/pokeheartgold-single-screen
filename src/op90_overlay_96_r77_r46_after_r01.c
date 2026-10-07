#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct {
    u16 pos[8];
    u8 x10[4];
    s32 flags;
    u16 x18;
    u8 pad1a[4];
    u8 x1e[4];
} Data;
typedef struct {
    BgConfig *bg;
    u8 pad4[0x18];
    void *x1c;
    u8 x20[0x10];
    u8 x30[0x3e0 - 0x30];
    void *x3e0;
    u8 pad3e4[0x642 - 0x3e4];
    u16 x642;
} Work;
void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void *PokeathlonCourse_GetDataCopyArea(void *course);
void *ov96_021E8A20(void *block);
BOOL ov96_021E5F24(void *course);
void ov96_0220050C(void *w, u8 i, u16 a);
void ov96_022005B4(void *w, u8 value, u32 score);
void ov96_021FE550(void *course, u8 me, void *a, void *area);
void ov96_021FEAEC(Work *work, u16 pos);
void ov96_021FFC34(void *a, u8 v);
void ov96_02200A64(void *w, u16 v);
void ov96_021EB570(void *obj, int anim);
void PlaySE(u16 se);
void ov96_021FFBD8(BgConfig *bg, u16 v);
void ov96_021E6454(void *course, u16 v);
void ov96_021FDC7C(void *course);
// @fn ov96_021FDC7C
void ov96_021FDC7C(void *course) {
    Work *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *area = PokeathlonCourse_GetDataCopyArea(course);
    Data *data = ov96_021E8A20(area + 0xf0);
    u8 me = ov96_021E5F24(course);
    int i;
    u8 v;
    for (i = 0; i < 4; i++) {
        ov96_0220050C(work->x3e0, i, data->pos[i]);
    }
    ScheduleSetBgPosText(work->bg, 0, BG_POS_OP_SET_X, data->pos[me]);
    ScheduleSetBgPosText(work->bg, 1, BG_POS_OP_SET_X, data->pos[me] / 2);
    ScheduleSetBgPosText(work->bg, 2, BG_POS_OP_SET_X, data->pos[me] * 2);
    ov96_021FE550(course, me, work->x30, area);
    ov96_021FEAEC(work, data->pos[me]);
    v = data->x10[me] & 0x7f;
    ov96_022005B4(work->x3e0, v, (u8)(((u8)((data->flags >> 26) & 0xf) >> me) & 1) ? 0 : data->pos[me]);
    if (!(u8)((data->flags >> 24) & 1)) {
        ov96_021FFC34(work->x20, v);
    }
    ov96_02200A64(work->x3e0, data->x18);
    switch (data->x1e[me]) {
    case 0:
        work->x642 = 0;
        ov96_021EB570(work->x1c, 11);
        break;
    case 3:
        work->x642 = 0;
        ov96_021EB570(work->x1c, 13);
        break;
    case 2:
        if (work->x642 == 0) {
            work->x642 = 1;
            PlaySE(0x89b);
        }
        ov96_021EB570(work->x1c, 12);
        break;
    case 4:
        if (work->x642 == 0) {
            work->x642 = 1;
            PlaySE(0x89c);
        }
        ov96_021EB570(work->x1c, 14);
        break;
    case 1:
        work->x642 = 0;
        ov96_021EB570(work->x1c, 15);
        break;
    }
    ov96_021FFBD8(work->bg, data->pos[me]);
    ov96_021E6454(course, data->x18);
}
