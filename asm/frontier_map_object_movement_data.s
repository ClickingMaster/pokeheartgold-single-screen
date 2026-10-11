	.include "asm/macros.inc"
	.include "overlay_80_02239BF0.inc"
	.include "global.inc"
	.public FrontierMapMovement_LookAround
	.public ov80_0223DB24


    .text
	.public FrontierMap_SetObjectMovement
	.public FrontierMap_RunObjectMovement
	.public FrontierMap_RunObjectMovements


    .rodata

ov80_0223DB24: ; 0x0223DB24
	.byte 0x00, 0x00, 0x00, 0x00
	.word FrontierMapMovement_LookAround
