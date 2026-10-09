#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"

	.public _021FF9E0
	.public ov112_021FFA18
	.public ov112_021FFA20

	.bss

_021FF9E0:
	.space 0x38

ov112_021FFA18: ; 0x021FFA18
	.space 0x8

ov112_021FFA20: ; 0x021FFA20
	.space 0x84
