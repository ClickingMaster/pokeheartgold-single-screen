	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"
	.extern ov96_02202738

.text
	thumb_func_start ov96_0220223C
ov96_0220223C: ; 0x0220223C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0xa0
	str r0, [sp, #4]
	bl PokeathlonCourse_GetDataCopyArea
	str r0, [sp, #0x24]
	ldr r0, [sp, #4]
	bl PokeathlonCourse_GetHeapAllocPtr4
	add r4, r0, #0
	ldr r0, [sp, #4]
	bl ov96_021E5F24
	cmp r0, #0
	beq _0220225C
	b _02202706
_0220225C:
	mov r0, #0x5d
	lsl r0, r0, #4
	ldrb r0, [r4, r0]
	cmp r0, #0
	beq _0220227C
	ldr r0, [sp, #0x24]
	add r0, #0x28
	str r0, [sp, #0x24]
	bl ov96_021E8A20
	add r1, r0, #0
	add r0, r4, #0
	bl ov96_02202738
	add sp, #0xa0
	pop {r3, r4, r5, r6, r7, pc}
_0220227C:
	ldr r0, [sp, #0x24]
	add r0, #0x50
	bl ov96_021E8A20
	add r5, r0, #0
	ldr r0, [sp, #0x24]
	bl ov96_021E8A20
	add r3, r0, #0
	mov r2, #4
_02202290:
	ldmia r3!, {r0, r1}
	stmia r5!, {r0, r1}
	sub r2, r2, #1
	bne _02202290
	ldr r0, [r3]
	str r0, [r5]
	ldr r0, _02202598 ; =0x000005E8
	ldrh r1, [r4, r0]
	cmp r1, #0
	beq _022022A8
	sub r1, r1, #1
	strh r1, [r4, r0]
_022022A8:
	ldr r0, [sp, #0x24]
	add r0, #0x28
	bl ov96_021E8A20
	mov r0, #0
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x24]
	add r5, r4, #0
	str r0, [sp, #0x1c]
	add r0, #0x50
	str r0, [sp, #0x1c]
	add r0, r4, #0
	str r0, [sp, #0x34]
	add r0, #0xd4
	str r0, [sp, #0x34]
	add r0, r4, #0
	str r0, [sp, #0x2c]
	add r0, #0xe0
	str r0, [sp, #0x2c]
	add r0, r4, #0
	str r0, [sp, #0x30]
	add r0, #0xec
	str r0, [sp, #0x30]
_022022D6:
	ldr r0, [sp, #0x1c]
	bl ov96_021E8A20
	add r7, r0, #0
	ldr r0, [r7]
	cmp r0, #0
	ldr r0, _0220259C ; =0x0000059C
	beq _02202316
	ldr r1, [r5, r0]
	cmp r1, #0
	beq _022022FC
	add r0, r0, #4
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _022022FC
	ldr r0, _0220259C ; =0x0000059C
	mov r1, #0
	str r1, [r5, r0]
	b _0220231E
_022022FC:
	cmp r1, #0
	bne _0220231E
	mov r0, #0x5a
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	cmp r0, #0
	bne _0220231E
	ldr r0, _0220259C ; =0x0000059C
	mov r1, #1
	str r1, [r5, r0]
	add r0, r0, #4
	str r1, [r5, r0]
	b _0220231E
_02202316:
	mov r1, #0
	str r1, [r5, r0]
	add r0, r0, #4
	str r1, [r5, r0]
_0220231E:
	ldr r0, _0220259C ; =0x0000059C
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _022023EA
	add r0, sp, #0x94
	str r0, [sp]
	ldr r1, [sp, #0x28]
	ldrb r2, [r7, #4]
	lsl r1, r1, #0x18
	ldrb r3, [r7, #5]
	add r0, r4, #0
	lsr r1, r1, #0x18
	bl ov96_022031A8
	add r6, r0, #0
	cmp r6, #0xc
	beq _022023E2
	lsl r0, r6, #5
	add r1, r4, r0
	mov r0, #0x43
	lsl r0, r0, #4
	ldrb r0, [r1, r0]
	cmp r0, #0
	bne _02202412
	mov r0, #0x48
	mul r0, r6
	add r0, r4, r0
	add r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	beq _02202360
	cmp r0, #2
	bne _022023E2
_02202360:
	ldr r0, [sp, #0x28]
	add r3, r6, #0
	add r1, r4, r0
	ldr r0, _022025A0 ; =0x000005CC
	strb r6, [r1, r0]
	mov r0, #0x48
	mul r3, r0
	add r0, sp, #0x94
	mov ip, r0
	add r0, r4, r3
	str r0, [sp, #0x48]
	add r0, #0xe0
	mov r2, ip
	str r0, [sp, #0x48]
	ldmia r2!, {r0, r1}
	mov ip, r2
	ldr r2, [sp, #0x48]
	stmia r2!, {r0, r1}
	mov r0, ip
	ldr r1, [r0]
	add r0, r2, #0
	str r1, [r0]
	ldrb r0, [r7, #4]
	str r2, [sp, #0x48]
	add r2, r4, r3
	lsl r0, r0, #0xc
	str r0, [sp, #0x88]
	ldrb r0, [r7, #5]
	add r7, sp, #0x88
	add r2, #0xec
	lsl r0, r0, #0xc
	str r0, [sp, #0x90]
	mov r0, #0
	str r0, [sp, #0x8c]
	ldmia r7!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r7]
	str r0, [r2]
	add r0, r4, r3
	add r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _022023E2
	mov r0, #0xc
	add r1, r6, #0
	mul r1, r0
	ldr r0, _022025A4 ; =ov96_0221C98C
	add r2, r4, r3
	add r6, r0, r1
	ldmia r6!, {r0, r1}
	add r2, #0xc8
	stmia r2!, {r0, r1}
	ldr r0, [r6]
	add r1, r4, r3
	str r0, [r2]
	add r1, #0xc4
	mov r0, #2
	str r0, [r1]
	add r1, r4, r3
	add r1, #0xfc
	mov r0, #0
	strh r0, [r1]
	add r1, r4, r3
	add r1, #0xfe
	strh r0, [r1]
_022023E2:
	ldr r0, _022025A8 ; =0x000005EA
	mov r1, #0
	strb r1, [r4, r0]
	b _022026A4
_022023EA:
	mov r0, #0x5a
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _02202420
	ldr r0, [sp, #0x28]
	add r1, r4, r0
	ldr r0, _022025A0 ; =0x000005CC
	ldrb r0, [r1, r0]
	cmp r0, #0xc
	beq _02202412
	ldr r0, _022025A8 ; =0x000005EA
	ldrb r0, [r4, r0]
	bl _dfltu
	ldr r3, _022025AC ; =0x403E0000
	mov r2, #0
	bl _dls
	blo _02202414
_02202412:
	b _022026A4
_02202414:
	ldr r0, _022025A8 ; =0x000005EA
	ldrb r0, [r4, r0]
	add r1, r0, #1
	ldr r0, _022025A8 ; =0x000005EA
	strb r1, [r4, r0]
	b _022026A4
_02202420:
	ldr r0, [sp, #0x28]
	add r1, r4, r0
	ldr r0, _022025A0 ; =0x000005CC
	str r1, [sp, #0x18]
	ldrb r0, [r1, r0]
	cmp r0, #0xc
	bne _02202430
	b _0220269E
_02202430:
	add r0, sp, #0x7c
	str r0, [sp]
	ldr r1, [sp, #0x28]
	ldrb r2, [r7, #4]
	lsl r1, r1, #0x18
	ldrb r3, [r7, #5]
	add r0, r4, #0
	lsr r1, r1, #0x18
	bl ov96_022031A8
	cmp r0, #0xc
	beq _022024A8
	ldr r2, [sp, #0x18]
	ldr r1, _022025A0 ; =0x000005CC
	ldrb r1, [r2, r1]
	cmp r0, r1
	bne _022024A8
	mov r1, #0x48
	mul r1, r0
	add r0, r4, r1
	add r0, #0xfc
	ldrh r0, [r0]
	cmp r0, #4
	bhs _022024EE
	add r0, r4, r1
	add r0, #0xfc
	ldrh r0, [r0]
	add r2, r0, #1
	add r0, r4, r1
	add r0, #0xfc
	strh r2, [r0]
	add r0, r4, r1
	add r0, #0xfc
	ldrh r0, [r0]
	cmp r0, #1
	bhi _02202482
	add r1, r4, r1
	add r1, #0xfe
	mov r0, #0
	strh r0, [r1]
	b _02202696
_02202482:
	cmp r0, #2
	bhi _02202490
	add r1, r4, r1
	add r1, #0xfe
	mov r0, #1
	strh r0, [r1]
	b _02202696
_02202490:
	cmp r0, #3
	bhi _0220249E
	add r1, r4, r1
	add r1, #0xfe
	mov r0, #2
	strh r0, [r1]
	b _02202696
_0220249E:
	add r1, r4, r1
	add r1, #0xfe
	mov r0, #3
	strh r0, [r1]
	b _02202696
_022024A8:
	mov r1, #0
	add r0, sp, #0x70
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	ldr r1, [sp, #0x18]
	ldr r0, _022025A0 ; =0x000005CC
	add r2, sp, #0x64
	ldrb r1, [r1, r0]
	mov r0, #0x48
	add r6, r1, #0
	str r1, [sp, #0x10]
	mul r6, r0
	ldr r1, [sp, #0x2c]
	add r0, sp, #0x7c
	add r1, r1, r6
	bl VEC_Subtract
	ldrb r0, [r7, #4]
	ldr r1, [sp, #0x30]
	add r2, sp, #0x58
	lsl r0, r0, #0xc
	str r0, [sp, #0x4c]
	ldrb r0, [r7, #5]
	add r1, r1, r6
	lsl r0, r0, #0xc
	str r0, [sp, #0x54]
	mov r0, #0
	str r0, [sp, #0x50]
	add r0, sp, #0x4c
	bl VEC_Subtract
	ldr r0, [sp, #0x60]
	cmp r0, #0
	ble _022024F0
_022024EE:
	b _02202696
_022024F0:
	add r3, sp, #0x64
	add r2, r4, r6
	ldmia r3!, {r0, r1}
	add r2, #0xd4
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	ldr r0, [sp, #0x10]
	lsl r0, r0, #5
	add r1, r4, r0
	ldr r0, _022025B0 ; =0x0000042E
	str r1, [sp, #0x14]
	ldrsh r0, [r1, r0]
	cmp r0, #0x3c
	bgt _02202516
	mov r0, #0x3f
	lsl r0, r0, #0x18
	str r0, [sp, #0x20]
	b _0220253A
_02202516:
	cmp r0, #0x46
	bgt _02202520
	ldr r0, _022025B4 ; =0x3F19999A
	str r0, [sp, #0x20]
	b _0220253A
_02202520:
	cmp r0, #0x50
	bgt _0220252A
	ldr r0, _022025B8 ; =0x3F333333
	str r0, [sp, #0x20]
	b _0220253A
_0220252A:
	cmp r0, #0x5a
	bgt _02202534
	ldr r0, _022025BC ; =0x3F4CCCCD
	str r0, [sp, #0x20]
	b _0220253A
_02202534:
	mov r0, #0xfe
	lsl r0, r0, #0x16
	str r0, [sp, #0x20]
_0220253A:
	ldr r0, [sp, #0x34]
	add r0, r0, r6
	bl VEC_Mag
	asr r1, r0, #0xb
	lsr r1, r1, #0x14
	add r1, r0, r1
	asr r0, r1, #0xc
	bl _fflt
	add r7, r0, #0
	bl _f2d
	mov r3, #1
	mov r2, #0
	lsl r3, r3, #0x1e
	bl _dgr
	bls _02202566
	mov r7, #1
	lsl r7, r7, #0x1e
	b _0220257A
_02202566:
	add r0, r7, #0
	bl _f2d
	ldr r3, _022025C0 ; =0x3FF00000
	mov r2, #0
	bl _dls
	bhs _0220257A
	mov r7, #0xfe
	lsl r7, r7, #0x16
_0220257A:
	ldr r0, _022025A8 ; =0x000005EA
	ldrb r0, [r4, r0]
	bl _ffltu
	bl _f2d
	ldr r3, _022025AC ; =0x403E0000
	mov r2, #0
	bl _ddiv
	add r3, r1, #0
	add r2, r0, #0
	ldr r1, _022025C0 ; =0x3FF00000
	b _022025C4
	nop
_02202598: .word 0x000005E8
_0220259C: .word 0x0000059C
_022025A0: .word 0x000005CC
_022025A4: .word ov96_0221C98C
_022025A8: .word 0x000005EA
_022025AC: .word 0x403E0000
_022025B0: .word 0x0000042E
_022025B4: .word 0x3F19999A
_022025B8: .word 0x3F333333
_022025BC: .word 0x3F4CCCCD
_022025C0: .word 0x3FF00000
_022025C4:
	mov r0, #0
	bl _dadd
	bl _d2f
	str r0, [sp, #0x38]
	ldr r0, [sp, #0x20]
	bl _f2d
	str r0, [sp, #0x3c]
	add r0, r7, #0
	str r1, [sp, #0xc]
	bl _f2d
	str r0, [sp, #0x40]
	add r7, r1, #0
	ldr r1, [sp, #0x14]
	ldr r0, _0220270C ; =0x0000041C
	ldr r0, [r1, r0]
	bl _f2d
	add r3, r1, #0
	mov r1, #1
	add r2, r0, #0
	mov r0, #0
	lsl r1, r1, #0x1e
	bl _dsub
	add r2, r0, #0
	add r3, r1, #0
	ldr r0, [sp, #0x40]
	add r1, r7, #0
	bl _dmul
	add r2, r0, #0
	add r3, r1, #0
	ldr r0, [sp, #0x3c]
	ldr r1, [sp, #0xc]
	bl _dmul
	str r0, [sp, #0x44]
	ldr r0, [sp, #0x38]
	add r7, r1, #0
	bl _f2d
	add r2, r0, #0
	add r3, r1, #0
	ldr r0, [sp, #0x44]
	add r1, r7, #0
	bl _ddiv
	bl _d2f
	add r7, r0, #0
	bl _f2d
	ldr r3, _02202710 ; =0x40080000
	mov r2, #0
	bl _dgr
	bls _02202642
	ldr r7, _02202714 ; =0x40400000
	b _02202656
_02202642:
	add r0, r7, #0
	bl _f2d
	ldr r3, _02202718 ; =0x3FF00000
	mov r2, #0
	bl _dls
	bhs _02202656
	mov r7, #0xfe
	lsl r7, r7, #0x16
_02202656:
	ldr r0, [sp, #0x34]
	ldr r1, [sp, #0x34]
	add r0, r0, r6
	add r1, r1, r6
	bl VEC_Normalize
	ldr r0, _0220271C ; =0x45800000
	add r1, r7, #0
	bl _fmul
	bl _ffix
	ldr r1, [sp, #0x34]
	ldr r3, [sp, #0x34]
	add r1, r1, r6
	add r2, sp, #0x70
	add r3, r3, r6
	bl VEC_MultAdd
	ldr r1, [sp, #0x14]
	ldr r0, _02202720 ; =0x00000428
	ldr r0, [r1, r0]
	ldr r1, [sp, #0x34]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r1, r1, r6
	bl ov96_02204320
	add r1, r4, r6
	add r1, #0xc4
	mov r0, #1
	str r0, [r1]
_02202696:
	ldr r1, [sp, #0x18]
	ldr r0, _02202724 ; =0x000005CC
	mov r2, #0xc
	strb r2, [r1, r0]
_0220269E:
	ldr r0, _02202728 ; =0x000005EA
	mov r1, #0
	strb r1, [r4, r0]
_022026A4:
	ldr r0, [sp, #0x1c]
	add r5, #0xc
	add r0, #0x28
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x28]
	add r0, r0, #1
	str r0, [sp, #0x28]
	cmp r0, #4
	bge _022026B8
	b _022022D6
_022026B8:
	ldr r1, _0220272C ; =0x000005DC
	ldr r0, [r4, r1]
	add r1, #0xc
	ldrh r2, [r4, r1]
	ldr r1, _02202730 ; =0x00000708
	sub r1, r1, r2
	bl ov96_022043C0
	ldr r0, [sp, #4]
	bl ov96_022033FC
	ldr r0, [sp, #4]
	bl ov96_02203468
	add r0, r4, #0
	bl ov96_02203754
	ldr r0, [sp, #4]
	add r1, r4, #0
	bl ov96_02203544
	ldr r0, _02202734 ; =0x000005E8
	ldrh r1, [r4, r0]
	cmp r1, #0
	bne _022026EE
	mov r1, #1
	b _022026F0
_022026EE:
	mov r1, #0
_022026F0:
	sub r0, #0x18
	strb r1, [r4, r0]
	ldr r0, [sp, #0x24]
	add r0, #0x28
	str r0, [sp, #0x24]
	bl ov96_021E8A20
	add r1, r0, #0
	add r0, r4, #0
	bl ov96_02202738
_02202706:
	add sp, #0xa0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0220270C: .word 0x0000041C
_02202710: .word 0x40080000
_02202714: .word 0x40400000
_02202718: .word 0x3FF00000
_0220271C: .word 0x45800000
_02202720: .word 0x00000428
_02202724: .word 0x000005CC
_02202728: .word 0x000005EA
_0220272C: .word 0x000005DC
_02202730: .word 0x00000708
_02202734: .word 0x000005E8
	thumb_func_end ov96_0220223C
