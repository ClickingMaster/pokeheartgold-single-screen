	.include "asm/macros.inc"
	.include "global.inc"

	.extern SPI_SendWait_03803D98

.text

	arm_func_start NVRAM_PageErase
NVRAM_PageErase: ; 0x038042CC
	stmdb sp!, {r3, r4, r5, lr}
	and r1, r0, #0xff0000
	mov r1, r1, lsr #0x10
	and r2, r0, #0xff00
	mov r2, r2, lsl #8
	ldr r3, _0380434C ; =0x040001C0
	mov r1, r1, lsl #0x10
	and r5, r0, #0xff
	mov r0, r1, lsr #0x10
	mov r4, r2, lsr #0x10
_038042F4:
	ldrh r1, [r3]
	tst r1, #0x80
	bne _038042F4
	ldr r1, _03804350 ; =0x040001C2
	mov r2, #0x8900
	strh r2, [r3]
	mov r2, #0xdb
	strh r2, [r1]
	sub r2, r1, #2
_03804318:
	ldrh r1, [r2]
	tst r1, #0x80
	bne _03804318
	bl SPI_SendWait_03803D98
	mov r0, r4
	bl SPI_SendWait_03803D98
	ldr r1, _0380434C ; =0x040001C0
	mov r2, #0x8100
	mov r0, r5
	strh r2, [r1]
	bl SPI_SendWait_03803D98
	ldmia sp!, {r3, r4, r5, lr}
	bx lr
	.align 2, 0
_0380434C: .word 0x040001C0
_03804350: .word 0x040001C2
	arm_func_end NVRAM_PageErase

	arm_func_start NVRAM_SectorErase
NVRAM_SectorErase: ; 0x03804354
	stmdb sp!, {r3, r4, r5, lr}
	and r1, r0, #0xff0000
	mov r1, r1, lsr #0x10
	and r2, r0, #0xff00
	mov r2, r2, lsl #8
	ldr r3, _038043D4 ; =0x040001C0
	mov r1, r1, lsl #0x10
	and r5, r0, #0xff
	mov r0, r1, lsr #0x10
	mov r4, r2, lsr #0x10
_0380437C:
	ldrh r1, [r3]
	tst r1, #0x80
	bne _0380437C
	ldr r1, _038043D8 ; =0x040001C2
	mov r2, #0x8900
	strh r2, [r3]
	mov r2, #0xd8
	strh r2, [r1]
	sub r2, r1, #2
_038043A0:
	ldrh r1, [r2]
	tst r1, #0x80
	bne _038043A0
	bl SPI_SendWait_03803D98
	mov r0, r4
	bl SPI_SendWait_03803D98
	ldr r1, _038043D4 ; =0x040001C0
	mov r2, #0x8100
	mov r0, r5
	strh r2, [r1]
	bl SPI_SendWait_03803D98
	ldmia sp!, {r3, r4, r5, lr}
	bx lr
	.align 2, 0
_038043D4: .word 0x040001C0
_038043D8: .word 0x040001C2
	arm_func_end NVRAM_SectorErase
