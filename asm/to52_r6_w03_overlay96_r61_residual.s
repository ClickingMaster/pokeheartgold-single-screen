	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.text

	.public ov96_021F7620
	.public ov96_021F7684

	thumb_func_start ov96_021F7620
ov96_021F7620: ; 0x021F7620
	push {r4, r5, r6, r7, lr}
	sub sp, #0x74
	mov r4, #0
	add r3, r4, #0
	add r1, sp, #0
_021F762A:
	ldr r2, [r0]
	cmp r2, #0
	bne _021F7636
	lsl r2, r4, #2
	add r4, r4, #1
	str r0, [r1, r2]
_021F7636:
	add r3, r3, #1
	add r0, #0x1c
	cmp r3, #0x1d
	blt _021F762A
	lsl r1, r4, #2
	add r0, sp, #0
	mov r7, #0
	add r5, r0, r1
_021F7646:
	cmp r4, #0
	ble _021F767E
	bl LCRandom
	add r1, r4, #0
	bl _s32_div_f
	lsl r0, r1, #0x18
	lsr r6, r0, #0x16
	add r0, sp, #0
	ldr r1, [r0, r6]
	mov r0, #1
	str r0, [r1]
	add r0, sp, #0
	ldr r0, [r0, r6]
	mov r1, #7
	ldr r0, [r0, #0x10]
	bl ov96_021EB564
	sub r0, r5, #4
	ldr r1, [r0]
	add r0, sp, #0
	add r7, r7, #1
	str r1, [r0, r6]
	sub r5, r5, #4
	sub r4, r4, #1
	cmp r7, #0x19
	blt _021F7646
_021F767E:
	add sp, #0x74
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021F7620


	thumb_func_start ov96_021F7684
ov96_021F7684: ; 0x021F7684
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r1, #0
	ldr r1, [sp, #0x28]
	add r4, r3, #0
	str r1, [sp, #0x28]
	mov r1, #0x24
	bl Heap_Alloc
	str r0, [sp, #8]
	mov r1, #0
	mov r2, #0x24
	bl MI_CpuFill8
	ldr r0, [sp, #8]
	strb r5, [r0, #0x1c]
	mov r0, #4
	sub r1, r0, r5
	ldr r0, [sp, #8]
	add r0, #0x21
	strb r1, [r0]
	ldr r0, [sp, #8]
	add r0, #0x21
	ldrb r1, [r0]
	ldr r0, [sp, #8]
	add r0, #0x20
	strb r1, [r0]
	ldr r0, [sp, #8]
	mov r1, #3
	strb r1, [r0, #0x1f]
	str r4, [r0, #4]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [sp, #8]
	ldrb r0, [r0, #0x1c]
	cmp r0, #0
	ble _021F7730
_021F76CE:
	ldr r0, [sp, #8]
	mov r4, #0
	add r0, #0x20
	ldrb r1, [r0]
	ldr r0, [sp, #0xc]
	add r0, r1, r0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r1, r0, #1
	str r0, [sp, #4]
	add r1, r0, r1
	ldr r0, [sp, #8]
	add r5, r0, r1
	add r1, r0, #0
	ldr r0, [sp, #4]
	add r0, r1, r0
	add r6, r0, #0
	add r7, r0, #0
	str r0, [sp]
	add r6, #8
	add r7, #0xc
_021F76F8:
	ldr r0, [sp, #0x28]
	ldr r1, [sp, #4]
	add r2, r4, #0
	bl ov96_021E60D8
	ldrb r2, [r6]
	ldrb r1, [r0, #2]
	add r4, r4, #1
	add r1, r2, r1
	strb r1, [r6]
	ldrb r1, [r7]
	ldrb r0, [r0, #4]
	add r0, r1, r0
	strb r0, [r7]
	ldr r0, [sp]
	ldrb r0, [r0, #8]
	strb r0, [r5, #0x10]
	add r5, r5, #1
	cmp r4, #3
	blt _021F76F8
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
	ldr r0, [sp, #8]
	ldrb r1, [r0, #0x1c]
	ldr r0, [sp, #0xc]
	cmp r0, r1
	blt _021F76CE
_021F7730:
	ldr r0, [sp, #8]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021F7684
