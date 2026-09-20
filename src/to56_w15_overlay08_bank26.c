#include "global.h"

#include "bg_window.h"

#pragma require_prototypes off

typedef struct Ov08Bank26Work {
    u8 padding00[4];
    BgConfig *bgConfig;
    u8 padding08[0x1144];
    u8 state;
    u8 paletteIndex;
} Ov08Bank26Work;

void ov08_02223454(Ov08Bank26Work *work);
void ov08_022233DC(Ov08Bank26Work *work, int state);
void ov08_02223480(Ov08Bank26Work *work, int state);
void ov08_02224A50(Ov08Bank26Work *work, int state);
void ov08_02224134(Ov08Bank26Work *work, int state);
void ov08_0222421C(Ov08Bank26Work *work, int state);
void ov08_02223F94(Ov08Bank26Work *work, int state);

void ov08_02223248(Ov08Bank26Work *work, int state) {
    switch (state) {
    case 0:
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_Y, 0);
        break;
    case 1:
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_X, 0x100);
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_Y, 0);
        break;
    case 2:
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_Y, 0x100);
        break;
    }
}

void ov08_022232AC(Ov08Bank26Work *work, int state) {
    if (state == 2) {
        BgTilemapRectChangePalette(work->bgConfig, 6, 2, 0x23, 0x1C, 4, work->paletteIndex + 8);
        BgTilemapRectChangePalette(work->bgConfig, 6, 2, 0x28, 0x1C, 8, work->paletteIndex + 8);
    }
}

void ov08_02223300(Ov08Bank26Work *work, int state) {
    ov08_022232AC(work, state);
    ov08_02223248(work, state);
    BgFillTilemapBufferAndSchedule(work->bgConfig, 4, 0);
    BgFillTilemapBufferAndSchedule(work->bgConfig, 5, 0);
    ov08_02223454(work);
    ov08_022233DC(work, state);
    ov08_02223480(work, state);
    ov08_02224A50(work, state);
    ov08_02224134(work, state);
    ov08_0222421C(work, state);
    work->state = state;
    ov08_02223F94(work, work->state);
}
