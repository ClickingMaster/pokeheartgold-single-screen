	.include "asm/macros.inc"
	.include "field_transition_effects_data.inc"
	.include "global.inc"

	.rodata

.public sMapEnterRoutines
.public sMapExitRoutines
.public sMapEnterPlacementRoutines;

.public FieldTransition_EnterWithDirectionalFade
.public FieldTransition_EnterWithFade
.public FieldTransition_EnterThroughSideDoor
.public FieldTransition_EnterFromWarpStairs
.public FieldTransition_EnterWithDoorExit
.public FieldTransition_ExitWithFade
.public FieldTransition_ExitThroughDoor
.public FieldTransition_ExitThroughSideDoor
.public FieldTransition_ExitOnWarpStairs
.public FieldTransition_ExitSinking
.public FieldTransition_ExitStairsFadeToWhite
.public FieldTransition_ExitStairsFadeOut
.public FieldTransition_ExitRising

sMapEnterRoutines:
	.word FieldTransition_EnterWithDirectionalFade
	.word FieldTransition_EnterWithFade
	.word FieldTransition_EnterThroughSideDoor
	.word FieldTransition_EnterFromWarpStairs
	.word FieldTransition_EnterWithFade
	.word FieldTransition_EnterWithDoorExit
	.word FieldTransition_EnterWithFade
	.word FieldTransition_EnterRising
	.word FieldTransition_EnterSinking
sMapExitRoutines:
	.word FieldTransition_ExitWithFade
	.word FieldTransition_ExitThroughDoor
	.word FieldTransition_ExitThroughSideDoor
	.word FieldTransition_ExitOnWarpStairs
	.word FieldTransition_ExitStairsFadeOut
	.word FieldTransition_ExitStairsFadeToWhite
	.word FieldTransition_ExitWithFade
	.word FieldTransition_ExitRising
	.word FieldTransition_ExitSinking
sMapEnterPlacementRoutines:
	.word 0
	.word 0
	.word FieldTransition_PlaceInsideSideDoor
	.word FieldTransition_PlaceOffWarpStairs
	.word 0
	.word 0
	.word 0
	.word FieldTransition_PlaceBelowFloor
	.word FieldTransition_PlaceAboveFloor

	.text
