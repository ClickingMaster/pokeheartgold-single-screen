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

.text
	arm_func_start PMi_DoSleep
PMi_DoSleep: ; 0x03801FBC
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, lr}
	ldr r0, _03802178 ; =0x04000208
	mov r8, #0
	ldrh sb, [r0]
	strh r8, [r0]
	bl OS_DisableInterrupts
	mov r4, r0
	mvn r0, #0xfe000000
	bl OS_DisableIrqMask
	mov r5, r0
	mov r0, r8
	bl PMi_GetRegister
	mov r6, r0
	mov r0, #2
	bl PM_SetLEDPattern
	mov r0, #2
	bl PMi_SetLED
	mov r0, #2
	bl PMi_SetLED
	bl SND_BeginSleep
	mov r0, #1
	bl PMi_ResetControl
	ldr r0, _0380217C ; =PMi_TriggerBL
	ldrh r0, [r0]
	tst r0, #1
	beq _03802040
	ldr r0, _03802180 ; =PMi_KeyPattern
	ldr r1, _03802184 ; =0x04000132
	ldrh r2, [r0]
	mov r0, #0x1000
	orr r2, r2, #0x4000
	strh r2, [r1]
	bl OS_EnableIrqMask
_03802040:
	ldr r0, _0380217C ; =PMi_TriggerBL
	ldrh r0, [r0]
	tst r0, #4
	beq _03802058
	mov r0, #0x400000
	bl OS_EnableIrqMask
_03802058:
	ldr r0, _0380217C ; =PMi_TriggerBL
	ldrh r0, [r0]
	tst r0, #2
	beq _0380209C
	ldr r1, _03802188 ; =0x04000134
	mov r0, #0x8000
	ldrh r7, [r1]
	mov r8, #1
	bl EXIi_SelectRcnt
	mov r0, #0x40
	mov r1, #0
	bl EXIi_SetBitRcnt0L
	mov r0, #0x100
	mov r1, r0
	bl EXIi_SetBitRcnt0L
	mov r0, #0x80
	bl OS_EnableIrqMask
_0380209C:
	ldr r0, _0380217C ; =PMi_TriggerBL
	ldrh r0, [r0]
	tst r0, #8
	beq _038020B4
	mov r0, #0x100000
	bl OS_EnableIrqMask
_038020B4:
	ldr r0, _0380217C ; =PMi_TriggerBL
	ldrh r0, [r0]
	tst r0, #0x10
	beq _038020CC
	mov r0, #0x2000
	bl OS_EnableIrqMask
_038020CC:
	mov r0, r4
	bl OS_RestoreInterrupts
	ldr r2, _03802178 ; =0x04000208
	mov r0, #1
	ldrh r1, [r2]
	strh r0, [r2]
	bl __VENEER_SVC_Sleep
	mov r1, r6
	mov r0, #0
	bl PMi_SetRegister
	ldr r0, _0380217C ; =PMi_TriggerBL
	ldrh r1, [r0]
	tst r1, #0x20
	movne r0, #6
	moveq r0, #7
	tst r1, #0x40
	movne r6, #4
	moveq r6, #5
	bl PMi_SwitchUtilityProc
	mov r0, r6
	bl PMi_SwitchUtilityProc
	cmp r8, #0
	ldrne r0, _03802188 ; =0x04000134
	strneh r7, [r0]
	mov r0, #1
	bl PMi_SetControl
	bl SND_EndSleep
	mov r1, #0
	ldr r3, _0380218C ; =PMi_Work
	mov r2, r1
	mov r0, #0x62
	str r1, [r3, #0x20]
	bl PMi_SendPxiCommand
	bl OS_DisableInterrupts
	mov r0, r5
	bl OS_SetIrqMask
	mov r0, r4
	bl OS_RestoreInterrupts
	ldr r1, _03802178 ; =0x04000208
	ldrh r0, [r1]
	strh sb, [r1]
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, lr}
	bx lr
	.align 2, 0
_03802178: .word 0x04000208
_0380217C: .word PMi_TriggerBL
_03802180: .word PMi_KeyPattern
_03802184: .word 0x04000132
_03802188: .word 0x04000134
_0380218C: .word PMi_Work
	arm_func_end PMi_DoSleep

	arm_func_start __VENEER_SVC_Sleep
__VENEER_SVC_Sleep: ; 0x03802190
	ldr ip, _03802198 ; =SVC_Sleep
	bx ip
	.align 2, 0
_03802198: .word SVC_Sleep
	arm_func_end __VENEER_SVC_Sleep
