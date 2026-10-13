#include "global_terminal_app_internal.h"

// DWC allocations of the Wi-Fi apps run by GlobalTerminalApp_Main (set once the DWC heap exists).
// The public label _021D1108 in asm/global_terminal_app_data.s is a separate, unreferenced word.
static NNSFndHeapHandle sDwcHeap;

BOOL GlobalTerminalApp_Init(OverlayManager *man, int *state) {
    GlobalTerminalApp *data;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    Heap_Create(HEAP_ID_3, HEAP_ID_123, 0x28000);
    data = OverlayManager_CreateAndGetData(man, sizeof(GlobalTerminalApp), HEAP_ID_123);
    MI_CpuFill8(data, 0, sizeof(GlobalTerminalApp));
    data->args = OverlayManager_GetArgs(man);
    Sound_SetSceneAndPlayBGM(11, 0x47D, 1);
    return TRUE;
}

BOOL GlobalTerminalApp_Main(OverlayManager *man, int *state) {
    GlobalTerminalApp *data = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        GlobalTerminalApp_LoadWifi(data);
        *state = 1;
        break;
    case 1:
        if (CommWireless_IsStartedUp()) {
            sDwcHeap = data->dwcHeap;
            DWC_SetMemFunc(GlobalTerminalApp_DwcAlloc, GlobalTerminalApp_DwcFree);
            data->wifiLoaded = 1;
            ++*state;
        }
        break;
    case 2:
        data->subApp = OverlayManager_New(&sOverlayTemplate_GlobalTerminalConnect, data, HEAP_ID_123);
        ++*state;
        break;
    case 3:
        if (OverlayManager_Run(data->subApp) == TRUE) {
            OverlayManager_Delete(data->subApp);
            if (data->isConnected == 1) {
                data->disconnect = 1;
                ++*state;
            } else {
                *state = 8;
            }
        }
        break;
    case 4: {
        const OverlayManagerTemplate *tmpl = VsRecorderApp_GetTemplate(data->args->vsRecorderMode);
        data->subApp = OverlayManager_New(tmpl, data->args->fieldSystem, HEAP_ID_123);
        ++*state;
        break;
    }
    case 5:
        if (OverlayManager_Run(data->subApp) == TRUE) {
            OverlayManager_Delete(data->subApp);
            ++*state;
        }
        break;
    case 6:
        data->subApp = OverlayManager_New(&sOverlayTemplate_GlobalTerminalConnect, data, HEAP_ID_123);
        ++*state;
        break;
    case 7:
        if (OverlayManager_Run(data->subApp) == TRUE) {
            OverlayManager_Delete(data->subApp);
            data->disconnect = 0;
            ++*state;
        }
        break;
    case 8:
        return TRUE;
    }

    if (data->wifiLoaded == 1 && data->disconnect == 1 && data->isConnected == 1) {
        DWC_UpdateConnection();
        WirelessStatusIcon_SetLevel(3 - DWC_GetLinkLevel());
    }
    return FALSE;
}

BOOL GlobalTerminalApp_Exit(OverlayManager *man, int *state) {
    GlobalTerminalApp *data = OverlayManager_GetData(man);

    GlobalTerminalApp_UnloadWifi(data);
    Heap_Free(data->args);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_123);
    return TRUE;
}

void GlobalTerminalApp_LoadWifi(GlobalTerminalApp *data) {
    if (data->wifiLoaded == 0) {
        LoadDwcOverlay();
        LoadOVY38();
        Comm_InitDwc(HEAP_ID_123);
        data->heapBuffer = Heap_Alloc(HEAP_ID_123, 0x20020);
        data->dwcHeap = NNS_FndCreateExpHeapEx((void *)(((u32)data->heapBuffer + 0x1F) & ~0x1F), 0x20000, 0);
        CommWireless_StartUp();
        Sys_ClearSleepDisableFlag(4);
    }
}

void GlobalTerminalApp_UnloadWifi(GlobalTerminalApp *data) {
    if (data->wifiLoaded == 1) {
        NNS_FndDestroyExpHeap(data->dwcHeap);
        Heap_Free(data->heapBuffer);
        UnloadOVY38();
        UnloadDwcOverlay();
        CommWireless_Terminate();
        data->wifiLoaded = 0;
    }
}

void *GlobalTerminalApp_DwcAlloc(int name, u32 size, int align) {
    void *ptr;
    OSIntrMode intrMode = OS_DisableInterrupts();
    ptr = NNS_FndAllocFromExpHeapEx(sDwcHeap, size, align);
    OS_RestoreInterrupts(intrMode);
    return ptr;
}

void GlobalTerminalApp_DwcFree(int name, void *ptr, u32 size) {
    if (ptr != NULL) {
        OSIntrMode intrMode = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(sDwcHeap, ptr);
        OS_RestoreInterrupts(intrMode);
    }
}
