#ifndef POKEHEARTGOLD_FRONTIER_H
#define POKEHEARTGOLD_FRONTIER_H

#include "bag_cursor.h"
#include "field_system.h"
#include "overlay_manager.h"

typedef struct FrontierLaunchArgs {
    void *unk0;
    Options *options;
    SaveData *saveData;
    BagCursor *bagCursor;
    u32 unk10;
    u32 unk14;
    u32 mapId;
    void *unk1C;
    u8 frontierMapId;
    FieldSystem *fieldSystem;
} FrontierLaunchArgs;

extern const OverlayManagerTemplate gOverlayTemplate_Frontier;

typedef void(UnkFuncPtr_020965A4)(void *);

FrontierLaunchArgs *Frontier_GetLaunchArgs(void *work);
void Frontier_SetData(void *work, u32 data);
void *Frontier_GetData(void *work);
void Frontier_LaunchApplication(void *work, const OverlayManagerTemplate *ovyTemp, void *args, u32 freeArgs, UnkFuncPtr_020965A4 func);

#endif // POKEHEARTGOLD_FRONTIER_H
