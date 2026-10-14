	.include "asm/macros.inc"
	.include "communication_command_data.inc"
	.include "global.inc"

	.bss

	.public gCommCommandTable
gCommCommandTable:
	.space 0x4

	.rodata

	.public gCommMaxChildren
gCommMaxChildren:
	.byte 0x01, 0x01, 0x01, 0x01
	.byte 0x03, 0x03, 0x03, 0x04, 0x03, 0x04, 0x07, 0x03, 0x07, 0x04, 0x01, 0x04, 0x01, 0x01, 0x04, 0x01
	.byte 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x04, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x03, 0x01, 0x03
	.byte 0x00, 0x01, 0x01, 0x01, 0x03
	.public gCommMinChildren
gCommMinChildren:
	.byte 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01
	.byte 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01
	.byte 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00
	.public gCommBaseCommands
gCommBaseCommands:
	.word 0, CommCmd_GetSize0, 0
	.word 0, CommCmd_GetSize0, 0
	.word Comm_RecvShutdown, CommCmd_GetSize0, 0
	.word CommPlayerInfo_RecvPlayerEntry, CommPlayerInfo_GetEntrySize, 0
	.word CommPlayerInfo_RecvEntry, CommPlayerInfo_GetEntrySize, 0
	.word CommPlayerInfo_RecvSetUnk3A4, CommCmd_GetSize0, 0
	.word CommState_RecvJoinRequest, CommState_GetJoinMessageSize, 0
	.word CommState_RecvJoinReply, CommState_GetJoinMessageSize, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word Comm_RecvTransferModeRequest, CommCmd_GetSize1, 0
	.word Comm_RecvTransferModeChange, CommCmd_GetSize1, 0
	.word Comm_RecvTransferModeAck, CommCmd_GetSize1, 0
	.word CommCmd_RecvReady, CommCmd_GetSize0, 0
	.word CommCmd_RecvShutdown, CommCmd_GetSize0, 0
	.word CommCmd_RecvNotReady, CommCmd_GetSize0, 0
	.word LinkSync_RecvRequest, CommCmd_GetSize1, 0
	.word LinkSync_RecvDone, CommCmd_GetSize1, 0
	.word LinkSync_RecvState, CommCmd_GetSize2, 0
	.word LinkSync_RecvValue, LinkSync_GetValueSize, 0
	.word LinkSync_RecvPacket, CommState_GetSyncPacketSize, 0
	.word CommState_RecvWifiDisconnect, CommCmd_GetSize0, 0
