#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "trade_communication_data.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text


	.rodata

	.public gWirelessTradeCommands
gWirelessTradeCommands:
	.word WirelessTrade_RecvPartyChunk, WirelessTrade_GetPartyChunkSize, WirelessTrade_GetRecvBuffer
	.word WirelessTrade_RecvPartnerSelection, CommCmd_GetSize1, 0
	.word WirelessTrade_RecvSlot, CommCmd_GetSize1, 0
	.word WirelessTrade_RecvUnused3, CommCmd_GetSize1, 0
	.word WirelessTrade_RecvUnused4, CommCmd_GetSize1, 0
	.word WirelessTrade_RecvSetState2, CommCmd_GetSize1, 0
	.word WirelessTrade_RecvPalPad, WirelessTrade_GetPalPadSize, WirelessTrade_GetRecvBuffer
	.word WirelessTrade_RecvPacket, WirelessTrade_GetPacketSize, WirelessTrade_GetRecvBuffer
	.word WirelessTrade_RecvUnused8, CommCmd_GetSize0, 0
	.word WirelessTrade_RecvRemoteState, CommCmd_GetSize1, 0
	.word WirelessTrade_RecvSpecialRibbons, WirelessTrade_GetSpecialRibbonsSize, 0
