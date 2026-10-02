	.extern ov47_0225999C
	.include "asm/macros.inc"
	.include "overlay_47.inc"
	.include "global.inc"
	.include "overlay47_exact_bank.inc"

.text
	thumb_func_start ov47_02259B74
ov47_02259B74: ; 0x02259B74
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	add r6, r0, #0
	ldr r0, [r6, #0x64]
	cmp r0, #0x14
	blt _02259B86
	add sp, #0x2c
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_02259B86:
	add r1, r0, #1
	mov r0, #0x50
	str r1, [r6, #0x64]
	mul r0, r1
	mov r1, #0x14
	bl _s32_div_f
	str r0, [sp, #0x24]
	mov r0, #0
	str r0, [sp, #0x1c]
	str r0, [sp, #0x20]
	str r6, [sp, #0x18]
_02259B9E:
	ldr r0, [sp, #0x18]
	ldr r2, [r0, #0x74]
	ldr r0, [r0, #0x68]
	cmp r0, r2
	bne _02259BB0
	ldr r0, [sp, #0x1c]
	add r0, r0, #1
	str r0, [sp, #0x1c]
	b _02259C18
_02259BB0:
	ldr r1, [sp, #0x24]
	cmp r2, r1
	blt _02259BBE
	sub r4, r1, r0
	add r2, r1, #0
	ldr r1, [sp, #0x18]
	b _02259BC2
_02259BBE:
	ldr r1, [sp, #0x18]
	sub r4, r2, r0
_02259BC2:
	mov r5, #0
	str r2, [r1, #0x68]
	cmp r4, #0
	ble _02259C0C
	ldr r1, [sp, #0x20]
	add r0, r0, #4
	add r1, r1, #2
	lsl r7, r1, #4
	str r0, [sp, #0x28]
_02259BD4:
	mov r0, #1
	lsl r0, r0, #8
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	ldr r0, [sp, #0x28]
	add r1, r6, #0
	add r0, r0, r5
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	mov r0, #1
	str r0, [sp, #0x10]
	mov r0, #0x10
	str r0, [sp, #0x14]
	add r1, #0x84
	ldr r1, [r1]
	add r0, r6, r7
	ldr r1, [r1, #0x14]
	mov r2, #0xf7
	mov r3, #0x10
	bl BlitBitmapRectToWindow
	add r5, r5, #1
	cmp r5, r4
	blt _02259BD4
_02259C0C:
	ldr r0, [sp, #0x20]
	add r0, r0, #2
	lsl r0, r0, #4
	add r0, r6, r0
	bl ScheduleWindowCopyToVram
_02259C18:
	ldr r0, [sp, #0x18]
	add r0, r0, #4
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x20]
	add r0, r0, #1
	str r0, [sp, #0x20]
	cmp r0, #3
	blt _02259B9E
	ldr r0, [sp, #0x1c]
	cmp r0, #3
	bne _02259C34
	add sp, #0x2c
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_02259C34:
	mov r0, #0
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov47_02259B74
