	.extern ov48_022598AC
	.extern ov48_022598BC
	.include "asm/macros.inc"
	.include "overlay_48.inc"
	.include "global.inc"

.text
	thumb_func_start ov48_022599A0
ov48_022599A0: ; 0x022599A0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x58
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	add r1, sp, #4
	bl ov48_022598AC
	add r0, r4, #0
	add r1, sp, #0x10
	bl ov48_022598BC
	ldr r0, _02259A54 ; =0x0000C010
	ldr r7, _02259A58 ; =0x0000019A
	mov r6, #0
	add r4, r5, r0
_022599C0:
	ldr r1, [sp, #4]
	ldr r2, [sp, #8]
	ldr r3, [sp, #0xc]
	add r0, r4, #0
	bl sub_020182A8
	cmp r6, #2
	ldr r1, _02259A5C ; =0x0000C004
	bne _022599E6
	ldr r3, _02259A60 ; =0x0000C00C
	ldr r2, _02259A64 ; =0x0000C008
	ldr r3, [r5, r3]
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	add r0, r4, #0
	add r3, r3, r7
	bl sub_020182C4
	b _022599F6
_022599E6:
	ldr r2, _02259A64 ; =0x0000C008
	ldr r3, _02259A60 ; =0x0000C00C
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	ldr r3, [r5, r3]
	add r0, r4, #0
	bl sub_020182C4
_022599F6:
	add r6, r6, #1
	add r4, #0x78
	cmp r6, #3
	blt _022599C0
	add r0, sp, #0x34
	bl MTX_Identity33_
	mov r0, #0xc1
	lsl r0, r0, #8
	add r0, r5, r0
	add r1, sp, #0x34
	bl sub_02018288
	ldr r0, [r5]
	mov r7, #0
	cmp r0, #0
	bls _02259A50
	ldr r0, _02259A54 ; =0x0000C010
	add r6, r5, #0
	add r0, r5, r0
	add r6, #8
	add r4, r5, #0
	str r0, [sp]
_02259A24:
	add r0, r6, #0
	add r1, sp, #0x10
	add r2, sp, #0x34
	bl MTX_Concat33
	ldrh r2, [r4, #0x2c]
	cmp r2, #3
	beq _02259A44
	mov r0, #0x78
	add r1, r2, #0
	mul r1, r0
	ldr r0, [sp]
	add r0, r0, r1
	add r1, sp, #0x34
	bl sub_02018288
_02259A44:
	ldr r0, [r5]
	add r7, r7, #1
	add r6, #0x30
	add r4, #0x30
	cmp r7, r0
	blo _02259A24
_02259A50:
	add sp, #0x58
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02259A54: .word 0x0000C010
_02259A58: .word 0x0000019A
_02259A5C: .word 0x0000C004
_02259A60: .word 0x0000C00C
_02259A64: .word 0x0000C008
	thumb_func_end ov48_022599A0
