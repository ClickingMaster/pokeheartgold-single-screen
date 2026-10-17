	.public sRecordMixingHandlers
	.extern RecordMixing_Block23List2_GetSize
	.extern RecordMixing_Block23List0_GetSize
	.extern RecordMixing_Block23List1_GetSize
	.extern RecordMixing_Block23List2_Load
	.extern RecordMixing_Block23List0_Load
	.extern RecordMixing_Block23List1_Load
	.extern RecordMixing_Block23List2_Load2
	.extern RecordMixing_Block23List0_Load2
	.extern RecordMixing_Block23List1_Load2
	.extern RecordMixing_FriendGroup_GetSize
	.extern RecordMixing_FriendGroup_Load
	.extern RecordMixing_FriendGroup_Merge
	.extern RecordMixing_Fashion_GetSize
	.extern RecordMixing_Fashion_Load
	.extern RecordMixing_Fashion_Merge
	.extern RecordMixing_Block23_Merge
	.extern RecordMixing_Rankings_Load
	.extern RecordMixing_Rankings_Merge
	.extern RecordMixing_BattleRoomTrainer_Load
	.extern RecordMixing_BattleRoomTrainer_Merge
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_73.inc"
	.include "global.inc"

	.rodata

sRecordMixingHandlers:
	.word RecordMixing_FriendGroup_GetSize, RecordMixing_FriendGroup_Load, RecordMixing_FriendGroup_Merge
	.word RecordMixing_Fashion_GetSize, RecordMixing_Fashion_Load, RecordMixing_Fashion_Merge
	.word RecordMixing_Block23List0_GetSize, RecordMixing_Block23List0_Load, 0
	.word RecordMixing_Block23List1_GetSize, RecordMixing_Block23List1_Load, 0
	.word RecordMixing_Block23List2_GetSize, RecordMixing_Block23List2_Load, 0
	.word Save_Rankings_GetMixingSize, RecordMixing_Rankings_Load, RecordMixing_Rankings_Merge
	.word BattleRoomTrainer_sizeof, RecordMixing_BattleRoomTrainer_Load, RecordMixing_BattleRoomTrainer_Merge
	.word RecordMixing_Block23List0_GetSize, RecordMixing_Block23List0_Load2, 0
	.word RecordMixing_Block23List1_GetSize, RecordMixing_Block23List1_Load2, 0
	.word RecordMixing_Block23List2_GetSize, RecordMixing_Block23List2_Load2, RecordMixing_Block23_Merge

