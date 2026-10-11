#pragma force_active on
#include "global.h"
typedef struct OffsetTable {
    s16 values[6][6];
} OffsetTable;
typedef struct Entry0230 {
    u8 data[0xAC];
} Entry0230;
typedef struct Inner {
    u32 unk00;
    u32 base0;
    u32 base1;
    int unk0C;
} Inner;
typedef struct Holder {
    Inner *inner;
} Holder;
typedef struct Work0230 {
    u8 unk00[0x20];
    Holder *holder;
    u8 unk24[4];
    u32 value0;
    u32 value1;
    int value2;
} Work0230;
extern OffsetTable ov07_02235440;
extern OffsetTable ov07_02235488;
void *ParticleSystem_GetEmitterParam(void);
u32 ov07_0221C468(void *ctx);
u32 ov07_0221C470(void *ctx);
int ov07_02231924(void *ctx, u32 battler);
void ov07_02220230(Work0230 *work);

// @fn ov07_02220230
void ov07_02220230(Work0230 *work) {
    s16 unused[6] = { 0 };
    OffsetTable tableA;
    OffsetTable tableB;
    void *ctx;
    u32 attacker, defender;
    int kindA, kindB;
    u32 offA, offB;
    tableA = ov07_02235440;
    tableB = ov07_02235488;
    ctx = ParticleSystem_GetEmitterParam();
    attacker = ov07_0221C468(ctx);
    defender = ov07_0221C470(ctx);
    kindA = ov07_02231924(ctx, attacker);
    kindB = ov07_02231924(ctx, defender);
    offA = tableA.values[kindA][kindB] * sizeof(Entry0230);
    work->value0 = offA + work->holder->inner->base0;
    offB = tableB.values[kindA][kindB] * sizeof(Entry0230);
    work->value1 = offB + work->holder->inner->base1;
    work->value2 = work->holder->inner->unk0C;
}
