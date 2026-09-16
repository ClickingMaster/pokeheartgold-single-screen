	.include "asm/macros.inc"
	.include "overlay_80_02239BF0.inc"
	.include "global.inc"
	.public ov80_02239C80
	.public ov80_0223DB24


    .text
	.public ov80_02239BF0
	.public ov80_02239C28
	.public ov80_02239C54


    .rodata

ov80_0223DB24: ; 0x0223DB24
	.byte 0x00, 0x00, 0x00, 0x00
	.word ov80_02239C80
