#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_0208F658.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text


	.rodata

	.public gWirelessTradeCommands
gWirelessTradeCommands:
	.word WirelessTrade_RecvPartyChunk, WirelessTrade_GetPartyChunkSize, WirelessTrade_GetRecvBuffer
	.word WirelessTrade_RecvPartnerSelection, sub_020342C4, 0
	.word WirelessTrade_RecvSlot, sub_020342C4, 0
	.word sub_0208F724, sub_020342C4, 0
	.word sub_0208F728, sub_020342C4, 0
	.word sub_0208F72C, sub_020342C4, 0
	.word WirelessTrade_RecvPalPad, WirelessTrade_GetPalPadSize, WirelessTrade_GetRecvBuffer
	.word WirelessTrade_RecvPacket, WirelessTrade_GetPacketSize, WirelessTrade_GetRecvBuffer
	.word sub_0208F738, sub_020342C0, 0
	.word WirelessTrade_RecvRemoteState, sub_020342C4, 0
	.word WirelessTrade_RecvSpecialRibbons, WirelessTrade_GetSpecialRibbonsSize, 0
