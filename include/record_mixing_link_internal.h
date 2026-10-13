#ifndef POKEHEARTGOLD_RECORD_MIXING_LINK_INTERNAL_H
#define POKEHEARTGOLD_RECORD_MIXING_LINK_INTERNAL_H
#include "global.h"

#include "math_util.h"
#include "sound.h"
#include "sound_playback.h"
#include "wireless_manager.h"
#include "communication_command.h"
#include "unk_02035900.h"
#include "comm_state.h"
typedef struct RecordMixingLinkWork {
    u8 pad0[0x384];
    u32 flag;
    u32 send[0x2EE];
    u32 checksum, random;
    u8 receive[5][0xBC0];
    u32 count, unk4a0c, remote;
    u8 active;
    u8 pad4a15[7];
    u32 participantCount, unk4a20, participantMask;
    u8 pad4a28[8];
    u16 assignedCount;
} RecordMixingLinkWork;
typedef struct RecordMixingJoinRequest {
    u8 sender, count, command, accepted;
} RecordMixingJoinRequest;
extern const u32 gRecordMixingLinkCommands[];
void RecordMixing_StartFlash(RecordMixingLinkWork *);
void RecordMixing_SetStateFromLink(RecordMixingLinkWork *, int, int);
void RecordMixing_SetStateFromLinkInMenu(RecordMixingLinkWork *, int, int);
int ov73_021E7488(void);
u32 MATH_CountPopulation(u32);
void CommState_SetMaxEntries(int);
BOOL sub_02036FD8(int, void *, int);
void RecordMixingLink_RegisterCommands(RecordMixingLinkWork *room);
void sub_0208FB78(int sender, int unused, void *data, RecordMixingLinkWork *room);
void sub_0208FB88(int sender, int unused, u8 *data, RecordMixingLinkWork *room);
void sub_0208FB98(int sender, int unused, void *data, RecordMixingLinkWork *room);
void sub_0208FBE0(int sender, int unused, void *data, RecordMixingLinkWork *room);
void RecordMixingLink_HandleJoinRequest(int sender, int unused, void *data, RecordMixingLinkWork *room);
void sub_0208FCDC(int sender, int unused, u8 *data, RecordMixingLinkWork *room);
void sub_0208FCFC(int sender, int unused, void *data, RecordMixingLinkWork *room);
void sub_0208FD00(int sender, int unused, void *data, RecordMixingLinkWork *room);
void sub_0208FD04(int sender, int unused, void *data, RecordMixingLinkWork *room);
void sub_0208FD1C(int sender);
void RecordMixingLink_SendRecords(RecordMixingLinkWork *room, int unused);
u32 sub_0208FD7C(void);
u32 sub_0208FD80(void);
u32 sub_0208FD84(void);
void *RecordMixingLink_GetRecvBuffer(int sender, RecordMixingLinkWork *room);

#endif
