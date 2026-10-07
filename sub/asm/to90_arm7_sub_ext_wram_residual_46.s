	.include "asm/macros.inc"
	.include "global.inc"

	.extern AllocateHeapBuf
	.extern DMA_Read

.text

	arm_func_start TakeoutRxFrame
TakeoutRxFrame: ; 0x0600E13C
	stmdb sp!, {r4, r5, r6, lr}
	ldr r2, _0600E1A8 ; =0x0380FFF4
	mov r5, r1
	ldr r1, [r2]
	mov r6, r0
	add r0, r1, #0x188
	add r1, r5, #0x22
	bl AllocateHeapBuf
	movs r4, r0
	moveq r0, #0
	beq _0600E1A0
	mov r1, r6
	add r0, r4, #0x18
	add r2, r5, #0xc
	bl DMA_Read
	sub r0, r5, #0x18
	strh r0, [r4, #0x16]
	ldrh r0, [r4, #0x22]
	ldrh r1, [r4, #0x1e]
	and r0, r0, #0xff
	and r1, r1, #0xff
	mov r0, r0, lsl #0x10
	orr r0, r1, r0, lsr #8
	strh r0, [r4, #0x1e]
	add r0, r4, #0x10
_0600E1A0:
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_0600E1A8: .word 0x0380FFF4
	arm_func_end TakeoutRxFrame
