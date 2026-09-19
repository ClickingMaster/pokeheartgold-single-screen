#include "overlay43_palpad_naming_private.h"

NamingScreenArgs *ov43_0222CD44(void *unused, PalPadSharedH17 *shared, enum HeapID heapId) {
    Options *options = Save_PlayerData_GetOptionsAddr(shared->saveData);
    NamingScreenArgs *args = NamingScreen_CreateArgs(heapId, NAME_SCREEN_UNK7, 0, 7, options, NULL);

    if (shared->useName == TRUE) {
        if (!sub_02020B78(shared->name[0])) {
            CopyU16ArrayToString(args->nameInputString, shared->name);
        } else {
            String_SetEmpty(args->nameInputString);
        }
        shared->useName = FALSE;
    }
    return args;
}

void ov43_0222CD90(void *sub, void *shared, void *base, u32 heapId) {
    ov43_0222CE48(sub, shared, base, heapId);
}

BOOL ov43_0222CD98(PalPadWorkH17 *work, PalPadNamingStateH17 *state, void *base) {
    switch (state->state) {
    case 0:
        ov43_0222D028(work, state, base);
        state->state = 1;
        break;
    case 1:
        if (ov43_0222CE14(work, state)) {
            PlaySE(0x5DD);
            ov43_0222D0F8(work, base, TRUE);
            work->timer = 4;
            state->state = 2;
        }
        break;
    case 2:
        work->timer--;
        if (work->timer <= 0) {
            ov43_0222D0F8(work, base, FALSE);
            ov43_0222A318(state, 0, 2);
            ov43_0222D090(work, base);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
