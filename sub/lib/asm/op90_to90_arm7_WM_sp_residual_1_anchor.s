	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_CreateThread
	.extern OS_InitMessageQueue
	.extern OS_InitMutex
	.extern OS_InitVAlarm
	.extern OS_IsVAlarmAvailable
	.extern OS_WakeupThreadDirect
	.extern PXI_Init
	.extern PXI_SetFifoRecvCallback

.text
