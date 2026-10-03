	.include "asm/macros.inc"
	.include "unk_02015DD8.inc"
	.include "global.inc"

.text
	thumb_func_start sub_0201630C
sub_0201630C: ; 0x0201630C
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	ldr r1, [r5, #4]
	add r7, r0, #0
	ldr r2, [r1, #0x10]
	mov r6, #8
	mov r0, #0
	cmp r2, #0
	ble _0201632A
_0201631E:
	ldr r2, [r5, #4]
	add r0, r0, #1
	ldr r2, [r2, #0x10]
	lsl r6, r6, #1
	cmp r0, r2
	blt _0201631E
_0201632A:
	ldr r1, [r1, #0x14]
	mov r4, #8
	mov r0, #0
	cmp r1, #0
	ble _02016340
_02016334:
	ldr r1, [r5, #4]
	add r0, r0, #1
	ldr r1, [r1, #0x14]
	lsl r4, r4, #1
	cmp r0, r1
	blt _02016334
_02016340:
	mov r1, #0xc
	mov r2, #0xe
	ldrsh r1, [r5, r1]
	ldrsh r2, [r5, r2]
	add r0, r7, #0
	bl sub_02015FC4
	lsr r1, r6, #0x1f
	lsr r2, r4, #0x1f
	add r1, r6, r1
	add r2, r4, r2
	lsl r1, r1, #0xf
	lsl r2, r2, #0xf
	add r0, r7, #0
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	bl sub_02015FD8
	mov r0, #1
	lsl r0, r0, #0xc
	str r0, [r7, #0xc]
	str r0, [r7, #0x10]
	add r0, r7, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_02015FE0
	ldr r1, [r5, #0x18]
	add r0, r7, #0
	bl sub_02015FF4
	ldr r1, [r5, #0x14]
	add r0, r7, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl sub_02015FFC
	ldr r1, [r5, #4]
	add r0, r7, #0
	add r1, #0x10
	bl sub_02016004
	ldr r0, [r5, #4]
	mov r1, #0
	add r0, r0, #4
	bl NNS_G2dGetImageLocation
	add r1, r0, #0
	add r0, r7, #0
	bl sub_02016008
	ldr r0, [r5, #8]
	mov r1, #0
	add r0, r0, #4
	bl NNS_G2dGetImagePaletteLocation
	add r1, r0, #0
	add r0, r7, #0
	bl sub_0201600C
	ldrh r1, [r5, #0x1c]
	add r0, r7, #0
	bl sub_02016010
	mov r1, #0
	add r0, r7, #0
	add r2, r1, #0
	bl sub_02016014
	add r0, r7, #0
	mov r1, #1
	mov r2, #0
	bl sub_02016014
	ldrh r1, [r5, #0x10]
	add r0, r7, #0
	bl sub_02016020
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end sub_0201630C
