	.include "asm/macros.inc"
	.include "global.inc"
	.extern OS_CancelVAlarms
	.extern OS_DisableInterrupts
	.extern OS_RestoreInterrupts
	.extern OS_SetPeriodicVAlarm
	.extern SPIi_CheckException
	.extern SPIi_GetException
	.extern SPIi_ReleaseException
	.extern SPIi_ReturnResult
	.extern SPIi_SetEntry
	.extern TP_ExecSampling
	.extern _u32_div_f
	.extern tpw
	.extern valid_cnt$3731
	.extern TpVAlarmHandler

.text
