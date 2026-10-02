	.include "asm/macros.inc"
	.include "overlay_80_0222FD08.inc"
	.include "global.inc"

.text
	thumb_func_start ov80_022308C4
ov80_022308C4: ; 0x022308C4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x4c
	mov r1, #0
	add r5, r0, #0
	add r2, sp, #0x3c
	add r3, sp, #0x2c
	add r0, r1, #0
_022308D2:
	add r1, r1, #1
	strh r0, [r2]
	strh r0, [r3]
	add r2, r2, #2
	add r3, r3, #2
	cmp r1, #8
	blt _022308D2
	ldrb r0, [r5, #4]
	mov r1, #1
	bl ov80_02236DF8
	str r0, [sp, #0x1c]
	ldr r0, _022309EC ; =0x000004D4
	ldr r0, [r5, r0]
	bl Party_GetCount
	mov r7, #0
	str r0, [sp, #0x20]
	cmp r0, #0
	ble _0223092C
	add r6, sp, #0x3c
	add r4, sp, #0x2c
_022308FE:
	ldr r0, _022309EC ; =0x000004D4
	add r1, r7, #0
	ldr r0, [r5, r0]
	bl Party_GetMonByIndex
	str r0, [sp, #0x24]
	mov r1, #5
	mov r2, #0
	bl GetMonData
	strh r0, [r6]
	ldr r0, [sp, #0x24]
	mov r1, #6
	mov r2, #0
	bl GetMonData
	strh r0, [r4]
	ldr r0, [sp, #0x20]
	add r7, r7, #1
	add r6, r6, #2
	add r4, r4, #2
	cmp r7, r0
	blt _022308FE
_0223092C:
	ldr r0, _022309F0 ; =0x000004D8
	ldr r0, [r5, r0]
	bl Party_GetCount
	mov r7, #0
	str r0, [sp, #0x14]
	cmp r0, #0
	ble _0223098A
	ldr r0, [sp, #0x20]
	str r5, [sp, #0x18]
	lsl r1, r0, #1
	add r0, sp, #0x3c
	add r6, r0, r1
	add r0, sp, #0x2c
	add r4, r0, r1
_0223094A:
	ldr r0, _022309F0 ; =0x000004D8
	add r1, r7, #0
	ldr r0, [r5, r0]
	bl Party_GetMonByIndex
	str r0, [sp, #0x28]
	mov r1, #5
	mov r2, #0
	bl GetMonData
	strh r0, [r6]
	ldr r0, [sp, #0x28]
	mov r1, #6
	mov r2, #0
	bl GetMonData
	strh r0, [r4]
	ldr r1, [sp, #0x18]
	ldr r0, _022309F4 ; =0x000003D2
	add r7, r7, #1
	ldrh r2, [r1, r0]
	mov r0, #0x95
	lsl r0, r0, #2
	strh r2, [r1, r0]
	add r0, r1, #0
	add r0, r0, #2
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x14]
	add r6, r6, #2
	add r4, r4, #2
	cmp r7, r0
	blt _0223094A
_0223098A:
	ldrb r0, [r5, #6]
	ldrb r1, [r5, #5]
	lsl r0, r0, #1
	add r0, r5, r0
	ldrh r0, [r0, #0x18]
	bl ov80_02236C2C
	ldr r1, _022309F4 ; =0x000003D2
	ldr r3, [sp, #0x20]
	add r2, r5, r1
	str r2, [sp]
	mov r2, #0xb
	str r2, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0
	ldr r2, [sp, #0x14]
	add r1, #8
	str r0, [sp, #0xc]
	add r0, r5, r1
	str r0, [sp, #0x10]
	add r2, r3, r2
	ldr r3, [sp, #0x1c]
	add r0, sp, #0x3c
	add r1, sp, #0x2c
	bl ov80_02236C9C
	mov r2, #0x3e
	lsl r2, r2, #4
	add r0, r5, r2
	str r0, [sp]
	ldr r0, [sp, #0x1c]
	add r1, r2, #0
	str r0, [sp, #4]
	mov r0, #0xb
	str r0, [sp, #8]
	mov r0, #0xcd
	str r0, [sp, #0xc]
	add r0, r2, #0
	add r0, #0x10
	sub r1, #0xe
	sub r2, r2, #6
	add r0, r5, r0
	add r1, r5, r1
	add r2, r5, r2
	mov r3, #0
	bl ov80_0222A52C
	add sp, #0x4c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_022309EC: .word 0x000004D4
_022309F0: .word 0x000004D8
_022309F4: .word 0x000003D2
	thumb_func_end ov80_022308C4
