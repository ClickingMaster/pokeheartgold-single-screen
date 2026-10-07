	.include "asm/macros.inc"
	.include "global.inc"

	.extern SPI_DummyWait_03801538

.text

	arm_func_start TPi_DetectPos
TPi_DetectPos: ; 0x0380155C
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #0x14
	cmp r2, #0
	ldr r2, _03801738 ; =0x040001C0
	mov r5, r3
	moveq r3, #0xd1
	moveq r4, #1
	mov r7, r0
	mov r6, r1
	movne r3, #0x91
	movne r4, #2
_03801588:
	ldrh r0, [r2]
	tst r0, #0x80
	bne _03801588
	ldr r1, _0380173C ; =0x00008A01
	ldr r0, _03801740 ; =0x040001C2
	strh r1, [r2]
	and r2, r3, #0xff
	strh r2, [r0]
	sub r1, r0, #2
_038015AC:
	ldrh r0, [r1]
	tst r0, #0x80
	bne _038015AC
	mov r0, r2, lsl #0x10
	ldr r2, _03801738 ; =0x040001C0
	mov ip, #0
	ldr r8, _03801744 ; =0x00007FF8
	add r1, sp, #0
	mov r0, r0, lsr #0x10
	mov r3, ip
_038015D4:
	strh r3, [r2, #2]
_038015D8:
	ldrh sb, [r2]
	tst sb, #0x80
	bne _038015D8
	ldrh sb, [r2, #2]
	and sb, sb, #0xff
	mov lr, sb, lsl #0x10
	mov sb, lr, lsr #8
	str sb, [r1, ip, lsl #2]
	strh r0, [r2, #2]
_038015FC:
	ldrh sb, [r2]
	tst sb, #0x80
	bne _038015FC
	ldrh lr, [r2, #2]
	ldr sb, [r1, ip, lsl #2]
	and lr, lr, #0xff
	mov lr, lr, lsl #0x10
	orr sb, sb, lr, lsr #16
	and lr, sb, r8
	mov lr, lr, asr #3
	str lr, [r1, ip, lsl #2]
	add ip, ip, #1
	cmp ip, #5
	blt _038015D4
	ldr r0, _03801748 ; =0x00008201
	strh r0, [r2]
	bl SPI_DummyWait_03801538
	mov r8, #0
	mov sb, r8
	add r2, sp, #0
_0380164C:
	ldr r1, [r2, sb, lsl #2]
	add r3, sb, #1
	b _03801670
_03801658:
	ldr r0, [r2, r3, lsl #2]
	add r3, r3, #1
	subs r0, r1, r0
	rsbmi r0, r0, #0
	cmp r0, r8
	movgt r8, r0
_03801670:
	cmp r3, #5
	blt _03801658
	add sb, sb, #1
	cmp sb, #4
	blt _0380164C
	strh r8, [r5]
	mov r8, #0
	add r0, sp, #0
	b _03801708
_03801694:
	ldr r3, [r0, r8, lsl #2]
	add sb, r8, #1
	b _038016FC
_038016A0:
	ldr r2, [r0, sb, lsl #2]
	subs r1, r3, r2
	rsbmi r1, r1, #0
	cmp r1, r6
	bgt _038016F8
	add r1, sb, #1
	b _038016F0
_038016BC:
	ldr r5, [r0, r1, lsl #2]
	subs ip, r3, r5
	rsbmi ip, ip, #0
	cmp ip, r6
	bgt _038016EC
	add r0, r2, r3, lsl #1
	add r0, r5, r0
	mov r0, r0, asr #2
	bic r0, r0, #7
	strh r0, [r7]
	mov r0, #0
	b _0380172C
_038016EC:
	add r1, r1, #1
_038016F0:
	cmp r1, #5
	blt _038016BC
_038016F8:
	add sb, sb, #1
_038016FC:
	cmp sb, #4
	blt _038016A0
	add r8, r8, #1
_03801708:
	cmp r8, #3
	blt _03801694
	ldr r2, [sp]
	ldr r1, [sp, #0x10]
	mov r0, r4
	add r1, r2, r1
	mov r1, r1, asr #1
	bic r1, r1, #7
	strh r1, [r7]
_0380172C:
	add sp, sp, #0x14
	ldmia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bx lr
	.align 2, 0
_03801738: .word 0x040001C0
_0380173C: .word 0x00008A01
_03801740: .word 0x040001C2
_03801744: .word 0x00007FF8
_03801748: .word 0x00008201
	arm_func_end TPi_DetectPos
