#include "network_overlay_loader.h"

#include "global.h"

#include "overlay_13_thumb_1.h"
#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_0);
FS_EXTERN_OVERLAY(OVY_13);
FS_EXTERN_OVERLAY(OVY_38);

BOOL LoadDwcOverlay(void) {
    return HandleLoadOverlay(FS_OVERLAY_ID(OVY_0), OVY_LOAD_ASYNC);
}

void UnloadDwcOverlay(void) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_0));
}

BOOL LoadOVY13(void) {
    return HandleLoadOverlay(FS_OVERLAY_ID(OVY_13), OVY_LOAD_ASYNC);
}

void UnloadOVY13(void) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_13));
}

// Runs the Nintendo WFC setup utility, then resets the system.
void RunNintendoWFCSetupAndReset(void *unused, enum HeapID heapID) {
    LoadDwcOverlay();
    LoadOVY13();
    RunWifiSetupApp(heapID);
    UnloadOVY13();
    UnloadDwcOverlay();
    OS_ResetSystem(0);
}

BOOL LoadOVY38(void) {
    return HandleLoadOverlay(FS_OVERLAY_ID(OVY_38), OVY_LOAD_ASYNC);
}

void UnloadOVY38(void) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_38));
}

BOOL NintendoWFCSetupApp_Init(OverlayManager *man, int *state) {
    void *args;

    Heap_Create(HEAP_ID_3, HEAP_ID_48, 0x41000);
    args = OverlayManager_GetArgs(man);
    RunNintendoWFCSetupAndReset(*(void **)((u8 *)args + 8), HEAP_ID_48);
    Heap_Destroy(HEAP_ID_48);
    OS_ResetSystem(0);
    return TRUE;
}
