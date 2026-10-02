	.extern ov92_02260428
	.extern ov92_022630E8
	.include "asm/macros.inc"
	.include "overlay_92.inc"
	.include "global.inc"
	.public ov92_0225EBE0

.text
	thumb_func_start ov92_0225EBE0
ov92_0225EBE0: ; 0x0225EBE0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	ldr r2, _0225ED44 ; =0x00001FC8
	add r7, r0, #0
	mov r0, #0
	mov r3, #0x19
	str r0, [r7, r2]
	add r1, r2, #4
	lsl r3, r3, #0xe
	str r3, [r7, r1]
	add r2, #8
	mov r3, #0x31
	ldr r1, _0225ED48 ; =0x0000051C
	str r0, [r7, r2]
	str r0, [r7, r1]
	add r2, r1, #4
	str r0, [r7, r2]
	lsl r3, r3, #4
	str r0, [r7, r3]
	add r2, r3, #4
	str r0, [r7, r2]
	sub r1, r1, #4
	str r0, [r7, r1]
	sub r1, r3, #4
	sub r3, #0x6c
	str r0, [r7, r1]
	add r0, r7, r3
	bl ov92_022630E8
	mov r0, #0xad
	lsl r0, r0, #2
	add r0, r7, r0
	bl ov92_022630E8
	mov r0, #0x4b
	lsl r0, r0, #4
	add r0, r7, r0
	bl ov92_022630E8
	mov r0, #0x13
	lsl r0, r0, #6
	add r0, r7, r0
	bl ov92_022630E8
	ldr r0, _0225ED4C ; =0x00000AD4
	add r0, r7, r0
	bl ov92_022630E8
	ldr r0, _0225ED50 ; =0x00000AE4
	add r0, r7, r0
	bl ov92_022630E8
	mov r0, #0
	str r0, [sp, #0xc]
	mov r0, #0xce
	lsl r0, r0, #4
	add r6, r7, r0
	add r0, #0x10
	add r4, r7, r0
	mov r0, #0xb5
	lsl r0, r0, #4
	add r5, r7, r0
_0225EC5C:
	add r0, r6, #0
	bl ov92_022630E8
	add r0, r4, #0
	bl ov92_022630E8
	mov r0, #5
	str r0, [sp]
	ldr r0, _0225ED54 ; =0x3F4CCCCD
	mov r1, #0
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	add r0, r5, #0
	add r2, r1, #0
	mov r3, #5
	bl ov92_02260428
	mov r1, #0
	mov r0, #4
	mvn r0, r0
	str r0, [sp]
	ldr r0, _0225ED54 ; =0x3F4CCCCD
	add r2, r1, #0
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	add r0, r5, #0
	sub r3, r1, #5
	bl ov92_02260428
	mov r0, #0x83
	lsl r0, r0, #2
	add r6, r6, r0
	add r4, r4, r0
	add r5, r5, r0
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
	cmp r0, #8
	blt _0225EC5C
	mov r3, #5
	ldr r0, _0225ED58 ; =0x3FF0A3D7
	str r3, [sp]
	str r0, [sp, #4]
	mov r1, #0
	mov r0, #0x45
	lsl r0, r0, #2
	add r0, r7, r0
	add r2, r1, #0
	str r1, [sp, #8]
	bl ov92_02260428
	mov r3, #4
	mvn r3, r3
	mov r1, #0
	ldr r0, _0225ED58 ; =0x3FF0A3D7
	str r3, [sp]
	str r0, [sp, #4]
	mov r0, #0x45
	lsl r0, r0, #2
	add r0, r7, r0
	add r2, r1, #0
	str r1, [sp, #8]
	bl ov92_02260428
	mov r3, #5
	mov r1, #0
	ldr r0, _0225ED54 ; =0x3F4CCCCD
	str r3, [sp]
	str r0, [sp, #4]
	mov r0, #0x32
	lsl r0, r0, #4
	add r0, r7, r0
	add r2, r1, #0
	str r1, [sp, #8]
	bl ov92_02260428
	mov r3, #4
	mvn r3, r3
	mov r1, #0
	ldr r0, _0225ED54 ; =0x3F4CCCCD
	str r3, [sp]
	str r0, [sp, #4]
	mov r0, #0x32
	lsl r0, r0, #4
	add r0, r7, r0
	add r2, r1, #0
	str r1, [sp, #8]
	bl ov92_02260428
	mov r3, #5
	mov r1, #0
	ldr r0, _0225ED58 ; =0x3FF0A3D7
	str r3, [sp]
	str r0, [sp, #4]
	ldr r0, _0225ED5C ; =0x00000944
	add r2, r1, #0
	add r0, r7, r0
	str r1, [sp, #8]
	bl ov92_02260428
	mov r3, #4
	mvn r3, r3
	mov r1, #0
	ldr r0, _0225ED54 ; =0x3F4CCCCD
	str r3, [sp]
	str r0, [sp, #4]
	ldr r0, _0225ED5C ; =0x00000944
	add r2, r1, #0
	add r0, r7, r0
	str r1, [sp, #8]
	bl ov92_02260428
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225ED44: .word 0x00001FC8
_0225ED48: .word 0x0000051C
_0225ED4C: .word 0x00000AD4
_0225ED50: .word 0x00000AE4
_0225ED54: .word 0x3F4CCCCD
_0225ED58: .word 0x3FF0A3D7
_0225ED5C: .word 0x00000944
	thumb_func_end ov92_0225EBE0
