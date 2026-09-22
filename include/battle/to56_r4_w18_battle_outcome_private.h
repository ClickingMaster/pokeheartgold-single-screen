#ifndef TO56_R4_W18_PRIVATE_H
#define TO56_R4_W18_PRIVATE_H
#include "global.h"

#include "battle/battle_system.h"

#include "party.h"
#include "pokemon.h"
typedef struct BattleCommandR4W18 {
    u8 command;
    u8 pad;
    u16 arg;
    u32 outcome;
    u8 payload[1];
} BattleCommandR4W18;
typedef struct OpponentDataR4W18 {
    u8 raw[0x198];
} OpponentDataR4W18;
void ov12_022597EC(BattleSystem *, OpponentDataR4W18 *);
void ov12_0223BF14(BattleSystem *, u16, u8 *);
void ov12_0226430C(BattleSystem *, int, int);
void ov12_02259928(OpponentDataR4W18 *);
#endif
