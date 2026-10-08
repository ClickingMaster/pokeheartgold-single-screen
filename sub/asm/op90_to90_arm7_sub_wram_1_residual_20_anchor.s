	.include "asm/macros.inc"
	.include "global.inc"

	.extern CARD_GetRomHeader
	.extern CARD_IsPulledOut
	.extern PXI_SendWordByFifo
	.extern VENEER_SVC_WaitByLoop
	.extern nextCount$3668
	.extern skipCheck$3668

.text
