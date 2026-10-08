	.include "asm/macros.inc"
	.include "global.inc"
	.extern WMSP_CleanSendQueue
	.extern WMSP_InitSendQueue
	.extern WMSP_PutSendQueue
	.extern WMSP_SetMPParameterCore
	.extern WMSP_SetThreadPriorityHigh
	.extern WMSP_SetVAlarm
	.extern WMSP_WL_ParamSetNullKeyResponseMode
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_GetTick
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text
	arm_func_start WMSP_StartMP
WMSP_StartMP: ; 0x06004D3C
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x204
	ldr r1, _060050C4 ; =wmspW + 0x1000
	mov sl, r0
	ldr r8, [r1, #0x550]
	ldr fp, [sl, #4]
	ldrh r1, [r8, #0x9c]
	ldr r4, [sl, #8]
	ldr r5, [sl, #0xc]
	ldr r6, [sl, #0x10]
	ldr r0, _060050C8 ; =wmspW
	cmp r1, #0
	mov r7, #0
	bne _06004DC0
	ldrh r1, [r8, #0x3c]
	add r1, r1, #0x1f
	bic r1, r1, #0x1f
	cmp r6, r1
	add r1, r8, #0x100
	ldrh r1, [r1, #0x88]
	movlo r7, #6
	cmp r1, #0
	ldrneh r1, [r8, #0x3e]
	addne r1, r1, #0x51
	bne _06004DB4
	ldrh r2, [r8, #0x3e]
	ldrh r1, [r8, #0xf8]
	add r2, r2, #0xc
	mul r1, r2, r1
	add r1, r1, #0x29
_06004DB4:
	bic r1, r1, #0x1f
	cmp r4, r1
	movlo r7, #6
_06004DC0:
	ldrh r1, [r8, #0xe6]
	cmp r1, #2
	bne _06004DF0
	add r0, r0, #0x1000
	ldr r1, [r0, #0x54c]
	add r0, r8, #0x100
	ldrh r1, [r1, #0x46]
	ldrh r3, [r0, #0xf6]
	mov r2, #1
	mov r0, r2, lsl r1
	tst r3, r0, asr #1
	moveq r7, #6
_06004DF0:
	cmp r7, #0
	beq _06004E18
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xe
	strh r1, [r0]
	strh r7, [r0, #2]
	mov r1, #0xa
	strh r1, [r0, #4]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060050B8
_06004E18:
	ldr r0, [r8, #0xc]
	mov r1, #0
	cmp r0, #0
	strne r1, [r8, #0xc]
	movne r1, #1
	cmp r1, #0
	beq _06004E3C
	ldr r0, _060050CC ; =0x0000FFFF
	bl WMSP_CleanSendQueue
_06004E3C:
	bl WMSP_InitSendQueue
	bl __VENEER_OS_DisableInterrupts
	str r0, [sp]
	add r0, sl, #0x14
	mov r1, #0
	bl WMSP_SetMPParameterCore
	ldr r0, _060050C4 ; =wmspW + 0x1000
	ldr r7, [sl, #0x30]
	ldr sb, [r0, #0x550]
	ldrh r0, [sb]
	cmp r0, #9
	cmpne r0, #0xa
	beq _06004F30
	bl __VENEER_OS_DisableInterrupts
	tst r7, #4
	ldrneh r2, [sl, #0x38]
	add r1, sb, #0x700
	ldreqh r2, [sb, #0x5c]
	cmp r2, #0
	moveq r2, #0x10
	tst r7, #1
	ldrneh r3, [sl, #0x34]
	ldreqh r3, [sb, #0x58]
	cmp r3, #0
	moveq r3, #0x10
	cmp r3, r2
	movhi r3, r2
	tst r7, #2
	ldrneh ip, [sl, #0x36]
	ldreqh ip, [sb, #0x5a]
	strh r2, [r1, #0xc4]
	cmp ip, #0
	moveq ip, #0x10
	cmp ip, r2
	strh r3, [r1, #0xc0]
	movhi ip, r2
	strh ip, [r1, #0xc2]
	ldrsh r1, [sb, #0x62]
	cmp r1, r2
	strgth r2, [sb, #0x62]
	tst r7, #0x200
	ldrneh r2, [sl, #0x3a]
	add r1, sb, #0x700
	ldreqh r2, [sb, #0x98]
	tst r7, #0x400
	strh r2, [r1, #0xca]
	ldrneb r2, [sl, #0x3c]
	add r1, sb, #0x700
	ldreqh r2, [sb, #0x92]
	tst r7, #0x800
	strh r2, [r1, #0xc6]
	ldrneb r2, [sl, #0x3d]
	add r1, sb, #0x700
	ldreqh r2, [sb, #0x94]
	tst r7, #0x1000
	strh r2, [r1, #0xc8]
	ldrneb r2, [sl, #0x3e]
	add r1, sb, #0x700
	ldreqh r2, [sb, #0x9a]
	strh r2, [r1, #0xcc]
	bl __VENEER_OS_RestoreInterrupts
_06004F30:
	ldrh r0, [r8]
	add r0, r0, #0xf9
	add r0, r0, #0xff00
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	cmp r0, #1
	bhi _06005090
	mov r2, #0
	strh r2, [r8, #0x84]
	strh r2, [r8, #0x5e]
	mov r1, #1
	strh r1, [r8, #0x60]
	strh r2, [r8, #0x88]
	strh r2, [r8, #0x9e]
	mov r0, #0x3c
	strh r0, [r8, #0xa0]
	str r2, [r8, #0x734]
	strh r2, [r8, #0x8a]
	strh r2, [r8, #0x8c]
	strh r2, [r8, #0x8e]
	strh r2, [r8, #0x90]
	strh r2, [r8, #0x66]
	str fp, [r8, #0x74]
	strh r4, [r8, #0x72]
	add r0, fp, r4
	str r0, [r8, #0x78]
	strh r2, [r8, #0x70]
	str r5, [r8, #0x7c]
	strh r6, [r8, #0x80]
	strh r2, [r8, #0x62]
	strh r2, [r8, #0x64]
	strh r2, [r8, #0x68]
	strh r2, [r8, #0x6a]
	rsb r0, r1, #0x10000
	strh r0, [r8, #0xbe]
	strh r1, [r8, #0xc0]
	bl __VENEER_OS_GetTick
	orr r1, r1, #0
	orr r2, r0, #1
	mov r3, #0
_06004FD0:
	add r0, r8, r3, lsl #3
	str r2, [r0, #0x738]
	add r3, r3, #1
	str r1, [r0, #0x73c]
	cmp r3, #0x10
	blt _06004FD0
	bl WMSP_SetThreadPriorityHigh
	mov r0, #0
	strh r0, [r8, #0xce]
	bl WMSP_SetVAlarm
	ldrh r0, [r8]
	cmp r0, #8
	moveq r0, #0xa
	streqh r0, [r8]
	beq _06005018
	cmp r0, #7
	moveq r0, #9
	streqh r0, [r8]
_06005018:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xe
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0xa
	strh r1, [r0, #4]
	bl __VENEER_WMSP_ReturnResult2Wm9
	mov r1, #1
	ldr r0, [sp]
	str r1, [r8, #0xc]
	bl __VENEER_OS_RestoreInterrupts
	add r0, sp, #4
	mov r1, #1
	bl WMSP_WL_ParamSetNullKeyResponseMode
	mov r4, r0
	ldrh r0, [r4, #4]
	cmp r0, #0
	beq _060050B8
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r2, #0xe
	strh r2, [r0]
	mov r1, #1
	strh r1, [r0, #2]
	add r1, r2, #0x208
	strh r1, [r0, #4]
	ldrh r1, [r4, #4]
	strh r1, [r0, #6]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060050B8
_06005090:
	ldr r0, [sp]
	bl __VENEER_OS_RestoreInterrupts
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xe
	strh r1, [r0]
	mov r1, #3
	strh r1, [r0, #2]
	mov r1, #0xa
	strh r1, [r0, #4]
	bl __VENEER_WMSP_ReturnResult2Wm9
_060050B8:
	add sp, sp, #0x204
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_060050C4: .word wmspW + 0x1000
_060050C8: .word wmspW
_060050CC: .word 0x0000FFFF
	arm_func_end WMSP_StartMP
