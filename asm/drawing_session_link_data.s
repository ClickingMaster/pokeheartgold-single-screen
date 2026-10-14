#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "drawing_session_link_data.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.public DrawingSessionLink_RegisterCommands
	.public DrawingSessionLink_ReceivePictureChunk
	.public DrawingSessionLink_ReceiveMemberStroke
	.public DrawingSessionLink_ReceiveStrokes
	.public DrawingSessionLink_ReceivePictureDone
	.public DrawingSessionLink_HandleJoinRequest
	.public DrawingSessionLink_ReceiveDummy103
	.public DrawingSessionLink_ReceiveMemberJoined
	.public DrawingSessionLink_ReceiveDummy99
	.public DrawingSessionLink_ReceiveDummy100
	.public DrawingSessionLink_ReceiveEvent15
	.public DrawingSessionLink_ReceiveJoinNotice
	.public DrawingSessionLink_ReceiveSetJoinState1
	.public DrawingSessionLink_SendPictureChunk
	.public DrawingSessionLink_GetChunkRecvBuffer
	.public DrawingSessionLink_GetSize4
	.public DrawingSessionLink_GetJoinRequestSize
	.public gDrawingSessionLinkCommands

	.rodata

gDrawingSessionLinkCommands:
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, DrawingSessionLink_GetSize4, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word DrawingSessionLink_ReceivePictureChunk, CommCmd_GetSize1008, DrawingSessionLink_GetChunkRecvBuffer
	.word DrawingSessionLink_ReceiveMemberStroke, CommCmd_GetSize10, 0
	.word DrawingSessionLink_ReceiveStrokes, CommCmd_GetSize50, 0
	.word DrawingSessionLink_ReceiveDummy99, CommCmd_GetSize1, 0
	.word DrawingSessionLink_ReceiveDummy100, CommCmd_GetSize1, 0
	.word DrawingSessionLink_ReceiveMemberJoined, CommCmd_GetSize1, 0
	.word DrawingSessionLink_ReceivePictureDone, CommCmd_GetSize0, 0
	.word DrawingSessionLink_ReceiveDummy103, CommCmd_GetSize0, 0
	.word DrawingSessionLink_HandleJoinRequest, DrawingSessionLink_GetJoinRequestSize, 0
	.word DrawingSessionLink_ReceiveEvent15, CommCmd_GetSize0, 0
	.word DrawingSessionLink_ReceiveJoinNotice, CommCmd_GetSize0, 0
	.word DrawingSessionLink_ReceiveSetJoinState1, CommCmd_GetSize0, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
	.word CommCmd_RecvIgnored, CommCmd_GetSize1, 0
