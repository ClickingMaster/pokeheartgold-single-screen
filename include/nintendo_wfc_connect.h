#ifndef POKEHEARTGOLD_NINTENDO_WFC_CONNECT_H
#define POKEHEARTGOLD_NINTENDO_WFC_CONNECT_H

#include "frontier_data.h"
#include "options.h"
#include "overlay_manager.h"
#include "sav_system_info.h"
#include "save.h"

typedef struct NintendoWifiConnectArgs {
    FrontierData *frontierData;
    u32 unk4; // BattleRoomDownload (Save_BattleRoomDownload_Get)
    SysInfo *sysInfo;
    SaveData *saveData;
    Options *options;
    void *unk14; // DWC user data, friend key source
    int unk18;   // 0: download a Battle Room, 1: upload the record, 2: check the records of a rank
    void *unk1C; // default DWC profile id
    int unk20;
    int unk24; // nonzero: connect right away
} NintendoWifiConnectArgs;

BOOL NintendoWfc_Init(OverlayManager *man, int *state);
BOOL NintendoWfc_Main(OverlayManager *man, int *state);
BOOL NintendoWfc_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_NINTENDO_WFC_CONNECT_H
