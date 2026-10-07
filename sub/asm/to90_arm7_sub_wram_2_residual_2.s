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

.text

	arm_func_start TP_ExecuteProcess
TP_ExecuteProcess: ; 0x038010E4
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0xc
	mov sl, r0
	ldr r1, [sl, #4]
	cmp r1, #2
	bhi _03801118
	cmp r1, #0
	beq _03801130
	cmp r1, #1
	beq _038012A4
	cmp r1, #2
	beq _03801368
	b _038013B4
_03801118:
	cmp r1, #0x10
	bne _038013B4
	ldr r0, _038013C0 ; =tpw
	ldr r0, [r0, #0x20]
	cmp r0, #2
	bne _038013B4
_03801130:
	bl OS_DisableInterrupts
	mov r4, r0
	mov r0, #0
	bl SPIi_CheckException
	cmp r0, #0
	bne _03801168
	mov r0, r4
	bl OS_RestoreInterrupts
	ldr r0, [sl, #4]
	mov r1, #4
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl SPIi_ReturnResult
	b _038013B4
_03801168:
	mov r0, #0
	bl SPIi_GetException
	mov r0, r4
	bl OS_RestoreInterrupts
	ldr r1, _038013C0 ; =tpw
	add r0, sp, #8
	ldr r1, [r1, #0x24]
	add r2, sp, #4
	bl TP_ExecSampling
	ldr r0, [sp, #8]
	ldrh r2, [sp, #4]
	mov r1, r0, lsl #7
	movs r1, r1, lsr #0x1f
	ldreq r0, _038013C4 ; =valid_cnt$3731
	moveq r1, #0
	streqb r1, [r0, #1]
	streqb r1, [r0]
	beq _03801250
	mov r0, r0, lsl #5
	movs r0, r0, lsr #0x1e
	beq _038011FC
	ldr r1, _038013C4 ; =valid_cnt$3731
	mov r3, #0
	strb r3, [r1]
	ldrb r0, [r1, #1]
	add r2, r0, #1
	and r0, r2, #0xff
	strb r2, [r1, #1]
	cmp r0, #4
	blo _03801250
	ldr r0, _038013C0 ; =tpw
	strb r3, [r1, #1]
	ldr r1, [r0, #0x24]
	cmp r1, #0x23
	addlt r1, r1, #1
	strlt r1, [r0, #0x24]
	b _03801250
_038011FC:
	ldr r0, _038013C0 ; =tpw
	ldr r1, _038013C4 ; =valid_cnt$3731
	ldr r4, [r0, #0x24]
	mov r5, #0
	strb r5, [r1, #1]
	cmp r2, r4, asr #1
	strgeb r5, [r1]
	bge _03801250
	ldrb r2, [r1]
	add r3, r2, #1
	and r2, r3, #0xff
	strb r3, [r1]
	cmp r2, #4
	blo _03801250
	strb r5, [r1]
	ldr r2, [r0, #0x28]
	cmp r4, r2
	subgt r3, r4, #1
	movgt r2, #3
	strgt r3, [r0, #0x24]
	strgtb r2, [r1, #1]
_03801250:
	ldrh r0, [sp, #8]
	ldr r1, _038013C8 ; =0x027FFFAA
	strh r0, [r1]
	ldrh r0, [sp, #0xa]
	strh r0, [r1, #2]
	ldr r0, [sl, #4]
	cmp r0, #0
	bne _03801284
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #0
	bl SPIi_ReturnResult
	b _03801298
_03801284:
	ldr r1, [sl, #8]
	mov r0, r0, lsl #0x10
	and r1, r1, #0xff
	mov r0, r0, lsr #0x10
	bl SPIi_ReturnResult
_03801298:
	mov r0, #0
	bl SPIi_ReleaseException
	b _038013B4
_038012A4:
	ldr r6, _038013C0 ; =tpw
	ldr r0, [r6, #0x20]
	cmp r0, #1
	bne _03801354
	ldr r8, _038013CC ; =0x00000107
	ldr r5, _038013D0 ; =tpw + 0x2C
	mov sb, #0
	mov r7, #0xd7
	mov fp, #0xa
	mov r4, #0x28
	b _03801324
_038012D0:
	mul r0, sb, r8
	bl _u32_div_f
	ldr r2, [sl, #0xc]
	mov r1, r8
	add r0, r2, r0
	bl _u32_div_f
	mov r0, r1, lsl #0x10
	mov r0, r0, lsr #0x10
	cmp r0, #0xc8
	blo _03801300
	cmp r0, #0xd7
	movlo r0, r7
_03801300:
	add r1, r6, sb, lsl #1
	strh r0, [r1, #0xcc]
	str sb, [sp]
	mla r0, sb, r4, r5
	ldrsh r1, [r1, #0xcc]
	ldr r3, _038013D4 ; =TpVAlarmHandler
	mov r2, fp
	bl OS_SetPeriodicVAlarm
	add sb, sb, #1
_03801324:
	ldr r1, [sl, #8]
	cmp sb, r1
	blo _038012D0
	ldr r0, [sl, #4]
	mov r1, #0
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl SPIi_ReturnResult
	ldr r0, _038013C0 ; =tpw
	mov r1, #2
	str r1, [r0, #0x20]
	b _038013B4
_03801354:
	mov r0, r1, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #3
	bl SPIi_ReturnResult
	b _038013B4
_03801368:
	ldr r0, _038013C0 ; =tpw
	ldr r0, [r0, #0x20]
	cmp r0, #3
	bne _038013A4
	ldr r0, _038013D8 ; =0x54505641
	bl OS_CancelVAlarms
	ldr r0, [sl, #4]
	mov r1, #0
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl SPIi_ReturnResult
	ldr r0, _038013C0 ; =tpw
	mov r1, #0
	str r1, [r0, #0x20]
	b _038013B4
_038013A4:
	mov r0, r1, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #3
	bl SPIi_ReturnResult
_038013B4:
	add sp, sp, #0xc
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_038013C0: .word tpw
_038013C4: .word valid_cnt$3731
_038013C8: .word 0x027FFFAA
_038013CC: .word 0x00000107
_038013D0: .word tpw + 0x2C
_038013D4: .word TpVAlarmHandler
_038013D8: .word 0x54505641
	arm_func_end TP_ExecuteProcess

	arm_func_start TpVAlarmHandler
TpVAlarmHandler: ; 0x038013DC
	stmdb sp!, {r3, r4, lr}
	sub sp, sp, #4
	mov r4, r0
	mov r3, r4
	mov r0, #0
	mov r1, #0x10
	mov r2, #1
	bl SPIi_SetEntry
	cmp r0, #0
	bne _03801434
	ldr r0, [sp]
	ldr r3, _03801440 ; =0x027FFFAA
	bic r0, r0, #0x6000000
	orr r0, r0, #0x6000000
	str r0, [sp]
	ldrh r0, [sp]
	ldrh r2, [sp, #2]
	strh r0, [r3]
	and r1, r4, #0xff
	mov r0, #0x10
	strh r2, [r3, #2]
	bl SPIi_ReturnResult
_03801434:
	add sp, sp, #4
	ldmia sp!, {r3, r4, lr}
	bx lr
	.align 2, 0
_03801440: .word 0x027FFFAA
	arm_func_end TpVAlarmHandler
