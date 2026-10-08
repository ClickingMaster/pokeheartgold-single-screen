	.include "asm/macros.inc"
	.include "global.inc"
	.extern EXIi_SelectRcnt
	.extern EXIi_SetBitRcnt0L
	.extern OS_DisableInterrupts
	.extern OS_DisableIrqMask
	.extern OS_EnableIrqMask
	.extern OS_RestoreInterrupts
	.extern OS_SetIrqMask
	.extern PM_SetLEDPattern
	.extern PMi_BlinkCounter
	.extern PMi_BlinkPatternData
	.extern PMi_GetRegister
	.extern PMi_KeyPattern
	.extern PMi_LEDStatus
	.extern PMi_ResetControl
	.extern PMi_SendPxiCommand
	.extern PMi_SetControl
	.extern PMi_SetLED
	.extern PMi_SetRegister
	.extern PMi_SwitchUtilityProc
	.extern PMi_TriggerBL
	.extern PMi_Work
	.extern SND_BeginSleep
	.extern SND_EndSleep
	.extern SPIi_SetEntry
	.extern SVC_Sleep
	.extern _u32_div_f
	.public __VENEER_SVC_Sleep

.text
	arm_func_start __VENEER_SVC_Sleep
__VENEER_SVC_Sleep: ; 0x03802190
	ldr ip, _03802198 ; =SVC_Sleep
	bx ip
	.align 2, 0
_03802198: .word SVC_Sleep
	arm_func_end __VENEER_SVC_Sleep
