	.include "asm/macros.inc"
	.include "battle_arcade_data_tables.inc"
	.include "global.inc"

    .text
    .data

gBattleArcadeEventItemLists: ; 0x0223DCA0
	.word gBattleArcadeBerries_Rank0
	.word gBattleArcadeBerries_Rank3
	.word gBattleArcadeBerries_Rank6
	.word gBattleArcadeItems_Rank0
	.word gBattleArcadeItems_Rank3
	.word gBattleArcadeItems_Rank6

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
	.word BattleArcade_Event_Sun
	.word BattleArcade_Event_Rain
	.word BattleArcade_Event_Sandstorm
	.word BattleArcade_Event_Hail
	.word BattleArcade_Event_Fog
	.word BattleArcade_Event_TrickRoom
	.word BattleArcade_Event_SpeedUp
	.word BattleArcade_Event_SpeedDown
	.word BattleArcade_Event_Random
	.word BattleArcade_Event_SwapParties
	.word BattleArcade_Event_Dummy28
	.word BattleArcade_Event_Dummy29
	.word BattleArcade_Event_Dummy30
	.word BattleArcade_Event_Dummy28

    .bss

gBattleArcadeData: ; 0x0223DD4C
	.space 0x4
