#include "field_comm_cmds.h"

#include "main.h"
#include "oaks_speech.h"
#include "scrcmd_battle_hall.h"
#include "scrcmd_battle_arcade.h"
#include "scrcmd_battle_castle.h"
#include "communication_command.h"
#include "comm_player_manager.h"
#include "communication_club_link.h"
#include "union_room.h"
#include "union_room_talk.h"

FS_EXTERN_OVERLAY(OVY_53);
const OverlayManagerTemplate gApplication_OakSpeech = { OakSpeech_Init, OakSpeech_Main, OakSpeech_Exit, FS_OVERLAY_ID(OVY_53) };

static const FieldCommCommand sFieldCommCommands[] = {
    { CommPlayerManager_ReceivePosition, CommPlayerManager_GetPositionSize,         NULL         },
    { CommPlayerManager_ReceiveHostPosition, CommPlayerManager_GetHostPositionSize,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommPlayerManager_ReceiveRemovePlayer, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommPlayerManager_ReceiveMoveEnabled, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommClubLink_SetCardReceived, CommCmd_GetTrainerCardSize,         CommClubLink_GetCard },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommPlayerManager_RecvUnkE6, CommCmd_GetSize1,         NULL         },
    { CommClubLink_RecvOnSpot, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvUnusedProfile, PlayerProfile_sizeof, NULL         },
    { UnionRoom_RecvTalkReply, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvUnused, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvBattleChoice, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvRestartSearch, CommCmd_GetSize0,         NULL         },
    { UnionRoom_RecvTalkChoice, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvTalkCancel, CommCmd_GetSize0,         NULL         },
    { UnionRoom_RecvTrainerCard, CommCmd_GetTrainerCardSize,         (FieldCommHandlerFunc)UnionRoom_GetTrainerCardBuffer },
    { Colosseum_RecvMixBattleMons, Colosseum_GetMixBattleMonsSize,         Colosseum_GetMixBattleMonsBuffer },
    { Colosseum_RecvMixBattleChoice, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize4,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize3008,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1008,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize10,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize50,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize4Copy,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize24,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1424,         NULL         },
    { BattleHall_RecvPartnerSpecies, CommCmd_GetSizeVariable,         NULL         },
    { BattleCastle_RecvPartnerTeam, CommCmd_GetSizeVariable,         NULL         },
    { BattleArcade_RecvPartnerTeam, CommCmd_GetSizeVariable,         NULL         }
};

u32 CommCmd_GetSize24() {
    return 0x18;
}

u32 CommCmd_GetSize1424() {
    return 0x590;
}

void CommCmd_RecvIgnored() {
}

void FieldComm_RegisterCommands(void *context) {
    Comm_RegisterCommandTable(&sFieldCommCommands, NELEMS(sFieldCommCommands), context);
}

u32 CommCmd_GetTrainerCardSize() {
    return 0x66C;
}

u32 CommCmd_GetSize3008() {
    return 0xBC0;
}

u32 CommCmd_GetSize1008() {
    return 0x3F0;
}

u32 CommCmd_GetSize10() {
    return 0xA;
}

u32 CommCmd_GetSize50() {
    return 0x32;
}

u32 CommCmd_GetSize4() {
    return 0x4;
}

u32 CommCmd_GetSize4Copy() {
    return 0x4;
}
