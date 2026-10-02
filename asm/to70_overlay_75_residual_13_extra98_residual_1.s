	.extern ov75_02247854
	.extern ov75_02247878
	.extern ov75_02247890
	.extern ov75_022494CC
	.extern ov75_02249534
	.include "asm/macros.inc"
	.include "overlay_75.inc"
	.include "global.inc"

.text
	thumb_func_start ov75_02248A20
ov75_02248A20: ; 0x02248A20
	push {r3, r4, r5, lr}
	add r4, r0, #0
	bl ov70_02237F38
	cmp r0, #0
	bne _02248A2E
	b _02248B56
_02248A2E:
	bl ov70_02237F58
	add r5, r0, #0
	add r0, r4, #0
	mov r1, #0
	add r0, #0xe8
	str r1, [r0]
	add r0, r5, #0
	add r0, #0xf
	cmp r0, #0x11
	bls _02248A46
	b _02248B4A
_02248A46:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02248A52: ; jump table
	.short _02248B2E - _02248A52 - 2 ; case 0
	.short _02248B3C - _02248A52 - 2 ; case 1
	.short _02248B4A - _02248A52 - 2 ; case 2
	.short _02248B2E - _02248A52 - 2 ; case 3
	.short _02248B4A - _02248A52 - 2 ; case 4
	.short _02248B4A - _02248A52 - 2 ; case 5
	.short _02248B4A - _02248A52 - 2 ; case 6
	.short _02248B4A - _02248A52 - 2 ; case 7
	.short _02248B4A - _02248A52 - 2 ; case 8
	.short _02248B4A - _02248A52 - 2 ; case 9
	.short _02248B4A - _02248A52 - 2 ; case 10
	.short _02248B4A - _02248A52 - 2 ; case 11
	.short _02248B4A - _02248A52 - 2 ; case 12
	.short _02248B3C - _02248A52 - 2 ; case 13
	.short _02248B20 - _02248A52 - 2 ; case 14
	.short _02248A76 - _02248A52 - 2 ; case 15
	.short _02248B12 - _02248A52 - 2 ; case 16
	.short _02248B20 - _02248A52 - 2 ; case 17
_02248A76:
	ldr r1, [r4]
	add r0, r1, #0
	add r0, #0xfc
	ldr r0, [r0]
	cmp r0, #0
	beq _02248A8C
	cmp r0, #1
	beq _02248AE6
	cmp r0, #2
	beq _02248AF6
	b _02248B06
_02248A8C:
	mov r0, #1
	lsl r0, r0, #8
	ldr r0, [r1, r0]
	cmp r0, #3
	bhi _02248AE0
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02248AA2: ; jump table
	.short _02248AC0 - _02248AA2 - 2 ; case 0
	.short _02248AAA - _02248AA2 - 2 ; case 1
	.short _02248AB0 - _02248AA2 - 2 ; case 2
	.short _02248AD0 - _02248AA2 - 2 ; case 3
_02248AAA:
	mov r0, #0x13
	str r0, [r4, #8]
	b _02248B74
_02248AB0:
	add r0, r4, #0
	bl ov75_02247878
	ldr r0, _02248B78 ; =0xFFFFEC77
	str r0, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248AC0:
	add r0, r4, #0
	bl ov75_02247878
	ldr r0, _02248B7C ; =0xFFFFEC76
	str r0, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248AD0:
	add r0, r4, #0
	bl ov75_02247878
	ldr r0, _02248B80 ; =0xFFFFEC75
	str r0, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248AE0:
	bl sub_020399EC
	b _02248B74
_02248AE6:
	add r0, r4, #0
	bl ov75_02247878
	ldr r0, _02248B84 ; =0xFFFFEC74
	str r0, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248AF6:
	add r0, r4, #0
	bl ov75_02247878
	ldr r0, _02248B88 ; =0xFFFFEC73
	str r0, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248B06:
	add r0, r4, #0
	bl ov75_02247878
	bl sub_020399EC
	b _02248B74
_02248B12:
	add r0, r4, #0
	bl ov75_02247878
	str r5, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248B20:
	add r0, r4, #0
	bl ov75_02247878
	str r5, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248B2E:
	add r0, r4, #0
	bl ov75_02247878
	str r5, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248B3C:
	add r0, r4, #0
	bl ov75_02247878
	str r5, [r4, #0x10]
	mov r0, #0x20
	str r0, [r4, #8]
	b _02248B74
_02248B4A:
	add r0, r4, #0
	bl ov75_02247878
	bl sub_020399EC
	b _02248B74
_02248B56:
	add r0, r4, #0
	add r0, #0xe8
	ldr r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0xe8
	str r1, [r0]
	add r4, #0xe8
	mov r0, #0xe1
	ldr r1, [r4]
	lsl r0, r0, #4
	cmp r1, r0
	bne _02248B74
	bl sub_020399EC
_02248B74:
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02248B78: .word 0xFFFFEC77
_02248B7C: .word 0xFFFFEC76
_02248B80: .word 0xFFFFEC75
_02248B84: .word 0xFFFFEC74
_02248B88: .word 0xFFFFEC73
	thumb_func_end ov75_02248A20
