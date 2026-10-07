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

	arm_func_start __VENEER_OS_InitMessageQueue
__VENEER_OS_InitMessageQueue: ; 0x060001CC
	ldr pc, _060001D0 ; =OS_InitMessageQueue
	.align 2, 0
_060001D0: .word OS_InitMessageQueue
	arm_func_end __VENEER_OS_InitMessageQueue

	arm_func_start __VENEER_OS_InitMutex
__VENEER_OS_InitMutex: ; 0x060001D4
	ldr pc, _060001D8 ; =OS_InitMutex
	.align 2, 0
_060001D8: .word OS_InitMutex
	arm_func_end __VENEER_OS_InitMutex

	arm_func_start __VENEER_OS_CreateThread
__VENEER_OS_CreateThread: ; 0x060001DC
	ldr pc, _060001E0 ; =OS_CreateThread
	.align 2, 0
_060001E0: .word OS_CreateThread
	arm_func_end __VENEER_OS_CreateThread

	arm_func_start __VENEER_OS_WakeupThreadDirect
__VENEER_OS_WakeupThreadDirect: ; 0x060001E4
	ldr pc, _060001E8 ; =OS_WakeupThreadDirect
	.align 2, 0
_060001E8: .word OS_WakeupThreadDirect
	arm_func_end __VENEER_OS_WakeupThreadDirect

	arm_func_start __VENEER_OS_IsVAlarmAvailable
__VENEER_OS_IsVAlarmAvailable: ; 0x060001EC
	ldr pc, _060001F0 ; =OS_IsVAlarmAvailable
	.align 2, 0
_060001F0: .word OS_IsVAlarmAvailable
	arm_func_end __VENEER_OS_IsVAlarmAvailable

	arm_func_start __VENEER_OS_InitVAlarm
__VENEER_OS_InitVAlarm: ; 0x060001F4
	ldr pc, _060001F8 ; =OS_InitVAlarm
	.align 2, 0
_060001F8: .word OS_InitVAlarm
	arm_func_end __VENEER_OS_InitVAlarm

	arm_func_start __VENEER_PXI_Init
__VENEER_PXI_Init: ; 0x060001FC
	ldr pc, _06000200 ; =PXI_Init
	.align 2, 0
_06000200: .word PXI_Init
	arm_func_end __VENEER_PXI_Init

	arm_func_start __VENEER_PXI_SetFifoRecvCallback
__VENEER_PXI_SetFifoRecvCallback: ; 0x06000204
	ldr pc, _06000208 ; =PXI_SetFifoRecvCallback
	.align 2, 0
_06000208: .word PXI_SetFifoRecvCallback
	arm_func_end __VENEER_PXI_SetFifoRecvCallback
