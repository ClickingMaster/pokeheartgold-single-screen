#ifndef POKEHEARTGOLD_GLOBAL_TERMINAL_APP_INTERNAL_H
#define POKEHEARTGOLD_GLOBAL_TERMINAL_APP_INTERNAL_H
#include "global.h"

#include "comm_state.h"
#include "comm_wireless.h"
#include "dwc/net_common_internal.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "launch_application.h"
#include "network_overlay_loader.h"
#include "overlay_manager.h"
#include "sound_system.h"
#include "system.h"

// Also the args of the Global Terminal connect app.
typedef struct GlobalTerminalApp {
    GlobalTerminalLaunchArgs *args;
    int disconnect; // set for the second connect app run: only disconnect
    OverlayManager *subApp;
    void *heapBuffer;
    NNSFndHeapHandle dwcHeap; // inside heapBuffer, 32-byte aligned
    NetConnection wifiConnection;
    int wifiLoaded;
    int isConnected; // set by the connect app
} GlobalTerminalApp;

const OverlayManagerTemplate *VsRecorderApp_GetTemplate(u8 mode);

BOOL GlobalTerminalApp_Init(OverlayManager *man, int *state);
BOOL GlobalTerminalApp_Main(OverlayManager *man, int *state);
BOOL GlobalTerminalApp_Exit(OverlayManager *man, int *state);
void GlobalTerminalApp_LoadWifi(GlobalTerminalApp *data);
void GlobalTerminalApp_UnloadWifi(GlobalTerminalApp *data);
void *GlobalTerminalApp_DwcAlloc(int name, u32 size, int align);
void GlobalTerminalApp_DwcFree(int name, void *ptr, u32 size);

BOOL GlobalTerminalConnect_Init(OverlayManager *man, int *state);
BOOL GlobalTerminalConnect_Main(OverlayManager *man, int *state);
BOOL GlobalTerminalConnect_Exit(OverlayManager *man, int *state);

#endif
