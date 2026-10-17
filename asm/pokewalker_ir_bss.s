#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"

	.public gPokewalkerIrLink
	.public gPokewalkerIrPacket
	.public gPokewalkerIrPayload

	.bss

gPokewalkerIrLink:
	.space 0x38

gPokewalkerIrPacket: ; 0x021FFA18
	.space 0x8

gPokewalkerIrPayload: ; 0x021FFA20
	.space 0x84
