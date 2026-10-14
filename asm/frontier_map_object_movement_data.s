	.include "asm/macros.inc"
	.include "frontier_map_object_movement_data.inc"
	.include "global.inc"
	.public FrontierMapMovement_LookAround
	.public gFrontierMapMovementFuncs


    .text
	.public FrontierMap_SetObjectMovement
	.public FrontierMap_RunObjectMovement
	.public FrontierMap_RunObjectMovements


    .rodata

gFrontierMapMovementFuncs: ; 0x0223DB24
	.byte 0x00, 0x00, 0x00, 0x00
	.word FrontierMapMovement_LookAround
