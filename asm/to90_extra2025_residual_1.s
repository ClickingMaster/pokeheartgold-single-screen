	.extern ov49_022589D8
	.extern ov49_0225CC4C
	.extern ov49_0225CDEC
	.extern ov49_0225CF28
	.extern ov49_0225D040
	.extern ov49_0225D098
	.extern ov49_0225D1C4
	.extern ov49_0225D1EC
	.extern ov49_0225D214
	.extern ov49_0225D328
	.extern ov49_0225D494
	.extern ov49_0225D57C
	.extern ov49_0225D5A0
	.extern ov49_0225D5C8
	.extern ov49_0225D5E4
	.extern ov49_0225EB00
	.extern ov49_0225EE4C
	.extern ov49_02269A88
	.extern ov49_02269AAC
	.include "asm/macros.inc"
	.include "overlay_49.inc"
	.include "global.inc"

.text
	thumb_func_start ov49_0225DF18
ov49_0225DF18: ; 0x0225DF18
	push {r4, r5, r6, r7, lr}
	sub sp, #0x34
	add r6, r1, #0
	add r5, r0, #0
	ldr r1, _0225E284 ; =0x00000614
	add r0, r3, #0
	add r7, r2, #0
	str r3, [sp, #4]
	bl Heap_Alloc
	ldr r2, _0225E284 ; =0x00000614
	mov r1, #0
	add r4, r0, #0
	bl memset
	mov r0, #0x61
	str r7, [r4]
	lsl r0, r0, #4
	strb r5, [r4, r0]
	add r0, r0, #1
	strb r6, [r4, r0]
	ldr r2, [sp, #4]
	ldr r3, [sp, #0x48]
	mov r0, #9
	mov r1, #0x80
	bl ov49_0225CC4C
	str r0, [r4, #4]
	ldr r0, [sp, #0x48]
	ldr r3, [sp, #4]
	str r0, [sp]
	ldr r0, [r4, #4]
	add r1, r6, #0
	add r2, r5, #0
	bl ov49_0225CDEC
	mov r0, #0
	ldr r5, _0225E288 ; =ov49_02269AAC
	str r0, [sp, #0xc]
_0225DF66:
	mov r0, #0
	str r0, [sp]
	ldrh r1, [r5]
	str r0, [sp, #8]
	add r2, sp, #0x1c
	ldr r0, [r4]
	add r2, #2
	add r3, sp, #0x1c
	bl ov49_022589D8
	cmp r0, #1
	beq _0225DF80
	b _0225E234
_0225DF80:
	mov r0, #0x92
	lsl r0, r0, #2
	add r0, r4, r0
	str r0, [sp, #0x10]
	mov r0, #0xda
	lsl r0, r0, #2
	add r0, r4, r0
	str r0, [sp, #0x14]
	ldr r0, _0225E28C ; =0x000004E8
	mov r7, sp
	add r0, r4, r0
	str r0, [sp, #0x18]
	sub r7, r7, #4
	add r6, sp, #0x1c
_0225DF9C:
	ldrh r1, [r5, #2]
	ldrh r2, [r6, #2]
	ldrh r3, [r6]
	ldr r0, [r4, #4]
	bl ov49_0225D098
	ldr r1, _0225E290 ; =0x00000612
	ldrb r1, [r4, r1]
	lsl r1, r1, #2
	add r1, r4, r1
	str r0, [r1, #8]
	ldrh r0, [r5, #2]
	cmp r0, #0x10
	bls _0225DFBA
	b _0225E20E
_0225DFBA:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225DFC6: ; jump table
	.short _0225E014 - _0225DFC6 - 2 ; case 0
	.short _0225E20E - _0225DFC6 - 2 ; case 1
	.short _0225E20E - _0225DFC6 - 2 ; case 2
	.short _0225E20E - _0225DFC6 - 2 ; case 3
	.short _0225E20E - _0225DFC6 - 2 ; case 4
	.short _0225E20E - _0225DFC6 - 2 ; case 5
	.short _0225DFE8 - _0225DFC6 - 2 ; case 6
	.short _0225DFE8 - _0225DFC6 - 2 ; case 7
	.short _0225DFE8 - _0225DFC6 - 2 ; case 8
	.short _0225E20E - _0225DFC6 - 2 ; case 9
	.short _0225E052 - _0225DFC6 - 2 ; case 10
	.short _0225E07E - _0225DFC6 - 2 ; case 11
	.short _0225E07E - _0225DFC6 - 2 ; case 12
	.short _0225E094 - _0225DFC6 - 2 ; case 13
	.short _0225E0F2 - _0225DFC6 - 2 ; case 14
	.short _0225E150 - _0225DFC6 - 2 ; case 15
	.short _0225E18C - _0225DFC6 - 2 ; case 16
_0225DFE8:
	ldr r1, _0225E290 ; =0x00000612
	mov r2, #0
	ldrb r1, [r4, r1]
	ldr r0, [r4, #4]
	add r3, r2, #0
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225D214
	ldr r0, _0225E290 ; =0x00000612
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r2, [r0, #8]
	ldrh r0, [r5, #2]
	lsl r0, r0, #2
	add r1, r4, r0
	mov r0, #0x85
	lsl r0, r0, #2
	str r2, [r1, r0]
	b _0225E20E
_0225E014:
	ldr r1, _0225E290 ; =0x00000612
	mov r2, #0
	ldrb r1, [r4, r1]
	ldr r0, [r4, #4]
	add r3, r2, #0
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225D214
	ldr r1, _0225E290 ; =0x00000612
	ldr r0, [r4, #4]
	ldrb r1, [r4, r1]
	mov r2, #1
	mov r3, #0
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225D214
	ldr r1, _0225E290 ; =0x00000612
	ldr r0, [r4, #4]
	ldrb r1, [r4, r1]
	mov r2, #2
	mov r3, #0
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225D214
	b _0225E20E
_0225E052:
	ldr r1, _0225E290 ; =0x00000612
	ldr r0, [r4, #4]
	ldrb r1, [r4, r1]
	mov r2, #0
	mov r3, #2
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225D214
	ldr r0, _0225E290 ; =0x00000612
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r2, [r0, #8]
	ldrh r0, [r5]
	lsl r0, r0, #2
	add r1, r4, r0
	mov r0, #0x45
	lsl r0, r0, #2
	str r2, [r1, r0]
	b _0225E20E
_0225E07E:
	ldr r1, _0225E290 ; =0x00000612
	ldr r0, [r4, #4]
	ldrb r1, [r4, r1]
	mov r2, #0
	mov r3, #2
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225D214
	b _0225E20E
_0225E094:
	ldr r0, _0225E294 ; =0x00000608
	mov r1, #0xc
	ldrb r0, [r4, r0]
	mul r1, r0
	ldr r0, [sp, #0x10]
	add r0, r0, r1
	ldr r1, _0225E290 ; =0x00000612
	ldrb r1, [r4, r1]
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225EB00
	ldr r0, _0225E294 ; =0x00000608
	ldrb r0, [r4, r0]
	add r1, r0, #1
	ldr r0, _0225E294 ; =0x00000608
	strb r1, [r4, r0]
	ldrb r0, [r4, r0]
	cmp r0, #0x18
	bls _0225E0C2
	bl GF_AssertFail
_0225E0C2:
	ldr r0, _0225E290 ; =0x00000612
	mov r1, #0
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r0, [r0, #8]
	bl ov49_0225D494
	ldrh r1, [r5]
	ldr r0, _0225E298 ; =0x0000FFA4
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #1
	bls _0225E0E2
	b _0225E20E
_0225E0E2:
	ldr r0, _0225E290 ; =0x00000612
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r0, [r0, #8]
	bl ov49_0225EE4C
	b _0225E20E
_0225E0F2:
	ldr r0, _0225E29C ; =0x00000609
	mov r1, #0xc
	ldrb r0, [r4, r0]
	mul r1, r0
	ldr r0, [sp, #0x14]
	add r0, r0, r1
	ldr r1, _0225E290 ; =0x00000612
	ldrb r1, [r4, r1]
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225EB00
	ldr r0, _0225E29C ; =0x00000609
	ldrb r0, [r4, r0]
	add r1, r0, #1
	ldr r0, _0225E29C ; =0x00000609
	strb r1, [r4, r0]
	sub r0, r0, #1
	ldrb r0, [r4, r0]
	cmp r0, #0x18
	bls _0225E122
	bl GF_AssertFail
_0225E122:
	ldr r0, _0225E290 ; =0x00000612
	mov r1, #0
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r0, [r0, #8]
	bl ov49_0225D494
	ldrh r1, [r5]
	ldr r0, _0225E298 ; =0x0000FFA4
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #1
	bhi _0225E20E
	ldr r0, _0225E290 ; =0x00000612
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r0, [r0, #8]
	bl ov49_0225EE4C
	b _0225E20E
_0225E150:
	ldr r0, _0225E290 ; =0x00000612
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r2, [r0, #8]
	ldr r0, _0225E2A0 ; =0x0000060A
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r1, r4, r0
	ldr r0, _0225E2A4 ; =0x00000488
	str r2, [r1, r0]
	ldr r0, _0225E2A0 ; =0x0000060A
	ldrb r0, [r4, r0]
	add r1, r0, #1
	ldr r0, _0225E2A0 ; =0x0000060A
	strb r1, [r4, r0]
	ldrb r0, [r4, r0]
	cmp r0, #0x18
	bls _0225E17A
	bl GF_AssertFail
_0225E17A:
	ldr r0, _0225E290 ; =0x00000612
	mov r1, #0
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r0, [r0, #8]
	bl ov49_0225D494
	b _0225E20E
_0225E18C:
	ldr r0, _0225E2A8 ; =0x0000060B
	mov r1, #0xc
	ldrb r0, [r4, r0]
	mul r1, r0
	ldr r0, [sp, #0x18]
	add r0, r0, r1
	ldr r1, _0225E290 ; =0x00000612
	ldrb r1, [r4, r1]
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r1, [r1, #8]
	bl ov49_0225EB00
	ldr r0, _0225E2A8 ; =0x0000060B
	ldrb r0, [r4, r0]
	add r1, r0, #1
	ldr r0, _0225E2A8 ; =0x0000060B
	strb r1, [r4, r0]
	ldrb r0, [r4, r0]
	cmp r0, #0x18
	bls _0225E1BA
	bl GF_AssertFail
_0225E1BA:
	ldr r0, _0225E290 ; =0x00000612
	mov r1, #0
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r0, [r0, #8]
	bl ov49_0225D494
	ldr r0, _0225E290 ; =0x00000612
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r0, [r0, #8]
	bl ov49_0225D1EC
	strh r0, [r6, #4]
	lsr r0, r0, #0x10
	strh r0, [r6, #6]
	ldrh r0, [r6, #4]
	strh r0, [r6, #8]
	ldrh r0, [r6, #6]
	strh r0, [r6, #0xa]
	mov r0, #8
	ldrsh r0, [r6, r0]
	add r0, #8
	strh r0, [r6, #8]
	mov r0, #0xa
	ldrsh r0, [r6, r0]
	add r0, #0x14
	strh r0, [r6, #0xa]
	ldr r0, _0225E290 ; =0x00000612
	ldrh r1, [r6, #8]
	ldrb r0, [r4, r0]
	lsl r0, r0, #2
	add r0, r4, r0
	ldr r0, [r0, #8]
	strh r1, [r7]
	ldrh r1, [r6, #0xa]
	strh r1, [r7, #2]
	ldr r1, [r7]
	bl ov49_0225D1C4
_0225E20E:
	ldr r0, _0225E290 ; =0x00000612
	add r2, sp, #0x1c
	ldrb r0, [r4, r0]
	add r2, #2
	add r3, sp, #0x1c
	add r1, r0, #1
	ldr r0, _0225E290 ; =0x00000612
	strb r1, [r4, r0]
	ldr r0, [sp, #8]
	add r0, r0, #1
	str r0, [sp]
	str r0, [sp, #8]
	ldrh r1, [r5]
	ldr r0, [r4]
	bl ov49_022589D8
	cmp r0, #1
	bne _0225E234
	b _0225DF9C
_0225E234:
	ldr r0, [sp, #0xc]
	add r5, r5, #4
	add r0, r0, #1
	str r0, [sp, #0xc]
	cmp r0, #0x23
	bhs _0225E242
	b _0225DF66
_0225E242:
	mov r0, #0xa
	mov r7, #0
	lsl r0, r0, #0xe
	ldr r6, _0225E2AC ; =ov49_02269A88
	str r7, [sp, #0x28]
	str r0, [sp, #0x30]
	str r7, [sp, #0x2c]
	add r5, r4, #0
_0225E252:
	ldrh r1, [r6]
	ldrh r2, [r6, #2]
	ldr r0, [r4, #4]
	add r3, sp, #0x28
	bl ov49_0225CF28
	mov r1, #0x82
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r0, r1, #0
	ldr r0, [r5, r0]
	mov r1, #0
	bl ov49_0225D040
	add r7, r7, #1
	add r6, r6, #4
	add r5, r5, #4
	cmp r7, #9
	blt _0225E252
	ldr r0, _0225E2B0 ; =0x00000613
	mov r1, #9
	strb r1, [r4, r0]
	add r0, r4, #0
	add sp, #0x34
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225E284: .word 0x00000614
_0225E288: .word ov49_02269AAC
_0225E28C: .word 0x000004E8
_0225E290: .word 0x00000612
_0225E294: .word 0x00000608
_0225E298: .word 0x0000FFA4
_0225E29C: .word 0x00000609
_0225E2A0: .word 0x0000060A
_0225E2A4: .word 0x00000488
_0225E2A8: .word 0x0000060B
_0225E2AC: .word ov49_02269A88
_0225E2B0: .word 0x00000613
	thumb_func_end ov49_0225DF18
