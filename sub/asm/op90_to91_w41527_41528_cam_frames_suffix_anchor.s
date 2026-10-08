	.include "asm/macros.inc"
	.include "global.inc"

	.extern CAM_ClrTIMElementBitmap
	.extern CAM_GetAID
	.extern CAM_ReleaseAID
	.extern CAM_SetDoze
	.extern CAM_SetTIMElementBitmap
	.extern __VENEER_OS_DisableIrqMask
	.extern __VENEER_OS_EnableIrqMask

.text
