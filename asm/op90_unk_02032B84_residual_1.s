	.include "asm/macros.inc"
	.include "unk_02032844.inc"
	.include "global.inc"
	.public sub_02032D4C
	.extern sub_02032C84

.text
	thumb_func_start sub_02032D4C
sub_02032D4C: ; 0x02032D4C
	push {r4, lr}
	add r4, r0, #0
	ldrh r0, [r4, #2]
	cmp r0, #0
	beq _02032D62
	bl sub_02032858
	mov r0, #9
	bl sub_02032844
	pop {r4, pc}
_02032D62:
	ldr r0, _02032E14 ; =_021D4124
	ldr r2, [r0, #4]
	ldr r0, _02032E18 ; =0x00001310
	ldr r1, [r2, r0]
	cmp r1, #2
	beq _02032D84
	mov r1, #0
	sub r0, r0, #2
	strh r1, [r2, r0]
	bl sub_02032E48
	cmp r0, #0
	bne _02032E10
	mov r0, #9
	bl sub_02032844
	pop {r4, pc}
_02032D84:
	ldrh r1, [r4, #8]
	cmp r1, #3
	beq _02032E10
	cmp r1, #4
	beq _02032E02
	cmp r1, #5
	bne _02032E02
	sub r0, #0xf0
	add r0, r2, r0
	mov r1, #0xc0
	bl DC_InvalidateRange
	ldr r0, _02032E14 ; =_021D4124
	ldr r1, [r0, #4]
	ldr r0, _02032E1C ; =0x00001320
	ldr r2, [r1, r0]
	cmp r2, #0
	beq _02032DB8
	ldrh r0, [r4, #0x36]
	cmp r0, #8
	blo _02032DB8
	add r1, r4, #0
	add r1, #0x48
	ldrb r1, [r1, #4]
	ldr r0, [r4, #0x3c]
	blx r2
_02032DB8:
	ldrh r0, [r4, #0x36]
	cmp r0, #8
	blo _02032E02
	ldr r0, _02032E14 ; =_021D4124
	ldr r2, [r4, #0x3c]
	ldr r0, [r0, #4]
	ldr r1, [r0, #8]
	cmp r2, r1
	bne _02032E02
	add r4, #0x43
	ldrb r2, [r4]
	mov r1, #3
	and r1, r2
	cmp r1, #1
	bne _02032E02
	mov r1, #0x13
	lsl r1, r1, #8
	ldr r2, [r0, r1]
	cmp r2, #0
	beq _02032DE6
	sub r1, #0xe0
	add r0, r0, r1
	blx r2
_02032DE6:
	ldr r0, _02032E14 ; =_021D4124
	ldr r1, [r0, #4]
	ldr r0, _02032E20 ; =0x0000130E
	ldrh r0, [r1, r0]
	cmp r0, #0
	beq _02032E02
	bl sub_02032E48
	cmp r0, #0
	bne _02032E10
	mov r0, #9
	bl sub_02032844
	pop {r4, pc}
_02032E02:
	bl sub_02032C84
	cmp r0, #0
	bne _02032E10
	mov r0, #9
	bl sub_02032844
_02032E10:
	pop {r4, pc}
	nop
_02032E14: .word _021D4124
_02032E18: .word 0x00001310
_02032E1C: .word 0x00001320
_02032E20: .word 0x0000130E
	thumb_func_end sub_02032D4C
