	.include "asm/macros.inc"
	.include "battle_arcade_data_tables.inc"
	.include "global.inc"

    .text
    .data

gBattleArcadeEventItemLists: ; 0x0223DCA0
	.word ov80_0223BEA8
	.word ov80_0223BF32
	.word ov80_0223BED8
	.word ov80_0223BEEC
	.word ov80_0223BF18
	.word ov80_0223BF02

gBattleArcadeEventFuncs: ; 0x0223DCB8
	.word BattleArcade_Event_LowerHp
	.word BattleArcade_Event_Poison
	.word BattleArcade_Event_Paralyze
	.word BattleArcade_Event_Burn
	.word BattleArcade_Event_Sleep
	.word BattleArcade_Event_Freeze
	.word BattleArcade_Event_GiveBerry
	.word BattleArcade_Event_GiveItem
	.word BattleArcade_Event_LevelUp
	.word BattleArcade_Event_LowerHp
	.word BattleArcade_Event_Poison
	.word BattleArcade_Event_Paralyze
	.word BattleArcade_Event_Burn
	.word BattleArcade_Event_Sleep
	.word BattleArcade_Event_Freeze
	.word BattleArcade_Event_GiveBerry
	.word BattleArcade_Event_GiveItem
	.word BattleArcade_Event_LevelUp
	.word ov80_022352BC
	.word BattleArcade_Event_Rain
	.word BattleArcade_Event_Sandstorm
	.word ov80_022352D8
	.word ov80_022352E0
	.word ov80_022352E8
	.word BattleArcade_Event_SpeedUp
	.word BattleArcade_Event_SpeedDown
	.word BattleArcade_Event_Random
	.word ov80_02235314
	.word ov80_02235318
	.word ov80_0223531C
	.word ov80_02235320
	.word ov80_02235318

    .bss

gBattleArcadeData: ; 0x0223DD4C
	.space 0x4
