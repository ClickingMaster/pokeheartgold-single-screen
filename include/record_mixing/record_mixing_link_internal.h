#ifndef POKEHEARTGOLD_RECORD_MIXING_LINK_INTERNAL_H
#define POKEHEARTGOLD_RECORD_MIXING_LINK_INTERNAL_H
#include "global.h"

#include "record_mixing/record_mixing_internal.h"

#include "comm_state.h"
#include "communication_command.h"
#include "communication_core.h"
#include "field_comm_cmds.h"
#include "math_util.h"
#include "sound.h"
#include "sound_playback.h"
#include "wireless_manager.h"
typedef struct RecordMixingJoinRequest {
    u8 sender, count, command, accepted;
} RecordMixingJoinRequest;
int RecordMixing_GetMemberCount(void);
u32 MATH_CountPopulation(u32);
void CommState_SetMaxEntries(int);
BOOL Comm_SendCommandNoCopy(int, void *, int);
void RecordMixingLink_SendRecords(RecordMixingApp *room, int unused);

#endif
