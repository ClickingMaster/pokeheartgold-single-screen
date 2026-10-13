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
	.word SpinTradeLink_HandleMemberJoined
	.word SpinTradeLink_HandleClearMessage
	.word SpinTradeLink_HandleJoinRequest
	.word SpinTradeLink_HandleCloseYesNo
	.word SpinTradeLink_HandleJoinNotice
	.word SpinTradeLink_HandleStartSync
	.word SpinTradeLink_HandleSyncCount
	.word SpinTradeLink_HandleLeaderMenuState
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
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize0, 0
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
