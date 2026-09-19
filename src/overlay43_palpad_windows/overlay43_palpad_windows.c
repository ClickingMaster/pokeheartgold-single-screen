#include "private.h"
void ov43_0222D028(Ov43R24Windows *windows, Ov43R24Context *ctx, Ov43R24Work *work) {
    Ov43R24Screen *screen;
    void *profile = Save_PlayerData_GetProfile(ctx->save);
    BufferPlayersName(work->msgFmt, 0, profile);
    ov43_0222A9F4(work, work->tilebuf, 0x32);
    screen = windows->screen;
    LoadRectToBgTilemapRect(work->bgConfig, 2, screen->data, 0, 0, (u8)(screen->width >> 3), (u8)(screen->height >> 3));
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 2);
    ScheduleWindowCopyToVram(&windows->windows[0]);
    ScheduleWindowCopyToVram(&windows->windows[1]);
    ScheduleWindowCopyToVram(&windows->windows[2]);
}
void ov43_0222D090(Ov43R24Windows *windows, Ov43R24Work *work) {
    ov43_0222AA70(work);
    FillBgTilemapRect(work->bgConfig, 2, 0, 0, 0, 0x20, 0x18, 0);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 2);
    ClearWindowTilemapAndScheduleTransfer(&windows->windows[0]);
    ClearWindowTilemapAndScheduleTransfer(&windows->windows[1]);
    ClearWindowTilemapAndScheduleTransfer(&windows->windows[2]);
    FillBgTilemapRect(work->bgConfig, 3, 0, 0, 0, 0x20, 0x18, 0);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 3);
}
