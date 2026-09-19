#include "to52_r4_overlay12_battle_controller_r10_private.h"

typedef union StatusByte {
    u8 raw;
    struct {
        u8 status : 4;
        u8 bit4 : 1;
        u8 bit5 : 1;
        u8 rest : 2;
    } b;
} StatusByte;
typedef union BattleFlags {
    u32 raw;
    struct {
        u32 low : 21;
        u32 flag21 : 1;
        u32 skip : 2;
        u32 flag24 : 1;
        u32 high : 7;
    } b;
} BattleFlags;

typedef struct HealthbarCmd {
    u8 command;
    u8 battler;
    u16 hp;
    u16 maxHp;
    u8 pad_06;
    u8 status;
    u32 field_08;
    u32 expProgress;
    u32 expToNext;
} HealthbarCmd;

typedef struct ExpCmd {
    u8 command;
    u8 pad[3];
    u32 previous;
    u32 expProgress;
    u32 expToNext;
} ExpCmd;

typedef struct FaintCmd {
    u8 command;
    u8 form;
    u16 species;
    u32 field_04;
    u8 field_08;
    u8 flag24;
    u8 flag20;
    u8 pad_0B;
    u16 speciesAll[4];
    u8 formAll[4];
    u8 fieldAll[4];
    u8 low5All[4];
    u32 valueAll[4];
} FaintCmd;

void BattleController_EmitHealthbarUpdate(void *bs, void *ctx, int battler) {
    HealthbarCmd cmd;
    void *mon = BattleSystem_GetPartyMon(bs, battler, U8((u8 *)ctx + battler, 0x219C));
    u32 species = GetMonData(mon, 5, NULL);
    u32 level = GetMonData(mon, 0xA1, NULL);
    u32 off;
    cmd.command = 0x18;
    off = battler * 0xC0;
    cmd.battler = U8(ctx, 0x2D74 + off);
    cmd.hp = U32(ctx, 0x2D8C + off);
    cmd.maxHp = U32(ctx, 0x2D90 + off);
    cmd.field_08 = U32(ctx, 0x215C);
    if ((U16(ctx, 0x2D40 + off) == 0x1D || U16(ctx, 0x2D40 + off) == 0x20) && (U32(ctx, 0x2D54 + off) >> 31) == 0) {
        cmd.status = 2;
    } else {
        cmd.status = ((StatusByte *)((u8 *)ctx + 0x2DBE + off))->b.status;
    }
    cmd.expProgress = U32(ctx, 0x2DA4 + off) - GetMonExpBySpeciesAndLevel(species, level);
    cmd.expToNext = GetMonExpBySpeciesAndLevel(species, level + 1) - GetMonExpBySpeciesAndLevel(species, level);
    ov12_02262240(bs, 1, battler, &cmd, sizeof(cmd));
}

void ov12_02263564(void *bs, void *ctx, int battler, u32 previous) {
    ExpCmd cmd;
    void *mon = BattleSystem_GetPartyMon(bs, battler, U8((u8 *)ctx + battler, 0x219C));
    u32 species = GetMonData(mon, 5, NULL);
    u32 level = GetMonData(mon, 0xA1, NULL);
    cmd.command = 0x19;
    cmd.previous = previous;
    cmd.expProgress = U32((u8 *)ctx + battler * 0xC0, 0x2DA4) - GetMonExpBySpeciesAndLevel(species, level);
    cmd.expToNext = GetMonExpBySpeciesAndLevel(species, level + 1) - GetMonExpBySpeciesAndLevel(species, level);
    ov12_02262240(bs, 1, battler, &cmd, sizeof(cmd));
}
