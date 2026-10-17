	.public gUnionRoomActivityBusyMsgs
	.public gUnionRoomActivityTradeMsgs
	.public gUnionRoomActivityTrainerCardMsgs
	.public gUnionRoomActivityDrawingMsgs
	.public gUnionRoomActivityBattleMsgs
	.public gUnionRoomActivityRecordMixMsgs
	.public gUnionRoomActivitySpinTradeMsgs
#include "msgdata/msg/msg_0738_UNION.h"
	.include "asm/macros.inc"
	.include "union_room_talk_data.inc"
	.include "global.inc"

	.rodata

gUnionRoomTalkGreetingMsgs:
	.word msg_0738_UNION_00037, msg_0738_UNION_00097

gUnionRoomTalkComeBackMsgs:
	.word msg_0738_UNION_00071, msg_0738_UNION_00131

gUnionRoomTalkAnythingElseMsgs:
	.word msg_0738_UNION_00039, msg_0738_UNION_00099

gUnionRoomTalkLeaveMsgs:
	.word msg_0738_UNION_00038, msg_0738_UNION_00098

gUnionRoomTalkUntradeableEggMsgs:
	.word msg_0738_UNION_00092, msg_0738_UNION_00152

gUnionRoomActivityBusyMsgs:
	.word msg_0738_UNION_00040, msg_0738_UNION_00100

gUnionRoomActivityTradeMsgs:
	.word msg_0738_UNION_00068, msg_0738_UNION_00128
	.word msg_0738_UNION_00069, msg_0738_UNION_00129

gUnionRoomActivityTrainerCardMsgs:
	.word msg_0738_UNION_00044, msg_0738_UNION_00104
	.word msg_0738_UNION_00045, msg_0738_UNION_00105

gUnionRoomGroupFirstSlot:
	.short 10, 14, 18, 22, 26, 30, 34, 38, 42, 46

gUnionRoomTalkJoinGroupMsgs:
	.word msg_0738_UNION_00049, msg_0738_UNION_00109
	.word msg_0738_UNION_00075, msg_0738_UNION_00135
	.word msg_0738_UNION_00087, msg_0738_UNION_00147

gUnionRoomTalkJoinAcceptedMsgs:
	.word msg_0738_UNION_00051, msg_0738_UNION_00111
	.word msg_0738_UNION_00077, msg_0738_UNION_00137
	.word msg_0738_UNION_00089, msg_0738_UNION_00149

gUnionRoomTalkJoinDeclinedMsgs:
	.word msg_0738_UNION_00050, msg_0738_UNION_00110
	.word msg_0738_UNION_00076, msg_0738_UNION_00136
	.word msg_0738_UNION_00088, msg_0738_UNION_00148

gUnionRoomTalkRequirementMsgs:
	.word msg_0738_UNION_00060, msg_0738_UNION_00120
	.word msg_0738_UNION_00070, msg_0738_UNION_00130
	.word msg_0738_UNION_00086, msg_0738_UNION_00146

gUnionRoomLanguageGreetings:
	.word 0, 1, 2, 3, 4, -1, 5

gUnionRoomActivityDrawingMsgs:
	.word msg_0738_UNION_00053, msg_0738_UNION_00113
	.word msg_0738_UNION_00054, msg_0738_UNION_00114
	.word msg_0738_UNION_00055, msg_0738_UNION_00115
	.word msg_0738_UNION_00056, msg_0738_UNION_00116

gUnionRoomActivityBattleMsgs:
	.word msg_0738_UNION_00061, msg_0738_UNION_00121
	.word msg_0738_UNION_00062, msg_0738_UNION_00122
	.word msg_0738_UNION_00063, msg_0738_UNION_00123
	.word msg_0738_UNION_00064, msg_0738_UNION_00124

gUnionRoomActivityRecordMixMsgs:
	.word msg_0738_UNION_00079, msg_0738_UNION_00139
	.word msg_0738_UNION_00080, msg_0738_UNION_00140
	.word msg_0738_UNION_00081, msg_0738_UNION_00141
	.word msg_0738_UNION_00082, msg_0738_UNION_00142

gUnionRoomActivitySpinTradeMsgs:
	.word msg_0738_UNION_00093, msg_0738_UNION_00153
	.word msg_0738_UNION_00094, msg_0738_UNION_00154
	.word msg_0738_UNION_00095, msg_0738_UNION_00155
	.word msg_0738_UNION_00096, msg_0738_UNION_00156

gUnionRoomTalkDeclineRequestMsgs:
	.word msg_0738_UNION_00043, msg_0738_UNION_00103
	.word msg_0738_UNION_00048, msg_0738_UNION_00108
	.word msg_0738_UNION_00059, msg_0738_UNION_00119
	.word msg_0738_UNION_00067, msg_0738_UNION_00127
	.word msg_0738_UNION_00085, msg_0738_UNION_00145

gUnionRoomTalkAcceptRequestMsgs:
	.word msg_0738_UNION_00074, msg_0738_UNION_00134
	.word msg_0738_UNION_00042, msg_0738_UNION_00102
	.word msg_0738_UNION_00057, msg_0738_UNION_00117
	.word msg_0738_UNION_00065, msg_0738_UNION_00125
	.word msg_0738_UNION_00046, msg_0738_UNION_00106
	.word msg_0738_UNION_00072, msg_0738_UNION_00132
	.word msg_0738_UNION_00083, msg_0738_UNION_00143
	.word msg_0738_UNION_00083, msg_0738_UNION_00143

	.text
