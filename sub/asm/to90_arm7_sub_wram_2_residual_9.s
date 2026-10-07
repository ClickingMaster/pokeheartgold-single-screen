	.include "asm/macros.inc"
	.include "global.inc"

	.extern SPI_DummyWaitReceive
	.extern micw

.text

	arm_func_start MIC_ExecSampling12
MIC_ExecSampling12: ; 0x03802BB4
	stmdb sp!, {r4, lr}
	ldr r2, _03802CCC ; =0x040001C0
_03802BBC:
	ldrh r0, [r2]
	tst r0, #0x80
	bne _03802BBC
	ldr r1, _03802CD0 ; =0x00008A01
	ldr r0, _03802CD4 ; =0x040001C2
	strh r1, [r2]
	mov r1, #0xe4
	strh r1, [r0]
	sub r1, r0, #2
_03802BE0:
	ldrh r0, [r1]
	tst r0, #0x80
	bne _03802BE0
	bl SPI_DummyWaitReceive
	ldr r2, _03802CD8 ; =0x00008201
	ldr r1, _03802CCC ; =0x040001C0
	mov r4, r0, lsl #0x18
	strh r2, [r1]
	bl SPI_DummyWaitReceive
	ldr r3, _03802CDC ; =micw + 0x3C
	ldr r1, _03802CE0 ; =0x00007FF8
	orr r0, r0, r4, lsr #16
	and r2, r0, r1
	ldr r0, [r3, #4]
	mov r2, r2, lsl #0x11
	add r4, r0, r2, lsr #16
	str r4, [r3, #4]
	ldr r0, [r3, #8]
	add r0, r0, #1
	str r0, [r3, #8]
	cmp r0, #0x100
	blo _03802C98
	mov r0, r1, lsl #1
	and r0, r0, r4, lsr #8
	ldrsh r4, [r3, #2]
	sub r0, r0, #0x8000
	mov r0, r0, lsl #0x10
	cmp r4, r0, asr #16
	mov r1, r0, asr #0x10
	cmplt r4, #0xc00
	addlt r0, r4, #0x10
	strlth r0, [r3, #2]
	blt _03802C88
	cmp r1, r4
	bge _03802C88
	mov r0, #0xc00
	rsb r0, r0, #0
	cmp r4, r0
	ldrgt r0, _03802CDC ; =micw + 0x3C
	ldrgtsh r1, [r0, #2]
	subgt r1, r1, #0x10
	strgth r1, [r0, #2]
_03802C88:
	ldr r0, _03802CDC ; =micw + 0x3C
	mov r1, #0
	str r1, [r0, #8]
	str r1, [r0, #4]
_03802C98:
	ldr r1, _03802CDC ; =micw + 0x3C
	ldr r0, _03802CE4 ; =0x0000FFF0
	ldrsh r1, [r1, #2]
	rsb r1, r1, r2, lsr #16
	cmp r1, r0
	movgt r1, r0
	bgt _03802CBC
	cmp r1, #0
	movlt r1, #0
_03802CBC:
	mov r0, r1, lsl #0x10
	mov r0, r0, lsr #0x10
	ldmia sp!, {r4, lr}
	bx lr
	.align 2, 0
_03802CCC: .word 0x040001C0
_03802CD0: .word 0x00008A01
_03802CD4: .word 0x040001C2
_03802CD8: .word 0x00008201
_03802CDC: .word micw + 0x3C
_03802CE0: .word 0x00007FF8
_03802CE4: .word 0x0000FFF0
	arm_func_end MIC_ExecSampling12
