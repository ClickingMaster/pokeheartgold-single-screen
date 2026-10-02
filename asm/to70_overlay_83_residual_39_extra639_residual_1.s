	.extern ov83_0224755C
	.extern ov83_02247668
	.extern ov83_02247768
	.include "asm/macros.inc"
	.include "overlay_83.inc"
	.include "global.inc"

.text
	thumb_func_start ov83_02246938
ov83_02246938: ; 0x02246938
	push {r4, r5, r6, lr}
	add r6, r0, #0
	cmp r1, #1
	bne _02246962
	mov r0, #0x11
	lsl r0, r0, #4
	mov r4, #0xc
	add r5, r6, r0
_02246948:
	add r0, r5, #0
	bl ScheduleWindowCopyToVram
	add r4, r4, #1
	add r5, #0x10
	cmp r4, #0x22
	bls _02246948
	ldr r0, _02246984 ; =0x00000544
	mov r1, #1
	ldr r0, [r6, r0]
	bl ov83_0224755C
	pop {r4, r5, r6, pc}
_02246962:
	mov r0, #0x11
	lsl r0, r0, #4
	mov r5, #0xc
	add r4, r6, r0
_0224696A:
	add r0, r4, #0
	bl ClearWindowTilemapAndScheduleTransfer
	add r5, r5, #1
	add r4, #0x10
	cmp r5, #0x22
	bls _0224696A
	ldr r0, _02246984 ; =0x00000544
	mov r1, #0
	ldr r0, [r6, r0]
	bl ov83_0224755C
	pop {r4, r5, r6, pc}
	.balign 4, 0
_02246984: .word 0x00000544
	thumb_func_end ov83_02246938
