#ifndef PRIVATE_OVERLAY_01_021E90C0_H
#define PRIVATE_OVERLAY_01_021E90C0_H

#include "global.h"

#include "constants/sndseq.h"

#include "field/area_light.h"
#include "nnsys/g3d/binres/res_struct.h"

#include "ascii_util.h"
#include "field_system.h"
#include "filesystem.h"
#include "gf_rtc.h"
#include "heap.h"
#include "map_header.h"
#include "screen_fade.h"
#include "unk_02055BF0.h"

typedef struct Struct021E90C0 {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u8 filler0C[0x10];
    u16 unk1C;
    u16 unk1E;
} Struct021E90C0;

typedef struct AreaLightTemplate {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
} AreaLightTemplate;

typedef struct DoorSEData {
    u32 unk00;
    u8 doorType; // 0x04
    u8 filler05[0x13];
} DoorSEData;

typedef struct Struct021E9F78 {
    u8 state; // 0x00
    u8 step;  // 0x01
    u16 pad02;
    u32 active; // 0x04
} Struct021E9F78;

/* No tracked prototypes exist for these. */
int GetDoorSE(FieldSystem *fieldSystem, int modelId, BOOL opening);
NARC *AreaDataManager_GetMapPropAnimListNarc(AreaDataManager *areaDataManager);
void PlaySE(u16 seq);

#endif
