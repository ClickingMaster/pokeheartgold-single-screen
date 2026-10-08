	.include "asm/macros.inc"
	.include "global.inc"

	.extern CARD_InitPulledOutCallback
	.extern CARDi_InitCommon
	.extern CARDi_ReadCard
	.extern OS_WakeupThreadDirect
	.extern cardi_common
	.extern cardi_rom_base
	.extern rom_stat

.text
