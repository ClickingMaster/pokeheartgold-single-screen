#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "unk_02096C88.inc"
	.include "global.inc"

	.rodata

	; File boundary
	.public gSpinTradeLinkPacketHandlers
gSpinTradeLinkPacketHandlers:
	.word sub_02096ED0
	.word sub_02096DE4
	.word SpinTradeLink_HandleJoinRequest
	.word sub_02096EF0
	.word sub_02096F0C
	.word sub_02096DBC
	.word sub_02096DB4
	.word sub_02096DA8
	.word SpinTradeLink_HandleStatus
	.word SpinTradeLink_HandleJoin
	.word SpinTradeLink_HandleJump
	.word SpinTradeLink_HandleSpin
	.word SpinTradeLink_HandleSlot
	.word SpinTradeLink_HandleCount
	.word SpinTradeLink_HandleTradeSlot
	.word SpinTradeLink_HandleResult
	.word SpinTradeLink_HandleCancel
	.word SpinTradeLink_HandleConfirm
	.public gSpinTradeLinkCommands
gSpinTradeLinkCommands:
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word sub_02091570, CommCmd_GetSize0, 0
	.word SpinTradeLink_ReceivePacket, SpinTradeLink_GetPacketSize, 0
	.word SpinTradeLink_ReceiveParty, SpinTradeLink_GetPartySize, SpinTradeLink_GetPartyRecvBuffer
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
