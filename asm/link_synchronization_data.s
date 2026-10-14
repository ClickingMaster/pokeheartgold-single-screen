	.include "asm/macros.inc"
	.include "link_synchronization_data.inc"
	.include "global.inc"

	.bss

	.public gLinkSync
gLinkSync:
	.space 0x4

	.text
