	.extern ov99_021E7C58
	.extern ov99_021E9F54
	.include "asm/macros.inc"
	.include "overlay_99.inc"
	.include "global.inc"
	.public ov99_021E7AD4

.text
	thumb_func_start ov99_021E7AD4
ov99_021E7AD4: ; 0x021E7AD4
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #0xfd
	lsl r0, r0, #2
	add r4, r1, #0
	ldr r1, [r5, r0]
	add r7, r2, #0
	lsl r2, r1, #0xd
	lsr r2, r2, #0x1b
	cmp r4, #0
	beq _021E7AFA
	ldr r3, _021E7BB8 ; =0xFFF83FFF
	and r1, r3
	add r3, r2, #1
	lsl r3, r3, #0x1b
	lsr r3, r3, #0xd
	orr r1, r3
	str r1, [r5, r0]
	b _021E7B0C
_021E7AFA:
	cmp r2, #0
	beq _021E7B0C
	ldr r3, _021E7BB8 ; =0xFFF83FFF
	and r1, r3
	sub r3, r2, #1
	lsl r3, r3, #0x1b
	lsr r3, r3, #0xd
	orr r1, r3
	str r1, [r5, r0]
_021E7B0C:
	mov r0, #0xfd
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	lsl r1, r0, #0x1b
	lsl r0, r0, #0xd
	lsr r3, r1, #0x1b
	lsr r0, r0, #0x1b
	cmp r0, r3
	bhi _021E7B20
	add r3, r0, #0
_021E7B20:
	mov r6, #0xfd
	lsl r6, r6, #2
	ldr r1, [r5, r6]
	ldr r0, _021E7BB8 ; =0xFFF83FFF
	and r0, r1
	lsl r1, r3, #0x1b
	lsr r1, r1, #0xd
	orr r0, r1
	str r0, [r5, r6]
	ldr r0, [r5, r6]
	lsl r0, r0, #0xd
	lsr r0, r0, #0x1b
	cmp r0, r2
	beq _021E7BB4
	ldr r0, _021E7BBC ; =0x000005DC
	bl PlaySE
	cmp r4, #0
	bne _021E7B4A
	mov r2, #1
	b _021E7B4C
_021E7B4A:
	mov r2, #0
_021E7B4C:
	mov r0, #0xfd
	lsl r0, r0, #2
	ldr r3, [r5, r0]
	ldr r1, _021E7BC0 ; =0xF7FFFFFF
	lsl r2, r2, #0x1f
	and r1, r3
	lsr r2, r2, #4
	orr r1, r2
	str r1, [r5, r0]
	mov r1, #1
	ldr r2, [r5, r0]
	lsl r1, r1, #0x1c
	orr r1, r2
	str r1, [r5, r0]
	add r0, #8
	ldr r0, [r5, r0]
	mov r6, #0
	bl GridInputHandler_GetNextInput
	cmp r7, #0
	beq _021E7B80
	cmp r0, #0x1e
	bne _021E7B96
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	b _021E7B96
_021E7B80:
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r4, #0
	beq _021E7B90
	sub r0, r0, #5
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	b _021E7B96
_021E7B90:
	add r0, r0, #5
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
_021E7B96:
	mov r0, #0xff
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, r6, #0
	bl GridInputHandler_SetNextInput
	add r0, r5, #0
	add r1, r6, #0
	bl ov99_021E7C58
	ldr r0, _021E7BC4 ; =0x00000408
	mov r1, #0
	ldr r0, [r5, r0]
	bl ManagedSprite_SetDrawFlag
_021E7BB4:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E7BB8: .word 0xFFF83FFF
_021E7BBC: .word 0x000005DC
_021E7BC0: .word 0xF7FFFFFF
_021E7BC4: .word 0x00000408
	thumb_func_end ov99_021E7AD4
