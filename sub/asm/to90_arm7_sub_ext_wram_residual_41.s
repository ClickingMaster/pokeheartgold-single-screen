	.include "asm/macros.inc"
	.include "global.inc"

	.extern BBPDiagSkipAdrsES1
	.extern BBPDiagSkipAdrsRelease
	.extern BBP_Read
	.extern BBP_Write

.text

	arm_func_start DiagBaseBand
DiagBaseBand: ; 0x0600C210
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	ldr r0, _0600C4E4 ; =0x0380FFF4
	mov r7, #0
	ldr r0, [r0]
	mov r6, #0
	add r0, r0, #0x500
	ldrh r0, [r0, #0xf8]
	mov r5, #0xff
	cmp r0, #5
	ldreq sl, _0600C4E8 ; =BBPDiagSkipAdrsES1
	mvn r4, #0
	ldrne sl, _0600C4EC ; =BBPDiagSkipAdrsRelease
	b _0600C260
_0600C244:
	mov r0, r6
	mov r1, r5
	bl BBP_Write
	cmp r0, r4
	moveq r7, #1
	beq _0600C4BC
	add r6, r6, #1
_0600C260:
	cmp r6, #0x69
	blo _0600C244
	mov r5, #0
	mov r4, r5
	b _0600C2B0
_0600C274:
	mov r0, r4, lsl #1
	ldrh r0, [sl, r0]
	cmp r5, r0
	addeq r4, r4, #1
	beq _0600C2AC
	mov r0, r5
	bl BBP_Read
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	cmp r0, #0xff
	beq _0600C2AC
	cmp r7, #0x20
	add r7, r7, #1
	bhi _0600C4BC
_0600C2AC:
	add r5, r5, #1
_0600C2B0:
	cmp r5, #0x69
	blo _0600C274
	mov r5, #0
	mov r4, r5
_0600C2C0:
	mov r0, r5
	mov r1, r4
	bl BBP_Write
	add r5, r5, #1
	cmp r5, #0x69
	blo _0600C2C0
	mov r4, #0
	mov r5, r4
	b _0600C31C
_0600C2E4:
	mov r0, r5, lsl #1
	ldrh r0, [sl, r0]
	cmp r4, r0
	addeq r5, r5, #1
	beq _0600C318
	mov r0, r4
	bl BBP_Read
	mov r0, r0, lsl #0x10
	movs r0, r0, lsr #0x10
	beq _0600C318
	cmp r7, #0x20
	add r7, r7, #1
	bhi _0600C4BC
_0600C318:
	add r4, r4, #1
_0600C31C:
	cmp r4, #0x69
	blo _0600C2E4
	mov r4, #0x55
	mov r5, #0
_0600C32C:
	mov r0, r5
	mov r1, r4
	bl BBP_Write
	mvn r0, r4
	add r5, r5, #1
	mov r0, r0, lsl #0x10
	cmp r5, #0x69
	mov r4, r0, lsr #0x10
	blo _0600C32C
	mov r4, #0
	mov r6, r4
	mov r5, #0x55
	b _0600C3A4
_0600C360:
	mov r0, r6, lsl #1
	ldrh r0, [sl, r0]
	cmp r4, r0
	addeq r6, r6, #1
	beq _0600C398
	mov r0, r4
	bl BBP_Read
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	cmp r0, r5
	beq _0600C398
	cmp r7, #0x20
	add r7, r7, #1
	bhi _0600C4BC
_0600C398:
	mvn r0, r5
	and r5, r0, #0xff
	add r4, r4, #1
_0600C3A4:
	cmp r4, #0x69
	blo _0600C360
	mov r5, #0xff
	mov r4, #0
_0600C3B4:
	mov r0, r4
	mov r1, r5
	bl BBP_Write
	sub r0, r5, #1
	add r4, r4, #1
	mov r0, r0, lsl #0x10
	cmp r4, #0x69
	mov r5, r0, lsr #0x10
	blo _0600C3B4
	mov r4, #0
	mov r6, r4
	mov r5, #0xff
	b _0600C430
_0600C3E8:
	mov r0, r6, lsl #1
	ldrh r0, [sl, r0]
	cmp r4, r0
	addeq r6, r6, #1
	beq _0600C420
	mov r0, r4
	bl BBP_Read
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	cmp r0, r5
	beq _0600C420
	cmp r7, #0x20
	add r7, r7, #1
	bhi _0600C4BC
_0600C420:
	sub r0, r5, #1
	mov r0, r0, lsl #0x10
	add r4, r4, #1
	mov r5, r0, lsr #0x10
_0600C430:
	cmp r4, #0x69
	blo _0600C3E8
	mov r8, #0
	mov sb, r8
	mov r4, #1
	mov fp, r8
	b _0600C4B4
_0600C44C:
	mov r0, sb, lsl #1
	ldrh r0, [sl, r0]
	cmp r8, r0
	addeq sb, sb, #1
	beq _0600C4B0
	mov r6, r4
	mov r5, fp
	b _0600C4A8
_0600C46C:
	mov r0, r8
	mov r1, r6
	bl BBP_Write
	mov r0, r8
	bl BBP_Read
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	cmp r0, r6
	beq _0600C49C
	cmp r7, #0x20
	add r7, r7, #1
	bhi _0600C4BC
_0600C49C:
	mov r0, r6, lsl #1
	and r6, r0, #0xff
	add r5, r5, #1
_0600C4A8:
	cmp r5, #9
	blo _0600C46C
_0600C4B0:
	add r8, r8, #1
_0600C4B4:
	cmp r8, #0x69
	blo _0600C44C
_0600C4BC:
	cmp r7, #0
	beq _0600C4DC
	ldr r0, _0600C4E4 ; =0x0380FFF4
	ldr r0, [r0]
	add r0, r0, #0x300
	ldrh r1, [r0, #0x3e]
	orr r1, r1, #8
	strh r1, [r0, #0x3e]
_0600C4DC:
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_0600C4E4: .word 0x0380FFF4
_0600C4E8: .word BBPDiagSkipAdrsES1
_0600C4EC: .word BBPDiagSkipAdrsRelease
	arm_func_end DiagBaseBand
