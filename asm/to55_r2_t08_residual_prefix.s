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


	.extern ov13_022224CC
	.extern ov13_02222534

	thumb_func_start ov13_02222108
ov13_02222108: ; 0x02222108
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r7, r1, #0
	mov r1, #0
	add r0, sp, #0x10
	strb r1, [r0]
	strh r1, [r0, #4]
	strh r1, [r0, #2]
	ldr r0, _0222223C ; =_0224CF98
	str r2, [sp, #0xc]
	ldr r5, [r0, #0x14]
	ldr r2, _02222240 ; =0x000005DC
	add r0, r5, #0
	bl ov13_02222978
	mov r0, #0x21
	lsl r0, r0, #4
	bl ov13_022208E8
	add r4, r0, #0
	bne _02222140
	mov r0, #2
	bl ov13_022214AC
	mov r0, #0
	add sp, #0x20
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_02222140:
	mov r2, #0x21
	mov r1, #0
	lsl r2, r2, #4
	bl ov13_02222978
	add r6, r5, #0
	ldr r0, _02222244 ; =ov13_0224DCF8
	add r1, r7, #0
	mov r2, #8
	add r6, #0x18
	bl ov13_02222968
	add r0, sp, #0x14
	ldr r1, _02222244 ; =ov13_0224DCF8
	add r0, #2
	mov r2, #8
	bl ov13_02222968
	add r0, r4, #4
	bl ov13_02222394
	add r1, sp, #0x10
	strh r0, [r1, #4]
	mov r0, #4
	ldrsh r0, [r1, r0]
	cmp r0, #0
	bge _0222218E
	mov r0, #3
	bl ov13_022214AC
	cmp r4, #0
	beq _02222186
	add r0, r4, #0
	bl ov13_022208F8
_02222186:
	mov r0, #0
	add sp, #0x20
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0222218E:
	mov r0, #0
	strb r0, [r4]
	ldrh r0, [r1, #4]
	bl ov13_02222A44
	strh r0, [r4, #2]
	add r1, sp, #0x10
	mov r0, #4
	ldrsh r0, [r1, r0]
	add r2, r4, #0
	add r3, sp, #0x14
	add r0, r0, #4
	strh r0, [r1, #4]
	add r0, sp, #0x10
	add r0, #2
	str r0, [sp]
	add r0, sp, #0x10
	str r0, [sp, #4]
	mov r0, #0
	add r1, r6, #0
	bl ov13_02222420
	add r1, sp, #0x10
	mov r0, #2
	ldrsh r2, [r1, r0]
	mov r0, #0x10
	mov r3, #6
	orr r0, r2
	strh r0, [r1, #2]
	add r0, sp, #0x14
	ldr r2, _02222248 ; =ov13_02245A20
	add r0, #2
	mov r1, #8
	bl ov13_022227A0
	cmp r0, #0
	beq _022221F0
	mov r0, #2
	bl ov13_022214AC
	cmp r4, #0
	beq _022221E8
	add r0, r4, #0
	bl ov13_022208F8
_022221E8:
	mov r0, #0
	add sp, #0x20
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_022221F0:
	add r3, sp, #0x10
	mov r0, #0
	ldrsb r0, [r3, r0]
	mov r1, #1
	mov r2, #4
	str r0, [sp]
	mov r0, #0x11
	str r0, [sp, #4]
	add r0, sp, #0x14
	add r0, #2
	str r0, [sp, #8]
	mov r6, #2
	ldrsh r2, [r3, r2]
	ldrsh r3, [r3, r6]
	add r0, r5, #0
	lsl r1, r1, #0xc
	bl ov13_02222474
	add r2, sp, #0x10
	mov r1, #4
	ldrsh r0, [r2, r1]
	ldr r3, [sp, #0xc]
	add r0, #0x18
	strh r0, [r2, #4]
	ldrsh r1, [r2, r1]
	add r0, r5, #0
	mov r2, #0xff
	bl ov13_022224CC
	cmp r4, #0
	beq _02222234
	add r0, r4, #0
	bl ov13_022208F8
_02222234:
	mov r0, #0
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222223C: .word _0224CF98
_02222240: .word 0x000005DC
_02222244: .word ov13_0224DCF8
_02222248: .word ov13_02245A20
	thumb_func_end ov13_02222108


	thumb_func_start ov13_0222224C
ov13_0222224C: ; 0x0222224C
	push {r3, r4, r5, r6, lr}
	sub sp, #0x24
	add r5, r1, #0
	mov r1, #0
	add r0, sp, #0xc
	strb r1, [r0]
	strh r1, [r0, #4]
	strh r1, [r0, #2]
	ldr r0, _0222231C ; =_0224CF98
	add r6, r2, #0
	ldr r4, [r0, #0x14]
	add r0, sp, #0x14
	mov r2, #8
	bl ov13_02222978
	ldr r2, _02222320 ; =0x000005DC
	add r0, r4, #0
	mov r1, #0
	bl ov13_02222978
	mov r1, #2
	add r0, sp, #0xc
	strb r1, [r0, #8]
	mov r1, #0
	strb r1, [r0, #9]
	mov r0, #4
	bl ov13_02222A44
	add r1, sp, #0xc
	strh r0, [r1, #0xa]
	ldr r0, _0222231C ; =_0224CF98
	ldr r0, [r0, #0x2c]
	str r0, [sp, #0x18]
	bl ov13_02222A1C
	str r0, [sp, #0x18]
	mov r1, #8
	add r0, sp, #0xc
	strh r1, [r0, #4]
	add r0, sp, #0xc
	add r0, #2
	str r0, [sp]
	add r0, sp, #0xc
	str r0, [sp, #4]
	ldr r0, _0222231C ; =_0224CF98
	add r1, r4, #0
	ldr r0, [r0]
	add r1, #0x18
	add r2, sp, #0x14
	add r3, sp, #0x10
	bl ov13_02222420
	add r5, #8
	add r0, sp, #0x1c
	add r1, r5, #0
	mov r2, #8
	bl ov13_02222968
	ldr r2, _02222324 ; =ov13_02245A20
	add r0, sp, #0x1c
	mov r1, #8
	mov r3, #6
	bl ov13_022227A0
	cmp r0, #0
	beq _022222DE
	mov r0, #2
	bl ov13_022214AC
	mov r0, #0
	add sp, #0x24
	mvn r0, r0
	pop {r3, r4, r5, r6, pc}
_022222DE:
	add r3, sp, #0xc
	mov r0, #0
	ldrsb r0, [r3, r0]
	mov r1, #2
	mov r2, #4
	str r0, [sp]
	mov r0, #0x11
	str r0, [sp, #4]
	add r0, sp, #0x1c
	str r0, [sp, #8]
	mov r5, #2
	ldrsh r2, [r3, r2]
	ldrsh r3, [r3, r5]
	add r0, r4, #0
	lsl r1, r1, #0xc
	bl ov13_02222474
	add r2, sp, #0xc
	mov r1, #4
	ldrsh r0, [r2, r1]
	add r3, r6, #0
	add r0, #0x18
	strh r0, [r2, #4]
	ldrsh r1, [r2, r1]
	add r0, r4, #0
	mov r2, #0
	bl ov13_022224CC
	mov r0, #0
	add sp, #0x24
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_0222231C: .word _0224CF98
_02222320: .word 0x000005DC
_02222324: .word ov13_02245A20
	thumb_func_end ov13_0222224C


	thumb_func_start ov13_02222328
ov13_02222328: ; 0x02222328
	push {r3, r4, r5, r6, lr}
	sub sp, #0x14
	ldr r0, _02222388 ; =_0224CF98
	add r6, r2, #0
	ldr r4, [r0, #0x14]
	ldr r2, _0222238C ; =0x000005DC
	add r5, r1, #0
	add r0, r4, #0
	mov r1, #0
	bl ov13_02222978
	add r5, #0x10
	add r0, sp, #0xc
	add r1, r5, #0
	mov r2, #8
	bl ov13_02222968
	ldr r0, _02222390 ; =ov13_02245A20
	bl ov13_02222A9C
	add r3, r0, #0
	ldr r2, _02222390 ; =ov13_02245A20
	add r0, sp, #0xc
	mov r1, #8
	bl ov13_022227A0
	mov r2, #0
	mov r1, #3
	str r2, [sp]
	mov r0, #0x11
	str r0, [sp, #4]
	add r0, sp, #0xc
	str r0, [sp, #8]
	add r0, r4, #0
	lsl r1, r1, #0xc
	add r3, r2, #0
	bl ov13_02222474
	add r0, r4, #0
	mov r1, #0x18
	mov r2, #0
	add r3, r6, #0
	bl ov13_022224CC
	mov r0, #0
	add sp, #0x14
	pop {r3, r4, r5, r6, pc}
	nop
_02222388: .word _0224CF98
_0222238C: .word 0x000005DC
_02222390: .word ov13_02245A20
	thumb_func_end ov13_02222328


	thumb_func_start ov13_02222394
ov13_02222394: ; 0x02222394
	push {r3, r4, r5, r6, r7, lr}
	ldr r1, _02222418 ; =ov13_0224CFB8
	add r5, r0, #0
	mov r0, #0x1d
	ldrsb r0, [r1, r0]
	ldr r1, _0222241C ; =_0224CF98
	mov r6, #0
	strb r0, [r5]
	mov r0, #1
	strb r0, [r5, #1]
	ldr r0, [r1, #0x28]
	ldr r1, [r1, #0x24]
	lsl r0, r0, #0x10
	asr r4, r0, #0x10
	add r0, r5, #6
	add r2, r4, #0
	bl ov13_02222968
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	bl ov13_02222A44
	strh r0, [r5, #2]
	add r0, r4, #6
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add r1, r0, #1
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	lsl r0, r0, #0x11
	asr r7, r0, #0x10
	lsl r0, r7, #0x10
	lsr r0, r0, #0x10
	bl ov13_02222A44
	strh r0, [r5, #4]
	add r0, r6, r7
	lsl r0, r0, #0x10
	asr r6, r0, #0x10
	mov r0, #0x60
	strb r0, [r5, r7]
	add r4, r5, r7
	mov r0, #0
	strb r0, [r4, #1]
	bl ov13_02222A44
	strh r0, [r4, #4]
	mov r0, #0xe
	bl ov13_02222A1C
	str r0, [sp]
	add r0, r4, #6
	add r1, sp, #0
	mov r2, #4
	bl ov13_02222968
	mov r0, #4
	bl ov13_02222A44
	add r6, #0xa
	strh r0, [r4, #2]
	lsl r0, r6, #0x10
	asr r0, r0, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02222418: .word ov13_0224CFB8
_0222241C: .word _0224CF98
	thumb_func_end ov13_02222394


	thumb_func_start ov13_02222420
ov13_02222420: ; 0x02222420
	push {r4, r5, lr}
	sub sp, #0xc
	add r5, r1, #0
	add r4, r3, #0
	cmp r0, #1
	bne _02222460
	ldr r0, [sp, #0x18]
	mov r1, #1
	strh r1, [r0]
	add r0, r5, #2
	str r0, [sp]
	ldr r0, _02222470 ; =ov13_0224DCF8
	ldr r3, [sp, #0x1c]
	str r0, [sp, #4]
	mov r0, #8
	str r0, [sp, #8]
	add r0, r2, #0
	mov r2, #0
	ldrsh r2, [r4, r2]
	add r1, r5, #4
	bl ov13_02222534
	ldrh r0, [r4]
	bl ov13_02222A44
	strh r0, [r5]
	mov r0, #0
	ldrsh r0, [r4, r0]
	add sp, #0xc
	add r0, r0, #4
	strh r0, [r4]
	pop {r4, r5, pc}
_02222460:
	add r1, r2, #0
	mov r2, #0
	ldrsh r2, [r4, r2]
	add r0, r5, #0
	bl ov13_02222968
	add sp, #0xc
	pop {r4, r5, pc}
	.balign 4, 0
_02222470: .word ov13_0224DCF8
	thumb_func_end ov13_02222420


	thumb_func_start ov13_02222474
ov13_02222474: ; 0x02222474
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #1
	add r4, r1, #0
	add r6, r2, #0
	add r7, r3, #0
	bl ov13_02222A44
	strh r0, [r5]
	mov r0, #0
	strh r0, [r5, #2]
	strh r0, [r5, #4]
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	bl ov13_02222A44
	strh r0, [r5, #6]
	mov r0, #0
	strh r0, [r5, #8]
	lsl r0, r6, #0x10
	lsr r0, r0, #0x10
	bl ov13_02222A44
	strh r0, [r5, #0xa]
	lsl r0, r7, #0x10
	lsr r0, r0, #0x10
	bl ov13_02222A44
	strh r0, [r5, #0xc]
	add r1, sp, #8
	mov r0, #0x10
	ldrsb r0, [r1, r0]
	mov r2, #8
	strb r0, [r5, #0xe]
	mov r0, #0x14
	ldrsb r0, [r1, r0]
	ldr r1, [sp, #0x20]
	strb r0, [r5, #0xf]
	add r5, #0x10
	add r0, r5, #0
	bl ov13_02222968
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov13_02222474
