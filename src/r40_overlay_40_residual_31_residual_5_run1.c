#include "r40_overlay_40_residual_31_residual_5_private.h"

u32 ov40_022383C8(void *p) {
    Ov40R31Work *w = PTR_AT(p, 0x860);
    switch (U32_AT(p, 8)) {
    case 0:
        ov40_02238304(p);
        ov40_02237C74(p);
        if (w->flag == 0) {
            ov40_02237D6C(p);
            VsRecorder_SetVBlankPaused(p, 1);
            VsRecorder_FreeTouchScroll((u8 *)p + 0x47c);
            VsRecorder_RemoveListWindow((u8 *)p + 0x49c);
            VsRecorder_FreeListArrows((u8 *)p + 0x49c, p);
            VsRecorder_SetVBlankPaused(p, 0);
        } else {
            VsRecorder_SetVBlankPaused(p, 1);
            VsRecorder_FreeProfile((u8 *)w + 0x2ed8, p);
            ov40_022361B0(p);
            VsRecorder_SetVBlankPaused(p, 0);
        }
        w->flag = 0;
        VsRecorder_SetVBlankPaused(p, 1);
        ov40_02236EB4(p);
        ov40_0223707C(p, 0xff);
        OrbitCursor_SetVisible(PTR_AT(p, 0x6f4), 0);
        OrbitCursor_SetActive(PTR_AT(p, 0x6f0), 0);
        OrbitCursor_SetVisible(PTR_AT(p, 0x6f0), 0);
        OrbitCursor_SetRadius(PTR_AT(p, 0x6f0), 0, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        BgClearTilemapBufferAndCommit(PTR_AT(p, 0x24), 3);
        VsRecorder_SetVBlankPaused(p, 0);
        U32_AT(p, 8)
        ++;
        /* fall through */
    case 1:
        if (!VsRecorder_StepBlend((u8 *)w + 0x1a4, (u8 *)w + 0x1a8, 1, 0)) {
            break;
        }
        GfGfxLoader_LoadScrnDataFromOpenNarc(PTR_AT(p, 0x14), 0x4f, PTR_AT(p, 0x24), 7, 0, 0, 0, 0x6d);
        U32_AT(p, 8)
        ++;
        break;
    case 2:
        if (!VsRecorder_StepBlend((u8 *)w + 0x1a4, (u8 *)w + 0x1a8, 0, 2)) {
            break;
        }
        ov40_0222C710(p, 2);
        GfGfx_EngineATogglePlanes(4, 1);
        GfGfx_EngineBTogglePlanes(4, 1);
        ov40_02237030(p, 0x113);
        ov40_02236F38(p);
        VsRecorder_SetSubState(p, 3);
        break;
    }
    return 0;
}
