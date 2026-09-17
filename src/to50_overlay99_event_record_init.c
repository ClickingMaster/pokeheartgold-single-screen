#include "to50_overlay99_event_record_init_private.h"
#define PTR(p, o)    (*(void **)((p) + (o)))
#define U32_AT(p, o) (*(u32 *)((p) + (o)))

BOOL PokeathlonEventRecord_Init(void *manager, int *state) {
    Ov99EventWork *work;
    void *args;

    HandleLoadOverlay((u32)&SDK_OVERLAY_OVY_98_ID, 2);
    args = OverlayManager_GetArgs(manager);
    if (args == NULL) {
        GF_AssertFail();
    }
    Heap_Create(3, 0x84, 0x30000);
    work = OverlayManager_CreateAndGetData(manager, 0xe4, 0x84);
    MI_CpuFill8(work, 0, 0xe4);
    U32_AT(work, 0xc) = 0x84;
    ov99_021E93DC(work, args);
    ov98_0221F090();
    ov99_021E8590(work);
    ov99_021E860C(work);
    PTR(work, 0x10) = ov98_0221EABC(U32_AT(work, 0xc), PTR(work, 4), 0x13, ov99_021EA45C, 0x13a);
    ov99_021E88D4(work);
    ov99_021E8788(work);
    ov99_021E8C04(work);
    ov99_021E8818(work);
    ResetVisibleHardwareWindows(0);
    ResetVisibleHardwareWindows(1);
    Main_SetVBlankIntrCB(ov99_021E856C, work);
    return TRUE;
}
