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

	arm_func_start PM_SelfBlinkProc
PM_SelfBlinkProc: ; 0x0380219C
	stmdb sp!, {r4, r5, r6, lr}
	ldr r1, _038022B0 ; =PMi_BlinkCounter
	ldr r3, [r1, #4]
	cmp r3, #0
	bne _038021D8
	mov r2, #1
	mov r3, r2
	mov r0, #3
	mov r1, #0x66
	bl SPIi_SetEntry
	cmp r0, #0
	beq _038022A8
	mov r0, #1
	bl PM_SetLEDPattern
	b _038022A8
_038021D8:
	cmp r3, #4
	bge _03802204
	ldr r0, _038022B4 ; =PMi_LEDStatus
	ldr r0, [r0]
	cmp r3, r0
	beq _038022A8
	mov r0, #3
	mov r1, #0x66
	mov r2, #1
	bl SPIi_SetEntry
	b _038022A8
_03802204:
	sub r2, r3, #4
	mov r0, #0xc
	mul r4, r2, r0
	ldr r5, _038022B8 ; =PMi_BlinkPatternData
	ldr r0, [r1]
	add r6, r5, r4
	ldrh r1, [r6, #0xa]
	bl _u32_div_f
	mov r1, #0
	mov r3, r1, lsr r0
	ldr ip, [r6, #4]
	mov r2, #-0x80000000
	rsb r1, r0, #0x20
	orr r3, r3, r2, lsl r1
	sub r1, r0, #0x20
	and ip, ip, r2, lsr r0
	orr r3, r3, r2, lsr r1
	ldr r0, [r5, r4]
	cmp ip, #0
	and r0, r0, r3
	cmpeq r0, #0
	movne r3, #1
	ldrh r2, [r6, #8]
	ldrh r1, [r6, #0xa]
	ldr r0, _038022B0 ; =PMi_BlinkCounter
	mul r1, r2, r1
	ldr r2, [r0]
	moveq r3, #2
	add r2, r2, #1
	cmp r2, r1
	str r2, [r0]
	movhs r1, #0
	strhs r1, [r0]
	ldr r0, _038022B4 ; =PMi_LEDStatus
	ldr r0, [r0]
	cmp r3, r0
	beq _038022A8
	mov r0, #3
	mov r1, #0x66
	mov r2, #1
	bl SPIi_SetEntry
_038022A8:
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_038022B0: .word PMi_BlinkCounter
_038022B4: .word PMi_LEDStatus
_038022B8: .word PMi_BlinkPatternData
	arm_func_end PM_SelfBlinkProc
