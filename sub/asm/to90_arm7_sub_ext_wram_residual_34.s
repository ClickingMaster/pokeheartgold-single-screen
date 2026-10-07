	.include "asm/macros.inc"
	.include "global.inc"

	.extern FLASH_Read
	.extern RF_Write
	.extern macTxRxRegAdrs

.text

	arm_func_start InitRF
InitRF: ; 0x0600B470
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #8
	ldr r0, _0600B5EC ; =0x0380FFF4
	ldr r5, _0600B5F0 ; =macTxRxRegAdrs
	ldr r0, [r0]
	mov r8, #0
	add r0, r0, #0x1f8
	str r8, [sp]
	add r4, r0, #0x400
	add r6, sp, #0
	mov r7, #2
_0600B49C:
	mov r0, r8, lsl #1
	mov r1, r7
	mov r2, r6
	add r0, r0, #0x44
	bl FLASH_Read
	mov r0, r8, lsl #1
	ldrh r0, [r5, r0]
	add r8, r8, #1
	add r0, r0, #0x4800000
	ldr r1, [sp]
	add r0, r0, #0x8000
	strh r1, [r0]
	cmp r8, #0x10
	blo _0600B49C
	ldrh r1, [r4, #2]
	ldr r0, _0600B5F4 ; =0x04808184
	mov r2, r1, lsr #7
	mov r1, r2, lsl #8
	str r1, [sp, #4]
	ldrh r1, [r4, #2]
	mov r7, #0xce
	and r1, r1, #0x7f
	orr r1, r1, r2, lsl #8
	str r1, [sp, #4]
	strh r1, [r0]
	ldrh r0, [r4, #2]
	ldrh r1, [r4]
	and r0, r0, #0x7f
	add r2, r0, #7
	mov r0, r2, asr #2
	add r0, r2, r0, lsr #29
	ldrh r8, [r4, #4]
	cmp r1, #3
	mov r6, r0, asr #3
	bne _0600B58C
	add r0, r8, #0xce
	add r2, r4, #8
	mov r1, #1
	bl FLASH_Read
	mov sb, #0
	add r4, sp, #4
	mov r6, sb
	mov r5, #1
	b _0600B580
_0600B54C:
	str r6, [sp, #4]
	mov r0, r7
	mov r1, r5
	mov r2, r4
	bl FLASH_Read
	mov r0, sb, lsl #8
	ldr r1, [sp, #4]
	add r0, r0, #0x50000
	orr r0, r1, r0
	str r0, [sp, #4]
	bl RF_Write
	add sb, sb, #1
	add r7, r7, #1
_0600B580:
	cmp sb, r8
	blo _0600B54C
	b _0600B5E0
_0600B58C:
	mov r0, #0
	str r0, [sp, #4]
	add r5, sp, #4
	b _0600B5D8
_0600B59C:
	mov r0, r7
	mov r1, r6
	mov r2, r5
	bl FLASH_Read
	ldr r0, [sp, #4]
	bl RF_Write
	ldrh r0, [r4]
	sub r8, r8, #1
	cmp r0, #2
	ldreq r1, [sp, #4]
	add r7, r7, r6
	moveq r0, r1, lsr #0x12
	cmpeq r0, #9
	biceq r0, r1, #0x7c00
	streq r0, [r4, #0xc]
_0600B5D8:
	cmp r8, #0
	bne _0600B59C
_0600B5E0:
	add sp, sp, #8
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, lr}
	bx lr
	.align 2, 0
_0600B5EC: .word 0x0380FFF4
_0600B5F0: .word macTxRxRegAdrs
_0600B5F4: .word 0x04808184
	arm_func_end InitRF
