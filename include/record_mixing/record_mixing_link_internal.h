#ifndef POKEHEARTGOLD_RECORD_MIXING_LINK_INTERNAL_H
#define POKEHEARTGOLD_RECORD_MIXING_LINK_INTERNAL_H
#include "global.h"

#include "comm_state.h"
#include "communication_command.h"
#include "communication_core.h"
#include "field_comm_cmds.h"
#include "math_util.h"
#include "sound.h"
#include "sound_playback.h"
#include "wireless_manager.h"
typedef struct RecordMixingLinkWork {
    u8 pad0[0x384];
    u32 flag;
    u32 send[0x2EE];
    u32 checksum, random;
    u8 receive[5][0xBC0];
    u32 recordsReceived, unk4a0c, leaderJoinPending;
    u8 isMixing;
    u8 pad4a15[7];
    u32 participantCount, joinPending, leavingMask;
    u8 pad4a28[8];
    u16 assignedCount;
} RecordMixingLinkWork;
typedef struct RecordMixingJoinRequest {
    u8 sender, count, command, accepted;
} RecordMixingJoinRequest;
void RecordMixing_StartFlash(RecordMixingLinkWork *);
void RecordMixing_SetStateFromLink(RecordMixingLinkWork *, int, int);
void RecordMixing_SetStateFromLinkInMenu(RecordMixingLinkWork *, int, int);
int RecordMixing_GetMemberCount(void);
u32 MATH_CountPopulation(u32);
void CommState_SetMaxEntries(int);
BOOL Comm_SendCommandNoCopy(int, void *, int);
void RecordMixingLink_RegisterCommands(RecordMixingLinkWork *room);
void RecordMixingLink_SendRecords(RecordMixingLinkWork *room, int unused);

#endif
