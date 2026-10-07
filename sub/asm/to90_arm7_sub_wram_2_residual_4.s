	.include "asm/macros.inc"
	.include "global.inc"

	.extern PM_GetLEDPattern
	.extern PM_SetLEDPattern
	.extern PMi_KeyPattern
	.extern PMi_Work
	.extern SPIi_ReturnResult
	.extern SPIi_SetEntry

.text

	arm_func_start PM_AnalyzeCommand
PM_AnalyzeCommand: ; 0x03801980
	stmdb sp!, {r3, r4, lr}
	sub sp, sp, #4
	tst r0, #0x2000000
	beq _038019B0
	ldr r1, _03801B34 ; =PMi_Work
	mov r4, #0
	mov r3, r4
_0380199C:
	mov r2, r4, lsl #1
	add r4, r4, #1
	strh r3, [r1, r2]
	cmp r4, #0x10
	blt _0380199C
_038019B0:
	ldr r1, _03801B34 ; =PMi_Work
	and r2, r0, #0xf0000
	mov r2, r2, lsr #0x10
	mov r2, r2, lsl #1
	strh r0, [r1, r2]
	tst r0, #0x1000000
	beq _03801B28
	ldr r1, _03801B38 ; =PMi_KeyPattern
	ldrh r2, [r1, #8]
	and r0, r2, #0xff00
	mov r0, r0, lsl #8
	mov r4, r0, lsr #0x10
	sub r0, r4, #0x60
	cmp r0, #7
	addls pc, pc, r0, lsl #2
	b _03801B1C
_038019F0: ; jump table
	b _03801A10 ; case 0
	b _03801A20 ; case 1
	b _03801B1C ; case 2
	b _03801AB8 ; case 3
	b _03801A54 ; case 4
	b _03801A88 ; case 5
	b _03801AEC ; case 6
	b _03801B04 ; case 7
_03801A10:
	mov r0, #0x60
	mov r1, #0
	bl SPIi_ReturnResult
	b _03801B28
_03801A20:
	ldrh ip, [r1, #0xa]
	mov r1, r4
	and r3, r2, #0xff
	mov r0, #3
	mov r2, #2
	str ip, [sp]
	bl SPIi_SetEntry
	cmp r0, #0
	bne _03801B28
	mov r0, r4
	mov r1, #4
	bl SPIi_ReturnResult
	b _03801B28
_03801A54:
	ldrh ip, [r1, #0xa]
	mov r1, r4
	and r3, r2, #0xff
	mov r0, #3
	mov r2, #2
	str ip, [sp]
	bl SPIi_SetEntry
	cmp r0, #0
	bne _03801B28
	mov r0, r4
	mov r1, #4
	bl SPIi_ReturnResult
	b _03801B28
_03801A88:
	mov r0, r2, lsl #0x10
	mov r1, r4
	mov r3, r0, lsr #0x10
	mov r0, #3
	mov r2, #1
	bl SPIi_SetEntry
	cmp r0, #0
	bne _03801B28
	mov r0, r4
	mov r1, #4
	bl SPIi_ReturnResult
	b _03801B28
_03801AB8:
	ldrh r0, [r1, #0xa]
	mov r2, r2, lsl #0x18
	mov r1, r4
	orr r3, r0, r2, lsr #8
	mov r0, #3
	mov r2, #1
	bl SPIi_SetEntry
	cmp r0, #0
	bne _03801B28
	mov r0, r4
	mov r1, #4
	bl SPIi_ReturnResult
	b _03801B28
_03801AEC:
	and r0, r2, #0xff
	bl PM_SetLEDPattern
	mov r0, #0x66
	mov r1, #0
	bl SPIi_ReturnResult
	b _03801B28
_03801B04:
	bl PM_GetLEDPattern
	mov r0, r0, lsl #0x10
	mov r1, r0, lsr #0x10
	mov r0, #0x67
	bl SPIi_ReturnResult
	b _03801B28
_03801B1C:
	mov r0, r4
	mov r1, #1
	bl SPIi_ReturnResult
_03801B28:
	add sp, sp, #4
	ldmia sp!, {r3, r4, lr}
	bx lr
	.align 2, 0
_03801B34: .word PMi_Work
_03801B38: .word PMi_KeyPattern
	arm_func_end PM_AnalyzeCommand
