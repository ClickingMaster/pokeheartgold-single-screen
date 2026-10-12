	.include "asm/macros.inc"
	.public _02237A86
	.public _02237A8E
	.public _02237A94
	.public _02237A9C
	.public _02237AAA
	.public _02237AB6
	.public _02237AC4
	.public _02237AC8
	.public _02237ACC
	.public _02237AD0
	.public _02237AD4
	.public _02237AD8
	.public _02237AEC
	.public _02237B04
	.public _02237B12
	.public _02237B1A
	.public _02237B36
	.public _02237B3E
	.public _02237B42
	.public _02237B4A
	.public _02237B4E
	.public _02237B6A
	.public _02237B72
	.public _02237B76
	.public _02237B7E
	.public _02237B82
	.public _02237BF0
	.public _02237BF2
	.public _02237C04
	.public _02237C2A
	.public _02237C70
	.public _02237C90
	.public _02237CB0
	.public _02237CC0
	.public _02237D28
	.public _02237D4E
	.public _02237D54
	.public _02237D6C
	.public _02237D74
	.public _02237D78
	.public _02237D7C
	.public _02237D80
	.public _02237D84
	.public _02237D94
	.public _02237D98
	.public _02237DAC
	.public _02237DDE
	.public _02237DEC
	.public _02237DF2
	.public _02237E5A
	.public _02237E7E
	.public _02237EA8
	.public _02237EC0
	.public _02237ECA
	.public _02237ECE
	.public _02237ED0
	.public _02237ED4
	.public _02237EF4
	.public _02237EF8
	.public _02237F4E
	.public _02237F6A
	.public _02237F74
	.public _02237F84
	.public _02237FA2
	.public _0223800E
	.public _02238030
	.public BattleCastle_GetRandomTrainerId
	.public BattleCastle_GetRoundTrainerIds
	.public BattleCastle_GetPartySize
	.public BattleCastle_GetOpponentMonCount
	.public BattleCastle_NewBattleSetup
	.public BattleCastle_GetBattleType
	.public BattleCastle_GetMonLevel
	.public BattleCastle_IsMultiType
	.public BattleCastle_ReviveParty
	.public BattleCastle_SetMonOriginalTrainer
	.public BattleCastle_AddMonToParty
	.public BattleCastle_SetOpponentParty
	.public BattleCastle_GetTrainerAIFlags
	.public BattleCastle_GetRound
	.public BattleCastle_DrawRoundTiles
	.public BattleCastle_BuildRoundTilemap
	.public BattleCastle_ClampRound
	.public BattleCastle_SpendPoints
	.public gBattleCastleRoundTrainerRanges
	.public ov80_0223D4D6
	.public ov80_0223D4D8
	.public ov80_0223D4DA
	.include "battle_castle_setup_data.inc"
	.include "global.inc"

    .text
	.public BattleCastle_GetRandomTrainerId
	.public BattleCastle_GetPartySize
	.public BattleCastle_GetOpponentMonCount
	.public BattleCastle_GetBattleType
	.public BattleCastle_GetMonLevel
	.public BattleCastle_IsMultiType
	.public BattleCastle_ReviveParty
	.public BattleCastle_SetMonOriginalTrainer
	.public BattleCastle_AddMonToParty
	.public BattleCastle_SetOpponentParty
	.public BattleCastle_GetTrainerAIFlags
	.public BattleCastle_GetRound
	.public BattleCastle_DrawRoundTiles
	.public BattleCastle_ClampRound
	.public BattleCastle_SpendPoints
    .rodata

gBattleCastleRoundTrainerRanges: ; 0x0223D4D4
	.byte 0x00, 0x00

ov80_0223D4D6: ; 0x0223D4D6
	.byte 0x63, 0x00

ov80_0223D4D8: ; 0x0223D4D8
	.byte 0x64, 0x00

ov80_0223D4DA: ; 0x0223D4DA
	.byte 0x77, 0x00, 0x50, 0x00, 0x77, 0x00
	.byte 0x78, 0x00, 0x8B, 0x00, 0x64, 0x00, 0x8B, 0x00, 0x8C, 0x00, 0x9F, 0x00, 0x78, 0x00, 0x9F, 0x00
	.byte 0xA0, 0x00, 0xB3, 0x00, 0x8C, 0x00, 0xB3, 0x00, 0xB4, 0x00, 0xC7, 0x00, 0xA0, 0x00, 0xC7, 0x00
	.byte 0xC8, 0x00, 0xDB, 0x00, 0xB4, 0x00, 0xDB, 0x00, 0xDC, 0x00, 0xEF, 0x00, 0xC8, 0x00, 0x2B, 0x01
	.byte 0xC8, 0x00, 0x2B, 0x01
