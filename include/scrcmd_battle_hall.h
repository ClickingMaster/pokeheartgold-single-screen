#ifndef POKEHEARTGOLD_SCRCMD_BATTLE_HALL_H
#define POKEHEARTGOLD_SCRCMD_BATTLE_HALL_H

#include "save.h"

typedef struct BattleHallSpeciesCheck {
    u8 state;
    u8 numReceived;
    u8 filler[2];
    u16 playerTeam;
    u16 partnerTeam;
    u16 *result;
} BattleHallSpeciesCheck;

void BattleHall_RecvPartnerSpecies(u32, u32, BattleHallSpeciesCheck *, BattleHallSpeciesCheck *);
void BattleHall_ResetStreak(SaveData *, u32, u8);

#endif // POKEHEARTGOLD_SCRCMD_BATTLE_HALL_H
