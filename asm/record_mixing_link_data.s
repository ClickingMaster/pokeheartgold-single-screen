#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_0208FB64.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text


	.rodata

	.public gRecordMixingLinkCommands
gRecordMixingLinkCommands:
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word RecordMixingLink_RecvUnused108, RecordMixingLink_GetSize1, 0
	.word RecordMixingLink_RecvUnused109, RecordMixingLink_GetSize1, 0
	.word RecordMixingLink_RecvMemberJoined, RecordMixingLink_GetSize1, 0
	.word RecordMixingLink_RecvClearMessage, RecordMixingLink_GetSize0, 0
	.word RecordMixingLink_HandleJoinRequest, RecordMixingLink_GetJoinRequestSize, 0
	.word RecordMixingLink_RecvCloseYesNo, RecordMixingLink_GetSize0, 0
	.word RecordMixingLink_RecvJoinNotice, RecordMixingLink_GetSize0, 0
	.word RecordMixingLink_RecvStartMixing, RecordMixingLink_GetSize0, 0
	.word RecordMixingLink_RecvRecords, CommCmd_GetSize3008, RecordMixingLink_GetRecvBuffer
	.word RecordMixingLink_RecvLeaderMenuState, RecordMixingLink_GetSize1, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
	.word CommCmd_RecvIgnored, RecordMixingLink_GetSize0, 0
