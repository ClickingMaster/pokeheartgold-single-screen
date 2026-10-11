#pragma force_active on
#include "global.h"
typedef struct Work {
    u32 unk00;
    u8 *windows;
    u32 unk08;
    void *sprite;
    u8 unk10[0xC];
    void *bgConfig;
    u32 unk20;
    void *msgData;
    u8 unk28[0x14];
    s16 page;
    s16 selection;
    u8 unk40[8];
    int selected;
    int state;
    u8 unk50[0x14];
    u8 enabled[26];
} Work;
typedef struct Range {
    int first;
    int last;
} Range;
extern Range ov70_02245F58[];
extern int ov70_02245E84[];
extern int ov70_02245F5C[][2];
extern int ov70_02245FA0[];
extern u8 ov70_02245E26[][2];
extern u8 ov70_02245E27[][2];
extern void BgCellManager_LoadCellTilemap(void *, int, int, int, int);
extern void BgCellManager_GetCellTilemap(void *, int);
extern void BgCellManager_DrawCellAt(void *, int, int, int);
extern void ov70_0224190C(Work *, int);
extern void ov70_02243E8C(void *, void *, void *, int, int, int);
extern void ov70_02243EB8(void *, void *, void *, int);
extern void Sprite_SetAnimCtrlSeq(void *, int);
extern void Sprite_SetDrawFlag(void *, int);
extern void ov70_02238F9C(void *, int, int);
extern int ov70_02243F54(Work *, int);
int ov70_02243254(Work *work);

// @fn ov70_02243254
int ov70_02243254(Work *work) {
    int i;
    int color;
    BgCellManager_LoadCellTilemap(work->bgConfig, 0, 100, 32, 1);
    BgCellManager_GetCellTilemap(work->bgConfig, 0);
    ov70_0224190C(work, 5);
    MI_CpuFill8(work->enabled, 1, 26);
    ov70_02243E8C(work->bgConfig, work->msgData, work->windows, work->page + 110, 2, 0xF0E02);
    i = 1;
    if (i <= ov70_02245F5C[work->page][0]) {
        do {
            if (ov70_02243F54(work, ov70_02245E84[work->page] + i - 1) > 0) {
                color = 0xF0E02;
                work->enabled[ov70_02245E84[work->page] + i - 1] = TRUE;
            } else {
                color = 0x80902;
                work->enabled[ov70_02245E84[work->page] + i - 1] = FALSE;
            }
            ov70_02243E8C(work->bgConfig, work->msgData, work->windows + i * 16, ov70_02245FA0[ov70_02245F58[work->page].first + i - 1], 5, color);
            i++;
        } while (i <= ov70_02245F58[work->page].last);
    }
    ov70_02243EB8(work->bgConfig, work->msgData, work->windows + 224, 68);
    BgCellManager_DrawCellAt(work->bgConfig, 0, 16, 0);
    Sprite_SetAnimCtrlSeq(work->sprite, 47);
    if (work->selection < 0) {
        work->selected = 0;
    } else {
        work->selected = work->selection;
    }
    ov70_02238F9C(work->sprite, (ov70_02245E26[work->selected][0] + 16) * 8, ov70_02245E27[work->selected][0] * 8);
    Sprite_SetDrawFlag(work->sprite, 1);
    work->state = 9;
    return -1;
}
