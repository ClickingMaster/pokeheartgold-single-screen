#include "unk_02091564.h"

#include "main.h"
#include "oaks_speech.h"
#include "scrcmd_battle_hall.h"
#include "scrcmd_battle_arcade.h"
#include "scrcmd_battle_castle.h"
#include "communication_command.h"
#include "comm_player_manager.h"
#include "communication_club_link.h"
#include "unk_02058AEC.h"
#include "union_room_talk.h"

FS_EXTERN_OVERLAY(OVY_53);
const OverlayManagerTemplate gApplication_OakSpeech = { OakSpeech_Init, OakSpeech_Main, OakSpeech_Exit, FS_OVERLAY_ID(OVY_53) };

static const UnkStruct_02091564 sFieldCommCommands[] = {
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
    { sub_02057D50, CommCmd_GetSize1,         NULL         },
    { sub_02058720, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { sub_0205A408, PlayerProfile_sizeof, NULL         },
    { UnionRoom_RecvTalkReply, CommCmd_GetSize1,         NULL         },
    { sub_0205A40C, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvBattleChoice, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvRestartSearch, CommCmd_GetSize0,         NULL         },
    { UnionRoom_RecvTalkChoice, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvTalkCancel, CommCmd_GetSize0,         NULL         },
    { UnionRoom_RecvTrainerCard, CommCmd_GetTrainerCardSize,         (UnkFuncPtr_02091564)UnionRoom_GetTrainerCardBuffer },
    { Colosseum_RecvMixBattleMons, Colosseum_GetMixBattleMonsSize,         Colosseum_GetMixBattleMonsBuffer },
    { Colosseum_RecvMixBattleChoice, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, sub_020915A8,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, sub_02091590,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, sub_02091598,         NULL         },
    { CommCmd_RecvIgnored, sub_020915A0,         NULL         },
    { CommCmd_RecvIgnored, sub_020915A4,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize1,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, sub_020915AC,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, CommCmd_GetSize0,         NULL         },
    { CommCmd_RecvIgnored, sub_02091564,         NULL         },
    { CommCmd_RecvIgnored, sub_02091568,         NULL         },
    { BattleHall_RecvPartnerSpecies, CommCmd_GetSizeVariable,         NULL         },
    { BattleCastle_RecvPartnerTeam, CommCmd_GetSizeVariable,         NULL         },
    { BattleArcade_RecvPartnerTeam, CommCmd_GetSizeVariable,         NULL         }
};

u32 sub_02091564() {
    return 0x18;
}

u32 sub_02091568() {
    return 0x590;
}

void CommCmd_RecvIgnored() {
}

void FieldComm_RegisterCommands(void *arg0) {
    Comm_RegisterCommandTable(&sFieldCommCommands, NELEMS(sFieldCommCommands), arg0);
}

u32 CommCmd_GetTrainerCardSize() {
    return 0x66C;
}

u32 sub_02091590() {
    return 0xBC0;
}

u32 sub_02091598() {
    return 0x3F0;
}

u32 sub_020915A0() {
    return 0xA;
}

u32 sub_020915A4() {
    return 0x32;
}

u32 sub_020915A8() {
    return 0x4;
}

u32 sub_020915AC() {
    return 0x4;
}
