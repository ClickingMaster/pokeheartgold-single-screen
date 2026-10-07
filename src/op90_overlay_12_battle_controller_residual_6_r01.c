#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_controller.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "constants/species.h"

typedef struct PartyGaugeEntry {
    u8 selectedMonIndex;
    u8 gender : 2;
    u8 exists : 1;
    u8 unk1_3 : 5;
    u8 status;
    u8 unk3;
    u16 hp;
    u16 maxHp;
} PartyGaugeEntry;

typedef struct PartyGaugeMessage {
    u8 command;
    u8 unk1;
    u16 unk2;
    PartyGaugeEntry entries[4];
} PartyGaugeMessage;

void ov12_02262240(BattleSystem *bsys, int direction, int battlerId, const void *data, u8 size);
// @fn ov12_02262FFC
void ov12_02262FFC(BattleSystem *battleSystem, BattleContext *ctx, int a2, int battlerId) {
    PartyGaugeMessage msg;
    int i;
    u32 battleType;

    BattleBuffer_Clear(ctx, battlerId);
    battleType = BattleSystem_GetBattleType(battleSystem);
    msg.command = 16;
    msg.unk2 = a2;
    if (!(battleType & BATTLE_TYPE_DOUBLES) || (battleType & BATTLE_TYPE_MULTI) || ((battleType & BATTLE_TYPE_DOUBLES) && battlerId >= 2)) {
        msg.unk1 = 1;
    } else {
        msg.unk1 = 0;
    }
    for (i = 0; i < 4; i++) {
        if (ctx->battleMons[i].hp) {
            msg.entries[i].hp = ctx->battleMons[i].hp;
            msg.entries[i].maxHp = ctx->battleMons[i].maxHp;
            msg.entries[i].exists = TRUE;
            if ((ctx->battleMons[i].species == SPECIES_NIDORAN_F || ctx->battleMons[i].species == SPECIES_NIDORAN_M) && !ctx->battleMons[i].hasNickname) {
                msg.entries[i].gender = MON_GENDERLESS;
            } else {
                msg.entries[i].gender = ctx->battleMons[i].gender;
            }
            msg.entries[i].selectedMonIndex = ctx->selectedMonIndex[i];
            if (ctx->battleMons[i].status) {
                msg.entries[i].status = 3;
            } else {
                msg.entries[i].status = 1;
            }
        } else {
            msg.entries[i].exists = FALSE;
            msg.entries[i].status = 2;
        }
    }
    ov12_02262240(battleSystem, 1, battlerId, &msg, sizeof(PartyGaugeMessage));
}
