	.include "asm/macros.inc"
	.include "global.inc"

	.extern MicSetTimerValue
	.extern OS_DisableInterrupts
	.extern OS_RestoreInterrupts
	.extern SPIi_ReturnResult
	.extern SPIi_SetEntry
	.extern micw

.text
