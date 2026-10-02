	.extern ov72_02237B54
	.extern ov72_02237B74
	.extern ov72_02237C30
	.extern ov72_02238680
	.extern ov72_0223A444
	.include "asm/macros.inc"
	.include "overlay_72.inc"
	.include "global.inc"

.text
	thumb_func_start ov72_02239A88
ov72_02239A88: ; 0x02239A88
	push {r4, lr}
	add r4, r0, #0
	bl ov72_02237B54
	cmp r0, #0
	beq _02239B3C
	bl ov72_02237B74
	ldr r1, _02239B58 ; =0x00000FD4
	mov r2, #0
	str r2, [r4, r1]
	add r2, r0, #7
	cmp r2, #7
	bhi _02239B52
	add r2, r2, r2
	add r2, pc
	ldrh r2, [r2, #6]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	add pc, r2
_02239AB0: ; jump table
	.short _02239B18 - _02239AB0 - 2 ; case 0
	.short _02239B08 - _02239AB0 - 2 ; case 1
	.short _02239B28 - _02239AB0 - 2 ; case 2
	.short _02239B18 - _02239AB0 - 2 ; case 3
	.short _02239AF8 - _02239AB0 - 2 ; case 4
	.short _02239AE8 - _02239AB0 - 2 ; case 5
	.short _02239AD8 - _02239AB0 - 2 ; case 6
	.short _02239AC0 - _02239AB0 - 2 ; case 7
_02239AC0:
	ldr r0, [r4]
	mov r1, #5
	ldr r0, [r0]
	mov r2, #2
	bl sub_0202D5DC
	add r0, r4, #0
	mov r1, #0x27
	mov r2, #0x1c
	bl ov72_02238680
	b _02239B52
_02239AD8:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239B52
_02239AE8:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239B52
_02239AF8:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239B52
_02239B08:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239B52
_02239B18:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239B52
_02239B28:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	bl sub_020399EC
	b _02239B52
_02239B3C:
	ldr r0, _02239B58 ; =0x00000FD4
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	ldr r1, [r4, r0]
	mov r0, #0xe1
	lsl r0, r0, #4
	cmp r1, r0
	bne _02239B52
	bl sub_020399EC
_02239B52:
	mov r0, #3
	pop {r4, pc}
	nop
_02239B58: .word 0x00000FD4
	thumb_func_end ov72_02239A88
