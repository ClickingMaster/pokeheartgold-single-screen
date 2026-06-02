	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start ov03_022542C8
ov03_022542C8: ; 0x022542C8
	push {r4, lr}
	sub sp, #0x68
	add r4, r0, #0
	ldr r0, _02254398 ; =ov03_022598A0
	ldr r0, [r0]
	add r0, #0x30
	bl WindowIsInUse
	cmp r0, #0
	bne _02254302
	ldr r0, _02254398 ; =ov03_022598A0
	mov r2, #3
	ldr r1, [r0]
	mov r0, #2
	str r0, [sp]
	mov r0, #8
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0xc9
	str r0, [sp, #0x10]
	ldr r0, [r1, #0x68]
	add r1, #0x30
	ldr r0, [r0, #8]
	mov r3, #0x17
	bl AddWindowParameterized
_02254302:
	ldr r0, _02254398 ; =ov03_022598A0
	ldr r2, _0225439C ; =0x000003D9
	ldr r0, [r0]
	mov r1, #1
	add r0, #0x30
	mov r3, #0xb
	bl DrawFrameAndWindow1
	ldr r0, _02254398 ; =ov03_022598A0
	mov r1, #0xf
	ldr r0, [r0]
	add r0, #0x30
	bl FillWindowPixelBuffer
	ldr r0, _02254398 ; =ov03_022598A0
	ldr r0, [r0]
	add r0, #0x30
	bl CopyWindowToVram
	add r0, r4, #0
	mov r1, #1
	add r0, #0x97
	strb r1, [r0]
	ldr r0, _022543A0 ; =ov03_02254D64
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	ldr r0, _02254398 ; =ov03_022598A0
	ldr r0, [r0]
	bl ov03_022543AC
	ldr r3, _022543A4 ; =ov03_0225927A
	add r2, sp, #0x14
	mov r1, #0x29
_02254348:
	ldrh r0, [r3]
	add r3, r3, #2
	strh r0, [r2]
	add r2, r2, #2
	sub r1, r1, #1
	bne _02254348
	ldr r0, _02254398 ; =ov03_022598A0
	ldr r1, [r0]
	add r0, r1, #0
	add r0, #0x91
	ldrb r0, [r0]
	cmp r0, #8
	bne _02254370
	add r1, #0x92
	ldrb r0, [r1]
	mov r1, #0
	add r0, #0x7a
	bl ov03_02253E20
	b _0225438E
_02254370:
	cmp r0, #0x28
	bne _02254382
	add r1, #0x92
	ldrb r0, [r1]
	mov r1, #0
	add r0, #0x94
	bl ov03_02253E20
	b _0225438E
_02254382:
	lsl r1, r0, #1
	add r0, sp, #0x14
	ldrh r0, [r0, r1]
	mov r1, #0
	bl ov03_02253E20
_0225438E:
	ldr r0, _022543A8 ; =ov03_02254420
	bl ov03_02254B4C
	add sp, #0x68
	pop {r4, pc}
	.balign 4, 0
_02254398: .word ov03_022598A0
_0225439C: .word 0x000003D9
_022543A0: .word ov03_02254D64
_022543A4: .word ov03_0225927A
_022543A8: .word ov03_02254420
	thumb_func_end ov03_022542C8