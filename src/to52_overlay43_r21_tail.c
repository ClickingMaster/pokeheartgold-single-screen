#include "to52_overlay43_r21_tail_private.h"
int ov43_0222CBB4(R21Runner *runner, void *context, R21State *state, int unused, int heapId) {
    u32 config[4];

    switch (state->state) {
    case 0:
        runner->namingArgs = ov43_0222CD44(runner, state, heapId);
        config[0] = 4;
        config[1] = 4;
        config[2] = 4;
        config[3] = 0;
        runner->secondaryArgs = sub_02085400(heapId, 12, config, Save_PlayerData_GetOptionsAddr(state->saveData), 0, 0);
        ov43_0222A1DC(context);
        runner->manager = OverlayManager_New(gOverlayTemplate_NamingScreen, runner->namingArgs, heapId);
        state->state = 1;
        break;
    case 1:
        if (OverlayManager_Run(runner->manager)) {
            OverlayManager_Delete(runner->manager);
            if (*(u32 *)((u8 *)runner->namingArgs + 0x14) == 0) {
                runner->manager = OverlayManager_New(_02102620, runner->secondaryArgs, heapId);
                state->state = 2;
            } else {
                ov43_0222A318(state, 0, 0);
                state->state = 3;
            }
        }
        break;
    case 2:
        if (OverlayManager_Run(runner->manager)) {
            OverlayManager_Delete(runner->manager);
            ov43_0222A338(state,
                *(u32 *)((u8 *)runner->namingArgs + 0x18),
                *(u32 *)((u8 *)runner->secondaryArgs + 0x1c));
            ov43_0222A318(state, 0, 7);
            state->state = 3;
        }
        break;
    case 3:
        NamingScreen_DeleteArgs(runner->namingArgs);
        sub_02085438(runner->secondaryArgs);
        ov43_0222A154(context, heapId);
        return 1;
    case 4:
        runner->namingArgs = ov43_0222CD44(runner, state, heapId);
        ov43_0222A1DC(context);
        runner->manager = OverlayManager_New(gOverlayTemplate_NamingScreen, runner->namingArgs, heapId);
        state->state = 5;
        break;
    case 5:
        if (OverlayManager_Run(runner->manager)) {
            OverlayManager_Delete(runner->manager);
            if (*(u32 *)((u8 *)runner->namingArgs + 0x14) == 0) {
                ov43_0222A318(state, 1, 12);
                ov43_0222A338(state, *(u32 *)((u8 *)runner->namingArgs + 0x18), 0);
                state->state = 6;
            } else {
                ov43_0222A318(state, 1, 13);
                state->state = 6;
            }
        }
        break;
    case 6:
        NamingScreen_DeleteArgs(runner->namingArgs);
        ov43_0222A154(context, heapId);
        return 1;
    }
    return 0;
}
