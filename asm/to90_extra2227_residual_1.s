	.extern ov83_02244BA8
	.include "asm/macros.inc"
	.include "overlay_83.inc"
	.include "global.inc"

.text
	thumb_func_start ov83_022448E4
ov83_022448E4: ; 0x022448E4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldrb r0, [r5, #9]
	bl ov80_02237D8C
	cmp r0, #0
	bne _02244902
	mov r6, #0x24
	b _02244904
_02244902:
	mov r6, #4
_02244904:
	ldrb r0, [r5, #9]
	mov r1, #1
	bl ov80_02237B58
	mov r7, #0
	str r0, [sp, #0xc]
	cmp r0, #0
	ble _022449C0
	add r0, r6, #0
	str r0, [sp, #0x14]
	add r0, #0x18
	str r0, [sp, #0x14]
	add r0, r6, #0
	str r0, [sp, #0x10]
	add r0, #0x20
	str r0, [sp, #0x10]
_02244924:
	ldr r0, _022449CC ; =0x0000054C
	ldr r0, [r5, r0]
	ldrb r0, [r0, r7]
	cmp r0, #0
	bne _02244946
	mov r0, #0x40
	str r0, [sp]
	mov r0, #0x10
	lsl r2, r6, #0x10
	str r0, [sp, #4]
	add r0, r4, #0
	mov r1, #0
	lsr r2, r2, #0x10
	mov r3, #1
	bl FillWindowPixelRect
	b _022449AA
_02244946:
	ldr r0, _022449D0 ; =0x0000055C
	add r1, r7, #0
	ldr r0, [r5, r0]
	bl Party_GetMonByIndex
	mov r1, #0xa3
	mov r2, #0
	str r0, [sp, #0x18]
	bl GetMonData
	str r4, [sp]
	add r1, r0, #0
	str r6, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0xad
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r2, #3
	mov r3, #1
	bl PrintUIntOnWindow
	mov r0, #1
	str r0, [sp]
	mov r0, #0xad
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r3, [sp, #0x14]
	mov r1, #0
	add r2, r4, #0
	bl sub_0200CDAC
	ldr r0, [sp, #0x18]
	mov r1, #0xa4
	mov r2, #0
	bl GetMonData
	add r1, r0, #0
	ldr r0, [sp, #0x10]
	str r4, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0xad
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r2, #3
	mov r3, #0
	bl PrintUIntOnWindow
_022449AA:
	ldr r0, [sp, #0x14]
	add r7, r7, #1
	add r0, #0x40
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x10]
	add r6, #0x40
	add r0, #0x40
	str r0, [sp, #0x10]
	ldr r0, [sp, #0xc]
	cmp r7, r0
	blt _02244924
_022449C0:
	add r0, r4, #0
	bl ScheduleWindowCopyToVram
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_022449CC: .word 0x0000054C
_022449D0: .word 0x0000055C
	thumb_func_end ov83_022448E4
