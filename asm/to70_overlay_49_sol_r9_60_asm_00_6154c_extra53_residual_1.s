	.extern ov49_02258CB8
	.extern ov49_02258D54
	.extern ov49_02258D70
	.extern ov49_02258DAC
	.extern ov49_02258EEC
	.extern ov49_02258F38
	.extern ov49_02259130
	.extern ov49_022591C0
	.extern ov49_02259FE8
	.extern ov49_02259FF0
	.extern ov49_0225A010
	.extern ov49_0225A04C
	.extern ov49_0225A06C
	.extern ov49_0225EF40
	.extern ov49_0225EF68
	.extern ov49_0225EF84
	.extern ov49_0225EF88
	.extern ov49_0225EF8C
	.extern ov49_0225EF90
	.extern ov49_0225EF98
	.extern ov49_0225EFC4
	.extern ov49_02261DBC
	.extern ov49_02269B40
	.extern ov49_02269B48
	.extern ov49_02269B50
	.extern ov49_02269B58
	.extern ov49_02269B80
	.include "asm/macros.inc"
	.include "overlay_49.inc"
	.include "global.inc"

.text
	thumb_func_start ov49_0226154C
ov49_0226154C: ; 0x0226154C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r6, r1, #0
	add r5, r0, #0
	add r0, r6, #0
	add r4, r2, #0
	bl ov49_02259FE8
	str r0, [sp, #8]
	add r0, r6, #0
	bl ov49_02259FF0
	add r1, r4, #0
	str r0, [sp, #0xc]
	bl ov49_02258D70
	add r7, r0, #0
	add r0, r5, #0
	bl ov49_0225EF84
	str r0, [sp, #0x10]
	add r0, r6, #0
	bl ov49_0225A010
	str r0, [sp, #0x14]
	add r0, r5, #0
	bl ov49_0225EF88
	cmp r0, #5
	bhi _02261606
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02261594: ; jump table
	.short _022615A0 - _02261594 - 2 ; case 0
	.short _022615B0 - _02261594 - 2 ; case 1
	.short _022615D4 - _02261594 - 2 ; case 2
	.short _02261618 - _02261594 - 2 ; case 3
	.short _022616CA - _02261594 - 2 ; case 4
	.short _022616DC - _02261594 - 2 ; case 5
_022615A0:
	add r0, r5, #0
	mov r1, #8
	bl ov49_0225EF40
	add r0, r5, #0
	mov r1, #1
	bl ov49_0225EF8C
_022615B0:
	ldr r0, [sp, #8]
	add r1, r4, #0
	bl ov45_0222A230
	add r6, r0, #0
	ldr r0, [sp, #8]
	add r1, r4, #0
	bl ov45_0222A550
	cmp r6, #1
	beq _022615CA
	cmp r0, #1
	bne _02261606
_022615CA:
	add r0, r5, #0
	mov r1, #2
	bl ov49_0225EF8C
	b _02261710
_022615D4:
	ldr r0, [sp, #8]
	add r1, r4, #0
	bl ov45_0222A578
	add r6, r0, #0
	bne _022615EA
	add r0, r5, #0
	mov r1, #1
	bl ov49_0225EF8C
	b _02261710
_022615EA:
	bl ov45_0222AADC
	cmp r0, #1
	bne _02261606
	add r0, r6, #0
	bl ov45_0222AA5C
	add r2, r0, #0
	ldr r0, [sp, #0xc]
	add r1, r4, #0
	bl ov49_02258CB8
	cmp r0, #0
	bne _02261608
_02261606:
	b _02261710
_02261608:
	mov r1, #0
	bl ov49_022591C0
	add r0, r5, #0
	mov r1, #3
	bl ov49_0225EF8C
	b _02261710
_02261618:
	ldr r0, [sp, #8]
	add r1, r4, #0
	bl ov45_0222A578
	cmp r0, #0
	bne _02261638
	cmp r7, #0
	beq _0226162E
	add r0, r7, #0
	bl ov49_02258D54
_0226162E:
	add r0, r5, #0
	mov r1, #1
	bl ov49_0225EF8C
	b _02261710
_02261638:
	bl ov45_0222A920
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #8]
	add r2, r6, #0
	add r3, r7, #0
	bl ov49_02261DBC
	cmp r0, #0
	beq _02261696
	add r0, r5, #0
	mov r1, #5
	bl ov49_0225EF8C
	ldr r0, [sp, #0xc]
	add r1, r7, #0
	mov r2, #0
	bl ov49_02258EEC
	lsl r1, r4, #0x18
	add r0, r6, #0
	lsr r1, r1, #0x18
	mov r2, #1
	bl ov49_0225A06C
	lsl r1, r4, #0x18
	add r0, r6, #0
	lsr r1, r1, #0x18
	mov r2, #1
	bl ov49_0225A04C
	add r0, r7, #0
	mov r1, #0
	bl ov49_02259130
	ldr r0, [sp, #0x14]
	ldr r2, _02261718 ; =ov49_02269B58
	ldr r3, [sp, #0x10]
	add r1, r4, #0
	bl ov49_0225EFC4
	b _02261710
_02261696:
	ldr r0, [sp, #0xc]
	add r1, r7, #0
	mov r2, #4
	bl ov49_02258EEC
	add r0, r5, #0
	mov r1, #4
	bl ov49_0225EF8C
	lsl r1, r4, #0x18
	add r0, r6, #0
	lsr r1, r1, #0x18
	mov r2, #1
	bl ov49_0225A04C
	lsl r1, r4, #0x18
	add r0, r6, #0
	lsr r1, r1, #0x18
	mov r2, #1
	bl ov49_0225A06C
	add r0, r7, #0
	mov r1, #1
	bl ov49_022591C0
	b _02261710
_022616CA:
	add r0, r7, #0
	bl ov49_02258F38
	cmp r0, #1
	bne _02261710
	add r0, r5, #0
	bl ov49_0225EF90
	b _02261710
_022616DC:
	add r0, r5, #0
	bl ov49_0225EF68
	ldr r0, [sp, #0xc]
	add r1, r7, #0
	mov r2, #2
	bl ov49_02258EEC
	ldr r0, [sp, #0x14]
	ldr r2, _0226171C ; =ov49_02269B40
	add r1, r4, #0
	mov r3, #0
	bl ov49_0225EF98
	lsl r1, r4, #0x18
	add r0, r6, #0
	lsr r1, r1, #0x18
	mov r2, #0
	bl ov49_0225A04C
	lsl r1, r4, #0x18
	add r0, r6, #0
	lsr r1, r1, #0x18
	mov r2, #0
	bl ov49_0225A06C
_02261710:
	mov r0, #0
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02261718: .word ov49_02269B58
_0226171C: .word ov49_02269B40
	thumb_func_end ov49_0226154C
