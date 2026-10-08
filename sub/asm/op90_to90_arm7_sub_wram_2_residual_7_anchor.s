	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIC_DisableMultipleInterrupt
	.extern MIC_EnableMultipleInterrupt
	.extern MIC_ExecSampling12
	.extern MIC_ExecSampling8
	.extern MIC_SetIrqFunction
	.extern MIC_TimerHandler
	.extern OS_DisableInterrupts
	.extern OS_EnableIrqMask
	.extern OS_RestoreInterrupts
	.extern SPIi_CheckException
	.extern SPIi_GetException
	.extern SPIi_ReleaseException
	.extern SPIi_ReturnResult
	.extern micw

.text
