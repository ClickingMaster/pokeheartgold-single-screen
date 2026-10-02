	.extern ov72_02237B54
	.extern ov72_02237B74
	.extern ov72_02238680
	.extern ov72_0223A350
	.extern ov72_0223A444
	.include "asm/macros.inc"
	.include "overlay_72.inc"
	.include "global.inc"

.text
	thumb_func_start ov72_02239D6C
ov72_02239D6C: ; 0x02239D6C
	push {r4, lr}
	add r4, r0, #0
	bl ov72_02237B54
	cmp r0, #0
	beq _02239E34
	bl ov72_02237B74
	ldr r1, _02239E50 ; =0x00000FD4
	mov r2, #0
	str r2, [r4, r1]
	add r2, r0, #7
	cmp r2, #7
	bhi _02239E4A
	add r2, r2, r2
	add r2, pc
	ldrh r2, [r2, #6]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	add pc, r2
_02239D94: ; jump table
	.short _02239E10 - _02239D94 - 2 ; case 0
	.short _02239E00 - _02239D94 - 2 ; case 1
	.short _02239E20 - _02239D94 - 2 ; case 2
	.short _02239E10 - _02239D94 - 2 ; case 3
	.short _02239DF0 - _02239D94 - 2 ; case 4
	.short _02239DE0 - _02239D94 - 2 ; case 5
	.short _02239DD0 - _02239D94 - 2 ; case 6
	.short _02239DA4 - _02239D94 - 2 ; case 7
_02239DA4:
	add r2, r4, #0
	add r3, r4, #0
	add r2, #0x94
	add r3, #0x9c
	ldr r0, [r4]
	ldr r2, [r2]
	ldr r3, [r3]
	ldr r1, _02239E54 ; =0x000006DC
	lsl r2, r2, #0x18
	lsl r3, r3, #0x18
	ldr r0, [r0, #4]
	add r1, r4, r1
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	bl sub_0202D8A4
	add r0, r4, #0
	mov r1, #0x27
	mov r2, #0x26
	bl ov72_02238680
	b _02239E4A
_02239DD0:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239E4A
_02239DE0:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239E4A
_02239DF0:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0xb
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239E4A
_02239E00:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239E4A
_02239E10:
	mov r2, #0xb
	str r2, [r4, #0x1c]
	sub r1, #0x78
	str r0, [r4, r1]
	add r0, r4, #0
	bl ov72_0223A444
	b _02239E4A
_02239E20:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov72_0223A444
	bl sub_020399EC
	b _02239E4A
_02239E34:
	ldr r0, _02239E50 ; =0x00000FD4
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	ldr r1, [r4, r0]
	mov r0, #0xe1
	lsl r0, r0, #4
	cmp r1, r0
	bne _02239E4A
	bl sub_020399EC
_02239E4A:
	mov r0, #3
	pop {r4, pc}
	nop
_02239E50: .word 0x00000FD4
_02239E54: .word 0x000006DC
	thumb_func_end ov72_02239D6C
