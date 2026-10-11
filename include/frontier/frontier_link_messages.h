#ifndef POKEHEARTGOLD_FRONTIER_FRONTIER_LINK_MESSAGES_H
#define POKEHEARTGOLD_FRONTIER_FRONTIER_LINK_MESSAGES_H

#include "frontier/overlay_80_022340E8.h"

// Link messages of the facilities' script side (frontier_link_messages.c). The work structs are views of the
// facilities' script work, defined in frontier_link_messages.c.
typedef struct FactoryLinkWork FactoryLinkWork;
typedef struct HallLinkWork HallLinkWork;
typedef struct CastleLinkWork CastleLinkWork;
typedef struct ArcadeMonsMessage ArcadeMonsMessage;

void ov80_0222AEF8(int netId, int size, void *data, HallLinkWork *work);
BOOL ov80_0222AF10(HallLinkWork *work);
void ov80_0222AF3C(int netId, int size, void *data, HallLinkWork *work);
BOOL ov80_0222AF54(HallLinkWork *work);
void ov80_0222AF84(int netId, int size, const u16 *data, HallLinkWork *work);
BOOL ov80_0222AFB8(HallLinkWork *work);
void ov80_0222AFEC(int netId, int size, const u16 *data, HallLinkWork *work);
BOOL ov80_0222B024(HallLinkWork *work, u16 value);
void ov80_0222B048(int netId, int size, const u16 *data, HallLinkWork *work);
BOOL ov80_0222B070(HallLinkWork *work);
void BattleHallLink_ReceivePartnerMon(int netId, int size, const void *data, HallLinkWork *work);
void *BattleHallLink_GetRecvMonBuffer(int netId, HallLinkWork *work, int size);
BOOL ov80_0222B108(FactoryLinkWork *work);
void ov80_0222B140(int netId, int size, const u16 *data, FactoryLinkWork *work);
BOOL BattleFactoryLink_SendTrainerIds(FactoryLinkWork *work);
void BattleFactoryLink_ReceiveTrainerIds(int netId, int size, const u16 *data, FactoryLinkWork *work);
BOOL BattleFactoryLink_SendRentalMons(FactoryLinkWork *work);
void BattleFactoryLink_ReceiveRentalMons(int netId, int size, const u16 *data, FactoryLinkWork *work);
BOOL BattleFactoryLink_SendOpponentMons(FactoryLinkWork *work);
void BattleFactoryLink_ReceiveOpponentMons(int netId, int size, const u16 *data, FactoryLinkWork *work);
BOOL ov80_0222B3B0(FactoryLinkWork *work, u16 value);
void ov80_0222B3D4(int netId, int size, const u16 *data, FactoryLinkWork *work);
BOOL ov80_0222B3FC(FactoryLinkWork *work, u16 value);
void ov80_0222B420(int netId, int size, const u16 *data, FactoryLinkWork *work);
BOOL BattleFactoryLink_SendPartyMons(FactoryLinkWork *work);
void BattleFactoryLink_ReceivePartyMons(int netId, int size, u16 *data, FactoryLinkWork *work);
BOOL ov80_0222B5C8(CastleLinkWork *work);
void ov80_0222B628(int netId, int size, const u16 *data, CastleLinkWork *work);
BOOL BattleCastleLink_SendTrainerIds(CastleLinkWork *work);
void BattleCastleLink_ReceiveTrainerIds(int netId, int size, const u16 *data, CastleLinkWork *work);
BOOL ov80_0222B6C8(CastleLinkWork *work);
void ov80_0222B740(int netId, int size, void *data, CastleLinkWork *work);
BOOL BattleCastleLink_SendOpponentMons(CastleLinkWork *work);
void BattleCastleLink_ReceiveOpponentMons(int netId, int size, void *data, CastleLinkWork *work);
BOOL ov80_0222B8D8(CastleLinkWork *work, u16 value);
void ov80_0222B8F8(int netId, int size, const u16 *data, CastleLinkWork *work);
BOOL ov80_0222B920(CastleLinkWork *work, u16 value);
void ov80_0222B940(int netId, int size, const u16 *data, CastleLinkWork *work);
BOOL BattleCastleLink_SendPartyMons(CastleLinkWork *work);
void BattleCastleLink_ReceivePartyMons(int netId, int size, void *data, CastleLinkWork *work);
void *BattleCastleLink_GetRecvMonBuffer(int netId, CastleLinkWork *work, int size);
BOOL BattleArcadeLink_SendWinStreak(ArcadeContext *ctx);
void BattleArcadeLink_ReceiveWinStreak(int netId, int size, const u16 *data, ArcadeContext *ctx);
BOOL BattleArcadeLink_SendTrainerIds(ArcadeContext *ctx);
void BattleArcadeLink_ReceiveTrainerIds(int netId, int size, const u16 *data, ArcadeContext *ctx);
BOOL BattleArcadeLink_SendOpponentMons(ArcadeContext *ctx);
void BattleArcadeLink_ReceiveOpponentMons(int netId, int size, ArcadeMonsMessage *msg, ArcadeContext *ctx);
BOOL ov80_0222BC48(ArcadeContext *ctx, u16 value);
void ov80_0222BC6C(int netId, int size, const u16 *data, ArcadeContext *ctx);
BOOL ov80_0222BC94(ArcadeContext *ctx, u16 value);
void ov80_0222BCB8(int netId, int size, const u16 *data, ArcadeContext *ctx);
BOOL BattleArcadeLink_SendPartyMons(ArcadeContext *ctx);
void BattleArcadeLink_ReceivePartyMons(int netId, int size, void *data, ArcadeContext *ctx);
void *BattleArcadeLink_GetRecvMonBuffer(int netId, ArcadeContext *ctx, int size);

#endif // POKEHEARTGOLD_FRONTIER_FRONTIER_LINK_MESSAGES_H
