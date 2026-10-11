	.include "asm/macros.inc"
	.include "unk_02055BF0_data.inc"
	.include "global.inc"

	.rodata

.public sMapEnterRoutines
.public sMapExitRoutines
.public _020FC76C;

.public sub_02056530
.public sub_020565FC
.public FieldTransition_EnterThroughSideDoor
.public FieldTransition_EnterFromWarpStairs
.public FieldTransition_EnterWithDoorExit
.public sub_02056004
.public sub_02056040
.public sub_020560C4
.public sub_0205613C
.public sub_02056424
.public sub_02056268
.public sub_02056220
.public sub_020562B0

sMapEnterRoutines:
	.word sub_02056530
	.word sub_020565FC
	.word FieldTransition_EnterThroughSideDoor
	.word FieldTransition_EnterFromWarpStairs
	.word sub_020565FC
	.word FieldTransition_EnterWithDoorExit
	.word sub_020565FC
	.word FieldTransition_EnterRising
	.word FieldTransition_EnterSinking
sMapExitRoutines:
	.word sub_02056004
	.word sub_02056040
	.word sub_020560C4
	.word sub_0205613C
	.word sub_02056220
	.word sub_02056268
	.word sub_02056004
	.word sub_020562B0
	.word sub_02056424
_020FC76C:
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
