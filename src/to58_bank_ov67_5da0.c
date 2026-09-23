#include "to58_bank_ov67_5da0_private.h"

void ov67_021E5DA0(void *work) {
    ScreenMode mode;
    BgTemplateRaw bg0;
    BgTemplateRaw bg1;
    BgTemplateRaw bg4;
    BgTemplateRaw bg5;

    U32(work, 0x10) = (u32)BgConfig_Alloc(U32(work, 0));
    mode = ov67_021E6D5C;
    SetBothScreensModesAndDisable(&mode);
    bg0 = ov67_021E6DA8;
    InitBgFromTemplate((void *)U32(work, 0x10), 0, &bg0, 0);
    BG_ClearCharDataRange(0, 0x20, 0, U32(work, 0));
    BgClearTilemapBufferAndCommit((void *)U32(work, 0x10), 0);
    bg1 = ov67_021E6DFC;
    InitBgFromTemplate((void *)U32(work, 0x10), 1, &bg1, 0);
    bg4 = ov67_021E6DC4;
    InitBgFromTemplate((void *)U32(work, 0x10), 4, &bg4, 0);
    BG_ClearCharDataRange(4, 0x20, 0, U32(work, 0));
    BgClearTilemapBufferAndCommit((void *)U32(work, 0x10), 4);
    bg5 = ov67_021E6DE0;
    InitBgFromTemplate((void *)U32(work, 0x10), 5, &bg5, 0);
}
