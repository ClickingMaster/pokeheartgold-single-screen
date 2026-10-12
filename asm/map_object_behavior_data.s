#include "constants/sndseq.h"
#include "constants/species.h"
#include "constants/maps.h"
#include "constants/pokemon.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/items.h"
#include "constants/std_script.h"
#include "fielddata/script/scr_seq/event_D24R0204.h"
#include "constants/field_move_response.h"
	.include "asm/macros.inc"
	.include "unk_020632B0.inc"
	.include "global.inc"

	.public MapObject_StartJumpByOffsetMovement
	.public MapObjectMovementCmd108_Step1
	.public _020632FC
	.public _0206330A
	.public _02063336
	.public _02063344
	.public _0206336A
	.public _02063378
	.public _020633A2
	.public _020633B2
	.public _02063416
	.public _0206341C
	.public _02063420
	.public _02063448
	.public _02063456
	.public _020634CC
	.public _020634D0
	.public _02063550
	.public _020635AA
	.public _020635B8
	.public _020635E0
	.public _020635EE
	.public _02063616
	.public _02063624
	.public _0206366E
	.public _0206367C
	.public _02063680
	.public gLookSidewaysAxisByFacing
	.public gLookSidewaysDirections
	.public gTypeBehaviorSpinOrders
	.public gMapObjectTypeBehaviorInits
	.public gMapObjectTypeBehaviorChecks
	.public gMapObjectTypeBehaviorUpdates
	.public _020FE194
	.public gTrainerApproachStateFuncs

	.text
	.public MapObjectMovementCmd108_Step1
	.rodata

	.public gLookSidewaysAxisByFacing
gLookSidewaysAxisByFacing:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00
	.public gLookSidewaysDirections
gLookSidewaysDirections:
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00
	.public gTypeBehaviorSpinOrders
gTypeBehaviorSpinOrders:
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00
	.public gMapObjectTypeBehaviorInits
gMapObjectTypeBehaviorInits:
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_LookSidewaysInit
	.word TypeBehavior_SpinInit
	.word TypeBehavior_SpinInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.public gMapObjectTypeBehaviorChecks
gMapObjectTypeBehaviorChecks:
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_LookSidewaysCheck
	.word TypeBehavior_SpinCheck
	.word TypeBehavior_SpinCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.public gMapObjectTypeBehaviorUpdates
gMapObjectTypeBehaviorUpdates:
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_LookSidewaysUpdate
	.word TypeBehavior_SpinUpdate
	.word TypeBehavior_SpinUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	; File boundary?
	.public _020FE194
_020FE194:
	.word TrainerSight_GetDistanceNorth
	.word TrainerSight_GetDistanceSouth
	.word TrainerSight_GetDistanceWest
	.word TrainerSight_GetDistanceEast
	.public gTrainerApproachStateFuncs
gTrainerApproachStateFuncs:
	.word TrainerApproach_UnpauseTrainer
	.word TrainerApproach_FaceDirection
	.word TrainerApproach_WaitForPlayer
	.word TrainerApproach_StartFace
	.word TrainerApproach_WaitFace
	.word TrainerApproach_SpawnExclamation
	.word TrainerApproach_WaitExclamation
	.word TrainerApproach_StartReveal
	.word TrainerApproach_WaitReveal
	.word TrainerApproach_Wait
	.word TrainerApproach_CheckDistance
	.word TrainerApproach_StartStep
	.word TrainerApproach_WaitStep
	.word TrainerApproach_WaitArrived
	.word TrainerApproach_TurnPlayer
	.word TrainerApproach_WaitPlayerTurn
	.word TrainerApproach_ResetTrainerMovement
	.word TrainerApproach_Finish
