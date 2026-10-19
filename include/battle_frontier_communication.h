#ifndef POKEHEARTGOLD_BATTLE_FRONTIER_COMMUNICATION_H
#define POKEHEARTGOLD_BATTLE_FRONTIER_COMMUNICATION_H

#include "global.h"

// Link command ids of the frontier commands, in the order of their handlers in FrontierLink_RegisterCommands
// (they follow the communication core's own commands).
enum FrontierLinkCommandId {
    FRONTIER_LINK_CMD_FACTORY_RECORD = 22,
    FRONTIER_LINK_CMD_FACTORY_TRAINER_IDS,
    FRONTIER_LINK_CMD_FACTORY_RENTAL_MONS,
    FRONTIER_LINK_CMD_FACTORY_OPPONENT_MONS,
    FRONTIER_LINK_CMD_FACTORY_VALUE_A,
    FRONTIER_LINK_CMD_FACTORY_VALUE_B,
    FRONTIER_LINK_CMD_FACTORY_PARTY_MONS,
    FRONTIER_LINK_CMD_FACTORY_PING,
    FRONTIER_LINK_CMD_FACTORY_RENTALS,
    FRONTIER_LINK_CMD_FACTORY_UNUSED,
    FRONTIER_LINK_CMD_FACTORY_SWAP,
    FRONTIER_LINK_CMD_HALL_UNUSED,
    FRONTIER_LINK_CMD_HALL_EMPTY_RECORD,
    FRONTIER_LINK_CMD_HALL_TRAINER_IDS,
    FRONTIER_LINK_CMD_HALL_SET_IDS,
    FRONTIER_LINK_CMD_HALL_VALUE,
    FRONTIER_LINK_CMD_HALL_PARTNER_MON,
    FRONTIER_LINK_CMD_HALL_PING,
    FRONTIER_LINK_CMD_HALL_CHOICE,
    FRONTIER_LINK_CMD_HALL_ANSWER,
    FRONTIER_LINK_CMD_CASTLE_RECORD,
    FRONTIER_LINK_CMD_CASTLE_TRAINER_IDS,
    FRONTIER_LINK_CMD_CASTLE_CHOICE,
    FRONTIER_LINK_CMD_CASTLE_OPPONENT_MONS,
    FRONTIER_LINK_CMD_CASTLE_VALUE_A,
    FRONTIER_LINK_CMD_CASTLE_VALUE_B,
    FRONTIER_LINK_CMD_CASTLE_PARTY_MONS,
    FRONTIER_LINK_CMD_CASTLE_PARTY_RANKS,
    FRONTIER_LINK_CMD_CASTLE_PARTY_ACTION,
    FRONTIER_LINK_CMD_CASTLE_PARTY_CURSOR,
    FRONTIER_LINK_CMD_CASTLE_PARTY_EXIT,
    FRONTIER_LINK_CMD_CASTLE_OPPONENT_RANKS,
    FRONTIER_LINK_CMD_CASTLE_OPPONENT_ACTION,
    FRONTIER_LINK_CMD_CASTLE_OPPONENT_CURSOR,
    FRONTIER_LINK_CMD_CASTLE_OPPONENT_EXIT,
    FRONTIER_LINK_CMD_LOBBY_FACILITY,
    FRONTIER_LINK_CMD_LOBBY_CHOICE,
    FRONTIER_LINK_CMD_LOBBY_PARTY,
    FRONTIER_LINK_CMD_LOBBY_VALUE74,
    FRONTIER_LINK_CMD_LOBBY_VALUE71,
    FRONTIER_LINK_CMD_TOWER_PARTNER_INFO,
    FRONTIER_LINK_CMD_TOWER_TRAINERS,
    FRONTIER_LINK_CMD_TOWER_FLAG,
    FRONTIER_LINK_CMD_ARCADE_WIN_STREAK,
    FRONTIER_LINK_CMD_ARCADE_TRAINER_IDS,
    FRONTIER_LINK_CMD_ARCADE_OPPONENT_MONS,
    FRONTIER_LINK_CMD_ARCADE_VALUE_A,
    FRONTIER_LINK_CMD_ARCADE_VALUE_B,
    FRONTIER_LINK_CMD_ARCADE_PARTY_MONS,
    FRONTIER_LINK_CMD_ARCADE_BOARD_LAYOUT,
    FRONTIER_LINK_CMD_ARCADE_BOARD_STOP_POS,
    FRONTIER_LINK_CMD_ARCADE_BOARD_CURSOR_POS,
};

struct BattleFactoryApp;
struct BattleFactoryLinkMessage;
struct BattleHallApp;
struct BattleCastlePartyApp;
struct BattleCastleOpponentApp;
struct FrontierFieldSystem;
struct FrontierLinkLobby;

void FrontierLink_RegisterCommands(void *data);
BOOL BattleTowerLink_SendTrainers(struct FrontierFieldSystem *);
BOOL FrontierLinkLobby_SendFacility(struct FrontierLinkLobby *);
BOOL FrontierLinkLobby_SendChoice(struct FrontierLinkLobby *, u16);
BOOL FrontierLinkLobby_SendParty(struct FrontierLinkLobby *, u16, u16);
BOOL FrontierLinkLobby_SendValue74(struct FrontierLinkLobby *lobby, u16 value);
BOOL FrontierLinkLobby_SendValue71(struct FrontierLinkLobby *, u16);

// Link command handlers of the facility apps
void BattleFactory_ReceivePing(int netId);
void BattleFactory_ReceiveRentals(int netId, int size, const u16 *data, struct BattleFactoryApp *app);
void BattleFactory_ReceiveUnused(void);
void BattleFactory_ReceiveSwap(int netId, int size, const struct BattleFactoryLinkMessage *msg, struct BattleFactoryApp *app);
void BattleHall_ReceivePing(u32 netId);
void BattleHall_ReceiveChoice(u32 netId, u32 unused, const u16 *msg, struct BattleHallApp *app);
void BattleHall_ReceiveAnswer(u32 netId, u32 unused, const u16 *msg, struct BattleHallApp *app);
void BattleCastlePartyApp_ReceiveRanks(int netId, void *unused, void *data, struct BattleCastlePartyApp *app);
void BattleCastlePartyApp_ReceiveAction(int netId, int unused, u16 *data, struct BattleCastlePartyApp *app);
void BattleCastlePartyApp_ReceiveCursor(int netId, int unused, u16 *data, struct BattleCastlePartyApp *app);
void BattleCastlePartyApp_ReceiveExit(int netId, int unused, u16 *data, struct BattleCastlePartyApp *app);
void BattleCastleOpponentApp_ReceiveRanks(int netId, void *unused, u16 *data, struct BattleCastleOpponentApp *app);
void BattleCastleOpponentApp_ReceiveAction(int netId, int unused, u16 *data, struct BattleCastleOpponentApp *app);
void BattleCastleOpponentApp_ReceiveCursor(int netId, int unused, u16 *data, struct BattleCastleOpponentApp *app);
void BattleCastleOpponentApp_ReceiveExit(int netId, int unused, u16 *data, struct BattleCastleOpponentApp *app);

#endif // POKEHEARTGOLD_BATTLE_FRONTIER_COMMUNICATION_H
