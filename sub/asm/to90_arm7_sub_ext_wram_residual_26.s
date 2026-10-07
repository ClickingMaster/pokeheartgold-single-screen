	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern OS_IsThreadTerminated
	.extern OS_JoinThread
	.extern pTaskFunc

.text

	arm_func_start __VENEER_OS_IsThreadTerminated
__VENEER_OS_IsThreadTerminated: ; 0x06008DA0
	ldr pc, _06008DA4 ; =OS_IsThreadTerminated
	.align 2, 0
_06008DA4: .word OS_IsThreadTerminated
	arm_func_end __VENEER_OS_IsThreadTerminated

	arm_func_start __VENEER_OS_JoinThread
__VENEER_OS_JoinThread: ; 0x06008DA8
	ldr pc, _06008DAC ; =OS_JoinThread
	.align 2, 0
_06008DAC: .word OS_JoinThread
	arm_func_end __VENEER_OS_JoinThread

	arm_func_start InitializeTask
InitializeTask: ; 0x06008DB0
	stmdb sp!, {r4, lr}
	ldr r0, _06008E30 ; =0x0380FFF4
	mov r3, #0
	ldr lr, [r0]
	ldr r2, _06008E34 ; =0x0000FFFF
	strh r3, [lr, #0x10]
	strh r3, [lr, #0x12]
_06008DCC:
	add r0, lr, r3, lsl #1
	strh r2, [r0, #8]
	ldrh r0, [r0, #8]
	mov r1, r3, lsl #1
	add r3, r3, #1
	strh r0, [lr, r1]
	cmp r3, #4
	blo _06008DCC
	ldr ip, _06008E34 ; =0x0000FFFF
	mov r4, #0
	ldr r1, _06008E38 ; =pTaskFunc
	mov r2, r4
_06008DFC:
	add r3, lr, r4, lsl #3
	strh ip, [r3, #0xbc]
	ldr r0, [r1, r4, lsl #2]
	strh r2, [r3, #0xbe]
	add r4, r4, #1
	str r0, [r3, #0xc0]
	cmp r4, #0x18
	blo _06008DFC
	mov r0, #3
	mov r1, #0xc
	bl AddTask
	ldmia sp!, {r4, lr}
	bx lr
	.align 2, 0
_06008E30: .word 0x0380FFF4
_06008E34: .word 0x0000FFFF
_06008E38: .word pTaskFunc
	arm_func_end InitializeTask
