#include "to41_overlay_14_residual_20_private.h"

u32 PCBox_UpdateBoxMenuSlide(void *p);
u32 ov14_021E9660(void *p);
void ov14_021E96A8(void *p);
u32 ov14_021E98AC(void *p);
u32 ov14_021E9920(void *p);
u32 ov14_021E9970(void *p);
u32 ov14_021E99A0(void *p);
u32 ov14_021E99F0(void *p);
u32 PCBox_RunStateMachine(void *p, u32 *state);

u32 PCBox_State_WaitFade(void *p);
u32 ov14_021EA068(void *p);
u32 ov14_021EA130(void *p);
u32 ov14_021EA728(void *p);
void ov14_021EA1F0(void *p);
u32 ov14_021EA0B8(void *p);
u32 ov14_021EA180(void *p);
u32 ov14_021EA378(void *p);
u32 ov14_021EAF08(void *p);
u32 PCBox_State_Exit(void *p);
u32 ov14_021EA674(void *p);

u32 ov14_021EA408(void *p);
u32 ov14_021EAC24(void *p);
u32 ov14_021EAB54(void *p);

u32 PCBox_State_Exit(void *p) {
    PCBox_SaveMenuInputState(p);
    SysTask_Destroy(PTR_AT(PTR_AT(p, 0x34), 0));
    PCBox_FreeGridInput(p);
    PCBox_DestroyYesNoPrompt(p);
    PCBox_FreePanels(PTR_AT(p, 0x34));
    ov14_021F29AC(PTR_AT(p, 0x34));
    PCBox_RemoveWindows(p);
    PCBox_DestroyMsgResources(p);
    PCBox_FreePaletteData(p);
    PCBox_FreeBgLayers(p);
    NARC_Delete(PTR_AT(PTR_AT(p, 0x34), 0x454));
    NARC_Delete(PTR_AT(PTR_AT(p, 0x34), 0x450));
    GF_TouchpadStopSampling();
    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04001050 = 0;
    GfGfx_EngineASetPlanes(0);
    GfGfx_EngineBSetPlanes(0);
    Heap_Free(PTR_AT(p, 0x34));
    Heap_Destroy(0xa);
    return U32_AT(p, 0x30);
}

u32 PCBox_State_WaitFade(void *p) {
    if (IsPaletteFadeFinished() == 1) {
        return U32_AT(PTR_AT(p, 0x34), 0x440);
    }
    return 2;
}
