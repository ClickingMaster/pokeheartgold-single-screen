	.include "asm/macros.inc"
	.include "battle_arcade_data_tables.inc"
	.include "global.inc"

    .text
    .data

ov80_0223DCA0: ; 0x0223DCA0
	.word ov80_0223BEA8
	.word ov80_0223BF32
	.word ov80_0223BED8
	.word ov80_0223BEEC
	.word ov80_0223BF18
	.word ov80_0223BF02

ov80_0223DCB8: ; 0x0223DCB8
	.word BattleArcade_Event_LowerHp
	.word BattleArcade_Event_Poison
	.word BattleArcade_Event_Paralyze
	.word BattleArcade_Event_Burn
	.word BattleArcade_Event_Sleep
	.word BattleArcade_Event_Freeze
	.word ov80_022351AC
	.word ov80_02235208
	.word BattleArcade_Event_LevelUp
	.word BattleArcade_Event_LowerHp
	.word BattleArcade_Event_Poison
	.word BattleArcade_Event_Paralyze
	.word BattleArcade_Event_Burn
	.word BattleArcade_Event_Sleep
	.word BattleArcade_Event_Freeze
	.word ov80_022351AC
	.word ov80_02235208
	.word BattleArcade_Event_LevelUp
	.word ov80_022352BC
	.word ov80_022352C8
	.word ov80_022352D0
	.word ov80_022352D8
	.word ov80_022352E0
	.word ov80_022352E8
	.word ov80_022352F4
	.word ov80_02235300
	.word ov80_0223530C
	.word ov80_02235314
	.word ov80_02235318
	.word ov80_0223531C
	.word ov80_02235320
	.word ov80_02235318

    .bss

gBattleArcadeData: ; 0x0223DD4C
	.space 0x4
