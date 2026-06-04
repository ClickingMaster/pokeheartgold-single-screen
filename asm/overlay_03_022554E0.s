	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start ov03_022554E0
ov03_022554E0: ; 0x022554E0
	push {r4, r5, lr}
	sub sp, #0x54
	ldr r5, _02255540 ; =ov03_022592CC
	add r4, r1, #0
	add r3, sp, #0
	mov r2, #0x29
_022554EC:
	ldrh r1, [r5]
	add r5, r5, #2
	strh r1, [r3]
	add r3, r3, #2
	sub r2, r2, #1
	bne _022554EC
	add r1, r4, #0
	bl ov03_0225574C
	ldr r0, _02255544 ; =ov03_022598A0
	ldr r1, [r0]
	add r0, r1, #0
	add r0, #0x91
	ldrb r0, [r0]
	cmp r0, #8
	bne _0225551A
	add r1, #0x92
	ldrb r0, [r1]
	mov r1, #0
	add r0, #0x89
	bl ov03_02253E20
	b _0225552C
_0225551A:
	add r0, r4, #0
	add r0, #0x91
	ldrb r0, [r0]
	lsl r1, r0, #1
	add r0, sp, #0
	ldrh r0, [r0, r1]
	mov r1, #0
	bl ov03_02253E20
_0225552C:
	bl sub_02058284
	ldr r0, _02255548 ; =ov03_0225554C
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x54
	pop {r4, r5, pc}
	nop
_02255540: .word ov03_022592CC
_02255544: .word ov03_022598A0
_02255548: .word ov03_0225554C
	thumb_func_end ov03_022554E0
	