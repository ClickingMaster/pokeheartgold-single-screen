#include "r40_overlay_40_residual_31_residual_6_private.h"

int ov40_02238D5C(void *p) {
    void *ctx = PTR(p, 0x860);
    switch (U32(p, 8)) {
    case 0:
        ov40_02230964(p, 1);
        ov40_02237008(p);
        ov40_02236534(p);
        OrbitCursor_SetActive(PTR(p, 0x6F4), 0);
        OrbitCursor_SetVisible(PTR(p, 0x6F4), 0);
        OrbitCursor_SetActive(PTR(p, 0x6F0), 1);
        OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
        ov40_02230964(p, 0);
        ov40_0222FB90(p, 1);
        U32(p, 8)
        ++;
        break;
    case 1:
        if (ov40_0222FBB4() != 0) {
            int i;
            for (i = 0; i < 5; i++) {
                TouchHitboxController_Destroy(((SolCtx40 *)ctx)->hitbox[i]);
            }
            VsRecorder_ResetFade((u8 *)ctx + 0x1AC);
            VsRecorder_RebuildMenuRowsParent(p);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
            U32(p, 8)
            ++;
        }
        break;
    default:
        if (VsRecorder_StepFade((u8 *)ctx + 0x1AC, 0) != 0) {
            VsRecorder_ClearPaletteBlend(p);
            VsRecorder_ResetFade((u8 *)ctx + 0x1AC);
            PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, 0x10, (u16)U32(p, 0x58));
            VsRecorder_SetScreenIfDone(p, 1, 1, PTR(p, 0x10));
            VsRecorder_SetSubState(p, 5);
            FrontierCard_Delete(PTR(ctx, 0x384));
            Heap_Free(ctx);
        } else {
            PaletteData_BlendPalettes(PTR(p, 0x28), 1, 2, (u8)U32(ctx, 0x1AC), (u16)U32(p, 0x58));
            PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)U32(ctx, 0x1AC), (u16)U32(p, 0x58));
        }
        break;
    }
    return 0;
}
