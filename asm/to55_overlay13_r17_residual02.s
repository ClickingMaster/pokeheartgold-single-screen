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


	thumb_func_start ov13_02224624
ov13_02224624: ; 0x02224624
	push {r3, r4, r5, r6}
	ldrh r5, [r0]
	mov r3, #0
	asr r4, r5, #8
	lsl r4, r4, #0x18
	lsr r6, r4, #0x18
	lsl r4, r5, #8
	mov r5, #0xff
	lsl r5, r5, #8
	and r4, r5
	orr r4, r6
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	str r4, [r1]
	ldrh r6, [r0, #2]
	asr r1, r6, #8
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	lsl r1, r6, #8
	and r1, r5
	orr r1, r4
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	add r1, r0, #6
	str r4, [r2]
	add r2, r1, r4
	add r4, r0, #0
	cmp r0, r2
	bhs _02224668
_0222465E:
	ldrb r1, [r4]
	add r4, r4, #1
	add r3, r3, r1
	cmp r4, r2
	blo _0222465E
_02224668:
	ldrh r5, [r2]
	lsl r1, r3, #0x10
	lsr r1, r1, #0x10
	asr r2, r5, #8
	lsl r2, r2, #0x18
	lsr r4, r2, #0x18
	mov r2, #0xff
	lsl r3, r5, #8
	lsl r2, r2, #8
	and r2, r3
	orr r2, r4
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	cmp r1, r2
	beq _0222468C
	mov r0, #0
	pop {r3, r4, r5, r6}
	bx lr
_0222468C:
	add r0, r0, #6
	pop {r3, r4, r5, r6}
	bx lr
	.balign 4, 0
	thumb_func_end ov13_02224624


	thumb_func_start ov13_02224694
ov13_02224694: ; 0x02224694
	push {r3, r4, r5, r6}
	add r4, r0, #0
	ldr r0, [r4]
	cmp r0, r1
	blo _022246A4
	mov r0, #0
	pop {r3, r4, r5, r6}
	bx lr
_022246A4:
	ldrh r5, [r0]
	asr r1, r5, #8
	lsl r1, r1, #0x18
	lsr r6, r1, #0x18
	lsl r1, r5, #8
	mov r5, #0xff
	lsl r5, r5, #8
	and r1, r5
	orr r1, r6
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [r2]
	ldrh r6, [r0, #2]
	add r0, r0, #4
	asr r1, r6, #8
	lsl r1, r1, #0x18
	lsr r2, r1, #0x18
	lsl r1, r6, #8
	and r1, r5
	orr r1, r2
	lsl r1, r1, #0x10
	lsr r2, r1, #0x10
	str r2, [r3]
	add r2, #0xb
	mov r1, #7
	bic r2, r1
	sub r1, r2, #4
	add r1, r0, r1
	str r1, [r4]
	pop {r3, r4, r5, r6}
	bx lr
	.balign 4, 0
	thumb_func_end ov13_02224694


	thumb_func_start ov13_022246E4
ov13_022246E4: ; 0x022246E4
	push {r3, r4, r5, r6, lr}
	sub sp, #4
	add r5, r0, #0
	add r5, #8
	str r5, [sp]
	add r3, r2, #0
	ldrh r2, [r0]
	add r4, r1, #0
	add r0, sp, #0
	asr r1, r2, #8
	lsl r6, r2, #8
	mov r2, #0xff
	lsl r1, r1, #0x18
	lsl r2, r2, #8
	lsr r1, r1, #0x18
	and r2, r6
	orr r1, r2
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r1, r5, r1
	add r2, r4, #0
	bl ov13_02224694
	add sp, #4
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov13_022246E4


	thumb_func_start ov13_02224718
ov13_02224718: ; 0x02224718
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r7, r3, #0
	add r6, r0, #0
	add r0, r7, #0
	sub r0, #8
	str r1, [sp, #4]
	lsl r0, r0, #0x10
	add r1, r2, #0
	mov r5, #0
	strb r5, [r1]
	lsr r0, r0, #0x10
	strb r5, [r1, #1]
	asr r2, r0, #8
	strb r5, [r1, #2]
	lsl r2, r2, #0x18
	strb r5, [r1, #3]
	lsr r3, r2, #0x18
	lsl r2, r0, #8
	mov r0, #0xff
	strb r5, [r1, #4]
	lsl r0, r0, #8
	strb r5, [r1, #5]
	and r0, r2
	strb r5, [r1, #6]
	orr r0, r3
	strb r5, [r1, #7]
	ldr r3, [sp, #0x20]
	add r4, r6, #0
	strh r0, [r1]
	cmp r3, #0
	beq _02224768
	mov r0, #0x10
	str r0, [sp]
	add r0, r6, #6
	add r2, r7, #0
	bl ov13_02225358
	add r7, #8
	b _02224770
_02224768:
	add r0, r6, #6
	add r2, r7, #0
	bl memcpy
_02224770:
	mov r0, #0
	strb r0, [r6]
	strb r0, [r6, #1]
	strb r0, [r6, #2]
	strb r0, [r6, #3]
	strb r0, [r6, #4]
	strb r0, [r6, #5]
	ldr r0, [sp, #4]
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	asr r0, r1, #8
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r1, r0
	orr r1, r2
	strh r1, [r6]
	lsl r1, r7, #0x10
	lsr r2, r1, #0x10
	asr r1, r2, #8
	lsl r1, r1, #0x18
	lsl r2, r2, #8
	lsr r1, r1, #0x18
	and r0, r2
	orr r0, r1
	strh r0, [r6, #2]
	add r0, r4, #6
	add r4, r0, r7
	add r1, r6, #0
	cmp r6, r4
	bhs _022247BC
_022247B2:
	ldrb r0, [r1]
	add r1, r1, #1
	add r5, r5, r0
	cmp r1, r4
	blo _022247B2
_022247BC:
	lsl r0, r5, #0x10
	lsr r1, r0, #0x10
	asr r0, r1, #8
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r0, r1
	orr r0, r2
	strh r0, [r4]
	add r0, r4, #2
	sub r0, r0, r6
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov13_02224718


	thumb_func_start ov13_022247DC
ov13_022247DC: ; 0x022247DC
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	lsl r0, r1, #0x10
	add r4, r3, #0
	mov r3, #0
	lsr r0, r0, #0x10
	add r7, r2, #0
	strb r3, [r5]
	asr r1, r0, #8
	lsl r2, r0, #8
	mov r0, #0xff
	strb r3, [r5, #1]
	lsl r1, r1, #0x18
	lsl r0, r0, #8
	strb r3, [r5, #2]
	lsr r1, r1, #0x18
	and r2, r0
	orr r1, r2
	strb r3, [r5, #3]
	add r2, r4, #0
	strh r1, [r5]
	add r2, #0xb
	mov r1, #7
	bic r2, r1
	lsl r1, r4, #0x10
	sub r6, r2, #4
	lsr r2, r1, #0x10
	asr r1, r2, #8
	lsl r1, r1, #0x18
	lsl r2, r2, #8
	and r0, r2
	lsr r1, r1, #0x18
	orr r0, r1
	strh r0, [r5, #2]
	add r5, r5, #4
	add r0, r5, #0
	add r1, r3, #0
	add r2, r6, #0
	bl memset
	add r0, r5, #0
	add r1, r7, #0
	add r2, r4, #0
	bl memcpy
	add r0, r5, r6
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov13_022247DC


	thumb_func_start ov13_0222483C
ov13_0222483C: ; 0x0222483C
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0
	strb r0, [r4]
	strb r0, [r4, #1]
	strb r0, [r4, #2]
	strb r0, [r4, #3]
	strb r0, [r4, #4]
	strb r0, [r4, #5]
	strb r0, [r4, #6]
	strb r0, [r4, #7]
	add r0, r4, #0
	add r0, #8
	bl ov13_022247DC
	sub r0, r0, r4
	add r1, r0, #0
	sub r1, #8
	strh r1, [r4]
	pop {r4, pc}
	thumb_func_end ov13_0222483C


	thumb_func_start ov13_02224864
ov13_02224864: ; 0x02224864
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	str r1, [sp]
	add r1, sp, #0x18
	add r2, sp, #0x14
	bl ov13_02224624
	mov r6, #0
	add r5, r0, #0
	add r7, r6, #0
	str r5, [sp, #8]
	str r6, [sp, #4]
	cmp r5, #0
	bne _02224886
	add sp, #0x1c
	add r0, r6, #0
	pop {r4, r5, r6, r7, pc}
_02224886:
	ldr r0, [sp, #0x18]
	cmp r0, #1
	beq _02224892
	add sp, #0x1c
	add r0, r6, #0
	pop {r4, r5, r6, r7, pc}
_02224892:
	ldr r4, [sp, #0x14]
	add r0, r5, #0
	add r0, #8
	str r0, [sp, #8]
	add r0, sp, #8
	add r1, r5, r4
	add r2, sp, #0x10
	add r3, sp, #0xc
	bl ov13_02224694
	cmp r0, #0
	beq _02224912
_022248AA:
	ldr r1, [sp, #0x10]
	cmp r1, #1
	beq _022248BA
	cmp r1, #2
	beq _022248D2
	cmp r1, #5
	beq _022248EC
	b _02224902
_022248BA:
	ldrh r1, [r0]
	asr r0, r1, #8
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r0, r1
	orr r0, r2
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	b _02224902
_022248D2:
	ldrh r1, [r0]
	asr r0, r1, #8
	lsl r2, r1, #8
	mov r1, #0xff
	lsl r0, r0, #0x18
	lsl r1, r1, #8
	lsr r0, r0, #0x18
	and r1, r2
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #4]
	b _02224902
_022248EC:
	ldrh r1, [r0]
	asr r0, r1, #8
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r0, r1
	orr r0, r2
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
_02224902:
	add r0, sp, #8
	add r1, r5, r4
	add r2, sp, #0x10
	add r3, sp, #0xc
	bl ov13_02224694
	cmp r0, #0
	bne _022248AA
_02224912:
	cmp r6, #1
	bne _0222491C
	ldr r0, [sp, #4]
	cmp r0, #1
	beq _02224922
_0222491C:
	add sp, #0x1c
	mov r0, #0
	pop {r4, r5, r6, r7, pc}
_02224922:
	cmp r7, #1
	blt _0222492C
	mov r1, #1
	ldr r0, [sp]
	b _02224930
_0222492C:
	ldr r0, [sp]
	mov r1, #0
_02224930:
	str r1, [r0]
	mov r0, #1
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov13_02224864


	thumb_func_start ov13_02224938
ov13_02224938: ; 0x02224938
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r5, r1, #0
	add r6, r2, #0
	add r1, sp, #8
	add r2, sp, #4
	add r4, r3, #0
	bl ov13_02224624
	add r1, r0, #0
	bne _02224954
	add sp, #0xc
	mov r0, #0
	pop {r3, r4, r5, r6, pc}
_02224954:
	ldr r0, [sp, #8]
	cmp r0, r5
	beq _02224960
	add sp, #0xc
	mov r0, #0
	pop {r3, r4, r5, r6, pc}
_02224960:
	cmp r4, #0
	beq _0222497A
	mov r0, #0x10
	str r0, [sp]
	ldr r2, [sp, #4]
	add r0, r6, #0
	add r3, r4, #0
	bl ov13_02225510
	ldr r0, [sp, #4]
	sub r0, #8
	str r0, [sp, #4]
	b _02224982
_0222497A:
	ldr r2, [sp, #4]
	add r0, r6, #0
	bl memcpy
_02224982:
	ldr r0, [sp, #4]
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	thumb_func_end ov13_02224938


	thumb_func_start ov13_02224988
ov13_02224988: ; 0x02224988
	push {r4, lr}
	sub sp, #0x10
	mov r1, #1
	add r4, r0, #0
	add r2, sp, #4
	lsl r1, r1, #8
	add r0, sp, #4
	strh r1, [r0]
	ldr r3, _02224A10 ; =ov13_02242680
	add r2, #2
	mov r1, #7
_0222499E:
	ldrb r0, [r3]
	add r3, r3, #1
	strb r0, [r2]
	add r2, r2, #1
	sub r1, r1, #1
	bne _0222499E
	ldr r0, _02224A14 ; =ov13_02245A58
	mov r1, #1
	ldr r0, [r0]
	add r2, sp, #4
	mov r3, #2
	bl ov13_022247DC
	mov r1, #2
	add r2, sp, #4
	add r3, r1, #0
	bl ov13_022247DC
	ldr r1, _02224A18 ; =ov13_0224DF30
	ldr r1, [r1, #0x50]
	cmp r1, #0
	beq _022249D4
	mov r1, #5
	add r2, sp, #4
	mov r3, #2
	bl ov13_022247DC
_022249D4:
	add r2, sp, #4
	mov r1, #3
	add r2, #2
	mov r3, #7
	bl ov13_022247DC
	ldr r1, _02224A18 ; =ov13_0224DF30
	add r3, r0, #0
	ldr r1, [r1, #0x50]
	cmp r1, #0
	beq _022249F6
	ldr r2, _02224A1C ; =ov13_0224E3D8
	mov r1, #4
	mov r3, #6
	bl ov13_022247DC
	add r3, r0, #0
_022249F6:
	mov r0, #0
	str r0, [sp]
	add r0, r4, #0
	ldr r4, _02224A14 ; =ov13_02245A58
	ldr r2, _02224A20 ; =ov13_0224E464
	ldr r4, [r4]
	mov r1, #2
	sub r3, r3, r4
	add r3, #8
	bl ov13_02224718
	add sp, #0x10
	pop {r4, pc}
	.balign 4, 0
_02224A10: .word ov13_02242680
_02224A14: .word ov13_02245A58
_02224A18: .word ov13_0224DF30
_02224A1C: .word ov13_0224E3D8
_02224A20: .word ov13_0224E464
	thumb_func_end ov13_02224988
