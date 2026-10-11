#include "to50_overlay65_r14_residual1_private.h"
BOOL WirelessTradeSelectMon_Init(OverlayManager *man, int *state) {
    void *narc;
    void *work;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(vu32 *)0x04000000 &= 0xFFFFE0FF;
    *(vu32 *)0x04001000 &= 0xFFFFE0FF;
    Heap_Create((enum HeapID)3, (enum HeapID)0x1A, 0x70FA0);
    FontID_Alloc(4, 0x1A);
    narc = NARC_New(0x34, 0x1A);
    work = OverlayManager_CreateAndGetData(man, 0x36CC, (enum HeapID)0x1A);
    MI_CpuClearFast(work, 0x36CC);
    PTR(work, 0x180) = BgConfig_Alloc(0x1A);
    PTR(work, 0x184) = MessageFormat_New_Custom(12, 0x16, 0x1A);
    PTR(work, 0x188) = MessageFormat_New(0x1A);
    PTR(work, 0x18C) = MessageFormat_New(0x1A);
    PTR(work, 0x190) = NewMsgDataFromNarc(0, 0x1B, 0xB7, 0x1A);
    U32(work, 0x4C) = 0;
    ov65_0221D0EC(work, man);
    SetKeyRepeatTimers(4, 8);
    ov65_0221CE98();
    ov65_0221CEB8(PTR(work, 0x180));
    BeginNormalPaletteFade(0, 1, 1, 0, 0x10, 1, 0x1A);
    ov65_0221D280(work, narc);
    Main_SetVBlankIntrCB(ov65_0221CE1C, work);
    ov65_0221D3B8();
    ov65_0221D3E8(work, narc);
    ov65_0221D930(work);
    ov65_0221F8D0(PTR(work, 0x180), (u8 *)work + 0x444, PTR(PTR(work, 8), 0x18));
    {
        void *x = PTR(PTR(work, 8), 0);
        PTR(work, 0) = x;
        sub_0208F7E0(x, work);
    }
    ov65_0221FB90((u8 *)work + 0x5B4, 0x14, 1, PTR(work, 0x190), PTR(work, 0x184));
    sub_0208F658(PTR(work, 0));
    ToggleBgLayer(0, 1);
    GfGfx_EngineATogglePlanes(0x10, 1);
    sub_020399FC(0x1A, PTR(work, 0x180));
    WirelessStatusIcon_Create();
    IsNighttime();
    sub_02055198(0, 0x427);
    sub_02005448(0x427);
    NARC_Delete(narc);
    return TRUE;
}
