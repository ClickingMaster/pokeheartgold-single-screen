	.include "asm/macros.inc"
	.include "global.inc"

	.extern MATH_CountPopulation
	.extern OS_LockMutex
	.extern OS_UnlockMutex
	.extern WMSP_WL_MaKeyData
	.extern WMSP_WL_MaMp
	.extern WmspGetTmptt
	.extern __VENEER_MIi_CpuCopy16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text

	arm_func_start __VENEER_OS_LockMutex
__VENEER_OS_LockMutex: ; 0x06006598
	ldr pc, _0600659C ; =OS_LockMutex
	.align 2, 0
_0600659C: .word OS_LockMutex
	arm_func_end __VENEER_OS_LockMutex

	arm_func_start __VENEER_OS_UnlockMutex
__VENEER_OS_UnlockMutex: ; 0x060065A0
	ldr pc, _060065A4 ; =OS_UnlockMutex
	.align 2, 0
_060065A4: .word OS_UnlockMutex
	arm_func_end __VENEER_OS_UnlockMutex

	arm_func_start WMSP_SendMaKeyData
WMSP_SendMaKeyData: ; 0x060065A8
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x260
	ldr r1, _06006A98 ; =wmspW + 0x1000
	ldr fp, [r1, #0x550]
	add r0, fp, #0x100
	ldrh r0, [r0, #0x82]
	cmp r0, #0
	beq _06006A8C
	mov r0, #1
	strh r0, [fp, #0x8a]
	str r0, [sp, #0x14]
	mov r0, #0
	strh r0, [sp, #0x5c]
	ldr r8, [r1, #0x550]
	add r1, sp, #0x5c
	ldrh sb, [r8, #0x38]
	str r1, [sp, #0x34]
	add r1, sb, #0x1f
	ldrh r2, [r8, #0x80]
	bic r1, r1, #0x1f
	ldr r7, [r8, #0x7c]
	cmp r2, r1
	blt _06006A38
	add r1, r8, #0x100
	ldrh r1, [r1, #0x88]
	cmp r1, #0x10
	bhs _06006A38
	ldrh r1, [r8]
	cmp r1, #9
	beq _0600662C
	cmp r1, #0xa
	streq r0, [sp, #0x14]
	bne _06006A38
_0600662C:
	ldr r0, [sp, #0x34]
	mov r6, #0
	strh r6, [r0]
	mov r0, #1
	cmp sb, #0
	strlt r0, [sp, #0x14]
	blt _06006A38
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x14]
	cmp r0, #0
	beq _0600666C
	add r0, r8, #0x700
	ldrh r0, [r0, #0xc6]
	cmp r0, #0
	strne r6, [sp, #0x18]
	b _06006670
_0600666C:
	str r6, [sp, #0x18]
_06006670:
	add r0, r8, #0x31c
	mov r1, #0
	str r1, [sp, #0xc]
	mov r1, #1
	add r0, r0, #0x400
	str r1, [sp, #0x10]
	bl __VENEER_OS_LockMutex
	ldr r0, [r8, #0x734]
	cmp r0, #1
	bne _060066B0
	add r0, r8, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_UnlockMutex
	mov r0, #1
	str r0, [sp, #0x14]
	b _06006A38
_060066B0:
	mov r0, #1
	str r0, [r8, #0x734]
	add r0, r8, #0x2f8
	str r0, [sp, #0x1c]
	mov r5, #0
	b _06006A14
_060066C8:
	add r0, r8, #0x30c
	add r0, r0, #0x400
	mov r1, r5, lsl #2
	ldrh sl, [r0, r1]
	add r0, r0, r5, lsl #2
	str r0, [sp, #0x20]
	add r0, r8, #0x2fc
	add r0, r0, #0x400
	add r0, r0, r5, lsl #2
	str r0, [sp, #0x24]
	ldr r0, [sp, #0x20]
	str r0, [sp, #0x28]
	ldr r0, _06006A9C ; =0x0000FFFF
	str r0, [sp, #0x2c]
	str r0, [sp, #0x38]
	str r0, [sp, #0x3c]
	add r0, r8, #0x100
	str r0, [sp, #0x40]
	ldr r0, [sp, #0x2c]
	sub r0, r0, #0x8000
	str r0, [sp, #0x44]
	ldr r0, [sp, #0x2c]
	str r0, [sp, #0x48]
	add r0, r8, #0x700
	str r0, [sp, #0x4c]
	ldr r0, [sp, #0x2c]
	str r0, [sp, #0x50]
	str r0, [sp, #0x54]
	str r0, [sp, #0x58]
	b _060069FC
_06006740:
	ldr r0, [sp, #0x1c]
	ldr r2, [sp, #0xc]
	add r4, r0, sl, lsl #5
	ldrh r0, [r4, #2]
	mov r1, #1
	str r0, [sp, #8]
	tst r2, r1, lsl r0
	bne _060069C0
	ldr r0, [sp, #0x18]
	cmp r0, #0
	ldr r0, [sp, #8]
	orr r0, r2, r1, lsl r0
	str r0, [sp, #0xc]
	ldrh r0, [r4, #6]
	and r1, r0, r1
	str r1, [sp, #0x30]
	beq _0600678C
	bics r1, r1, #1
	bne _060069C0
_0600678C:
	ldr r1, [sp, #8]
	tst r1, #8
	movne r1, #1
	strne r1, [sp, #4]
	moveq r1, #0
	streq r1, [sp, #4]
	ldr r1, [sp, #0x14]
	cmp r1, #0
	orrne r1, r0, #1
	ldrne r0, [sp, #0x3c]
	cmpne r1, r0
	movne r0, #1
	strne r0, [sp]
	moveq r0, #0
	streq r0, [sp]
	ldrh r0, [r4, #0xe]
	and r1, r0, #1
	cmp r1, #1
	addeq r0, r0, #1
	streqh r0, [r4, #0xe]
	ldr r0, [sp, #4]
	ldr r1, [sp, #0x10]
	cmp r0, #0
	movne r0, #2
	moveq r0, #0
	cmp r1, #0
	movne r3, #0
	ldr r1, [sp]
	moveq r3, #2
	ldrh r2, [r4, #0xe]
	cmp r1, #0
	movne r1, #2
	add r2, r2, r3
	moveq r1, #0
	add r0, r2, r0
	add r0, r1, r0
	cmp r0, sb
	bgt _060069C0
	ldr r0, [sp, #0x10]
	cmp r0, #0
	streq r7, [sp, #0x34]
	moveq r0, #0
	streqh r0, [r7], #2
	ldr r0, [sp, #0x34]
	ldrh r2, [r4, #2]
	ldrh r1, [r4, #0xe]
	mov r2, r2, lsl #8
	mov r1, r1, lsr #1
	ldrh r0, [r0]
	and r2, r2, #0xf00
	and r1, r1, #0xff
	orr r1, r2, r1
	orr r1, r0, r1
	ldr r0, [sp, #0x34]
	addeq r6, r6, #2
	strh r1, [r0]
	ldr r0, [r4, #0x14]
	ldrh r2, [r4, #0xe]
	mov r1, r7
	subeq sb, sb, #2
	bl __VENEER_MIi_CpuCopy16
	ldrh r1, [r4, #0xe]
	ldr r0, [sp, #4]
	add r6, r6, r1
	cmp r0, #1
	bic r0, r1, #1
	add r7, r7, r0
	sub sb, sb, r1
	bne _060068F4
	ldrh r0, [r4, #0x10]
	tst r0, #1
	moveq r2, r0, asr #1
	beq _060068E0
	ldr r0, [sp, #0x40]
	ldrh r1, [r4, #2]
	ldrh r0, [r0, #0x88]
	mov r1, r1, lsl #0x1d
	add r0, r8, r0, lsl #4
	add r0, r0, r1, lsr #28
	add r0, r0, #0x100
	ldrh r2, [r0, #0xf8]
	add r1, r2, #1
	strh r1, [r0, #0xf8]
	mov r0, r2, lsl #1
	strh r0, [r4, #0x10]
_060068E0:
	ldr r0, [sp, #0x44]
	add r6, r6, #2
	and r0, r2, r0
	strh r0, [r7], #2
	sub sb, sb, #2
_060068F4:
	ldr r0, [sp]
	cmp r0, #1
	bne _06006924
	ldr r0, [sp, #0x34]
	add r6, r6, #2
	ldrh r0, [r0]
	sub sb, sb, #2
	orr r1, r0, #0x1000
	ldr r0, [sp, #0x34]
	strh r1, [r0]
	ldrh r0, [r4, #6]
	strh r0, [r7], #2
_06006924:
	ldr r0, [sp, #0x10]
	cmp r0, #1
	moveq r0, #0
	streq r0, [sp, #0x10]
	ldr r0, [sp, #0x20]
	ldrh r0, [r0, #2]
	cmp r0, sl
	ldreq r1, [sp, #0x2c]
	ldreq r0, [sp, #0x20]
	streqh r1, [r0, #2]
	ldrh r1, [r4]
	ldr r0, [sp, #0x28]
	strh r1, [r0]
	ldr r0, [sp, #0x38]
	strh r0, [r4]
	ldr r0, [sp, #0x24]
	ldrh r1, [r0, #2]
	ldr r0, [sp, #0x48]
	cmp r1, r0
	ldreq r0, [sp, #0x24]
	movne r1, r1, lsl #5
	streqh sl, [r0]
	ldrne r0, [sp, #0x1c]
	strneh sl, [r0, r1]
	ldr r0, [sp, #8]
	mov r1, #1
	mvn r1, r1, lsl r0
	ldr r0, [sp, #0xc]
	and r0, r0, r1
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x24]
	strh sl, [r0, #2]
	ldr r0, [sp, #0x30]
	ldr sl, [sp, #0x2c]
	strh r0, [r4, #0xa]
	ldr r0, [sp, #0x4c]
	ldrh r0, [r0, #0xc8]
	cmp r0, #1
	beq _06006A24
_060069C0:
	ldr r0, [sp, #0x50]
	str sl, [sp, #0x2c]
	cmp sl, r0
	ldrne r0, [sp, #0x1c]
	addne r0, r0, sl, lsl #5
	strne r0, [sp, #0x28]
	ldreq r0, [sp, #0x20]
	streq r0, [sp, #0x28]
	ldr r0, [sp, #0x54]
	cmp sl, r0
	ldrne r0, [sp, #0x1c]
	movne r1, sl, lsl #5
	ldrneh sl, [r0, r1]
	ldreq r0, [sp, #0x20]
	ldreqh sl, [r0]
_060069FC:
	ldr r0, [sp, #0x58]
	cmp sl, r0
	beq _06006A10
	cmp sb, #2
	bgt _06006740
_06006A10:
	add r5, r5, #1
_06006A14:
	cmp r5, #4
	bhs _06006A24
	cmp sb, #2
	bgt _060066C8
_06006A24:
	add r0, r8, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_UnlockMutex
	mov r0, #0
	str r0, [sp, #0x14]
_06006A38:
	ldr r0, [sp, #0x14]
	cmp r0, #1
	moveq r0, #0
	streqh r0, [fp, #0x8a]
	beq _06006A8C
	ldr r0, [fp, #0x1c]
	mov r1, r6, lsl #0x10
	cmp r0, #1
	ldreqh r0, [sp, #0x5c]
	mov r1, r1, lsr #0x10
	orreq r0, r0, #0x8000
	streqh r0, [sp, #0x5c]
	ldrh r2, [sp, #0x5c]
	ldr r3, [fp, #0x7c]
	add r0, sp, #0x60
	bl WMSP_WL_MaKeyData
	ldrh r0, [r0, #4]
	cmp r0, #0
	cmpne r0, #8
	movne r0, #0
	strneh r0, [fp, #0x8a]
_06006A8C:
	add sp, sp, #0x260
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06006A98: .word wmspW + 0x1000
_06006A9C: .word 0x0000FFFF
	arm_func_end WMSP_SendMaKeyData

	arm_func_start WMSP_SendMaMP
WMSP_SendMaMP: ; 0x06006AA0
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x2c8
	ldr r1, _06007438 ; =wmspW + 0x1000
	str r0, [sp, #0x18]
	ldr r7, [r1, #0x550]
	bl __VENEER_OS_DisableInterrupts
	add r1, r7, #0x100
	ldrh r1, [r1, #0x82]
	ldrh r4, [r7, #0x86]
	str r1, [sp, #0x74]
	bl __VENEER_OS_RestoreInterrupts
	mov r1, #0
	strh r1, [sp, #0xc4]
	ldrh r0, [r7, #0x88]
	cmp r0, #1
	ldreq r0, _0600743C ; =0x0000FFFF
	streq r0, [sp, #0x18]
	streqh r1, [r7, #0x88]
	ldrh r0, [r7, #0x9e]
	cmp r0, #0
	bne _06007388
	ldr r1, _06007438 ; =wmspW + 0x1000
	ldrh r0, [r7, #0x3a]
	ldr r8, [r1, #0x550]
	str r0, [sp, #0x24]
	add r0, sp, #0xc4
	ldrh sb, [r8, #0x38]
	str r0, [sp, #0x70]
	add r0, sb, #0x1f
	ldrh r1, [r8, #0x80]
	bic r0, r0, #0x1f
	cmp r1, r0
	ldr r6, [r8, #0x7c]
	movlt r0, #1
	blt _06006FC8
	add r0, r8, #0x100
	ldrh r0, [r0, #0x88]
	cmp r0, #0x10
	movhs r0, #1
	bhs _06006FC8
	ldrh r0, [r8]
	cmp r0, #9
	moveq r0, #1
	streq r0, [sp, #0x48]
	beq _06006B6C
	cmp r0, #0xa
	moveq r0, #0
	streq r0, [sp, #0x48]
	beq _06006B6C
	mov r0, #1
	b _06006FC8
_06006B6C:
	ldr r0, [sp, #0x70]
	mov r5, #0
	mov sl, r5
	mov fp, r5
	strh r5, [r0]
	cmp sb, #0
	movlt r0, #1
	blt _06006FC8
	ldr r0, [sp, #0x48]
	str r4, [sp, #0x50]
	cmp r0, #0
	mov r0, #1
	str r0, [sp, #0x4c]
	beq _06006BC0
	add r0, r8, #0x700
	ldrh r0, [r0, #0xc6]
	cmp r0, #0
	ldrne r0, [sp, #0x74]
	strne r5, [sp, #0x4c]
	strne r0, [sp, #0x50]
	b _06006BC4
_06006BC0:
	str r5, [sp, #0x4c]
_06006BC4:
	ldr r0, [sp, #0x18]
	and r4, r4, r0
	mov r0, #0
	str r0, [sp, #0x40]
	mov r0, #1
	str r0, [sp, #0x44]
	add r0, r8, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_LockMutex
	ldr r0, [r8, #0x734]
	cmp r0, #1
	bne _06006C08
	add r0, r8, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_UnlockMutex
	mov r0, #1
	b _06006FC8
_06006C08:
	mvn r0, r4
	str r0, [sp, #0x2c]
	mov r0, #1
	str r0, [r8, #0x734]
	add r0, r8, #0x2f8
	str r0, [sp, #0x54]
	mov r0, #0
	str r0, [sp, #0x58]
	b _06006FA0
_06006C2C:
	ldr r0, [sp, #0x58]
	mov r2, r0, lsl #2
	add r0, r8, #0x30c
	add r1, r0, #0x400
	ldr r0, [sp, #0x58]
	ldrh sl, [r1, r2]
	add r0, r1, r0, lsl #2
	str r0, [sp, #0x5c]
	add r0, r8, #0x2fc
	add r1, r0, #0x400
	ldr r0, [sp, #0x58]
	add r0, r1, r0, lsl #2
	str r0, [sp, #0x60]
	ldr r0, [sp, #0x5c]
	str r0, [sp, #0x64]
	ldr r0, _0600743C ; =0x0000FFFF
	str r0, [sp, #0x68]
	str r0, [sp, #0x78]
	str r0, [sp, #0x8c]
	add r0, r8, #0x100
	str r0, [sp, #0x90]
	ldr r0, [sp, #0x68]
	sub r0, r0, #0x8000
	str r0, [sp, #0x94]
	ldr r0, [sp, #0x68]
	str r0, [sp, #0x98]
	add r0, r8, #0x700
	str r0, [sp, #0x9c]
	ldr r0, [sp, #0x68]
	str r0, [sp, #0xa0]
	str r0, [sp, #0xa4]
	str r0, [sp, #0xa8]
	b _06006F80
_06006CB0:
	ldr r0, [sp, #0x54]
	ldr r2, [sp, #0x40]
	add r4, r0, sl, lsl #5
	ldrh r0, [r4, #2]
	mov r1, #1
	str r0, [sp, #0x28]
	tst r2, r1, lsl r0
	bne _06006F44
	ldr r0, [sp, #0x4c]
	cmp r0, #0
	ldr r0, [sp, #0x28]
	orr r0, r2, r1, lsl r0
	str r0, [sp, #0x40]
	ldrh r0, [r4, #6]
	ldr r1, [sp, #0x50]
	and r1, r0, r1
	str r1, [sp, #0x6c]
	beq _06006D08
	mov r2, r1
	ldr r1, [sp, #0x2c]
	tst r2, r1
	bne _06006F44
_06006D08:
	ldr r1, [sp, #0x28]
	tst r1, #8
	movne r1, #1
	strne r1, [sp, #0x20]
	moveq r1, #0
	streq r1, [sp, #0x20]
	ldr r1, [sp, #0x48]
	cmp r1, #0
	orrne r1, r0, #1
	ldrne r0, [sp, #0x8c]
	cmpne r1, r0
	movne r0, #1
	strne r0, [sp, #0x1c]
	moveq r0, #0
	streq r0, [sp, #0x1c]
	ldrh r0, [r4, #0xe]
	and r1, r0, #1
	cmp r1, #1
	addeq r0, r0, #1
	streqh r0, [r4, #0xe]
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #0x44]
	cmp r0, #0
	movne r0, #2
	moveq r0, #0
	cmp r1, #0
	movne r3, #0
	ldr r1, [sp, #0x1c]
	moveq r3, #2
	ldrh r2, [r4, #0xe]
	cmp r1, #0
	movne r1, #2
	add r2, r2, r3
	moveq r1, #0
	add r0, r2, r0
	add r0, r1, r0
	cmp r0, sb
	bgt _06006F44
	ldr r0, [sp, #0x44]
	cmp r0, #0
	streq r6, [sp, #0x70]
	moveq r0, #0
	streqh r0, [r6], #2
	ldr r0, [sp, #0x70]
	ldrh r2, [r4, #2]
	ldrh r1, [r4, #0xe]
	mov r2, r2, lsl #8
	mov r1, r1, lsr #1
	ldrh r0, [r0]
	and r2, r2, #0xf00
	and r1, r1, #0xff
	orr r1, r2, r1
	orr r1, r0, r1
	ldr r0, [sp, #0x70]
	addeq fp, fp, #2
	strh r1, [r0]
	ldr r0, [r4, #0x14]
	ldrh r2, [r4, #0xe]
	mov r1, r6
	subeq sb, sb, #2
	bl __VENEER_MIi_CpuCopy16
	ldrh r1, [r4, #0xe]
	ldr r0, [sp, #0x20]
	add fp, fp, r1
	cmp r0, #1
	bic r0, r1, #1
	add r6, r6, r0
	sub sb, sb, r1
	bne _06006E70
	ldrh r0, [r4, #0x10]
	tst r0, #1
	moveq r2, r0, asr #1
	beq _06006E5C
	ldr r0, [sp, #0x90]
	ldrh r1, [r4, #2]
	ldrh r0, [r0, #0x88]
	mov r1, r1, lsl #0x1d
	add r0, r8, r0, lsl #4
	add r0, r0, r1, lsr #28
	add r0, r0, #0x100
	ldrh r2, [r0, #0xf8]
	add r1, r2, #1
	strh r1, [r0, #0xf8]
	mov r0, r2, lsl #1
	strh r0, [r4, #0x10]
_06006E5C:
	ldr r0, [sp, #0x94]
	add fp, fp, #2
	and r0, r2, r0
	strh r0, [r6], #2
	sub sb, sb, #2
_06006E70:
	ldr r0, [sp, #0x1c]
	cmp r0, #1
	bne _06006EA0
	ldr r0, [sp, #0x70]
	add fp, fp, #2
	ldrh r0, [r0]
	sub sb, sb, #2
	orr r1, r0, #0x1000
	ldr r0, [sp, #0x70]
	strh r1, [r0]
	ldrh r0, [r4, #6]
	strh r0, [r6], #2
_06006EA0:
	ldr r0, [sp, #0x44]
	cmp r0, #1
	moveq r0, #0
	streq r0, [sp, #0x44]
	ldr r0, [sp, #0x5c]
	ldrh r0, [r0, #2]
	cmp r0, sl
	ldreq r1, [sp, #0x68]
	ldreq r0, [sp, #0x5c]
	streqh r1, [r0, #2]
	ldrh r1, [r4]
	ldr r0, [sp, #0x64]
	strh r1, [r0]
	ldr r0, [sp, #0x78]
	strh r0, [r4]
	ldr r0, [sp, #0x60]
	ldrh r1, [r0, #2]
	ldr r0, [sp, #0x98]
	cmp r1, r0
	ldreq r0, [sp, #0x60]
	movne r1, r1, lsl #5
	streqh sl, [r0]
	ldrne r0, [sp, #0x54]
	strneh sl, [r0, r1]
	ldr r0, [sp, #0x28]
	mov r1, #1
	mvn r1, r1, lsl r0
	ldr r0, [sp, #0x40]
	and r0, r0, r1
	str r0, [sp, #0x40]
	ldr r0, [sp, #0x60]
	strh sl, [r0, #2]
	ldr r0, [sp, #0x6c]
	ldr sl, [sp, #0x68]
	strh r0, [r4, #0xa]
	ldrh r0, [r4, #4]
	orr r5, r5, r0
	ldr r0, [sp, #0x9c]
	ldrh r0, [r0, #0xc8]
	cmp r0, #1
	beq _06006FB4
_06006F44:
	ldr r0, [sp, #0xa0]
	str sl, [sp, #0x68]
	cmp sl, r0
	ldrne r0, [sp, #0x54]
	addne r0, r0, sl, lsl #5
	strne r0, [sp, #0x64]
	ldreq r0, [sp, #0x5c]
	streq r0, [sp, #0x64]
	ldr r0, [sp, #0xa4]
	cmp sl, r0
	ldrne r0, [sp, #0x54]
	movne r1, sl, lsl #5
	ldrneh sl, [r0, r1]
	ldreq r0, [sp, #0x5c]
	ldreqh sl, [r0]
_06006F80:
	ldr r0, [sp, #0xa8]
	cmp sl, r0
	beq _06006F94
	cmp sb, #2
	bgt _06006CB0
_06006F94:
	ldr r0, [sp, #0x58]
	add r0, r0, #1
	str r0, [sp, #0x58]
_06006FA0:
	ldr r0, [sp, #0x58]
	cmp r0, #4
	bhs _06006FB4
	cmp sb, #2
	bgt _06006C2C
_06006FB4:
	add r0, r8, #0x31c
	add r0, r0, #0x400
	mov sl, fp
	bl __VENEER_OS_UnlockMutex
	mov r0, #0
_06006FC8:
	cmp r0, #1
	moveq r0, #0
	streqh r0, [r7, #0x62]
	streqh r0, [r7, #0x64]
	beq _0600742C
	add r0, r7, #0x700
	ldrh r1, [r0, #0xc6]
	ldr r0, [sp, #0x24]
	cmp r1, #1
	add r4, r0, #2
	ldrne r5, [sp, #0x18]
	ldr r0, [sp, #0x74]
	and r5, r5, r0
	mov r0, r5
	bl __VENEER_MATH_CountPopulation
	ldr r1, [sp, #0x24]
	add r1, r1, #0xc
	mul r0, r1, r0
	add r0, r0, #0x29
	ldrh r1, [r7, #0x72]
	bic r0, r0, #0x1f
	cmp r1, r0
	bge _06007348
	ldr r0, _06007438 ; =wmspW + 0x1000
	ldr sb, [r0, #0x550]
	ldrh r0, [sb]
	add r8, sb, #0x2f8
	cmp r0, #9
	moveq r4, #1
	beq _0600704C
	cmp r0, #0xa
	bne _06007338
	mov r4, #0
_0600704C:
	add r0, sb, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_LockMutex
	ldr r0, [sb, #0x734]
	cmp r0, #0
	bne _06007074
	add r0, sb, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_UnlockMutex
	b _06007338
_06007074:
	cmp r4, #0
	moveq r0, #1
	streq r0, [sp, #0x3c]
	beq _06007098
	bl __VENEER_OS_DisableInterrupts
	add r1, sb, #0x100
	ldrh r1, [r1, #0x82]
	str r1, [sp, #0x3c]
	bl __VENEER_OS_RestoreInterrupts
_06007098:
	mov r0, r5, lsl #0x10
	mvn r0, r0, lsr #16
	str r0, [sp, #0x30]
	mov sl, #0
_060070A8:
	ldr r0, _0600743C ; =0x0000FFFF
	add fp, sb, #0x600
	str r0, [sp, #0x38]
	add r0, sb, #0x2fc
	add r1, r0, #0x400
	mov r0, sl, lsl #2
	ldrh r5, [r1, r0]
	ldr r0, [sp, #0x38]
	add r6, r1, sl, lsl #2
	str r0, [sp, #0x34]
	ldr r0, [sp, #0x38]
	str r0, [sp, #0x7c]
	str r0, [sp, #0x80]
	str r0, [sp, #0x84]
	str r0, [sp, #0x88]
	str r0, [sp, #0xac]
	str r0, [sp, #0xb0]
	add r0, sb, #0x100
	str r0, [sp, #0xb4]
	ldr r0, [sp, #0x38]
	str r0, [sp, #0xb8]
	str r0, [sp, #0xbc]
	str r0, [sp, #0xc0]
	b _060072C4
_06007108:
	add r4, r8, r5, lsl #5
	ldrh r1, [r4, #0xa]
	ldr r0, [sp, #0x30]
	ldrh r2, [r4, #8]
	and r0, r1, r0
	orr r0, r2, r0
	strh r0, [r4, #8]
	ldrh r0, [r4, #8]
	ldrh r1, [r4, #6]
	mvn r0, r0
	and r0, r1, r0
	strh r0, [r4, #6]
	ldrh r1, [r4, #6]
	ldr r0, [sp, #0x3c]
	and r0, r1, r0
	strh r0, [r4, #6]
	mov r0, #0
	strh r0, [r4, #0xa]
	ldrh r0, [r4, #6]
	cmp r0, #0
	beq _060071D8
	ldrh r0, [r4, #2]
	tst r0, #8
	ldreqh r0, [r4, #0x12]
	cmpeq r0, #0
	beq _060071D8
	ldrh r0, [r4, #0x12]
	cmp r0, #0
	subne r0, r0, #1
	strneh r0, [r4, #0x12]
	ldrh r1, [r4]
	ldr r0, [sp, #0xac]
	cmp r1, r0
	ldreq r0, [sp, #0x7c]
	streqh r0, [r6, #2]
	ldr r1, [sp, #0x38]
	ldr r0, [sp, #0xb0]
	cmp r1, r0
	ldrh r0, [r4]
	strh r0, [r6]
	ldr r0, [sp, #0x80]
	strh r0, [r4]
	moveq r0, r5, lsl #0x10
	moveq r0, r0, lsr #0x10
	streq r0, [sp, #0x34]
	ldrne r0, [sp, #0x38]
	movne r0, r0, lsl #5
	strneh r5, [r8, r0]
	mov r0, r5, lsl #0x10
	mov r0, r0, lsr #0x10
	str r0, [sp, #0x38]
	b _060072C0
_060071D8:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x81
	strh r1, [r0]
	ldrh r1, [r4, #6]
	ldr r3, [sp, #0xb4]
	cmp r1, #0
	moveq r1, #0
	movne r1, #0xf
	strh r1, [r0, #2]
	mov r1, #0x14
	strh r1, [r0, #8]
	ldrh r1, [r4, #2]
	strh r1, [r0, #0xa]
	ldrh r1, [r4, #4]
	strh r1, [r0, #0xc]
	ldrh r1, [r4, #6]
	strh r1, [r0, #0xe]
	ldrh r1, [r4, #8]
	strh r1, [r0, #0x10]
	ldrh r1, [r4, #0xe]
	strh r1, [r0, #0x18]
	ldr r1, [r4, #0x14]
	str r1, [r0, #0x14]
	ldr r1, [r4, #0x18]
	str r1, [r0, #0x1c]
	ldr r1, [r4, #0x1c]
	str r1, [r0, #0x20]
	ldrh r1, [r4, #0x10]
	strh r1, [r0, #0x1a]
	ldrh r3, [r3, #0x88]
	ldrh r1, [sb, #0x30]
	cmp r3, #0
	ldrh r2, [sb, #0x32]
	moveq r3, r1
	movne r3, r2
	strh r3, [r0, #0x24]
	ldr r3, [sp, #0xb4]
	ldrh r3, [r3, #0x88]
	cmp r3, #0
	movne r2, r1
	strh r2, [r0, #0x26]
	bl __VENEER_WMSP_ReturnResult2Wm9
	ldrh r1, [r4]
	ldr r0, [sp, #0xb8]
	cmp r1, r0
	ldreq r0, [sp, #0x84]
	streqh r0, [r6, #2]
	ldrh r0, [r4]
	strh r0, [r6]
	ldr r0, [sp, #0x88]
	strh r0, [r4]
	ldrh r1, [fp, #0xfa]
	ldr r0, [sp, #0xbc]
	cmp r1, r0
	streqh r5, [fp, #0xf8]
	movne r0, r1, lsl #5
	strneh r5, [r8, r0]
	strh r5, [fp, #0xfa]
_060072C0:
	ldrh r5, [r6]
_060072C4:
	ldr r0, [sp, #0xc0]
	cmp r5, r0
	bne _06007108
	ldr r2, _0600743C ; =0x0000FFFF
	ldr r0, [sp, #0x38]
	cmp r0, r2
	beq _06007318
	add r0, sb, sl, lsl #2
	add r1, r0, #0x700
	ldr r0, [sp, #0x38]
	ldrh r3, [r1, #0xc]
	mov r0, r0, lsl #5
	strh r3, [r8, r0]
	ldrh r0, [r1, #0xe]
	cmp r0, r2
	ldreq r0, [sp, #0x38]
	streqh r0, [r1, #0xe]
	add r0, sb, sl, lsl #2
	add r1, r0, #0x700
	ldr r0, [sp, #0x34]
	strh r0, [r1, #0xc]
_06007318:
	add sl, sl, #1
	cmp sl, #4
	blt _060070A8
	add r0, sb, #0x31c
	mov r1, #0
	add r0, r0, #0x400
	str r1, [sb, #0x734]
	bl __VENEER_OS_UnlockMutex
_06007338:
	mov r0, #0
	strh r0, [r7, #0x62]
	strh r0, [r7, #0x64]
	b _0600742C
_06007348:
	ldrsh r0, [r7, #0x62]
	cmp r0, #1
	ldrnesh r0, [r7, #0x64]
	cmpne r0, #1
	movne r6, #0
	bne _060073C8
	ldrh r3, [r7, #0x40]
	mov r0, sl
	mov r1, r4
	mov r2, r5
	bl WmspGetTmptt
	ldrh r1, [sp, #0xc4]
	mov r6, r0
	orr r0, r1, #0x8000
	strh r0, [sp, #0xc4]
	b _060073C8
_06007388:
	mov r6, #0
	strh r6, [r7, #0x9e]
	ldrh r0, [sp, #0xc4]
	ldr r5, [sp, #0x74]
	bic r0, r0, #0x8000
	strh r0, [sp, #0xc4]
	mov sl, r6
	ldr r4, _06007440 ; =0x000080D6
	bl __VENEER_OS_DisableInterrupts
	ldrsh r1, [r7, #0x62]
	add r1, r1, #1
	strh r1, [r7, #0x62]
	ldrsh r1, [r7, #0x64]
	add r1, r1, #1
	strh r1, [r7, #0x64]
	bl __VENEER_OS_RestoreInterrupts
_060073C8:
	mov r0, r5, lsl #0x10
	mov r0, r0, lsr #0x10
	str r0, [sp]
	ldr r0, _06007444 ; =0x048080F8
	str r6, [sp, #4]
	ldrh r1, [r0]
	mov r0, sl, lsl #0x10
	str r1, [sp, #8]
	mov r0, r0, lsr #0x10
	str r0, [sp, #0xc]
	ldrh r0, [sp, #0xc4]
	mov r3, r4, lsl #0x10
	str r0, [sp, #0x10]
	ldr r2, [r7, #0x7c]
	mov r1, #0
	str r2, [sp, #0x14]
	add r0, sp, #0xc8
	mov r2, r1
	mov r3, r3, lsr #0x10
	bl WMSP_WL_MaMp
	strh r5, [r7, #0x68]
	ldrh r0, [sp, #0xc4]
	strh r0, [r7, #0x6a]
	strh r4, [r7, #0x6c]
	strh sl, [r7, #0x6e]
_0600742C:
	add sp, sp, #0x2c8
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06007438: .word wmspW + 0x1000
_0600743C: .word 0x0000FFFF
_06007440: .word 0x000080D6
_06007444: .word 0x048080F8
	arm_func_end WMSP_SendMaMP

	arm_func_start __VENEER_MATH_CountPopulation
__VENEER_MATH_CountPopulation: ; 0x06007448
	ldr pc, _0600744C ; =MATH_CountPopulation
	.align 2, 0
_0600744C: .word MATH_CountPopulation
	arm_func_end __VENEER_MATH_CountPopulation
