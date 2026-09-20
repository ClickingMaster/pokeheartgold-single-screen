#include "to56_w21_private.h"

void ov08_0221D81C(u8 *work) {
    u16 i;
    for (i = 0; i < 6; i++) {
        ov08_0221D77C(work, 0x125, i);
    }
}

void ov08_0221D840(u8 *work, int state) {
    extern void ov08_0221D8B0(u8 *, int);
    extern void ov08_0221D6CC(u8 *, int);
    ov08_0221D8B0(work, state);
    BgFillTilemapBufferAndSchedule(*(BgConfig **)(work + 0x1e4), 4, 0);
    BgFillTilemapBufferAndSchedule(*(BgConfig **)(work + 0x1e4), 5, 0);
    ov08_02220C5C(work, state);
    ov08_0221DD28(work);
    ov08_0221DC3C(work, state);
    ov08_0221DD70(work, state);
    ov08_0221D6CC(work, state);
    ov08_0222171C(work, state);
    ov08_022221CC(work, state);
    ov08_02222524(work, state);
    work[0x207a] = state;
}

void ov08_0221D8B0(u8 *work, int state) {
    u32 i;
    const u32 *ids = &ov08_02224F80[state * 2];
    for (i = 0; i < 2; i++) {
        NNSG2dScreenData *screen;
        void *file = AllocAndReadWholeNarcMemberByIdPair((NarcId)0x47, ids[i], (enum HeapID) * (u32 *)(*(u8 **)work + 0xc));
        NNS_G2dGetUnpackedScreenData(file, &screen);
        LoadRectToBgTilemapRect(*(BgConfig **)(work + 0x1e4), i + 6, (u8 *)screen + 0xc, 0, 0, 0x20, 0x18);
        ScheduleBgTilemapBufferTransfer(*(BgConfig **)(work + 0x1e4), i + 6);
        Heap_Free(file);
    }
}
