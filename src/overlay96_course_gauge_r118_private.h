#ifndef POKEHEARTGOLD_OVERLAY96_COURSE_GAUGE_R118_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY96_COURSE_GAUGE_R118_PRIVATE_H

#include <nitro/fx/fx_vec.h>

#include "global.h"

typedef union Ov96R118GaugeFlags {
    u32 raw;
    struct {
        u32 pad0 : 20;
        u32 field20 : 2;
        u32 field22 : 2;
        u32 pad24 : 2;
        u32 bit26 : 1;
        u32 bit27 : 1;
        u32 bit28 : 1;
        u32 bit29 : 1;
        u32 bit30 : 1;
        u32 bit31 : 1;
    } bits;
} Ov96R118GaugeFlags;

typedef struct Ov96R118Object {
    void *unk00;
    void *unk04;
    void *unk08;
    void *unk0C;
    void *unk10;
    s32 state;
    u8 pad18[8];
    VecFx32 vec20;
    VecFx32 vec2C;
    VecFx32 vec38;
    VecFx32 vec44;
    s32 unk50;
    s32 unk54;
    s8 unk58;
    s8 unk59;
    s8 unk5A;
    s8 unk5B;
    s8 unk5C;
    s8 unk5D;
    s8 unk5E;
    u8 unk5F;
    Ov96R118GaugeFlags flags;
} Ov96R118Object;

typedef struct Ov96R118Gauge {
    u32 unk00;
    void *config;
    s32 value;
    u32 unk0C;
} Ov96R118Gauge;
typedef struct Ov96R118Unit {
    u8 pad00[0x0C];
    Ov96R118Object object;
    u8 pad70[4];
    Ov96R118Gauge gauges[3];
    u8 padA4[4];
} Ov96R118Unit;

void ov96_02217FD0(Ov96R118Unit *);
void ov96_021E8228(void *, u8, u8, int, int);

#endif
