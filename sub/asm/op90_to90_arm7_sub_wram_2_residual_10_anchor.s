	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_IRQTable
	.extern OS_IrqHandler_ThreadSwitch
	.extern OSi_ThreadInfo
	.extern micIntrInfo
	.extern micIntrPrio

.text
