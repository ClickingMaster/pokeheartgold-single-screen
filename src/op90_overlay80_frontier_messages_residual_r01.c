#pragma force_active on
#include "global.h"
#include "options.h"
#include "frontier/frontier_system.h"
#include "frontier/frontier_map.h"

typedef struct FrontierLaunchArgs {
    u8 pad0[4];
    Options *options;
} FrontierLaunchArgs;

typedef struct FrontierMapPrivate {
    u8 pad0[8];
    void *unk8;
} FrontierMapPrivate;

typedef struct FrontierTextParam {
    u8 speed;
    u8 unk1;
    u8 unk2;
} FrontierTextParam;

FrontierLaunchArgs *Frontier_GetLaunchArgs(void *frontier);
void ov80_0222E2B8(FrontierSystem *system);
void ov80_0222E328(FrontierSystem *system, void *msgData, u16 msgId);
void ov80_0222E344(FrontierSystem *system, u8 unk2, u8 speed, u16 unk3, u8 unk1);
void ov80_0222E268(FrontierSystem *system, void *msgData, u16 msgId, u16 unk3, const FrontierTextParam *param);

// @fn ov80_0222E268
void ov80_0222E268(FrontierSystem *system, void *msgData, u16 msgId, u16 unk3, const FrontierTextParam *param) {
    u8 speed, unk1, unk2;

    ov80_0222E2B8(system);
    ov80_0222E328(system, msgData, msgId);
    if (param == NULL) {
        speed = Options_GetTextFrameDelay(Frontier_GetLaunchArgs(((FrontierMapPrivate *)FrontierSystem_GetFrontierMap(system))->unk8)->options);
        unk1 = 0;
        unk2 = 1;
    } else {
        speed = param->speed;
        unk1 = param->unk1;
        unk2 = param->unk2;
    }
    ov80_0222E344(system, unk2, speed, unk3, unk1);
}
