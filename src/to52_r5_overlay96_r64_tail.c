#include "to52_r5_overlay96_r64_private.h"
void ov96_021F82E4(Ov96R64Work *work, void *handle, s32 delay) {
    Ov96R64Item *item = &work->items[work->index];
    if (item->active != 0) {
        GF_AssertFail();
    }
    item->active = 1;
    item->handle = handle;
    item->delay = delay;
    work->index = (work->index + 1) % 4;
    work->count++;
}

void ov96_021F8334(Ov96R64Work *work, void *handle, s32 delay) {
    Ov96R64Item *item = &work->single;
    if (item->active != 0) {
        GF_AssertFail();
    }
    item->active = 1;
    item->handle = handle;
    item->delay = delay;
}
