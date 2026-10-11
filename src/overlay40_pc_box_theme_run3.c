#include "overlay40_pc_box_theme_private.h"

void ov40_02238EBC(void *p, u32 *a);
void ov40_02238F00(void *p);
void ov40_02239340(void *p);
void ov40_022393F4(void *p);
void ov40_02239418(void *p);
void ov40_02239514(void *p);
u32 ov40_02239538(void *p, int idx);
void ov40_02239574(void *p);
void ov40_022397BC(void *p, int a);
void ov40_02239838(void *p);
void ov40_022398F8(void *p);
void ov40_0223992C(void *p);
void ov40_02239954(u32 sel, int flag, void *scene);
int ov40_022399B8(void *p);
int ov40_02239A58(void *p);

int ov40_0223A034(void *p) {
    void *w = PTR(p, 0x860);
    VsRecorder_UpdateTouchScroll((u8 *)p + 0x47C);
    VsRecorder_ListScroll((u8 *)p + 0x49C, *(s16 *)((u8 *)p + 0x48C));
    VsRecorder_RedrawRankingList((u8 *)p + 0x49C, p, WORD(w, 0xC), WORD(w, 0x14), PTR(w, 0x748));
    TouchHitboxController_IsTriggered(PTR(w, 0xDC));
    return 0;
}

int ov40_0223A080(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        VsRecorder_SetVBlankPaused(p, 1);
        VsRecorder_FreeTouchScroll((u8 *)p + 0x47C);
        VsRecorder_RemoveListWindow((u8 *)p + 0x49C);
        if (PTR(w, 0x748) != NULL) {
            String_Delete(PTR(w, 0x748));
        }
        VsRecorder_FreeListArrows((u8 *)p + 0x49C, p);
        VsRecorder_SetVBlankPaused(p, 0);
        OrbitCursor_SetVisible(PTR(p, 0x6F4), 0);
        GfGfx_EngineBTogglePlanes(4, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        WORD(p, 8) = WORD(p, 8) + 1;
        break;
    case 1:
        VsRecorder_StepFade((u8 *)w + 8, 1);
        if (VsRecorder_StepBlend(w, (u8 *)w + 4, 1, 0)) {
            WORD(p, 8) = WORD(p, 8) + 1;
        }
        PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, WORD(w, 8), WORD(p, 0x58));
        PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, WORD(w, 8), WORD(p, 0x58));
        break;
    default:
        VsRecorder_SetSubState(p, 8);
        break;
    }
    return 0;
}

int ov40_0223A158(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        ov40_022393F4(p);
        ov40_02239514(p);
        VsRecorder_SetVBlankPaused(p, 1);
        ov40_022398F8(p);
        VsRecorder_SetVBlankPaused(p, 0);
        TouchHitboxController_Destroy(PTR(w, 0xDC));
        WORD(p, 8) = WORD(p, 8) + 1;
        break;
    case 1:
        VsRecorder_SetVBlankPaused(p, 1);
        VsRecorder_ResetFade((u8 *)w + 8);
        VsRecorder_RebuildMenuRowsParent(p);
        VsRecorder_SetVBlankPaused(p, 0);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
        VsRecorder_StartFlipScreens(p, 1);
        WORD(p, 8) = WORD(p, 8) + 1;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(p) != NULL) {
            if (VsRecorder_StepFade((u8 *)w + 8, 0)) {
                VsRecorder_ClearPaletteBlend(p);
                VsRecorder_ResetFade((u8 *)w + 8);
                PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, 0x10, WORD(p, 0x58));
                VsRecorder_SetScreenIfDone(p, 1, TRUE, (u32 *)PTR(p, 0x10));
                VsRecorder_SetSubState(p, 5);
                Heap_Free(w);
            } else {
                PaletteData_BlendPalettes(PTR(p, 0x28), 1, 2, WORD(w, 8), WORD(p, 0x58));
                PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, WORD(w, 8), WORD(p, 0x58));
            }
        }
        break;
    }
    return 0;
}
