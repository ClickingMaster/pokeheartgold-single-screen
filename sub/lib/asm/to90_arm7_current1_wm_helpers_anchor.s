	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_DisableInterrupts
	.extern OS_DisableScheduler
	.extern OS_EnableScheduler
	.extern OS_RestoreInterrupts
	.extern OS_SetThreadPriority
	.extern WL_GetThreadStruct
	.extern wmspIndicateThread
	.extern wmspRequestThread
	.extern wmspW


.text
