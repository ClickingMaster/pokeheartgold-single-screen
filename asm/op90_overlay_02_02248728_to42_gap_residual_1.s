	.extern ov02_02253A4C
#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/sndseq.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/std_script.h"
	.include "asm/macros.inc"
	.include "overlay_02.inc"
	.include "global.inc"
	.public ov02_0224E828

.text
.extern ov02_0224EF6C
	thumb_func_start ov02_0224E828
ov02_0224E828: ; 0x0224E828
	push {r4, r5, r6, r7, lr}
	sub sp, #0x6c
	add r4, r0, #0
	add r5, r1, #0
	ldr r0, [sp, #0x84]
	str r2, [sp, #4]
	ldr r1, _0224EB44 ; =ov02_02253A4C
	str r0, [sp, #0x84]
	ldr r0, [sp, #0x88]
	ldrb r2, [r1, #2]
	str r0, [sp, #0x88]
	ldrb r1, [r1, #3]
	str r3, [sp, #8]
	add r0, sp, #0x68
	strb r2, [r0, #1]
	strb r1, [r0, #2]
	ldr r1, [sp, #0x84]
	ldr r6, [sp, #0x80]
	strh r5, [r1]
	ldr r2, [sp, #4]
	ldr r1, [sp, #0x88]
	strh r2, [r1]
	ldr r1, [sp, #8]
	cmp r1, #3
	bls _0224E85C
	b _0224EB3E
_0224E85C:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0224E868: ; jump table
	.short _0224E870 - _0224E868 - 2 ; case 0
	.short _0224E870 - _0224E868 - 2 ; case 1
	.short _0224E992 - _0224E868 - 2 ; case 2
	.short _0224E992 - _0224E868 - 2 ; case 3
_0224E870:
	mov r0, #0
	str r0, [sp, #0x50]
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _0224E87E
	mov r0, #1
	b _0224E880
_0224E87E:
	ldr r0, [sp, #0x50]
_0224E880:
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	mov r0, #0
	str r0, [sp, #0x54]
	add r0, sp, #0x68
	add r0, #1
	ldrsb r0, [r0, r1]
	ldr r7, [sp, #0x54]
	str r0, [sp, #0x38]
	add r0, sp, #0x70
	ldrb r0, [r0, #0x1c]
	str r0, [sp, #0x48]
	lsl r0, r5, #4
	str r0, [sp, #0x44]
	add r0, #8
	str r0, [sp, #0x44]
	lsl r0, r0, #0xc
	str r0, [sp, #0x40]
_0224E8A4:
	ldr r0, [sp, #0x38]
	add r1, r7, r0
	ldr r0, [sp, #4]
	add r0, r0, r1
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #0x3c]
	ldr r2, [sp, #0x3c]
	add r0, r4, #0
	add r1, r5, #0
	bl GetMetatileBehavior
	str r0, [sp, #0x58]
	ldr r2, [sp, #0x3c]
	add r0, r4, #0
	add r1, r5, #0
	bl sub_020548C0
	add r1, r0, #0
	ldr r0, [sp, #0x58]
	ldr r2, [sp, #0x48]
	bl ov02_0224EF6C
	cmp r0, #0
	beq _0224E95C
	ldr r0, [sp, #0x3c]
	lsl r0, r0, #4
	add r0, #8
	cmp r0, #0
	ble _0224E8F4
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	str r0, [sp, #0x14]
	b _0224E904
_0224E8F4:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
	str r0, [sp, #0x14]
_0224E904:
	ldr r0, [sp, #0x44]
	cmp r0, #0
	ble _0224E91C
	ldr r0, [sp, #0x40]
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0224E92A
_0224E91C:
	ldr r0, [sp, #0x40]
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0224E92A:
	bl _ffix
	str r0, [sp, #0x5c]
	ldr r0, [sp, #0x14]
	bl _ffix
	add r3, r0, #0
	add r0, sp, #0x68
	str r0, [sp]
	ldr r2, [sp, #0x5c]
	add r0, r4, #0
	add r1, r6, #0
	bl sub_02054774
	add r1, sp, #0x68
	ldrb r1, [r1]
	cmp r1, #1
	bne _0224E95C
	cmp r0, r6
	bne _0224E95C
	ldr r0, [sp, #0x50]
	add r0, r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x50]
_0224E95C:
	ldr r0, [sp, #0x38]
	add r7, r7, r0
	ldr r0, [sp, #0x54]
	add r0, r0, #1
	str r0, [sp, #0x54]
	cmp r0, #2
	blt _0224E8A4
	ldr r0, [sp, #0x50]
	cmp r0, #2
	beq _0224E972
	b _0224EB3E
_0224E972:
	ldr r0, [sp, #0x84]
	strh r5, [r0]
	ldr r0, [sp, #8]
	cmp r0, #0
	bne _0224E984
	ldr r0, [sp, #4]
	sub r1, r0, #1
	ldr r0, [sp, #0x88]
	b _0224E98A
_0224E984:
	ldr r0, [sp, #4]
	add r1, r0, #2
	ldr r0, [sp, #0x88]
_0224E98A:
	strh r1, [r0]
	add sp, #0x6c
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0224E992:
	ldr r1, [sp, #8]
	cmp r1, #2
	bne _0224E99C
	mov r1, #1
	b _0224E99E
_0224E99C:
	mov r1, #2
_0224E99E:
	ldrsb r0, [r0, r1]
	ldr r2, [sp, #4]
	add r0, r5, r0
	lsl r0, r0, #0x10
	asr r5, r0, #0x10
	add r0, r4, #0
	add r1, r5, #0
	bl GetMetatileBehavior
	str r0, [sp, #0x20]
	ldr r2, [sp, #4]
	add r0, r4, #0
	add r1, r5, #0
	bl sub_020548C0
	str r0, [sp, #0x18]
	ldr r0, [sp, #4]
	lsl r0, r0, #4
	add r0, #8
	cmp r0, #0
	ble _0224E9DC
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	str r0, [sp, #0x10]
	b _0224E9EC
_0224E9DC:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
	str r0, [sp, #0x10]
_0224E9EC:
	lsl r0, r5, #4
	str r0, [sp, #0x2c]
	add r0, #8
	str r0, [sp, #0x2c]
	cmp r0, #0
	ble _0224EA0A
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0224EA18
_0224EA0A:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0224EA18:
	bl _ffix
	add r7, r0, #0
	ldr r0, [sp, #0x10]
	bl _ffix
	add r3, r0, #0
	add r0, sp, #0x68
	str r0, [sp]
	add r0, r4, #0
	add r1, r6, #0
	add r2, r7, #0
	bl sub_02054774
	add r7, r0, #0
	add r0, sp, #0x70
	ldrb r0, [r0, #0x1c]
	ldr r1, [sp, #0x18]
	str r0, [sp, #0x34]
	ldr r0, [sp, #0x20]
	ldr r2, [sp, #0x34]
	bl ov02_0224EF6C
	cmp r0, #0
	beq _0224EA56
	add r0, sp, #0x68
	ldrb r0, [r0]
	cmp r0, #1
	bne _0224EA56
	cmp r7, r6
	beq _0224EA5C
_0224EA56:
	add sp, #0x6c
	mov r0, #0
	pop {r4, r5, r6, r7, pc}
_0224EA5C:
	mov r0, #0
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x2c]
	add r7, sp, #0x68
	lsl r0, r0, #0xc
	add r7, #1
	str r0, [sp, #0x4c]
_0224EA6A:
	mov r0, #0
	ldrsb r1, [r7, r0]
	ldr r0, [sp, #4]
	add r0, r0, r1
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #0x30]
	ldr r2, [sp, #0x30]
	add r0, r4, #0
	add r1, r5, #0
	bl GetMetatileBehavior
	str r0, [sp, #0x24]
	ldr r2, [sp, #0x30]
	add r0, r4, #0
	add r1, r5, #0
	bl sub_020548C0
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x30]
	lsl r0, r0, #4
	add r0, #8
	cmp r0, #0
	ble _0224EAAE
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	str r0, [sp, #0xc]
	b _0224EABE
_0224EAAE:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
	str r0, [sp, #0xc]
_0224EABE:
	ldr r0, [sp, #0x2c]
	cmp r0, #0
	ble _0224EAD6
	ldr r0, [sp, #0x4c]
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0224EAE4
_0224EAD6:
	ldr r0, [sp, #0x4c]
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0224EAE4:
	bl _ffix
	str r0, [sp, #0x60]
	ldr r0, [sp, #0xc]
	bl _ffix
	add r3, r0, #0
	add r0, sp, #0x68
	str r0, [sp]
	ldr r2, [sp, #0x60]
	add r0, r4, #0
	add r1, r6, #0
	bl sub_02054774
	str r0, [sp, #0x64]
	ldr r0, [sp, #0x24]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x34]
	bl ov02_0224EF6C
	cmp r0, #0
	beq _0224EB32
	add r0, sp, #0x68
	ldrb r0, [r0]
	cmp r0, #1
	bne _0224EB32
	ldr r0, [sp, #0x64]
	cmp r0, r6
	bne _0224EB32
	ldr r0, [sp, #0x84]
	strh r5, [r0]
	ldr r1, [sp, #4]
	ldr r0, [sp, #0x28]
	add r1, r1, r0
	ldr r0, [sp, #0x88]
	add sp, #0x6c
	strh r1, [r0]
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0224EB32:
	ldr r0, [sp, #0x28]
	add r7, r7, #1
	add r0, r0, #1
	str r0, [sp, #0x28]
	cmp r0, #2
	blt _0224EA6A
_0224EB3E:
	mov r0, #0
	add sp, #0x6c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0224EB44: .word ov02_02253A4C
	thumb_func_end ov02_0224E828
