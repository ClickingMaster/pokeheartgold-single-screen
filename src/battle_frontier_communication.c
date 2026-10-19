#include "battle_frontier_communication.h"

#include "global.h"

#include "frontier/battle_arcade_game_board.h"
#include "frontier/frontier_link_lobby.h"
#include "frontier/frontier_link_messages.h"
#include "frontier/frontier_map.h"

#include "communication_command.h"
#include "communication_core.h"
#include "frontier_save_access.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "scrcmd_9.h"

typedef void (*FrontierLinkRecvFunc)();

typedef struct FrontierLinkCommand {
    FrontierLinkRecvFunc receive;
    u32 (*getSize)(void);
    void *(*getBuffer)();
} FrontierLinkCommand;

static void BattleTowerLink_ReceivePartnerInfo(int sender, int unused, const u16 *src, FrontierFieldSystem *tower);
static void BattleTowerLink_ReceiveTrainers(int sender, int unused, const void *src, FrontierFieldSystem *tower);
static void BattleTowerLink_ReceiveFlag(int sender, int unused, const u16 *src, FrontierFieldSystem *tower);
static void FrontierLinkLobby_ReceiveFacility(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby);
static void FrontierLinkLobby_ReceiveChoice(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby);
static void FrontierLinkLobby_ReceiveParty(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby);
static void FrontierLinkLobby_ReceiveValue74(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby);
static void FrontierLinkLobby_ReceiveValue71(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby);

static const FrontierLinkCommand sFrontierLinkCommands[] = {
    { BattleFactoryLink_ReceiveRecord,          CommCmd_GetSizeVariable, NULL                              },
    { BattleFactoryLink_ReceiveTrainerIds,      CommCmd_GetSizeVariable, NULL                              },
    { BattleFactoryLink_ReceiveRentalMons,      CommCmd_GetSizeVariable, NULL                              },
    { BattleFactoryLink_ReceiveOpponentMons,    CommCmd_GetSizeVariable, NULL                              },
    { BattleFactoryLink_ReceiveValueA,          CommCmd_GetSizeVariable, NULL                              },
    { BattleFactoryLink_ReceiveValueB,          CommCmd_GetSizeVariable, NULL                              },
    { BattleFactoryLink_ReceivePartyMons,       CommCmd_GetSizeVariable, NULL                              },
    { BattleFactory_ReceivePing,                CommCmd_GetSizeVariable, NULL                              },
    { BattleFactory_ReceiveRentals,             CommCmd_GetSizeVariable, NULL                              },
    { BattleFactory_ReceiveUnused,              CommCmd_GetSizeVariable, NULL                              },
    { BattleFactory_ReceiveSwap,                CommCmd_GetSizeVariable, NULL                              },
    { BattleHallLink_ReceiveUnused,             CommCmd_GetSizeVariable, NULL                              },
    { BattleHallLink_ReceiveEmptyRecord,        CommCmd_GetSizeVariable, NULL                              },
    { BattleHallLink_ReceiveTrainerIds,         CommCmd_GetSizeVariable, NULL                              },
    { BattleHallLink_ReceiveSetIds,             CommCmd_GetSizeVariable, NULL                              },
    { BattleHallLink_ReceiveValue,              CommCmd_GetSizeVariable, NULL                              },
    { BattleHallLink_ReceivePartnerMon,         CommCmd_GetSizeVariable, BattleHallLink_GetRecvMonBuffer   },
    { BattleHall_ReceivePing,                   CommCmd_GetSizeVariable, NULL                              },
    { BattleHall_ReceiveChoice,                 CommCmd_GetSizeVariable, NULL                              },
    { BattleHall_ReceiveAnswer,                 CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleLink_ReceiveRecord,           CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleLink_ReceiveTrainerIds,       CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleLink_ReceiveChoice,           CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleLink_ReceiveOpponentMons,     CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleLink_ReceiveValueA,           CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleLink_ReceiveValueB,           CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleLink_ReceivePartyMons,        CommCmd_GetSizeVariable, BattleCastleLink_GetRecvMonBuffer },
    { BattleCastlePartyApp_ReceiveRanks,        CommCmd_GetSizeVariable, NULL                              },
    { BattleCastlePartyApp_ReceiveAction,       CommCmd_GetSizeVariable, NULL                              },
    { BattleCastlePartyApp_ReceiveCursor,       CommCmd_GetSizeVariable, NULL                              },
    { BattleCastlePartyApp_ReceiveExit,         CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleOpponentApp_ReceiveRanks,     CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleOpponentApp_ReceiveAction,    CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleOpponentApp_ReceiveCursor,    CommCmd_GetSizeVariable, NULL                              },
    { BattleCastleOpponentApp_ReceiveExit,      CommCmd_GetSizeVariable, NULL                              },
    { FrontierLinkLobby_ReceiveFacility,        CommCmd_GetSizeVariable, NULL                              },
    { FrontierLinkLobby_ReceiveChoice,          CommCmd_GetSizeVariable, NULL                              },
    { FrontierLinkLobby_ReceiveParty,           CommCmd_GetSizeVariable, NULL                              },
    { FrontierLinkLobby_ReceiveValue74,         CommCmd_GetSizeVariable, NULL                              },
    { FrontierLinkLobby_ReceiveValue71,         CommCmd_GetSizeVariable, NULL                              },
    { BattleTowerLink_ReceivePartnerInfo,       CommCmd_GetSizeVariable, NULL                              },
    { BattleTowerLink_ReceiveTrainers,          CommCmd_GetSizeVariable, NULL                              },
    { BattleTowerLink_ReceiveFlag,              CommCmd_GetSizeVariable, NULL                              },
    { BattleArcadeLink_ReceiveWinStreak,        CommCmd_GetSizeVariable, NULL                              },
    { BattleArcadeLink_ReceiveTrainerIds,       CommCmd_GetSizeVariable, NULL                              },
    { BattleArcadeLink_ReceiveOpponentMons,     CommCmd_GetSizeVariable, NULL                              },
    { BattleArcadeLink_ReceiveValueA,           CommCmd_GetSizeVariable, NULL                              },
    { BattleArcadeLink_ReceiveValueB,           CommCmd_GetSizeVariable, NULL                              },
    { BattleArcadeLink_ReceivePartyMons,        CommCmd_GetSizeVariable, BattleArcadeLink_GetRecvMonBuffer },
    { BattleArcadeGameBoard_ReceiveBoardLayout, CommCmd_GetSizeVariable, NULL                              },
    { BattleArcadeGameBoard_ReceiveStopPos,     CommCmd_GetSizeVariable, NULL                              },
    { BattleArcadeGameBoard_ReceiveCursorPos,   CommCmd_GetSizeVariable, NULL                              },
};

void FrontierLink_RegisterCommands(void *data) {
    Comm_RegisterCommandTable(sFrontierLinkCommands, NELEMS(sFrontierLinkCommands), data);
}

static void BattleTowerLink_ReceivePartnerInfo(int sender, int unused, const u16 *src, FrontierFieldSystem *tower) {
    u16 result = 0;
    tower->linkReceiveCount++;
    if (sender != Comm_GetLocalNetId()) {
        tower->linkAllyGender = src[0];
        tower->linkAllyMonSpecies[0] = src[1];
        tower->linkAllyMonSpecies[1] = src[2];
        tower->linkAllyRound = src[3];
        tower->multiBattleAllyID = tower->linkAllyGender + 5;
        if (tower->partyMonSpecies[0] == tower->linkAllyMonSpecies[0] || tower->partyMonSpecies[0] == tower->linkAllyMonSpecies[1]) {
            result += 1;
        }
        if (tower->partyMonSpecies[1] == tower->linkAllyMonSpecies[0] || tower->partyMonSpecies[1] == tower->linkAllyMonSpecies[1]) {
            result += 2;
        }
        tower->partnerCheckResult = result;
    }
}

BOOL BattleTowerLink_SendTrainers(FrontierFieldSystem *tower) {
    MI_CpuCopy8(tower->trainersInCurrentRound, tower->linkSendBuffer, sizeof(tower->trainersInCurrentRound));
    if (Comm_SendCommand(FRONTIER_LINK_CMD_TOWER_TRAINERS, tower->linkSendBuffer, sizeof(tower->trainersInCurrentRound)) == 1) {
        return TRUE;
    }
    return FALSE;
}

static void BattleTowerLink_ReceiveTrainers(int sender, int unused, const void *src, FrontierFieldSystem *tower) {
    tower->linkReceiveCount++;
    if (sender != Comm_GetLocalNetId() && Comm_GetLocalNetId() != 0) {
        MI_CpuCopy8(src, tower->trainersInCurrentRound, sizeof(tower->trainersInCurrentRound));
    }
}

static void BattleTowerLink_ReceiveFlag(int sender, int unused, const u16 *src, FrontierFieldSystem *tower) {
    tower->partnerCheckResult = 0;
    tower->linkReceiveCount++;
    if (sender != Comm_GetLocalNetId()) {
        if (tower->linkFlag != 0 || src[0] != 0) {
            tower->partnerCheckResult = 1;
        }
    }
}

BOOL FrontierLinkLobby_SendFacility(FrontierLinkLobby *lobby) {
    int stat;
    FrontierSave *frontier;
    lobby->sendBuffer[0] = lobby->facility;
    stat = FrontierLinkLobby_GetWinStreakStat(lobby->facility);
    frontier = Save_Frontier_GetStatic(lobby->saveData);
    lobby->sendBuffer[1] = FrontierSave_GetStat(frontier, stat, FrontierStat_GetFriendIndex(stat));
    if (Comm_SendCommand(FRONTIER_LINK_CMD_LOBBY_FACILITY, lobby->sendBuffer, sizeof(lobby->sendBuffer)) == 1) {
        return TRUE;
    }
    return FALSE;
}

static void FrontierLinkLobby_ReceiveFacility(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby) {
    lobby->receiveCount++;
    if (sender != Comm_GetLocalNetId()) {
        lobby->partnerFacility = src[0];
        lobby->partnerStreak = src[1];
    }
}

BOOL FrontierLinkLobby_SendChoice(FrontierLinkLobby *lobby, u16 value) {
    lobby->sendBuffer[0] = value;
    if (Comm_SendCommand(FRONTIER_LINK_CMD_LOBBY_CHOICE, lobby->sendBuffer, sizeof(lobby->sendBuffer)) == 1) {
        return TRUE;
    }
    return FALSE;
}

static void FrontierLinkLobby_ReceiveChoice(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby) {
    lobby->receiveCount++;
    if (sender != Comm_GetLocalNetId()) {
        lobby->partnerChoice = src[0];
    }
}

BOOL FrontierLinkLobby_SendParty(FrontierLinkLobby *lobby, u16 first, u16 second) {
    Party *party = SaveArray_Party_Get(lobby->saveData);
    Pokemon *mon;
    lobby->partySlots[0] = first;
    lobby->partySlots[1] = second;
    if (first == 0xFF) {
        lobby->species[0] = 0;
        lobby->items[0] = 0;
        lobby->species[1] = 0;
        lobby->items[1] = 0;
    } else {
        mon = Party_GetMonByIndex(party, first);
        lobby->species[0] = GetMonData(mon, MON_DATA_SPECIES, NULL);
        lobby->items[0] = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
        mon = Party_GetMonByIndex(party, second);
        lobby->species[1] = GetMonData(mon, MON_DATA_SPECIES, NULL);
        lobby->items[1] = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    lobby->sendBuffer[0] = lobby->species[0];
    lobby->sendBuffer[1] = lobby->items[0];
    lobby->sendBuffer[2] = lobby->species[1];
    lobby->sendBuffer[3] = lobby->items[1];
    if (Comm_SendCommand(FRONTIER_LINK_CMD_LOBBY_PARTY, lobby->sendBuffer, sizeof(lobby->sendBuffer)) == 1) {
        return TRUE;
    }
    return FALSE;
}

static void FrontierLinkLobby_ReceiveParty(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby) {
    lobby->receiveCount++;
    if (sender != Comm_GetLocalNetId()) {
        lobby->partnerSpecies[0] = src[0];
        lobby->partnerItems[0] = src[1];
        lobby->partnerSpecies[1] = src[2];
        lobby->partnerItems[1] = src[3];
    }
}

BOOL FrontierLinkLobby_SendValue74(FrontierLinkLobby *lobby, u16 value) {
    lobby->sendBuffer[0] = value;
    if (Comm_SendCommand(FRONTIER_LINK_CMD_LOBBY_VALUE74, lobby->sendBuffer, sizeof(lobby->sendBuffer)) == 1) {
        return TRUE;
    }
    return FALSE;
}

static void FrontierLinkLobby_ReceiveValue74(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby) {
    lobby->receiveCount++;
    if (sender != Comm_GetLocalNetId()) {
        lobby->partnerValue74 = src[0];
    }
}

BOOL FrontierLinkLobby_SendValue71(FrontierLinkLobby *lobby, u16 value) {
    lobby->sendBuffer[0] = value;
    if (Comm_SendCommand(FRONTIER_LINK_CMD_LOBBY_VALUE71, lobby->sendBuffer, sizeof(lobby->sendBuffer)) == 1) {
        return TRUE;
    }
    return FALSE;
}

static void FrontierLinkLobby_ReceiveValue71(int sender, int unused, const u16 *src, FrontierLinkLobby *lobby) {
    lobby->receiveCount++;
    if (sender != Comm_GetLocalNetId()) {
        lobby->partnerValue71 = src[0];
    }
}
