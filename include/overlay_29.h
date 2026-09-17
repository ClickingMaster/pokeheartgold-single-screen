#ifndef POKEHEARTGOLD_OVERLAY_29_H
#define POKEHEARTGOLD_OVERLAY_29_H

#include "bg_window.h"
#include "sys_task.h"

// Numeric entry app (sub-screen number pad). Launched from the overlay_01
// sub-app table at ov01_02206C60 alongside touch_save_app.
//
// args->max is the upper bound on the entered value; args->result is set to
// -1 on launch, the entered value on confirm, or 0 on cancel.
typedef struct NumberEntryArgs {
    u32 max;
    int result;
} NumberEntryArgs;

SysTask *ov29_0225D520(BgConfig *bgConfig, void *a1, void *a2, NumberEntryArgs *args);
void ov29_0225D5EC(BgConfig *bgConfig, SysTask *task);
BOOL ov29_0225D61C(void *a0);

#endif // POKEHEARTGOLD_OVERLAY_29_H
