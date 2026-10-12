#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "bg_window.h"

// VS Recorder box data view: removes the box list title. Its own translation unit: built at -O2 (Makefile override).

#pragma force_active on

void VsRecorder_RemoveBoxListTitle(VsRecorderWork *work) {
    u32 i = 0;
    Window *window = ((VsRecorderBoxData *)work->screenWork)->windows;

    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        i++;
        window++;
    } while (i < 1);
}

#pragma force_active reset
