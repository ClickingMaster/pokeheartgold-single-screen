	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start ov03_022558C4
ov03_022558C4: ; 0x022558C4
	push {r4, r5, lr}
	sub sp, #0x54
	add r5, r0, #0
	add r4, r1, #0
	bl sub_0203769C
	cmp r0, #0
	bne _022558EA
	bl sub_02037454
	add r1, r4, #0
	add r1, #0x90
	ldrb r1, [r1]
	cmp r1, r0
	beq _022558EA
	ldr r0, _022559F4 ; =ov03_02255A00
	bl ov03_02254B4C
	b _0225597A
_022558EA:
	bl ov03_02255C80
	cmp r0, #0
	bne _022558FA
	bl sub_02037700
	cmp r0, #0
	beq _02255904
_022558FA:
	ldr r0, _022559F4 ; =ov03_02255A00
	bl ov03_02254B4C
	add sp, #0x54
	pop {r4, r5, pc}
_02255904:
	mov r0, #0
	bl sub_020373B4
	cmp r0, #0
	bne _02255918
	ldr r0, _022559F4 ; =ov03_02255A00
	bl ov03_02254B4C
	add sp, #0x54
	pop {r4, r5, pc}
_02255918:
	mov r0, #0
	mov r1, #5
	bl sub_02037BA0
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _02255932
	ldr r0, _022559F4 ; =ov03_02255A00
	bl ov03_02254B4C
	add sp, #0x54
	pop {r4, r5, pc}
_02255932:
	mov r0, #0xa
	bl sub_02037B38
	cmp r0, #0
	beq _0225597A
	bl sub_02037A10
	cmp r0, #0
	beq _0225597A
	ldr r0, _022559F8 ; =ov03_022598A0
	ldr r0, [r0]
	add r0, #0x94
	ldrb r0, [r0]
	bl IsPrintFinished
	cmp r0, #0
	bne _02255960
	ldr r0, _022559F8 ; =ov03_022598A0
	ldr r0, [r0]
	add r0, #0x94
	ldrb r0, [r0]
	bl RemoveTextPrinter
_02255960:
	bl sub_02037454
	add r1, r4, #0
	add r1, #0x90
	strb r0, [r1]
	add r0, r4, #0
	bl ov03_02255B44
	add r0, r5, #0
	bl SysTask_Destroy
	add sp, #0x54
	pop {r4, r5, pc}
_0225597A:
	ldr r2, _022559F8 ; =ov03_022598A0
	ldr r0, [r2]
	add r1, r0, #0
	add r1, #0x96
	ldrb r1, [r1]
	cmp r1, #0
	beq _022559F0
	add r1, r0, #0
	add r1, #0x96
	ldrb r1, [r1]
	add r0, #0x96
	sub r1, r1, #1
	strb r1, [r0]
	ldr r0, [r2]
	add r1, r0, #0
	add r1, #0x96
	ldrb r1, [r1]
	cmp r1, #0
	bne _022559F0
	ldr r5, _022559FC ; =ov03_02259228
	add r3, sp, #0
	mov r2, #0x29
_022559A6:
	ldrh r1, [r5]
	add r5, r5, #2
	strh r1, [r3]
	add r3, r3, #2
	sub r2, r2, #1
	bne _022559A6
	add r1, r0, #0
	add r1, #0x91
	ldrb r1, [r1]
	cmp r1, #8
	bne _022559CC
	add r0, #0x92
	ldrb r0, [r0]
	mov r1, #0
	add r0, #0x84
	bl ov03_02253E20
	add sp, #0x54
	pop {r4, r5, pc}
_022559CC:
	cmp r1, #0x28
	bne _022559E0
	add r0, #0x92
	ldrb r0, [r0]
	mov r1, #0
	add r0, #0xa0
	bl ov03_02253E20
	add sp, #0x54
	pop {r4, r5, pc}
_022559E0:
	add r4, #0x91
	ldrb r0, [r4]
	lsl r1, r0, #1
	add r0, sp, #0
	ldrh r0, [r0, r1]
	mov r1, #0
	bl ov03_02253E20
_022559F0:
	add sp, #0x54
	pop {r4, r5, pc}
	.balign 4, 0
_022559F4: .word ov03_02255A00
_022559F8: .word ov03_022598A0
_022559FC: .word ov03_02259228
	thumb_func_end ov03_022558C4
	