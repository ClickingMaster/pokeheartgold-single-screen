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
	.extern ov13_02224A30
	.extern ov13_02224B2C
	.extern ov13_02224D00
	.extern ov13_02224DB4
	.extern ov13_02224F3C
	.extern ov13_02225320
	.extern ov13_02225358
	.extern ov13_02225510
	.extern ov13_022256C8
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


	thumb_func_start ov13_02225710
ov13_02225710: ; 0x02225710
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r3, r1, #0
	add r1, r2, #0
	mov r2, #0
	str r2, [sp]
	ldrb r2, [r3, #2]
	ldrb r6, [r3, #1]
	ldrb r5, [r3, #3]
	lsl r4, r2, #8
	ldrb r2, [r3]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r4
	eor r2, r5
	str r2, [r0]
	ldrb r2, [r3, #6]
	ldrb r6, [r3, #5]
	ldrb r5, [r3, #7]
	lsl r4, r2, #8
	ldrb r2, [r3, #4]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r4
	eor r2, r5
	str r2, [r0, #4]
	ldrb r2, [r3, #0xa]
	ldrb r6, [r3, #9]
	ldrb r5, [r3, #0xb]
	lsl r4, r2, #8
	ldrb r2, [r3, #8]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r4
	eor r2, r5
	str r2, [r0, #8]
	ldrb r2, [r3, #0xe]
	ldrb r6, [r3, #0xd]
	ldrb r5, [r3, #0xf]
	lsl r4, r2, #8
	ldrb r2, [r3, #0xc]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r4
	eor r2, r5
	str r2, [r0, #0xc]
	cmp r1, #0x80
	bne _022257F4
	ldr r5, _022259C0 ; =ov13_02242688
	ldr r2, _022259C4 ; =ov13_022442B0
_0222577C:
	ldr r4, [r0, #0xc]
	ldr r3, [r5]
	lsr r1, r4, #0x18
	lsl r1, r1, #2
	ldr r1, [r2, r1]
	add r5, r5, #4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #8]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x16
	ldr r6, [r2, r1]
	mov r1, #0xff
	lsl r1, r1, #8
	add r7, r6, #0
	and r7, r1
	ldr r1, [r0]
	str r1, [sp, #4]
	lsr r1, r4, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x16
	ldr r6, [r2, r1]
	mov r1, #0xff
	lsl r1, r1, #0x18
	lsr r4, r4, #8
	and r6, r1
	ldr r1, [sp, #4]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	eor r1, r6
	ldr r6, [r2, r4]
	mov r4, #0xff
	lsl r4, r4, #0x10
	and r4, r6
	eor r1, r4
	add r4, r7, #0
	eor r4, r1
	ldr r1, [sp, #8]
	eor r1, r4
	eor r3, r1
	str r3, [r0, #0x10]
	ldr r1, [r0, #4]
	eor r3, r1
	str r3, [r0, #0x14]
	ldr r1, [r0, #8]
	eor r3, r1
	str r3, [r0, #0x18]
	ldr r1, [r0, #0xc]
	eor r1, r3
	str r1, [r0, #0x1c]
	ldr r1, [sp]
	add r1, r1, #1
	str r1, [sp]
	cmp r1, #0xa
	blt _022257F0
	add sp, #0x18
	mov r0, #0xa
	pop {r3, r4, r5, r6, r7, pc}
_022257F0:
	add r0, #0x10
	b _0222577C
_022257F4:
	ldrb r2, [r3, #0x12]
	ldrb r6, [r3, #0x11]
	ldrb r5, [r3, #0x13]
	lsl r4, r2, #8
	ldrb r2, [r3, #0x10]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r4
	eor r2, r5
	str r2, [r0, #0x10]
	ldrb r2, [r3, #0x16]
	ldrb r6, [r3, #0x15]
	ldrb r5, [r3, #0x17]
	lsl r4, r2, #8
	ldrb r2, [r3, #0x14]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r4
	eor r2, r5
	str r2, [r0, #0x14]
	cmp r1, #0xc0
	bne _022258B0
	ldr r5, _022259C0 ; =ov13_02242688
	ldr r3, _022259C4 ; =ov13_022442B0
_02225828:
	ldr r4, [r0, #0x14]
	ldr r6, [r5]
	lsr r1, r4, #0x18
	lsl r1, r1, #2
	ldr r1, [r3, r1]
	add r5, r5, #4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0xc]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x16
	ldr r2, [r3, r1]
	mov r1, #0xff
	lsl r1, r1, #8
	add r7, r2, #0
	and r7, r1
	ldr r1, [r0]
	mov lr, r1
	lsr r1, r4, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x16
	ldr r2, [r3, r1]
	mov r1, #0xff
	lsl r1, r1, #0x18
	and r1, r2
	mov r2, lr
	eor r1, r2
	lsr r2, r4, #8
	lsl r2, r2, #0x18
	lsr r2, r2, #0x16
	ldr r4, [r3, r2]
	mov r2, #0xff
	lsl r2, r2, #0x10
	and r2, r4
	eor r1, r2
	add r2, r7, #0
	eor r2, r1
	ldr r1, [sp, #0xc]
	eor r1, r2
	add r2, r6, #0
	eor r2, r1
	str r2, [r0, #0x18]
	ldr r1, [r0, #4]
	eor r2, r1
	str r2, [r0, #0x1c]
	ldr r1, [r0, #8]
	eor r2, r1
	str r2, [r0, #0x20]
	ldr r1, [r0, #0xc]
	eor r1, r2
	str r1, [r0, #0x24]
	ldr r1, [sp]
	add r1, r1, #1
	str r1, [sp]
	cmp r1, #8
	blt _0222589E
	add sp, #0x18
	mov r0, #0xc
	pop {r3, r4, r5, r6, r7, pc}
_0222589E:
	ldr r2, [r0, #0x10]
	ldr r1, [r0, #0x24]
	eor r2, r1
	str r2, [r0, #0x28]
	ldr r1, [r0, #0x14]
	eor r1, r2
	str r1, [r0, #0x2c]
	add r0, #0x18
	b _02225828
_022258B0:
	ldrb r2, [r3, #0x1a]
	ldrb r6, [r3, #0x19]
	ldrb r5, [r3, #0x1b]
	lsl r4, r2, #8
	ldrb r2, [r3, #0x18]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r4
	eor r2, r5
	str r2, [r0, #0x18]
	ldrb r2, [r3, #0x1e]
	ldrb r5, [r3, #0x1f]
	lsl r4, r2, #8
	ldrb r2, [r3, #0x1c]
	ldrb r3, [r3, #0x1d]
	lsl r2, r2, #0x18
	lsl r3, r3, #0x10
	eor r2, r3
	eor r2, r4
	eor r2, r5
	str r2, [r0, #0x1c]
	mov r2, #1
	lsl r2, r2, #8
	cmp r1, r2
	bne _022259BA
	ldr r5, _022259C0 ; =ov13_02242688
	ldr r3, _022259C4 ; =ov13_022442B0
_022258E8:
	ldr r4, [r0, #0x1c]
	ldr r6, [r5]
	lsr r1, r4, #0x18
	lsl r1, r1, #2
	ldr r1, [r3, r1]
	add r5, r5, #4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0x10]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x16
	ldr r2, [r3, r1]
	mov r1, #0xff
	lsl r1, r1, #8
	add r7, r2, #0
	and r7, r1
	ldr r1, [r0]
	mov ip, r1
	lsr r1, r4, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x16
	ldr r2, [r3, r1]
	mov r1, #0xff
	lsl r1, r1, #0x18
	and r1, r2
	mov r2, ip
	eor r1, r2
	lsr r2, r4, #8
	lsl r2, r2, #0x18
	lsr r2, r2, #0x16
	ldr r4, [r3, r2]
	mov r2, #0xff
	lsl r2, r2, #0x10
	and r2, r4
	eor r1, r2
	add r2, r7, #0
	eor r2, r1
	ldr r1, [sp, #0x10]
	eor r1, r2
	add r2, r6, #0
	eor r2, r1
	str r2, [r0, #0x20]
	ldr r1, [r0, #4]
	eor r2, r1
	str r2, [r0, #0x24]
	ldr r1, [r0, #8]
	eor r2, r1
	str r2, [r0, #0x28]
	ldr r1, [r0, #0xc]
	eor r1, r2
	str r1, [r0, #0x2c]
	ldr r1, [sp]
	add r1, r1, #1
	str r1, [sp]
	cmp r1, #7
	blt _0222595E
	add sp, #0x18
	mov r0, #0xe
	pop {r3, r4, r5, r6, r7, pc}
_0222595E:
	ldr r6, [r0, #0x2c]
	ldr r7, [r0, #0x10]
	lsl r1, r6, #0x18
	lsr r1, r1, #0x16
	ldr r1, [r3, r1]
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	lsr r1, r6, #8
	lsl r1, r1, #0x18
	lsr r1, r1, #0x16
	ldr r2, [r3, r1]
	mov r1, #0xff
	lsl r1, r1, #8
	and r1, r2
	str r1, [sp, #0x14]
	lsr r1, r6, #0x18
	lsl r1, r1, #2
	ldr r2, [r3, r1]
	mov r1, #0xff
	lsl r1, r1, #0x18
	and r1, r2
	lsr r2, r6, #0x10
	lsl r2, r2, #0x18
	lsr r2, r2, #0x16
	ldr r6, [r3, r2]
	mov r2, #0xff
	lsl r2, r2, #0x10
	eor r1, r7
	and r2, r6
	eor r2, r1
	ldr r1, [sp, #0x14]
	eor r1, r2
	add r2, r4, #0
	eor r2, r1
	str r2, [r0, #0x30]
	ldr r1, [r0, #0x14]
	eor r2, r1
	str r2, [r0, #0x34]
	ldr r1, [r0, #0x18]
	eor r2, r1
	str r2, [r0, #0x38]
	ldr r1, [r0, #0x1c]
	eor r1, r2
	str r1, [r0, #0x3c]
	add r0, #0x20
	b _022258E8
_022259BA:
	ldr r0, [sp]
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022259C0: .word ov13_02242688
_022259C4: .word ov13_022442B0
	thumb_func_end ov13_02225710


	thumb_func_start ov13_022259C8
ov13_022259C8: ; 0x022259C8
	push {r3, r4, r5, r6, r7, lr}
	add r4, r0, #0
	bl ov13_02225710
	mov ip, r0
	mov r1, ip
	lsl r6, r1, #2
	mov r0, #0
	cmp r6, #0
	ble _02225A0E
	lsl r2, r6, #2
	add r1, r4, #0
	add r2, r4, r2
_022259E2:
	ldr r5, [r1]
	ldr r3, [r2]
	add r0, r0, #4
	str r3, [r1]
	str r5, [r2]
	ldr r5, [r1, #4]
	ldr r3, [r2, #4]
	sub r6, r6, #4
	str r3, [r1, #4]
	str r5, [r2, #4]
	ldr r5, [r1, #8]
	ldr r3, [r2, #8]
	str r3, [r1, #8]
	str r5, [r2, #8]
	ldr r5, [r1, #0xc]
	ldr r3, [r2, #0xc]
	str r3, [r1, #0xc]
	str r5, [r2, #0xc]
	add r1, #0x10
	sub r2, #0x10
	cmp r0, r6
	blt _022259E2
_02225A0E:
	mov r0, #1
	str r0, [sp]
	mov r0, ip
	cmp r0, #1
	bgt _02225A1A
	b _02225B3E
_02225A1A:
	ldr r2, _02225B44 ; =ov13_022442B0
	ldr r7, _02225B48 ; =ov13_02242AB0
_02225A1E:
	add r4, #0x10
	ldr r3, [r4]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r2, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r1, [r7, r0]
	lsr r0, r3, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r2, r0]
	lsl r0, r0, #0x18
	lsr r5, r0, #0x16
	ldr r0, _02225B4C ; =ov13_022426B0
	ldr r6, [r0, r5]
	lsr r0, r3, #0x18
	lsl r0, r0, #2
	lsr r3, r3, #0x10
	ldr r0, [r2, r0]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x16
	lsl r0, r0, #0x18
	ldr r3, [r2, r3]
	lsr r5, r0, #0x16
	ldr r0, _02225B50 ; =ov13_022446B0
	lsl r3, r3, #0x18
	ldr r0, [r0, r5]
	lsr r5, r3, #0x16
	ldr r3, _02225B54 ; =ov13_02244AB0
	ldr r3, [r3, r5]
	eor r0, r3
	eor r0, r6
	eor r0, r1
	str r0, [r4]
	ldr r3, [r4, #4]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r2, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r1, [r7, r0]
	lsr r0, r3, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r2, r0]
	lsl r0, r0, #0x18
	lsr r5, r0, #0x16
	ldr r0, _02225B4C ; =ov13_022426B0
	ldr r6, [r0, r5]
	lsr r0, r3, #0x18
	lsl r0, r0, #2
	lsr r3, r3, #0x10
	ldr r0, [r2, r0]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x16
	lsl r0, r0, #0x18
	ldr r3, [r2, r3]
	lsr r5, r0, #0x16
	ldr r0, _02225B50 ; =ov13_022446B0
	lsl r3, r3, #0x18
	ldr r0, [r0, r5]
	lsr r5, r3, #0x16
	ldr r3, _02225B54 ; =ov13_02244AB0
	ldr r3, [r3, r5]
	eor r0, r3
	eor r0, r6
	eor r0, r1
	str r0, [r4, #4]
	ldr r3, [r4, #8]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r2, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r1, [r7, r0]
	lsr r0, r3, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r2, r0]
	lsl r0, r0, #0x18
	lsr r5, r0, #0x16
	ldr r0, _02225B4C ; =ov13_022426B0
	ldr r6, [r0, r5]
	lsr r0, r3, #0x18
	lsl r0, r0, #2
	lsr r3, r3, #0x10
	ldr r0, [r2, r0]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x16
	lsl r0, r0, #0x18
	ldr r3, [r2, r3]
	lsr r5, r0, #0x16
	ldr r0, _02225B50 ; =ov13_022446B0
	lsl r3, r3, #0x18
	ldr r0, [r0, r5]
	lsr r5, r3, #0x16
	ldr r3, _02225B54 ; =ov13_02244AB0
	ldr r3, [r3, r5]
	eor r0, r3
	eor r0, r6
	eor r0, r1
	str r0, [r4, #8]
	ldr r3, [r4, #0xc]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r2, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r1, [r7, r0]
	lsr r0, r3, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r2, r0]
	lsl r0, r0, #0x18
	lsr r5, r0, #0x16
	ldr r0, _02225B4C ; =ov13_022426B0
	ldr r6, [r0, r5]
	lsr r0, r3, #0x18
	lsl r0, r0, #2
	lsr r3, r3, #0x10
	ldr r0, [r2, r0]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x16
	lsl r0, r0, #0x18
	ldr r3, [r2, r3]
	lsr r5, r0, #0x16
	ldr r0, _02225B50 ; =ov13_022446B0
	lsl r3, r3, #0x18
	ldr r0, [r0, r5]
	lsr r5, r3, #0x16
	ldr r3, _02225B54 ; =ov13_02244AB0
	ldr r3, [r3, r5]
	eor r0, r3
	eor r0, r6
	eor r0, r1
	str r0, [r4, #0xc]
	ldr r0, [sp]
	add r1, r0, #1
	mov r0, ip
	str r1, [sp]
	cmp r1, r0
	bge _02225B3E
	b _02225A1E
_02225B3E:
	mov r0, ip
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02225B44: .word ov13_022442B0
_02225B48: .word ov13_02242AB0
_02225B4C: .word ov13_022426B0
_02225B50: .word ov13_022446B0
_02225B54: .word ov13_02244AB0
	thumb_func_end ov13_022259C8


	thumb_func_start ov13_02225B58
ov13_02225B58: ; 0x02225B58
	push {r4, r5, r6, r7, lr}
	sub sp, #0x4c
	add r7, r1, #0
	add r1, r2, #0
	ldrb r2, [r1, #2]
	str r3, [sp]
	ldrb r6, [r1, #1]
	lsl r3, r2, #8
	ldrb r2, [r1]
	ldrb r4, [r1, #3]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r3
	eor r2, r4
	ldr r5, [r0]
	ldrb r6, [r1, #5]
	eor r2, r5
	str r2, [sp, #0xc]
	ldrb r2, [r1, #6]
	ldrb r4, [r1, #7]
	lsl r6, r6, #0x10
	lsl r3, r2, #8
	ldrb r2, [r1, #4]
	ldr r5, [r0, #4]
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r3
	eor r2, r4
	eor r2, r5
	str r2, [sp, #8]
	ldrb r2, [r1, #0xa]
	ldrb r6, [r1, #9]
	ldrb r4, [r1, #0xb]
	lsl r3, r2, #8
	ldrb r2, [r1, #8]
	lsl r6, r6, #0x10
	ldr r5, [r0, #8]
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r3
	eor r2, r4
	eor r5, r2
	ldrb r2, [r1, #0xe]
	ldrb r4, [r1, #0xf]
	ldr r6, [r0, #0xc]
	lsl r3, r2, #8
	ldrb r2, [r1, #0xc]
	ldrb r1, [r1, #0xd]
	lsl r2, r2, #0x18
	lsl r1, r1, #0x10
	eor r1, r2
	eor r1, r3
	eor r1, r4
	eor r6, r1
	asr r1, r7, #1
	str r1, [sp, #4]
_02225BCA:
	lsl r1, r6, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F00 ; =ov13_02243EB0
	ldr r4, [r0, #0x10]
	ldr r3, [r1, r2]
	lsr r1, r5, #8
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F04 ; =ov13_02243AB0
	ldr r1, [r1, r2]
	str r1, [sp, #0x40]
	ldr r1, [sp, #0xc]
	lsr r1, r1, #0x18
	lsl r2, r1, #2
	ldr r1, _02225F08 ; =ov13_022432B0
	ldr r7, [r1, r2]
	ldr r1, [sp, #8]
	lsr r1, r1, #0x10
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F0C ; =ov13_022436B0
	ldr r1, [r1, r2]
	add r2, r7, #0
	eor r2, r1
	ldr r1, [sp, #0x40]
	eor r1, r2
	eor r1, r3
	eor r1, r4
	str r1, [sp, #0x10]
	ldr r1, [sp, #0xc]
	ldr r4, [r0, #0x14]
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F00 ; =ov13_02243EB0
	ldr r3, [r1, r2]
	lsr r1, r6, #8
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F04 ; =ov13_02243AB0
	ldr r1, [r1, r2]
	str r1, [sp, #0x48]
	ldr r1, [sp, #8]
	lsr r1, r1, #0x18
	lsl r2, r1, #2
	ldr r1, _02225F08 ; =ov13_022432B0
	ldr r7, [r1, r2]
	lsr r1, r5, #0x10
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F0C ; =ov13_022436B0
	ldr r1, [r1, r2]
	add r2, r7, #0
	eor r2, r1
	ldr r1, [sp, #0x48]
	eor r1, r2
	eor r1, r3
	eor r1, r4
	str r1, [sp, #0x44]
	ldr r1, [sp, #8]
	ldr r4, [r0, #0x18]
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F00 ; =ov13_02243EB0
	ldr r3, [r1, r2]
	ldr r1, [sp, #0xc]
	lsr r1, r1, #8
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F04 ; =ov13_02243AB0
	ldr r1, [r1, r2]
	str r1, [sp, #0x14]
	lsr r1, r5, #0x18
	lsl r2, r1, #2
	ldr r1, _02225F08 ; =ov13_022432B0
	ldr r7, [r1, r2]
	lsr r1, r6, #0x10
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _02225F0C ; =ov13_022436B0
	ldr r1, [r1, r2]
	add r2, r7, #0
	eor r2, r1
	ldr r1, [sp, #0x14]
	ldr r7, [r0, #0x1c]
	eor r1, r2
	eor r1, r3
	add r2, r4, #0
	eor r2, r1
	lsl r1, r5, #0x18
	lsr r3, r1, #0x16
	ldr r1, _02225F00 ; =ov13_02243EB0
	add r0, #0x20
	ldr r3, [r1, r3]
	ldr r1, [sp, #8]
	lsr r1, r1, #8
	lsl r1, r1, #0x18
	lsr r4, r1, #0x16
	ldr r1, _02225F04 ; =ov13_02243AB0
	ldr r1, [r1, r4]
	lsr r4, r6, #0x18
	lsl r5, r4, #2
	ldr r4, _02225F08 ; =ov13_022432B0
	ldr r6, [r4, r5]
	ldr r4, [sp, #0xc]
	lsr r4, r4, #0x10
	lsl r4, r4, #0x18
	lsr r5, r4, #0x16
	ldr r4, _02225F0C ; =ov13_022436B0
	ldr r4, [r4, r5]
	eor r4, r6
	eor r1, r4
	eor r1, r3
	ldr r3, [sp, #4]
	eor r1, r7
	sub r3, r3, #1
	str r3, [sp, #4]
	beq _02225D94
	lsl r3, r1, #0x18
	lsr r5, r3, #0x16
	ldr r3, _02225F00 ; =ov13_02243EB0
	ldr r4, [r0]
	ldr r3, [r3, r5]
	lsr r5, r2, #8
	lsl r5, r5, #0x18
	lsr r6, r5, #0x16
	ldr r5, _02225F04 ; =ov13_02243AB0
	ldr r7, [r5, r6]
	ldr r5, [sp, #0x10]
	lsr r5, r5, #0x18
	lsl r6, r5, #2
	ldr r5, _02225F08 ; =ov13_022432B0
	ldr r5, [r5, r6]
	str r5, [sp, #0x18]
	ldr r5, [sp, #0x44]
	lsr r5, r5, #0x10
	lsl r5, r5, #0x18
	lsr r6, r5, #0x16
	ldr r5, _02225F0C ; =ov13_022436B0
	ldr r6, [r5, r6]
	ldr r5, [sp, #0x18]
	eor r5, r6
	eor r5, r7
	eor r3, r5
	eor r3, r4
	str r3, [sp, #0xc]
	ldr r3, [sp, #0x10]
	ldr r4, [r0, #4]
	lsl r3, r3, #0x18
	lsr r5, r3, #0x16
	ldr r3, _02225F00 ; =ov13_02243EB0
	ldr r3, [r3, r5]
	lsr r5, r1, #8
	lsl r5, r5, #0x18
	lsr r6, r5, #0x16
	ldr r5, _02225F04 ; =ov13_02243AB0
	ldr r7, [r5, r6]
	ldr r5, [sp, #0x44]
	lsr r5, r5, #0x18
	lsl r6, r5, #2
	ldr r5, _02225F08 ; =ov13_022432B0
	ldr r5, [r5, r6]
	str r5, [sp, #0x1c]
	lsr r5, r2, #0x10
	lsl r5, r5, #0x18
	lsr r6, r5, #0x16
	ldr r5, _02225F0C ; =ov13_022436B0
	ldr r6, [r5, r6]
	ldr r5, [sp, #0x1c]
	eor r5, r6
	eor r5, r7
	eor r3, r5
	eor r3, r4
	str r3, [sp, #8]
	ldr r3, [sp, #0x44]
	ldr r6, [r0, #8]
	lsl r3, r3, #0x18
	lsr r4, r3, #0x16
	ldr r3, _02225F00 ; =ov13_02243EB0
	ldr r5, [r3, r4]
	ldr r3, [sp, #0x10]
	lsr r3, r3, #8
	lsl r3, r3, #0x18
	lsr r4, r3, #0x16
	ldr r3, _02225F04 ; =ov13_02243AB0
	ldr r3, [r3, r4]
	str r3, [sp, #0x20]
	lsr r3, r2, #0x18
	lsl r4, r3, #2
	ldr r3, _02225F08 ; =ov13_022432B0
	lsl r2, r2, #0x18
	ldr r7, [r3, r4]
	lsr r3, r1, #0x10
	lsl r3, r3, #0x18
	lsr r4, r3, #0x16
	ldr r3, _02225F0C ; =ov13_022436B0
	lsr r1, r1, #0x18
	ldr r3, [r3, r4]
	add r4, r7, #0
	eor r4, r3
	ldr r3, [sp, #0x20]
	ldr r7, [r0, #0xc]
	eor r3, r4
	eor r3, r5
	add r5, r6, #0
	eor r5, r3
	lsr r3, r2, #0x16
	ldr r2, _02225F00 ; =ov13_02243EB0
	ldr r3, [r2, r3]
	ldr r2, [sp, #0x44]
	lsr r2, r2, #8
	lsl r2, r2, #0x18
	lsr r4, r2, #0x16
	ldr r2, _02225F04 ; =ov13_02243AB0
	ldr r2, [r2, r4]
	lsl r4, r1, #2
	ldr r1, _02225F08 ; =ov13_022432B0
	ldr r6, [r1, r4]
	ldr r1, [sp, #0x10]
	lsr r1, r1, #0x10
	lsl r1, r1, #0x18
	lsr r4, r1, #0x16
	ldr r1, _02225F0C ; =ov13_022436B0
	ldr r1, [r1, r4]
	eor r1, r6
	eor r1, r2
	eor r1, r3
	add r6, r7, #0
	eor r6, r1
	b _02225BCA
_02225D94:
	ldr r3, [r0]
	mov r6, #0xff
	str r3, [sp, #0x24]
	lsl r3, r1, #0x18
	lsr r4, r3, #0x16
	ldr r3, _02225F10 ; =ov13_022442B0
	lsl r6, r6, #0x10
	ldr r4, [r3, r4]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #0x28]
	lsr r4, r2, #8
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #8
	and r4, r5
	str r4, [sp, #0x2c]
	ldr r4, [sp, #0x10]
	lsr r4, r4, #0x18
	lsl r4, r4, #2
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #0x18
	and r5, r4
	ldr r4, [sp, #0x44]
	lsr r4, r4, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r4, [r3, r4]
	and r4, r6
	eor r5, r4
	ldr r4, [sp, #0x2c]
	eor r5, r4
	ldr r4, [sp, #0x28]
	eor r5, r4
	ldr r4, [sp, #0x24]
	eor r4, r5
	ldr r5, [sp]
	lsr r6, r4, #0x18
	strb r6, [r5]
	lsr r6, r4, #0x10
	strb r6, [r5, #1]
	lsr r6, r4, #8
	strb r6, [r5, #2]
	strb r4, [r5, #3]
	ldr r4, [r0, #4]
	mov r6, #0xff
	str r4, [sp, #0x30]
	ldr r4, [sp, #0x10]
	lsl r6, r6, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r4, [r3, r4]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #0x34]
	lsr r4, r1, #8
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #8
	and r4, r5
	str r4, [sp, #0x38]
	ldr r4, [sp, #0x44]
	lsr r4, r4, #0x18
	lsl r4, r4, #2
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #0x18
	and r5, r4
	lsr r4, r2, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r4, [r3, r4]
	and r4, r6
	eor r5, r4
	ldr r4, [sp, #0x38]
	eor r5, r4
	ldr r4, [sp, #0x34]
	eor r5, r4
	ldr r4, [sp, #0x30]
	eor r4, r5
	ldr r5, [sp]
	lsr r6, r4, #0x18
	strb r6, [r5, #4]
	lsr r6, r4, #0x10
	strb r6, [r5, #5]
	lsr r6, r4, #8
	strb r6, [r5, #6]
	strb r4, [r5, #7]
	ldr r4, [r0, #8]
	str r4, [sp, #0x3c]
	ldr r4, [sp, #0x44]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r4, [r3, r4]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov lr, r4
	ldr r4, [sp, #0x10]
	lsr r4, r4, #8
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #8
	and r4, r5
	mov ip, r4
	lsr r4, r2, #0x18
	lsl r4, r4, #2
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #0x18
	add r6, r5, #0
	and r6, r4
	lsr r4, r1, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #0x10
	and r4, r5
	eor r4, r6
	mov r5, ip
	eor r5, r4
	mov r4, lr
	eor r5, r4
	ldr r4, [sp, #0x3c]
	eor r4, r5
	ldr r5, [sp]
	lsr r6, r4, #0x18
	strb r6, [r5, #8]
	lsr r6, r4, #0x10
	strb r6, [r5, #9]
	lsr r6, r4, #8
	strb r6, [r5, #0xa]
	strb r4, [r5, #0xb]
	ldr r5, [r0, #0xc]
	lsl r0, r2, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r3, r0]
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0x44]
	lsr r0, r0, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r2, [r3, r0]
	mov r0, #0xff
	lsl r0, r0, #8
	and r2, r0
	lsr r0, r1, #0x18
	lsl r0, r0, #2
	ldr r1, [r3, r0]
	mov r0, #0xff
	lsl r0, r0, #0x18
	and r0, r1
	ldr r1, [sp, #0x10]
	lsr r1, r1, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x16
	ldr r3, [r3, r1]
	mov r1, #0xff
	lsl r1, r1, #0x10
	and r1, r3
	eor r0, r1
	eor r0, r2
	eor r0, r4
	eor r0, r5
	ldr r1, [sp]
	lsr r2, r0, #0x18
	strb r2, [r1, #0xc]
	lsr r2, r0, #0x10
	strb r2, [r1, #0xd]
	lsr r2, r0, #8
	strb r2, [r1, #0xe]
	strb r0, [r1, #0xf]
	add sp, #0x4c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02225F00: .word ov13_02243EB0
_02225F04: .word ov13_02243AB0
_02225F08: .word ov13_022432B0
_02225F0C: .word ov13_022436B0
_02225F10: .word ov13_022442B0
	thumb_func_end ov13_02225B58


	thumb_func_start ov13_02225F14
ov13_02225F14: ; 0x02225F14
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x48
	add r7, r1, #0
	add r1, r2, #0
	ldrb r2, [r1, #2]
	str r3, [sp]
	ldrb r6, [r1, #1]
	lsl r3, r2, #8
	ldrb r2, [r1]
	ldrb r4, [r1, #3]
	lsl r6, r6, #0x10
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r3
	eor r2, r4
	ldr r5, [r0]
	ldrb r6, [r1, #5]
	eor r2, r5
	str r2, [sp, #0xc]
	ldrb r2, [r1, #6]
	ldrb r4, [r1, #7]
	lsl r6, r6, #0x10
	lsl r3, r2, #8
	ldrb r2, [r1, #4]
	ldr r5, [r0, #4]
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r3
	eor r2, r4
	eor r2, r5
	str r2, [sp, #8]
	ldrb r2, [r1, #0xa]
	ldrb r6, [r1, #9]
	ldrb r4, [r1, #0xb]
	lsl r3, r2, #8
	ldrb r2, [r1, #8]
	lsl r6, r6, #0x10
	ldr r5, [r0, #8]
	lsl r2, r2, #0x18
	eor r2, r6
	eor r2, r3
	eor r2, r4
	eor r2, r5
	str r2, [sp, #0x40]
	ldrb r2, [r1, #0xe]
	ldr r5, [r0, #0xc]
	ldrb r4, [r1, #0xf]
	lsl r3, r2, #8
	ldrb r2, [r1, #0xc]
	ldrb r1, [r1, #0xd]
	add r6, r5, #0
	lsl r2, r2, #0x18
	lsl r1, r1, #0x10
	eor r1, r2
	eor r1, r3
	eor r1, r4
	eor r6, r1
	asr r1, r7, #1
	str r1, [sp, #4]
_02225F8A:
	ldr r1, [sp, #8]
	ldr r5, [r0, #0x10]
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _022262BC ; =ov13_02242AB0
	ldr r4, [r1, r2]
	ldr r1, [sp, #0x40]
	lsr r1, r1, #8
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _022262C0 ; =ov13_022426B0
	ldr r3, [r1, r2]
	ldr r1, [sp, #0xc]
	lsr r1, r1, #0x18
	lsl r2, r1, #2
	ldr r1, _022262C4 ; =ov13_022446B0
	ldr r7, [r1, r2]
	lsr r1, r6, #0x10
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _022262C8 ; =ov13_02244AB0
	ldr r1, [r1, r2]
	eor r1, r7
	eor r1, r3
	eor r1, r4
	eor r1, r5
	str r1, [sp, #0x10]
	ldr r1, [sp, #0x40]
	ldr r5, [r0, #0x14]
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _022262BC ; =ov13_02242AB0
	ldr r4, [r1, r2]
	lsr r1, r6, #8
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _022262C0 ; =ov13_022426B0
	ldr r3, [r1, r2]
	ldr r1, [sp, #8]
	lsr r1, r1, #0x18
	lsl r2, r1, #2
	ldr r1, _022262C4 ; =ov13_022446B0
	ldr r7, [r1, r2]
	ldr r1, [sp, #0xc]
	lsr r1, r1, #0x10
	lsl r1, r1, #0x18
	lsr r2, r1, #0x16
	ldr r1, _022262C8 ; =ov13_02244AB0
	ldr r1, [r1, r2]
	eor r1, r7
	eor r1, r3
	eor r1, r4
	eor r1, r5
	str r1, [sp, #0x44]
	lsl r1, r6, #0x18
	lsr r2, r1, #0x16
	ldr r1, _022262BC ; =ov13_02242AB0
	ldr r3, [r0, #0x18]
	ldr r2, [r1, r2]
	ldr r1, [sp, #0xc]
	lsr r1, r1, #8
	lsl r1, r1, #0x18
	lsr r4, r1, #0x16
	ldr r1, _022262C0 ; =ov13_022426B0
	ldr r1, [r1, r4]
	str r1, [sp, #0x14]
	ldr r1, [sp, #0x40]
	lsr r1, r1, #0x18
	lsl r4, r1, #2
	ldr r1, _022262C4 ; =ov13_022446B0
	ldr r1, [r1, r4]
	ldr r4, [sp, #8]
	lsr r4, r4, #0x10
	lsl r4, r4, #0x18
	lsr r5, r4, #0x16
	ldr r4, _022262C8 ; =ov13_02244AB0
	ldr r4, [r4, r5]
	eor r4, r1
	ldr r1, [sp, #0x14]
	eor r1, r4
	eor r1, r2
	add r2, r3, #0
	eor r2, r1
	ldr r1, [sp, #0xc]
	ldr r3, [r0, #0x1c]
	lsl r1, r1, #0x18
	lsr r4, r1, #0x16
	ldr r1, _022262BC ; =ov13_02242AB0
	add r0, #0x20
	ldr r1, [r1, r4]
	ldr r4, [sp, #8]
	lsr r4, r4, #8
	lsl r4, r4, #0x18
	lsr r5, r4, #0x16
	ldr r4, _022262C0 ; =ov13_022426B0
	ldr r7, [r4, r5]
	lsr r4, r6, #0x18
	lsl r5, r4, #2
	ldr r4, _022262C4 ; =ov13_022446B0
	ldr r6, [r4, r5]
	ldr r4, [sp, #0x40]
	lsr r4, r4, #0x10
	lsl r4, r4, #0x18
	lsr r5, r4, #0x16
	ldr r4, _022262C8 ; =ov13_02244AB0
	ldr r4, [r4, r5]
	eor r4, r6
	eor r4, r7
	eor r1, r4
	eor r1, r3
	ldr r3, [sp, #4]
	sub r3, r3, #1
	str r3, [sp, #4]
	beq _0222614E
	ldr r3, [sp, #0x44]
	ldr r4, [r0]
	lsl r3, r3, #0x18
	lsr r5, r3, #0x16
	ldr r3, _022262BC ; =ov13_02242AB0
	ldr r3, [r3, r5]
	lsr r5, r2, #8
	lsl r5, r5, #0x18
	lsr r6, r5, #0x16
	ldr r5, _022262C0 ; =ov13_022426B0
	ldr r7, [r5, r6]
	ldr r5, [sp, #0x10]
	lsr r5, r5, #0x18
	lsl r6, r5, #2
	ldr r5, _022262C4 ; =ov13_022446B0
	ldr r5, [r5, r6]
	str r5, [sp, #0x18]
	lsr r5, r1, #0x10
	lsl r5, r5, #0x18
	lsr r6, r5, #0x16
	ldr r5, _022262C8 ; =ov13_02244AB0
	ldr r6, [r5, r6]
	ldr r5, [sp, #0x18]
	eor r5, r6
	eor r5, r7
	eor r3, r5
	eor r3, r4
	str r3, [sp, #0xc]
	lsl r3, r2, #0x18
	lsr r5, r3, #0x16
	ldr r3, _022262BC ; =ov13_02242AB0
	ldr r4, [r0, #4]
	ldr r3, [r3, r5]
	lsr r5, r1, #8
	lsl r5, r5, #0x18
	lsr r6, r5, #0x16
	ldr r5, _022262C0 ; =ov13_022426B0
	ldr r7, [r5, r6]
	ldr r5, [sp, #0x44]
	lsr r5, r5, #0x18
	lsl r6, r5, #2
	ldr r5, _022262C4 ; =ov13_022446B0
	ldr r5, [r5, r6]
	str r5, [sp, #0x1c]
	ldr r5, [sp, #0x10]
	lsr r5, r5, #0x10
	lsl r5, r5, #0x18
	lsr r6, r5, #0x16
	ldr r5, _022262C8 ; =ov13_02244AB0
	ldr r6, [r5, r6]
	ldr r5, [sp, #0x1c]
	eor r5, r6
	eor r5, r7
	eor r3, r5
	eor r3, r4
	str r3, [sp, #8]
	lsl r3, r1, #0x18
	lsr r4, r3, #0x16
	ldr r3, _022262BC ; =ov13_02242AB0
	ldr r6, [r0, #8]
	ldr r5, [r3, r4]
	ldr r3, [sp, #0x10]
	lsr r1, r1, #0x18
	lsr r3, r3, #8
	lsl r3, r3, #0x18
	lsr r4, r3, #0x16
	ldr r3, _022262C0 ; =ov13_022426B0
	ldr r3, [r3, r4]
	str r3, [sp, #0x20]
	lsr r3, r2, #0x18
	lsl r4, r3, #2
	ldr r3, _022262C4 ; =ov13_022446B0
	lsr r2, r2, #0x10
	ldr r7, [r3, r4]
	ldr r3, [sp, #0x44]
	lsl r2, r2, #0x18
	lsr r3, r3, #0x10
	lsl r3, r3, #0x18
	lsr r4, r3, #0x16
	ldr r3, _022262C8 ; =ov13_02244AB0
	ldr r3, [r3, r4]
	add r4, r7, #0
	eor r4, r3
	ldr r3, [sp, #0x20]
	eor r3, r4
	eor r3, r5
	eor r3, r6
	str r3, [sp, #0x40]
	ldr r3, [sp, #0x10]
	ldr r5, [r0, #0xc]
	lsl r3, r3, #0x18
	lsr r4, r3, #0x16
	ldr r3, _022262BC ; =ov13_02242AB0
	ldr r4, [r3, r4]
	ldr r3, [sp, #0x44]
	lsr r3, r3, #8
	lsl r3, r3, #0x18
	lsr r6, r3, #0x16
	ldr r3, _022262C0 ; =ov13_022426B0
	ldr r3, [r3, r6]
	lsl r6, r1, #2
	ldr r1, _022262C4 ; =ov13_022446B0
	ldr r1, [r1, r6]
	lsr r6, r2, #0x16
	ldr r2, _022262C8 ; =ov13_02244AB0
	ldr r2, [r2, r6]
	add r6, r5, #0
	eor r1, r2
	eor r1, r3
	eor r1, r4
	eor r6, r1
	b _02225F8A
_0222614E:
	ldr r3, [r0]
	mov r6, #0xff
	str r3, [sp, #0x24]
	ldr r3, [sp, #0x44]
	lsl r6, r6, #0x10
	lsl r3, r3, #0x18
	lsr r4, r3, #0x16
	ldr r3, _022262CC ; =ov13_02242EB0
	ldr r4, [r3, r4]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #0x28]
	lsr r4, r2, #8
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #8
	and r4, r5
	str r4, [sp, #0x2c]
	ldr r4, [sp, #0x10]
	lsr r4, r4, #0x18
	lsl r4, r4, #2
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #0x18
	and r5, r4
	lsr r4, r1, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r4, [r3, r4]
	and r4, r6
	eor r5, r4
	ldr r4, [sp, #0x2c]
	eor r5, r4
	ldr r4, [sp, #0x28]
	eor r5, r4
	ldr r4, [sp, #0x24]
	eor r4, r5
	ldr r5, [sp]
	lsr r6, r4, #0x18
	strb r6, [r5]
	lsr r6, r4, #0x10
	strb r6, [r5, #1]
	lsr r6, r4, #8
	strb r6, [r5, #2]
	strb r4, [r5, #3]
	ldr r4, [r0, #4]
	mov r6, #0xff
	str r4, [sp, #0x30]
	lsl r4, r2, #0x18
	lsr r4, r4, #0x16
	ldr r4, [r3, r4]
	lsl r6, r6, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #0x34]
	lsr r4, r1, #8
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #8
	and r4, r5
	str r4, [sp, #0x38]
	ldr r4, [sp, #0x44]
	lsr r4, r4, #0x18
	lsl r4, r4, #2
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #0x18
	and r5, r4
	ldr r4, [sp, #0x10]
	lsr r4, r4, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r4, [r3, r4]
	and r4, r6
	eor r5, r4
	ldr r4, [sp, #0x38]
	eor r5, r4
	ldr r4, [sp, #0x34]
	eor r5, r4
	ldr r4, [sp, #0x30]
	eor r4, r5
	ldr r5, [sp]
	lsr r6, r4, #0x18
	strb r6, [r5, #4]
	lsr r6, r4, #0x10
	strb r6, [r5, #5]
	lsr r6, r4, #8
	strb r6, [r5, #6]
	strb r4, [r5, #7]
	ldr r4, [r0, #8]
	str r4, [sp, #0x3c]
	lsl r4, r1, #0x18
	lsr r4, r4, #0x16
	ldr r4, [r3, r4]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov lr, r4
	ldr r4, [sp, #0x10]
	lsr r4, r4, #8
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #8
	and r4, r5
	mov ip, r4
	lsr r4, r2, #0x18
	lsl r4, r4, #2
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #0x18
	add r6, r5, #0
	and r6, r4
	ldr r4, [sp, #0x44]
	lsr r4, r4, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x16
	ldr r5, [r3, r4]
	mov r4, #0xff
	lsl r4, r4, #0x10
	and r4, r5
	eor r4, r6
	mov r5, ip
	eor r5, r4
	mov r4, lr
	eor r5, r4
	ldr r4, [sp, #0x3c]
	eor r4, r5
	ldr r5, [sp]
	lsr r6, r4, #0x18
	strb r6, [r5, #8]
	lsr r6, r4, #0x10
	strb r6, [r5, #9]
	lsr r6, r4, #8
	strb r6, [r5, #0xa]
	strb r4, [r5, #0xb]
	ldr r5, [r0, #0xc]
	ldr r0, [sp, #0x10]
	lsr r2, r2, #0x10
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r0, [r3, r0]
	lsr r1, r1, #0x18
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0x44]
	lsl r2, r2, #0x18
	lsr r0, r0, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x16
	ldr r6, [r3, r0]
	mov r0, #0xff
	lsl r0, r0, #8
	lsl r1, r1, #2
	and r0, r6
	mov r6, #0xff
	lsr r2, r2, #0x16
	ldr r1, [r3, r1]
	ldr r3, [r3, r2]
	mov r2, #0xff
	lsl r6, r6, #0x18
	lsl r2, r2, #0x10
	and r1, r6
	and r2, r3
	eor r1, r2
	eor r0, r1
	eor r0, r4
	eor r0, r5
	ldr r1, [sp]
	lsr r2, r0, #0x18
	strb r2, [r1, #0xc]
	lsr r2, r0, #0x10
	strb r2, [r1, #0xd]
	lsr r2, r0, #8
	strb r2, [r1, #0xe]
	strb r0, [r1, #0xf]
	add sp, #0x48
	pop {r3, r4, r5, r6, r7, pc}
	nop
_022262BC: .word ov13_02242AB0
_022262C0: .word ov13_022426B0
_022262C4: .word ov13_022446B0
_022262C8: .word ov13_02244AB0
_022262CC: .word ov13_02242EB0
	thumb_func_end ov13_02225F14
