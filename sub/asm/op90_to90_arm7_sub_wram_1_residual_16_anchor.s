	.include "asm/macros.inc"
	.include "global.inc"

	.extern CARDi_EnableFlag
	.extern CARDi_OnFifoRecv
	.extern CARDi_TaskThread
	.extern OS_CreateThread
	.extern OS_WakeupThreadDirect
	.extern PXI_SetFifoRecvCallback
	.extern cardi_common
	.extern status_checked$3825

.text
