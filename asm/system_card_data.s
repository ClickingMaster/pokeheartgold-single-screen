	.include "asm/macros.inc"
	.include "system_card_data.inc"
	.include "global.inc"

	.rodata
	.balign 4, 0
	.public sRomArchiveName
sRomArchiveName:
	.asciz "rom"
