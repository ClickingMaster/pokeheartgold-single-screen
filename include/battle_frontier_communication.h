#ifndef POKEHEARTGOLD_BATTLE_FRONTIER_COMMUNICATION_H
#define POKEHEARTGOLD_BATTLE_FRONTIER_COMMUNICATION_H

#include "global.h"

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
