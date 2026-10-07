#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct ListEntry {
    void *name;
    int value;
} ListEntry;
typedef struct ListWork {
    u8 unk00[4];
    Window *windows;
    u8 unk08[0x14];
    BgConfig *bgConfig;
    u8 unk20[0x14];
    ListEntry *entries;
} ListWork;
void sub_02019A60(BgConfig *bgConfig, int a1, Window *window);
void ov70_02242FC4(BgConfig *bgConfig, Window *window, void *name, int a3, int color);
void ov70_022434C0(ListWork *work, int page, int count);
void ov70_0224352C(ListWork *work, int page, int count);

// @fn ov70_022434C0
void ov70_022434C0(ListWork *work, int page, int count) {
    int i;
    for (i = 0; i < 4; i++) {
        FillWindowPixelBuffer(&work->windows[i], 0x22);
        if (page * 4 + i < count) {
            ov70_02242FC4(work->bgConfig, &work->windows[i], work->entries[page * 4 + i].name, 0, 0xF0E02);
        } else {
            CopyWindowPixelsToVram_TextMode(&work->windows[i]);
            sub_02019A60(work->bgConfig, 0, &work->windows[i]);
        }
    }
}

// @fn ov70_0224352C
void ov70_0224352C(ListWork *work, int page, int count) {
    int i;
    for (i = 0; i < 5; i++) {
        FillWindowPixelBuffer(&work->windows[i], 0x22);
        if (page * 5 + i < count) {
            ov70_02242FC4(work->bgConfig, &work->windows[i], work->entries[page * 5 + i].name, 0, 0xF0E02);
        } else {
            CopyWindowPixelsToVram_TextMode(&work->windows[i]);
            sub_02019A60(work->bgConfig, 0, &work->windows[i]);
        }
    }
}
