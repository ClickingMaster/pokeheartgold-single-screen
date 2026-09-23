	.include "asm/macros.inc"
	.include "overlay_13_thumb_2.inc"
	.include "global.inc"

	.text

	.extern ov13_022208E8
	.extern ov13_022208F8
	.extern ov13_02220908
	.extern ov13_022209F8
	.extern ov13_02221410
	.extern ov13_02221428
	.extern ov13_02221454
	.extern ov13_022214AC
	.extern ov13_022214B8
	.extern ov13_022214C4
	.extern ov13_022216BC
	.extern ov13_022216E0
	.extern ov13_0222175C
	.extern ov13_022217A0
	.extern ov13_022217D0
	.extern ov13_022217FC
	.extern ov13_0222189C
	.extern ov13_02221978
	.extern ov13_02221A50
	.extern ov13_02221AF4
	.extern ov13_02221BE8
	.extern ov13_02221C2C
	.extern ov13_02221C78
	.extern ov13_02221D34
	.extern ov13_02221D58
	.extern ov13_02221EAC
	.extern ov13_02221F64
	.extern ov13_02221F94
	.extern ov13_022220A4
	.extern ov13_022220B4
	.extern ov13_02222108
	.extern ov13_0222224C
	.extern ov13_02222328
	.extern ov13_02222394
	.extern ov13_02222420
	.extern ov13_02222474
	.extern ov13_022224CC
	.extern ov13_02222534
	.extern ov13_022225B0
	.extern ov13_02222638
	.extern ov13_0222269C
	.extern ov13_022226C8
	.extern ov13_02222708
	.extern ov13_02222730
	.extern ov13_02222768
	.extern ov13_022227A0
	.extern ov13_0222281C
	.extern ov13_02222850
	.extern ov13_02222874
	.extern ov13_022228A4
	.extern ov13_022228BC
	.extern ov13_022228CC
	.extern ov13_02222924
	.extern ov13_02222948
	.extern ov13_02222968
	.extern ov13_02222978
	.extern ov13_02222984
	.extern ov13_02222998
	.extern ov13_022229EC
	.extern ov13_022229FC
	.extern ov13_02222A00
	.extern ov13_02222A08
	.extern ov13_02222A14
	.extern ov13_02222A1C
	.extern ov13_02222A44
	.extern ov13_02222A5C
	.extern ov13_02222A84
	.extern ov13_02222A9C
	.extern ov13_02222AB0
	.extern ov13_02222B20
	.extern ov13_02222B2C
	.extern ov13_02222B34
	.extern ov13_02222BC0
	.extern ov13_02222BE4
	.extern ov13_02222BF4
	.extern ov13_02222C08
	.extern ov13_02222C1C
	.extern ov13_02222CE0
	.extern ov13_02222D78
	.extern ov13_02222DB0
	.extern ov13_02222F28
	.extern ov13_022230F8
	.extern ov13_02223100
	.extern ov13_02223114
	.extern ov13_022233CC
	.extern ov13_02223434
	.extern ov13_02223478
	.extern ov13_02223568
	.extern ov13_022235A4
	.extern ov13_022235DC
	.extern ov13_02223634
	.extern ov13_022236B8
	.extern ov13_02223770
	.extern ov13_02223A7C
	.extern ov13_02223AE4
	.extern ov13_02223B28
	.extern ov13_02223C10
	.extern ov13_02223C48
	.extern ov13_02223CA0
	.extern ov13_02223D24
	.extern ov13_02223DE0
	.extern ov13_02223DEC
	.extern ov13_02223E1C
	.extern ov13_02223E44
	.extern ov13_02223E6C
	.extern ov13_02223EA4
	.extern ov13_02223ED4
	.extern ov13_02223EE0
	.extern ov13_02223F84
	.extern ov13_02223FDC
	.extern ov13_02224100
	.extern ov13_02224164
	.extern ov13_02224330
	.extern ov13_022245C4
	.extern ov13_022245DC
	.extern ov13_02224618
	.extern ov13_02224624
	.extern ov13_02224694
	.extern ov13_022246E4
	.extern ov13_02224718
	.extern ov13_022247DC
	.extern ov13_0222483C
	.extern ov13_02224864
	.extern ov13_02224938
	.extern ov13_02224988
	.extern ov13_02224A24
	.extern ov13_022256C8
	.extern ov13_02225710
	.extern ov13_022259C8
	.extern ov13_02225B58
	.extern ov13_02225F14
	.extern ov13_022262D0
	.extern ov13_022262F8
	.extern ov13_02226370
	.extern ov13_022263C4
	.extern ov13_02226C0C
	.extern ov13_02226C38
	.extern ov13_02226C64
	.extern ov13_02226C7C
	.extern ov13_02226C94
	.extern ov13_02226CBC
	.extern ov13_02226CD8
	.extern ov13_02226CFC
	.extern ov13_02226D0C
	.extern ov13_02226D40
	.extern ov13_02226D74
	.extern ov13_02226E5C
	.extern ov13_02226EF0
	.extern ov13_02226F28
	.extern ov13_02226F3C
	.extern _0224267C
	.extern ov13_02242680
	.extern ov13_02242688
	.extern ov13_022426B0
	.extern ov13_02242AB0
	.extern ov13_02242EB0
	.extern ov13_022432B0
	.extern ov13_022436B0
	.extern ov13_02243AB0
	.extern ov13_02243EB0
	.extern ov13_022442B0
	.extern ov13_022446B0
	.extern ov13_02244AB0
	.extern _022459B0
	.extern ov13_022459B4
	.extern ov13_022459BC
	.extern ov13_02245A14
	.extern ov13_02245A20
	.extern ov13_02245A28
	.extern ov13_02245A58
	.extern ov13_02245A6C
	.extern ov13_02245AC4
	.extern ov13_02245ACC
	.extern ov13_02245AD8
	.extern ov13_02245B18
	.extern _0224CF98
	.extern ov13_0224CF9C
	.extern ov13_0224CFB8
	.extern ov13_0224CFBC
	.extern ov13_0224CFD8
	.extern ov13_0224D258
	.extern ov13_0224D658
	.extern ov13_0224D660
	.extern ov13_0224D790
	.extern ov13_0224D8C0
	.extern ov13_0224D930
	.extern ov13_0224DCF8
	.extern ov13_0224DD00
	.extern ov13_0224DD02
	.extern ov13_0224DD80
	.extern ov13_0224DD8C
	.extern ov13_0224DD90
	.extern ov13_0224DDA0
	.extern ov13_0224DDC0
	.extern ov13_0224DE80
	.extern ov13_0224DEE0
	.extern ov13_0224DF08
	.extern ov13_0224DF10
	.extern ov13_0224DF30
	.extern ov13_0224DF80
	.extern ov13_0224DFB0
	.extern ov13_0224DFB4
	.extern ov13_0224DFC4
	.extern ov13_0224DFDC
	.extern ov13_0224E09C
	.extern ov13_0224E0B0
	.extern ov13_0224E0C4
	.extern ov13_0224E144
	.extern ov13_0224E184
	.extern ov13_0224E270
	.extern ov13_0224E284
	.extern ov13_0224E2B0
	.extern ov13_0224E2E8
	.extern ov13_0224E380
	.extern ov13_0224E3D8
	.extern ov13_0224E3E0
	.extern ov13_0224E3E8
	.extern ov13_0224E3F0
	.extern ov13_0224E400
	.extern ov13_0224E408
	.extern ov13_0224E420
	.extern ov13_0224E440
	.extern ov13_0224E464
	.extern ov13_0224EC64


	.extern ov13_02224D00
	.extern ov13_02225320
	.extern ov13_02224A30
	.extern ov13_02224B2C
	.extern ov13_02224DB4
	.extern ov13_02224F3C
	.extern ov13_02225358
	.extern ov13_02225510
	thumb_func_start ov13_02224F3C
ov13_02224F3C: ; 0x02224F3C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	mov r4, #0
	sub r0, r4, #5
	str r0, [sp, #8]
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #1
	str r4, [sp, #0xc]
	str r4, [sp, #4]
	add r6, r4, #0
	str r1, [r0, #0x1c]
	b _022252EE
_02224F54:
	mov r0, #0x7d
	lsl r0, r0, #2
	bl OS_Sleep
	ldr r0, _02225284 ; =ov13_0224DF30
	ldr r0, [r0, #0x1c]
	cmp r0, #0xa
	bls _02224F66
	b _022252EE
_02224F66:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02224F72: ; jump table
	.short _022252EE - _02224F72 - 2 ; case 0
	.short _02224F88 - _02224F72 - 2 ; case 1
	.short _02224FA8 - _02224F72 - 2 ; case 2
	.short _02224FBE - _02224F72 - 2 ; case 3
	.short _0222500C - _02224F72 - 2 ; case 4
	.short _02225074 - _02224F72 - 2 ; case 5
	.short _02225098 - _02224F72 - 2 ; case 6
	.short _02225142 - _02224F72 - 2 ; case 7
	.short _0222519E - _02224F72 - 2 ; case 8
	.short _02225226 - _02224F72 - 2 ; case 9
	.short _022252C4 - _02224F72 - 2 ; case 10
_02224F88:
	bl ov13_02224330
	str r0, [sp, #8]
	cmp r0, #1
	beq _02224F96
	mov r6, #1
	b _022252EE
_02224F96:
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x38]
	bl ov13_02226F3C
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #2
	str r1, [r0, #0x1c]
	b _022252EE
_02224FA8:
	bl ov13_02223FDC
	str r0, [sp, #8]
	cmp r0, #1
	beq _02224FB6
	mov r6, #1
	b _022252EE
_02224FB6:
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x1c]
	b _022252EE
_02224FBE:
	mov r0, #2
	add r1, r0, #0
	mov r2, #0
	bl SOC_Socket
	add r4, r0, #0
	bpl _02224FD6
	mov r0, #1
	mvn r0, r0
	str r0, [sp, #8]
	mov r6, #1
	b _022252EE
_02224FD6:
	add r5, sp, #0x24
	mov r1, #0
	str r1, [r5]
	str r1, [r5, #4]
	mov r3, #8
	add r2, sp, #0x10
	strb r3, [r2, #0x14]
	mov r3, #2
	strb r3, [r2, #0x15]
	ldr r3, _02225288 ; =0x000001E6
	strh r3, [r2, #0x16]
	str r1, [sp, #0x28]
	add r1, r5, #0
	bl SOC_Bind
	str r0, [sp, #8]
	cmp r0, #0
	bge _02225004
	mov r0, #1
	mvn r0, r0
	str r0, [sp, #8]
	mov r6, #1
	b _022252EE
_02225004:
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #4
	str r1, [r0, #0x1c]
	b _022252EE
_0222500C:
	bl ov13_02226CBC
	ldr r1, _0222528C ; =ov13_02245A58
	ldr r1, [r1, #0xc]
	cmp r0, r1
	blo _02225028
	add r0, r4, #0
	bl SOC_Close
	mov r0, #2
	mvn r0, r0
	str r0, [sp, #8]
	mov r6, #1
	b _022252EE
_02225028:
	mov r1, #8
	add r0, sp, #0x10
	strb r1, [r0, #0xc]
	ldr r0, _02225290 ; =ov13_0224E3F0
	add r1, sp, #0x1c
	bl ov13_02224A30
	add r0, sp, #0x1c
	mov r2, #2
	str r0, [sp]
	ldr r1, _02225294 ; =ov13_0224EC64
	add r0, r4, #0
	lsl r2, r2, #0xa
	mov r3, #4
	bl SOC_RecvFrom
	cmp r0, #0
	ble _02225126
	ldr r0, _02225294 ; =ov13_0224EC64
	ldr r1, _02225298 ; =ov13_0224DF80
	bl ov13_02224864
	cmp r0, #0
	beq _02225126
	bl ov13_02226CBC
	ldr r1, _0222529C ; =0x00007530
	add r1, r0, r1
	ldr r0, _0222528C ; =ov13_02245A58
	str r1, [r0, #0xc]
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #5
	str r1, [r0, #0x1c]
	mov r1, #4
	str r1, [r0, #0x38]
	bl ov13_02226F3C
	b _022252EE
_02225074:
	ldr r0, _02225294 ; =ov13_0224EC64
	bl ov13_02224988
	add r3, r0, #0
	ldr r0, _02225284 ; =ov13_0224DF30
	ldr r2, _02225294 ; =ov13_0224EC64
	str r3, [r0, #0x14]
	add r0, r4, #0
	add r1, sp, #0x1c
	bl ov13_02224618
	bl ov13_02226CBC
	str r0, [sp, #0xc]
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #6
	str r1, [r0, #0x1c]
	b _022252EE
_02225098:
	bl ov13_02226CBC
	ldr r1, _0222528C ; =ov13_02245A58
	ldr r1, [r1, #0xc]
	cmp r0, r1
	blo _022250B4
	add r0, r4, #0
	bl SOC_Close
	mov r0, #3
	mvn r0, r0
	str r0, [sp, #8]
	mov r6, #1
	b _022252EE
_022250B4:
	add r0, sp, #0x1c
	mov r2, #2
	str r0, [sp]
	ldr r1, _02225294 ; =ov13_0224EC64
	add r0, r4, #0
	lsl r2, r2, #0xa
	mov r3, #4
	bl SOC_RecvFrom
	cmp r0, #0
	ble _02225128
	ldr r0, _02225294 ; =ov13_0224EC64
	ldr r2, _022252A0 ; =ov13_0224E464
	ldr r3, _02225290 ; =ov13_0224E3F0
	mov r1, #3
	bl ov13_02224938
	cmp r0, #0
	beq _02225128
	ldr r0, _022252A0 ; =ov13_0224E464
	add r1, sp, #0x18
	add r2, sp, #0x14
	bl ov13_022246E4
	add r5, r0, #0
	ldr r1, [sp, #0x18]
	ldr r0, _022252A4 ; =0x00000101
	cmp r1, r0
	bne _02225126
	bl ov13_02226CBC
	ldr r2, _022252A8 ; =ov13_0224E400
	str r0, [sp, #0x10]
	mov r1, #8
_022250F8:
	ldrb r0, [r5]
	add r5, r5, #1
	strb r0, [r2]
	add r2, r2, #1
	sub r1, r1, #1
	bne _022250F8
	ldr r0, _022252AC ; =ov13_0224E408
	add r1, sp, #0x10
	mov r2, #4
	bl ov13_02226C94
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #7
	str r1, [r0, #0x1c]
	mov r1, #5
	str r1, [r0, #0x38]
	ldr r0, _0222528C ; =ov13_02245A58
	sub r1, r1, #6
	str r1, [r0, #0xc]
	bl ov13_02226F3C
_02225126:
	b _022252EE
_02225128:
	bl ov13_02226CBC
	mov r2, #0xfa
	ldr r1, [sp, #0xc]
	lsl r2, r2, #2
	add r1, r1, r2
	cmp r0, r1
	bhs _0222513A
	b _022252EE
_0222513A:
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #5
	str r1, [r0, #0x1c]
	b _022252EE
_02225142:
	ldr r0, _022252A0 ; =ov13_0224E464
	ldr r1, _022252B0 ; =0x00000102
	ldr r2, _022252AC ; =ov13_0224E408
	mov r3, #8
	bl ov13_0222483C
	ldr r3, _02225284 ; =ov13_0224DF30
	ldr r2, _022252A0 ; =ov13_0224E464
	str r0, [r3, #0x34]
	ldr r0, _02225290 ; =ov13_0224E3F0
	mov r1, #4
	str r0, [sp]
	ldr r0, _02225294 ; =ov13_0224EC64
	ldr r3, [r3, #0x34]
	bl ov13_02224718
	add r3, r0, #0
	ldr r0, _02225284 ; =ov13_0224DF30
	ldr r2, _02225294 ; =ov13_0224EC64
	str r3, [r0, #0x14]
	add r0, r4, #0
	add r1, sp, #0x1c
	bl ov13_02224618
	bl ov13_02226CBC
	str r0, [sp, #0xc]
	mov r0, #0
	mov r5, #0x12
	ldr r7, _022252B4 ; =ov13_0224E184
	add r1, r0, #0
	add r2, r0, #0
	add r3, r0, #0
	mov ip, r5
_02225186:
	stmia r7!, {r0, r1, r2, r3}
	mov r5, ip
	sub r5, r5, #1
	stmia r7!, {r0, r1, r2, r3}
	mov ip, r5
	bne _02225186
	stmia r7!, {r0, r1, r2, r3}
	str r0, [r7]
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #8
	str r1, [r0, #0x1c]
	b _022252EE
_0222519E:
	add r0, sp, #0x1c
	mov r2, #2
	str r0, [sp]
	ldr r1, _02225294 ; =ov13_0224EC64
	add r0, r4, #0
	lsl r2, r2, #0xa
	mov r3, #4
	bl SOC_RecvFrom
	cmp r0, #0
	ble _022251F4
	ldr r0, _02225294 ; =ov13_0224EC64
	ldr r2, _022252A0 ; =ov13_0224E464
	ldr r3, _022252A8 ; =ov13_0224E400
	mov r1, #5
	bl ov13_02224938
	ldr r1, _02225284 ; =ov13_0224DF30
	cmp r0, #0
	str r0, [r1, #0x34]
	beq _022251F4
	ldr r0, _022252A0 ; =ov13_0224E464
	bl ov13_02224B2C
	cmp r0, #0
	beq _022251F4
	ldr r1, _022252B8 ; =ov13_0224E270
	mov r0, #0x14
	ldrsb r0, [r1, r0]
	cmp r0, #0
	beq _022251E2
	mov r1, #1
	ldr r0, _02225284 ; =ov13_0224DF30
	b _022251E6
_022251E2:
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #0
_022251E6:
	strb r1, [r0]
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #9
	str r1, [r0, #0x1c]
	b _022252EE
_022251F4:
	bl ov13_02226CBC
	mov r2, #0xfa
	ldr r1, [sp, #0xc]
	lsl r2, r2, #2
	add r1, r1, r2
	cmp r0, r1
	blo _022252EE
	ldr r0, [sp, #4]
	add r0, r0, #1
	str r0, [sp, #4]
	cmp r0, #0xa
	blt _0222521E
	add r0, r4, #0
	bl SOC_Close
	mov r0, #1
	mvn r0, r0
	str r0, [sp, #8]
	mov r6, #1
	b _022252EE
_0222521E:
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #7
	str r1, [r0, #0x1c]
	b _022252EE
_02225226:
	ldr r0, _022252A0 ; =ov13_0224E464
	ldr r1, _022252BC ; =0x00000301
	ldr r2, _022252C0 ; =ov13_0224DF30
	mov r3, #1
	bl ov13_0222483C
	ldr r3, _02225284 ; =ov13_0224DF30
	ldr r2, _022252A0 ; =ov13_0224E464
	str r0, [r3, #0x34]
	ldr r0, _022252A8 ; =ov13_0224E400
	mov r1, #6
	str r0, [sp]
	ldr r0, _02225294 ; =ov13_0224EC64
	ldr r3, [r3, #0x34]
	bl ov13_02224718
	ldr r1, _02225284 ; =ov13_0224DF30
	str r0, [r1, #0x14]
	bl ov13_02223DE0
	cmp r0, #7
	beq _02225268
	bl ov13_02226CBC
	mov r1, #0xfa
	lsl r1, r1, #2
	add r0, r0, r1
	str r0, [sp, #0xc]
	mov r0, #0xa
	ldr r1, _02225284 ; =ov13_0224DF30
	str r0, [sp, #4]
	str r0, [r1, #0x1c]
	b _022252EE
_02225268:
	ldr r3, _02225284 ; =ov13_0224DF30
	ldr r2, _02225294 ; =ov13_0224EC64
	ldr r3, [r3, #0x14]
	add r0, r4, #0
	add r1, sp, #0x1c
	bl ov13_02224618
	bl ov13_02226CBC
	str r0, [sp, #0xc]
	ldr r0, _02225284 ; =ov13_0224DF30
	mov r1, #0xa
	str r1, [r0, #0x1c]
	b _022252EE
	.balign 4, 0
_02225284: .word ov13_0224DF30
_02225288: .word 0x000001E6
_0222528C: .word ov13_02245A58
_02225290: .word ov13_0224E3F0
_02225294: .word ov13_0224EC64
_02225298: .word ov13_0224DF80
_0222529C: .word 0x00007530
_022252A0: .word ov13_0224E464
_022252A4: .word 0x00000101
_022252A8: .word ov13_0224E400
_022252AC: .word ov13_0224E408
_022252B0: .word 0x00000102
_022252B4: .word ov13_0224E184
_022252B8: .word ov13_0224E270
_022252BC: .word 0x00000301
_022252C0: .word ov13_0224DF30
_022252C4:
	bl ov13_02226CBC
	mov r2, #0xfa
	ldr r1, [sp, #0xc]
	lsl r2, r2, #2
	add r1, r1, r2
	cmp r0, r1
	blo _022252EE
	ldr r0, [sp, #4]
	add r0, r0, #1
	str r0, [sp, #4]
	cmp r0, #0xa
	blt _022252E8
	mov r6, #1
	bl ov13_02224DB4
	str r0, [sp, #8]
	b _022252EE
_022252E8:
	ldr r0, _0222531C ; =ov13_0224DF30
	mov r1, #9
	str r1, [r0, #0x1c]
_022252EE:
	cmp r6, #0
	bne _022252FC
	ldr r0, _0222531C ; =ov13_0224DF30
	ldr r0, [r0, #0x10]
	cmp r0, #0
	bne _022252FC
	b _02224F54
_022252FC:
	cmp r4, #0
	beq _02225306
	add r0, r4, #0
	bl SOC_Close
_02225306:
	ldr r0, _0222531C ; =ov13_0224DF30
	ldr r0, [r0, #0x10]
	cmp r0, #0
	beq _02225314
	mov r0, #7
	mvn r0, r0
	str r0, [sp, #8]
_02225314:
	ldr r0, [sp, #8]
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	nop
_0222531C: .word ov13_0224DF30
	thumb_func_end ov13_02224F3C
