	.include "asm/macros.inc"
	.include "global.inc"

	.extern SPIi_ReturnResult
	.extern SPIi_SetEntry
	.extern nvramw

.text

	arm_func_start NVRAM_AnalyzeCommand
NVRAM_AnalyzeCommand: ; 0x03803888
	stmdb sp!, {r3, r4, r5, lr}
	sub sp, sp, #8
	tst r0, #0x2000000
	beq _038038B8
	ldr r2, _03803A28 ; =nvramw
	mov r5, #0
	mov lr, r5
_038038A4:
	mov r4, r5, lsl #1
	add r5, r5, #1
	strh lr, [r2, r4]
	cmp r5, #0x10
	blt _038038A4
_038038B8:
	ldr r2, _03803A28 ; =nvramw
	and r4, r0, #0xf0000
	mov r4, r4, lsr #0x10
	mov r4, r4, lsl #1
	strh r0, [r2, r4]
	tst r0, #0x1000000
	beq _03803A1C
	ldr lr, _03803A2C ; =nvramw
	ldrh r0, [lr]
	and r2, r0, #0xff00
	mov r2, r2, lsl #8
	mov r4, r2, lsr #0x10
	sub r2, r4, #0x22
	cmp r2, #0xa
	addls pc, pc, r2, lsl #2
	b _038039F0
_038038F8: ; jump table
	b _03803924 ; case 0
	b _0380395C ; case 1
	b _0380395C ; case 2
	b _038039A4 ; case 3
	b _038039A4 ; case 4
	b _038039E4 ; case 5
	b _038039E4 ; case 6
	b _038039F0 ; case 7
	b _038039F0 ; case 8
	b _038039F0 ; case 9
	b _03803924 ; case 10
_03803924:
	ldrh ip, [lr, #4]
	ldrh r2, [lr, #2]
	mov r0, r0, lsl #0x18
	and ip, ip, #0xff00
	orr r0, r0, r2, lsl #8
	orr ip, r0, ip, lsr #8
	cmp ip, #0x2000000
	blo _0380394C
	cmp ip, #0x2800000
	blo _038039F0
_0380394C:
	mov r0, r4
	mov r1, #2
	bl SPIi_ReturnResult
	b _03803A1C
_0380395C:
	ldrh r2, [lr, #8]
	ldrh r1, [lr, #0xa]
	orr ip, r1, r2, lsl #16
	cmp ip, #0x2000000
	blo _03803978
	cmp ip, #0x2800000
	blo _03803988
_03803978:
	mov r0, r4
	mov r1, #2
	bl SPIi_ReturnResult
	b _03803A1C
_03803988:
	ldrh r2, [lr, #2]
	mov r3, r0, lsl #0x18
	ldrh r1, [lr, #4]
	ldrh r0, [lr, #6]
	orr r3, r2, r3, lsr #8
	orr r1, r0, r1, lsl #16
	b _038039F0
_038039A4:
	ldrh r2, [lr, #6]
	ldrh r1, [lr, #8]
	orr ip, r1, r2, lsl #16
	cmp ip, #0x2000000
	blo _038039C0
	cmp ip, #0x2800000
	blo _038039D0
_038039C0:
	mov r0, r4
	mov r1, #2
	bl SPIi_ReturnResult
	b _03803A1C
_038039D0:
	ldrh r2, [lr, #2]
	mov r0, r0, lsl #0x18
	ldrh r1, [lr, #4]
	orr r3, r2, r0, lsr #8
	b _038039F0
_038039E4:
	ldrh r2, [lr, #2]
	mov r0, r0, lsl #0x18
	orr r3, r2, r0, lsr #8
_038039F0:
	str r1, [sp]
	mov r1, r4
	mov r0, #1
	mov r2, #3
	str ip, [sp, #4]
	bl SPIi_SetEntry
	cmp r0, #0
	bne _03803A1C
	mov r0, r4
	mov r1, #4
	bl SPIi_ReturnResult
_03803A1C:
	add sp, sp, #8
	ldmia sp!, {r3, r4, r5, lr}
	bx lr
	.align 2, 0
_03803A28: .word nvramw
_03803A2C: .word nvramw
	arm_func_end NVRAM_AnalyzeCommand
