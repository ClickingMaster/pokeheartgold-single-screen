	.include "asm/macros.inc"
	.include "global.inc"
	.extern MI_CpuFill8
	.extern MIi_CpuCopy16
	.extern OS_CancelAlarm
	.extern OS_ExitThread
	.extern OS_GetTick
	.extern OS_SetAlarm
	.extern WMSP_AddRssiToList
	.extern WMSP_CancelVAlarm
	.extern WMSP_CheckMacAddress
	.extern WMSP_CleanSendQueue
	.extern WMSP_FlushSendQueue
	.extern WMSP_GetAverageLinkLevel
	.extern WMSP_GetInternalRequestBuf
	.extern WMSP_GetRssi8_06001B5C
	.extern WMSP_ParsePortPacket
	.extern WMSP_RequestResumeMP
	.extern WMSP_ResetSizeVars
	.extern WMSP_SetChildSize
	.extern WMSP_SetThreadPriorityLow
	.extern WmspFreeBufOfWL
	.extern WmspKickMPChild
	.extern WmspKickMPParent
	.extern WmspMPChildIntervalAlarmCallback
	.extern WmspMPParentIntervalAlarmCallback
	.extern WmspMaMultiPollAckAlarmCallback
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_ReceiveMessage
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_OS_SendMessage
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspMPAckAlarm
	.extern wmspMPIntervalAlarm
	.extern wmspW
	.public __VENEER_MI_CpuFill8
	.public __VENEER_MIi_CpuCopy16
	.public __VENEER_OS_CancelAlarm
	.public __VENEER_OS_ExitThread
	.public __VENEER_OS_GetTick
	.public __VENEER_OS_SetAlarm

.text
	arm_func_start __VENEER_OS_ExitThread
__VENEER_OS_ExitThread: ; 0x06001AEC
	ldr pc, _06001AF0 ; =OS_ExitThread
	.align 2, 0
_06001AF0: .word OS_ExitThread
	arm_func_end __VENEER_OS_ExitThread

	arm_func_start __VENEER_MI_CpuFill8
__VENEER_MI_CpuFill8: ; 0x06001AF4
	ldr pc, _06001AF8 ; =MI_CpuFill8
	.align 2, 0
_06001AF8: .word MI_CpuFill8
	arm_func_end __VENEER_MI_CpuFill8

	arm_func_start __VENEER_OS_GetTick
__VENEER_OS_GetTick: ; 0x06001AFC
	ldr pc, _06001B00 ; =OS_GetTick
	.align 2, 0
_06001B00: .word OS_GetTick
	arm_func_end __VENEER_OS_GetTick

	arm_func_start __VENEER_MIi_CpuCopy16
__VENEER_MIi_CpuCopy16: ; 0x06001B04
	ldr pc, _06001B08 ; =MIi_CpuCopy16
	.align 2, 0
_06001B08: .word MIi_CpuCopy16
	arm_func_end __VENEER_MIi_CpuCopy16

	arm_func_start __VENEER_OS_CancelAlarm
__VENEER_OS_CancelAlarm: ; 0x06001B0C
	ldr pc, _06001B10 ; =OS_CancelAlarm
	.align 2, 0
_06001B10: .word OS_CancelAlarm
	arm_func_end __VENEER_OS_CancelAlarm

	arm_func_start __VENEER_OS_SetAlarm
__VENEER_OS_SetAlarm: ; 0x06001B14
	ldr pc, _06001B18 ; =OS_SetAlarm
	.align 2, 0
_06001B18: .word OS_SetAlarm
	arm_func_end __VENEER_OS_SetAlarm
