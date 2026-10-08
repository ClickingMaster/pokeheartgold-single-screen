	.include "asm/macros.inc"
	.include "global.inc"

	.extern CTRDG_IsExisting
	.extern CTRDG_IsPulledOut
	.extern OS_Sleep
	.extern PXI_SendWordByFifo
	.extern isFirstCheck$3769
	.extern lock_id

.text
