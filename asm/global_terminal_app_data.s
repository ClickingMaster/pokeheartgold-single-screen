	.include "asm/macros.inc"
	.include "unk_020192D0.inc"
	.include "global.inc"

	.rodata

	.public sOverlayTemplate_GlobalTerminalConnect
sOverlayTemplate_GlobalTerminalConnect:
	.word GlobalTerminalConnect_Init, GlobalTerminalConnect_Main, GlobalTerminalConnect_Exit, 0xFFFFFFFF

	.bss

	.public _021D1108
_021D1108:
	.space 0x4

	.text
