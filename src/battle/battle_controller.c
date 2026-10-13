#include "battle/battle_controller.h"

#include "global.h"

#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/pokemon.h"
#include "constants/species.h"

#include "battle/battle.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "battle_link_comm.h"
#include "move.h"
#include "party.h"
#include "pokemon.h"
#include "battle_recording.h"
#include "communication_core.h"

// Packets sent by the BattleController_Emit* functions. The first byte is the command that
// OpponentData_RunCommand dispatches on.

typedef struct BattleControllerPacketEncounterAnimation {
    u32 command;
    u32 randTemp;
} BattleControllerPacketEncounterAnimation;

typedef struct BattleControllerPacketPokemonEncounter {
    u8 command;
    u8 gender : 2;
    u8 shiny : 1;
    u8 form : 5;
    u16 species;
    u32 personality;
    u32 battlerType;
    u16 moves[MAX_MON_MOVES];
    u16 movePPCur[MAX_MON_MOVES];
    u16 movePP[MAX_MON_MOVES];
    u16 nickname[POKEMON_NAME_LENGTH + 1];
} BattleControllerPacketPokemonEncounter;

// Shared by the slide-in (3) and send-out (4) commands
typedef struct BattleControllerPacketPokemonSendOut {
    u8 command;
    u8 gender : 2;
    u8 shiny : 1;
    u8 form : 5;
    u16 species;
    u32 personality;
    u32 battlerType;
    u32 selectedMonIndex;
    u32 ball;
    u32 breakOut;
    u16 moves[MAX_MON_MOVES];
    u16 movePPCur[MAX_MON_MOVES];
    u16 movePP[MAX_MON_MOVES];
    u16 nickname[POKEMON_NAME_LENGTH + 1];
    u8 filler_46[2];
    u32 partnerSelectedMonIndex;
    u32 isSubstitute;
    u16 allSpecies[BATTLER_MAX];
    u8 allGender[BATTLER_MAX];
    u8 allShiny[BATTLER_MAX];
    u8 allForm[BATTLER_MAX];
    u32 allPersonality[BATTLER_MAX];
} BattleControllerPacketPokemonSendOut;

typedef struct BattleControllerPacketRecallPokemon {
    u8 command;
    u8 height;
    u16 ball;
    u32 isSubstitute;
    u16 allSpecies[BATTLER_MAX];
    u8 allGender[BATTLER_MAX];
    u8 allShiny[BATTLER_MAX];
    u8 allForm[BATTLER_MAX];
    u32 allPersonality[BATTLER_MAX];
    u32 selectedMonIndex;
} BattleControllerPacketRecallPokemon;

typedef struct BattleControllerPacketPokemonEnterBall {
    u8 command;
    u8 height;
    u16 ball;
} BattleControllerPacketPokemonEnterBall;

typedef struct BattleControllerPacketTrainerEncounter {
    u8 command;
    u8 trainerGender;
    u16 trainerClass;
} BattleControllerPacketTrainerEncounter;

typedef struct BattleControllerPacketThrowPokeball {
    u8 command;
    u8 throwType;
    u16 partnerSelectedMonIndex;
} BattleControllerPacketThrowPokeball;

typedef struct BattleControllerPacketTrainerSlideIn {
    u8 command;
    u8 trainerGender;
    u16 trainerClass;
    u32 index;
} BattleControllerPacketTrainerSlideIn;

typedef struct BattleControllerPacketHealthbarSlideIn {
    u8 command;
    u8 level;
    u16 hp;
    u16 maxHp;
    u8 selectedMonIndex;
    u8 status : 5;
    u8 gender : 2;
    u8 caught : 1;
    u32 exp;
    u32 maxExp;
    u32 safariBallCount;
    u8 delay;
    u8 filler_15[3];
} BattleControllerPacketHealthbarSlideIn;

typedef struct BattleControllerPacketSelectCommand {
    u8 command;
    u8 partySlot;
    u8 expPercents[PARTY_SIZE];
    u8 ballStatus[2][PARTY_SIZE];
    u16 moves[MAX_MON_MOVES];
    u8 movePPCur[MAX_MON_MOVES];
    u8 movePP[MAX_MON_MOVES];
    s16 hp;
    u16 maxHp;
    u8 ballStatusBattler;
    u8 switchingOrCanPickCommandMask;
    u16 unk_2A;
} BattleControllerPacketSelectCommand;

typedef struct BattleControllerPacketSelectMove {
    u8 command;
    u8 selectedMonIndex;
    u16 unusableMovesMask;
    u16 moves[MAX_MON_MOVES];
    u8 movePPCur[MAX_MON_MOVES];
    u8 movePP[MAX_MON_MOVES];
} BattleControllerPacketSelectMove;

typedef struct BattleControllerTargetGauge {
    u8 selectedMonIndex;
    u8 gender : 2;
    u8 exists : 1;
    u8 unk_01_3 : 5;
    u8 status;
    u8 unk_03;
    u16 hp;
    u16 maxHp;
} BattleControllerTargetGauge;

typedef struct BattleControllerPacketSelectTarget {
    u8 command;
    u8 unk_01;
    u16 moveRange;
    BattleControllerTargetGauge gauges[BATTLER_MAX];
} BattleControllerPacketSelectTarget;

typedef struct BattleControllerPacketSelectItem {
    u8 command;
    u8 unk_01;
    u8 targetSemiInvulnerable;
    u8 targetSubstitute;
    u8 selectedMonIndex[BATTLER_MAX];
    u8 partyOrder[BATTLER_MAX][PARTY_SIZE];
    u8 embargoFlag[BATTLER_MAX];
} BattleControllerPacketSelectItem;

typedef struct BattleControllerPacketShowMonList {
    u8 command;
    u8 battlerId;
    u8 mode;
    u8 reservedMon;
    u8 selectedMonIndex[BATTLER_MAX];
    u8 partyOrder[BATTLER_MAX][PARTY_SIZE];
    u32 trapped;
    u8 switchInFlag;
    u8 filler_29[3];
} BattleControllerPacketShowMonList;

typedef struct BattleControllerPacketYesNoBox {
    u8 command;
    u8 mode;
    u16 msgId;
    u32 value;
    u32 param;
} BattleControllerPacketYesNoBox;

typedef struct BattleControllerPacketAttackMessage {
    u8 command;
    u8 selectedMonIndex;
    u16 move;
} BattleControllerPacketAttackMessage;

typedef struct BattleControllerPacketHealthbarUpdate {
    u8 command;
    u8 level;
    u16 hp;
    u16 maxHp;
    u8 unk_06;
    u8 gender;
    u32 hpCalc;
    u32 exp;
    u32 maxExp;
} BattleControllerPacketHealthbarUpdate;

typedef struct BattleControllerPacketExpBarUpdate {
    u8 command;
    u8 filler_01[3];
    u32 prevExp;
    u32 exp;
    u32 maxExp;
} BattleControllerPacketExpBarUpdate;

typedef struct BattleControllerPacketFaintAnimation {
    u8 command;
    u8 gender;
    u16 species;
    u32 personality;
    u8 form;
    u8 isSubstitute;
    u8 isTransformed;
    u8 unk_0B;
    u16 allSpecies[BATTLER_MAX];
    u8 allGender[BATTLER_MAX];
    u8 allShiny[BATTLER_MAX];
    u8 allForm[BATTLER_MAX];
    u32 allPersonality[BATTLER_MAX];
} BattleControllerPacketFaintAnimation;

typedef struct BattleControllerPacketPlaySE {
    u8 command;
    u8 unk_01;
    u16 sndSeqNo;
} BattleControllerPacketPlaySE;

typedef struct BattleControllerPacketToggleVanish {
    u8 command;
    u8 vanish;
    u8 isSubstitute;
    u8 unk_03;
    u16 allSpecies[BATTLER_MAX];
    u8 allGender[BATTLER_MAX];
    u8 allShiny[BATTLER_MAX];
    u8 allForm[BATTLER_MAX];
    u32 allPersonality[BATTLER_MAX];
} BattleControllerPacketToggleVanish;

typedef struct BattleControllerPacketHealthbarStatus {
    u8 command;
    u8 status;
    u16 unk_02;
} BattleControllerPacketHealthbarStatus;

typedef struct BattleControllerPacketTrainerMessage {
    u8 command;
    u8 msg;
    u16 unk_02;
} BattleControllerPacketTrainerMessage;

// Recall (32) and send-out (33) messages; the value grades the enemy's remaining HP
typedef struct BattleControllerPacketSwitchMessage {
    u8 command;
    u8 selectedMonIndex;
    u16 value;
} BattleControllerPacketSwitchMessage;

typedef struct BattleControllerPacketFirstSendOutMessage {
    u8 command;
    u8 filler_01[3];
    u8 selectedMonIndex[BATTLER_MAX];
} BattleControllerPacketFirstSendOutMessage;

typedef struct BattleControllerPacketSelectionMessage {
    u8 command;
    u8 filler_01[3];
    BattleMessage msg;
} BattleControllerPacketSelectionMessage;

typedef struct BattleControllerPacketRefreshHealthbar {
    u8 command;
    u8 level;
    u16 hp;
    u16 maxHp;
    u8 selectedMonIndex;
    u8 status : 5;
    u8 gender : 2;
    u8 caught : 1;
    u32 exp;
    u32 maxExp;
    u32 safariBallCount;
} BattleControllerPacketRefreshHealthbar;

typedef struct BattleControllerPacketBattleMonToPartyMonCopy {
    u8 command;
    u8 selectedMonIndex : 4;
    u8 mimicedMoveIndex : 4;
    s16 hp;
    u32 status;
    u32 battlerBitKnockedOffItem;
    u16 item;
    u16 moves[MAX_MON_MOVES];
    u8 movePPCur[MAX_MON_MOVES];
    u8 filler_1A[2];
    u32 status2;
    u16 form;
    u16 unk_22;
    u32 ability;
    u16 recalcMonStats;
    u16 formChange;
} BattleControllerPacketBattleMonToPartyMonCopy;

typedef struct BattleControllerPacketPartyStatusHeal {
    u8 command;
    u8 ability;
    u16 moveNo;
} BattleControllerPacketPartyStatusHeal;

typedef struct BattleControllerPacketSelectMoveToForget {
    u8 command;
    u8 slot;
    u16 move;
} BattleControllerPacketSelectMoveToForget;

typedef struct BattleControllerPacketMosaicAnimation {
    u8 command;
    u8 mosaic;
    u8 delay;
    u8 unk_03;
} BattleControllerPacketMosaicAnimation;

typedef struct BattleControllerPacketChangeForm {
    u8 command;
    u8 form;
    u16 species;
    u8 gender;
    u8 shiny;
    u16 unk_06;
    u32 personality;
} BattleControllerPacketChangeForm;

typedef struct BattleControllerPacketBallGauge {
    u8 command;
    u8 unk_01;
    u8 status[PARTY_SIZE];
} BattleControllerPacketBallGauge;

typedef struct BattleControllerPacketIncrementGameStat {
    u8 command;
    u8 flag;
    u16 id;
} BattleControllerPacketIncrementGameStat;

// Commands that also carry the record data of a recorded link battle (see BattleSystem_PackRecordUpdate)
typedef struct BattleControllerPacketRecordData {
    u8 command;
    u8 escapedMask;
    u16 recordSize;
    u8 record[28];
} BattleControllerPacketRecordData;

typedef struct BattleControllerPacketMoveSE {
    u8 command;
    u8 effectiveness;
    u8 filler_02[2];
} BattleControllerPacketMoveSE;

typedef struct BattleControllerPacketPlaySong {
    u8 command;
    u8 unk_01;
    u16 song;
} BattleControllerPacketPlaySong;

typedef struct BattleControllerPacketBattleResults {
    u8 command;
    u8 unk_01;
    vu16 recordSize; // volatile only to match: the assert re-reads the size from the packet
    u32 outcomeFlags;
    u8 record[28];
} BattleControllerPacketBattleResults;

typedef struct BattleControllerPacketCommandComplete {
    u8 command;
    u8 netId;
    u16 unk_02;
} BattleControllerPacketCommandComplete;

typedef struct BattleControllerPacketScreenFade {
    u8 command;
    u8 mode;
    u16 unk_02;
} BattleControllerPacketScreenFade;

void BattleController_InitBallGaugePacket(BattleSystem *battleSystem, BattleContext *ctx, BattleControllerPacketBallGauge *packet, int command, int battlerId);

#pragma force_active on

void BattleController_SendData(BattleSystem *battleSystem, int direction, int battlerId, const u8 *data, u8 size) {
    u8 *buffer;
    u16 *writePos;
    u16 *endPos;
    BattleLinkHeader header;
    u32 i;
    int j;
    u8 *headerBytes;

    if (direction == 1) {
        buffer = BattleSystem_GetRecvBufferPtr(battleSystem);
        writePos = BattleSystem_GetRecvBufferWritePos(battleSystem);
        endPos = BattleSystem_GetRecvBufferEndPos(battleSystem);
    } else {
        buffer = BattleSystem_GetSendBufferPtr(battleSystem);
        writePos = BattleSystem_GetSendBufferWritePos(battleSystem);
        endPos = BattleSystem_GetSendBufferEndPos(battleSystem);
    }
    if (*writePos + 5u + size > 0x1000) {
        *endPos = *writePos;
        *writePos = 0;
    }
    header.command = direction;
    header.battler = battlerId;
    header.size = size;
    headerBytes = (u8 *)&header;
    for (i = 0; i < sizeof(BattleLinkHeader); i++) {
        buffer[*writePos] = headerBytes[i];
        (*writePos)++;
    }
    for (j = 0; j < size; j++) {
        buffer[*writePos] = data[j];
        (*writePos)++;
    }
}

BOOL BattleController_RecvData(BattleSystem *battleSystem, const u8 *data) {
    int ret = FALSE;
    int direction = data[0];
    int battlerId = data[1];
    int size = data[2] | (data[3] << 8);

    if (direction == 0) {
        if (battleSystem->ctx->battleBuffer[battlerId][0] == 0) {
            for (ret = 0; ret < size; ret++) {
                battleSystem->ctx->battleBuffer[battlerId][ret] = data[4 + ret];
            }
            ret = TRUE;
        }
    } else if (direction == 1) {
        if (battleSystem->opponentData[battlerId]->unk8C[8] == 0) {
            for (ret = 0; ret < size; ret++) {
                battleSystem->opponentData[battlerId]->unk8C[8 + ret] = data[4 + ret];
            }
            ret = TRUE;
        }
    } else if (direction == 2) {
        int command = data[4];
        int netId = data[5];
        if (BattleSystem_IsServer(battleSystem)) {
            BattleContext_RemoveLinkBufferEntry(battleSystem->ctx, netId, battlerId, command);
        }
        ret = TRUE;
    }
    return ret;
}

#pragma force_active reset

void BattleController_ProcessQueue(BattleSystem *battleSystem, int direction) {
    u8 *buffer;
    u16 *readPos;
    u16 *writePos;
    u16 *endPos;

    if (direction == 1) {
        buffer = BattleSystem_GetRecvBufferPtr(battleSystem);
        readPos = BattleSystem_GetRecvBufferReadPos(battleSystem);
        writePos = BattleSystem_GetRecvBufferWritePos(battleSystem);
        endPos = BattleSystem_GetRecvBufferEndPos(battleSystem);
    } else {
        buffer = BattleSystem_GetSendBufferPtr(battleSystem);
        readPos = BattleSystem_GetSendBufferReadPos(battleSystem);
        writePos = BattleSystem_GetSendBufferWritePos(battleSystem);
        endPos = BattleSystem_GetSendBufferEndPos(battleSystem);
    }
    if (*readPos != *writePos) {
        if (*readPos == *endPos) {
            *readPos = 0;
            *endPos = 0;
        }
        if (BattleController_RecvData(battleSystem, &buffer[*readPos]) == TRUE) {
            *readPos += (buffer[*readPos + 2] | (buffer[*readPos + 3] << 8)) + sizeof(BattleLinkHeader);
        }
    }
}

void BattleController_EmitPacket(BattleSystem *battleSystem, int direction, int battlerId, const void *data, u8 size) {
    int i;

    if ((battleSystem->battleType & BATTLE_TYPE_LINK) && !(battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING)) {
        if (direction == 1) {
            for (i = 0; i < Comm_CountConnected(); i++) {
                BattleContext_AddLinkBufferEntry(battleSystem->ctx, i, battlerId, *(const u8 *)data);
            }
        }
        BattleLink_QueueSendData(battleSystem, direction, battlerId, (u8 *)data, size);
    } else {
        if (direction == 1) {
            BattleContext_AddLinkBufferEntry(battleSystem->ctx, 0, battlerId, *(const u8 *)data);
        }
        BattleController_SendData(battleSystem, direction, battlerId, data, size);
    }
}

void BattleController_EmitPlayEncounterAnimation(BattleSystem *battleSystem, BOOL battlerId) {
    BattleControllerPacketEncounterAnimation packet;

    packet.command = 1;
    packet.randTemp = BattleSystem_GetRandTemp(battleSystem);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitPokemonEncounter(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketPokemonEncounter packet;
    int i;

    packet.command = 2;
    packet.gender = battleSystem->ctx->battleMons[battlerId].gender;
    packet.shiny = battleSystem->ctx->battleMons[battlerId].shiny;
    packet.species = battleSystem->ctx->battleMons[battlerId].species;
    packet.personality = battleSystem->ctx->battleMons[battlerId].personality;
    packet.battlerType = ov12_02256748(battleSystem->ctx, battlerId, BattleSystem_GetBattlerType(battleSystem, battlerId), TRUE);
    packet.form = battleSystem->ctx->battleMons[battlerId].form;
    for (i = 0; i < MAX_MON_MOVES; i++) {
        packet.moves[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_MOVE1 + i, NULL);
        packet.movePPCur[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_CUR_PP_1 + i, NULL);
        packet.movePP[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_MAX_PP_1 + i, NULL);
    }
    GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_NICKNAME, packet.nickname);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitPokemonSlideIn(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketPokemonSendOut packet;
    int i;

    packet.command = 3;
    packet.gender = battleSystem->ctx->battleMons[battlerId].gender;
    packet.shiny = battleSystem->ctx->battleMons[battlerId].shiny;
    packet.species = battleSystem->ctx->battleMons[battlerId].species;
    packet.personality = battleSystem->ctx->battleMons[battlerId].personality;
    packet.battlerType = ov12_02256748(battleSystem->ctx, battlerId, BattleSystem_GetBattlerType(battleSystem, battlerId), TRUE);
    packet.selectedMonIndex = battleSystem->ctx->selectedMonIndex[battlerId];
    packet.form = battleSystem->ctx->battleMons[battlerId].form;
    packet.ball = battleSystem->ctx->battleMons[battlerId].ball;
    packet.partnerSelectedMonIndex = battleSystem->ctx->selectedMonIndex[BattleSystem_GetBattlerIdPartner(battleSystem, battlerId)];
    BattleSystem_FlagMonBattled(battleSystem, battlerId, packet.selectedMonIndex);
    for (i = 0; i < MAX_MON_MOVES; i++) {
        packet.moves[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_MOVE1 + i, NULL);
        packet.movePPCur[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_CUR_PP_1 + i, NULL);
        packet.movePP[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_MAX_PP_1 + i, NULL);
    }
    GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_NICKNAME, packet.nickname);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active on

void BattleController_EmitPokemonSendOut(BattleSystem *battleSystem, int battlerId, int ball, int a3) {
    BattleControllerPacketPokemonSendOut packet;

    packet.command = 4;
    if (battleSystem->ctx->battleMons[battlerId].status2 & STATUS2_TRANSFORM) {
        packet.gender = (u8)battleSystem->ctx->battleMons[battlerId].moveEffectData.transformGender;
        packet.personality = battleSystem->ctx->battleMons[battlerId].moveEffectData.transformPersonality;
    } else {
        packet.gender = battleSystem->ctx->battleMons[battlerId].gender;
        packet.personality = battleSystem->ctx->battleMons[battlerId].personality;
    }
    packet.shiny = battleSystem->ctx->battleMons[battlerId].shiny;
    packet.species = battleSystem->ctx->battleMons[battlerId].species;
    packet.battlerType = ov12_02256748(battleSystem->ctx, battlerId, BattleSystem_GetBattlerType(battleSystem, battlerId), FALSE);
    packet.selectedMonIndex = battleSystem->ctx->selectedMonIndex[battlerId];
    packet.form = battleSystem->ctx->battleMons[battlerId].form;
    if (ball) {
        packet.ball = ball;
    } else {
        packet.ball = battleSystem->ctx->battleMons[battlerId].ball;
    }
    packet.breakOut = a3;
    packet.isSubstitute = !!(battleSystem->ctx->battleMons[battlerId].status2 & STATUS2_SUBSTITUTE);
    BattleSystem_FlagMonBattled(battleSystem, battlerId, packet.selectedMonIndex);
    for (int i = 0; i < MAX_MON_MOVES; i++) {
        packet.moves[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_MOVE1 + i, NULL);
        packet.movePPCur[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_CUR_PP_1 + i, NULL);
        packet.movePP[i] = GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_MAX_PP_1 + i, NULL);
    }
    GetBattlerVar(battleSystem->ctx, battlerId, BMON_DATA_NICKNAME, packet.nickname);
    for (int i = 0; i < BATTLER_MAX; i++) {
        packet.allSpecies[i] = battleSystem->ctx->battleMons[i].species;
        packet.allShiny[i] = battleSystem->ctx->battleMons[i].shiny;
        packet.allForm[i] = battleSystem->ctx->battleMons[i].form;
        if (battleSystem->ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            packet.allGender[i] = battleSystem->ctx->battleMons[i].moveEffectData.transformGender;
            packet.allPersonality[i] = battleSystem->ctx->battleMons[i].moveEffectData.transformPersonality;
        } else {
            packet.allGender[i] = battleSystem->ctx->battleMons[i].gender;
            packet.allPersonality[i] = battleSystem->ctx->battleMons[i].personality;
        }
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitRecallPokemon(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    BattleControllerPacketRecallPokemon packet;
    int facing;

    if (battleSystem->opponentData[battlerId]->battlerType & 1) {
        facing = 2;
    } else {
        facing = 0;
    }
    int form = battleSystem->ctx->battleMons[battlerId].form;
    packet.command = 5;
    if (battleSystem->ctx->battleMons[battlerId].status2 & STATUS2_TRANSFORM) {
        packet.height = GetMonPicHeightBySpeciesGenderForm(battleSystem->ctx->battleMons[battlerId].species, battleSystem->ctx->battleMons[battlerId].moveEffectData.transformGender, facing, form, battleSystem->ctx->battleMons[battlerId].moveEffectData.transformPersonality);
    } else {
        packet.height = GetMonPicHeightBySpeciesGenderForm(battleSystem->ctx->battleMons[battlerId].species, battleSystem->ctx->battleMons[battlerId].gender, facing, form, battleSystem->ctx->battleMons[battlerId].personality);
    }
    packet.ball = battleSystem->ctx->battleMons[battlerId].ball;
    packet.isSubstitute = !!(battleSystem->ctx->battleMons[battlerId].status2 & STATUS2_SUBSTITUTE);
    packet.selectedMonIndex = battleSystem->ctx->selectedMonIndex[battlerId];
    for (int i = 0; i < BATTLER_MAX; i++) {
        packet.allSpecies[i] = ctx->battleMons[i].species;
        packet.allShiny[i] = ctx->battleMons[i].shiny;
        packet.allForm[i] = ctx->battleMons[i].form;
        if (ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            packet.allGender[i] = ctx->battleMons[i].moveEffectData.transformGender;
            packet.allPersonality[i] = ctx->battleMons[i].moveEffectData.transformPersonality;
        } else {
            packet.allGender[i] = ctx->battleMons[i].gender;
            packet.allPersonality[i] = ctx->battleMons[i].personality;
        }
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitPokemonEnterBall(BattleSystem *battleSystem, s32 battlerId, s32 ball) {
    BattleControllerPacketPokemonEnterBall packet;
    int form;
    int facing;

    facing = battleSystem->opponentData[battlerId]->battlerType & 1 ? 2 : 0;
    form = battleSystem->ctx->battleMons[battlerId].form;
    packet.command = 6;
    if (battleSystem->ctx->battleMons[battlerId].status2 & STATUS2_TRANSFORM) {
        packet.height = GetMonPicHeightBySpeciesGenderForm(battleSystem->ctx->battleMons[battlerId].species, battleSystem->ctx->battleMons[battlerId].moveEffectData.transformGender, facing, form, battleSystem->ctx->battleMons[battlerId].moveEffectData.transformPersonality);
    } else {
        packet.height = GetMonPicHeightBySpeciesGenderForm(battleSystem->ctx->battleMons[battlerId].species, battleSystem->ctx->battleMons[battlerId].gender, facing, form, battleSystem->ctx->battleMons[battlerId].personality);
    }
    packet.ball = ball;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitDeletePokemon(BattleSystem *battleSystem, int battlerId) {
    u32 command = 7;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

#pragma force_active on

void BattleController_EmitTrainerEncounter(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketTrainerEncounter packet;

    packet.command = 8;
    packet.trainerClass = battleSystem->trainers[battlerId].data.trainerClass;
    packet.trainerGender = battleSystem->trainerGender[battlerId];
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitThrowPokeball(BattleSystem *battleSystem, int battlerId, u32 throwType) {
    BattleControllerPacketThrowPokeball packet;

    packet.command = 9;
    packet.throwType = throwType;
    packet.partnerSelectedMonIndex = battleSystem->ctx->selectedMonIndex[BattleSystem_GetBattlerIdPartner(battleSystem, battlerId)];
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitTrainerSlideOut(BattleSystem *battleSystem, int battlerId) {
    u32 command = 10;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

#pragma force_active on

void BattleController_EmitTrainerSlideIn(BattleSystem *battleSystem, int battlerId, u32 index) {
    BattleControllerPacketTrainerSlideIn packet;

    packet.command = 11;
    packet.trainerClass = battleSystem->trainers[battlerId].data.trainerClass;
    packet.trainerGender = battleSystem->trainerGender[battlerId];
    packet.index = index;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitHealthbarSlideIn(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, u32 delay) {
    BattleControllerPacketHealthbarSlideIn packet;
    Pokemon *mon = BattleSystem_GetPartyMon(battleSystem, battlerId, ctx->selectedMonIndex[battlerId]);
    int species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    int level = GetMonData(mon, MON_DATA_LEVEL, NULL);

    packet.command = 12;
    packet.level = ctx->battleMons[battlerId].level;
    packet.hp = ctx->battleMons[battlerId].hp;
    packet.maxHp = ctx->battleMons[battlerId].maxHp;
    packet.selectedMonIndex = ctx->selectedMonIndex[battlerId];
    packet.status = (u8)GetBattlerStatusCondition(ctx, battlerId);
    if ((ctx->battleMons[battlerId].species == SPECIES_NIDORAN_F || ctx->battleMons[battlerId].species == SPECIES_NIDORAN_M) && !ctx->battleMons[battlerId].hasNickname) {
        packet.gender = MON_GENDERLESS;
    } else {
        packet.gender = ctx->battleMons[battlerId].gender;
    }
    packet.exp = ctx->battleMons[battlerId].exp - GetMonExpBySpeciesAndLevel(species, level);
    int nextLevelExp = GetMonExpBySpeciesAndLevel(species, level + 1);
    packet.maxExp = nextLevelExp - GetMonExpBySpeciesAndLevel(species, level);
    packet.caught = (u8)BattleSystem_CheckMonCaught(battleSystem, ctx->battleMons[battlerId].species);
    packet.safariBallCount = BattleSystem_GetSafariBallCount(battleSystem);
    packet.delay = delay;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitHealthbarSlideOut(BattleSystem *battleSystem, int battlerId) {
    u32 command = 13;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

void BattleController_EmitSelectCommand(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int partySlot) {
    BattleControllerPacketSelectCommand packet;
    int i;
    int battler;
    int species;
    int count;
    Party *party;
    Pokemon *mon;
    u32 battleType;
    int unavailableMask;

    MI_CpuClearFast(&packet, sizeof(packet));
    BattleBuffer_Clear(BattleSystem_GetBattleContext(battleSystem), battlerId);

    unavailableMask = 0;
    for (i = 0; i < BattleSystem_GetMaxBattlers(battleSystem); i++) {
        if (Battler_CanSelectAction(ctx, i) == FALSE) {
            unavailableMask |= MaskOfFlagNo(i);
        }
    }

    packet.command = 14;
    packet.partySlot = partySlot;
    packet.switchingOrCanPickCommandMask = ctx->switchInFlag | unavailableMask;
    battleType = BattleSystem_GetBattleType(battleSystem);

    if ((battleType & BATTLE_TYPE_DOUBLES) && !(battleType & BATTLE_TYPE_MULTI)) {
        battler = battlerId & 1;
    } else {
        battler = battlerId;
    }

    party = BattleSystem_GetParty(battleSystem, battler);
    count = 0;
    for (i = 0; i < Party_GetCount(party); i++) {
        mon = Party_GetMonByIndex(party, ctx->partyOrder[battler][i]);
        species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
        if (species != SPECIES_NONE && species != SPECIES_EGG) {
            if (GetMonData(mon, MON_DATA_HP, NULL)) {
                if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                    packet.ballStatus[0][count] = 3;
                } else {
                    packet.ballStatus[0][count] = 1;
                }
            } else {
                packet.ballStatus[0][count] = 2;
            }
            if (battleType & BATTLE_TYPE_NO_EXP) {
                packet.expPercents[count] = 0;
            } else {
                packet.expPercents[count] = GetPercentProgressTowardsNextLevel(mon);
            }
            count++;
        }
    }

    if (((battleType & (BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI)) == (BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI))
        || (battleType & BATTLE_TYPE_TAG)
        || battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI)
        || battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI | BATTLE_TYPE_FRONTIER)) {
        if (BattleSystem_GetFieldSide(battleSystem, battlerId)) {
            battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_1);
        } else {
            battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_1);
        }
        party = BattleSystem_GetParty(battleSystem, battler);
        count = 0;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->partyOrder[battler][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != SPECIES_NONE && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL)) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                        packet.ballStatus[1][count] = 3;
                    } else {
                        packet.ballStatus[1][count] = 1;
                    }
                } else {
                    packet.ballStatus[1][count] = 2;
                }
                count++;
            }
        }

        if (BattleSystem_GetFieldSide(battleSystem, battlerId)) {
            battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_2);
        } else {
            battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_2);
        }
        party = BattleSystem_GetParty(battleSystem, battler);
        count = 3;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->partyOrder[battler][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != SPECIES_NONE && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL)) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                        packet.ballStatus[1][count] = 3;
                    } else {
                        packet.ballStatus[1][count] = 1;
                    }
                } else {
                    packet.ballStatus[1][count] = 2;
                }
                count++;
            }
        }
    } else {
        battler = BattleSystem_GetOpponentBattler(battleSystem, battlerId, 2);
        party = BattleSystem_GetParty(battleSystem, battler);
        count = 0;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->partyOrder[battler][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != SPECIES_NONE && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL)) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                        packet.ballStatus[1][count] = 3;
                    } else {
                        packet.ballStatus[1][count] = 1;
                    }
                } else {
                    packet.ballStatus[1][count] = 2;
                }
                count++;
            }
        }
    }

    for (i = 0; i < MAX_MON_MOVES; i++) {
        packet.moves[i] = GetBattlerVar(ctx, battlerId, BMON_DATA_MOVE1 + i, NULL);
        packet.movePPCur[i] = GetBattlerVar(ctx, battlerId, BMON_DATA_CUR_PP_1 + i, NULL);
        packet.movePP[i] = GetBattlerVar(ctx, battlerId, BMON_DATA_MAX_PP_1 + i, NULL);
    }
    packet.hp = ctx->battleMons[battlerId].hp;
    packet.maxHp = ctx->battleMons[battlerId].maxHp;
    if (packet.hp) {
        if (ctx->battleMons[battlerId].status) {
            packet.ballStatusBattler = 3;
        } else {
            packet.ballStatusBattler = 1;
        }
    } else {
        packet.ballStatusBattler = 2;
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_ReplyCommandSelection(BattleSystem *battleSystem, int battlerId, u32 input) {
    BattleController_EmitPacket(battleSystem, 0, battlerId, &input, sizeof(input));
}

#pragma force_active on
#pragma push
#pragma opt_lifetimes off

void BattleController_EmitSelectMove(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    BattleControllerPacketSelectMove packet;
    int i;

    BattleBuffer_Clear(BattleSystem_GetBattleContext(battleSystem), battlerId);
    packet.command = 15;
    packet.selectedMonIndex = ctx->selectedMonIndex[battlerId];
    for (i = 0; i < MAX_MON_MOVES; i++) {
        packet.moves[i] = ctx->battleMons[battlerId].moves[i];
        packet.movePPCur[i] = ctx->battleMons[battlerId].movePPCur[i];
        packet.movePP[i] = GetMoveMaxPP(ctx->battleMons[battlerId].moves[i], ctx->battleMons[battlerId].movePP[i]);
    }
    packet.unusableMovesMask = StruggleCheck(battleSystem, ctx, battlerId, 0, -1);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma pop
#pragma force_active reset

void BattleController_ReplyMoveSelection(BattleSystem *battleSystem, int battlerId, u32 input) {
    BattleController_EmitPacket(battleSystem, 0, battlerId, &input, sizeof(input));
}

#pragma force_active on

void BattleController_EmitSelectTarget(BattleSystem *battleSystem, BattleContext *ctx, int a2, int battlerId) {
    BattleControllerPacketSelectTarget packet;
    int i;
    u32 battleType;

    BattleBuffer_Clear(ctx, battlerId);
    battleType = BattleSystem_GetBattleType(battleSystem);
    packet.command = 16;
    packet.moveRange = a2;
    if (!(battleType & BATTLE_TYPE_DOUBLES) || (battleType & BATTLE_TYPE_MULTI) || ((battleType & BATTLE_TYPE_DOUBLES) && battlerId >= BATTLER_PLAYER2)) {
        packet.unk_01 = 1;
    } else {
        packet.unk_01 = 0;
    }
    for (i = 0; i < BATTLER_MAX; i++) {
        if (ctx->battleMons[i].hp) {
            packet.gauges[i].hp = ctx->battleMons[i].hp;
            packet.gauges[i].maxHp = ctx->battleMons[i].maxHp;
            packet.gauges[i].exists = TRUE;
            if ((ctx->battleMons[i].species == SPECIES_NIDORAN_F || ctx->battleMons[i].species == SPECIES_NIDORAN_M) && !ctx->battleMons[i].hasNickname) {
                packet.gauges[i].gender = MON_GENDERLESS;
            } else {
                packet.gauges[i].gender = ctx->battleMons[i].gender;
            }
            packet.gauges[i].selectedMonIndex = ctx->selectedMonIndex[i];
            if (ctx->battleMons[i].status) {
                packet.gauges[i].status = 3;
            } else {
                packet.gauges[i].status = 1;
            }
        } else {
            packet.gauges[i].exists = FALSE;
            packet.gauges[i].status = 2;
        }
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_ReplyTargetSelection(BattleSystem *battleSystem, int battlerId, u32 input) {
    BattleController_EmitPacket(battleSystem, 0, battlerId, &input, sizeof(input));
}

static inline void SelectItemPacket_SetTargetFlags(BattleControllerPacketSelectItem *packet, const BattleMon *mon) {
    packet->unk_01 = 0;
    if (mon->moveEffectFlags & MOVE_EFFECT_FLAG_SEMI_INVULNERABLE) {
        packet->targetSemiInvulnerable = TRUE;
        packet->targetSubstitute = FALSE;
    } else if (mon->status2 & STATUS2_SUBSTITUTE) {
        packet->targetSemiInvulnerable = FALSE;
        packet->targetSubstitute = TRUE;
    } else {
        packet->targetSemiInvulnerable = FALSE;
        packet->targetSubstitute = FALSE;
    }
}

#pragma force_active on

void BattleController_EmitSelectItem(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    BattleControllerPacketSelectItem packet;
    int i;
    int j;

    BattleBuffer_Clear(ctx, battlerId);
    packet.command = 17;
    for (i = 0; i < BATTLER_MAX; i++) {
        packet.selectedMonIndex[i] = ctx->selectedMonIndex[i];
        for (j = 0; j < PARTY_SIZE; j++) {
            packet.partyOrder[i][j] = ctx->partyOrder[i][j];
        }
        packet.embargoFlag[i] = ctx->battleMons[i].moveEffectData.embargoFlag;
    }
    if (BattleSystem_GetBattleType(battleSystem) == (BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI)) {
        if (!(MaskOfFlagNo(BATTLER_ENEMY) & ctx->switchInFlag) && !(MaskOfFlagNo(BATTLER_ENEMY2) & ctx->switchInFlag)) {
            packet.unk_01 = 1;
            packet.targetSemiInvulnerable = FALSE;
            packet.targetSubstitute = FALSE;
        } else if (!(MaskOfFlagNo(BATTLER_ENEMY) & ctx->switchInFlag)) {
            SelectItemPacket_SetTargetFlags(&packet, &ctx->battleMons[BATTLER_ENEMY]);
        } else {
            SelectItemPacket_SetTargetFlags(&packet, &ctx->battleMons[BATTLER_ENEMY2]);
        }
    } else if (BattleSystem_GetBattleType(battleSystem) == BATTLE_TYPE_NONE) {
        SelectItemPacket_SetTargetFlags(&packet, &ctx->battleMons[BATTLER_ENEMY]);
    } else {
        packet.unk_01 = 0;
        packet.targetSemiInvulnerable = FALSE;
        packet.targetSubstitute = FALSE;
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_ReplyItemSelection(BattleSystem *battleSystem, int battlerId, u32 input) {
    BattleController_EmitPacket(battleSystem, 0, battlerId, &input, sizeof(input));
}

#pragma force_active on

void BattleController_EmitShowMonList(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, u32 a3, u32 canSwitch, u32 a5) {
    BattleControllerPacketShowMonList packet;
    int i;
    int j;

    BattleBuffer_Clear(ctx, battlerId);
    packet.command = 18;
    packet.battlerId = battlerId;
    packet.mode = a3;
    packet.trapped = canSwitch;
    packet.reservedMon = a5;
    packet.switchInFlag = ctx->switchInFlag;
    for (i = 0; i < BATTLER_MAX; i++) {
        packet.selectedMonIndex[i] = ctx->selectedMonIndex[i];
        for (j = 0; j < PARTY_SIZE; j++) {
            packet.partyOrder[i][j] = ctx->partyOrder[i][j];
        }
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_ReplyPartySelection(BattleSystem *battleSystem, int battlerId, u32 input) {
    BattleController_EmitPacket(battleSystem, 0, battlerId, &input, sizeof(input));
}

#pragma force_active on

void BattleController_EmitDrawYesNoBox(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int msgId, int a4, int a5, int a6) {
    BattleControllerPacketYesNoBox packet;

    BattleBuffer_Clear(ctx, battlerId);
    packet.command = 19;
    packet.msgId = msgId;
    packet.mode = a4;
    packet.value = a5;
    packet.param = a6;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitPrintAttackMessage(BattleSystem *battleSystem, BattleContext *ctx) {
    BattleControllerPacketAttackMessage packet;

    packet.command = 20;
    packet.selectedMonIndex = ctx->selectedMonIndex[ctx->battlerIdAttacker];
    packet.move = ctx->moveNoCur;
    BattleController_EmitPacket(battleSystem, 1, ctx->battlerIdAttacker, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitPrintMessage(BattleSystem *battleSystem, BattleContext *ctx, BattleMessage *msg) {
    msg->unk0 = 21;
    BattleController_EmitPacket(battleSystem, 1, ctx->battlerIdAttacker, msg, sizeof(BattleMessage));
}

void BattleController_SetMoveAnimation(BattleSystem *battleSystem, BattleContext *ctx, u16 move) {
    MoveAnimData packet;
    BattleController_InitMoveAnimData(battleSystem, ctx, &packet, 0, 0, ctx->battlerIdAttacker, ctx->battlerIdTarget, move);
    BattleController_EmitPacket(battleSystem, 1, ctx->battlerIdAttacker, &packet, sizeof(packet));
}

void BattleController_SetMoveAnimationForBattlers(BattleSystem *battleSystem, BattleContext *ctx, u16 move, int attacker, int target) {
    MoveAnimData packet;
    BattleController_InitMoveAnimData(battleSystem, ctx, &packet, 0, 0, attacker, target, move);
    BattleController_EmitPacket(battleSystem, 1, attacker, &packet, sizeof(packet));
}

void BattleController_EmitMonFlicker(BattleSystem *battleSystem, int battlerId, int moveStatusFlag) {
    u32 command = 23;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

void BattleController_EmitHealthbarUpdate(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    BattleControllerPacketHealthbarUpdate packet;
    Pokemon *mon = BattleSystem_GetPartyMon(battleSystem, battlerId, ctx->selectedMonIndex[battlerId]);
    u32 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u32 level = GetMonData(mon, MON_DATA_LEVEL, NULL);

    packet.command = 24;
    packet.level = ctx->battleMons[battlerId].level;
    packet.hp = ctx->battleMons[battlerId].hp;
    packet.maxHp = ctx->battleMons[battlerId].maxHp;
    packet.hpCalc = ctx->hpCalc;
    if ((ctx->battleMons[battlerId].species == SPECIES_NIDORAN_F || ctx->battleMons[battlerId].species == SPECIES_NIDORAN_M) && !ctx->battleMons[battlerId].hasNickname) {
        packet.gender = MON_GENDERLESS;
    } else {
        packet.gender = ctx->battleMons[battlerId].gender;
    }
    packet.exp = ctx->battleMons[battlerId].exp - GetMonExpBySpeciesAndLevel(species, level);
    packet.maxExp = GetMonExpBySpeciesAndLevel(species, level + 1) - GetMonExpBySpeciesAndLevel(species, level);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitExpBarUpdate(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, u32 prevExp) {
    BattleControllerPacketExpBarUpdate packet;
    Pokemon *mon = BattleSystem_GetPartyMon(battleSystem, battlerId, ctx->selectedMonIndex[battlerId]);
    u32 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u32 level = GetMonData(mon, MON_DATA_LEVEL, NULL);

    packet.command = 25;
    packet.prevExp = prevExp;
    packet.exp = ctx->battleMons[battlerId].exp - GetMonExpBySpeciesAndLevel(species, level);
    packet.maxExp = GetMonExpBySpeciesAndLevel(species, level + 1) - GetMonExpBySpeciesAndLevel(species, level);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active on

void BattleController_EmitPlayFaintAnimation(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    BattleControllerPacketFaintAnimation packet;

    packet.command = 26;
    packet.species = ctx->battleMons[battlerId].species;
    packet.form = ctx->battleMons[battlerId].form;
    packet.isSubstitute = (ctx->battleMons[battlerId].status2 & STATUS2_SUBSTITUTE) != 0;
    packet.isTransformed = (ctx->battleMons[battlerId].status2 & STATUS2_TRANSFORM) != 0;
    if (ctx->battleMons[battlerId].status2 & STATUS2_TRANSFORM) {
        packet.gender = ctx->battleMons[battlerId].moveEffectData.transformGender;
        packet.personality = ctx->battleMons[battlerId].moveEffectData.transformPersonality;
    } else {
        packet.gender = ctx->battleMons[battlerId].gender;
        packet.personality = ctx->battleMons[battlerId].personality;
    }
    for (int i = 0; i < BATTLER_MAX; i++) {
        packet.allSpecies[i] = ctx->battleMons[i].species;
        packet.allShiny[i] = ctx->battleMons[i].shiny;
        packet.allForm[i] = ctx->battleMons[i].form;
        if (ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            packet.allGender[i] = ctx->battleMons[i].moveEffectData.transformGender;
            packet.allPersonality[i] = ctx->battleMons[i].moveEffectData.transformPersonality;
        } else {
            packet.allGender[i] = ctx->battleMons[i].gender;
            packet.allPersonality[i] = ctx->battleMons[i].personality;
        }
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitPlaySE(BattleSystem *battleSystem, BattleContext *ctx, u32 sndSeqNo, int battlerId) {
    BattleControllerPacketPlaySE packet;
    packet.command = 27;
    packet.sndSeqNo = sndSeqNo;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitFadeOutBattle(BattleSystem *battleSystem, BattleContext *ctx) {
    u32 command = 28;
    BattleController_EmitPacket(battleSystem, 1, 0, &command, sizeof(command));
}

#pragma force_active on

void BattleController_EmitToggleVanish(BattleSystem *battleSystem, int battlerId, int vanish) {
    BattleControllerPacketToggleVanish packet;
    int i;

    packet.command = 29;
    packet.vanish = vanish;
    packet.isSubstitute = (battleSystem->ctx->battleMons[battlerId].status2 & STATUS2_SUBSTITUTE) ? 1 : 0;
    for (i = 0; i < BATTLER_MAX; i++) {
        packet.allSpecies[i] = battleSystem->ctx->battleMons[i].species;
        packet.allShiny[i] = battleSystem->ctx->battleMons[i].shiny;
        packet.allForm[i] = battleSystem->ctx->battleMons[i].form;
        if (battleSystem->ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            packet.allGender[i] = battleSystem->ctx->battleMons[i].moveEffectData.transformGender;
            packet.allPersonality[i] = battleSystem->ctx->battleMons[i].moveEffectData.transformPersonality;
        } else {
            packet.allGender[i] = battleSystem->ctx->battleMons[i].gender;
            packet.allPersonality[i] = battleSystem->ctx->battleMons[i].personality;
        }
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitHealthbarStatus(BattleSystem *battleSystem, int battlerId, int status) {
    BattleControllerPacketHealthbarStatus packet;
    packet.command = 30;
    packet.status = status;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitPrintTrainerMessage(BattleSystem *battleSystem, int battlerId, int msg) {
    BattleControllerPacketTrainerMessage packet;
    packet.command = 31;
    packet.msg = msg;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitSetStatus2Effect(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int status) {
    MoveAnimData packet;
    BattleController_InitMoveAnimData(battleSystem, ctx, &packet, 1, status, battlerId, battlerId, 0);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitCopyStatus2Effect(BattleSystem *battleSystem, BattleContext *ctx, int battlerIdA, int battlerIdB, int status) {
    MoveAnimData packet;
    BattleController_InitMoveAnimData(battleSystem, ctx, &packet, 1, status, battlerIdA, battlerIdB, 0);
    BattleController_EmitPacket(battleSystem, 1, battlerIdA, &packet, sizeof(packet));
}

#pragma force_active on

void BattleController_EmitPrintReturnMessage(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int selectedMonIndex) {
    BattleControllerPacketSwitchMessage packet;

    packet.command = 32;
    packet.selectedMonIndex = selectedMonIndex;
    packet.value = (100 * (ctx->hpTemp - ctx->battleMons[BATTLER_ENEMY].hp)) / ctx->hpTemp;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitPrintSendOutMessage(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int selectedMonIndex) {
    BattleControllerPacketSwitchMessage packet;

    packet.command = 33;
    packet.selectedMonIndex = selectedMonIndex;
    if (ctx->battleMons[BATTLER_ENEMY].hp == 0) {
        packet.value = 1000;
    } else {
        packet.value = 1000 * ctx->battleMons[BATTLER_ENEMY].hp / ctx->battleMons[BATTLER_ENEMY].maxHp;
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitPrintEncounterMessage(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    u32 command = 34;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

#pragma force_active on

void BattleController_EmitPrintFirstSendOutMessage(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    BattleControllerPacketFirstSendOutMessage packet;
    int i;

    packet.command = 35;
    for (i = 0; i < BattleSystem_GetMaxBattlers(battleSystem); i++) {
        packet.selectedMonIndex[i] = ctx->selectedMonIndex[i];
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitExpBarFullFlash(BattleSystem *battleSystem, int battlerId) {
    u32 command = 36;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

#pragma force_active on

void BattleController_EmitPrintSelectionMessage(BattleSystem *battleSystem, int battlerId, BattleMessage msg) {
    BattleControllerPacketSelectionMessage packet;

    BattleBuffer_Clear(BattleSystem_GetBattleContext(battleSystem), battlerId);
    packet.command = 37;
    packet.msg = msg;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_ReplySelectionMessageDone(BattleSystem *battleSystem, int battlerId) {
    u32 input = 1;
    BattleController_EmitPacket(battleSystem, 0, battlerId, &input, sizeof(input));
}

#pragma force_active on

void BattleController_EmitRefreshHealthbar(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    Pokemon *mon = BattleSystem_GetPartyMon(battleSystem, battlerId, ctx->selectedMonIndex[battlerId]);
    int species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    int level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    BattleControllerPacketRefreshHealthbar packet;

    packet.command = 38;
    packet.level = ctx->battleMons[battlerId].level;
    packet.hp = ctx->battleMons[battlerId].hp;
    packet.maxHp = ctx->battleMons[battlerId].maxHp;
    packet.selectedMonIndex = ctx->selectedMonIndex[battlerId];
    packet.status = (u8)GetBattlerStatusCondition(ctx, battlerId);
    if ((ctx->battleMons[battlerId].species == SPECIES_NIDORAN_F || ctx->battleMons[battlerId].species == SPECIES_NIDORAN_M) && !ctx->battleMons[battlerId].hasNickname) {
        packet.gender = MON_GENDERLESS;
    } else {
        packet.gender = ctx->battleMons[battlerId].gender;
    }
    packet.exp = ctx->battleMons[battlerId].exp - GetMonExpBySpeciesAndLevel(species, level);
    packet.maxExp = GetMonExpBySpeciesAndLevel(species, level + 1) - GetMonExpBySpeciesAndLevel(species, level);
    packet.caught = (u8)BattleSystem_CheckMonCaught(battleSystem, ctx->battleMons[battlerId].species);
    packet.safariBallCount = BattleSystem_GetSafariBallCount(battleSystem);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitBattleMonToPartyMonCopy(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    BattleControllerPacketBattleMonToPartyMonCopy packet;

    packet.command = 39;
    packet.selectedMonIndex = ctx->selectedMonIndex[battlerId];
    packet.mimicedMoveIndex = (u8)ctx->battleMons[battlerId].moveEffectData.mimicedMoveIndex;
    packet.hp = ctx->battleMons[battlerId].hp;
    packet.item = ctx->battleMons[battlerId].item;
    packet.battlerBitKnockedOffItem = ctx->fieldSideConditionData[BattleSystem_GetFieldSide(battleSystem, battlerId)].battlerBitKnockedOffItem;
    packet.form = ctx->battleMons[battlerId].form;
    packet.ability = ctx->battleMons[battlerId].ability;
    for (int i = 0; i < MAX_MON_MOVES; i++) {
        packet.moves[i] = ctx->battleMons[battlerId].moves[i];
        packet.movePPCur[i] = ctx->battleMons[battlerId].movePPCur[i];
    }
    if (packet.hp) {
        packet.status = ctx->battleMons[battlerId].status & ~STATUS_POISON_COUNT;
        packet.status2 = ctx->battleMons[battlerId].status2;
    } else {
        packet.status = STATUS_NONE;
        packet.status2 = ctx->battleMons[battlerId].status2;
    }
    if (ctx->battleStatus2 & BATTLE_STATUS2_FORM_CHANGE) {
        packet.formChange = TRUE;
        ctx->battleStatus2 &= ~BATTLE_STATUS2_FORM_CHANGE;
    } else {
        packet.formChange = FALSE;
    }
    if (ctx->battleStatus2 & BATTLE_STATUS2_RECALC_MON_STATS) {
        packet.recalcMonStats = TRUE;
        packet.formChange = TRUE;
        ctx->battleStatus2 &= ~BATTLE_STATUS2_RECALC_MON_STATS;
    } else {
        packet.recalcMonStats = FALSE;
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitBackgroundSlideIn(BattleSystem *battleSystem, int battlerId) {
    u32 command = 40;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

void BattleController_EmitCancelSelection(BattleSystem *battleSystem, int battlerId) {
    u32 command = 41;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

#pragma force_active on

void BattleControl_EmitPartyStatusHeal(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int moveNo) {
    BattleControllerPacketPartyStatusHeal packet;

    packet.command = 42;
    packet.moveNo = moveNo;
    packet.ability = ctx->battleMons[battlerId].ability;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitSelectMoveToForget(BattleSystem *battleSystem, int battlerId, u32 move, int slot) {
    BattleControllerPacketSelectMoveToForget packet;
    BattleBuffer_Clear(BattleSystem_GetBattleContext(battleSystem), battlerId);
    packet.command = 43;
    packet.move = move;
    packet.slot = slot;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitPlayMosaicAnimation(BattleSystem *battleSystem, int battlerId, int a2, int delay) {
    BattleControllerPacketMosaicAnimation packet;
    packet.command = 44;
    packet.mosaic = a2;
    packet.delay = delay;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active on

void BattleController_EmitChangeForm(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketChangeForm packet;

    packet.command = 45;
    packet.species = battleSystem->ctx->battleMons[battlerId].species;
    packet.shiny = battleSystem->ctx->battleMons[battlerId].shiny;
    if (battleSystem->ctx->battleMons[battlerId].status2 & STATUS2_TRANSFORM) {
        packet.gender = battleSystem->ctx->battleMons[battlerId].moveEffectData.transformGender;
        packet.personality = battleSystem->ctx->battleMons[battlerId].moveEffectData.transformPersonality;
    } else {
        packet.gender = battleSystem->ctx->battleMons[battlerId].gender;
        packet.personality = battleSystem->ctx->battleMons[battlerId].personality;
    }
    packet.form = battleSystem->ctx->battleMons[battlerId].form;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitSetBattleBackground(BattleSystem *battleSystem, int battlerId) {
    u32 command = 46;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

void BattleController_EmitEndSelection(BattleSystem *battleSystem, int battlerId) {
    u32 command = 47;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

void BattleController_EmitInitStartBallGauge(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketBallGauge packet;
    BattleController_InitBallGaugePacket(battleSystem, battleSystem->ctx, &packet, 48, battlerId);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitDeleteStartBallGauge(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketBallGauge packet;
    BattleController_InitBallGaugePacket(battleSystem, battleSystem->ctx, &packet, 49, battlerId);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitInitBallGauge(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketBallGauge packet;
    BattleController_InitBallGaugePacket(battleSystem, battleSystem->ctx, &packet, 50, battlerId);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitDeleteBallGauge(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketBallGauge packet;
    BattleController_InitBallGaugePacket(battleSystem, battleSystem->ctx, &packet, 51, battlerId);
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitLoadBallGfx(BattleSystem *battleSystem) {
    u32 command = 52;
    BattleController_EmitPacket(battleSystem, 1, 0, &command, sizeof(command));
}

void BattleController_EmitDeleteBallGfx(BattleSystem *battleSystem) {
    u32 command = 53;
    BattleController_EmitPacket(battleSystem, 1, 0, &command, sizeof(command));
}

void BattleController_EmitIncrementGameStat(BattleSystem *battleSystem, int battlerId, int flag, int id) {
    BattleControllerPacketIncrementGameStat packet;
    packet.command = 54;
    packet.flag = flag;
    packet.id = id;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active on

void BattleController_EmitShowWaitMessage(BattleSystem *battleSystem, int battlerId) {
    BattleControllerPacketRecordData packet;
    u32 battleType = BattleSystem_GetBattleType(battleSystem);

    packet.command = 55;
    packet.recordSize = 0;
    if ((battleType & BATTLE_TYPE_LINK) && BattleRecord_Exists() == TRUE && !(battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING)) {
        packet.recordSize = BattleSystem_PackRecordUpdate(battleSystem, packet.record);
        GF_ASSERT(packet.recordSize < 28);
        BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
    }
}

void BattleController_EmitRestoreSprite(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    MoveAnimData packet;
    int i;

    packet.command = 56;
    for (i = 0; i < BATTLER_MAX; i++) {
        packet.species[i] = ctx->battleMons[i].species;
        packet.shiny[i] = ctx->battleMons[i].shiny;
        packet.form[i] = ctx->battleMons[i].form;
        if (ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            packet.gender[i] = ctx->battleMons[i].moveEffectData.transformGender;
            packet.personality[i] = ctx->battleMons[i].moveEffectData.transformPersonality;
        } else {
            packet.gender[i] = ctx->battleMons[i].gender;
            packet.personality[i] = ctx->battleMons[i].personality;
        }
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitSpriteToOAM(BattleSystem *battleSystem, int battlerId) {
    u32 command = 57;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

void BattleController_EmitOAMToSprite(BattleSystem *battleSystem, int battlerId) {
    u32 command = 58;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &command, sizeof(command));
}

void BattleController_EmitPrintResultMessage(BattleSystem *battleSystem) {
    u32 command = 59;
    BattleController_EmitPacket(battleSystem, 1, 0, &command, sizeof(command));
}

void BattleController_EmitRunAwayMessage(BattleSystem *battleSystem, BattleContext *ctx) {
    BattleControllerPacketRecordData packet;
    int i;
    u32 battleType = BattleSystem_GetBattleType(battleSystem);

    packet.command = 60;
    packet.escapedMask = 0;
    packet.recordSize = 0;
    for (i = 0; i < BattleSystem_GetMaxBattlers(battleSystem); i++) {
        if (ctx->playerActions[i].command == CONTROLLER_COMMAND_RUN_INPUT) {
            packet.escapedMask |= MaskOfFlagNo(i);
        }
    }
    if ((battleType & BATTLE_TYPE_LINK) && BattleRecord_Exists() == TRUE && !(battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING)) {
        packet.recordSize = BattleSystem_PackRecordUpdate(battleSystem, packet.record);
        GF_ASSERT(packet.recordSize < 28);
    }
    BattleController_EmitPacket(battleSystem, 1, 0, &packet, sizeof(packet));
}

void BattleController_EmitForefitMessage(BattleSystem *battleSystem) {
    BattleControllerPacketRecordData packet;
    u32 battleType = BattleSystem_GetBattleType(battleSystem);

    packet.command = 61;
    packet.recordSize = 0;
    if ((battleType & BATTLE_TYPE_LINK) && BattleRecord_Exists() == TRUE && !(battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING)) {
        packet.recordSize = BattleSystem_PackRecordUpdate(battleSystem, packet.record);
        GF_ASSERT(packet.recordSize < 28);
    }
    BattleController_EmitPacket(battleSystem, 1, 0, &packet, sizeof(packet));
}

#pragma force_active on

void BattleController_EmitSwapToSubstituteSprite(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    MoveAnimData packet;
    int i;

    packet.command = 62;
    for (i = 0; i < BATTLER_MAX; i++) {
        packet.species[i] = ctx->battleMons[i].species;
        packet.shiny[i] = ctx->battleMons[i].shiny;
        packet.form[i] = ctx->battleMons[i].form;
        if (ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            packet.gender[i] = ctx->battleMons[i].moveEffectData.transformGender;
            packet.personality[i] = ctx->battleMons[i].moveEffectData.transformPersonality;
        } else {
            packet.gender[i] = ctx->battleMons[i].gender;
            packet.personality[i] = ctx->battleMons[i].personality;
        }
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitPlayMoveSE(BattleSystem *battleSystem, BattleContext *ctx, int battlerId) {
    BattleControllerPacketMoveSE packet;

    packet.command = 63;
    if (ctx->moveStatusFlag & MOVE_STATUS_SUPER_EFFECTIVE) {
        packet.effectiveness = 2;
    } else if (ctx->moveStatusFlag & MOVE_STATUS_NOT_VERY_EFFECTIVE) {
        packet.effectiveness = 1;
    } else {
        packet.effectiveness = 0;
    }
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

void BattleController_EmitPlaySong(BattleSystem *battleSystem, int battlerId, int song) {
    BattleControllerPacketPlaySong packet;
    packet.command = 64;
    packet.song = song;
    BattleController_EmitPacket(battleSystem, 1, battlerId, &packet, sizeof(packet));
}

#pragma force_active on

void BattleController_EmitSetBattleResults(BattleSystem *battleSystem) {
    u32 battleType = BattleSystem_GetBattleType(battleSystem);
    BattleControllerPacketBattleResults packet;

    packet.command = 65;
    packet.outcomeFlags = BattleSystem_GetBattleOutcomeFlags(battleSystem);
    packet.recordSize = 0;
    if ((battleType & BATTLE_TYPE_LINK) && BattleRecord_Exists() == TRUE && !(battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING)) {
        packet.recordSize = BattleSystem_PackRecordUpdate(battleSystem, packet.record);
        GF_ASSERT(packet.recordSize <= 28);
    }
    BattleController_EmitPacket(battleSystem, 1, 0, &packet, sizeof(packet));
}

#pragma force_active reset

void BattleController_EmitBlankMessage(BattleSystem *battleSystem) {
    u32 command = 66;
    BattleController_EmitPacket(battleSystem, 1, 0, &command, sizeof(command));
}

void BattleController_EmitCommandComplete(BattleSystem *battleSystem, int battlerId, int command) {
    BattleControllerPacketCommandComplete packet;
    packet.command = command;
    packet.netId = Comm_GetLocalNetId();
    BattleController_EmitPacket(battleSystem, 2, battlerId, &packet, sizeof(packet));
}

#pragma force_active on

BOOL BattleController_RecvLinkData(BattleSystem *battleSystem, u8 *data) {
    BOOL ret = TRUE;
    int battlerId = data[1];
    int direction = data[0];
    int size = data[2] | (data[3] << 8);
    int i;

    if (direction == 0) {
        for (i = 0; i < size; i++) {
            battleSystem->ctx->battleBuffer[battlerId][i] = data[4 + i];
        }
    } else if (direction == 1) {
        if (battleSystem->opponentData[battlerId]->commandReceived == 0) {
            battleSystem->opponentData[battlerId]->commandReceived = 1;
            for (i = 0; i < size; i++) {
                battleSystem->opponentData[battlerId]->unk8C[8 + i] = data[4 + i];
            }
        } else {
            ret = FALSE;
        }
    } else if (direction == 2) {
        int command = data[4];
        int netId = data[5];
        if (BattleSystem_IsServer(battleSystem)) {
            BattleContext_RemoveLinkBufferEntry(battleSystem->ctx, netId, battlerId, command);
        }
    }
    return ret;
}

void BattleController_InitMoveAnimData(BattleSystem *battleSystem, BattleContext *ctx, MoveAnimData *animData, int a3, int a4, int attacker, int defender, u16 move) {
    int i;
    u8 slot;

    animData->command = 22;
    animData->move = move;
    animData->attacker = attacker;
    animData->defender = defender;
    animData->isEffectAnim = a3;
    animData->effectAnimId = a4;
    animData->terrain = BattleSystem_GetTerrainId(battleSystem);
    animData->attackerActive = FALSE;
    animData->defenderActive = FALSE;
    if (ctx == NULL) {
        return;
    }
    animData->damage = ctx->damage;
    if (ctx->movePower) {
        animData->power = ctx->movePower;
    } else {
        animData->power = ctx->trainerAIData.moveData[move].power;
    }
    animData->friendship = ctx->battleMons[attacker].friendship;
    if (!CheckAbilityActive(battleSystem, ctx, CHECK_ABILITY_ALL_HP, 0, ABILITY_CLOUD_NINE) && !CheckAbilityActive(battleSystem, ctx, CHECK_ABILITY_ALL_HP, 0, ABILITY_AIR_LOCK)) {
        animData->fieldCondition = ctx->fieldCondition;
    } else {
        animData->fieldCondition = 0;
    }
    animData->moveAnimVariant = ctx->moveAnimVariant;
    animData->isSubstitute = (ctx->battleMons[(u32)attacker].status2 & STATUS2_SUBSTITUTE) ? 1 : 0;
    animData->isTransformed = (ctx->battleMons[(u32)attacker].status2 & STATUS2_TRANSFORM) != 0;
    for (i = 0; i < BATTLER_MAX; i++) {
        animData->species[i] = ctx->battleMons[i].species;
        animData->shiny[i] = ctx->battleMons[i].shiny;
        animData->form[i] = ctx->battleMons[i].form;
        animData->moveEffectFlags[i] = ctx->battleMons[i].moveEffectFlags;
        if (ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            animData->gender[i] = ctx->battleMons[i].moveEffectData.transformGender;
            animData->personality[i] = ctx->battleMons[i].moveEffectData.transformPersonality;
        } else {
            animData->gender[i] = ctx->battleMons[i].gender;
            animData->personality[i] = ctx->battleMons[i].personality;
        }
    }
    if (attacker != BATTLER_NONE) {
        slot = BattleSystem_GetFollowMonSlot(battleSystem, attacker);
        if (slot != 0xFF && slot == ctx->selectedMonIndex[attacker]) {
            animData->attackerActive = TRUE;
        }
    }
    if (defender != BATTLER_NONE) {
        slot = BattleSystem_GetFollowMonSlot(battleSystem, defender);
        if (slot != 0xFF && slot == ctx->selectedMonIndex[defender]) {
            animData->defenderActive = TRUE;
        }
    }
}

#pragma force_active reset

void BattleController_EmitScreenFade(BattleSystem *battleSystem, BattleContext *ctx, u8 a2) {
    BattleControllerPacketScreenFade packet;
    MI_CpuFill8(&packet, 0, sizeof(packet));
    packet.command = 67;
    packet.mode = a2;
    BattleController_EmitPacket(battleSystem, 1, 0, &packet, sizeof(packet));
}

#pragma force_active on

void BattleController_InitBallGaugePacket(BattleSystem *battleSystem, BattleContext *ctx, BattleControllerPacketBallGauge *packet, int command, int battlerId) {
    Party *party;
    int slot;
    int i;
    Pokemon *mon;
    u32 species;
    u32 battleType;
    int partner;

    MI_CpuClearFast(packet, sizeof(BattleControllerPacketBallGauge));
    battleType = BattleSystem_GetBattleType(battleSystem);
    packet->command = command;
    if ((battleType & (BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI)) == (BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI)
        || ((battleType & BATTLE_TYPE_TAG) && BattleSystem_GetFieldSide(battleSystem, battlerId) != 0)
        || (battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI) && BattleSystem_GetFieldSide(battleSystem, battlerId) != 0)
        || battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI | BATTLE_TYPE_FRONTIER)) {
        if (BattleSystem_GetBattlerType(battleSystem, battlerId) == BATTLER_TYPE_PLAYER_SIDE_SLOT_1 || BattleSystem_GetBattlerType(battleSystem, battlerId) == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
            partner = battlerId;
            battlerId = BattleSystem_GetBattlerIdPartner(battleSystem, battlerId);
        } else {
            partner = BattleSystem_GetBattlerIdPartner(battleSystem, battlerId);
        }

        party = BattleSystem_GetParty(battleSystem, partner);
        slot = 0;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->partyOrder[partner][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != SPECIES_NONE && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL) != 0) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL) != 0) {
                        packet->status[slot] = 3;
                    } else {
                        packet->status[slot] = 1;
                    }
                } else {
                    packet->status[slot] = 2;
                }
                slot++;
            }
        }

        party = BattleSystem_GetParty(battleSystem, battlerId);
        slot = 3;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->partyOrder[battlerId][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != SPECIES_NONE && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL) != 0) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL) != 0) {
                        packet->status[slot] = 3;
                    } else {
                        packet->status[slot] = 1;
                    }
                } else {
                    packet->status[slot] = 2;
                }
                slot++;
            }
        }
    } else {
        if ((battleType & BATTLE_TYPE_DOUBLES) && !(battleType & BATTLE_TYPE_MULTI)) {
            battlerId &= 1;
        }
        party = BattleSystem_GetParty(battleSystem, battlerId);
        slot = 0;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->partyOrder[battlerId][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != SPECIES_NONE && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL) != 0) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL) != 0) {
                        packet->status[slot] = 3;
                    } else {
                        packet->status[slot] = 1;
                    }
                } else {
                    packet->status[slot] = 2;
                }
                slot++;
            }
        }
    }
}

#pragma force_active reset
