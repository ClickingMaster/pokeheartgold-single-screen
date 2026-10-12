#include "unk_02091564.h"

#include "main.h"
#include "oaks_speech.h"
#include "scrcmd_20.h"
#include "scrcmd_battle_arcade.h"
#include "scrcmd_battle_castle.h"
#include "communication_command.h"
#include "comm_player_manager.h"
#include "communication_club_link.h"
#include "unk_02058AEC.h"
#include "union_room_talk.h"

FS_EXTERN_OVERLAY(OVY_53);
const OverlayManagerTemplate gApplication_OakSpeech = { OakSpeech_Init, OakSpeech_Main, OakSpeech_Exit, FS_OVERLAY_ID(OVY_53) };

static const UnkStruct_02091564 sUnkTable_02106078[] = {
    { CommPlayerManager_ReceivePosition, CommPlayerManager_GetPositionSize,         NULL         },
    { CommPlayerManager_ReceiveHostPosition, CommPlayerManager_GetHostPositionSize,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { CommPlayerManager_ReceiveRemovePlayer, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_020573AC, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { CommClubLink_SetCardReceived, sub_02091588,         CommClubLink_GetCard },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02057D50, CommCmd_GetSize1,         NULL         },
    { sub_02058720, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_0205A408, PlayerProfile_sizeof, NULL         },
    { sub_0205A430, CommCmd_GetSize1,         NULL         },
    { sub_0205A40C, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvBattleChoice, CommCmd_GetSize1,         NULL         },
    { sub_0205A410, CommCmd_GetSize0,         NULL         },
    { UnionRoom_RecvTalkChoice, CommCmd_GetSize1,         NULL         },
    { UnionRoom_RecvTalkCancel, CommCmd_GetSize0,         NULL         },
    { UnionRoom_RecvTrainerCard, sub_02091588,         (UnkFuncPtr_02091564)UnionRoom_GetTrainerCardBuffer },
    { Colosseum_RecvMixBattleMons, Colosseum_GetMixBattleMonsSize,         Colosseum_GetMixBattleMonsBuffer },
    { Colosseum_RecvMixBattleChoice, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, sub_020915A8,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, sub_02091590,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, sub_02091598,         NULL         },
    { sub_02091570, sub_020915A0,         NULL         },
    { sub_02091570, sub_020915A4,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize1,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, sub_020915AC,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, CommCmd_GetSize0,         NULL         },
    { sub_02091570, sub_02091564,         NULL         },
    { sub_02091570, sub_02091568,         NULL         },
    { sub_0204F85C, CommCmd_GetSizeVariable,         NULL         },
    { sub_0204FE58, CommCmd_GetSizeVariable,         NULL         },
    { sub_02050398, CommCmd_GetSizeVariable,         NULL         }
};

u32 sub_02091564() {
    return 0x18;
}

u32 sub_02091568() {
    return 0x590;
}

void sub_02091570() {
}

void sub_02091574(void *arg0) {
    Comm_RegisterCommandTable(&sUnkTable_02106078, NELEMS(sUnkTable_02106078), arg0);
}

u32 sub_02091588() {
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
