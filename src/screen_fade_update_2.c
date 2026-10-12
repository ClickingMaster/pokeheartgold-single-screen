#include "screen_fade_update_internal.h"

void sub_0200FF5C(FadeBlankPair *data, int index) {
    data->active[index] = 0;
    if (data->active[0] == 0 && data->active[1] == 0) {
        HBlankInterruptDisable();
    }
    data->funcs[index] = sub_02010014;
    data->values[index] = 0;
}

void sub_0200FF88(void *blankPair, void *data, void (*callback)(void *data), int screen, enum HeapID heapId) {
    FadeSetTaskData *task = Heap_AllocAtEnd(heapId, sizeof(FadeSetTaskData));
    task->blankPair = blankPair;
    task->data = data;
    task->callback = callback;
    task->screen = screen;
    SysTask_CreateOnVWaitQueue(sub_0200FFD8, task, 0x400);
}

void sub_0200FFB4(void *blankPair, int screen, enum HeapID heapId) {
    FadeClearTaskData *data = Heap_AllocAtEnd(heapId, sizeof(FadeClearTaskData));
    data->data = blankPair;
    data->index = screen;
    SysTask_CreateOnVWaitQueue(sub_0200FFF8, data, 0x400);
}

void sub_0200FFD8(SysTask *task, void *arg) {
    FadeSetTaskData *data = arg;
    sub_0200FEE4(data->blankPair, data->data, data->callback, data->screen);
    SysTask_Destroy(task);
    Heap_Free(data);
}

void sub_0200FFF8(SysTask *task, void *arg) {
    FadeClearTaskData *data = arg;
    sub_0200FF5C(data->data, data->index);
    SysTask_Destroy(task);
    Heap_Free(data);
}

void sub_02010014(u32 value) {
}

u16 sub_02010018(u8 *data, u16 color) {
    if (color == 0xFFFF) {
        color = *(u16 *)(data + 0x150);
    }
    return color;
}

u16 sub_0201002C(u8 *data) {
    ScreenFade *fade;
    if (*(u32 *)(data + 0xC) == 1) {
        fade = (ScreenFade *)(data + 0x14);
    } else {
        fade = (ScreenFade *)(data + 0x44);
    }
    if (fade->fadeOut == TRUE) {
        return fade->color;
    }
    return *(u16 *)(data + 0x150);
}

void sub_02010050(SysTask *task, void *arg) {
    ScreenFade *fade = arg;
    SetMasterBrightness(fade->screen, 0);
    SysTask_Destroy(task);
}

// A fade in from black or white done by a window wipe: the master brightness goes back to neutral on the next VBlank.
void sub_02010064(ScreenFade *fade) {
    if (fade->fadeOut == FALSE && (fade->color == RGB_WHITE || fade->color == RGB_BLACK) && !fade->isBrightnessFade) {
        SysTask_CreateOnVWaitQueue(sub_02010050, fade, 0x400);
    }
}

// A fade out to black or white done by a window wipe: the screen is left at full brightness of that color with
// the windows off.
void sub_02010094(ScreenFade *fade) {
    if (fade->fadeOut == TRUE && (fade->color == RGB_WHITE || fade->color == RGB_BLACK) && !fade->isBrightnessFade) {
        SetMasterBrightnessByColor((PMLCDTarget)fade->screen, fade->color);
        ResetVisibleHardwareWindows((PMLCDTarget)fade->screen);
    }
}
