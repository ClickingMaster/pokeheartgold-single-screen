#ifndef POKEHEARTGOLD_SCRCMD_BATTLE_ARCADE_H
#define POKEHEARTGOLD_SCRCMD_BATTLE_ARCADE_H

#include "global.h"

typedef struct BattleArcadeTeamCheck {
    u8 state;
    u8 numReceived;
    u8 filler[2];
    u16 playerTeam[2];
    u16 partnerTeam[2];
    u16 *result;
} BattleArcadeTeamCheck;

void BattleArcade_RecvPartnerTeam(u32, u32, BattleArcadeTeamCheck *, BattleArcadeTeamCheck *);

#endif // POKEHEARTGOLD_SCRCMD_BATTLE_ARCADE_H
