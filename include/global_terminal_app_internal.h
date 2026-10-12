#ifndef POKEHEARTGOLD_GLOBAL_TERMINAL_APP_INTERNAL_H
#define POKEHEARTGOLD_GLOBAL_TERMINAL_APP_INTERNAL_H
#include "global.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sound_system.h"
#include "system.h"
#include "comm_wireless.h"
#include "unk_02037C94.h"
#include "network_overlay_loader.h"

typedef struct GlobalTerminalAppArgs {
    void *unk00;
    u32 unk04;
    u32 unk08;
    u8 unk0C;
    u8 padding[3];
} GlobalTerminalAppArgs;

// Also the args of the Global Terminal connect app (src/global_terminal_connect.c has its own view).
typedef struct GlobalTerminalApp {
    GlobalTerminalAppArgs *args;
    int disconnect;           // 0x04, set for the second connect app run: only disconnect
    OverlayManager *subApp;   // 0x08
    void *heapBuffer;         // 0x0C
    NNSFndHeapHandle dwcHeap; // 0x10, inside heapBuffer, 32-byte aligned
    u8 unk14[0x78 - 0x14];
    int wifiLoaded;  // 0x78
    int isConnected; // 0x7C, set by the connect app
} GlobalTerminalApp;

void LoadOVY38(void);
void UnloadOVY38(void);
void WirelessStatusIcon_SetLevel(int a0);
const OverlayManagerTemplate *sub_02087E10(u8 a0);
BOOL CommWireless_IsStartedUp(void);
void CommWireless_Terminate(void);
void ov00_021EC294(void *(*alloc)(int, u32, int), void (*free)(int, void *, u32));
int ov00_021EC9D4(void);
void ov00_021ECB40(void);

extern const OverlayManagerTemplate sOverlayTemplate_GlobalTerminalConnect;

BOOL GlobalTerminalApp_Init(OverlayManager *man, int *state);
BOOL GlobalTerminalApp_Main(OverlayManager *man, int *state);
BOOL GlobalTerminalApp_Exit(OverlayManager *man, int *state);
void GlobalTerminalApp_LoadWifi(GlobalTerminalApp *data);
void GlobalTerminalApp_UnloadWifi(GlobalTerminalApp *data);
void *GlobalTerminalApp_DwcAlloc(int name, u32 size, int align);
void GlobalTerminalApp_DwcFree(int name, void *ptr, u32 size);

BOOL GlobalTerminalApp_Init(OverlayManager *man, int *state);
BOOL GlobalTerminalApp_Main(OverlayManager *man, int *state);
BOOL GlobalTerminalApp_Exit(OverlayManager *man, int *state);
void GlobalTerminalApp_LoadWifi(GlobalTerminalApp *data);
void GlobalTerminalApp_UnloadWifi(GlobalTerminalApp *data);
void *GlobalTerminalApp_DwcAlloc(int name, u32 size, int align);
void GlobalTerminalApp_DwcFree(int name, void *ptr, u32 size);

#endif
