#pragma force_active on
#include "global.h"
#include "bg_window.h"

typedef struct Ov85WindowSet {
    int count;
    u8 pad4[0x18];
    Window windows[8];
} Ov85WindowSet;

typedef struct Ov85Work {
    u8 pad0[0xC9C];
    Ov85WindowSet windowSet;
    u8 padD1C[0xD84 - 0xC9C - sizeof(Ov85WindowSet)];
    BgConfig *bgConfig;
} Ov85Work;

extern const WindowTemplate *ov85_021EA5AC[];
void ov85_021E730C(Ov85Work *work, int count);
void ov85_021E7350(Ov85Work *work);

// @fn ov85_021E730C
void ov85_021E730C(Ov85Work *work, int count) {
    int i;
    Ov85WindowSet *set = &work->windowSet;
    const WindowTemplate *templates = ov85_021EA5AC[count];

    set->count = count;
    for (i = 0; i < count; i++) {
        AddWindow(work->bgConfig, &set->windows[i], &templates[i]);
    }
}

// @fn ov85_021E7350
void ov85_021E7350(Ov85Work *work) {
    int i;
    Ov85WindowSet *set = &work->windowSet;

    for (i = 0; i < set->count; i++) {
        ClearWindowTilemapAndCopyToVram(&set->windows[i]);
        RemoveWindow(&set->windows[i]);
    }
}
