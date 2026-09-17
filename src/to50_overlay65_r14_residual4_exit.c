#include "to50_overlay65_r14_residual4_private.h"
BOOL WirelessTradeSelectMon_Exit(OverlayManager *man, int *state) {
    void *work = OverlayManager_GetData(man);
    void *args = OverlayManager_GetArgs(man);
    U32(args, 0x24) = U32(work, 0x68);
    ov65_0221CD0C(work);
    String_Delete(PTR(work, 0x19C));
    Heap_Free(PTR(work, 0x2228));
    ov65_0221CCB0(work);
    ov65_0221FAE0((u8 *)work + 0x444);
    ov65_0221D1C8(PTR(work, 0x180));
    Heap_FreeExplicit((enum HeapID)0x1A, PTR(work, 0x180));
    DestroyMsgData(PTR(work, 0x190));
    MessageFormat_Delete(PTR(work, 0x18C));
    MessageFormat_Delete(PTR(work, 0x188));
    MessageFormat_Delete(PTR(work, 0x184));
    String_Delete(PTR(work, 0x198));
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    FontID_Release(4);
    Heap_Destroy((enum HeapID)0x1A);
    return TRUE;
}
