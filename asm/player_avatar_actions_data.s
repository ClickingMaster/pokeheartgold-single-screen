#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public sWhirlpoolSteps
	.public sWaterfallUpSteps
	.public sWaterfallDownSteps
	.public sRockClimbCutInSteps
	.public sRockClimbFollowMonSteps
	.public sIcePathBoulderHoles
	.public sPlayerAvatarBitUpdateFuncs
	.public Field_PlayerAvatar_TransitionToApricornShake
	.public Field_PlayerAvatar_TransitionToCycling
	.public Field_PlayerAvatar_TransitionToFishing
	.public Field_PlayerAvatar_TransitionToHeal
	.public Field_PlayerAvatar_TransitionToLadder
	.public Field_PlayerAvatar_TransitionToPokeathlon
	.public Field_PlayerAvatar_TransitionToPoketch
	.public Field_PlayerAvatar_TransitionToRocket
	.public Field_PlayerAvatar_TransitionToRocketHeal
	.public Field_PlayerAvatar_TransitionToRocketSaving
	.public Field_PlayerAvatar_TransitionToSaving
	.public Field_PlayerAvatar_TransitionToSurfing
	.public Field_PlayerAvatar_TransitionToWalking
	.public Field_PlayerAvatar_TransitionToWatering
	.public Field_PlayerAvatar_TransitionX0008
	.public RockClimb_CheckNextTile
	.public RockClimb_CreateEffect
	.public RockClimb_Finish
	.public RockClimb_FollowMonReact
	.public RockClimb_PlayFollowMonCry
	.public RockClimb_StartCutIn
	.public RockClimb_StartJump
	.public RockClimb_StartStep
	.public RockClimb_WaitCutIn
	.public RockClimb_WaitFollowMonCry
	.public RockClimb_WaitJump
	.public WaterfallDown_Advance
	.public WaterfallDown_Descend
	.public WaterfallDown_Init
	.public WaterfallDown_StartCutIn
	.public WaterfallDown_WaitCutIn
	.public WaterfallUp_Climb
	.public WaterfallUp_Init
	.public WaterfallUp_Rise
	.public WaterfallUp_StartCutIn
	.public WaterfallUp_WaitCutIn
	.public Whirlpool_CheckNextTile
	.public Whirlpool_CreateEffect
	.public Whirlpool_StartCutIn
	.public Whirlpool_StartStep
	.public Whirlpool_WaitCutIn

	.text
	.rodata

sWhirlpoolSteps: ; 0x02206994
	.word Whirlpool_StartCutIn
	.word Whirlpool_WaitCutIn
	.word Whirlpool_CreateEffect
	.word Whirlpool_StartStep
	.word Whirlpool_CheckNextTile

sWaterfallUpSteps: ; 0x022069A8
	.word WaterfallUp_StartCutIn
	.word WaterfallUp_WaitCutIn
	.word WaterfallUp_Init
	.word WaterfallUp_Rise
	.word WaterfallUp_Climb

sWaterfallDownSteps: ; 0x022069BC
	.word WaterfallDown_StartCutIn
	.word WaterfallDown_WaitCutIn
	.word WaterfallDown_Init
	.word WaterfallDown_Advance
	.word WaterfallDown_Descend

sRockClimbCutInSteps: ; 0x022069D0
	.word RockClimb_StartCutIn
	.word RockClimb_WaitCutIn
	.word RockClimb_CreateEffect
	.word RockClimb_StartJump
	.word RockClimb_WaitJump
	.word RockClimb_StartStep
	.word RockClimb_CheckNextTile
	.word RockClimb_Finish

sRockClimbFollowMonSteps: ; 0x022069F0
	.word RockClimb_FollowMonReact
	.word RockClimb_PlayFollowMonCry
	.word RockClimb_WaitFollowMonCry
	.word RockClimb_CreateEffect
	.word RockClimb_StartJump
	.word RockClimb_WaitJump
	.word RockClimb_StartStep
	.word RockClimb_CheckNextTile
	.word RockClimb_Finish

sIcePathBoulderHoles: ; 0x02206A14
	.byte 0xED, 0x00, 0x0B, 0x00, 0x0A, 0x00, 0x00, 0x00, 0xEE, 0x01, 0xED, 0x00
	.byte 0x0A, 0x00, 0x12, 0x00, 0x01, 0x00, 0xEF, 0x01, 0xED, 0x00, 0x12, 0x00, 0x07, 0x00, 0x02, 0x00
	.byte 0xF0, 0x01, 0xED, 0x00, 0x13, 0x00, 0x13, 0x00, 0x03, 0x00, 0xF1, 0x01

sPlayerAvatarBitUpdateFuncs: ; 0x02206A3C
	.word Field_PlayerAvatar_TransitionToWalking
	.word Field_PlayerAvatar_TransitionToCycling
	.word Field_PlayerAvatar_TransitionToSurfing
	.word Field_PlayerAvatar_TransitionX0008
	.word Field_PlayerAvatar_TransitionToWatering
	.word Field_PlayerAvatar_TransitionToFishing
	.word Field_PlayerAvatar_TransitionToPoketch
	.word Field_PlayerAvatar_TransitionToSaving
	.word Field_PlayerAvatar_TransitionToHeal
	.word Field_PlayerAvatar_TransitionToLadder
	.word Field_PlayerAvatar_TransitionToRocket
	.word Field_PlayerAvatar_TransitionToRocketHeal
	.word Field_PlayerAvatar_TransitionToPokeathlon
	.word Field_PlayerAvatar_TransitionToApricornShake
	.word Field_PlayerAvatar_TransitionToRocketSaving
