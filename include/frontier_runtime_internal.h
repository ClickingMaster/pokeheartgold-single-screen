#ifndef POKEHEARTGOLD_FRONTIER_RUNTIME_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_RUNTIME_INTERNAL_H

#include "global.h"

#include "overlay_manager.h"

typedef struct FrontierObjectGfxSlot {
    u16 value;
    u16 pad;
} FrontierObjectGfxSlot;

typedef struct FrontierObjectSlot {
    u8 pad[12];
    u16 value;
    u8 rest[46];
} FrontierObjectSlot;

typedef struct FrontierWork {
    void *launchArgs;
    OverlayManager *childManager;
    void *childArgs;
    void (*childExit)(void *);
    u8 freeChildArgs;
    u8 pad_011[3];
    void *system;
    void *map;
    u8 pad_01c;
    u8 mapActive;
    u8 rebuildPending;
    u8 pad_01f;
    u16 taskId;
    u8 exitRequested;
    u8 pad_023;
    FrontierObjectGfxSlot objectGfx[24];
    FrontierObjectSlot objects[32];
    u8 pad_804[0x200];
    u16 spriteState[8];
    u8 pad_a14[0x40];
} FrontierWork;

typedef struct FrontierLaunchArgsInternal {
    void *data;
    u8 pad[0x1c];
    u8 mode;
} FrontierLaunchArgsInternal;

void *FrontierMap_Init(void);
void FrontierMap_Free(void *);
void FrontierSystem_Delete(void *);
void *FrontierSystem_Create(FrontierWork *, int, u8);
void FrontierSystem_AddTask(void *, u8, u16);
int FrontierSystem_Main(void *);
void FrontierMap_SaveObjects(void *);
void FrontierMap_RestoreObjects(void *);
void FrontierSystem_ChangeMsgBank(void *, u8, int);
void *FrontierSystem_SaveVars(void *, int);
void FrontierSystem_RestoreVars(void *, void *);

void Frontier_CreateMap(FrontierWork *);
void Frontier_FreeMap(FrontierWork *);
void *Frontier_GetLaunchArgs(FrontierWork *);
void *Frontier_GetMap(FrontierWork *);
void *Frontier_GetData(FrontierWork *);
void Frontier_SetData(FrontierWork *, void *);
void Frontier_LaunchApplication(FrontierWork *, const OverlayManagerTemplate *, void *, u8, void (*)(void *));
void Frontier_RequestExit(FrontierWork *);
void Frontier_ChangeMode(FrontierWork *, u8, u16);
void *Frontier_GetMapObjectGfx(FrontierWork *);
void *Frontier_GetMapObjects(FrontierWork *);
void *Frontier_GetMapObject(FrontierWork *, int);
void *Frontier_GetMapSpriteState(FrontierWork *);
void Frontier_ResetMapSpriteState(FrontierWork *);
BOOL Frontier_Exit(OverlayManager *, int *);
void Frontier_ResetMapObjects(FrontierWork *);
void Frontier_LoadOverlays(void);
void Frontier_UnloadOverlays(void);
BOOL Frontier_Init(OverlayManager *, int *);
BOOL Frontier_Main(OverlayManager *, int *);

#endif
