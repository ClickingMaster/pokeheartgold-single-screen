#ifndef POKEHEARTGOLD_SCRCMD_BATTLE_CASTLE_H
#define POKEHEARTGOLD_SCRCMD_BATTLE_CASTLE_H

#include "global.h"

typedef struct BattleCastleTeamCheck {
    u8 state;
    u8 numReceived;
    u8 filler[2];
    u16 playerTeam[2];
    u16 partnerTeam[2];
    u16 *result;
} BattleCastleTeamCheck;

void BattleCastle_RecvPartnerTeam(u32, u32, BattleCastleTeamCheck *, BattleCastleTeamCheck *);

#endif // POKEHEARTGOLD_SCRCMD_BATTLE_CASTLE_H
